#pragma once
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
			fxs.data(),      // start of data to write
			fxs.size() * 28,  // number of bytes to write
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
		lstrcpyn(master.fdate, fxs.front().date, 5);
		lstrcpyn(master.ldate, fxs.back().date, 5);
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