#pragma once
void VeriAktar(_In_ HWND   hwndDlg) {
	CreateDirectory("GUNLUK", NULL);
	char buffer[250];
	std::string sLine = "";
	std::ifstream infile;
	std::ifstream incsv;

	char* str;

	infile.open("BIST30.txt");

	while (!infile.eof())
	{
		getline(infile, sLine);
		SendDlgItemMessage(hwndDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)sLine.c_str());
		sprintf(buffer, "http://www.google.com/finance/historical?output=csv&q=%s", sLine.c_str());
		if (S_OK == URLDownloadToFile(NULL, buffer, sLine.c_str(), 0, NULL)) {
			//"Ok";
		}
		else {
			MessageBox(hwndDlg, "URLDownloadToFile error", 0, MB_OK);
			return;

		}

		incsv.open(sLine.c_str());
		getline(incsv, sLine);//ilk satýrý al
		while (!incsv.eof())
		{
			getline(incsv, sLine);
			char sday[12], smonth[12], syear[12], sopen[12], shigh[12], slow[12], sclose[12], svol[12];
			sscanf(sLine.c_str(), "%[^-]-%[^-]-%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", sday, smonth, syear, sopen, shigh, slow, sclose, svol);

			const char * months[] = { "Jan", "Feb","Mar", "Apr","May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
			for (size_t i = 0; i < 11; i++)
			{
				//MessageBox(hwndDlg, months[i],0, MB_OK);

			}


			//atof(sopen);
			//****
			//atoi(svol);

			//MessageBox(hwndDlg, sLine.c_str(), 0, MB_OK);
		}
		incsv.close();
		DeleteFile(sLine.c_str());
	}


	infile.close();


}