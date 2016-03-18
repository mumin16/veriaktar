#pragma once
FX fx;
std::vector<FX> fxs;
FXI fxi;
std::vector<FXI> fxis;
Metastock* ms;

std::vector<std::string> yuklenemeyenler;

void Dakikalikindir(_In_ HWND   hwndDlg) {
	char buffer[250];
	std::string smarketopenminute = "";
	std::string sLine = "";
	std::ifstream incsv;


	char buffer2[255];
	int length2 = SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETCOUNT, 0, 0);

	for (size_t i = 0; i < length2; i++)
	{
		//http://www.google.com/finance/getprices?&i=60&p=1d&q=XU030        60=SANÝYE 1D=1GÜNLÜK tarih ayýklayýcý=A1/86400 + 25569 + (5.5/24)
		SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETTEXT, i, (LPARAM)buffer2);
		sprintf(buffer, "http://www.google.com/finance/getprices?&i=60&p=2d&q=%s", buffer2);
		if (S_OK == URLDownloadToFile(NULL, buffer, buffer2, 0, NULL)) {
			//"Ok";
			ms = new Metastock;
			incsv.open(buffer2);
			getline(incsv, sLine);//ilk8 satýrý al
			getline(incsv, smarketopenminute);//ilk8 satýrý al
			getline(incsv, sLine);//ilk8 satýrý al
			getline(incsv, sLine);//ilk8 satýrý al
			getline(incsv, sLine);//ilk8 satýrý al
			getline(incsv, sLine);//ilk8 satýrý al
			getline(incsv, sLine);//ilk8 satýrý al
			getline(incsv, sLine);//ilk8 satýrý al
			if (incsv.eof())return;
			while (!incsv.eof())
			{
				
				getline(incsv, sLine);
				if (incsv.eof())break;
				char sdate[12], sclose[12], shigh[12], slow[12], sopen[12], svolume[12];
				sscanf(sLine.c_str(), "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", sdate, sclose, shigh, slow, sopen, svolume);

				unsigned char ret[255];
				float f = atof(sopen);
				ms->IEEEToBasic(&f, ret);
				memmove(fx.open, (LPCSTR)ret, 4);

				f = atof(shigh);
				ms->IEEEToBasic(&f, ret);
				memmove(fx.high, (LPCSTR)ret, 4);

				f = atof(slow);
				ms->IEEEToBasic(&f, ret);
				memmove(fx.low, (LPCSTR)ret, 4);

				f = atof(sclose);
				ms->IEEEToBasic(&f, ret);
				memmove(fx.close, (LPCSTR)ret, 4);

				f = atof(svolume);
				ms->IEEEToBasic(&f, ret);
				memmove(fx.volume, (LPCSTR)ret, 4);

				std::string date = "20160317";


				f = atof(date.c_str());
				ms->IEEEToBasic(&f, ret);
				memmove(fx.date, (LPCSTR)ret, 4);

				fxs.push_back(fx);


			}
			


			std::string dir = curdir;
	
				dir.append("\\1DAKIKA");
			

			ms->WriteSec(buffer2, fxs, dir.c_str());
			delete ms;

			fxs.clear();
			incsv.close();
			DeleteFile(buffer2);
			std::string a = buffer2;
			a.append(" yuklendi, aktarildi...");
			SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_INSERTSTRING, 0, (LPARAM)a.c_str());

		}
		else {
			std::string a = buffer2;
			a.append(" yuklenemedi...");
			SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_INSERTSTRING, 0, (LPARAM)a.c_str());
			yuklenemeyenler.push_back(buffer2);
			continue;

		}



	}

	SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_RESETCONTENT, 0, 0);

	for (std::vector<int>::size_type i = 0; i != yuklenemeyenler.size(); i++) {
		/* std::cout << someVector[i]; ... */
		SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_INSERTSTRING, 0, (LPARAM)yuklenemeyenler[i].c_str());
	}

	yuklenemeyenler.clear();
	SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_INSERTSTRING, 0, (LPARAM)"::::::::YUKLEME TAMAMLANDI::::::::");
}
void VeriIndir(_In_ HWND   hwndDlg) {


	if (0 == SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETCOUNT, 0, 0)){
		MessageBox(hwndDlg, "Yuklenecekler Belirtilmemis!", 0, MB_OK);
		return;
	}

	if (1 == SendDlgItemMessage(hwndDlg, IDC_1DAKIKA, BM_GETCHECK, 0, 0)) {  
		Dakikalikindir(hwndDlg);
		return;
	}
	
	
	//if(1==SendDlgItemMessage(hwndDlg, IDC_GUNLUK, BM_GETCHECK, 0, 0))	MessageBox(hwndDlg, "Günlük Periyot secilmis!", 0, MB_OK);
		
	


	

	char buffer[250];
	std::string sLine = "";
	std::ifstream incsv;


	char buffer2[255];
	int length2 = SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETCOUNT, 0, 0);

	for (size_t i = 0; i < length2; i++)
	{
		//http://www.google.com/finance/getprices?&i=60&p=1d&q=XU030        60=SANÝYE 1D=1GÜNLÜK tarih ayýklayýcý=A1/86400 + 25569 + (5.5/24)
		SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETTEXT, i, (LPARAM)buffer2);
		sprintf(buffer, "http://www.google.com/finance/historical?output=csv&startdate=Mar+17%,+2014&enddate=&q=%s", buffer2);
		if (S_OK == URLDownloadToFile(NULL, buffer, buffer2, 0, NULL)) {
			//"Ok";
			ms = new Metastock;
			incsv.open(buffer2);
			getline(incsv, sLine);//ilk satýrý al
			while (!incsv.eof())
			{
				
				getline(incsv, sLine);
				if (incsv.eof())break;
				char sday[12], smonth[12], syear[12], sopen[12], shigh[12], slow[12], sclose[12], svol[12];
				sscanf(sLine.c_str(), "%[^-]-%[^-]-%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", sday, smonth, syear, sopen, shigh, slow, sclose, svol);

				unsigned char ret[255];
				float f = atof(sopen);
				ms->IEEEToBasic(&f, ret);
				memmove(fx.open, (LPCSTR)ret, 4);

				f = atof(shigh);
				ms->IEEEToBasic(&f, ret);
				memmove(fx.high, (LPCSTR)ret, 4);

				f = atof(slow);
				ms->IEEEToBasic(&f, ret);
				memmove(fx.low, (LPCSTR)ret, 4);

				f = atof(sclose);
				ms->IEEEToBasic(&f, ret);
				memmove(fx.close, (LPCSTR)ret, 4);

				f = atof(svol);
				ms->IEEEToBasic(&f, ret);
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
				ms->IEEEToBasic(&f, ret);
				memmove(fx.date, (LPCSTR)ret, 4);
			
				fxs.push_back(fx);
				

			}
			std::reverse(fxs.begin(), fxs.end());    // 9 8 7 6 5 4 3 2 1
	


				std::string dir = curdir;
				
					dir.append("\\GUNLUK");
			
				
				ms->WriteSec(buffer2, fxs, dir.c_str());
				delete ms;

			fxs.clear();
			incsv.close();
			DeleteFile(buffer2);
			std::string a = buffer2;
			a.append(" yuklendi, aktarildi...");
			SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_INSERTSTRING, 0, (LPARAM)a.c_str());
			
		}
		else {
			std::string a = buffer2;
			a.append(" yuklenemedi...");
			SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_INSERTSTRING, 0, (LPARAM)a.c_str());
			yuklenemeyenler.push_back(buffer2);
			continue;

		}

	

	}


	SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_RESETCONTENT, 0, 0);

	for (std::vector<int>::size_type i = 0; i != yuklenemeyenler.size(); i++) {
		/* std::cout << someVector[i]; ... */
		SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_INSERTSTRING, 0, (LPARAM)yuklenemeyenler[i].c_str());
	}

	yuklenemeyenler.clear();
	SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_INSERTSTRING, 0, (LPARAM)"::::::::YUKLEME TAMAMLANDI::::::::");
}


DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
	) {
	VeriIndir((HWND)lpParameter);
	return 0;
}