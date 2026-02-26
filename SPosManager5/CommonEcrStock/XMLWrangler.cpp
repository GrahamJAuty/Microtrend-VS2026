/**********************************************************************/
 
/**********************************************************************/
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "XMLWrangler.h"
/**********************************************************************/

CXMLWrangler::CXMLWrangler( int nNetworkIdx, CSSXMLParser& Parser, const char* szPrefix ) : m_Parser( Parser )
{
	m_nNetworkIdx = nNetworkIdx;
	m_bCashRSPFileOpen = FALSE;
	
	CString strFilename;
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	strFilename.Format( "%s_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
		szPrefix,
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond() );

	m_strDestFile1 = "";
	m_strDestFile1 += dbNetwork.GetFolderPathSyssetCashRSP( m_nNetworkIdx );
	m_strDestFile1 += "\\";
	m_strDestFile1 += strFilename;
	
	m_strDestFile2 = m_strDestFile1;
	m_strDestFile1 += ".tmp";
	m_strDestFile2 += ".001";
}

/**********************************************************************/

const char* CXMLWrangler::GetStringValue( CXMLNodeArray* pNodeArray, const char* szPath, const char* szElement )
{
	m_strStringValue = "";

	CXMLNodeArray ar;		
	int nCount = m_Parser.GetNode ( pNodeArray, ar, szPath );
			
	if ( nCount >= 1 )
	{
		CXMLNodeArray* pNodeSubArray = ar.GetAt(0);
		m_strStringValue = m_Parser.GetValue( pNodeSubArray, szElement );
	}
	
	return m_strStringValue;
}

/**********************************************************************/

bool CXMLWrangler::OpenCashRSPFile()
{
	if ( FALSE == m_bCashRSPFileOpen )
		m_bCashRSPFileOpen = m_fileCashRSP.Open( m_strDestFile1, "wb" );

	return m_bCashRSPFileOpen;
}

/**********************************************************************/

void CXMLWrangler::CloseCashRSPFile()
{
	if ( TRUE == m_bCashRSPFileOpen )
	{
		m_fileCashRSP.Close();
		m_bCashRSPFileOpen = FALSE;
	}
}

/**********************************************************************/

bool CXMLWrangler::RenameCashRSPFile()
{
	return ( rename( m_strDestFile1, m_strDestFile2 ) == 0 );
}

/**********************************************************************/

const char* CXMLWrangler::GetPennyString( const char* szPoundString, bool bFlipMinus )
{
	CString strPoundString = szPoundString;
	
	//states
	//0 = want minus or number or decimal point
	//1 = want number or decimal point
	//2 = want number

	int nState = 0;
	bool bGotMinus = FALSE;
	bool bGotNonZero = FALSE;
	int nAfterDP = 0;
	bool bIsValid = TRUE;
	CString strNumbers = "";
	
	for ( int n = 0; n < strPoundString.GetLength(); n++ )
	{
		char c = strPoundString.GetAt(n);

		switch( c )
		{
		case '-':
			switch( nState )
			{
			case 0:
				bGotMinus = TRUE;
				nState = 1;
				break;

			case 1:
			case 2:
				bIsValid = FALSE;
				break;
			}
			break;

		case '.':
			switch( nState )
			{
			case 0:	
			case 1:
				nState = 2;
				break;

			case 2:
				bIsValid = FALSE;
				break;
			}
			break;

		case '0':
			switch( nState )
			{
			case 0:
				nState = 1;
				break;

			case 1:
				if ( strNumbers != "" )
					strNumbers += c;
				break;

			case 2:
				if ( nAfterDP < 2 )
				{
					strNumbers += c;
					nAfterDP++;
				}
				break;
			}
			break;
				
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			switch( nState )
			{
			case 0:
			case 1:
				strNumbers += c;
				nState = 1;
				bGotNonZero = TRUE;
				break;

			case 2:
				if ( nAfterDP < 2 )
				{
					strNumbers += c;
					nAfterDP++;
					bGotNonZero = TRUE;
				}
				break;
			}
			break;

		default:
			bIsValid = FALSE;
			break;
		}

		if ( FALSE == bIsValid )
			break;
	}

	m_strPennyString = "";

	if ( ( TRUE == bIsValid ) && ( TRUE == bGotNonZero ) )
	{
		if ( bFlipMinus != bGotMinus )
			m_strPennyString += "-";

		m_strPennyString += strNumbers;

		switch( nAfterDP )
		{
		case 0:	m_strPennyString += "00";	break;
		case 1:	m_strPennyString += "0";	break;
		}
	}

	return m_strPennyString;
}

/**********************************************************************/
