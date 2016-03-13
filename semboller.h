#pragma once
struct SEMBOLLER
{
	char listeismi[16];
	std::vector<std::string> listedekisemboller;
}tagSEMBOLLER;

SEMBOLLER semboller;
std::vector <SEMBOLLER> sembollers;
void Semboller(_In_ HWND   hwndDlg) {
	std::string sLine = "";
	std::ifstream incsv;

	if (S_OK != URLDownloadToFile(NULL, "https://raw.githubusercontent.com/mumin16/veriaktar/master/SEMBOLLER.txt", "SEMBOLLER.txt", 0, NULL)) {
		SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_ADDSTRING, 0, (LPARAM)"Sembol listesi indirilemedi!");
	}
	incsv.open("SEMBOLLER.txt");
	getline(incsv, sLine);
	while (!incsv.eof())
	{
			
			if (0 == strncmp(sLine.c_str(), "#", 1)) {

				strcpy(semboller.listeismi, sLine.c_str()+1);
				getline(incsv, sLine);
				while ( 0 != strncmp(sLine.c_str(), "#", 1)){

					
					semboller.listedekisemboller.push_back(sLine.c_str());
					getline(incsv, sLine);
					if (incsv.eof())break;
				}
				
			}
		
	sembollers.push_back(semboller);
	semboller={0};

	}



	for (std::vector<int>::size_type i = 0; i != sembollers.size(); i++) {
		/* std::cout << someVector[i]; ... */
		SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)sembollers[i].listeismi);
	}
	
}

DWORD WINAPI ThreadProcSemboller(
	_In_ LPVOID lpParameter
	) {
	Semboller((HWND)lpParameter);

	return 0;
}