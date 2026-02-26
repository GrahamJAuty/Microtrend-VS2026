/**********************************************************************/

void AddLeading ( CString& strBuf, int nWidth, char c )
{
	int nLen = nWidth - strBuf.GetLength();
	if ( nLen > 0 )
	{
		CString strTemp ( c, nLen );
		strBuf = strTemp + strBuf;
	}
}

/**********************************************************************/

void AddTrailing ( CString& strBuf, int nWidth, char c )
{
	int nLen = nWidth - strBuf.GetLength();
	if ( nLen > 0 )
	{
		CString strTemp ( c, nLen );
		strBuf += strTemp;
	}
}

/**********************************************************************/
