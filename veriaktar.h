#pragma once

//MessageBox(hwndDlg, GetLastErrorAsString().c_str(), 0, 0);
//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

void VeriYaz(_In_ HWND   hwndDlg, _In_  LPTSTR symbol) {
	HANDLE hMFile=0, hFXFile=0;
	DWORD dwBytesWritten = 0;

	if (0 != SendDlgItemMessage(hwndDlg, IDC_GUNLUK, BM_GETCHECK, 0, 0))
	{

		std::string dir = curdir;
		dir.append("\\GUNLUK");
		SetCurrentDirectory(dir.c_str());
	}

	hMFile = CreateFile("MASTER",               // file to open
		GENERIC_READ | GENERIC_WRITE,          // open for reading
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL , // normal file
		NULL);                 // no attr. template
	
	if (hMFile == INVALID_HANDLE_VALUE)
	{

		hFXFile = CreateFile("F1.DAT",                // name of the write
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

		hMFile = CreateFile("MASTER",                // name of the write
			GENERIC_READ | GENERIC_WRITE,          // open for writing
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,                      // do not share
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

	else
	{
		//SetFilePointer(hMFile, 0, 0, FILE_BEGIN);
		ReadFile(hMFile, &masterheader, sizeof(masterheader), &dwBytesWritten, NULL);
		
		// Check for eof.
		ReadFile(hMFile, &master, sizeof(master), &dwBytesWritten, NULL);
		while (dwBytesWritten != 0)
		{

			
			if (0 == lstrcmp(symbol, master.secsymbol))
			{
				MessageBox(hwndDlg, "eslesti", 0, 0);
				CloseHandle(hMFile);
				CloseHandle(hFXFile);
				SetCurrentDirectory(curdir);
				return;
			}
			ReadFile(hMFile, &master, sizeof(master), &dwBytesWritten, NULL);
			
		} 

		//eþleþen yok
		if (masterheader.totalfx[0] == 0xff) {
			//xmastera bak



			//yoksa xmastera ekle
		}
		else {
		//yeni ekle mastera
		
		}
		
		

	}

	








	CloseHandle(hMFile);
	CloseHandle(hFXFile);
	SetCurrentDirectory(curdir);
}