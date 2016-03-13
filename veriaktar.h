#pragma once
HANDLE hMFile = 0, hXMFile = 0, hFXFile = 0;
DWORD dwBytesWritten = 0;
void xmaster_ekleguncelle(_In_  LPTSTR symbol, bool ekle) {

	if (ekle == TRUE) {
		masterheader._totalfx[0] = masterheader.totalfx[0] = masterheader.totalfx[0] + 1;
		SetFilePointer(hMFile, 0, 0, FILE_BEGIN);
		WriteFile(
			hMFile,           // open file handle
			&masterheader,      // start of data to write
			sizeof(masterheader),  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);
		SetFilePointer(hMFile, 0, 0, FILE_END);

		master.fx[0] = masterheader.totalfx[0];
		lstrcpy(master.secname, symbol);
		lstrcpy(master.secsymbol, symbol);
		memmove(master.fdate, fxs.front().date, 4);
		memmove(master.ldate, fxs.back().date, 4);


		WriteFile(
			hMFile,           // open file handle
			&master,      // start of data to write
			sizeof(master),  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);
	}


	char a[255] = "";
	int i = master.fx[0];
	itoa(i, a, 10);
	std::string dat = "F";
	dat.append(a);
	dat.append(".DAT");


	hFXFile = CreateFile(dat.c_str(),                // name of the write
		GENERIC_READ | GENERIC_WRITE,          // open for writing
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,                      // do not share
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
		fxs.data(),      // start of data to write
		fxs.size() * 28,  // number of bytes to write
		&dwBytesWritten, // number of bytes that were written
		NULL);


	CloseHandle(hMFile);
	CloseHandle(hFXFile);
	SetCurrentDirectory(curdir);
}
void master_ekleguncelle(_In_  LPTSTR symbol,bool ekle) {

	if (ekle == TRUE) {
		masterheader._totalfx[0] = masterheader.totalfx[0] = masterheader.totalfx[0] + 1;
		SetFilePointer(hMFile, 0, 0, FILE_BEGIN);
		WriteFile(
			hMFile,           // open file handle
			&masterheader,      // start of data to write
			sizeof(masterheader),  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);
		SetFilePointer(hMFile, 0, 0, FILE_END);

		master.fx[0] = masterheader.totalfx[0];
		lstrcpy(master.secname, symbol);
		lstrcpy(master.secsymbol, symbol);
		memmove(master.fdate, fxs.front().date, 4);
		memmove(master.ldate, fxs.back().date, 4);


		WriteFile(
			hMFile,           // open file handle
			&master,      // start of data to write
			sizeof(master),  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);
	}

	
		char a[255] = "";
		int i = master.fx[0];
		itoa(i, a, 10);
		std::string dat = "F";
		dat.append(a);
		dat.append(".DAT");


		hFXFile = CreateFile(dat.c_str(),                // name of the write
			GENERIC_READ | GENERIC_WRITE,          // open for writing
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,                      // do not share
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
			fxs.data(),      // start of data to write
			fxs.size() * 28,  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);


	CloseHandle(hMFile);
	CloseHandle(hFXFile);
	SetCurrentDirectory(curdir);
}



void VeriYaz(_In_ HWND   hwndDlg, _In_  LPTSTR symbol) {


	if (0 != SendDlgItemMessage(hwndDlg, IDC_GUNLUK, BM_GETCHECK, 0, 0))
	{
		std::string dir = curdir;
		dir.append("\\GUNLUK");
		SetCurrentDirectory(dir.c_str());

	}

	// master aç
	hMFile = CreateFile("MASTER",GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL ,NULL);  
	
	//master yok
	if (hMFile == INVALID_HANDLE_VALUE)
	{

				hMFile = CreateFile("MASTER",                // name of the write
					GENERIC_READ | GENERIC_WRITE,          // open for writing
					FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,                      // do not share
					NULL,                   // default security
					CREATE_NEW,             // create new file only
					FILE_ATTRIBUTE_NORMAL,  // normal file
					NULL);                  // no attr. template
				master_ekleguncelle(symbol,TRUE);
				return;
	}
	//master var
	else
	{
		//SetFilePointer(hMFile, 0, 0, FILE_BEGIN);
		ReadFile(hMFile, &masterheader, sizeof(masterheader), &dwBytesWritten, NULL);
		
		// Check for eof.
		ReadFile(hMFile, &master, sizeof(master), &dwBytesWritten, NULL);
		while (dwBytesWritten != 0)
		{

			
			if (0 == strncmp(symbol, master.secsymbol,lstrlen(symbol)))
			{
				//MessageBox(hwndDlg, "eslesti", 0, 0);
				SetFilePointer(hMFile, -sizeof(MASTER), 0, FILE_CURRENT);
				memmove(master.fdate, fxs.front().date, 4);
				memmove(master.ldate, fxs.back().date, 4);


				WriteFile(
					hMFile,           // open file handle
					&master,      // start of data to write
					sizeof(master),  // number of bytes to write
					&dwBytesWritten, // number of bytes that were written
					NULL);
				master_ekleguncelle(symbol, FALSE);
				return;
			}
			ReadFile(hMFile, &master, sizeof(master), &dwBytesWritten, NULL);
			
		} 

		//eþleþen yok
		if (masterheader.totalfx[0] == 0xff) {
			//xmastera bak

			// xmaster aç
			hXMFile = CreateFile("XMASTER", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			//xmaster yok
			if (hXMFile == INVALID_HANDLE_VALUE)
			{

				hXMFile = CreateFile("XMASTER",                // name of the write
					GENERIC_READ | GENERIC_WRITE,          // open for writing
					FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,                      // do not share
					NULL,                   // default security
					CREATE_NEW,             // create new file only
					FILE_ATTRIBUTE_NORMAL,  // normal file
					NULL);                  // no attr. template
				xmaster_ekleguncelle(symbol, TRUE);
				return;
			}





			//yoksa xmastera ekle



		}
		else {
		//yeni ekle mastera
			master_ekleguncelle(symbol,TRUE);
			return;
		}
		
		

	}

	
}



/*

ara(){

->masteryokmu

->masterdayokmu

->xmasterdayokmu
}

ekle(){

->masteraekle

->xmasteraekle
}

fxyarat(){

}
*/