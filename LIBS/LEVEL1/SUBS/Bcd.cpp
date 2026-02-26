/**********************************************************************/

void BCDtoAscii ( BYTE* pBCD, char* szBuf, int nBytes )
{
	int i;
	for ( i = 0 ; i < nBytes ; i++ )
	{
		szBuf[i*2] = ( pBCD[i] >> 4 ) + '0';
		szBuf[i*2+1] = ( pBCD[i] & 0x0f ) + '0';
	}
	szBuf[i*2] = 0;
}

/**********************************************************************/

long SignedBCDtoLong ( BYTE* buf, int len )
{
	int i;
	int sign = 0;
	long lNum;
	char szTmp[16];

	BCDtoAscii ( buf, szTmp, len );				/* compile string */

	i = ( len * 2 ) - 1;
	if ( szTmp[i] < '0' || szTmp[i] > '9' )
		sign = 1;
	szTmp[i] = 0;								/* loose sign */

	lNum = atol ( szTmp );
	if ( sign == 1 )
		lNum = -lNum;

	return lNum;
}

/**********************************************************************/

long BCDtoLong ( BYTE* buf, int len )
{
	char szTmp[16];

	BCDtoAscii ( buf, szTmp, len );			/* compile string */

	return ( atol ( szTmp ) );
}

/**********************************************************************/

void LongToBCD ( long lNum, BYTE* pBuf, int nLen )
{
	char Tmp[10];			/* 10 digits */
	int i = 0;

	do
	{
		Tmp[i++] = (char) ( lNum % 10L );
	}
	while (( lNum /= 10L ) != 0 );

	while ( i < (nLen*2) )
		Tmp[i++] = 0;	/* add leading zeros */

	/* Tmp contains reversed unpacked bcd number */

	{
		BYTE bcd;
		char* ptr = &Tmp[(nLen*2-1)];

		for ( i = 0 ; i < nLen ; i++ )
		{
			bcd = *ptr-- << 4;
			bcd |= *ptr--;
			pBuf[i] = bcd;
		}
	}
}

/**********************************************************************/

void LongToSignedBCD ( long lNum, BYTE* pBuf, int nLen )
{
	char Tmp[10];			/* 9 digits + sign */
	char cSign = 0;		/* assume positive */
	int i = 0;

	if ( lNum < 0 )
	{
		cSign = 0x0d;		/* negative */
		lNum = -lNum;		/* make number positive */
	}
	Tmp[i++] = cSign;

	do
	{
		Tmp[i++] = (char) ( lNum % 10L );
	}
	while (( lNum /= 10L ) != 0 );

	while ( i < (nLen*2) )
		Tmp[i++] = 0;	/* add leading zeros */

	/* Tmp contains reversed unpacked signed bcd number */

	{
		BYTE bcd;
		char* ptr = &Tmp[(nLen*2-1)];

		for ( i = 0 ; i < nLen ; i++ )
		{
			bcd = *ptr-- << 4;
			bcd |= *ptr--;
			pBuf[i] = bcd;
		}
	}
}

/**********************************************************************/

