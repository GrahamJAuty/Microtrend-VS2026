/**********************************************************************/
#include "SPOSKeyboardDefines.h"
/**********************************************************************/
#include "SPOSKeyboardLayoutOptions.h"
/**********************************************************************/

CSPOSKeyboardLayoutOptions::CSPOSKeyboardLayoutOptions()
{
	Reset();
}

/**********************************************************************/

void CSPOSKeyboardLayoutOptions::Reset()
{
	m_strDataPath = "";
	m_strKeyboardName = "";
	m_strSQLInFilename = "";
	m_strSQLOutFilename = "";
	m_strImageFolder = "";
	m_strReportFolder = ".\\System\\EcrRep";
	m_strReportSettingsFolder = ".\\System\\EcrRep";
	m_nMaxBasePluLen = 6;
	m_nPluTextOrder = 0;
	RestoreDefaults();
}

/**********************************************************************/

void CSPOSKeyboardLayoutOptions::SetMaxBasePluLen( int n )
{
	if ( ( n >= 0 ) && ( n <= 13 ) )
		m_nMaxBasePluLen = n;
}

/**********************************************************************/

void CSPOSKeyboardLayoutOptions::SetPluTextOrder( int n )
{
	if ( ( n >= 0 ) && ( n <= 5 ) )
		m_nPluTextOrder = n;
}

/**********************************************************************/

void CSPOSKeyboardLayoutOptions::RestoreDefaults()
{
	m_nXMarginDisplay = 200;
	m_nSharpPosResolutionType = 2;
	m_bShowPluNo = FALSE;
	m_nKeyListHeight = 351;
	m_nLastKeyType = KEY_TYPE_PLUNUMBER;
	m_nLastKeyValue = 0;
	m_nLastFgndId = -2;
	m_nLastBgndId = -2;
	m_nLastFontSize = 999;
	m_nKeyListWidth = 280;
	m_bDefaultBold = FALSE;
	m_bEditKeyTopLeft = FALSE;
	m_bImageWithStyle = FALSE;
	m_nInitialViewMode = 0;
	m_nInitialZoomType = 2;
}

/**********************************************************************/

const char* CSPOSKeyboardLayoutOptions::GetCSVLine()
{
	CCSV csv;
	csv.Add ( 3 );
	csv.Add ( m_nXMarginDisplay );
	csv.Add ( m_nSharpPosResolutionType );
	csv.Add ( m_bShowPluNo );
	csv.Add ( m_nKeyListHeight );
	csv.Add ( m_nLastKeyType );
	csv.Add ( m_nLastKeyValue );
	csv.Add ( m_nLastFgndId );
	csv.Add ( m_nLastBgndId );
	csv.Add ( m_nLastFontSize );
	csv.Add ( m_nKeyListWidth );
	csv.Add ( m_bDefaultBold );
	csv.Add ( m_nInitialViewMode );
	csv.Add ( m_nInitialZoomType );
	csv.Add ( m_bEditKeyTopLeft );
	csv.Add ( m_bImageWithStyle );
	m_strCSVLine = csv.GetLine();
	return m_strCSVLine;
}

/**********************************************************************/

void CSPOSKeyboardLayoutOptions::ReadV1To2 ( CCSV& csv, int nVer )
{
	int nOffset = 1;

	m_nXMarginDisplay = csv.GetInt(nOffset++);
	
	if ( nVer >= 2 )
		m_nSharpPosResolutionType = csv.GetInt(nOffset++);
	else
		m_nSharpPosResolutionType = 2;
	
	m_bShowPluNo = csv.GetBool(nOffset++);
	nOffset += 11;
	m_nKeyListHeight = csv.GetInt(nOffset++);
	m_nLastKeyType = csv.GetInt(nOffset++);
	m_nLastKeyValue = csv.GetInt(nOffset++);
	m_nLastFgndId = csv.GetInt(nOffset++);
	m_nLastBgndId = csv.GetInt(nOffset++);
	m_nLastFontSize = csv.GetInt(nOffset++);
	m_nKeyListWidth = csv.GetInt(nOffset++);
	m_bDefaultBold = csv.GetBool(nOffset++);
	m_nInitialViewMode = csv.GetInt(nOffset++);
	nOffset++;
	m_nInitialZoomType = csv.GetInt(nOffset++);
	m_bEditKeyTopLeft = csv.GetBool(nOffset++);
}

/**********************************************************************/

void CSPOSKeyboardLayoutOptions::ReadV3 ( CCSV& csv )
{
	int nOffset = 1;

	m_nXMarginDisplay = csv.GetInt(nOffset++);
	m_nSharpPosResolutionType = csv.GetInt(nOffset++);
	m_bShowPluNo = csv.GetBool(nOffset++);
	m_nKeyListHeight = csv.GetInt(nOffset++);
	m_nLastKeyType = csv.GetInt(nOffset++);
	m_nLastKeyValue = csv.GetInt(nOffset++);
	m_nLastFgndId = csv.GetInt(nOffset++);
	m_nLastBgndId = csv.GetInt(nOffset++);
	m_nLastFontSize = csv.GetInt(nOffset++);
	m_nKeyListWidth = csv.GetInt(nOffset++);
	m_bDefaultBold = csv.GetBool(nOffset++);
	m_nInitialViewMode = csv.GetInt(nOffset++);
	m_nInitialZoomType = csv.GetInt(nOffset++);
	m_bEditKeyTopLeft = csv.GetBool(nOffset++);
	m_bImageWithStyle = csv.GetBool(nOffset++);
}

/**********************************************************************/

bool CSPOSKeyboardLayoutOptions::Read()
{
	CSSFile FileLayout;
	
	if ( FileLayout.Open ( m_strFilename, "rb" ) == TRUE )
	{
		CString strBuffer;
		if ( FileLayout.ReadString ( strBuffer ) == TRUE )
		{	
			CCSV csv ( strBuffer );
			
			switch ( csv.GetInt(0) )
			{
			case 1:	ReadV1To2	( csv, 1 );		break;
			case 2:	ReadV1To2	( csv, 2 );		break;
			case 3:	ReadV3		( csv );		break;
			}
		}	
	}
	else
		RestoreDefaults();

	if ( ( m_nXMarginDisplay < 0  ) || ( m_nXMarginDisplay > 400 ) )
		m_nXMarginDisplay = 200;

	if ( ( m_nKeyListHeight < 100 ) || ( m_nKeyListHeight > 800 ) )
		m_nKeyListHeight = 351;

	if ( ( m_nKeyListWidth < 200 ) || ( m_nKeyListWidth > 500 ) )
		m_nKeyListWidth = 300;

	if ( ( m_nInitialViewMode < 0 ) || ( m_nInitialViewMode > 5 ) )
		m_nInitialViewMode = 0;

	if ( ( m_nInitialZoomType < 1 ) || ( m_nInitialZoomType > 3 ) )
		m_nInitialZoomType = 2;

	return TRUE;
}
	
/**********************************************************************/

bool CSPOSKeyboardLayoutOptions::Write()
{
	::CreateSubdirectory( GetDataPath() );

	CSSFile FileLayout;
	if ( FileLayout.Open ( m_strFilename, "wb" ) == TRUE )
		FileLayout.WriteLine ( GetCSVLine() );
		
	return TRUE;
}

/**********************************************************************/

