#include <Windows.h>
#include <Urlmon.h>
#pragma comment(lib,"Urlmon.lib")
#include "resource.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#pragma comment(linker, "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")

char curdir[MAX_PATH];


#include "hakkinda.h"
#include "Metastock.h"


#include "veriindir.h"
#include "bolunmeler.h"
#include "semboller.h"




#define BUILDVERSION "1"



INT_PTR CALLBACK DialogProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{


		if (S_OK == URLDownloadToFile(NULL, "https://raw.githubusercontent.com/mumin16/veriaktar/master/versiyon.txt", "versiyon.txt", 0, NULL)) {
			//"Ok";
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


		



		PostMessage(GetDlgItem(hwndDlg, IDC_CHECK1), BM_SETCHECK, BST_CHECKED, 0);

	
		
	SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_ADDSTRING, 0, (LPARAM)"Programin son surumunun kullanildigina emin olun!");
	break;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDR_VERIAKTAR) {
			CloseHandle(CreateThread(NULL, NULL, ThreadProc, hwndDlg, 0, 0));
			
		}
		else if (LOWORD(wParam) == IDR_HAKKINDA)Hakkinda(hwndDlg);
		else if (LOWORD(wParam) == IDR_PORTFOYYUKLE)PortfoyYukle(hwndDlg);
		else if (LOWORD(wParam) == IDR_PORTFOYKAYDET)PortfoyKaydet(hwndDlg);
		else if (LOWORD(wParam) == IDC_EKLE)Ekle(hwndDlg);
		else if (LOWORD(wParam) == IDC_HEPSINIEKLE)HepsiniEkle(hwndDlg);
		else if (LOWORD(wParam) == IDC_CIKAR)Cikar(hwndDlg);
		else if (LOWORD(wParam) == IDC_HEPSINICIKAR)HepsiniCikar(hwndDlg);
		else if (LOWORD(wParam) == IDC_COMBO1 && HIWORD(wParam) == CBN_SELCHANGE)ComboDegisti(hwndDlg);
		
		break;
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		break;
	}

	return false;
}



int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
	) {



	//unsigned char ret[255];
	//float f = 1160309;//
	//IEEEToBasic(&f, ret);
	CreateDirectory("GUNLUK", 0);	
	GetCurrentDirectory(MAX_PATH, curdir);
	return DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, NULL);

}



/*

//MessageBox(hwndDlg, GetLastErrorAsString().c_str(), 0, 0);
//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetLastErrorAsString()
{
//Get the error message, if any.
DWORD errorMessageID = ::GetLastError();
if (errorMessageID == 0)
return std::string(); //No error message has been recorded

LPSTR messageBuffer = nullptr;
size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

std::string message(messageBuffer, size);

//Free the buffer.
LocalFree(messageBuffer);

return message;
}
*/