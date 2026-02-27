//*******************************************************************************/
#include "TLIDDatabase.h"
//*******************************************************************************/

CTLIDRecord::CTLIDRecord ( int nTerminalType )
{
	Reset ( 0, nTerminalType );
}

void CTLIDRecord::Reset( int nTerminalNo, int nTerminalType )
{
	m_nTerminalType	= nTerminalType;
	m_nTerminalNo	= nTerminalNo;
	m_date.SetDate ( "" );
	m_time.SetTime ( "" );
	m_nCCNo			= 0;
	m_strUserID		= "";
	m_dCardValue	= 0.0;
	m_nTTLCounter	= 0;
	m_dTTLInDrawer	= 0.0;
	m_nStatusFlags	= 0x00;
}

//********************************************************************

void CTLIDRecord::ConvertFromCSV ( CCSV& csv )
{
	m_nTerminalType	= csv.GetInt(0);
	m_nTerminalNo	= csv.GetInt(1);
	m_date.SetDate ( csv.GetString(2) );
	m_time.SetTime ( csv.GetString(3) );
	m_nCCNo			= csv.GetInt(4);
	m_strUserID		= csv.GetString(5);
	m_dCardValue	= csv.GetDouble(6);
	m_nTTLCounter	= csv.GetInt(7);
	m_dTTLInDrawer	= csv.GetDouble(8);
	m_nStatusFlags	= csv.GetInt(9);
}

//********************************************************************

void CTLIDRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nTerminalType );
	csv.Add ( m_nTerminalNo );
	csv.Add ( m_date.GetCSVDate() );
	csv.Add ( m_time.GetCSVTime() );
	csv.Add ( m_nCCNo );
	csv.Add ( m_strUserID );
	csv.Add ( m_dCardValue, 2 );
	csv.Add ( m_nTTLCounter );
	csv.Add ( m_dTTLInDrawer, 2 );
	csv.Add ( m_nStatusFlags );
}

//********************************************************************

void CTLIDRecord::SetDate ( const char* szDate )
{
	if ( lstrlen ( szDate ) == 0 )		m_date.SetCurrentDate();
	else								m_date.SetDate ( szDate);
}

void CTLIDRecord::SetTime ( const char* szTime )
{
	if ( lstrlen ( szTime ) == 0 )	m_time.SetCurrentTime();
	else							m_time.SetTime ( szTime);
}

//********************************************************************
// clear last cash transaction info - CCNO = terminals current CCNo when cash emptied
// Add line to SmartPayMonitor Cash Log

void CTLIDRecord::SetCashRemoved ( int nCCNo, double dGT, const char* szCashFilename )
{
	CString strFilename = szCashFilename;
	if ( strFilename != "" )
	{
		if ( m_dTTLInDrawer != 0.0 )
		{
			CSSFile file;
			if ( file.Open ( strFilename, "ab" ) == TRUE )	// SmartPay Monitor cash log
			{
				CCSV csv;
				csv.Add ( m_date.GetCSVDate() );
				csv.Add ( m_time.GetCSVTime() );
				csv.Add ( nCCNo );						// Kiosk CCNo
				csv.Add ( m_nTTLCounter );				// total visits
				csv.Add ( m_dTTLInDrawer, 2 );			// cash removed
				csv.Add ( dGT, 2 );						// GT when cash removed
				file.Write ( csv );
				file.Close();
			}
		}
	}

	m_strUserID		= "";
	m_dCardValue	= 0.0;
	m_nTTLCounter	= 0;
	m_dTTLInDrawer	= 0.0;
	SetDoorOpen(FALSE);						// Speeder - door mustbe closed
}

//********************************************************************

void CTLIDRecord::UpdateCID( double dValue )
{
	++m_nTTLCounter;
	m_dTTLInDrawer += dValue;
}

//********************************************************************

int CTLIDRecord::IncrementCCNo()
{
	if ( ++m_nCCNo > 9999 )
		m_nCCNo = 1;

	return m_nCCNo;
}

//********************************************************************

const char* CTLIDRecord::GetTerminalText()
{
	if ( m_nTerminalType == nRECORDTYPE_EPOS )		return "Epos";
	
	if ( m_nTerminalType == nRECORDTYPE_KIOSK )		return "Kiosk";

	if ( m_nTerminalType == nRECORDTYPE_VENDING )	return "Vending";

	return "";
}

//********************************************************************
