#pragma once
struct MASTERHEADER {
	unsigned __int8 totalfx = 0;//max 0xff
	const char padding = 0;
	unsigned __int8 lastaddedfx = 0;//max 0xff 
	const char zero[50] = { 0 };
}tagMASTERHEADER;

struct MASTER {
	unsigned __int8 fx =  0 ;
	char unknown[2] = { 0 };
	const unsigned __int8 fieldssize=24;//7kolonsayisi*4
	const unsigned __int8 fieldsnumber = 6;//date-time-o-h-l-c-v
	char ___unknown[2] = { 0 };
	char secname[16] = { 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20 };
	char _unknown[2] = { 0 };
	char fdate[4] = { 0 };//YYMMDD ieee  YY=yyyy- 1900 
	char ldate[4] = { 0 };//YYMMDD ieee 
	char period[1] = { 'D' };//'I', 'D'
	char timeframe[2] = { 0 };//0 and 60 minutes
	char secsymbol[16] = { 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20 };
	char __unknown[1] = { 0 };
}tagMASTER;

struct XMASTERHEADER {
	unsigned char reserved[4] = { 0x5d,0xfe,0x58,0x4d };
	unsigned char unknown[6] = { 0 };
	unsigned __int16 totalfx_mwd =  0 ;//tersi
	unsigned char _unknown[2] = { 0 };
	unsigned __int16 _totalfx_mwd =  0 ;//tersi
	unsigned char __unknown[2] = { 0 };
	unsigned __int16 comingasfx_mwd = { 0 };//tersi//totalmwd+255+1
	unsigned char ___unknown[130] = { 0 };
}tagXMASTERHEADER;

#pragma pack(1)
struct XMASTER {
	char reserved1[1] = { 1 };
	char secsymbol[15] = { 0 };
	char secname[46] = { 0 };
	char period = 'D';//'I', 'D'
	unsigned __int16 timeframe = 0;//0 and 60 minutes
	unsigned __int16 fx =  0 ;//TERSÝ//256ilki
	unsigned __int32 _unknown = { 0x3f000000 };//BF intraday 3f daily
	char ___unknown[9] = { 0 };
	char notimportantfdate[4] = { 0 };//TERSÝ 20160229
	char ____unknown[20] = { 0 };
	unsigned __int32 fdate = { 0 };//TERSÝ 20160229
	unsigned __int32 _fdate = { 0 };//TERSÝ 20160229
	char _____unknown[4] = { 0 };
	unsigned __int32 ldate = { 0 };//TERSÝ 20160315
	unsigned char _______unknown[30] = { 0 };
	
}tagXMASTER;

struct FXHEADER {
	unsigned __int16 unkown =  0 ;
	unsigned __int16 totalrecord =  0 ;//ters+1 ieee deðil// +1 boþ satýr
	char _unkown[20] = { 0 };
}tagFXHEADER;

