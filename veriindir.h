#pragma once
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
		sprintf(buffer, "http://www.google.com/finance/historical?output=csv&startdate=May+1%,+2010&enddate=&q=%s", buffer2);
		if (S_OK == URLDownloadToFile(NULL, buffer, buffer2, 0, NULL)) {
			//"Ok";


			incsv.open(buffer2);
			getline(incsv, sLine);//ilk satýrý al
			while (!incsv.eof())
			{
				
				
				getline(incsv, sLine);
				char sday[12], smonth[12], syear[12], sopen[12], shigh[12], slow[12], sclose[12], svol[12];
				sscanf(sLine.c_str(), "%[^-]-%[^-]-%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", sday, smonth, syear, sopen, shigh, slow, sclose, svol);

				unsigned char ret[255];
				float f = atof(sopen);
				IEEEToBasic(&f, ret);
				memmove(fx.open, (LPCSTR)ret, 4);

				f = atof(shigh);
				IEEEToBasic(&f, ret);
				memmove(fx.high, (LPCSTR)ret, 4);

				f = atof(slow);
				IEEEToBasic(&f, ret);
				memmove(fx.low, (LPCSTR)ret, 4);

				f = atof(sclose);
				IEEEToBasic(&f, ret);
				memmove(fx.close, (LPCSTR)ret, 4);

				f = atof(svol);
				IEEEToBasic(&f, ret);
				memmove(fx.volume, (LPCSTR)ret, 4);

				std::string date = std::to_string(atoi(syear) + 100);
						
				if (0 == lstrcmp(smonth, "Jan")) lstrcpy(smonth, "01");
				if (0 == lstrcmp(smonth, "Feb")) lstrcpy(smonth, "02");
				if (0 == lstrcmp(smonth, "Mar")) lstrcpy(smonth, "03");
				if (0 == lstrcmp(smonth, "Apr")) lstrcpy(smonth, "04");
				if (0 == lstrcmp(smonth, "May")) lstrcpy(smonth, "05");
				if (0 == lstrcmp(smonth, "Jun")) lstrcpy(smonth, "06");
				if (0 == lstrcmp(smonth, "Jul")) lstrcpy(smonth, "07");
				if (0 == lstrcmp(smonth, "Aug")) lstrcpy(smonth, "08");
				if (0 == lstrcmp(smonth, "Sep")) lstrcpy(smonth, "09");
				if (0 == lstrcmp(smonth, "Oct")) lstrcpy(smonth, "10");
				if (0 == lstrcmp(smonth, "Nov")) lstrcpy(smonth, "11");
				if (0 == lstrcmp(smonth, "Dec")) lstrcpy(smonth, "12");
				date.append(smonth);

				if (1 == lstrlen(sday))date.append("0");
				date.append(sday);

				f = atof(date.c_str());
				IEEEToBasic(&f, ret);
				memmove(fx.date, (LPCSTR)ret, 4);
			
				fxs.push_back(fx);

			}
			std::reverse(fxs.begin(), fxs.end());    // 9 8 7 6 5 4 3 2 1
			
				if(fxs.size()<255)
				{ 
					fxheader.totalrecord[0] = fxs.size()  + 1;
				}
				else
				{
					unsigned char ret[255];
					float f = fxs.size() + 1;
					IEEEToBasic(&f, ret);
					std::reverse(ret, ret + 4);
					fxheader.totalrecord[0] = ret[1];
					fxheader.totalrecord[1] = ret[0];
				}

			
			VeriYaz(hwndDlg, buffer2);
			fxs.clear();
			ZeroMemory(&master, sizeof(MASTER));
			ZeroMemory(&masterheader, sizeof(MASTER));
			incsv.close();
			DeleteFile(buffer2);
			std::string a = buffer2;
			a.append(" yüklendi, aktarýldý..");
			SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_INSERTSTRING, 0, (LPARAM)a.c_str());
		}
		else {
			MessageBox(hwndDlg, "Yükleme sýrasýnda hata", buffer2, MB_OK);
			continue;

		}


		
	}





}


DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
	) {
	VeriIndir((HWND)lpParameter);
	return 0;
}