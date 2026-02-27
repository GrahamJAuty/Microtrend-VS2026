#include "EmailLogDatabase.h"

//*****************************************************************************

//header = "CardNo,Name,DateEmailSent,TimeEmailSent,EmailAddress,P1Balance,P1DateLastUsed,P1TimeLastUsed,P2Balance,P2DateLastUsed,P2TimeLastUsed,P3Balance,P3DateLastUsed,P3TimeLastUsed,PointsBalance

static const char* szHEADER		= "K1,N1,D1,T1,E1,AA,AD,AT,BA,BD,BT,CA,CD,CT,DA";
static const char* szSORTFIELD	= "K1,ascending";							// CardNo
static int nRETRIES				= 10;

CEmailLogDatabase::CEmailLogDatabase()
{
	m_strKey = "";
}

/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

int CEmailLogDatabase::Open ( const char* szFilename, int nMode )
{
	int nRetries = 10;

	CreateSSDatabase ( szFilename, "", szHEADER, szSORTFIELD, nRETRIES );	// create db if not found
	Close();

	if ( nMode == DB_READONLY )
		return OpenSSDatabaseReadOnly ( szFilename, szSORTFIELD, nRETRIES );

	return OpenSSDatabaseReadWrite ( szFilename, "", szSORTFIELD, nRETRIES );
}	

//******************************************************************

const char* CEmailLogDatabase::FormatKey ( const char* szCardNo )
{
	m_strKey = System.FormatCardNo ( szCardNo );
	return m_strKey;
}

//******************************************************************

void CEmailLogDatabase::SetDateLastSent ( const char* szDate )
{
	if ( lstrlen ( szDate ) == 0 )	CSSDatabase::Set ( DBEmailLog::DateLastSent.Label, CSSDate::GetCurrentDate().GetDate() );
	else							CSSDatabase::Set ( DBEmailLog::DateLastSent.Label, szDate );
}

void CEmailLogDatabase::SetTimeLastSent ( const char* szTime )
{
	if ( lstrlen ( szTime ) == 0 )	CSSDatabase::Set ( DBEmailLog::TimeLastSent.Label, CSSTime::GetCurrentTime().GetTime() );
	else							CSSDatabase::Set ( DBEmailLog::TimeLastSent.Label, szTime );
}

//******************************************************************
