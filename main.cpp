#include <Windows.h>
#include <winsock2.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "resource.h"



#include <shlobj.h>

#include "metastock.h"
#include "yahoo.h"

const unsigned int BUILDVERSION = 20241109;
DWORD WINAPI ThreadProcUpdateCheck(_In_ LPVOID hwndDlg) {
  // guncel mi
  CoInitialize(NULL);
  if (S_OK == URLDownloadToFile(NULL,
                                "https://raw.githubusercontent.com/mumin16/"
                                "veriaktar/master/version.json",
                                "version.json", 0, NULL)) {
    std::ifstream f("version.json");

    json version_json;
    try {
      version_json = json::parse(f);
    } catch (json::parse_error &ex) {
    }
    f.close();

    if (!version_json["version"].is_null()) {
      if (version_json["version"].get<unsigned int>() > BUILDVERSION) {
        MessageBox((HWND)hwndDlg,
                   version_json["note"].get<std::string>().c_str(),
                   "Update available!", MB_OK);
        SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
                           (LPARAM) "Update available!");
      }
    }

  } else {
    SendDlgItemMessage(
        (HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
        (LPARAM) "Update Check Failed! Please check the latest version.");
  }

  CoUninitialize();

  std::remove("version.json");  // delete file
  return 0;
}
static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam,
                                       LPARAM lpData) {
  /*
      if(uMsg == BFFM_INITIALIZED)
      {
          std::string tmp = (const char *) lpData;
          std::cout << "path: " << tmp << std::endl;
          SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
      }
  */
  return 0;
}

std::string BrowseFolder(const std::string &saved_path) {
  TCHAR path[MAX_PATH];

  const char *path_param = saved_path.c_str();

  BROWSEINFO bi = {nullptr};
  bi.lpszTitle = ("Select Output folder");
  bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
  bi.lpfn = BrowseCallbackProc;
  bi.lParam = (LPARAM)path_param;

  LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

  if (pidl != nullptr) {
    // get the name of the folder and put it in path
    SHGetPathFromIDList(pidl, path);

    // free memory used
    IMalloc *imalloc = nullptr;
    if (SUCCEEDED(SHGetMalloc(&imalloc))) {
      imalloc->Free(pidl);
      imalloc->Release();
    }

    return path;
  }

  return "";
}

