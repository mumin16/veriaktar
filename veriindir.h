#pragma once
FX fx;
std::vector<FX> fxs;
FXI fxi;
std::vector<FXI> fxis;
Metastock* ms;

std::vector<std::string> yuklenemeyenler;

char unformatteddate[12];
std::string idate = "";
std::string itime = "";
struct tm* time_info;

void Dakikalikindir(_In_ HWND   hwndDlg) {
	char buffer[250];
	std::string sLine = "";
	std::ifstream incsv;


	char buffer2[255];
	int length2 = SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETCOUNT, 0, 0);

	for (size_t i = 0; i < length2; i++)
	{
		//http://www.google.com/finance/getprices?&i=60&p=1d&q=XU030        60=SAN�YE 1D=1G�NL�K tarih ay�klay�c�=A1/86400 + 25569 + (5.5/24)
		SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETTEXT, i, (LPARAM)buffer2);
		sprintf(buffer, "http://www.google.com/finance/getprices?&i=60&p=15d&q=%s", buffer2);
		if (S_OK == URLDownloadToFile(NULL, buffer, buffer2, 0, NULL)) {
			//"Ok";
			
			ms = new Metastock;
			incsv.open(buffer2);
			getline(incsv, sLine);//ilk8 sat�r� al
			getline(incsv, sLine);//ilk8 sat�r� al
			getline(incsv, sLine);//ilk8 sat�r� al
			getline(incsv, sLine);//ilk8 sat�r� al
			getline(incsv, sLine);//ilk8 sat�r� al
			getline(incsv, sLine);//ilk8 sat�r� al
			getline(incsv, sLine);//ilk8 sat�r� al
			getline(incsv, sLine);//ilk8 sat�r� al
			if (incsv.eof()) {
				std::string a = buffer2;
				a.append(" yuklenemedi...");
				SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_INSERTSTRING, 0, (LPARAM)a.c_str());
				yuklenemeyenler.push_back(buffer2);
				incsv.close();
				DeleteFile(buffer2);
				continue;
			}
			while (!incsv.eof())
			{
				
				getline(incsv, sLine);
				if (incsv.eof())break;
				char sdate[255], sclose[255], shigh[255], slow[255], sopen[255], svolume[255];
				sscanf(sLine.c_str(), "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", sdate, sclose, shigh, slow, sopen, svolume);

				unsigned char ret[255];
				float f = atof(sopen);
				ms->IEEEToBasic(&f, ret);
				memmove(fxi.open, (LPCSTR)ret, 4);

				f = atof(shigh);
				ms->IEEEToBasic(&f, ret);
				memmove(fxi.high, (LPCSTR)ret, 4);

				f = atof(slow);
				ms->IEEEToBasic(&f, ret);
				memmove(fxi.low, (LPCSTR)ret, 4);

				f = atof(sclose);
				ms->IEEEToBasic(&f, ret);
				memmove(fxi.close, (LPCSTR)ret, 4);

				f = atof(svolume);
				ms->IEEEToBasic(&f, ret);
				memmove(fxi.volume, (LPCSTR)ret, 4);

				
				if (0 == strncmp(sdate, "a", 1))
				{
				strcpy(sdate, sdate + 1);
				strcpy(unformatteddate, sdate );
				
				time_t timeNow= atol(sdate);
				time_info= localtime(&timeNow);

				idate = std::to_string(time_info->tm_year);
				if (time_info->tm_mon+1 <10)idate.append("0");
				idate.append(std::to_string(time_info->tm_mon+1));
				if (time_info->tm_mday <10)idate.append("0");
				idate.append(std::to_string(time_info->tm_mday));
				
				itime = std::to_string(time_info->tm_hour);
				if (time_info->tm_min <10)itime.append("0");
				itime.append(std::to_string(time_info->tm_min));
				itime.append("00");

				
				}
				else {
					time_t timeNow = atol(unformatteddate)+60* atoi(sdate);
					time_info = localtime(&timeNow);

					idate = std::to_string(time_info->tm_year);
					if (time_info->tm_mon + 1 <10)idate.append("0");
					idate.append(std::to_string(time_info->tm_mon + 1));
					if (time_info->tm_mday <10)idate.append("0");
					idate.append(std::to_string(time_info->tm_mday));

					itime = std::to_string(time_info->tm_hour);
					if (time_info->tm_min <10)itime.append("0");
					itime.append(std::to_string(time_info->tm_min));
					itime.append("00");
				}

				f = atof(idate.c_str());
				ms->IEEEToBasic(&f, ret);
				memmove(fxi.date, (LPCSTR)ret, 4);


				f = atof(itime.c_str());
				ms->IEEEToBasic(&f, ret);
				memmove(fxi.time, (LPCSTR)ret, 4);

				fxis.push_back(fxi);


			}
			
			if (fxis.size() == 0) {
				std::string a = buffer2;
				a.append(" yuklenemedi...");
				SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_INSERTSTRING, 0, (LPARAM)a.c_str());
				yuklenemeyenler.push_back(buffer2);
				delete ms;

				fxis.clear();
				incsv.close();
				DeleteFile(buffer2);
				continue;

			}

			std::string dir = curdir;
	
				dir.append("\\1DAKIKA");
			
			SetCurrentDirectory(dir.c_str());
			ms->WriteSecwithData(buffer2, fxs,fxis,TRUE);
			SetCurrentDirectory(curdir);
			delete ms;

			fxis.clear();
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
	
	
	//if(1==SendDlgItemMessage(hwndDlg, IDC_GUNLUK, BM_GETCHECK, 0, 0))	MessageBox(hwndDlg, "G�nl�k Periyot secilmis!", 0, MB_OK);
		
	


	

	char buffer[250];
	std::string sLine = "";
	std::ifstream incsv;


	char buffer2[255];
	int length2 = SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETCOUNT, 0, 0);

	for (size_t i = 0; i < length2; i++)
	{
		//http://www.google.com/finance/getprices?&i=60&p=1d&q=XU030        60=SAN�YE 1D=1G�NL�K tarih ay�klay�c�=A1/86400 + 25569 + (5.5/24)
		SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETTEXT, i, (LPARAM)buffer2);
		sprintf(buffer, "http://www.google.com/finance/historical?output=csv&startdate=Mar+21%,+2011&enddate=&q=%s", buffer2);
		if (S_OK == URLDownloadToFile(NULL, buffer, buffer2, 0, NULL)) {
			//"Ok";



			ms = new Metastock;
			incsv.open(buffer2);
			getline(incsv, sLine);//ilk sat�r� al
			while (!incsv.eof())
			{
				
				getline(incsv, sLine);
				if (incsv.eof())break;
				char sday[12], smonth[12], syear[12], sopen[12], shigh[12], slow[12], sclose[12], svol[12];
				sscanf(sLine.c_str(), "%[^-]-%[^-]-%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", sday, smonth, syear, sopen, shigh, slow, sclose, svol);

				unsigned char ret[255];
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

				float f = atof(date.c_str());
				ms->IEEEToBasic(&f, ret);
				memmove(fx.date, (LPCSTR)ret, 4);

				float ratio=1.0;
				//for (std::vector<int>::size_type i = 0; i != bolunmes.size(); i++) 
				//{
				//	/* std::cout << someVector[i]; ... */
				//	if (0 == lstrcmp(bolunmes[i].shisse, buffer2)) {
				//		int k= atoi(bolunmes[i].sdate) - 19000000-1;
				//		int l = atoi(date.c_str());
				//		if (k >= l){
				//			ratio = ratio*atof(bolunmes[i].soran);
				//			continue;
				//		}

				//	}
				//}
				
				f = atof(sopen)*ratio;
				ms->IEEEToBasic(&f, ret);
				memmove(fx.open, (LPCSTR)ret, 4);

				f = atof(shigh)*ratio;
				ms->IEEEToBasic(&f, ret);
				memmove(fx.high, (LPCSTR)ret, 4);

				f = atof(slow)*ratio;
				ms->IEEEToBasic(&f, ret);
				memmove(fx.low, (LPCSTR)ret, 4);

				f = atof(sclose)*ratio;
				ms->IEEEToBasic(&f, ret);
				memmove(fx.close, (LPCSTR)ret, 4);

				f = atof(svol)*ratio;
				ms->IEEEToBasic(&f, ret);
				memmove(fx.volume, (LPCSTR)ret, 4);


			
				fxs.push_back(fx);
				

			}
			std::reverse(fxs.begin(), fxs.end());    // 9 8 7 6 5 4 3 2 1
	
		if(fxs.size()==0) {
			std::string a = buffer2;
			a.append(" yuklenemedi...");
			SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_INSERTSTRING, 0, (LPARAM)a.c_str());
			yuklenemeyenler.push_back(buffer2);
			delete ms;

			fxs.clear();
			incsv.close();
			DeleteFile(buffer2);
			continue;

		}

				std::string dir = curdir;
				
					dir.append("\\GUNLUK");
			
					SetCurrentDirectory(dir.c_str());
				
				ms->WriteSecwithData(buffer2, fxs,fxis,FALSE );
				SetCurrentDirectory(curdir);
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