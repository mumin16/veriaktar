#include <Windows.h>
#include <Urlmon.h>
#pragma comment(lib,"Urlmon.lib")
#include "resource.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#pragma comment(linker, "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")

struct FXHEADER {
	char unkown[2] = { 0 };
	char totalrecord[2] = { 0 };//ters+1 ieee
	char _unkown[24] = { 0 };
}tagFXHEADER;

struct FX {
	char date[4] = { 0 };//YYMMDD ieee  YY=yyyy- 1900 
	char time[4] = { 0 };//2359000 ieee
	char open[4] = { 0 };//ieee
	char high[4] = { 0 };//ieee
	char low[4] = { 0 };//ieee
	char close[4] = { 0 };//ieee
	char volume[4] = { 0 };//ieee

}tagFX;

struct MASTERHEADER {
	unsigned char totalfx[1] = { 0 };
	char unknown[1] = { 0 };
	unsigned char _totalfx[1] = { 0 };
	char _unknown[50] = { 0 };
}tagMASTERHEADER;

struct MASTER {
	unsigned char fx[1] = { 0 };
	char unknown[2] = { 0 };
	char bune[1] = { 0x1c };//7kolonsayisi*4
	char kolonsayisi[1] = { 7 };//date-time-o-h-l-c-v
	char ___unknown[2] = { 0 };
	char secname[16] = { 0 };
	char _unknown[2] = { 0 };
	char fdate[4] = { 0 };//YYMMDD ieee  YY=yyyy- 1900 
	char ldate[4] = { 0 };//YYMMDD ieee 
	char period[1] = { 'I' };//'I', 'D'
	char timeframe[2] = { 1 };//0 and 60 minutes
	char secsymbol[14] = { 0 };
	char __unknown[3] = { 0 };
}tagMASTER;

struct XMASTERHEADER {
	unsigned char unknown[18] = { 0 };
	unsigned char totalfx_datmwd[2] = { 0 };//tersi
	unsigned char _unknown[130] = { 0 };
}tagXMASTERHEADER;

struct XMASTER {
	unsigned char reserved1[1] = { 1 };
	char secsymbol[15] = { 0 };
	char secname[46] = { 0 };
	char period[1] = { 'I' };//'I', 'D'
	char timeframe[2] = { 1 };//0 and 60 minutes
	unsigned char fx[2] = { 0 };//TERSİ
	unsigned char _unknown[3] = { 0 };
	unsigned char __unknown[1] = { 0xBF };//3d daily
	unsigned char ___unknown[33] = { 0 };
	char fdate[4] = { 0 };//TERSİ 20160229
	char _fdate[4] = { 0 };//TERSİ 20160229
	unsigned char ____unknown[4] = { 0 };
	char ldate[4] = { 0 };//TERSİ 20160229
	unsigned char _____unknown[30] = { 0 };
}tagXMASTER;

XMASTERHEADER xmasterheader;
XMASTER xmaster;
MASTERHEADER masterheader;
MASTER master;
FXHEADER fxheader;
FX fx;




std::vector<FX> fxs;
std::vector<FXHEADER> fxhs;



char curdir[MAX_PATH];
#include "msbinieee.h"
#include "hakkinda.h"
#include "portfoyyuklekaydet.h"
#include "veriaktar.h"
#include "veriindir.h"
#include "eklecikar.h"




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
		URLDownloadToFile(NULL, "https://raw.githubusercontent.com/mumin16/veriaktar/master/BIST30.txt", "BIST30.txt", 0, NULL);
		URLDownloadToFile(NULL, "https://raw.githubusercontent.com/mumin16/veriaktar/master/BIST50.txt", "BIST50.txt", 0, NULL);
		URLDownloadToFile(NULL, "https://raw.githubusercontent.com/mumin16/veriaktar/master/BIST100.txt", "BIST100.txt", 0, NULL);
		URLDownloadToFile(NULL, "https://raw.githubusercontent.com/mumin16/veriaktar/master/BISTTUM.txt", "BISTTUM.txt", 0, NULL);
//		URLDownloadToFile(NULL, "https://raw.githubusercontent.com/mumin16/veriaktar/master/BISTTUM.txt", "BISTENDEKSLER.txt", 0, NULL);
		URLDownloadToFile(NULL, "https://raw.githubusercontent.com/mumin16/veriaktar/master/BISTTUM.txt", "BOLUNMELER.txt", 0, NULL);

		SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"BIST30");
		SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"BIST50");
		SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"BIST100");
		SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"BISTTUM");
//		SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"BISTENDEKSLER");
//		SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"DUNYAENDEKSLER");
		//SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"PARITELER");
		SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_SETCURSEL, 0, NULL);



		PostMessage(GetDlgItem(hwndDlg, IDC_CHECK1), BM_SETCHECK, BST_CHECKED, 0);

		std::string sLine = "";
		std::ifstream infile;
		infile.open("BIST30.txt");
		while (!infile.eof())
		{
			getline(infile, sLine);
			SendDlgItemMessage(hwndDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)sLine.c_str());
		}
		infile.close();

	
		
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