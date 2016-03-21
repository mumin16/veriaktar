#pragma once
INT_PTR CALLBACK AyarlarDialogProc(_In_ HWND   hwndDlg, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG: 
	{
		PostMessage(GetDlgItem(hwndDlg, IDC_PGUNCELLEME), BM_SETCHECK, BST_CHECKED, 0);
		PostMessage(GetDlgItem(hwndDlg, IDC_SGUNCELLEME), BM_SETCHECK, BST_CHECKED, 0);
		PostMessage(GetDlgItem(hwndDlg, IDC_BGUNCELLEME), BM_SETCHECK, BST_CHECKED, 0);
		SetDlgItemInt(hwndDlg, IDC_SUGUNLUK, 15, 0);
		SendDlgItemMessage(hwndDlg, IDC_SUDAKIKALIK, CB_ADDSTRING, 0, (LPARAM)"1");
		SendDlgItemMessage(hwndDlg, IDC_SUDAKIKALIK, CB_ADDSTRING, 0, (LPARAM)"5");
		SendDlgItemMessage(hwndDlg, IDC_SUDAKIKALIK, CB_ADDSTRING, 0, (LPARAM)"15");
		SendDlgItemMessage(hwndDlg, IDC_SUDAKIKALIK, CB_ADDSTRING, 0, (LPARAM)"30");
		SendDlgItemMessage(hwndDlg, IDC_SUDAKIKALIK, CB_ADDSTRING, 0, (LPARAM)"60");
		SendDlgItemMessage(hwndDlg, IDC_SUDAKIKALIK, CB_SETCURSEL, 0, 0);
		SetDlgItemInt(hwndDlg, IDC_ILKTARIH, 21032011, 0);

		break;
	}
	case WM_CLOSE:
	{
		EndDialog(hwndDlg, 0);
		break;
	}
	}

	return false;
}
int CALLBACK Ayarlar(_In_ HWND   hwndDlg) {
	return DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hwndDlg, AyarlarDialogProc, NULL);
}