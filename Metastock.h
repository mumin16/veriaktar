#pragma once
struct MASTERHEADER {
	unsigned char totalfx[1] = { 0 };
	char unknown[1] = { 0 };
	unsigned char _totalfx[1] = { 0 };
	char _unknown[50] = { 0 };
}tagMASTERHEADER;

struct MASTER {
	unsigned char fx[1] = { 0 };
	char unknown[2] = { 0 };
	char bune[1] = { 0x1c };//7kolonsayisi*4
	char kolonsayisi[1] = { 7 };//date-time-o-h-l-c-v
	char ___unknown[2] = { 0 };
	char secname[16] = { 0x20 };
	char _unknown[2] = { 0 };
	char fdate[4] = { 0 };//YYMMDD ieee  YY=yyyy- 1900 
	char ldate[4] = { 0 };//YYMMDD ieee 
	char period[1] = { 'I' };//'I', 'D'
	char timeframe[2] = { 1 };//0 and 60 minutes
	char secsymbol[14] = { 0x20 };
	char __unknown[3] = { 0 };
}tagMASTER;

struct XMASTERHEADER {
	unsigned char unknown[18] = { 0 };
	unsigned char totalfx_datmwd[2] = { 0 };//tersi
	unsigned char _unknown[130] = { 0 };
}tagXMASTERHEADER;

struct XMASTER {
	unsigned char reserved1[1] = { 1 };
	char secsymbol[15] = { 0 };
	char secname[46] = { 0 };
	char period[1] = { 'I' };//'I', 'D'
	char timeframe[2] = { 1 };//0 and 60 minutes
	unsigned char fx[2] = { 0 };//TERSÝ
	unsigned char _unknown[3] = { 0 };
	unsigned char __unknown[1] = { 0xBF };//3d daily
	unsigned char ___unknown[33] = { 0 };
	char fdate[4] = { 0 };//TERSÝ 20160229
	char _fdate[4] = { 0 };//TERSÝ 20160229
	unsigned char ____unknown[4] = { 0 };
	char ldate[4] = { 0 };//TERSÝ 20160229
	unsigned char _____unknown[30] = { 0 };
}tagXMASTER;

struct FXHEADER {
	char unkown[2] = { 0 };
	char totalrecord[2] = { 0 };//ters+1 ieee
	char _unkown[24] = { 0 };
}tagFXHEADER;

struct FX {
	char date[4] = { 0 };//YYMMDD ieee  YY=yyyy- 1900 
	char time[4] = { 0 };//2359000 ieee
	char open[4] = { 0 };//ieee
	char high[4] = { 0 };//ieee
	char low[4] = { 0 };//ieee
	char close[4] = { 0 };//ieee
	char volume[4] = { 0 };//ieee

}tagFX;

class Metastock
{
public:
	float BasicToIEEE(unsigned char *value)

	{
		float result;

		unsigned char *msbin = (unsigned char *)value;

		unsigned char *ieee = (unsigned char *)&result;

		unsigned char sign = 0x00;

		unsigned char ieee_exp = 0x00;

		int i;

		/* MS Binary Format */

		/* byte order => m3 | m2 | m1 | exponent */

		/* m1 is most significant byte => sbbb|bbbb */

		/* m3 is the least significant byte */

		/* m = mantissa byte */

		/* s = sign bit */

		/* b = bit */

		sign = msbin[2] & 0x80; /* 1000|0000b */

								/* IEEE Single Precision Float Format */

								/* m3 m2 m1 exponent */

								/* mmmm|mmmm mmmm|mmmm emmm|mmmm seee|eeee */

								/* s = sign bit */

								/* e = exponent bit */

								/* m = mantissa bit */

		for (i = 0; i<4; i++) ieee[i] = 0;

		/* any msbin w/ exponent of zero = zero */

		if (msbin[3] == 0) return 0;

		ieee[3] |= sign;

		/* MBF is bias 128 and IEEE is bias 127. ALSO, MBF places */

		/* the decimal point before the assumed bit, while */

		/* IEEE places the decimal point after the assumed bit. */

		ieee_exp = msbin[3] - 2; /* actually, msbin[3]-1-128+127 */

								 /* the first 7 bits of the exponent in ieee[3] */

		ieee[3] |= ieee_exp >> 1;

		/* the one remaining bit in first bin of ieee[2] */

		ieee[2] |= ieee_exp << 7;

		/* 0111|1111b : mask out the msbin sign bit */

		ieee[2] |= msbin[2] & 0x7f;

		ieee[1] = msbin[1];

		ieee[0] = msbin[0];

		return (result);

	}

