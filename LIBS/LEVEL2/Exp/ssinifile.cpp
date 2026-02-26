/**********************************************************************/

static const char* szPassword = "CSSIniFile::AES";

/**********************************************************************/

CSSIniFile::CSSIniFile ( int nFlags )
	: m_nFlags(nFlags)
{
	m_bModified = FALSE;
}

/**********************************************************************/

CSSIniFile::~CSSIniFile()
{
	Write();
}

/**********************************************************************/

void CSSIniFile::ReadDataNormal ( CSSFile& file )
{
	CString strCurrentSection = "";

	CString strBuf;
	while ( file.ReadString ( strBuf ) == TRUE )	// read data lines
	{
		if (( strBuf.Left(1) == "[" ) && strBuf.Right(1) == "]" )
			strCurrentSection = strBuf.Mid(1,strBuf.GetLength()-2);
		else
		{
			CCSV csv ( strBuf, '=' );
			Set ( strCurrentSection, csv.GetString(0), csv.GetString(1) );
		}
	}
}

/**********************************************************************/

void CSSIniFile::ReadDataHex ( CSSFile& file )
{
	CString strCurrentSection = "";

	CString strBuf;
	while ( file.ReadString ( strBuf ) == TRUE )	// read data lines
	{
		strBuf.MakeReverse();

		CString strAscii;
		for ( int i = 0 ; i < strBuf.GetLength() ; i+=2 )
			strAscii += (char) strtol ( strBuf.Mid(i,2), NULL, 16 );

		strBuf = strAscii;

		if (( strBuf.Left(1) == "[" ) && strBuf.Right(1) == "]" )
			strCurrentSection = strBuf.Mid(1,strBuf.GetLength()-2);
		else
		{
			CCSV csv ( strBuf, '=' );
			Set ( strCurrentSection, csv.GetString(0), csv.GetString(1) );
		}
	}
}

/**********************************************************************/

int CSSIniFile::ReadDataAES ( CSSFile& file )
{
	int nReply = -8;

	CString strSource;

	CString strBuf;
	while ( file.ReadString ( strBuf ) == TRUE )
		strSource += strBuf;

	int nLength = strSource.GetLength();

	if ( nLength >= 2 )
	{
		nLength = nLength / 2;

		unsigned char* pSource = new unsigned char [nLength];
		if ( pSource != NULL )
		{
			unsigned char* pDest = new unsigned char [nLength];
			if ( pDest != NULL )
			{
				for ( int i = 0 ; i < nLength ; i++ )
					pSource[i] = (unsigned char) strtol ( strSource.Mid(i*2,2), NULL, 16 );

				int n = CSSAES::Decrypt ( pSource, nLength, pDest, szPassword );
				if ( n >= 0 )
				{
					CStringArray ar;
					CString strLine;
					for ( int i = 0 ; i < n ; i++ )
					{
						unsigned char uc = pDest[i];
						if ( uc == '\r' )
							continue;
						else if ( uc == '\n' )
						{
							ar.Add ( strLine );
							strLine = "";
						}
						else
							strLine += uc;
					}

					CString strCurrentSection = "";

					for ( int i = 0 ; i < ar.GetSize() ; i++ )
					{
						CString strBuf = ar.GetAt(i);

						if (( strBuf.Left(1) == "[" ) && strBuf.Right(1) == "]" )
							strCurrentSection = strBuf.Mid(1,strBuf.GetLength()-2);
						else
						{
							CCSV csv ( strBuf, '=' );
							Set ( strCurrentSection, csv.GetString(0), csv.GetString(1) );
						}
					}
					nReply = 0;	// no error
				}
				else
					nReply = n;

				delete [] pDest;
			}
			delete [] pSource;
		}
	}
	return nReply;
}

/**********************************************************************/

