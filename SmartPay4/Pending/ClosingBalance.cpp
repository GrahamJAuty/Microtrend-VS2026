//****************************************************************
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLAccountManager.h"
//****************************************************************
#include "ClosingBalance.h"
//***************************************************************

CClosingBalance::CClosingBalance()
{
	m_dLiabilityPurse1 = 0.0;
	m_dBalancePurse2 = 0.0;
	m_dLiabilityPurse3 = 0.0;
	m_dCreditPurse1 = 0.0;
	m_dCreditPurse3 = 0.0;
	m_strCurrentKey = "";
}

//****************************************************************

bool CClosingBalance::Read ( int nMode )
{
	CString strFilename = Filenames.GetClosingBalanceFilename();

	if ( CClosingBalanceDatabase::Open ( strFilename, nMode ) == DB_ERR_NONE )
		return TRUE;

	return FALSE;
}

//************************************************************************
//
//bool CClosingBalance::HaveBalances()
//{
//	return CClosingBalanceDatabase::MoveLast();
//}

bool CClosingBalance::HaveBalances ( const char* szDate )
{
	CSSDate date ( szDate );
	return HaveBalances ( &date );
}

bool CClosingBalance::HaveBalances ( CSSDate* pDate )
{
	m_strCurrentKey = CClosingBalanceDatabase::MakeKey ( pDate );				// use yyyymmdd as key into db

	return CClosingBalanceDatabase::FindRecord ( m_strCurrentKey );
}

//************************************************************************

bool CClosingBalance::GetNearestDate ( const char* szDate )
{
	CSSDate date ( szDate );
	if ( HaveBalances ( &date ) == TRUE )							// have an entry for given date
		return TRUE;

	CClosingBalanceDatabase::MoveFirst();
	CSSDate dateFirst;	
	dateFirst.SetDate ( CClosingBalanceDatabase::GetDate() );

	if ( date < dateFirst )											// requested date before first entry
		return FALSE;

	CClosingBalanceDatabase::MoveLast();
	CSSDate dateLast;	
	dateLast.SetDate ( CClosingBalanceDatabase::GetDate() );

	if ( date > dateLast )											// requested date after last entry
		return FALSE;												// use last entry

// no exact match for given date
	 
	CTime t1 ( date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0 );
	CTime t2;
	int nDayShift = 1;

	while ( nDayShift < 32 )										// set limit just in case
	{
		t2 = t1 - CTimeSpan( nDayShift++, 0, 0, 0 );				// get previous day
		date.SetDate ( t2 );

		if ( date >= dateFirst && date <= dateLast )				// see if in range
		{
			if ( HaveBalances ( &date ) == TRUE )					// have an entry for given date
				return TRUE;
		}
		else
			break;													// date out of range
	}

	return FALSE;
}

//*****************************************************************************

bool CClosingBalance::SaveCalculatedBalances ( CSSDate* pDate )
{
	CSSTime timeMidnight;
	timeMidnight.SetTime ( "00:00" );				// midnight

	return SaveBalances ( pDate, &timeMidnight, m_dLiabilityPurse1, m_dBalancePurse2, m_dLiabilityPurse3, m_dCreditPurse1, m_dCreditPurse3 );
}

bool CClosingBalance::SaveBalances ( double dLiabilityPurse1, double dBalancePurse2, double dLiabilityPurse3, double dCreditPurse1, double dCreditPurse3 )
{
	CSSDate	date;	date.SetCurrentDate();
	CSSTime time;	time.SetCurrentTime();

	return SaveBalances ( &date, &time, dLiabilityPurse1, dBalancePurse2, dLiabilityPurse3, dCreditPurse1, dCreditPurse3 );
}

bool CClosingBalance::SaveBalances(CSSDate* pDate, CSSTime* pTime, double dLiabilityPurse1, double dBalancePurse2, double dLiabilityPurse3, double dCreditPurse1, double dCreditPurse3)
{
	m_strCurrentKey = CClosingBalanceDatabase::MakeKey(pDate);		// use yyyymmdd as key into db

	if (CClosingBalanceDatabase::FindRecord(m_strCurrentKey) == FALSE)
	{
		CClosingBalanceDatabase::AddBlankRecord(m_strCurrentKey);
		CClosingBalanceDatabase::SetDate(pDate->GetCSVDate());
	}

	CClosingBalanceDatabase::SetTime(pTime->GetCSVTime());
	CClosingBalanceDatabase::SetPurse1Liability(dLiabilityPurse1);
	CClosingBalanceDatabase::SetPurse2Balance(dBalancePurse2);
	CClosingBalanceDatabase::SetPurse3Liability(dLiabilityPurse3);
	CClosingBalanceDatabase::SetPurse1Credit(dCreditPurse1);
	CClosingBalanceDatabase::SetPurse3Credit(dCreditPurse3);

	return TRUE;
}

//********************************************************************************
//********************************************************************************
// calculate Closing balance @ Midnight from given date from Audits & database etc