struct FX {
	char date[4] = { 0 };//YYMMDD ieee  YY=yyyy- 1900 
//	char time[4] = { 0 };//2359000 ieee
	char open[4] = { 0 };//ieee
	char high[4] = { 0 };//ieee
	char low[4] = { 0 };//ieee
	char close[4] = { 0 };//ieee
	char volume[4] = { 0 };//ieee

}tagFX;
struct FXI {
	char date[4] = { 0 };//YYMMDD ieee  YY=yyyy- 1900 
	char time[4] = { 0 };//2359000 ieee
	char open[4] = { 0 };//ieee
	char high[4] = { 0 };//ieee
	char low[4] = { 0 };//ieee
	char close[4] = { 0 };//ieee
	char volume[4] = { 0 };//ieee

}tagFXI;
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
	MASTERHEADER masterheader;
	MASTER master;
	XMASTERHEADER xmasterheader;
	XMASTER xmaster ;

	void FxYarat(const char* filename, std::vector<FX> fxs) {
		FXHEADER fxheader;
		FX fx;

		hFXFile = CreateFile(filename,                // name of the write
			GENERIC_READ | GENERIC_WRITE,          // open for writing
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,                      // do not share
			NULL,                   // default security
			CREATE_ALWAYS,             // create new file only
			FILE_ATTRIBUTE_NORMAL,  // normal file
			NULL);                  // no attr. template

	
		

			fxheader.totalrecord = fxs.size()+1;//+1 bir boþ satýr


		
		WriteFile(
			hFXFile,           // open file handle
			&fxheader,      // start of data to write
			sizeof(fxheader),  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);

		WriteFile(
			hFXFile,           // open file handle
			fxs.data(),      // start of data to write
			fxs.size() * sizeof(FX),  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);


		CloseHandle(hFXFile);
	
	}
	void OpenReadWrite(char* symbolname, std::vector<FX> fxs) {


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

			masterheader.totalfx = masterheader.lastaddedfx = 1;
			WriteFile(
				hMFile,           // open file handle
				&masterheader,      // start of data to write
				sizeof(masterheader),  // number of bytes to write
				&dwBytesWritten, // number of bytes that were written
				NULL);

			master.fx = 1;
			memset(master.secname, 0x20, 16);
			memmove(master.secname, symbolname, lstrlen(symbolname));
			memset(master.secsymbol, 0x20, 16);
			memmove(master.secsymbol, symbolname,lstrlen(symbolname));
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
					std::string a = "F" + std::to_string(master.fx) + ".DAT";
					FxYarat(a.c_str(), fxs);
					CloseHandle(hMFile);
					return;
				}
				ReadFile(hMFile, &master, sizeof(master), &dwBytesWritten, NULL);
			}
			if (masterheader.totalfx != 0xff) {
					SetFilePointer(hMFile, 0, 0, FILE_BEGIN);
					masterheader.totalfx = masterheader.lastaddedfx = masterheader.totalfx + 1;
					WriteFile(
						hMFile,           // open file handle
						&masterheader,      // start of data to write
						sizeof(masterheader),  // number of bytes to write
						&dwBytesWritten, // number of bytes that were written
						NULL);
					SetFilePointer(hMFile, 0, 0, FILE_END);
					master.fx = masterheader.totalfx;
					memset(master.secname, 0x20, 16);
					memmove(master.secname, symbolname, lstrlen(symbolname));
					memset(master.secsymbol, 0x20, 16);
					memmove(master.secsymbol, symbolname, lstrlen(symbolname));
					memmove(master.fdate, fxs.front().date, 4);
					memmove(master.ldate, fxs.back().date, 4);
					WriteFile(
						hMFile,           // open file handle
						&master,      // start of data to write
						sizeof(master),  // number of bytes to write
						&dwBytesWritten, // number of bytes that were written
						NULL);
					std::string a= "F"+std::to_string(master.fx)+".DAT";
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

					
					xmasterheader.comingasfx_mwd = 257;
					xmasterheader._totalfx_mwd = 1;
					xmasterheader.totalfx_mwd = 1;
					
					WriteFile(
						hXMFile,           // open file handle
						&xmasterheader,      // start of data to write
						sizeof(xmasterheader),  // number of bytes to write
						&dwBytesWritten, // number of bytes that were written
						NULL);

					
					xmaster.fx= xmasterheader.comingasfx_mwd-1;
					memset(master.secname, 0x20, 16);
					memmove(xmaster.secname, symbolname, lstrlen(symbolname));
					memset(master.secsymbol, 0x20, 16);
					memmove(xmaster.secsymbol, symbolname, lstrlen(symbolname));
					float f = BasicToIEEE((unsigned char*)fxs.front().date)+ 19000000.0f;
					xmaster.fdate = f;
					xmaster._fdate = f;
					xmaster.notimportantfdate[0] = f;
					f = BasicToIEEE((unsigned char*)fxs.back().date) + 19000000.0f;
					xmaster.ldate =f;

					
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

				

				//xmaster ara
				ReadFile(hXMFile, &xmasterheader, sizeof(xmasterheader), &dwBytesWritten, NULL);
				ReadFile(hXMFile, &xmaster, sizeof(xmaster), &dwBytesWritten, NULL);
				// Check for eof.
				while (dwBytesWritten != 0)
				{
					if (0 == strncmp(symbolname, xmaster.secsymbol, lstrlen(symbolname)))
					{
						SetFilePointer(hMFile, -sizeof(xmaster), 0, FILE_CURRENT);
						float f = BasicToIEEE((unsigned char*)fxs.front().date) + 19000000.0f;
						xmaster.fdate = f;
						xmaster._fdate = f;
						xmaster.notimportantfdate[0] = f;
						f = BasicToIEEE((unsigned char*)fxs.back().date) + 19000000.0f;
						xmaster.ldate = f;
						WriteFile(
							hXMFile,           // open file handle
							&xmaster,      // start of data to write
							sizeof(xmaster),  // number of bytes to write
							&dwBytesWritten, // number of bytes that were written
							NULL);
						std::string a = "F" + std::to_string(xmaster.fx) + ".MWD";
						FxYarat(a.c_str(), fxs);
						CloseHandle(hXMFile);
						return;
					}
					ReadFile(hMFile, &xmaster, sizeof(xmaster), &dwBytesWritten, NULL);
				}

				////yoksa xmastera ekle
				
				SetFilePointer(hXMFile, 0, 0, FILE_BEGIN);
				xmasterheader._totalfx_mwd = xmasterheader.comingasfx_mwd-255;
				xmasterheader.totalfx_mwd = xmasterheader.comingasfx_mwd-255;
				xmasterheader.comingasfx_mwd = xmasterheader.comingasfx_mwd+1;
				WriteFile(
					hXMFile,           // open file handle
					&xmasterheader,      // start of data to write
					sizeof(xmasterheader),  // number of bytes to write
					&dwBytesWritten, // number of bytes that were written
					NULL);
				SetFilePointer(hXMFile, 0, 0, FILE_END);

				xmaster.fx = xmasterheader.comingasfx_mwd - 1;
				memset(master.secname, 0x20, 16);
				memmove(xmaster.secname, symbolname, lstrlen(symbolname));
				memset(master.secsymbol, 0x20, 16);
				memmove(xmaster.secsymbol, symbolname, lstrlen(symbolname));
				float f = BasicToIEEE((unsigned char*)fxs.front().date) + 19000000.0f;
				xmaster.fdate = f;
				xmaster._fdate = f;
				xmaster.notimportantfdate[0] = f;
				f = BasicToIEEE((unsigned char*)fxs.back().date) + 19000000.0f;
				xmaster.ldate = f;


				WriteFile(
					hXMFile,           // open file handle
					&xmaster,      // start of data to write
					sizeof(xmaster),  // number of bytes to write
					&dwBytesWritten, // number of bytes that were written
					NULL);
				std::string a = "F" + std::to_string(xmaster.fx) + ".MWD";
				FxYarat(a.c_str(), fxs);
				CloseHandle(hXMFile);
				return;


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

