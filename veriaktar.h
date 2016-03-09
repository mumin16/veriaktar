#pragma once
struct MASTER {
	char fx[1];
	char unknown1[6];
	char name[16];
	char unknown2[2];
	char fdate[4];//YYMMDD float cvs
	char ldate[4];//YYMMDD float cvs
	char unknown3[3];
	char symbol[14];
	char unknown4[3];
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
					MASTER master= { 0 };
					master.fx[0] = 0;
					memcpy(master.symbol, buffer2,14);
					memcpy(master.name, buffer2, 16);

					char Master[] = "This is some test data to write to the file.";
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