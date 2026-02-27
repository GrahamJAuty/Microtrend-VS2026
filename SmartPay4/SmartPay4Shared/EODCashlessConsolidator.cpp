/******************************************************************************/
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLDb.h"
/******************************************************************************/
#include "RepmanHandler.h"
/******************************************************************************/
#include "EODCashlessConsolidator.h"
/******************************************************************************/

CEODGroupSale::CEODGroupSale()
{
	m_nHeadCount = 0;
	m_nPossibleCount = 0;
	m_dPurse1Liability = 0.0;
	m_dPurse1Credit = 0.0;
	m_dPurse2 = 0.0;
	m_dPurse3Liability = 0.0;
	m_dPurse3Credit = 0.0;
	m_dCash = 0.0;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

CEODCashlessTotals::CEODCashlessTotals()
{
	Reset();
}

/******************************************************************************/

void CEODCashlessTotals::Reset()
{
	for (int n = 0; n < 7; n++)
	{
		m_dAdded[n] = 0.0;
		m_dSubtracted[n] = 0.0;
	}
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

CEODEntry::CEODEntry()
{
	Reset();
}

/******************************************************************************/

void CEODEntry::Reset()
{
	m_dAddCash = 0.0;
	m_dAddPurse0 = 0.0;
	m_dAddPurse1Liability = 0.0;
	m_dAddPurse1Credit = 0.0;
	m_dAddPurse2 = 0.0;
	m_dAddPurse3Liability = 0.0;
	m_dAddPurse3Credit = 0.0;
	/*****/
	m_dSubCash = 0.0;
	m_dSubPurse0 = 0.0;
	m_dSubPurse1Liability = 0.0;
	m_dSubPurse1Credit = 0.0;
	m_dSubPurse2 = 0.0;
	m_dSubPurse3Liability = 0.0;
	m_dSubPurse3Credit = 0.0;
}

/******************************************************************************/

CEODCashlessConsolidator::CEODCashlessConsolidator( bool bSecondaryReport )
{
	m_bSecondaryReport = bSecondaryReport;
	m_bSecondarySaveReqd = FALSE;
	m_strNoSalesFilename = "";

	m_TNoDatabase.OpenForSharing();														// open Terminal to TerminalSet database in ReadOnly mode
	if (bSecondaryReport == FALSE)													// see if first time 
	{
		m_bSecondarySaveReqd = (m_TNoDatabase.GetRecordCount() == 0) ? FALSE : TRUE;		// see if any termainls linked to a set

		for (int nSetNo = 1; nSetNo <= 4; nSetNo++)
		{
			CFileRecycler::SendToRecycleBin(GetSecondaryFilename(nSetNo));							// clear previous secondary data if found
		}
	}

	ResetCounters();
	OpenCardCounter();
}

/******************************************************************************/

CEODCashlessConsolidator::~CEODCashlessConsolidator()
{
	CloseCardCounter();
}

//*********************************************************************************

void CEODCashlessConsolidator::OpenCardCounter()
{
	CString strFilename = ( m_bSecondaryReport == TRUE ) ? "R126-$2" : "R126-$1";

	m_strCardCounterFilename = Filenames.GetTempReportPath ( strFilename, "dat" );
	m_dbCardCounter.Open ( m_strCardCounterFilename );
}

//*********************************************************************************

void CEODCashlessConsolidator::CloseCardCounter()
{
	m_dbCardCounter.Close();
	CFileRecycler::SendToRecycleBin ( m_strCardCounterFilename );		// delete temp card counter database
}

//*********************************************************************************

void CEODCashlessConsolidator::ResetCardCounter()
{
	CloseCardCounter();										// delete current counter database
	OpenCardCounter();										// & start again with new one
}

/**********************************************************************/

void CEODCashlessConsolidator::ResetCounters()
{
	for (int n = 0; n < EODENTRY_TYPE_COUNT; n++)
	{
		m_EODEntries[n].Reset();
	}

	m_dMealSales = 0;
	m_dVendingSales = 0;
	
	m_CashlessTotals.Reset();

	m_dTotalSubCash = 0;					// none account sale

	for (int i = 0; i <= 9; i++)
	{
		m_GroupSet[i].m_nHeadCount = 0;
		m_GroupSet[i].m_nPossibleCount = 0;
		m_GroupSet[i].m_dPurse1Liability = 0.0;
		m_GroupSet[i].m_dPurse1Credit = 0.0;
		m_GroupSet[i].m_dPurse2 = 0.0;
		m_GroupSet[i].m_dPurse3Liability = 0.0;
		m_GroupSet[i].m_dPurse3Credit = 0.0;
		m_GroupSet[i].m_dCash = 0.0;
	}

	m_dNotFoundSales = 0;
	m_dSalesUnknown = 0;

	for (int i = 0; i < nMAX_TAXCODES; i++)
	{
		m_dSales[i] = 0;
	}
}

//***************************************************************
// file to hold list of audit lines when cannot find matching purchase history
// szKey = "yyyymmdd" / "consol"
//
void CEODCashlessConsolidator::SetNoSalesFilename ( const char* szKey )
{
	m_strNoSalesFilename = ::Filenames.GetEODNoSalesFilename ( szKey );
	CFileRecycler::SendToRecycleBin ( m_strNoSalesFilename );
}
//
//*****************************************************************************
// NOTE: Once compiled need to call CalculateTotals() to get report totals

int CEODCashlessConsolidator::ConsolidateFromAudit(CSmartPayAuditFile* pAudit, CTime DateTimeFrom, CTime DateTimeTo, bool bFirstEOD, bool bRegenerate, CWorkingDlg* pWorking, int& nRecordsChecked)
{
	bool bStarted = bFirstEOD;		// on very first report there will be no audit_eodbalsnce in audit file
	bool bFinished = FALSE;

	CString strDateFrom;
	strDateFrom.Format("%2.2d/%2.2d/%4.4d",
		DateTimeFrom.GetDay(),
		DateTimeFrom.GetMonth(),
		DateTimeFrom.GetYear());

	pAudit->JumpIn(strDateFrom);

	CTime DateTimeFromCutOff = DateTimeFrom + CTimeSpan(0, 0, 1, 0);

	//int nFiddleCount = 0;

	while (pAudit->ReadLine(&m_atc) == TRUE)
	{
		if (pWorking != NULL)
		{
			if ((++nRecordsChecked % 10) == 0)
			{
				if (pWorking->SetCaption2RecordsChecked(nRecordsChecked, TRUE) == TRUE)
				{
					return nREPORT_CANCEL;
				}
			}
		}

		CSSDate dateAudit(m_atc.GetDateFiled());
		CSSTime timeAudit(m_atc.GetTimeFiled());

		if ((dateAudit.IsSet() == FALSE) || (timeAudit.IsSet() == FALSE))
		{
			continue;
		}

		CTime DateTimeAudit = CTime(dateAudit.GetYear(), dateAudit.GetMonth(), dateAudit.GetDay(), timeAudit.GetHour(), timeAudit.GetMinute(), timeAudit.GetSecond());

		// before start date
		if (DateTimeAudit < DateTimeFrom)
		{
			continue;
		}

		// locate EOD balance line to start processing from
		if (FALSE == bStarted)
		{
			if ((DateTimeAudit >= DateTimeFrom) && (DateTimeAudit <= DateTimeFromCutOff))	// date found 
			{
				// EOD line encountered so skip & start from this point
				if (m_atc.GetSourceType() == AUDIT_EODBALANCE)
				{
					bStarted = TRUE;
				}
			}
		}
		else
		{
			//FIDDLE : DISCARD SERVER EDIT LINES CAUSED BY PREVIOUS BUG
			if (m_atc.GetSourceType() == AUDIT_SERVER)
			{
				if (m_atc.GetApplicationNo() == APPNO_EDIT)
				{
					continue;
				}
			}

			if (ProcessLine(bRegenerate, DateTimeTo, DateTimeAudit, bFinished) == FALSE)
			{
				break;
			}
		}
	}

	int nResult = nREPORT_NOERROR;

	if (FALSE == bStarted)
	{
		nResult = nREPORT_NOEODSTART;
	}
	else if ((TRUE == bRegenerate) && (FALSE == bFinished))
	{
		nResult = nREPORT_NOEODEND;
	}

	return nResult;
}

//*****************************************************************************
// Cut down version audit file - so can use all lines to create report

int CEODCashlessConsolidator::ConsolidateFromAudit2 ( CSmartPayAuditFile* pAudit, CWorkingDlg* pWorkingDlg )
{
	m_bSecondarySaveReqd = FALSE;

	CTime dummyTime = CTime::GetCurrentTime();
	bool dummyFlag = FALSE;

	while (pAudit->ReadLine(&m_atc) == TRUE)
	{
		//FIDDLE : DISCARD SERVER EDIT LINES CAUSED BY PREVIOUS BUG
		if (m_atc.GetSourceType() == AUDIT_SERVER)
		{
			if (m_atc.GetApplicationNo() == APPNO_EDIT)
			{
				continue;
			}
		}

		if ( pWorkingDlg != NULL )
		{
			if ( pWorkingDlg->SetCaption2PercentDone( pAudit->GetPercentPosition(), TRUE ) == TRUE )
			{
				return nREPORT_CANCEL;
			}
		}

		if (ProcessLine(FALSE, dummyTime, dummyTime, dummyFlag) == FALSE)
		{
			break;
		}
	}

	return nREPORT_NOERROR;
}

//*****************************************************************************

bool CEODCashlessConsolidator::ProcessLine( bool bCheckEOD, CTime DateTimeEOD, CTime DateTimeAudit, bool& bFinished )
{
	bool bContinue = TRUE;
	int nAppNo	= m_atc.GetApplicationNo();
	int nTNo = m_atc.GetTerminalNo();

	bool bSaveReqd = FALSE;
	switch ( m_atc.GetSourceType() )
	{
	case AUDIT_SERVER:		
		ProcessServerLine(nAppNo);					
		break;

	case AUDIT_PC:			
		ProcessPcLine(nAppNo);						
		break;

	case AUDIT_KIOSKLINK:	
		bSaveReqd = ProcessKioskLine(nAppNo);		
		break;
	
	case AUDIT_POS:			
	case AUDIT_EXT:
		//FIDDLE : AUDIT EXT SHOULD HAVE OWN FUNCTION
		bSaveReqd = ProcessEcrLine(nAppNo);			
		break;

	case AUDIT_VENDING:		
		bSaveReqd = ProcessVendingLine(nAppNo);		
		break;

	case AUDIT_EODBALANCE:	
		{
			bSaveReqd = FALSE;

			if ( TRUE == bCheckEOD )
			{
				CTime DateTimeEODCutOff = DateTimeEOD + CTimeSpan( 0, 0, 1, 0 );
				
				if ( ( DateTimeAudit >= DateTimeEOD ) && ( DateTimeAudit <= DateTimeEODCutOff ) )
				{
					bContinue = FALSE;
					bFinished = TRUE;
				}
			}
		}
		break;
	}

	// see if need to save audit file for secondary report
	if ( ( TRUE == bContinue ) && ( TRUE == m_bSecondarySaveReqd ) )
	{
		if ( TRUE == bSaveReqd)
		{
			SaveSecondaryData(nTNo);										// save data in terminal set file
		}
	}

	return bContinue;
}

//*******************************************************************

void CEODCashlessConsolidator::AllocatePcLineValue(double dValue, int nPurseType, int nActionType, bool bInvertNegative)
{
	AllocatePcLineValueInternal(dValue, nPurseType, nActionType, bInvertNegative, FALSE, FALSE);
}

//*******************************************************************

void CEODCashlessConsolidator::AllocatePcLineValuePositive(double dValue, int nPurseType, int nActionType)
{
	AllocatePcLineValueInternal(dValue, nPurseType, nActionType, FALSE, TRUE, FALSE);
}

//*******************************************************************

void CEODCashlessConsolidator::AllocatePcLineValueNegative(double dValue, int nPurseType, int nActionType)
{
	AllocatePcLineValueInternal(dValue, nPurseType, nActionType, FALSE, FALSE, TRUE);
}

//*******************************************************************

void CEODCashlessConsolidator::AllocatePcLineValueInternal(double dValue, int nPurseType, int nActionType, bool bInvertNegative, bool bForcePositive, bool bForceNegative)
{
	if ((nActionType >= 0) && (nActionType < EODENTRY_TYPE_COUNT))
	{
		bool bNegative = FALSE;

		if (TRUE == bForceNegative)
		{
			bNegative = TRUE;
		}
		else if ( ( FALSE == bForcePositive ) && ( dValue < 0.0 ))
		{
			bNegative = TRUE;

			if (TRUE == bInvertNegative)
			{
				dValue *= (-1);
			}
		}

		if ( FALSE == bNegative )
		{
			switch (nPurseType)
			{
			case EOD_PURSE0:
				m_EODEntries[nActionType].m_dAddPurse0 += dValue;
				break;

			case EOD_PURSE1LIABILITY:
				m_EODEntries[nActionType].m_dAddPurse1Liability += dValue;
				break;

			case EOD_PURSE1CREDIT:
				m_EODEntries[nActionType].m_dAddPurse1Credit += dValue;
				break;

			case EOD_PURSE2:
				m_EODEntries[nActionType].m_dAddPurse2 += dValue;
				break;

			case EOD_PURSE3LIABILITY:
				m_EODEntries[nActionType].m_dAddPurse3Liability += dValue;
				break;

			case EOD_PURSE3CREDIT:
				m_EODEntries[nActionType].m_dAddPurse3Credit += dValue;
				break;
			}
		}
		else
		{
			switch (nPurseType)
			{
			case EOD_PURSE0:
				m_EODEntries[nActionType].m_dSubPurse0 += dValue;
				break;

			case EOD_PURSE1LIABILITY:
				m_EODEntries[nActionType].m_dSubPurse1Liability += dValue;
				break;

			case EOD_PURSE1CREDIT:
				m_EODEntries[nActionType].m_dSubPurse1Credit += dValue;
				break;

			case EOD_PURSE2:
				m_EODEntries[nActionType].m_dSubPurse2 += dValue;
				break;

			case EOD_PURSE3LIABILITY:
				m_EODEntries[nActionType].m_dSubPurse3Liability += dValue;
				break;

			case EOD_PURSE3CREDIT:
				m_EODEntries[nActionType].m_dSubPurse3Credit += dValue;
				break;
			}
		}
	}
}

//*******************************************************************

void CEODCashlessConsolidator::ProcessPcLine ( int nAppNo )
{
	if ( m_atc.IsDeleteAppNo() == TRUE )								// see if record deletion
	{
		m_EODEntries[EODENTRY_DELETION].m_dSubPurse1Liability += m_atc.GetCurrentPurse1Liability();
		m_EODEntries[EODENTRY_DELETION].m_dSubPurse1Credit += m_atc.GetCurrentPurse1Credit();
		m_EODEntries[EODENTRY_DELETION].m_dSubPurse2 += m_atc.GetCurrentPurse2Balance();
		m_EODEntries[EODENTRY_DELETION].m_dSubPurse3Liability += m_atc.GetCurrentPurse3Liability();
		m_EODEntries[EODENTRY_DELETION].m_dSubPurse3Credit += m_atc.GetCurrentPurse3Credit();
		return;
	}

	double dPurse1TranLiability = m_atc.GetTransactionPurse1Liability();
	double dPurse1TranCredit = m_atc.GetTransactionPurse1Credit();
	double dPurse2TranValue = m_atc.GetTransactionPurse2Balance();
	double dPurse3TranLiability = m_atc.GetTransactionPurse3Liability();
	double dPurse3TranCredit = m_atc.GetTransactionPurse3Credit();
	double dTranCash = m_atc.GetTransactionCash();

	switch ( nAppNo )
	{
	case APPNO_EDIT:		
		AllocatePcLineValue(dPurse1TranLiability, EOD_PURSE1LIABILITY, EODENTRY_ADJUST, TRUE);
		AllocatePcLineValue(dPurse1TranCredit, EOD_PURSE1CREDIT, EODENTRY_ADJUST, TRUE);
		AllocatePcLineValue(dPurse2TranValue, EOD_PURSE2, EODENTRY_ADJUST, TRUE);
		AllocatePcLineValue(dPurse3TranLiability, EOD_PURSE3LIABILITY, EODENTRY_ADJUST, TRUE);
		AllocatePcLineValue(dPurse3TranCredit, EOD_PURSE3CREDIT, EODENTRY_ADJUST, TRUE);
		break;
		
	case APPNO_ADDCASH:	
		AllocatePcLineValue(dPurse1TranLiability, EOD_PURSE1LIABILITY, EODENTRY_PCTOPUP, FALSE);
		AllocatePcLineValue(dPurse1TranCredit, EOD_PURSE1CREDIT, EODENTRY_PCTOPUP, FALSE);
		AllocatePcLineValue(dPurse2TranValue, EOD_PURSE2, EODENTRY_PCTOPUP, FALSE);
		AllocatePcLineValue(dPurse3TranLiability, EOD_PURSE3LIABILITY, EODENTRY_PCTOPUP, FALSE);
		AllocatePcLineValue(dPurse3TranCredit, EOD_PURSE3CREDIT, EODENTRY_PCTOPUP, FALSE);
		break;
		
	case APPNO_CREDIT:	
		if (System.GetCreditOptionsFlag() == TRUE)
		{
			AllocatePcLineValue(dPurse1TranCredit, EOD_PURSE1CREDIT, EODENTRY_CREDIT, FALSE);
			AllocatePcLineValue(dPurse3TranCredit, EOD_PURSE3CREDIT, EODENTRY_CREDIT, FALSE);
		}
		else
		{
			AllocatePcLineValue(dPurse1TranCredit, EOD_PURSE1CREDIT, EODENTRY_PCTOPUP, FALSE);
			AllocatePcLineValue(dPurse3TranCredit, EOD_PURSE3CREDIT, EODENTRY_PCTOPUP, FALSE);
		}
		break;
		
	case APPNO_IMPORTTOPUP:
		AllocatePcLineValue(dPurse1TranLiability, EOD_PURSE1LIABILITY, EODENTRY_IMPORTTOPUP, FALSE);
		AllocatePcLineValue(dPurse1TranCredit, EOD_PURSE1CREDIT, EODENTRY_IMPORTTOPUP, FALSE);
		AllocatePcLineValue(dPurse2TranValue, EOD_PURSE2, EODENTRY_IMPORTTOPUP, FALSE);
		AllocatePcLineValue(dPurse3TranLiability, EOD_PURSE3LIABILITY, EODENTRY_IMPORTTOPUP, FALSE);
		AllocatePcLineValue(dPurse3TranCredit, EOD_PURSE3CREDIT, EODENTRY_IMPORTTOPUP, FALSE);
		break;

	case APPNO_BATCHUPDATE:	
		AllocatePcLineValue(dPurse1TranLiability, EOD_PURSE1LIABILITY, EODENTRY_ADJUST, TRUE);
		AllocatePcLineValue(dPurse1TranCredit, EOD_PURSE1CREDIT, EODENTRY_ADJUST, TRUE);
		AllocatePcLineValue(dPurse3TranLiability, EOD_PURSE3LIABILITY, EODENTRY_ADJUST, TRUE);
		AllocatePcLineValue(dPurse3TranCredit, EOD_PURSE3CREDIT, EODENTRY_ADJUST, TRUE);
		break;

	case APPNO_PARENTPAY:
	case APPNO_WISEPAY:
	case APPNO_TUCASI:
	case APPNO_TUCASIv2:
	case APPNO_SCHOOLCOMMS:	
	case APPNO_PARENTMAIL:	
	case APPNO_SQUID:
	case APPNO_BROMCOM:
	case APPNO_PEBBLE_TOPUP:
	case APPNO_PEBBLE_TOPUP_REFUND:
		AllocatePcLineValue(dPurse1TranLiability, EOD_PURSE1LIABILITY, EODENTRY_WEBPAYMENT, FALSE);
		AllocatePcLineValue(dPurse1TranCredit, EOD_PURSE1CREDIT, EODENTRY_WEBPAYMENT, FALSE);
		AllocatePcLineValue(dPurse3TranLiability, EOD_PURSE3LIABILITY, EODENTRY_WEBPAYMENT, FALSE);
		AllocatePcLineValue(dPurse3TranCredit, EOD_PURSE3CREDIT, EODENTRY_WEBPAYMENT, FALSE);
		break;
	
	case APPNO_PEBBLE_ORDER:
	case APPNO_PEBBLE_ORDER_REFUND:
		AllocatePcLineValue(dPurse1TranLiability, EOD_PURSE1LIABILITY, EODENTRY_PEBBLEORDER, FALSE);
		AllocatePcLineValue(dPurse1TranCredit, EOD_PURSE1CREDIT, EODENTRY_PEBBLEORDER, FALSE);
		AllocatePcLineValue(dPurse3TranLiability, EOD_PURSE3LIABILITY, EODENTRY_PEBBLEORDER, FALSE);
		AllocatePcLineValue(dPurse3TranCredit, EOD_PURSE3CREDIT, EODENTRY_PEBBLEORDER, FALSE);
		break;

	case APPNO_REWARDBONUSCASH:	
		AllocatePcLineValuePositive(dPurse2TranValue, EOD_PURSE1LIABILITY, EODENTRY_SPENDBONUS);
		break;

	case APPNO_REDEEM:		
	case APPNO_BATCHREDEEM:	
		AllocatePcLineValuePositive(dPurse1TranLiability, EOD_PURSE1LIABILITY, EODENTRY_ADDEDBONUS);
		AllocatePcLineValuePositive(dPurse2TranValue, EOD_PURSE2, EODENTRY_ADDEDBONUS);
		AllocatePcLineValuePositive(dPurse3TranLiability, EOD_PURSE3LIABILITY, EODENTRY_ADDEDBONUS);
		break;

	case APPNO_REFRESH:		
		AllocatePcLineValuePositive(dPurse2TranValue, EOD_PURSE2, EODENTRY_PCTOPUP);
		break;

	case APPNO_MEAL:		
		AllocatePcLineValueNegative(-dPurse1TranLiability, EOD_PURSE1LIABILITY, EODENTRY_MEAL);
		AllocatePcLineValueNegative(-dPurse1TranCredit, EOD_PURSE1CREDIT, EODENTRY_MEAL);
		AllocatePcLineValueNegative(-dPurse2TranValue, EOD_PURSE2, EODENTRY_MEAL);
		AllocatePcLineValueNegative(-dPurse3TranLiability, EOD_PURSE3LIABILITY, EODENTRY_MEAL);
		AllocatePcLineValueNegative(-dPurse3TranCredit, EOD_PURSE3CREDIT, EODENTRY_MEAL);
		AllocatePcLineValueNegative(-dTranCash, EOD_PURSE0, EODENTRY_MEAL);
		m_dMealSales += -(dPurse1TranLiability);
		m_dMealSales += -(dPurse1TranCredit);
		m_dMealSales += -(dPurse2TranValue);
		m_dMealSales += -(dPurse3TranLiability);
		m_dMealSales += -(dPurse3TranCredit);
		m_dMealSales += -(dTranCash);
		break;

	default:				
		break;
	}
}

//*******************************************************************

bool CEODCashlessConsolidator::ProcessEcrLine ( int nAppNo )
{
	bool bReply = FALSE;

	bool bCashOnlySale = FALSE;
	double dCashValue = m_atc.GetTransactionCash();

	if ( dCashValue != 0 )
	{
		CString strUserID = m_atc.GetUserID();
		strUserID.TrimLeft ( '0' );

		if (strUserID == "")
		{
			bCashOnlySale = TRUE;
		}
	}

	double dPurse1TranLiability = m_atc.GetTransactionPurse1Liability();
	double dPurse1TranCredit = m_atc.GetTransactionPurse1Credit();
	double dPurse2TranValue = m_atc.GetTransactionPurse2Balance();
	double dPurse3TranLiability = m_atc.GetTransactionPurse3Liability();
	double dPurse3TranCredit = m_atc.GetTransactionPurse3Credit();

	switch ( nAppNo )
	{
	case APPNO_SALE:			
		if ( bCashOnlySale == TRUE )						
		{					
			m_EODEntries[EODENTRY_ECRSALE].m_dAddCash += -(dCashValue);
		}						
		else
		{
			m_EODEntries[EODENTRY_ECRSALE].m_dAddPurse1Liability += -(dPurse1TranLiability);
			m_EODEntries[EODENTRY_ECRSALE].m_dAddPurse1Credit += -(dPurse1TranCredit);
			m_EODEntries[EODENTRY_ECRSALE].m_dAddPurse2 += -(dPurse2TranValue);
			m_EODEntries[EODENTRY_ECRSALE].m_dAddPurse3Liability += -(dPurse3TranLiability);
			m_EODEntries[EODENTRY_ECRSALE].m_dAddPurse3Credit += -(dPurse3TranCredit);
			m_EODEntries[EODENTRY_ECRSALE].m_dAddPurse0 += -(dCashValue);
			AddToGroupSetTotal(&m_atc);								// add to Group Set sales
		}
								
		AddToSalesTaxes( &m_atc );			
		bReply = TRUE;						
		break;

	case APPNO_REFUNDSALE:		
		if (bCashOnlySale == TRUE)
		{
			m_EODEntries[EODENTRY_ECRSALE].m_dSubCash += -(dCashValue);
		}
		else
		{
			m_EODEntries[EODENTRY_ECRSALE].m_dSubPurse1Liability += -(dPurse1TranLiability);			// refund = +ve so make negative
			m_EODEntries[EODENTRY_ECRSALE].m_dSubPurse2 += -(dPurse2TranValue);
			m_EODEntries[EODENTRY_ECRSALE].m_dSubPurse3Liability += -(dPurse3TranLiability);
			m_EODEntries[EODENTRY_ECRSALE].m_dSubPurse0 += -(dCashValue);
		}
								
		AddToSalesTaxes ( &m_atc );									//08/12/2016 - v1.14x	
		bReply = TRUE;					
		break;


	case APPNO_ADDCASH:		
		AllocatePcLineValue(dPurse1TranLiability, EOD_PURSE1LIABILITY, EODENTRY_ECRTOPUP, FALSE);
		AllocatePcLineValue(dPurse2TranValue, EOD_PURSE2, EODENTRY_ECRTOPUP, FALSE);
		AllocatePcLineValue(dPurse3TranLiability, EOD_PURSE3LIABILITY, EODENTRY_ECRTOPUP, FALSE);
		bReply = TRUE;
		break;

	case APPNO_REFRESH:			
		m_EODEntries[EODENTRY_ECRTOPUP].m_dAddPurse2 += dPurse2TranValue;
		bReply = TRUE;
		break;
	}

	return bReply;
}

//******************************************************************
// TO DO

bool CEODCashlessConsolidator::ProcessVendingLine ( int nAppNo )
{
	bool bReply = FALSE;

	double dCashValue = m_atc.GetTransactionCash();
	double dPurse1TranLiability = m_atc.GetTransactionPurse1Liability();
	double dPurse1TranCredit = m_atc.GetTransactionPurse1Credit();
	double dPurse2TranValue = m_atc.GetTransactionPurse2Balance();
	double dPurse3TranLiability = m_atc.GetTransactionPurse3Liability();
	double dPurse3TranCredit = m_atc.GetTransactionPurse3Credit();

	switch ( nAppNo )
	{
	case APPNO_SALE:	
		m_EODEntries[EODENTRY_ECRSALE].m_dSubPurse1Liability -= dPurse1TranLiability;
		m_EODEntries[EODENTRY_ECRSALE].m_dSubPurse1Credit -= dPurse1TranCredit;
		m_EODEntries[EODENTRY_ECRSALE].m_dSubPurse2 -= dPurse2TranValue;
		m_EODEntries[EODENTRY_ECRSALE].m_dSubPurse3Liability -= dPurse3TranLiability;
		m_EODEntries[EODENTRY_ECRSALE].m_dSubPurse3Credit -= dPurse3TranCredit;
		m_EODEntries[EODENTRY_ECRSALE].m_dSubPurse0 -= dCashValue;
		/****/
		m_dVendingSales -= dPurse1TranLiability;
		m_dVendingSales -= dPurse1TranCredit;
		m_dVendingSales -= dPurse2TranValue;
		m_dVendingSales -= dPurse3TranLiability;
		m_dVendingSales -= dPurse3TranCredit;
		m_dVendingSales -= dCashValue;
		bReply = TRUE;
		break;
	}
	return bReply;
}

//******************************************************************

bool CEODCashlessConsolidator::ProcessKioskLine ( int nAppNo )
{
	bool bReply = FALSE;

	double dPurse1TranValue = m_atc.GetTransactionPurse1Total();
	double dPurse3TranValue = m_atc.GetTransactionPurse3Total();

	switch ( nAppNo )
	{
	case APPNO_ADDCASH:		
		if ( dPurse1TranValue > 0.0 )
		{
			m_EODEntries[EODENTRY_KIOSKADDED].m_dAddPurse1Liability += dPurse1TranValue;
			bReply = TRUE;
		}

		if ( dPurse3TranValue > 0.0 )
		{
			m_EODEntries[EODENTRY_KIOSKADDED].m_dAddPurse3Liability += dPurse3TranValue;
			bReply = TRUE;
		}

		break;
	}
	return bReply;
}

//******************************************************************

void CEODCashlessConsolidator::ProcessServerLine ( int nAppNo )
{
	double dPurse1TranValue = m_atc.GetTransactionPurse1Total();
	double dPurse2TranValue = m_atc.GetTransactionPurse2Balance();
	double dPurse3TranValue = m_atc.GetTransactionPurse3Total();

	switch ( nAppNo )
	{
	case APPNO_REFRESH:		
		m_EODEntries[EODENTRY_ECRTOPUP].m_dAddPurse2 += dPurse2TranValue;
		break;

	case APPNO_REDEEM:					
		m_EODEntries[EODENTRY_ADDEDBONUS].m_dAddPurse1Liability += dPurse1TranValue;
		m_EODEntries[EODENTRY_ADDEDBONUS].m_dAddPurse2 += dPurse2TranValue;
		m_EODEntries[EODENTRY_ADDEDBONUS].m_dAddPurse3Liability += dPurse3TranValue;
		break;

	case APPNO_REVALUEBONUSCASH:		// NB: Purse2Transaction field holds the bonus value
	case APPNO_REWARDBONUSCASH:			
		m_EODEntries[EODENTRY_SPENDBONUS].m_dAddPurse1Liability += dPurse2TranValue;
		break;

	case APPNO_REFUNDREWARDBONUSCASH:	
		m_EODEntries[EODENTRY_SPENDBONUS].m_dSubPurse1Liability += dPurse2TranValue;
		break;
	}
}

//**********************************************************************

void CEODCashlessConsolidator::CalculateTotals()
{
	for (int n = 0; n < EODENTRY_TYPE_COUNT; n++)
	{
		switch (n)
		{
		case EODENTRY_ECRSALE:
			m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE1LIABILITY] += m_EODEntries[n].m_dAddPurse1Liability;
			m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE1CREDIT] += m_EODEntries[n].m_dAddPurse1Credit;
			m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE2] += m_EODEntries[n].m_dAddPurse2;
			m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE3LIABILITY] += m_EODEntries[n].m_dAddPurse3Liability;
			m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE3CREDIT] += m_EODEntries[n].m_dAddPurse3Credit;
			break;

		default:
			m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE1LIABILITY] += m_EODEntries[n].m_dAddPurse1Liability;
			m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE1CREDIT] += m_EODEntries[n].m_dAddPurse1Credit;
			m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE2] += m_EODEntries[n].m_dAddPurse2;
			m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE3LIABILITY] += m_EODEntries[n].m_dAddPurse3Liability;
			m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE3CREDIT] += m_EODEntries[n].m_dAddPurse3Credit;
			break;
		}
		
		/*****/

		switch (n)
		{
		case EODENTRY_ECRTOPUP:
		case EODENTRY_PCTOPUP:
		case EODENTRY_IMPORTTOPUP:
		case EODENTRY_CREDIT:
		case EODENTRY_SPENDBONUS:
		case EODENTRY_WEBPAYMENT:
		case EODENTRY_PEBBLEORDER:
			m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE1LIABILITY] += m_EODEntries[n].m_dSubPurse1Liability;
			m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE1CREDIT] += m_EODEntries[n].m_dSubPurse1Credit;
			m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE2] += m_EODEntries[n].m_dSubPurse2;
			m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE3LIABILITY] += m_EODEntries[n].m_dSubPurse3Liability;
			m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE3CREDIT] += m_EODEntries[n].m_dSubPurse3Credit;
			break;

		default:
			m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE1LIABILITY] += m_EODEntries[n].m_dSubPurse1Liability;
			m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE1CREDIT] += m_EODEntries[n].m_dSubPurse1Credit;
			m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE2] += m_EODEntries[n].m_dSubPurse2;
			m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE3LIABILITY] += m_EODEntries[n].m_dSubPurse3Liability;
			m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE3CREDIT] += m_EODEntries[n].m_dSubPurse3Credit;
			break;
		}

		/*****/
		m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_CASH] += m_EODEntries[n].m_dAddPurse0;
	}

	m_dTotalSubCash += m_EODEntries[EODENTRY_ECRSALE].m_dAddCash;
	m_dTotalSubCash += m_EODEntries[EODENTRY_ECRSALE].m_dSubCash;

	GetPossibleCount();
}