	bool IEEEToBasic(float *value, unsigned char *result)

	{
		unsigned char *ieee = (unsigned char *)value;

		unsigned char *msbin = (unsigned char *)result;

		unsigned char sign = 0x00;

		unsigned char msbin_exp = 0x00;

		int i;

		/* See _fmsbintoieee() for details of formats */

		sign = ieee[3] & 0x80;

		msbin_exp |= ieee[3] << 1;

		msbin_exp |= ieee[2] >> 7;

		/* An ieee exponent of 0xfe overflows in MBF */

		if (msbin_exp == 0xfe) return (FALSE);

		msbin_exp += 2; /* actually, -127 + 128 + 1 */

		for (i = 0; i<4; i++) msbin[i] = 0;

		msbin[3] = msbin_exp;

		msbin[2] |= sign;

		msbin[2] |= ieee[2] & 0x7f;

		msbin[1] = ieee[1];

		msbin[0] = ieee[0];

		return (TRUE);

	}
private:

	HANDLE hMFile = 0, hXMFile = 0, hFXFile = 0;
	DWORD dwBytesWritten = 0;


	void FxYarat(const char* filename, std::vector<FX> fxs) {
		FXHEADER fxheader;
		FX fx;

		hFXFile = CreateFile(filename,                // name of the write
			GENERIC_READ | GENERIC_WRITE,          // open for writing
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,                      // do not share
			NULL,                   // default security
			CREATE_NEW,             // create new file only
			FILE_ATTRIBUTE_NORMAL,  // normal file
			NULL);                  // no attr. template

		if (fxs.size()<255)
		{
			fxheader.totalrecord[0] = fxs.size() + 1;
		}
		else
		{
			unsigned char ret[255];
			float f = fxs.size() + 1;
			IEEEToBasic(&f, ret);
			std::reverse(ret, ret + 4);
			fxheader.totalrecord[0] = ret[1];
			fxheader.totalrecord[1] = ret[0];
		}
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


		CloseHandle(hFXFile);
	
	}
	void OpenReadWrite(char* symbolname, std::vector<FX> fxs) {
		MASTERHEADER masterheader;
		MASTER master;
		XMASTERHEADER xmasterheader;
		XMASTER xmaster;

		// master aç
		hMFile = CreateFile("MASTER", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		//master yok
		if (hMFile == INVALID_HANDLE_VALUE)
		{
			hMFile = CreateFile("MASTER",                // name of the write
				GENERIC_READ | GENERIC_WRITE,          // open for writing
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,                      // do not share
				NULL,                   // default security
				CREATE_NEW,             // create new file only
				FILE_ATTRIBUTE_NORMAL,  // normal file
				NULL);                  // no attr. template

			masterheader._totalfx[0] = masterheader.totalfx[0] = 1;
			WriteFile(
				hMFile,           // open file handle
				&masterheader,      // start of data to write
				sizeof(masterheader),  // number of bytes to write
				&dwBytesWritten, // number of bytes that were written
				NULL);

			master.fx[0] = 1;
			memmove(master.secname, symbolname,16);
			memmove(master.secsymbol, symbolname,14);
			memmove(master.fdate, fxs.front().date, 4);
			memmove(master.ldate, fxs.back().date, 4);
			WriteFile(
				hMFile,           // open file handle
				&master,      // start of data to write
				sizeof(master),  // number of bytes to write
				&dwBytesWritten, // number of bytes that were written
				NULL);
			FxYarat("F1.DAT",fxs);
			CloseHandle(hMFile);
			return;
		}
		//master var
		else
		{
			ReadFile(hMFile, &masterheader, sizeof(masterheader), &dwBytesWritten, NULL);
			ReadFile(hMFile, &master, sizeof(master), &dwBytesWritten, NULL);
			// Check for eof.
			while (dwBytesWritten != 0)
			{
				if (0 == strncmp(symbolname, master.secsymbol, lstrlen(symbolname)))
				{
					SetFilePointer(hMFile, -sizeof(MASTER), 0, FILE_CURRENT);
					memmove(master.fdate, fxs.front().date, 4);
					memmove(master.ldate, fxs.back().date, 4);
					WriteFile(
						hMFile,           // open file handle
						&master,      // start of data to write
						sizeof(master),  // number of bytes to write
						&dwBytesWritten, // number of bytes that were written
						NULL);
					std::string a = "F" + std::to_string(master.fx[0]) + ".DAT";
					FxYarat(a.c_str(), fxs);
					CloseHandle(hMFile);
					return;
				}
				ReadFile(hMFile, &master, sizeof(master), &dwBytesWritten, NULL);
			}
			if (masterheader.totalfx[0] != 0xff) {
					SetFilePointer(hMFile, 0, 0, FILE_BEGIN);
					masterheader._totalfx[0] = masterheader.totalfx[0] = masterheader.totalfx[0] + 1;
					WriteFile(
						hMFile,           // open file handle
						&masterheader,      // start of data to write
						sizeof(masterheader),  // number of bytes to write
						&dwBytesWritten, // number of bytes that were written
						NULL);
					SetFilePointer(hMFile, 0, 0, FILE_END);
					master.fx[0] = masterheader.totalfx[0];
					memmove(master.secname, symbolname, lstrlen(symbolname));
					memmove(master.secsymbol, symbolname, lstrlen(symbolname));
					memmove(master.fdate, fxs.front().date, 4);
					memmove(master.ldate, fxs.back().date, 4);
					WriteFile(
						hMFile,           // open file handle
						&master,      // start of data to write
						sizeof(master),  // number of bytes to write
						&dwBytesWritten, // number of bytes that were written
						NULL);
					std::string a= "F"+std::to_string(master.fx[0])+".DAT";
					FxYarat(a.c_str(), fxs);
					CloseHandle(hMFile);
					return;
			}
			CloseHandle(hMFile);


				//xmastera bak
				// xmaster aç
				hXMFile = CreateFile("XMASTER", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				//xmaster yok
				if (hXMFile == INVALID_HANDLE_VALUE)
				{

					hXMFile = CreateFile("XMASTER",                // name of the write
						GENERIC_READ | GENERIC_WRITE,          // open for writing
						FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,                      // do not share
						NULL,                   // default security
						CREATE_NEW,             // create new file only
						FILE_ATTRIBUTE_NORMAL,  // normal file
						NULL);                  // no attr. template

					xmasterheader.totalfx_datmwd[1] = 256;
					WriteFile(
						hXMFile,           // open file handle
						&xmasterheader,      // start of data to write
						sizeof(xmasterheader),  // number of bytes to write
						&dwBytesWritten, // number of bytes that were written
						NULL);

					xmaster.fx[0] = 256;
					memmove(xmaster.secname, symbolname, 16);
					memmove(xmaster.secsymbol, symbolname, 14);
					memmove(xmaster.fdate, fxs.front().date, 4);
					memmove(xmaster._fdate, fxs.front().date, 4);
					memmove(xmaster.ldate, fxs.back().date, 4);
					WriteFile(
						hXMFile,           // open file handle
						&xmaster,      // start of data to write
						sizeof(xmaster),  // number of bytes to write
						&dwBytesWritten, // number of bytes that were written
						NULL);
					FxYarat("F256.MWD", fxs);
					CloseHandle(hXMFile);
					return;
				}





		//		//yoksa xmastera ekle



		//	}



		}
	}
public:

	Metastock()
	{
	}

	virtual ~Metastock()
	{
	}


	void WriteSec(char* symbolname, std::vector<FX> fxs,const char* workingdir) {
		char curdir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, curdir);
		SetCurrentDirectory(workingdir);

		OpenReadWrite(symbolname,fxs);


		SetCurrentDirectory(curdir);
	}

};

