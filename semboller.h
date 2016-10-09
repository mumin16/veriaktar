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
	/*
	if (S_OK != URLDownloadToFile(NULL, "https://raw.githubusercontent.com/mumin16/veriaktar/master/SEMBOLLER.txt", "SEMBOLLER.txt", 0, NULL)) {
		SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_ADDSTRING, 0, (LPARAM)"Sembol listesi indirilemedi!");
	}
	*/
	incsv.open("SEMBOLLER.txt");
	if (!incsv)return;
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

	SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_SETCURSEL, 0, 0);
	for (std::vector<int>::size_type j = 0; j != sembollers[0].listedekisemboller.size(); j++) {
		SendDlgItemMessage(hwndDlg, IDC_SYMBOL1, LB_ADDSTRING, 0, (LPARAM)sembollers[0].listedekisemboller[j].c_str());

	}
	
}

DWORD WINAPI ThreadProcSemboller(
	_In_ LPVOID lpParameter
	) {
	Semboller((HWND)lpParameter);

	return 0;
}



void PortfoyYukle(_In_ HWND   hwndDlg) {


	OPENFILENAME ofn = { 0 };
	char szSaveFileName[MAX_PATH] = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwndDlg;
	CHAR currentDirectory[1024];
	GetCurrentDirectory(1024, currentDirectory);
	ofn.lpstrInitialDir = currentDirectory;
	ofn.lpstrFilter = "Portfoy Text (*.Ptxt)\0*.Ptxt";
	ofn.lpstrFile = szSaveFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = "Ptxt";
	if (0 == GetOpenFileName(&ofn))return;


	SendDlgItemMessage(hwndDlg, IDC_SYMBOL1, LB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_RESETCONTENT, 0, 0);


	std::string sLine = "";
	std::ifstream ptxt;
	ptxt.open(ofn.lpstrFile);
	while (!ptxt.eof())
	{
		getline(ptxt, sLine);

		if (0 != sLine.length()) {
			SendDlgItemMessage(hwndDlg, IDC_SYMBOL1, LB_ADDSTRING, 0, (LPARAM)sLine.c_str());
			SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_ADDSTRING, 0, (LPARAM)sLine.c_str());
		}
			

	}
	ptxt.close();

}

void PortfoyKaydet(_In_ HWND   hwndDlg) {

	char buffer[255];
	int length = SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETCOUNT, 0, 0);

	if (0 == length) {
		MessageBox(hwndDlg, "Portfoye Kaydedilecek hic sembol yok", 0, MB_OK);
		return;
	}

	OPENFILENAME ofn = { 0 };
	char szSaveFileName[MAX_PATH] = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwndDlg;
	CHAR currentDirectory[1024];
	GetCurrentDirectory(1024, currentDirectory);
	ofn.lpstrInitialDir = currentDirectory;
	ofn.lpstrFilter = "Portfoy Text (*.Ptxt)\0*.Ptxt";
	ofn.lpstrFile = szSaveFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = "Ptxt";
	if (0 == GetSaveFileName(&ofn))return;

	std::ofstream ptxt;
	ptxt.open(ofn.lpstrFile);
	for (size_t i = 0; i < length; i++)
	{
		SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETTEXT, i, (LPARAM)buffer);
		ptxt << buffer << std::endl;

	}
	ptxt.close();

}



void ComboDegisti(_In_ HWND   hwndDlg) {
	SendDlgItemMessage(hwndDlg, IDC_SYMBOL1, LB_RESETCONTENT, 0, 0);
	char c[256];
	GetDlgItemText(hwndDlg, IDC_COMBO1, c, 256);


	//SendDlgItemMessage(hwndDlg, IDC_SYMBOL1, LB_ADDSTRING, 0, (LPARAM)sLine.c_str());


	for (std::vector<int>::size_type i = 0; i != sembollers.size(); i++) {
		/* std::cout << someVector[i]; ... */
		if (0 == strcmp(sembollers[i].listeismi, c)) {
			for (std::vector<int>::size_type j = 0; j != sembollers[i].listedekisemboller.size(); j++) {
				SendDlgItemMessage(hwndDlg, IDC_SYMBOL1, LB_ADDSTRING, 0, (LPARAM)sembollers[i].listedekisemboller[j].c_str());

			}
		}

	}

}
void Ekle(_In_ HWND   hwndDlg) {
	char buffer[255];
	int cursel = SendDlgItemMessage(hwndDlg, IDC_SYMBOL1, LB_GETCURSEL, 0, 0);
	if (cursel == LB_ERR)return;
	SendDlgItemMessage(hwndDlg, IDC_SYMBOL1, LB_GETTEXT, cursel, (LPARAM)buffer);

	char buffer2[255];
	int length = SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETCOUNT, 0, 0);

	for (size_t i = 0; i < length; i++)
	{
		SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETTEXT, i, (LPARAM)buffer2);
		if (lstrcmp(buffer, buffer2) == 0) 	return;

	}
	SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_ADDSTRING, 0, (LPARAM)buffer);
}
void HepsiniEkle(_In_ HWND   hwndDlg) {


	int length = SendDlgItemMessage(hwndDlg, IDC_SYMBOL1, LB_GETCOUNT, 0, 0);

	char buffer[255];

	for (size_t i = 0; i < length; i++)
	{

		SendDlgItemMessage(hwndDlg, IDC_SYMBOL1, LB_GETTEXT, i, (LPARAM)buffer);

		char buffer2[255];
		int length2 = SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETCOUNT, 0, 0);

		for (size_t i = 0; i < length2; i++)
		{
			SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETTEXT, i, (LPARAM)buffer2);
			if (lstrcmp(buffer, buffer2) == 0) break;

		}

		if (lstrcmp(buffer, buffer2) == 0) continue;
		SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_ADDSTRING, 0, (LPARAM)buffer);
		//SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_ADDSTRING, 0, (LPARAM)buffer);

	}

}



void Cikar(_In_ HWND   hwndDlg) {
	int cursel = SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETCURSEL, 0, 0);
	if (cursel == LB_ERR)return;
	SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_DELETESTRING, cursel, 0);
}
void HepsiniCikar(_In_ HWND   hwndDlg) {

	SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_RESETCONTENT, 0, 0);
}