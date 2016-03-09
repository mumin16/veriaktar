#pragma once
#include <bitset>
struct MASTERHEADER {
	char totalfx[1]={0};
	char unknown[1] = { 0 };
	char _totalfx[1] = { 0 };
	char _unknown[50] = { 0 };
}tagMASTERHEADER;
struct MASTER {
	char fx[1] = { 0 };
	char unknown[6] = { 0 };
	char secname[16] = { 0x20 };
	char _unknown[2] = { 0 };
	char fdate[4] = { 0 };//YYMMDD float cvs  YY=yyyy- 1900 
	char ldate[4] = { 0 };//YYMMDD float cvs 
	char period[1] = { 'D' };//'I', 'D'
	char timeframe[2] = { 0 };//0 and 60 minutes
	char secsymbol[14] = { 0x20 };
	char __unknown[3] = { 0 };
}tagMASTER;

void VeriYaz(_In_ HWND   hwndDlg,_In_ const char* dir) {
}
void VeriIndir(_In_ HWND   hwndDlg) {
	char curdir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, curdir);


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

				if (0 != SendDlgItemMessage(hwndDlg, IDC_GUNLUK, BM_GETCHECK, 0, 0))
				{
				
					std::string dir = curdir;
					dir.append("\\GUNLUK");
					SetCurrentDirectory(dir.c_str());
				}

				HANDLE hFile = CreateFile("MASTER",               // file to open
					GENERIC_READ,          // open for reading
					FILE_SHARE_READ,       // share for reading
					NULL,                  // default security
					OPEN_EXISTING,         // existing file only
					FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
					NULL);                 // no attr. template

				if (hFile == INVALID_HANDLE_VALUE)
				{
					hFile = CreateFile("MASTER",                // name of the write
						GENERIC_WRITE,          // open for writing
						0,                      // do not share
						NULL,                   // default security
						CREATE_NEW,             // create new file only
						FILE_ATTRIBUTE_NORMAL,  // normal file
						NULL);                  // no attr. template
					DWORD dwBytesWritten = 0;
					MASTERHEADER masterheader;
					masterheader.totalfx[0] = 1;
					masterheader._totalfx[0] = 1;
			
					WriteFile(
						hFile,           // open file handle
						&masterheader,      // start of data to write
						sizeof(masterheader),  // number of bytes to write
						&dwBytesWritten, // number of bytes that were written
						NULL);

			
					MASTER master;
					master.fx[0] = 1;
					memcpy(master.secname, buffer2, lstrlen(buffer2));
					memcpy(master.secsymbol, buffer2, lstrlen(buffer2));
					
					unsigned char ret[255];
					float f = 1160309;//
					IEEEToBasic(&f, ret);
					

					WriteFile(
						hFile,           // open file handle
						&master,      // start of data to write
						sizeof(master),  // number of bytes to write
						&dwBytesWritten, // number of bytes that were written
						NULL);

				}
				else {
				
				}
				//if (0 != SendDlgItemMessage(hwndDlg, IDC_1DAKIKA, BM_GETCHECK, 0, 0))
					//VeriYaz(hwndDlg, "1DAK");
				
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