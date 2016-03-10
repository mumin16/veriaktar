#pragma once
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
	char totalfx[1]={0};
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

int totalrecord=0;
bool lastrecord=FALSE;

void VeriYaz(_In_ HWND   hwndDlg, _In_  LPTSTR symbol) {
	HANDLE hMFile, hFXFile;
	DWORD dwBytesWritten = 0;

	if (0 != SendDlgItemMessage(hwndDlg, IDC_GUNLUK, BM_GETCHECK, 0, 0))
	{

		std::string dir = curdir;
		dir.append("\\GUNLUK");
		SetCurrentDirectory(dir.c_str());
	}

	hMFile = CreateFile("MASTER",               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
		NULL);                 // no attr. template

	if (hMFile == INVALID_HANDLE_VALUE)
	{

		hFXFile = CreateFile("F1.DAT",                // name of the write
			GENERIC_WRITE,          // open for writing
			0,                      // do not share
			NULL,                   // default security
			CREATE_NEW,             // create new file only
			FILE_ATTRIBUTE_NORMAL,  // normal file
			NULL);                  // no attr. template

		

		WriteFile(
			hFXFile,           // open file handle
			&fxheader,      // start of data to write
			sizeof(fxheader),  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);

		WriteFile(
			hFXFile,           // open file handle
			&fx,      // start of data to write
			sizeof(fx),  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);

		hMFile = CreateFile("MASTER",                // name of the write
			GENERIC_WRITE,          // open for writing
			0,                      // do not share
			NULL,                   // default security
			CREATE_NEW,             // create new file only
			FILE_ATTRIBUTE_NORMAL,  // normal file
			NULL);                  // no attr. template

		masterheader.totalfx[0] = 1;
		masterheader._totalfx[0] = 1;
		WriteFile(
			hMFile,           // open file handle
			&masterheader,      // start of data to write
			sizeof(masterheader),  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);

		master.fx[0] = 1;
		lstrcpy(master.secname, symbol);
		lstrcpy(master.secsymbol, symbol);
		lstrcpyn(master.fdate, fx.date,5);
		lstrcpyn(master.ldate, fx.date, 5);
		master.period[0] = 'I';
	


		WriteFile(
			hMFile,           // open file handle
			&master,      // start of data to write
			sizeof(master),  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);

	
	}


	//if (!hMFile)CloseHandle(hMFile);
	//if(!hFXFile)CloseHandle(hFXFile);
	SetCurrentDirectory(curdir);
}

void VeriOku(_In_ HWND   hwndDlg, _In_ const char* dir) {
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
			totalrecord = 0;
			lastrecord = FALSE;
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
			


				++totalrecord;
				f = float(totalrecord + 1);

				if(totalrecord<255)
				{ 
					fxheader.totalrecord[0] = totalrecord+1;
				}
				else
				{
					IEEEToBasic(&f, ret);
					std::reverse(ret, ret + 4);
					fxheader.totalrecord[0] = ret[1];
					fxheader.totalrecord[1] = ret[0];
				}
				



				VeriYaz(hwndDlg, buffer2);
				
				//const char * months[] = { "Jan", "Feb","Mar", "Apr","May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
				//for (size_t i = 0; i < 11; i++)
				//{
				

				//}

				
			}

			lastrecord = TRUE;
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