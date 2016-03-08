#pragma once
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


	SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_RESETCONTENT, 0, 0);


	std::string sLine = "";
	std::ifstream ptxt;
	ptxt.open(ofn.lpstrFile);
	while (!ptxt.eof())
	{
		getline(ptxt, sLine);
		
		if(0!=sLine.length())SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_ADDSTRING, 0, (LPARAM)sLine.c_str());

	}
	ptxt.close();

}

void PortfoyKaydet(_In_ HWND   hwndDlg) {

	char buffer[255];
	int length = SendDlgItemMessage(hwndDlg, IDC_SYMBOL2, LB_GETCOUNT, 0, 0);

	if(0== length){
	MessageBox(hwndDlg, "Portfoy Kaydedilecek Hiç sembol yok", 0, MB_OK);
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