#pragma once
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
	

	int length =SendDlgItemMessage(hwndDlg, IDC_SYMBOL1, LB_GETCOUNT, 0, 0);
	
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