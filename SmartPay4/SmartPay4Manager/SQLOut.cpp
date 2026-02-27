 /**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/


/*
	BYTE m_ucColumnType[]

	bit 0	1 = quotes required, 0 = quotes not required
	bit 1	1 = condition, 0 = value		(used for WriteModify() function)
	bit 2	1 = hex, 0 = not hex
	bit 3	not used
	bit 4	not used
	bit 5	not used
	bit 6	not used
	bit 7	not used
*/
/**********************************************************************/

CSQLOut::CSQLOut(CSSFile& file, const char* szTableName, CStringArray& arrayFields)
	: m_file(file), m_strTableName(szTableName)
{
	m_strHeader = "";

	for (int n = 0; n < arrayFields.GetSize(); n++)
	{
		CString strField = "[";
		strField += arrayFields.GetAt(n);
		strField += "]";

		if (m_strHeader != "")
		{
			m_strHeader += ",";
		}

		m_strHeader += strField;
	}

	m_bRemoveZeros = FALSE;
	m_bMinimizeFloat = FALSE;
	m_cQuote = '\'';			// single quote
}

/**********************************************************************/

void CSQLOut::SetHeader( const char* szHeader )
{
	m_strHeader = szHeader;
}

/**********************************************************************/

int CSQLOut::Add ( const char* szText )
{
	return AddString ( FormatString(szText), 0x01 );	// quotes required
}

/**********************************************************************/

int CSQLOut::AddHexString ( const char* szText )
{
	return AddString ( FormatString(szText), 0x04 );	// quotes NOT required
}

/**********************************************************************/

int CSQLOut::Add ( int nNum )
{
	return AddString ( FormatString(nNum), 0x00 );
}

/**********************************************************************/

int CSQLOut::Add ( long lNum )
{
	return AddString ( FormatString(lNum), 0x00 );
}

/**********************************************************************/

int CSQLOut::Add ( __int64 nNum )
{
	return AddString ( FormatString(nNum), 0x00 );
}

/**********************************************************************/

int CSQLOut::Add ( double dNum, int nDp )
{
	return AddString ( FormatString(dNum,nDp,m_bMinimizeFloat), 0x00 );
}

/**********************************************************************/

int CSQLOut::Add ( double dNum, int nDp, bool bMinimize )
{
	return AddString ( FormatString(dNum,nDp,bMinimize), 0x00 );
}

/**********************************************************************/

int CSQLOut::Add ( bool b )
{
	return AddString ( FormatString(b), 0x00 );
}

/**********************************************************************/

int CSQLOut::AddCondition ( const char* szText )
{
	return AddString ( FormatString(szText), 0x03 );	// quotes required
}

/**********************************************************************/

int CSQLOut::AddCondition ( int nNum )
{
	return AddString ( FormatString(nNum), 0x02 );
}

/**********************************************************************/

int CSQLOut::AddCondition ( long lNum )
{
	return AddString ( FormatString(lNum), 0x02 );
}

/**********************************************************************/

int CSQLOut::AddCondition ( double dNum, int nDp )
{
	return AddString ( FormatString(dNum,nDp,m_bMinimizeFloat), 0x02 );
}

/**********************************************************************/

int CSQLOut::AddCondition ( double dNum, int nDp, bool bMinimize )
{
	return AddString ( FormatString(dNum,nDp,bMinimize), 0x02 );
}

/**********************************************************************/

int CSQLOut::AddCondition ( bool b )
{
	return AddString ( FormatString(b), 0x02 );
}

/**********************************************************************/

bool CSQLOut::WriteFileBegin()
{
	return m_file.WriteLine( "-->FILEBEGIN" );
}

/**********************************************************************/

bool CSQLOut::WriteFileEnd()
{
	return m_file.WriteLine( "-->FILEEND" );
}

/**********************************************************************/

bool CSQLOut::WriteDelete()
{
	CString strBuf = "";
	strBuf.Format ( "-->TABLE: %s\r\nDELETE FROM %s\r\n--->GO\r\n", m_strTableName, m_strTableName );

	return m_file.WriteLine ( strBuf );
}

/**********************************************************************/

bool CSQLOut::WriteInsert()
{
	CString strBuf = "";
	strBuf.Format ( "INSERT %s\r\n(%s)\r\n", m_strTableName, m_strHeader );

	strBuf += " VALUES (";
	for (int i = 0; i < m_strValues.GetSize(); i++)
	{
		if (i > 0)
		{
			strBuf += ", ";
		}

		strBuf += GetString(i);
	}
	strBuf += ")\r\n";

	strBuf += ( "--->GO" );

	m_ucColumnType.RemoveAll();
	m_strValues.RemoveAll();

	return m_file.WriteLine ( strBuf );
}

/**********************************************************************/

