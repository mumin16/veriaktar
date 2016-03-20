#pragma once
struct BOLUNME
{
	char shisse[12], soran[12], sdate[12];
}tagBOLUNME;

BOLUNME bolunme;
std::vector <BOLUNME> bolunmes;



void Bolunmeler(_In_ HWND   hwndDlg) {
	std::string sLine = "";
	std::ifstream incsv;

	if (S_OK != URLDownloadToFile(NULL, "https://raw.githubusercontent.com/mumin16/veriaktar/master/BOLUNMELER.txt", "BOLUNMELER.txt", 0, NULL)) {
		SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_ADDSTRING, 0, (LPARAM)"Bolunmeler indirilemedi!");
	}
		incsv.open("BOLUNMELER.txt");
		getline(incsv, sLine);//ilk satýrý al
		while (!incsv.eof())
		{
			getline(incsv, sLine);
			sscanf(sLine.c_str(), "%[^;];%[^;];%[^;]", bolunme.shisse, bolunme.soran, bolunme.sdate);
			bolunmes.push_back(bolunme);

		}
		std::reverse(bolunmes.begin(), bolunmes.end());    // 9 8 7 6 5 4 3 2 1
}

DWORD WINAPI ThreadProcBolunmeler(
	_In_ LPVOID lpParameter
	) {
	Bolunmeler((HWND)lpParameter);
	return 0;
}