//********************************************************************

void CEODCashlessConsolidator::AddToGroupSetTotal ( CSQLAuditRecord* pAtc )
{
	double dCashValue = m_atc.GetTransactionCash();
	double dPurse1TranLiability = m_atc.GetTransactionPurse1Liability();
	double dPurse1TranCredit = m_atc.GetTransactionPurse1Credit();
	double dPurse2TranValue = m_atc.GetTransactionPurse2Balance();
	double dPurse3TranLiability = m_atc.GetTransactionPurse3Liability();
	double dPurse3TranCredit = m_atc.GetTransactionPurse3Credit();

	int nGroupSetNo = 0;

	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(pAtc->GetGroupNo());

	CSQLRepositoryGroup RepoGroup;
	if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		nGroupSetNo = RowGroup.GetSetNo();
	}
	
	if (m_dbCardCounter.AddCard(m_atc.GetUserID()) == TRUE)
	{
		++m_GroupSet[nGroupSetNo].m_nHeadCount;
	}

	m_GroupSet[nGroupSetNo].m_dPurse1Liability += -(dPurse1TranLiability);
	m_GroupSet[nGroupSetNo].m_dPurse1Credit += -(dPurse1TranCredit);
	m_GroupSet[nGroupSetNo].m_dPurse2 += -(dPurse2TranValue);
	m_GroupSet[nGroupSetNo].m_dPurse3Liability += -(dPurse3TranLiability);
	m_GroupSet[nGroupSetNo].m_dPurse3Credit += -(dPurse3TranCredit);
	m_GroupSet[nGroupSetNo].m_dCash += -(dCashValue);
}