bool CClosingBalance::CalculateBalances ( const char* szDate, CSmartPayAuditFile* pAudit )
{
#ifdef SYSTEMTYPE_MANAGER
	CWaitCursor cursor;
#endif

	m_dateBalance.SetDate ( szDate );

// scan audit file ***********************************************************

	CRecordSorter sorter;

	CString strDateFrom;
	strDateFrom.Format ( "%2.2d%2.2d%4.4d", m_dateBalance.GetDay(), m_dateBalance.GetMonth(), m_dateBalance.GetYear() - 1 );	// go back 1 year

	pAudit->JumpIn ( strDateFrom );										// starting position StartDate - 1Year back

	while ( TRUE )
	{
		int nStartFilePos = pAudit->GetFilePos();						// position in file to start from
		if (pAudit->ReadLine(&m_atc) == FALSE)
		{
			break;
		}

		CString strUserID = m_atc.GetUserID();
		if ( strUserID != "" )
		{
			switch ( IsValidAtc ( &m_atc ) )
			{
			case -1:	
				sorter.Update ( System.FormatCardNo(strUserID), nStartFilePos, TRUE );	// date before 'from' date
				break;												// overwrite any previous entry		

			case 0:		
				sorter.Update ( System.FormatCardNo(strUserID), nStartFilePos, FALSE );	// Date between 'from' & 'to' date
				break;												// only add if no previous entry

			case 1:		
				break;												// Date after 'to' date
			}
		}
	}

// scan database ****************************************************************

	{
		CArray<__int64, __int64> arrayUserIDs;
		SQLAccountManager.GetUserIDList(arrayUserIDs, NULL);
		
		for (int n = 0; n < arrayUserIDs.GetSize(); n++)
		{ 	
			__int64 nUserID = arrayUserIDs.GetAt(n);
			sorter.Update(System.FormatCardNo(nUserID, TRUE), -1, FALSE);	// only add if no previous entry
		}
	}

	if (sorter.GetSize() == 0)
	{
		return FALSE;
	}

// see which data to use ( database record or audit line ) ************************

	for ( int nIndex = 0 ; nIndex < sorter.GetSize() ; nIndex++ )
	{
		int nFilePos = sorter.GetRecordNo(nIndex);

		if (nFilePos < 0)											// database record entry not found in audit
		{
			SaveDatabaseLine(sorter.GetLabel(nIndex));			// get record number
		}
		else
		{
			pAudit->SetFilePos ( nFilePos  );
			if ( pAudit->ReadLine ( &m_atc ) == TRUE )				// should always read
			{
				bool bCalcNearestBalance = ( IsValidAtc ( &m_atc ) == 0 ) ? TRUE : FALSE;
				SaveStartingBalanceLine ( bCalcNearestBalance );
			}
		}
	}

	SaveCalculatedBalances ( &m_dateBalance );

	return TRUE;
}

//*****************************************************************************

void CClosingBalance::SaveStartingBalanceLine ( bool bCalcBalance ) 
{
	if ( m_atc.IsDeleteAppNo() == TRUE )					// Apno = delete \ batch delete \ import delete
	{
		CSSDate date ( m_atc.GetDateFiled() );				// date record deleted

		if ( date < m_dateBalance )
			return;											// card deleted before start date

		if ( date > m_dateBalance )
			return;
	}

	m_dLiabilityPurse1 += m_atc.GetCurrentPurse1Liability();		// Purse1 total
	m_dCreditPurse1 += m_atc.GetCurrentPurse1Credit();				// Purse1 total
	m_dBalancePurse2 += m_atc.GetCurrentPurse2Balance();			// Purse2 total
	m_dLiabilityPurse3 += m_atc.GetCurrentPurse3Liability();		// Purse3 total
	m_dCreditPurse3 += m_atc.GetCurrentPurse3Credit();				// Purse3 total

	if ( bCalcBalance == TRUE )
	{
		m_dLiabilityPurse1 -= m_atc.GetTransactionPurse1Liability();
		m_dCreditPurse1 -= m_atc.GetTransactionPurse1Credit();
		m_dBalancePurse2 -= m_atc.GetTransactionPurse2Balance();
		m_dLiabilityPurse3 -= m_atc.GetTransactionPurse3Liability();
		m_dCreditPurse3 -= m_atc.GetTransactionPurse3Credit();
	}
}

//*****************************************************************************
// to get to here the record is in database but no lines have been found in the audit file

void CClosingBalance::SaveDatabaseLine ( const char* szUserID ) 
{
	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserIDString(szUserID);

	if (SQLAccountManager.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
	{
// Purse1

		CSSDate datePurse1 ( RowAccount.GetPurse1LastSpendDate() );

		if (datePurse1 < m_dateBalance)
		{
			m_dLiabilityPurse1 += RowAccount.GetPurse1Liability();
			m_dCreditPurse1 += RowAccount.GetPurse1Credit();
		}

// Purse2

		CSSDate datePurse2 (RowAccount.GetPurse2LastSpendDate() );

		if (datePurse2 < m_dateBalance)							// is lastused date before 'from' date - yes can used it
		{
			m_dBalancePurse2 += RowAccount.GetPurse2Balance();
		}

//Purse3

		CSSDate datePurse3 ( RowAccount.GetPurse3LastSpendDate() );

		// is lastused date before 'from' date - yes can used it
		if (datePurse3 < m_dateBalance)
		{
			m_dLiabilityPurse3 += RowAccount.GetPurse3Liability();
			m_dCreditPurse3 += RowAccount.GetPurse3Credit();
		}
	}
}

//******************************************************************
//	reply	-1 = Date before 'from' date
//			 0 = date after 'from' date
//			+1 = Date after 'to' date
//******************************************************************

int CClosingBalance::IsValidAtc ( CSQLAuditRecord* atcRecord )
{
	CSSDate dateAudit ( atcRecord->GetDateFiled() );

	if (dateAudit < m_dateBalance)					// before day before start date
	{
		return -1;
	}

	if (dateAudit > m_dateBalance)					// past required date
	{
		return 1;
	}

	return 0;
}

//************************************************************************

