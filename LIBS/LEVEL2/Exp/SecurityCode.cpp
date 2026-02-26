/**********************************************************************/

CString CSecurityCode::GetCode ( int nSerialNo )
{
	int xlat[16] = { 15,13,11,9,7,5,3,1,0,2,4,6,8,10,12,14 };

	CString strSecurityCode;
	if ( nSerialNo > 0 && nSerialNo <= 999999 )
	{
		CString strSerialNo;
		strSerialNo.Format ( "%6.6d", nSerialNo );
		CString strTemp = CSSMD5::CalcMD5 ( strSerialNo );
		for ( int i = 0 ; i < 10 ; i++ )
		{
			char c = strTemp.GetAt(i);
			if ( c >= 'a' && c <= 'f' )
				c -= '0';
			strSerialNo += c;
		}

		for ( int i = 0 ; i < 16 ; i++ )
			strSecurityCode += strSerialNo.GetAt(xlat[i]);

		strSecurityCode.Insert ( 12, ' ' );
		strSecurityCode.Insert (  8, ' ' );
		strSecurityCode.Insert (  4, ' ' );
	}
	return strSecurityCode;
}

/**********************************************************************/

int CSecurityCode::GetSerialNo ( const char* szSecurityCode )
{
	int xlat[16] = { 8,7,9,6,10,5,11,4,12,3,13,2,14,1,15,0 };
	int nSerialNo = -1;

	CString strSecurityCode = szSecurityCode;
	CString strTemp;

	for ( int i = 0 ; i < strSecurityCode.GetLength() ; i++ )
	{
		char c = strSecurityCode.GetAt(i);
		if ( c >= '0' && c <= '9' )		// discard spaces etc
			strTemp += c;
	}

	if ( strTemp.GetLength() == 16 )
	{
		strSecurityCode = "";
		for ( int i = 0 ; i < 16 ; i++ )
			strSecurityCode += strTemp.GetAt(xlat[i]);

		nSerialNo = atoi ( strSecurityCode.Left(6) );
		if ( GetCode(nSerialNo) != szSecurityCode )
			nSerialNo = -1;
	}
	return nSerialNo;
}

/**********************************************************************/