#include <BS_thread_pool.hpp>
static bool completedDownload{false};
static BS::thread_pool *pool{nullptr};
static std::vector<std::vector<FXI>> fxiVecs;
static std::vector<std::vector<FX>> fxVecs;
static std::vector<std::string> symbols;
void veriaktar(HWND hwndDlg) {
  ModifyMenu(GetMenu((HWND)hwndDlg), IDR_VERIAKTAR, MF_STRING, IDR_VERIAKTAR,
             "<<Wait>>");

  EnableMenuItem(GetMenu((HWND)hwndDlg), IDR_VERIAKTAR,
                 MF_DISABLED | MF_GRAYED);

  EnableMenuItem(GetMenu((HWND)hwndDlg), IDR_PORTFOYYUKLE,
                 MF_DISABLED | MF_GRAYED);

  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_SYMBOLADD),
               false);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_SYMBOLREMOVE),
               false);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_SYMBOLSCLEAR),
               false);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_PERIOD),
               false);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_LENGTH),
               false);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_MSDIRSELECT),
               false);  // to disable button or other control

  pool = new BS::thread_pool();

  for (size_t i = 0; i < default_json["symbols"].size(); i++)
    pool->detach_task([=]() {
      std::vector<FXI> fxiVec;
      std::vector<FX> fxVec;

      YahooDownload(default_json["symbols"][i], fxVec, fxiVec,
                    default_json["period"], default_json["timeframe"],
                    GetDlgItemInt((HWND)hwndDlg, IDC_LENGTH, nullptr, 0));

      if (fxVec.empty() && fxiVec.empty()) {
        SendDlgItemMessage(
            (HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
            (LPARAM)std::string{default_json["symbols"][i].get<std::string>() +
                                " Download Unsuccessful!"}
                .c_str());

        SendDlgItemMessage(
            (HWND)hwndDlg, IDC_BILGI, LB_SETCURSEL,
            SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_GETCOUNT, 0, 0) - 1,
            0);
      }

      else {
        symbols.push_back(default_json["symbols"][i].get<std::string>());

        if (fxVec.empty()) {
          fxiVecs.push_back(fxiVec);
        } else {
          fxVecs.push_back(fxVec);
        }

        /*
        if (!WriteSecwithData(
                default_json["msdir"].get<std::string>(),
                (char *)default_json["symbols"][i].get<std::string>().c_str(),
                fxVec, fxiVec, default_json["period"],
                (default_json["period"] == 'I'
                     ? default_json["timeframe"].get<int>()
                     : 0))) {
          SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
                             (LPARAM)std::string{
                                 default_json["symbols"][i].get<std::string>() +
                                 " Metastock File Not Created!"}
                                 .c_str());
        }
        */
      }

      char msg[256];
      sprintf(msg, "Remaining:%zu", pool->get_tasks_total() - 1);
      SetWindowText((HWND)hwndDlg, msg);
      
    });



  pool->detach_task([&]() {
    for (;;) {
      if (pool->get_tasks_total() == 1) {

 
        completedDownload = true;
        break;
      }
    }
  });
}
/*
bool continueThread = false;
using namespace std::chrono;
DWORD WINAPI ThreadProcVeriaktar(_In_ LPVOID hwndDlg) {
  ModifyMenu(GetMenu((HWND)hwndDlg), IDR_VERIAKTAR, MF_STRING, IDR_VERIAKTAR,
             "<<Stop>>");

  EnableMenuItem(GetMenu((HWND)hwndDlg), IDR_PORTFOYYUKLE,
                 MF_DISABLED | MF_GRAYED);

  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_SYMBOLADD),
               false);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_SYMBOLREMOVE),
               false);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_SYMBOLSCLEAR),
               false);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_PERIOD),
               false);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_LENGTH),
               false);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_MSDIRSELECT),
               false);  // to disable button or other control

  std::vector<microseconds> avarage;
  for (size_t i = 0; i < default_json["symbols"].size(); i++) {
    if (!continueThread) break;

    std::vector<FXI> fxiVec;
    std::vector<FX> fxVec;

    // Recording the timestamp at the start of the code
    auto beg = high_resolution_clock::now();

    if (YahooDownload(default_json["symbols"][i], fxVec, fxiVec,
                      default_json["period"], default_json["timeframe"],
                      GetDlgItemInt((HWND)hwndDlg, IDC_LENGTH, nullptr, 0))) {
      SendDlgItemMessage(
          (HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
          (LPARAM)std::string{default_json["symbols"][i].get<std::string>() +
                              " download success!"}
              .c_str());

      SendDlgItemMessage(
          (HWND)hwndDlg, IDC_BILGI, LB_SETCURSEL,
          SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_GETCOUNT, 0, 0) - 1,
          0);

      if (!WriteSecwithData(
              default_json["msdir"].get<std::string>(),
              (char *)default_json["symbols"][i].get<std::string>().c_str(),
              fxVec, fxiVec, default_json["period"],
              (default_json["period"] == 'I'
                   ? default_json["timeframe"].get<int>()
                   : 0)))  // 60 dakika
        SendDlgItemMessage(
            (HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
            (LPARAM)std::string{default_json["symbols"][i].get<std::string>() +
                                " metastock file not created!"}
                .c_str());

    } else {
      SendDlgItemMessage(
          (HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
          (LPARAM)std::string{default_json["symbols"][i].get<std::string>() +
                              " download failed!"}
              .c_str());
    }

    auto end = high_resolution_clock::now();

    avarage.push_back(duration_cast<microseconds>(end - beg));
    auto const count = static_cast<float>(avarage.size());

    auto duration = duration_cast<microseconds>(
                        std::reduce(avarage.begin(), avarage.end()) / count) *
                    (default_json["symbols"].size() - i);

    using namespace std::chrono;
    auto secs = duration_cast<seconds>(duration);
    duration_cast<milliseconds>(secs);
    auto mins = duration_cast<minutes>(secs);
    secs -= duration_cast<seconds>(mins);
    auto hour = duration_cast<hours>(mins);
    mins -= duration_cast<minutes>(hour);

    std::string ss;
    ss = "Remaining Time: " + std::to_string(hour.count()) + ":" +
         std::to_string(mins.count()) + ":" + std::to_string(secs.count());
    SetWindowText((HWND)hwndDlg, ss.c_str());
  }

  SetWindowText((HWND)hwndDlg, "VeriAktar");

  ModifyMenu(GetMenu((HWND)hwndDlg), IDR_VERIAKTAR, MF_STRING, IDR_VERIAKTAR,
             "<<Start>>");

  EnableMenuItem(GetMenu((HWND)hwndDlg), IDR_PORTFOYYUKLE, MF_ENABLED);

  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_SYMBOLADD),
               true);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_SYMBOLREMOVE),
               true);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_SYMBOLSCLEAR),
               true);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_PERIOD),
               true);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_LENGTH),
               true);  // to disable button or other control
  EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_MSDIRSELECT),
               true);  // to disable button or other control

  SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
                     (LPARAM) "Completed!");

  SendDlgItemMessage(
      (HWND)hwndDlg, IDC_BILGI, LB_SETCURSEL,
      SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_GETCOUNT, 0, 0) - 1, 0);

  // MessageBox((HWND)hwndDlg, "Completed!", "Info", MB_OK);

  continueThread = false;

  if (SendDlgItemMessage((HWND)hwndDlg, IDC_LOOP, BM_GETCHECK, 0, 0)) {
    continueThread = true;

    SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
                       (LPARAM) "Loop: Restart Download!");

    SendDlgItemMessage(
        (HWND)hwndDlg, IDC_BILGI, LB_SETCURSEL,
        SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_GETCOUNT, 0, 0) - 1, 0);

    ThreadProcVeriaktar(hwndDlg);
  }

  return 0;
}
*/
void PortfoyYukle(_In_ HWND hwndDlg) {
  OPENFILENAME ofn = {0};
  char szSaveFileName[MAX_PATH] = {0};
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwndDlg;
  CHAR currentDirectory[1024];
  GetCurrentDirectory(1024, currentDirectory);
  ofn.lpstrInitialDir = currentDirectory;
  ofn.lpstrFilter = "Symbols List (*.json)\0*.json";
  ofn.lpstrFile = szSaveFileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags =
      OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
  ofn.lpstrDefExt = "json";
  if (0 == GetOpenFileName(&ofn)) return;

  std::ifstream f(ofn.lpstrFile);
  default_json.clear();
  default_json = json::parse(f);
  // default_json.update(upd);

  SendDlgItemMessage(hwndDlg, IDC_SYMBOLS, LB_RESETCONTENT, (WPARAM)0,
                     (LPARAM)0);

  for (const auto &i : default_json["symbols"])
    SendDlgItemMessage(hwndDlg, IDC_SYMBOLS, LB_ADDSTRING, 0,
                       (LPARAM)i.get<std::string>().c_str());

  SetDlgItemText(hwndDlg, IDC_OUTPUTDIR,
                 default_json["msdir"].get<std::string>().c_str());

  SetDlgItemText(hwndDlg, IDC_TARGETDIR,
                 default_json["targetdir"].get<std::string>().c_str());

  if (!std::filesystem::exists(default_json["msdir"].get<std::string>()))
    std::filesystem::create_directory(default_json["msdir"]);

  SetDlgItemText(hwndDlg, IDC_DIVISOR,
                 default_json["div"].get<std::string>().c_str());

  int sel = default_json["period"] == 'D'                   ? 0
            : default_json["period"] == 'M'                 ? 1
            : default_json["period"] == 'W'                 ? 2
            : default_json["timeframe"] == PERIOD::MINUTE5  ? 3
            : default_json["timeframe"] == PERIOD::MINUTE15 ? 4
            : default_json["timeframe"] == PERIOD::MINUTE30 ? 5
                                                            : 0;

  SendDlgItemMessage(hwndDlg, IDC_XMINUTE, CB_SETCURSEL, sel, 0);

  SetDlgItemInt(hwndDlg, IDC_LENGTH, default_json["len"].get<int>(), false);
}