//********************************************************************

void CEODCashlessConsolidator::GetPossibleCount()
{
	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NoParams{}, TRUE);

	int nGroupSetNo = 0;
	CSQLRowAccountFull RowAccount;
	while (RecordSet.StepSelectAll(RowAccount,nGroupSetNo) == TRUE)
	{
		if (RowAccount.HasUserExpired() == TRUE)
		{
			continue;
		}

		if (RowAccount.GetInactive() == TRUE)
		{
			continue;
		}

		if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)			// UserID = Mifare number
		{
			if (RowAccount.IsAlertSet() == TRUE)					// Hotlisted
			{
				continue;
			}
		}

		if (System.GetInterfaceType() == nINTERFACE_MIFAREv2)			//26/04/2016 - v1.11c
		{
			if (RowAccount.GetExternalRef1() == "")						// not issued
			{
				continue;
			}
		}

		++m_GroupSet[nGroupSetNo].m_nPossibleCount;
	}
}

//********************************************************************

void CEODCashlessConsolidator::AddToSalesTaxes(CSQLAuditRecord* pAtc)
{
	if (pAtc->ExtractSalesTaxes(m_strNoSalesFilename) == TRUE)
	{
		for (int i = 0; i < nMAX_TAXCODES; i++)
		{
			m_dSales[i] += pAtc->m_dSales[i];
		}

		m_dSalesUnknown += pAtc->m_dSalesUnknown;				// sales with no VAT ( eg Discount )
	}
	else
	{
		m_dNotFoundSales += pAtc->m_dNotFoundSales;				// no purchase history found
	}
}

//*******************************************************************

void CEODCashlessConsolidator::SaveSecondaryData ( int nTno )
{
	if ( m_TNoDatabase.FindRecord ( nTno ) == TRUE )
	{
		CSSFile file;
		if ( file.Open ( GetSecondaryFilename ( m_TNoDatabase.GetSetNo() ) , "ab" ) == TRUE )
		{
			m_atc.WriteLineToFile( &file );
			file.Close();
		}
	}
}

//*******************************************************************

CString CEODCashlessConsolidator::GetSecondaryFilename ( int nSetNo )
{
	CString strExtension = "";
	strExtension.Format ( "%3.3d", nSetNo );

	CString strFilename = Filenames.GetTempReportPath ( "SNoList", strExtension );
	return strFilename;
}

//*******************************************************************
