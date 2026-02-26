/**********************************************************************/
#include "CashRSPImporterDepositInfo.h"
/**********************************************************************/

CCashRSPImporterDepositInfo::CCashRSPImporterDepositInfo(void)
{
	m_nState = DEPOSITINFO_WANT_DEPOSIT;
	m_bEndCurrentDeposit = FALSE;
	m_bGotCurrentDeposit = FALSE;
	m_bGotNewDeposit = FALSE;
	ResetNewDeposit();
	ResetCurrentDeposit();
}

/**********************************************************************/

CCashRSPImporterDepositInfo::~CCashRSPImporterDepositInfo(void)
{
}

/**********************************************************************/

void CCashRSPImporterDepositInfo::ResetCurrentDeposit()
{
	m_nDepositID = 0;
	m_nServerNo = 0;
	m_nDepositAmount = 0;
	m_strTaxBand = "";
	m_nTaxAmount = 0;
	m_strBooking = "";
	m_strCustName = "";
	m_arrayCustInfo.RemoveAll();
}

/**********************************************************************/

void CCashRSPImporterDepositInfo::ResetNewDeposit()
{
	m_nNewDepositID = 0;
	m_nNewServerNo = 0;
	m_nNewDepositAmount = 0;
	m_strNewTaxBand = "";
	m_nNewTaxAmount = 0;
}

/**********************************************************************/

void CCashRSPImporterDepositInfo::ForceDepositEnd()
{
	m_bEndCurrentDeposit = TRUE;
	m_bGotCurrentDeposit = ( m_strCustName != "" ) || ( m_strBooking == "" );
	m_nState = DEPOSITINFO_WANT_DEPOSIT;
}

/**********************************************************************/

void CCashRSPImporterDepositInfo::ProcessLine( const char* szLine )
{
	m_bEndCurrentDeposit = FALSE;
	m_bGotCurrentDeposit = FALSE;
	m_bGotNewDeposit = FALSE;
	m_nNewDepositID = 0;

	CCSV csv( szLine );
	CString strType = csv.GetString(0);
	strType.MakeUpper();

	if ( strType == "$DEPOSIT" )
	{
		m_bEndCurrentDeposit = TRUE;
		m_bGotCurrentDeposit = ( m_strCustName != "" ) || ( m_strBooking != "" );
		m_bGotNewDeposit = TRUE;
		m_nNewDepositID = csv.GetInt(1);
		m_nNewServerNo = csv.GetInt(4);
		m_nNewDepositAmount = csv.GetInt(5);
		m_strNewTaxBand = csv.GetString(6);
		m_nNewTaxAmount = csv.GetInt(7);
		m_nState = DEPOSITINFO_WANT_HEADER;
	}
	else if ( strType == "$DEPOSITEND" )
	{
		m_bEndCurrentDeposit = TRUE;
		m_bGotCurrentDeposit = ( m_strCustName != "" ) || ( m_strBooking != "" );
		m_nState = DEPOSITINFO_WANT_DEPOSIT;
	}
	else if ( strType == "$CUSTNAME" )
	{
		if ( m_nState != DEPOSITINFO_WANT_DEPOSIT )
			m_nState = DEPOSITINFO_WANT_CUSTNAME;
	}
	else if ( strType == "$BOOKING" )
	{
		if ( m_nState != DEPOSITINFO_WANT_DEPOSIT )
			m_nState = DEPOSITINFO_WANT_BOOKING;
	}
	else if ( strType == "$CUSTINFO" )
	{
		if ( m_nState != DEPOSITINFO_WANT_DEPOSIT )
			m_nState = DEPOSITINFO_WANT_CUSTINFO;
	}
	else
	{
		switch( m_nState )
		{
		case DEPOSITINFO_WANT_CUSTNAME:
			m_strCustName = csv.GetString(0);
			break;

		case DEPOSITINFO_WANT_BOOKING:
			m_strBooking = csv.GetString(0);
			break;

		case DEPOSITINFO_WANT_CUSTINFO:
			if ( m_arrayCustInfo.GetSize() < 4 )
				m_arrayCustInfo.Add( csv.GetString(0) );
			break;
		}
	}
}

/**********************************************************************/

void CCashRSPImporterDepositInfo::PostProcessLine()
{
	if ( TRUE == m_bEndCurrentDeposit )
		ResetCurrentDeposit();

	if ( TRUE == m_bGotNewDeposit )
	{
		m_nDepositID = m_nNewDepositID;
		m_nServerNo = m_nNewServerNo;
		m_nDepositAmount = m_nNewDepositAmount;
		m_strTaxBand = m_strNewTaxBand;
		m_nTaxAmount = m_nNewTaxAmount;
	}
	
	m_bEndCurrentDeposit = FALSE;
	m_bGotCurrentDeposit = FALSE;
	m_bGotNewDeposit = FALSE;
	
	ResetNewDeposit();
}

/**********************************************************************/

const char* CCashRSPImporterDepositInfo::GetCustInfo(int n)
{
	if ( ( n >= 0 ) && ( n < m_arrayCustInfo.GetSize() ) )
		m_strCustInfo = m_arrayCustInfo.GetAt(n);
	else
		m_strCustInfo = "";

	return m_strCustInfo;
}

/**********************************************************************/