bool CSSIniFile::Read ( const char* szFilename )
{
	bool bReply = TRUE;

	m_strFilename = szFilename;

	CSSFile file;
	if ( file.Open ( szFilename, "rb" ) == TRUE )
	{
		CString strMD5;
		CString strBuf;

		switch ( m_nFlags )
		{
		case SS_INI_NORMAL:
			ReadDataNormal ( file );			// read data lines into array
			break;

		case SS_INI_PROTECTED:
			file.ReadString ( strBuf );		// read warning line and discard
			file.ReadString ( strMD5 );		// read md5 line
			ReadDataNormal ( file );			// read data lines into array
			if ( CalcMD5() != strMD5 )		// calc md5 on array
			{
				m_array.RemoveAll();
				bReply = FALSE;
			}
			break;

		case SS_INI_HEX:
		case SS_INI_HEX | SS_INI_PROTECTED:	// for backwards compatibility only
			file.ReadString ( strMD5 );		// read md5 line
			ReadDataHex ( file );			// read data lines into array
			if ( CalcMD5() != strMD5 )		// calc md5 on array
			{
				m_array.RemoveAll();
				bReply = FALSE;
			}
			break;

		case SS_INI_AES:
			file.ReadString ( strMD5 );		// read md5 line
			if ( ReadDataAES ( file ) == 0 )	// read data lines into array
			{
				if ( CalcMD5() != strMD5 )	// calc md5 on array
				{
					m_array.RemoveAll();
					bReply = FALSE;
				}
			}
			else
			{
				m_array.RemoveAll();
				bReply = FALSE;
			}
			break;
		}
		file.Close();
	}
	m_bModified = FALSE;
	return bReply;
}

/**********************************************************************/

static CString ConvertAsciiToHex ( const char* szAscii )
{
	CString strHex;
	for ( int i = 0 ; i < (int) strlen(szAscii) ; i++ )
	{
		CString strTemp;
		strTemp.Format ( "%2.2x", szAscii[i] );
		strHex += strTemp;
	}
	return strHex;
}

/**********************************************************************/

void CSSIniFile::WriteDataNormal ( CSSFile& file )
{
	CString strCurrentSection = "";

	for ( int i = 0 ; i < m_array.GetSize() ; i++ )
	{
		CString strLine;
		CIniObj* pObj = (CIniObj*) m_array.GetAt(i);

		if ( strCurrentSection != pObj->m_strSection )
		{
			CString strBuf;
			strBuf.Format ( "[%s]", (const char*) pObj->m_strSection );

			file.WriteLine ( strBuf );
			strCurrentSection = pObj->m_strSection;
		}

		CCSV csvOut ( '=' );
		csvOut.Add ( pObj->m_strLabel );	// label;
		csvOut.Add ( pObj->m_strValue );	// value;
		strLine = csvOut.GetLine();

		file.WriteLine ( strLine );
	}
}

/**********************************************************************/

void CSSIniFile::WriteDataHex ( CSSFile& file )
{
	CString strCurrentSection = "";

	for ( int i = 0 ; i < m_array.GetSize() ; i++ )
	{
		CString strLine;
		CIniObj* pObj = (CIniObj*) m_array.GetAt(i);

		if ( strCurrentSection != pObj->m_strSection )
		{
			CString strBuf;
			strBuf.Format ( "[%s]", (const char*) pObj->m_strSection );
			strBuf = ConvertAsciiToHex ( strBuf );
			strBuf.MakeReverse();
			file.WriteLine ( strBuf );
			strCurrentSection = pObj->m_strSection;
		}

		CCSV csvOut ( '=' );
		csvOut.Add ( pObj->m_strLabel );	// label;
		csvOut.Add ( pObj->m_strValue );	// value;
		strLine = csvOut.GetLine();
		strLine = ConvertAsciiToHex ( strLine );
		strLine.MakeReverse();

		file.WriteLine ( strLine );
	}
}

/**********************************************************************/

int CSSIniFile::WriteDataAES ( CSSFile& file )
{
	int nReply = -8;

	CString strData;
	CString strCurrentSection = "";

	for ( int i = 0 ; i < m_array.GetSize() ; i++ )
	{
		CString strBuf;
		CIniObj* pObj = (CIniObj*) m_array.GetAt(i);

		if ( strCurrentSection != pObj->m_strSection )
		{
			strBuf.Format ( "[%s]", (const char*) pObj->m_strSection );

			strData += strBuf;
			strData += "\r\n";
			strCurrentSection = pObj->m_strSection;
		}

		CCSV csvOut ( '=' );
		csvOut.Add ( pObj->m_strLabel );	// label;
		csvOut.Add ( pObj->m_strValue );	// value;
		strData += csvOut.GetLine();
		strData += "\r\n";
	}

	int nLength = strData.GetLength();

	unsigned char* pDest = new unsigned char [nLength+16];
	if ( pDest != NULL )
	{
		int n = CSSAES::Encrypt ( (const unsigned char*) (LPCTSTR) strData, nLength, pDest, szPassword );
		if ( n >= 0 )
		{
			for ( int i = 0 ; i < n/16 ; i++ )
			{
				CString strLine;
				for ( int j = 0 ; j < 16 ; j++ )
				{
					CString strTemp;
					strTemp.Format ( "%2.2x", pDest[i*16+j] );
					strLine += strTemp;
				}
				file.WriteLine ( strLine );
			}

			nReply = 0;	// no errors
		}
		else
			nReply = n;

		delete [] pDest;
	}

	return nReply;
}

