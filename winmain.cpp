#include <Windows.h>
#include <Urlmon.h>
#pragma comment(lib,"Urlmon.lib")
#include "resource.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

char curdir[MAX_PATH];

#include "bolunmeler.h"
#include "Metastock.h"
#include "veriindir.h"
#include "semboller.h"
#include "ayarlar.h"

#define BUILDVERSION "20160321"

INT_PTR CALLBACK DialogProc(_In_ HWND   hwndDlg,_In_ UINT   uMsg,_In_ WPARAM wParam,_In_ LPARAM lParam)
{
	switch (uMsg) {
		case WM_INITDIALOG:
		{
			if (S_OK == URLDownloadToFile(NULL, "https://raw.githubusercontent.com/mumin16/veriaktar/master/versiyon.txt", "versiyon.txt", 0, NULL)) {
				std::string sLine = "";
				std::ifstream infile;
				infile.open("versiyon.txt");
				getline(infile, sLine);
				if(lstrcmp(sLine.c_str(),BUILDVERSION)>0)MessageBox(hwndDlg,"Yeni Güncelleme var!","Bilgi",MB_OK);
			}
			else {
				MessageBox(hwndDlg, "Versiyon Kontrolu Yapılamadı\n Son Sürümü kullandığınıza emin olun\n internet bağlantısını kontrol edin", 0, MB_OK);
			}

			CloseHandle(CreateThread(NULL, NULL, ThreadProcSemboller, hwndDlg, 0, 0));
			CloseHandle(CreateThread(NULL, NULL, ThreadProcBolunmeler, hwndDlg, 0, 0));
			PostMessage(GetDlgItem(hwndDlg, IDC_GUNLUK), BM_SETCHECK, BST_CHECKED, 0);
			SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_ADDSTRING, 0, (LPARAM)"Programin son surumunun kullanildigina emin olun!");
		break;
		}
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDR_VERIAKTAR) {
				CloseHandle(CreateThread(NULL, NULL, ThreadProc, hwndDlg, 0, 0));
			}
			else if (LOWORD(wParam) == IDR_HAKKINDA)MessageBox(hwndDlg, "Mumin GULER\nmumin16@hotmail.com\nhttps://github.com/mumin16/veriaktar/\nhttp://veriaktar.blogspot.com.tr/", "Hakkinda", MB_OK);
			else if (LOWORD(wParam) == IDR_PORTFOYYUKLE)PortfoyYukle(hwndDlg);
			else if (LOWORD(wParam) == IDR_PORTFOYKAYDET)PortfoyKaydet(hwndDlg);
			else if (LOWORD(wParam) == IDR_AYARLAR)Ayarlar(hwndDlg);
			else if (LOWORD(wParam) == IDC_EKLE)Ekle(hwndDlg);
			else if (LOWORD(wParam) == IDC_HEPSINIEKLE)HepsiniEkle(hwndDlg);
			else if (LOWORD(wParam) == IDC_CIKAR)Cikar(hwndDlg);
			else if (LOWORD(wParam) == IDC_HEPSINICIKAR)HepsiniCikar(hwndDlg);
			else if (LOWORD(wParam) == IDC_COMBO1 && HIWORD(wParam) == CBN_SELCHANGE)ComboDegisti(hwndDlg);
		
		break;
		}
		case WM_CLOSE:
		{
			EndDialog(hwndDlg, 0);
		break;
		}
	}

return false;
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance,_In_ HINSTANCE hPrevInstance,_In_ LPSTR     lpCmdLine,_In_ int       nCmdShow)
{
	GetCurrentDirectory(MAX_PATH, curdir);
	CreateDirectory("GUNLUK", 0);	
	CreateDirectory("1DAKIKA", 0);
	return DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, NULL);
}