bool CSQLOut::WriteModify()
{
	CString strBuf = "";
	strBuf.Format ( "UPDATE %s\r\n", m_strTableName );

	CCSV csvHeader ( m_strHeader );

	strBuf += " SET ";

	int n = 0;
	int i;
	for ( i = 0 ; i < m_strValues.GetSize() ; i++ )
	{
		if (( m_ucColumnType.GetAt(i) & 0x02 ) == 0x00 )	// value
		{
			if (n++ > 0)
			{
				strBuf += ", ";
			}

			CString strTmp = "";
			strTmp.Format ( "%s=%s", csvHeader.GetString(i), GetString(i) );

			strBuf += strTmp;
		}
	}
	strBuf += "\r\n";
	strBuf += " WHERE ";

	n = 0;
	for ( i = 0 ; i < m_strValues.GetSize() ; i++ )
	{
		if (( m_ucColumnType.GetAt(i) & 0x02 ) == 0x02 )	// condition
		{
			if (n++ > 0)
			{
				strBuf += " AND ";
			}

			CString strHeader = csvHeader.GetString(i);
			strHeader.TrimLeft ( '[' );
			strHeader.TrimRight ( ']' );

			CString strTmp = "";
			strTmp.Format ( "%s=%s", strHeader, GetString(i) );

			strBuf += strTmp;
		}
	}

	strBuf += "\r\n";
	strBuf += "--->GO";

	m_ucColumnType.RemoveAll();
	m_strValues.RemoveAll();

	return m_file.WriteLine ( strBuf );
}

/**********************************************************************/

CString CSQLOut::FormatString ( const char* szText )
{
	CString strBuf = szText;
	return strBuf;
}

/**********************************************************************/

CString CSQLOut::FormatString(int nNum)
{
	CString strBuf = "";

	if ((FALSE == m_bRemoveZeros) || (nNum != 0))
	{
		strBuf.Format("%d", nNum);
	}

	return strBuf;
}

/**********************************************************************/

CString CSQLOut::FormatString(__int64 nNum)
{
	CString strBuf = "";

	if ((FALSE == m_bRemoveZeros) || (nNum != 0))
	{
		strBuf.Format("%I64d", nNum);
	}

	return strBuf;
}

/**********************************************************************/

CString CSQLOut::FormatString(long lNum)
{
	CString strBuf = "";

	if ((FALSE == m_bRemoveZeros) || (lNum != 0))
	{
		strBuf.Format("%ld", lNum);
	}

	return strBuf;
}

/**********************************************************************/

CString CSQLOut::FormatString(double dNum, int nDp, bool bMinimize)
{
	CString strBuf = "";

	if ((FALSE == m_bRemoveZeros) || (dNum != 0.0))
	{
		strBuf.Format("%.*f", nDp, dNum);

		if ((nDp > 0) && (TRUE == bMinimize))
		{
			strBuf.TrimRight('0');
			strBuf.TrimRight('.');
		}
	}

	return strBuf;
}

/**********************************************************************/

CString CSQLOut::FormatString(bool b)
{
	CString strBuf = "";

	if ((FALSE == m_bRemoveZeros) || (b != 0))
	{
		strBuf = (b == 0) ? "0" : "1";
	}

	return strBuf;
}

/**********************************************************************/

int CSQLOut::AddString ( const char* szText, BYTE ucType )
{
	m_ucColumnType.Add ( ucType );
	return m_strValues.Add ( szText );
}

/**********************************************************************/

CString CSQLOut::GetString ( int nIndex )
{
	int nType = m_ucColumnType.GetAt(nIndex);
	bool bQuote = ( ( nType & 0x01 ) != 0 );
	bool bHex = ( ( nType & 0x04 ) != 0 );

	return GetQuotedString( m_strValues.GetAt( nIndex ), bQuote, bHex );
}

/**********************************************************************/

CString CSQLOut::GetQuotedString ( const char* szText, bool bQuote, bool bHex )
{
	CString strBuf = "";			// add leading quotes around field
	if (TRUE == bQuote)
	{
		strBuf += "'";
	}

	int nLen = (int) strlen ( szText );

	if ( ( TRUE == bQuote ) || ( TRUE == bHex ) )
	{
		for ( int n = 0 ; n < nLen ; n++ )		// double up quotes
		{
			char c = szText[n];
			strBuf += c;

			if (TRUE == bQuote)
			{
				if (c == m_cQuote)
				{
					strBuf += c;
				}
			}

			if ( TRUE == bHex )
			{
				if ((n % 70) == 69)
				{
					strBuf += "\r\n";
				}
			}
		}
	}
	else
	{
		strBuf = szText;
	}

	if (TRUE == bQuote)
	{
		strBuf += "'";			// add trailing quotes around field
	}

	return strBuf;
}

/**********************************************************************/