/**********************************************************************/

CString CSSIniFile::CalcMD5()
{
	CString strData;

	for ( int i = 0 ; i < m_array.GetSize() ; i++ )
	{
		CIniObj* pObj = (CIniObj*) m_array.GetAt(i);
		strData += pObj->m_strSection;
		strData += pObj->m_strLabel;
		strData += pObj->m_strValue;
	}

	return CSSMD5::CalcMD5(strData);
}

/**********************************************************************/

bool CSSIniFile::Write ( const char* szFilename )
{
	bool bReply = TRUE;

	bool bDifferentFile = FALSE;
	if ( m_strFilename != "" && m_strFilename != szFilename )	// writing to a different file than was read
		bDifferentFile = TRUE;

	if ( m_array.GetSize() > 0 && m_bModified == TRUE || bDifferentFile == TRUE )
	{
		CSSFile file;
		if ( file.Open ( szFilename, "wb" ) == TRUE )
		{
			switch ( m_nFlags )
			{
			case SS_INI_NORMAL:
					WriteDataNormal ( file );
					break;

			case SS_INI_PROTECTED:
					file.WriteLine ( "WARNING: If you modify this data your program will stop working!" );
					file.WriteLine ( CalcMD5() );
					WriteDataNormal ( file );
					break;

			case SS_INI_HEX:
			case SS_INI_HEX | SS_INI_PROTECTED:	// for backwards compatibility only
					file.WriteLine ( CalcMD5() );
					WriteDataHex ( file );
					break;

			case SS_INI_AES:
					file.WriteLine ( CalcMD5() );
					if ( WriteDataAES ( file ) != 0 )
						bReply = FALSE;
					break;
			}

			file.Close();

			if ( bDifferentFile == FALSE )	// dont change modified flag if writing different file
				m_bModified = FALSE;
		}
	}
	return bReply;
}

/**********************************************************************/

bool CSSIniFile::Write()
{
	if ( m_strFilename != "" )
		return Write ( m_strFilename );

	return FALSE;
}

/**********************************************************************/

CString CSSIniFile::GetString ( const char* szLabel, const char* szDefault )
{
	return GetString ( "", szLabel, szDefault );
}

int CSSIniFile::GetInt ( const char* szLabel, int nDefault )
{
	return GetInt ( "", szLabel, nDefault );
}

__int64 CSSIniFile::GetInt64 ( const char* szLabel, __int64 nDefault )
{
	return GetInt64 ( "", szLabel, nDefault );
}

long CSSIniFile::GetLong ( const char* szLabel, long lDefault )
{
	return GetLong ( "", szLabel, lDefault );
}

double CSSIniFile::GetDouble ( const char* szLabel, double dDefault )
{
	return GetDouble ( "", szLabel, dDefault );
}

bool CSSIniFile::GetBool ( const char* szLabel, bool bDefault )
{
	return GetBool ( "", szLabel, bDefault );
}

/**********************************************************************/

void CSSIniFile::Set ( const char* szLabel, const char* szValue )
{
	Set ( "", szLabel, szValue );
}

void CSSIniFile::Set ( const char* szLabel, int nValue )
{
	Set ( "", szLabel, nValue );
}

void CSSIniFile::Set ( const char* szLabel, __int64 nValue )
{
	Set ( "", szLabel, nValue );
}

void CSSIniFile::Set ( const char* szLabel, bool bValue )
{
	Set ( "", szLabel, bValue );
}

void CSSIniFile::Set ( const char* szLabel, double dValue, int nDp )
{
	Set ( "", szLabel, dValue, nDp );
}

void CSSIniFile::Set ( const char* szLabel, double dValue, int nDp, bool bMinimize )
{
	Set ( "", szLabel, dValue, nDp, bMinimize );
}

/**********************************************************************/

CString CSSIniFile::GetString ( const char* szSection, const char* szLabel, const char* szDefault )
{
	CString str;

	int n;
	if ( Find ( szSection, szLabel, n ) == TRUE )
		str = m_array.GetValue(n);
	else
		str = szDefault;

	return str;
}

