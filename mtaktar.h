#pragma once
#include <shlobj.h>
template<class T>
T remove_extension(T const & filename)
{
	typename T::size_type const p(filename.find_last_of('.'));
	return p > 0 && p != T::npos ? filename.substr(0, p) : filename;
}

void mtaktar(_In_ HWND   hwndDlg) {


	OPENFILENAME ofn = { 0 };
	char szSaveFileName[MAX_PATH] = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwndDlg;
	CHAR currentDirectory[1024];
	SHGetFolderPath(NULL, CSIDL_DESKTOP, NULL, 0, currentDirectory);
	ofn.lpstrInitialDir = currentDirectory;
	ofn.lpstrFilter = "MetaTrader CSV (*.csv)\0*.csv";
	ofn.lpstrFile = szSaveFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = "cdv";
	if (0 == GetOpenFileName(&ofn))return;

	ms = new Metastock;
	std::string sLine = "";
	std::ifstream ptxt;
	ptxt.open(ofn.lpstrFile);

	while (!ptxt.eof())
	{
		getline(ptxt, sLine);
		char syear[12], smonth[12], sday[12], shour[12], sminute[12], sopen[12], shigh[12], slow[12], sclose[12], svol[12];
		sscanf(sLine.c_str(), "%[^.].%[^.].%[^,],%[^:]:%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", syear, smonth, sday, shour, sminute, sopen, shigh, slow, sclose, svol);

		unsigned char ret[255];
		float f = atof(sopen);
		ms->IEEEToBasic(&f, ret);
		memmove(fxi.open, (LPCSTR)ret, 4);

		f = atof(shigh);
		ms->IEEEToBasic(&f, ret);
		memmove(fxi.high, (LPCSTR)ret, 4);

		f = atof(slow);
		ms->IEEEToBasic(&f, ret);
		memmove(fxi.low, (LPCSTR)ret, 4);

		f = atof(sclose);
		ms->IEEEToBasic(&f, ret);
		memmove(fxi.close, (LPCSTR)ret, 4);

		f = atof(svol);
		ms->IEEEToBasic(&f, ret);
		memmove(fxi.volume, (LPCSTR)ret, 4);


		int iyear = atoi(syear) - 1900;
		std::string date = std::to_string(iyear);
		date.append(smonth);
		date.append(sday);

		std::string time = shour;
		time.append(sminute);
		time.append("00");

		f = atof(date.c_str());
		ms->IEEEToBasic(&f, ret);
		memmove(fxi.date, (LPCSTR)ret, 4);


		f = atof(time.c_str());
		ms->IEEEToBasic(&f, ret);
		memmove(fxi.time, (LPCSTR)ret, 4);

		fxis.push_back(fxi);
	}
	ptxt.close();

	std::string dir = curdir;

	dir.append("\\METATRADER");

	if (fxis.size() != 0) {
		SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_INSERTSTRING, 0, (LPARAM)"Metatrader aktarimi basarili!");
	}
	else
	{
		SendDlgItemMessage(hwndDlg, IDC_BILGI, LB_INSERTSTRING, 0, (LPARAM)"Metatrader aktarimi basarisiz!");
	}
	
	if(fxis.size()>65500)fxis.erase(fxis.begin(), fxis.end()-65500);

		std::string fullpathnamewithfilenameandext = ofn.lpstrFile;
		std::string filenameandext = fullpathnamewithfilenameandext.substr(fullpathnamewithfilenameandext.find_last_of("/\\") + 1);
		std::string filename=remove_extension(filenameandext);
		
		SetCurrentDirectory(dir.c_str());
		ms->WriteSecwithData((char*)filename.c_str(), fxs, fxis,TRUE,1);
		SetCurrentDirectory(curdir);
		delete ms;

		fxis.clear();

		

	}
