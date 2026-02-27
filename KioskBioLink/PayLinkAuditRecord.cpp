//***************************************************************************************
//*** exactly same file used in KioskCardLink \ KioskBioLink ****************************
//***************************************************************************************

#include "PayLinkAuditRecord.h"

/**********************************************************************/
// Date , Time , CCNO, UserID ,   UserName     , GroupNo , CoinValue   , CurrentGT , {Startup posting}	// individual coinage entered
// Date , Time ,  0  , 9999nn ,    EMPTY       ,    0    , CashRemoved , CurrentGT ,					// Cash Empty Log

// Date , Time ,  0  ,        ,                ,    0    , LastPaylink ,		   ,					// KIosk Test

CPayLinkAuditRecord::CPayLinkAuditRecord()
{
	m_date.SetDate ( "" );
	m_time.SetTime ( "" );
	m_strUserID		= "";
	m_strUserName	= "";
	m_nGroupNo		= 0;
	m_nCCNo			= 0;
	m_lValue1		= 0;
	m_lValue2		= 0;
	m_strText		= "";
	m_nTopupPurse	= 0;
}

//*******************************************************************

bool CPayLinkAuditRecord::SaveLine ( CSSFile *pFile )
{
	CCSV csv;

	csv.Add ( CSSDate::GetCurrentDate().GetCSVDate() );
	csv.Add ( CSSTime::GetCurrentTime().GetCSVTime() );
	csv.Add ( m_nCCNo );
	csv.Add ( m_strUserID );
	csv.Add ( m_strUserName );
	csv.Add ( m_nGroupNo );
	csv.Add ( m_lValue1 );
	csv.Add ( m_lValue2 );
	csv.Add ( m_strText );
	csv.Add ( m_nTopupPurse );

	return pFile->Write ( csv );						// audit record line
}

//*********************************************************************

bool CPayLinkAuditRecord::ReadLine ( CSSFile* file )
{
	CCSV csv;
	while ( TRUE )
	{
		if ( file->Read ( csv ) == FALSE )
			return FALSE;								// end of file

		if ( csv.GetSize() > 0 )						// lose blank lines, just in case
			break;
	}

	m_date.SetDate ( csv.GetString(0) );				// date filed
	m_time.SetTime ( csv.GetString(1) );				// time filed
	csv.Get ( 2, m_nCCNo );
	csv.Get ( 3, m_strUserID );
	csv.Get ( 4, m_strUserName );
	csv.Get ( 5, m_nGroupNo );
	csv.Get ( 6, m_lValue1 );							// value added / differ value / PaylinkCurrentLevel
	csv.Get ( 7, m_lValue2 );
	csv.Get ( 8, m_strText );
	if ( csv.IsEmpty(9) == FALSE )	m_nTopupPurse = csv.GetInt(9);

	return TRUE;
}

//**********************************************************************

double CPayLinkAuditRecord::ValueAdded()	{ return MakeDouble ( m_lValue1 ); }
double CPayLinkAuditRecord::ValueDiffer()	{ return MakeDouble ( m_lValue1 ); }
double CPayLinkAuditRecord::ValueGT()		{ return MakeDouble ( m_lValue2 ); }

double CPayLinkAuditRecord::MakeDouble ( long lValue  )
{
	double dValue = ( (double)lValue ) / 100;
	return dValue;
}

//**********************************************************************
