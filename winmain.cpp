#include <Windows.h>
#include <Urlmon.h>
#pragma comment(lib,"Urlmon.lib")
#include "resource.h"
#include <iostream>
#include <fstream>
#include <string>
#include "msbinieee.h"
#include "hakkinda.h"
#include "portfoyyuklekaydet.h"
#include "veriaktar.h"
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
			MessageBox(hwndDlg, "Versiyon Kontrolu Yapýlamadý\n Son Sürümü kullandýðýnýza emin olun\n internet baðlantýsýný kontrol edin", 0, MB_OK);
			

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

	
		
	SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_ADDSTRING, 0, (LPARAM)"Program Kullanýma Hazýr.");
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


	return DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc, NULL);

}