/**********************************************************************/

int CSSIniFile::GetInt ( const char* szSection, const char* szLabel, int nDefault )
{
	int n;
	if ( Find ( szSection, szLabel, n ) == TRUE )
		return ( atoi ( m_array.GetValue(n)) );

	return nDefault;
}

/**********************************************************************/

__int64 CSSIniFile::GetInt64 ( const char* szSection, const char* szLabel, __int64 nDefault )
{
	int n;
	if ( Find ( szSection, szLabel, n ) == TRUE )
		return ( _atoi64 ( m_array.GetValue(n)) );

	return nDefault;
}

/**********************************************************************/

long CSSIniFile::GetLong ( const char* szSection, const char* szLabel, long lDefault )
{
	int n;
	if ( Find ( szSection, szLabel, n ) == TRUE )
		return ( atol ( m_array.GetValue(n)) );

	return lDefault;
}

/**********************************************************************/

double CSSIniFile::GetDouble ( const char* szSection, const char* szLabel, double dDefault )
{
	int n;
	if ( Find ( szSection, szLabel, n ) == TRUE )
		return ( atof ( m_array.GetValue(n)) );

	return dDefault;
}

/**********************************************************************/

bool CSSIniFile::GetBool ( const char* szSection, const char* szLabel, bool bDefault )
{
	int n;
	if ( Find ( szSection, szLabel, n ) == TRUE )
		return ( atoi(m_array.GetValue(n)) == 0 ) ? FALSE : TRUE;

	return bDefault;
}

/**********************************************************************/

void CSSIniFile::Set ( const char* szSection, const char* szLabel, const char* szValue )
{
	int n;
	if ( Find ( szSection, szLabel, n ) == TRUE )
	{
		CIniObj* pObj = (CIniObj*) m_array.GetAt(n);
		pObj->m_strValue = szValue;
		m_array.SetAt ( n, pObj );
	}
	else
	{
		CIniObj* pObj = new CIniObj ( szSection, szLabel, szValue );
		m_array.InsertAt ( n, pObj );
	}

	m_bModified = TRUE;
}

/**********************************************************************/

void CSSIniFile::Set ( const char* szSection, const char* szLabel, int nValue )
{
	CString strValue;
	strValue.Format ( "%d", nValue );
	Set ( szSection, szLabel, strValue );
}

/**********************************************************************/

void CSSIniFile::Set ( const char* szSection, const char* szLabel, __int64 nValue )
{
	CString strValue;
	strValue.Format ( "%I64d", nValue );
	Set ( szSection, szLabel, strValue );
}

/**********************************************************************/

void CSSIniFile::Set ( const char* szSection, const char* szLabel, double dValue, int nDp )
{
	CString strValue;
	strValue.Format ( "%.*f", nDp, dValue );
	Set ( szSection, szLabel, strValue );
}

/**********************************************************************/

void CSSIniFile::Set ( const char* szSection, const char* szLabel, double dValue, int nDp, bool bMinimize )
{
	CString strValue;
	strValue.Format ( "%.*f", nDp, dValue );
	if ( nDp > 0 && bMinimize == TRUE )
	{
		strValue.TrimRight ( '0' );
		strValue.TrimRight ( '.' );
	}
	Set ( szSection, szLabel, strValue );
}

/**********************************************************************/

void CSSIniFile::Set ( const char* szSection, const char* szLabel, bool bValue )
{
	CString strValue = ( bValue == TRUE ) ? "1" : "0";
	Set ( szSection, szLabel, strValue );
}

/**********************************************************************/

bool CSSIniFile::Find ( const char* szSection, const char* szLabel, int& nIndex )
{
	int nLow = 0;
	int nHigh = m_array.GetSize() - 1;

	while ( nLow <= nHigh )
	{
		nIndex = ( nLow + nHigh ) / 2;

		CIniObj* pObj = (CIniObj*) m_array.GetAt(nIndex);

		int nComp = strcmp ( szSection, pObj->m_strSection );	// compare sections

		if ( nComp == 0 )
			nComp = strcmp ( szLabel, pObj->m_strLabel );	// compare labels

		if ( nComp < 0 )
			nHigh = nIndex - 1;

		else if ( nComp > 0 )
			nLow = nIndex + 1;

		else						// strings compare
			return TRUE;
	}

	nIndex = nLow;
	return FALSE;
}

/**********************************************************************/
