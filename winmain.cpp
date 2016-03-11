#include <Windows.h>
#include <Urlmon.h>
#pragma comment(lib,"Urlmon.lib")
#include "resource.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
	char totalfx[1] = { 0 };
	char unknown[1] = { 0 };
	char _totalfx[1] = { 0 };
	char _unknown[50] = { 0 };
}tagMASTERHEADER;

struct MASTER {
	char fx[1] = { 0 };
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




#define BUILDVERSION "0"

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

		SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"BIST30");
		SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"BIST50");
		SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"BIST100");
		SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"BISTENDEKSLER");
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

	
		
	SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_ADDSTRING, 0, (LPARAM)"Program Kullanıma Hazır.");
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

	GetCurrentDirectory(MAX_PATH, curdir);
	return DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, NULL);

}