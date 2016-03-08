#pragma once

void VeriYaz(_In_ HWND   hwndDlg) {
}
void VeriIndir(_In_ HWND   hwndDlg) {

	if (0 == SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETCOUNT, 0, 0)){
		MessageBox(hwndDlg, "Yüklenecekler Belirtilmemiþ!", 0, MB_OK);
		return;
	}

	if (0 == SendDlgItemMessage(hwndDlg, IDC_GUNLUK, BM_GETCHECK, 0, 0) && 0 == SendDlgItemMessage(hwndDlg, IDC_1DAKIKA, BM_GETCHECK, 0, 0)) {
		MessageBox(hwndDlg, "Periyot seçilmemiþ!", 0, MB_OK);
		return;
	}


	char buffer[250];
	std::string sLine = "";
	std::ifstream incsv;


	char buffer2[255];
	int length2 = SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETCOUNT, 0, 0);

	for (size_t i = 0; i < length2; i++)
	{
		SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETTEXT, i, (LPARAM)buffer2);
		sprintf(buffer, "http://www.google.com/finance/historical?output=csv&q=%s", buffer2);
		if (S_OK == URLDownloadToFile(NULL, buffer, buffer2, 0, NULL)) {
			//"Ok";
			incsv.open(buffer2);
			getline(incsv, sLine);//ilk satýrý al
			while (!incsv.eof())
			{
				getline(incsv, sLine);
				char sday[12], smonth[12], syear[12], sopen[12], shigh[12], slow[12], sclose[12], svol[12];
				sscanf(sLine.c_str(), "%[^-]-%[^-]-%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", sday, smonth, syear, sopen, shigh, slow, sclose, svol);


				//const char * months[] = { "Jan", "Feb","Mar", "Apr","May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
				//for (size_t i = 0; i < 11; i++)
				//{
				

				//}


			//	//atof(sopen);
			//	//****
			//	//atoi(svol);

			
			}
			incsv.close();
			DeleteFile(buffer2);
			std::string a = buffer2;
			a.append(" yüklendi, aktarýldý..");
			SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_INSERTSTRING, 0, (LPARAM)a.c_str());
		}
		else {
			MessageBox(hwndDlg, "Yükleme sýrasýnda hata", buffer2, MB_OK);
			return;

		}


		
	}





}


DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
	) {
	VeriIndir((HWND)lpParameter);
	return 0;
}