void PortfoyKaydet(_In_ HWND hwndDlg) {
  int length = SendDlgItemMessage(hwndDlg, IDC_SYMBOLS, LB_GETCOUNT, 0, 0);

  if (0 == length) {
    MessageBox(hwndDlg, "There is no symbols!", nullptr, MB_OK);
    return;
  }

  OPENFILENAME ofn = {0};
  char szSaveFileName[MAX_PATH] = {0};
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwndDlg;
  CHAR currentDirectory[1024];
  GetCurrentDirectory(1024, currentDirectory);
  ofn.lpstrInitialDir = currentDirectory;
  ofn.lpstrFilter = "Symbols List (*.json)\0*.json";
  ofn.lpstrFile = szSaveFileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags =
      OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
  ofn.lpstrDefExt = "json";
  if (0 == GetSaveFileName(&ofn)) return;

  std::ofstream fout(ofn.lpstrFile);
  fout << default_json.dump();
}

INT_PTR CALLBACK DialogProc(_In_ HWND hwndDlg, _In_ UINT uMsg,
                            _In_ WPARAM wParam, _In_ LPARAM lParam) {
  switch (uMsg) {
    case WM_INITDIALOG: {
      SetDlgItemText(hwndDlg, IDC_DIVISOR,
                     default_json["div"].get<std::string>().c_str());
      SetDlgItemText(hwndDlg, IDC_TARGETDIR,
                     default_json["targetdir"].get<std::string>().c_str());

      for (const auto &i : default_json["symbols"])
        SendDlgItemMessage(hwndDlg, IDC_SYMBOLS, LB_ADDSTRING, 0,
                           (LPARAM)i.get<std::string>().c_str());

      SetDlgItemText(hwndDlg, IDC_OUTPUTDIR,
                     default_json["msdir"].get<std::string>().c_str());

      SendDlgItemMessage(hwndDlg, IDC_PERIOD, CB_ADDSTRING, 0,
                         (LPARAM) "Daily");
      SendDlgItemMessage(hwndDlg, IDC_PERIOD, CB_ADDSTRING, 0,
                         (LPARAM) "Monthly");
      SendDlgItemMessage(hwndDlg, IDC_PERIOD, CB_ADDSTRING, 0,
                         (LPARAM) "Weekly");
      SendDlgItemMessage(hwndDlg, IDC_PERIOD, CB_ADDSTRING, 0,
                         (LPARAM) "5Minute");
      SendDlgItemMessage(hwndDlg, IDC_PERIOD, CB_ADDSTRING, 0,
                         (LPARAM) "15Minute");
      SendDlgItemMessage(hwndDlg, IDC_PERIOD, CB_ADDSTRING, 0,
                         (LPARAM) "30Minute");

      SendDlgItemMessage(hwndDlg, IDC_PERIOD, CB_SETCURSEL,
                         default_json["period"] == 'D'     ? 0
                         : default_json["period"] == 'M'   ? 1
                         : default_json["period"] == 'W'   ? 2
                         : default_json["timeframa"] == 5  ? 3
                         : default_json["timeframa"] == 15 ? 4
                                                           : 5,
                         0);

      SetDlgItemInt(hwndDlg, IDC_LENGTH, default_json["len"].get<int>(), false);

      CloseHandle(CreateThread(nullptr, NULL, ThreadProcUpdateCheck, hwndDlg, 0,
                               nullptr));

      break;
    }
    case WM_COMMAND: {
      if (LOWORD(wParam) == IDR_ABOUT)
        MessageBox(
            hwndDlg,
            "Mumin "
            "GULER\nmumin16@hotmail.com\nhttps://github.com/mumin16/veriaktar/",
            "About", MB_OK);
      else if (LOWORD(wParam) == IDC_SYMBOLSCLEAR) {
        default_json["symbols"] = {};
        SendDlgItemMessage(hwndDlg, IDC_SYMBOLS, LB_RESETCONTENT, 0, 0);
      }

      else if (LOWORD(wParam) == IDC_SYMBOLREMOVE) {
        int cursel =
            SendDlgItemMessage(hwndDlg, IDC_SYMBOLS, LB_GETCURSEL, 0, 0);
        if (cursel == LB_ERR) break;
        SendDlgItemMessage(hwndDlg, IDC_SYMBOLS, LB_DELETESTRING, cursel, 0);

        default_json["symbols"].erase(cursel);
      } else if (HIWORD(wParam) == EN_UPDATE && LOWORD(wParam) == IDC_DIVISOR) {
        char buf[16];
        if (GetDlgItemText(hwndDlg, IDC_DIVISOR, (LPSTR)&buf, 16))
          default_json["div"] = buf;

      } else if (HIWORD(wParam) == EN_UPDATE && LOWORD(wParam) == IDC_LENGTH) {
        default_json["len"] =
            GetDlgItemInt((HWND)hwndDlg, IDC_LENGTH, nullptr, 0);

      }

      else if (LOWORD(wParam) == IDC_ACTION) {
        EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_TARGETDIRSELECT),
                     false);  // to disable button or other control
        EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_ACTION),
                     false);  // to disable button or other control
        EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_EXTENSION),
                     false);  // to disable button or other control

        MultiSymbolAction(hwndDlg);

        EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_TARGETDIRSELECT),
                     true);  // to disable button or other control
        EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_ACTION),
                     true);  // to disable button or other control
        EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_EXTENSION),
                     true);  // to disable button or other control
        MessageBox((HWND)hwndDlg, "Multi-Symbol Generator Completed!", "Info",
                   MB_OK);

      }

      else if (LOWORD(wParam) == IDC_SYMBOLADD) {
        char txt[64];
        GetDlgItemText(hwndDlg, IDC_SYMBOL, (LPSTR)&txt, 64);

        if (strlen(txt) == 0) break;

        SetDlgItemText(hwndDlg, IDC_SYMBOL, "");
        SendDlgItemMessage(hwndDlg, IDC_SYMBOLS, LB_ADDSTRING, 0, (LPARAM)txt);
        default_json["symbols"] += txt;

      } else if (LOWORD(wParam) == IDR_VERIAKTAR) {
        /*
        if (!continueThread) {
          continueThread = true;
          SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_RESETCONTENT, 0, 0);

           CloseHandle(
             CreateThread(nullptr, NULL, ThreadProcVeriaktar, hwndDlg, 0,
             nullptr));

        } else {
          continueThread = false;
        }
        */
                 SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
                             (LPARAM)
                                 "Started, Please Wait..."
                                 );

                                       SendDlgItemMessage(
          (HWND)hwndDlg, IDC_BILGI, LB_SETCURSEL,
          SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_GETCOUNT, 0, 0) - 1,
          0);

        veriaktar((HWND)hwndDlg);
      }

      else if (LOWORD(wParam) == IDR_PORTFOYYUKLE) {
        PortfoyYukle(hwndDlg);
      } else if (LOWORD(wParam) == IDR_PORTFOYKAYDET) {
        PortfoyKaydet(hwndDlg);
      } else if (LOWORD(wParam) == IDC_MSDIRSELECT) {
        std::string str = BrowseFolder(default_json["msdir"]);
        if (!str.empty()) {
          default_json["msdir"] = str + "\\";
          SetDlgItemText(hwndDlg, IDC_OUTPUTDIR,
                         default_json["msdir"].get<std::string>().c_str());
        }
      } else if (LOWORD(wParam) == IDC_TARGETDIRSELECT) {
        std::string str = BrowseFolder(default_json["msdir"]);
        if (!str.empty()) {
          str += "\\";
          default_json["targetdir"] = str;
          SetDlgItemText(hwndDlg, IDC_TARGETDIR, str.c_str());
        }
      }

      else if (HIWORD(wParam) == CBN_SELCHANGE &&
               LOWORD(wParam) == IDC_PERIOD) {
        auto sel = SendDlgItemMessage(hwndDlg, IDC_XMINUTE, CB_GETCURSEL, 0, 0);
        default_json["period"] = sel == 0   ? 'D'
                                 : sel == 1 ? 'M'
                                 : sel == 2 ? 'W'
                                            : 'I';

        default_json["timeframe"] = sel == 3   ? PERIOD::MINUTE5
                                    : sel == 4 ? PERIOD::MINUTE15
                                    : sel == 5 ? PERIOD::MINUTE30
                                               : 0;
      }

      break;
    }
    case WM_CLOSE: {
      EndDialog(hwndDlg, 0);
      break;
    }
    default: {
      if (completedDownload) {
        delete pool;
        pool = nullptr;
        completedDownload = false;
        SetWindowText(hwndDlg, "VeriAktar");

        SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
                           (LPARAM) "Writing Metastock Files...");

        SendDlgItemMessage(
            (HWND)hwndDlg, IDC_BILGI, LB_SETCURSEL,
            SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_GETCOUNT, 0, 0) - 1,
            0);

               for (size_t i = 0; i < symbols.size(); i++) {
          if(!WriteSecwithData(
                  default_json["msdir"].get<std::string>(),
                  (char *)symbols[i].c_str(),
                  fxVecs.empty() ? std::vector<FX>{} : fxVecs[i],
                  fxiVecs.empty() ? std::vector<FXI>{} : fxiVecs[i],
                  default_json["period"],
                  (default_json["period"] == 'I'
                       ? default_json["timeframe"].get<int>()
                       : 0))){
                SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
                           (LPARAM) "Metastock file write error!");

        SendDlgItemMessage(
            (HWND)hwndDlg, IDC_BILGI, LB_SETCURSEL,
            SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_GETCOUNT, 0, 0) - 1,
            0);

                       }
        }
        symbols.clear();
        fxiVecs.clear();
        fxVecs.clear();
        

        ModifyMenu(GetMenu((HWND)hwndDlg), IDR_VERIAKTAR, MF_STRING,
                   IDR_VERIAKTAR, "<<Start>>");

        EnableMenuItem(GetMenu((HWND)hwndDlg), IDR_VERIAKTAR, MF_ENABLED);

        EnableMenuItem(GetMenu((HWND)hwndDlg), IDR_PORTFOYYUKLE, MF_ENABLED);

        EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_SYMBOLADD),
                     true);  // to disable button or other control
        EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_SYMBOLREMOVE),
                     true);  // to disable button or other control
        EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_SYMBOLSCLEAR),
                     true);  // to disable button or other control
        EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_PERIOD),
                     true);  // to disable button or other control
        EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_LENGTH),
                     true);  // to disable button or other control
        EnableWindow(GetDlgItem((HWND)hwndDlg, IDC_MSDIRSELECT),
                     true);  // to disable button or other control

        SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
                           (LPARAM) "Completed!");

        SendDlgItemMessage(
            (HWND)hwndDlg, IDC_BILGI, LB_SETCURSEL,
            SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_GETCOUNT, 0, 0) - 1,
            0);

        if (SendDlgItemMessage((HWND)hwndDlg, IDC_LOOP, BM_GETCHECK, 0, 0)) {
          SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_ADDSTRING, 0,
                             (LPARAM) "Loop: Restart Download!");

          SendDlgItemMessage(
              (HWND)hwndDlg, IDC_BILGI, LB_SETCURSEL,
              SendDlgItemMessage((HWND)hwndDlg, IDC_BILGI, LB_GETCOUNT, 0, 0) -
                  1,
              0);

          veriaktar(hwndDlg);
        }
      }
      break;
    }
  }

  return false;
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
  default_json["symbols"] = {
      "A1CAP.IS", "ACSEL.IS", "ADEL.IS",  "ADESE.IS", "ADGYO.IS", "AEFES.IS",
      "AFYON.IS", "AGESA.IS", "AGHOL.IS", "AGROT.IS", "AGYO.IS",  "AHGAZ.IS",
      "AHSGY.IS", "AKBNK.IS", "AKCNS.IS", "AKENR.IS", "AKFGY.IS", "AKFYE.IS",
      "AKGRT.IS", "AKMGY.IS", "AKSA.IS",  "AKSEN.IS", "AKSGY.IS", "AKSUE.IS",
      "AKYHO.IS", "ALARK.IS", "ALBRK.IS", "ALCAR.IS", "ALCTL.IS", "ALFAS.IS",
      "ALGYO.IS", "ALKA.IS",  "ALKIM.IS", "ALKLC.IS", "ALMAD.IS", "ALTNY.IS",
      "ALVES.IS", "ANELE.IS", "ANGEN.IS", "ANHYT.IS", "ANSGR.IS", "ARASE.IS",
      "ARCLK.IS", "ARDYZ.IS", "ARENA.IS", "ARSAN.IS", "ARTMS.IS", "ARZUM.IS",
      "ASELS.IS", "ASGYO.IS", "ASTOR.IS", "ASUZU.IS", "ATAGY.IS", "ATAKP.IS",
      "ATATP.IS", "ATEKS.IS", "ATLAS.IS", "ATSYH.IS", "AVGYO.IS", "AVHOL.IS",
      "AVOD.IS",  "AVPGY.IS", "AVTUR.IS", "AYCES.IS", "AYDEM.IS", "AYEN.IS",
      "AYES.IS",  "AYGAZ.IS", "AZTEK.IS", "BAGFS.IS", "BAHKM.IS", "BAKAB.IS",
      "BALAT.IS", "BANVT.IS", "BARMA.IS", "BASCM.IS", "BASGZ.IS", "BAYRK.IS",
      "BEGYO.IS", "BERA.IS",  "BEYAZ.IS", "BFREN.IS", "BIENY.IS", "BIGCH.IS",
      "BIMAS.IS", "BINBN.IS", "BINHO.IS", "BIOEN.IS", "BIZIM.IS", "BJKAS.IS",
      "BLCYT.IS", "BMSCH.IS", "BMSTL.IS", "BNTAS.IS", "BOBET.IS", "BORLS.IS",
      "BORSK.IS", "BOSSA.IS", "BRISA.IS", "BRKO.IS",  "BRKSN.IS", "BRKVY.IS",
      "BRLSM.IS", "BRMEN.IS", "BRSAN.IS", "BRYAT.IS", "BSOKE.IS", "BTCIM.IS",
      "BUCIM.IS", "BURCE.IS", "BURVA.IS", "BVSAN.IS", "BYDNR.IS", "CANTE.IS",
      "CASA.IS",  "CATES.IS", "CCOLA.IS", "CELHA.IS", "CEMAS.IS", "CEMTS.IS",
      "CEMZY.IS", "CEOEM.IS", "CIMSA.IS", "CLEBI.IS", "CMBTN.IS", "CMENT.IS",
      "CONSE.IS", "COSMO.IS", "CRDFA.IS", "CRFSA.IS", "CUSAN.IS", "CVKMD.IS",
      "CWENE.IS", "DAGHL.IS", "DAGI.IS",  "DAPGM.IS", "DARDL.IS", "DCTTR.IS",
      "DENGE.IS", "DERHL.IS", "DERIM.IS", "DESA.IS",  "DESPC.IS", "DEVA.IS",
      "DGATE.IS", "DGGYO.IS", "DGNMO.IS", "DIRIT.IS", "DITAS.IS", "DMRGD.IS",
      "DMSAS.IS", "DNISI.IS", "DOAS.IS",  "DOBUR.IS", "DOCO.IS",  "DOFER.IS",
      "DOGUB.IS", "DOHOL.IS", "DOKTA.IS", "DURDO.IS", "DURKN.IS", "DYOBY.IS",
      "DZGYO.IS", "EBEBK.IS", "ECILC.IS", "ECZYT.IS", "EDATA.IS", "EDIP.IS",
      "EFORC.IS", "EGEEN.IS", "EGEPO.IS", "EGGUB.IS", "EGPRO.IS", "EGSER.IS",
      "EKGYO.IS", "EKIZ.IS",  "EKOS.IS",  "EKSUN.IS", "ELITE.IS", "EMKEL.IS",
      "EMNIS.IS", "ENERY.IS", "ENJSA.IS", "ENKAI.IS", "ENSRI.IS", "ENTRA.IS",
      "EPLAS.IS", "ERBOS.IS", "ERCB.IS",  "EREGL.IS", "ERSU.IS",  "ESCAR.IS",
      "ESCOM.IS", "ESEN.IS",  "ETILR.IS", "ETYAT.IS", "EUHOL.IS", "EUKYO.IS",
      "EUPWR.IS", "EUREN.IS", "EUYO.IS",  "EYGYO.IS", "FADE.IS",  "FENER.IS",
      "FLAP.IS",  "FMIZP.IS", "FONET.IS", "FORMT.IS", "FORTE.IS", "FRIGO.IS",
      "FROTO.IS", "FZLGY.IS", "GARAN.IS", "GARFA.IS", "GEDIK.IS", "GEDZA.IS",
      "GENIL.IS", "GENTS.IS", "GEREL.IS", "GESAN.IS", "GIPTA.IS", "GLBMD.IS",
      "GLCVY.IS", "GLRYH.IS", "GLYHO.IS", "GMTAS.IS", "GOKNR.IS", "GOLTS.IS",
      "GOODY.IS", "GOZDE.IS", "GRNYO.IS", "GRSEL.IS", "GRTHO.IS", "GSDDE.IS",
      "GSDHO.IS", "GSRAY.IS", "GUBRF.IS", "GUNDG.IS", "GWIND.IS", "GZNMI.IS",
      "HALKB.IS", "HATEK.IS", "HATSN.IS", "HDFGS.IS", "HEDEF.IS", "HEKTS.IS",
      "HKTM.IS",  "HLGYO.IS", "HOROZ.IS", "HRKET.IS", "HTTBT.IS", "HUBVC.IS",
      "HUNER.IS", "HURGZ.IS", "ICBCT.IS", "ICUGS.IS", "IDGYO.IS", "IEYHO.IS",
      "IHAAS.IS", "IHEVA.IS", "IHGZT.IS", "IHLAS.IS", "IHLGM.IS", "IHYAY.IS",
      "IMASM.IS", "INDES.IS", "INFO.IS",  "INGRM.IS", "INTEK.IS", "INTEM.IS",
      "INVEO.IS", "INVES.IS", "IPEKE.IS", "ISBIR.IS", "ISBTR.IS", "ISCTR.IS",
      "ISDMR.IS", "ISFIN.IS", "ISGSY.IS", "ISGYO.IS", "ISKPL.IS", "ISMEN.IS",
      "ISSEN.IS", "ISYAT.IS", "IZENR.IS", "IZFAS.IS", "IZINV.IS", "IZMDC.IS",
      "JANTS.IS", "KAPLM.IS", "KAREL.IS", "KARSN.IS", "KARTN.IS", "KARYE.IS",
      "KATMR.IS", "KAYSE.IS", "KBORU.IS", "KCAER.IS", "KCHOL.IS", "KENT.IS",
      "KERVN.IS", "KERVT.IS", "KFEIN.IS", "KGYO.IS",  "KIMMR.IS", "KLGYO.IS",
      "KLKIM.IS", "KLMSN.IS", "KLNMA.IS", "KLRHO.IS", "KLSER.IS", "KLSYN.IS",
      "KMPUR.IS", "KNFRT.IS", "KOCMT.IS", "KONKA.IS", "KONTR.IS", "KONYA.IS",
      "KOPOL.IS", "KORDS.IS", "KOTON.IS", "KOZAA.IS", "KOZAL.IS", "KRDMA.IS",
      "KRDMB.IS", "KRDMD.IS", "KRGYO.IS", "KRONT.IS", "KRPLS.IS", "KRSTL.IS",
      "KRTEK.IS", "KRVGD.IS", "KSTUR.IS", "KTLEV.IS", "KTSKR.IS", "KUTPO.IS",
      "KUVVA.IS", "KUYAS.IS", "KZBGY.IS", "KZGYO.IS", "LIDER.IS", "LIDFA.IS",
      "LILAK.IS", "LINK.IS",  "LKMNH.IS", "LMKDC.IS", "LOGO.IS",  "LRSHO.IS",
      "LUKSK.IS", "LYDHO.IS", "LYDYE.IS", "MAALT.IS", "MACKO.IS", "MAGEN.IS",
      "MAKIM.IS", "MAKTK.IS", "MANAS.IS", "MARBL.IS", "MARKA.IS", "MARTI.IS",
      "MAVI.IS",  "MEDTR.IS", "MEGAP.IS", "MEGMT.IS", "MEKAG.IS", "MEPET.IS",
      "MERCN.IS", "MERIT.IS", "MERKO.IS", "METRO.IS", "METUR.IS", "MGROS.IS",
      "MHRGY.IS", "MIATK.IS", "MMCAS.IS", "MNDRS.IS", "MNDTR.IS", "MOBTL.IS",
      "MOGAN.IS", "MPARK.IS", "MRGYO.IS", "MRSHL.IS", "MSGYO.IS", "MTRKS.IS",
      "MTRYO.IS", "MZHLD.IS", "NATEN.IS", "NETAS.IS", "NIBAS.IS", "NTGAZ.IS",
      "NTHOL.IS", "NUGYO.IS", "NUHCM.IS", "OBAMS.IS", "OBASE.IS", "ODAS.IS",
      "ODINE.IS", "OFSYM.IS", "ONCSM.IS", "ONRYT.IS", "ORCAY.IS", "ORGE.IS",
      "ORMA.IS",  "OSMEN.IS", "OSTIM.IS", "OTKAR.IS", "OTTO.IS",  "OYAKC.IS",
      "OYAYO.IS", "OYLUM.IS", "OYYAT.IS", "OZATD.IS", "OZGYO.IS", "OZKGY.IS",
      "OZRDN.IS", "OZSUB.IS", "OZYSR.IS", "PAGYO.IS", "PAMEL.IS", "PAPIL.IS",
      "PARSN.IS", "PASEU.IS", "PATEK.IS", "PCILT.IS", "PEHOL.IS", "PEKGY.IS",
      "PENGD.IS", "PENTA.IS", "PETKM.IS", "PETUN.IS", "PGSUS.IS", "PINSU.IS",
      "PKART.IS", "PKENT.IS", "PLTUR.IS", "PNLSN.IS", "PNSUT.IS", "POLHO.IS",
      "POLTK.IS", "PRDGS.IS", "PRKAB.IS", "PRKME.IS", "PRZMA.IS", "PSDTC.IS",
      "PSGYO.IS", "QNBFB.IS", "QNBFL.IS", "QUAGR.IS", "RALYH.IS", "RAYSG.IS",
      "REEDR.IS", "RGYAS.IS", "RNPOL.IS", "RODRG.IS", "RTALB.IS", "RUBNS.IS",
      "RYGYO.IS", "RYSAS.IS", "SAFKR.IS", "SAHOL.IS", "SAMAT.IS", "SANEL.IS",
      "SANFM.IS", "SANKO.IS", "SARKY.IS", "SASA.IS",  "SAYAS.IS", "SDTTR.IS",
      "SEGMN.IS", "SEGYO.IS", "SEKFK.IS", "SEKUR.IS", "SELEC.IS", "SELGD.IS",
      "SELVA.IS", "SEYKM.IS", "SILVR.IS", "SISE.IS",  "SKBNK.IS", "SKTAS.IS",
      "SKYLP.IS", "SKYMD.IS", "SMART.IS", "SMRTG.IS", "SNGYO.IS", "SNICA.IS",
      "SNKRN.IS", "SNPAM.IS", "SODSN.IS", "SOKE.IS",  "SOKM.IS",  "SONME.IS",
      "SRVGY.IS", "SUMAS.IS", "SUNTK.IS", "SURGY.IS", "SUWEN.IS", "TABGD.IS",
      "TARKM.IS", "TATEN.IS", "TATGD.IS", "TAVHL.IS", "TBORG.IS", "TCELL.IS",
      "TCKRC.IS", "TDGYO.IS", "TEKTU.IS", "TERA.IS",  "TEZOL.IS", "TGSAS.IS",
      "THYAO.IS", "TKFEN.IS", "TKNSA.IS", "TLMAN.IS", "TMPOL.IS", "TMSN.IS",
      "TNZTP.IS", "TOASO.IS", "TRCAS.IS", "TRGYO.IS", "TRILC.IS", "TSGYO.IS",
      "TSKB.IS",  "TSPOR.IS", "TTKOM.IS", "TTRAK.IS", "TUCLK.IS", "TUKAS.IS",
      "TUPRS.IS", "TUREX.IS", "TURGG.IS", "TURSG.IS", "UFUK.IS",  "ULAS.IS",
      "ULKER.IS", "ULUFA.IS", "ULUSE.IS", "ULUUN.IS", "UNLU.IS",  "USAK.IS",
      "VAKBN.IS", "VAKFN.IS", "VAKKO.IS", "VANGD.IS", "VBTYZ.IS", "VERTU.IS",
      "VERUS.IS", "VESBE.IS", "VESTL.IS", "VKFYO.IS", "VKGYO.IS", "VKING.IS",
      "VRGYO.IS", "YAPRK.IS", "YATAS.IS", "YAYLA.IS", "YBTAS.IS", "YEOTK.IS",
      "YESIL.IS", "YGGYO.IS", "YGYO.IS",  "YIGIT.IS", "YKBNK.IS", "YKSLN.IS",
      "YONGA.IS", "YUNSA.IS", "YYAPI.IS", "YYLGD.IS", "ZEDUR.IS", "ZOREN.IS",
      "ZRGYO.IS", "XU030.IS", "XU100.IS", "TRY=X",    "EURTRY=X", "GC=F",
      "BZ=F",     "BTC-USD"};

  default_json["period"] = 'D';
  default_json["timeframe"] = 0;
  default_json["len"] = 4;
  default_json["div"] = "TRY=X";
  default_json["msdir"] = fs::current_path().append("daily\\");
  default_json["targetdir"] = default_json["msdir"];
  std::filesystem::create_directory(default_json["msdir"]);

  return DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr,
                        DialogProc, NULL);
}
