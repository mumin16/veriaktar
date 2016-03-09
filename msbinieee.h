#pragma once
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