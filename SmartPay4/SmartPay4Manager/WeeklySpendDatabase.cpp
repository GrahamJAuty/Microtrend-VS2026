//#include "FilenameHandler.h"
//#include "GlobalFunctions.h"
#include "WeeklySpendDatabase.h"

//**********************************************************************

static const char* szHEADER		= "UID,N1,N2,D1,C1,D2,C2,D3,C3,D4,C4,D5,C5,D6,C6,D7,C7";
static const char* szSORTFIELD	= "UID,ascending";

//**********************************************************************

CWeeklySpendDatabase::CWeeklySpendDatabase()
{
	m_strError	= "";
	m_strKey	= "";
	m_nDayNo	= 0;
}

/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

bool CWeeklySpendDatabase::OpenForSharing ( const char* szFilename )
{
	int nRetries = 10;

	CreateSSDatabase ( szFilename, "", szHEADER, szSORTFIELD, nRetries );	// create db if not found
	Close();

	if ( OpenSSDatabaseReadWrite ( szFilename, "", szSORTFIELD, nRetries ) == DB_ERR_NONE )
		return TRUE;

	m_strError.Format ( "Weekly Spend database error - %s", CSSStringDatabase::GetError() );
	return FALSE;
}	

/**********************************************************************/

const char* CWeeklySpendDatabase::FormatKey ( const char* szKey )
{
	m_strKey = szKey;
	AddLeading ( m_strKey, 16, '0' );
	return m_strKey;
}

/**********************************************************************/

int CWeeklySpendDatabase::GetPercentPosition()
{
	return  ( GetCurrentRow() * 100L ) / GetRecordCount();
}

//*******************************************************************

CString CWeeklySpendDatabase::GetDayLabel ( int nDayNo )
{
	CString strLabel;	strLabel.Format ( "D%d", nDayNo );	
	return strLabel;
}

CString CWeeklySpendDatabase::GetCountLabel ( int nDayNo )
{
	CString strLabel;		strLabel.Format ( "C%d", nDayNo );
	return strLabel;
}

//*******************************************************************
// Musr be called befoer calling SaveEntry \ IncrementCounter

bool CWeeklySpendDatabase::SetDayNo ( const char* szDate )
{
	CSSDate date ( szDate );
	m_nDayNo = date.GetDayOfWeek();							// 1=sunday 2=monday..7=saturday (0=not set) */

	return ( m_nDayNo == 0 ) ? FALSE : TRUE;				// 0=not set
}

//*******************************************************************
//  NOTE - Must Call SetDayNo  before calling this routine or IncrementCounter

void CWeeklySpendDatabase::SaveSpend ( double dValue )
{
	CString strLabel = GetDayLabel ( m_nDayNo );	
	double dSpend = GetSpend ( strLabel );
	CSSDatabase::Set ( strLabel, (double)(dSpend + dValue ), 2 );
}

void CWeeklySpendDatabase::IncrementCount()
{
	CString strLabel = GetCountLabel ( m_nDayNo );
	int nCount = GetCount ( strLabel );
	CSSDatabase::Set ( strLabel, ++nCount );
}

//******************************************************************************