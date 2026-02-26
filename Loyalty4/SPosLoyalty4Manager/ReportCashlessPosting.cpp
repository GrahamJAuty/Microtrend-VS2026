/******************************************************************************/
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRecordSetAccountFull.h"
/******************************************************************************/
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "RecSort.h"
#include "RepmanHandler.h"
#include "WorkingDlg.h"
/******************************************************************************/
#include "ReportCashlessPosting.h"
/******************************************************************************/

CReportCashlessPosting::CReportCashlessPosting(const char* szLabel, CWnd* pParent)
	: m_dlgCashlessFilter(this, pParent)
	, m_ReportInfo(szLabel, FALSE)
{
	m_strReportLabel = szLabel;
	m_pParent = pParent;
	m_bShowPossibleSpend = System.GetEnablePurse2Flag();
	Reset();
}

/**********************************************************************/

CReportCashlessPosting::~CReportCashlessPosting()
{
}

/**********************************************************************/

void CReportCashlessPosting::Reset()
{
	m_bCancelButton = FALSE;

	for (int s = 0; s < CASHLESS_SOURCE_CNT; s++)
	{
		for (int a = 0; a < CASHLESS_ACTION_CNT; a++)
		{
			m_arrayTotals[s][a].Reset();
		}
	}

	m_dDailySpend = 0.0;
	m_dPossibleSpend = 0.0;
	m_dDatabasePossibleSpend = 0.0;

	for (int i = 0; i < 3; i++)
	{
		m_dTotalAdd[i] = 0.0;
		m_dTotalSub[i] = 0.0;
	}

	m_arrayGroupDailyRefresh.RemoveAll();
}

/******************************************************************************/

void CReportCashlessPosting::Show()
{
	CLoyaltyAuditFile auditFile;							
	int nErrorNo = auditFile.ValidateFile();				
	
	if ( nErrorNo == nREPORT_NOERROR )
	{
		m_dlgCashlessFilter.SetReportInfo(&m_ReportInfo);
		m_dlgCashlessFilter.DoModal();
		nErrorNo = m_dlgCashlessFilter.GetFinalError();
	}
	
	CRepmanHandler repman ( m_ReportInfo.GetReportTitle(), m_pParent );
	repman.DisplayError ( nErrorNo );
}

//*******************************************************************

int CReportCashlessPosting::ShowInternal()
{
	m_ReportInfo.SetCutoffTimeFlag(FALSE);

	CString strReportFile = Filenames.GetReportFilename ( m_strReportLabel );
	CString strParamsFile = Filenames.GetReportParamsFilename ( m_strReportLabel );

	CLoyaltyAuditFile auditFile;							// main audit file

	int nErrorNo = auditFile.ValidateFile();				// see if audit file ok to report on
	if (nErrorNo == nREPORT_NOERROR)
	{
		if (m_Report.Open(strReportFile) == TRUE)
		{
			CString strTitle = m_ReportInfo.GetReportTitle();
			strTitle += m_ReportInfo.GetDateRangeString();
			m_Report.SetStyle1(strTitle);

			AddColumnText();

			if (auditFile.OpenReadOnly() == TRUE)			// open main audit file
			{
				CompileReport(&auditFile);	// scan audit file & compile totals

				if (TRUE == m_bShowPossibleSpend)
				{
					CalculateDatabaseSpend();
				}

				auditFile.Close();

				if (m_bCancelButton == TRUE)
				{
					nErrorNo = nREPORT_CANCEL;
				}
				else
				{
					ShowTotals();
				}
			}
			else
			{
				nErrorNo = nREPORT_OPENERROR;
			}

			m_Report.Close();
		}
		else
		{
			nErrorNo = nREPORT_CREATEFAIL;
		}
	}

	if ( nREPORT_NOERROR == nErrorNo )
	{
		CRepmanHandler repman ( m_ReportInfo.GetReportTitle(), m_pParent );
		nErrorNo = repman.DisplayReport( strReportFile, strParamsFile );
	}

	return nErrorNo;
}

//*****************************************************************************
// also called to by cashless liability to calculate adds/subs between dates

void CReportCashlessPosting::CompileReport ( CLoyaltyAuditFile* pAudit )
{
	CWorkingDlg WorkingDlg ( m_ReportInfo.GetReportTitle() );
	WorkingDlg.Create();
	Reset();

	CRecordSorter sorter;									// hold count of run of free meal users

	pAudit->JumpIn ( m_ReportInfo.GetDatePreviousString() );
	while ( pAudit->ReadLine ( &m_atc ) == TRUE )
	{
		if (WorkingDlg.SetCaption2PercentDone(pAudit->GetPercentPosition(), TRUE) == TRUE)
		{
			m_bCancelButton = TRUE;
			break;
		}

		CString strDate = "";
		CString strTime = "";

		switch (m_atc.GetSourceType())
		{
		case AUDIT_POS:
		case AUDIT_EXT:
			strDate = m_atc.GetDateLastUsed();
			strTime = m_atc.GetTimeLastUsed();
			break;

		default:
			strDate = m_atc.GetDateFiled();
			strTime = m_atc.GetTimeFiled();
			break;
		}

		if ( m_ReportInfo.IsValidReportDate( strDate, strTime ) == FALSE )
		{
			continue;
		}

		ProcessLine();

		if ( m_bShowPossibleSpend == TRUE )					// see if show possible freemeal spend reqd
		{
			switch (m_atc.GetSourceType())
			{
			case AUDIT_POS:
			case AUDIT_EXT:
				if ((m_atc.GetApplicationNo() == APPNO_SALE) && (m_atc.GetPurse2Transaction() != 0.0))	// sale from purse2 
				{
					if (GetDailyRefreshValue(m_atc.GetGroupNo()) != 0.0)	// See if daily refresh
					{
						CString strLabel;
						strLabel.Format("%2.2d%s", 
							m_atc.GetGroupNo(), 
							(const char*) System.FormatCardNo(m_atc.GetCardNo()));

						sorter.Consolidate(strLabel, 0L);
					}
				}
				break;
			}
		}
	}

	CalculateTotals();											// calc purse totals

	if ( m_bShowPossibleSpend == TRUE )
	{
		for ( int nIndex = 0 ; nIndex < sorter.GetSize() ; nIndex++ )
		{
			CString strLabel = sorter.GetLabel(nIndex);
			int nGroupNo = atoi ( strLabel.Left(2) );
			m_dPossibleSpend += GetDailyRefreshValue ( nGroupNo );
		}
	}
}

//*****************************************************************************

void CReportCashlessPosting::ProcessLine() 
{
	int nAppNo = m_atc.GetApplicationNo();

	switch ( m_atc.GetSourceType() )
	{
	case AUDIT_SERVER:		
		ProcessServerLine(nAppNo);	
		break;

	case AUDIT_PC:			
		ProcessPcLine(nAppNo);		
		break;

	case AUDIT_POS:
	case AUDIT_EXT:
		ProcessEcrOrExtLine(nAppNo);		
		break;
	}
}

//*******************************************************************

void CReportCashlessPosting::Consolidate(int nSource, int nAction, int nPurseNo, double dAmount)
{
	if (nSource < 0 || nSource >= CASHLESS_SOURCE_CNT)
	{
		return;
	}

	if (nAction < 0 || nAction >= CASHLESS_ACTION_CNT)
	{
		return;
	}

	m_arrayTotals[nSource][nAction].Consolidate(nPurseNo, dAmount);
}

//*******************************************************************

void CReportCashlessPosting::ConsolidatePlus(int nSource, int nAction, int nPurseNo, double dAmount)
{
	if (nSource < 0 || nSource >= CASHLESS_SOURCE_CNT)
	{
		return;
	}

	if (nAction < 0 || nAction >= CASHLESS_ACTION_CNT)
	{
		return;
	}

	m_arrayTotals[nSource][nAction].ConsolidatePlus(nPurseNo, dAmount);
}

//*******************************************************************

void CReportCashlessPosting::ConsolidateMinus(int nSource, int nAction, int nPurseNo, double dAmount)
{
	if (nSource < 0 || nSource >= CASHLESS_SOURCE_CNT)
	{
		return;
	}

	if (nAction < 0 || nAction >= CASHLESS_ACTION_CNT)
	{
		return;
	}

	m_arrayTotals[nSource][nAction].ConsolidateMinus(nPurseNo, dAmount);
}

//*******************************************************************

void CReportCashlessPosting::ProcessPcLine ( int nAppNo ) 
{
	double dPurse1Amount = m_atc.GetPurse1Transaction();
	double dPurse2Amount = m_atc.GetPurse2Transaction();

	if (nAppNo == APPNO_MANUAL_REFUND)
	{
		if (m_ReportInfo.GetRefundProcessType() != 0)
		{
			nAppNo = APPNO_MANUAL_SPEND;
		}
	}

	switch (nAppNo)
	{
	case APPNO_EDIT:
	case APPNO_BATCHUPDATE:
		Consolidate(CASHLESS_SOURCE_PC, CASHLESS_ACTION_ADJUST, 1, dPurse1Amount);
		Consolidate(CASHLESS_SOURCE_PC, CASHLESS_ACTION_ADJUST, 2, dPurse2Amount);
		break;

	case APPNO_PURSE1_TOPUP1:
	case APPNO_PURSE1_TOPUP2:
	case APPNO_PURSE1_TOPUP3:
	case APPNO_PURSE1_TOPUP4:
	case APPNO_PURSE1_TOPUP5:
	case APPNO_PURSE2_TOPUP1:
	case APPNO_PURSE2_TOPUP2:
	case APPNO_PURSE2_TOPUP3:
	case APPNO_PURSE2_TOPUP4:
	case APPNO_PURSE2_TOPUP5:
	case APPNO_PURSE1_MANTOPUP:
	case APPNO_PURSE2_MANTOPUP:
	case APPNO_PURSE1_IMPTOPUP:
	case APPNO_PURSE2_IMPTOPUP:
	case APPNO_SUBCASH:
		Consolidate(CASHLESS_SOURCE_PC, CASHLESS_ACTION_TOPUP, 1, dPurse1Amount);
		Consolidate(CASHLESS_SOURCE_PC, CASHLESS_ACTION_TOPUP, 2, dPurse2Amount);
		break;

	case APPNO_REVALUEBONUSCASH:
		Consolidate(CASHLESS_SOURCE_PC, CASHLESS_ACTION_TOPUPBONUS, 1, dPurse1Amount);
		Consolidate(CASHLESS_SOURCE_PC, CASHLESS_ACTION_TOPUPBONUS, 2, dPurse2Amount);
		break;

	case APPNO_REDEEM:
	case APPNO_BATCHREDEEM:		
		ConsolidatePlus(CASHLESS_SOURCE_PC, CASHLESS_ACTION_REDEEM, 1, dPurse1Amount);
		ConsolidatePlus(CASHLESS_SOURCE_PC, CASHLESS_ACTION_REDEEM, 2, dPurse2Amount);
		break;

	case APPNO_REWARDBONUSCASH:	
		ConsolidatePlus(CASHLESS_SOURCE_PC, CASHLESS_ACTION_SPENDBONUS, 1, dPurse1Amount);
		ConsolidatePlus(CASHLESS_SOURCE_PC, CASHLESS_ACTION_SPENDBONUS, 2, dPurse2Amount);
		break;

	case APPNO_REFUNDREWARDBONUSCASH:
		ConsolidateMinus(CASHLESS_SOURCE_PC, CASHLESS_ACTION_SPENDBONUS, 1, dPurse1Amount);
		ConsolidateMinus(CASHLESS_SOURCE_PC, CASHLESS_ACTION_SPENDBONUS, 2, dPurse2Amount);
		break;

	case APPNO_MANUAL_SPEND:
		ConsolidateMinus(CASHLESS_SOURCE_PC, CASHLESS_ACTION_MANUALSPEND, 1, dPurse1Amount);
		break;

	case APPNO_MANUAL_REFUND:
		ConsolidatePlus(CASHLESS_SOURCE_PC, CASHLESS_ACTION_MANUALSPEND, 1, dPurse1Amount);
		break;

	case APPNO_XFERIN:
	case APPNO_HOTXFERIN:
		ConsolidatePlus(CASHLESS_SOURCE_PC, CASHLESS_ACTION_TRANSFER, 1, dPurse1Amount);
		ConsolidatePlus(CASHLESS_SOURCE_PC, CASHLESS_ACTION_TRANSFER, 2, dPurse2Amount);
		break;

	case APPNO_XFEROUT:
		case APPNO_HOTXFEROUT:
		ConsolidateMinus(CASHLESS_SOURCE_PC, CASHLESS_ACTION_TRANSFER, 1, dPurse1Amount);
		ConsolidateMinus(CASHLESS_SOURCE_PC, CASHLESS_ACTION_TRANSFER, 2, dPurse2Amount);
		break;

	default:					
		break;
	}
}

//*******************************************************************

void CReportCashlessPosting::ProcessEcrOrExtLine ( int nAppNo ) 
{
	double dPurse1Amount = m_atc.GetPurse1Transaction();
	double dPurse2Amount = m_atc.GetPurse2Transaction();

	if ( nAppNo == APPNO_REFUNDSALE)
	{
		if (m_ReportInfo.GetRefundProcessType() != 0)
		{
			nAppNo = APPNO_SALE;
		}
	}

	switch (nAppNo)
	{
	case APPNO_SALE:
		ConsolidateMinus(CASHLESS_SOURCE_ECR, CASHLESS_ACTION_SALE, 1, dPurse1Amount);

		if (dPurse2Amount != 0.0)
		{
			ConsolidateMinus(CASHLESS_SOURCE_ECR, CASHLESS_ACTION_SALE, 2, dPurse2Amount);

			if (GetDailyRefreshValue(m_atc.GetGroupNo()) != 0.0)
			{
				m_dDailySpend -= dPurse2Amount;
			}
		}
		break;

	case APPNO_REFUNDSALE:
		ConsolidatePlus(CASHLESS_SOURCE_ECR, CASHLESS_ACTION_SALE, 1, dPurse1Amount);

		if (dPurse2Amount != 0.0)
		{
			ConsolidatePlus(CASHLESS_SOURCE_ECR, CASHLESS_ACTION_SALE, 2, dPurse2Amount);

			if (GetDailyRefreshValue(m_atc.GetGroupNo()) != 0.0)
			{
				m_dDailySpend -= dPurse2Amount;
			}
		}
		break;

	case APPNO_ADDGIFT:
	case APPNO_ADDGIFTX:
	case APPNO_SUBCASH:
		Consolidate(CASHLESS_SOURCE_ECR, CASHLESS_ACTION_TOPUP, 1, dPurse1Amount);
		Consolidate(CASHLESS_SOURCE_ECR, CASHLESS_ACTION_TOPUP, 2, dPurse2Amount);
		break;

	case APPNO_REFRESH:
		ConsolidatePlus(CASHLESS_SOURCE_ECR, CASHLESS_ACTION_REFRESH, 2, dPurse2Amount);
		break;

	default:
		{
			int nPurseNo = 0;
			int nTopupNo = 0;
			CAuditReportTexts::GetAddCashIndexByAppNo(nAppNo, nPurseNo, nTopupNo);

			if ((1 == nPurseNo) || (2 == nPurseNo))
			{
				Consolidate(CASHLESS_SOURCE_ECR, CASHLESS_ACTION_TOPUP, 1, dPurse1Amount);
				Consolidate(CASHLESS_SOURCE_ECR, CASHLESS_ACTION_TOPUP, 2, dPurse2Amount);
			}
		}
		break;
	}
}

//*******************************************************************

void CReportCashlessPosting::ProcessServerLine ( int nAppNo ) 
{
	double dPurse1Amount = m_atc.GetPurse1Transaction();
	double dPurse2Amount = m_atc.GetPurse2Transaction();

	switch ( nAppNo )
	{
	case APPNO_REFRESH:				
		ConsolidatePlus(CASHLESS_SOURCE_SRV, CASHLESS_ACTION_REFRESH, 2, dPurse2Amount);
		break;

	case APPNO_REDEEM:	
		ConsolidatePlus(CASHLESS_SOURCE_SRV, CASHLESS_ACTION_REDEEM, 1, dPurse1Amount);
		ConsolidatePlus(CASHLESS_SOURCE_SRV, CASHLESS_ACTION_REDEEM, 2, dPurse2Amount);
		break;

	case APPNO_REVALUEBONUSCASH:	
		Consolidate(CASHLESS_SOURCE_SRV, CASHLESS_ACTION_TOPUPBONUS, 1, dPurse1Amount);
		Consolidate(CASHLESS_SOURCE_SRV, CASHLESS_ACTION_TOPUPBONUS, 2, dPurse2Amount);
		break;

	case APPNO_REWARDBONUSCASH:		
		ConsolidatePlus(CASHLESS_SOURCE_SRV, CASHLESS_ACTION_SPENDBONUS, 1, dPurse1Amount);
		ConsolidatePlus(CASHLESS_SOURCE_SRV, CASHLESS_ACTION_SPENDBONUS, 2, dPurse2Amount);
		break;

	case APPNO_REFUNDREWARDBONUSCASH:	
		ConsolidateMinus(CASHLESS_SOURCE_SRV, CASHLESS_ACTION_SPENDBONUS, 1, dPurse1Amount);
		ConsolidateMinus(CASHLESS_SOURCE_SRV, CASHLESS_ACTION_SPENDBONUS, 2, dPurse2Amount);
		break;

	case APPNO_DEPTBONUSCASH:
		ConsolidatePlus(CASHLESS_SOURCE_SRV, CASHLESS_ACTION_DEPTBONUS, 1, dPurse1Amount);
		ConsolidatePlus(CASHLESS_SOURCE_SRV, CASHLESS_ACTION_DEPTBONUS, 2, dPurse2Amount);
		break;

	case APPNO_REFUNDDEPTBONUSCASH:
		ConsolidateMinus(CASHLESS_SOURCE_SRV, CASHLESS_ACTION_DEPTBONUS, 1, dPurse1Amount);
		ConsolidateMinus(CASHLESS_SOURCE_SRV, CASHLESS_ACTION_DEPTBONUS, 2, dPurse2Amount);
		break;
	}
}

//**********************************************************************

void CReportCashlessPosting::CalculateTotals()
{
	for (int s = 0; s < CASHLESS_SOURCE_CNT; s++)
	{
		for (int a = 0; a < CASHLESS_ACTION_CNT; a++)
		{
			// TRANSFERS SHOULD CANCEL EACH OTHER OUT SO NOT INCLUDED IN TOTALS
			if (a != CASHLESS_ACTION_TRANSFER)
			{
				m_dTotalAdd[1] += m_arrayTotals[s][a].m_dPurse1Plus;
				m_dTotalAdd[2] += m_arrayTotals[s][a].m_dPurse2Plus;
				m_dTotalSub[1] += m_arrayTotals[s][a].m_dPurse1Minus;
				m_dTotalSub[2] += m_arrayTotals[s][a].m_dPurse2Minus;
			}
		}
	}
}

//**********************************************************************

CString CReportCashlessPosting::GetSourceText(int s)
{
	switch (s)
	{
	case CASHLESS_SOURCE_ECR:	
		return "SPOS";

	case CASHLESS_SOURCE_PC:	
		return "LOYALTY MANAGER";

	case CASHLESS_SOURCE_SRV:	
		return "LOYALTY SERVER";

	case CASHLESS_SOURCE_EXT:	
		return "EXTERNAL";

	default:				
		return "UNKNOWN";
	}
}

//**********************************************************************

CString CReportCashlessPosting::GetPlusActionText(int a)
{
	switch (a)
	{
	case CASHLESS_ACTION_SALE:	
		return "Refund Sale";

	case CASHLESS_ACTION_MANUALSPEND:
		return "Refund Manual Spend";

	case CASHLESS_ACTION_TOPUP:	
		return "Topup";

	case CASHLESS_ACTION_TOPUPBONUS:
		return "Topup Bonus";

	case CASHLESS_ACTION_SPENDBONUS:
		return "Spend Bonus";

	case CASHLESS_ACTION_DEPTBONUS:
		return "Dept Spend Bonus";

	case CASHLESS_ACTION_REDEEM:
		return "Redeem Points";

	case CASHLESS_ACTION_ADJUST:	
		return "Manual Adjustment";

	case CASHLESS_ACTION_REFRESH:
		return "Refresh Purse";

	case CASHLESS_ACTION_TRANSFER:
		return "Transfer In";

	default:
		return "Unknown";
	}
}

//**********************************************************************

CString CReportCashlessPosting::GetMinusActionText(int a)
{
	switch (a)
	{
	case CASHLESS_ACTION_SALE:	
		return "Sale";

	case CASHLESS_ACTION_MANUALSPEND:
		return "Manual Spend";

	case CASHLESS_ACTION_TOPUP:
		return "Refund Top-up";

	case CASHLESS_ACTION_TOPUPBONUS:
		return "Refund Top-up Bonus";

	case CASHLESS_ACTION_SPENDBONUS:
		return "Refund Spend Bonus";

	case CASHLESS_ACTION_DEPTBONUS:
		return "Refund Dept Spend Bonus";

	case CASHLESS_ACTION_ADJUST:
		return "Manual Adjustment";

	case CASHLESS_ACTION_TRANSFER:
		return "Transfer Out";

	default:					
		return "UNKNOWN";
	}
}

//**********************************************************************

void CReportCashlessPosting::ShowTotals()
{
	CString strMsg = "";
	CString strLine = "";

	m_Report.WriteLine ( "<..>Additions" );
	m_Report.WriteLine ( "<LI>" );

	bool bGotLine = FALSE;
	
	for (int s = 0; s < CASHLESS_SOURCE_CNT; s++)
	{
		CString strSource = GetSourceText(s);
		bool bGotLineThisSource = FALSE;

		for (int a = 0; a < CASHLESS_ACTION_CNT; a++)
		{
			// Skip transfers in additions section
			if ( a == CASHLESS_ACTION_TRANSFER )
			{
				continue;
			}

			CString strAction = GetPlusActionText(a);

			double dPurse1 = m_arrayTotals[s][a].m_dPurse1Plus;
			double dPurse2 = m_arrayTotals[s][a].m_dPurse2Plus;

			bool bShowLine = ((dPurse1 != 0.0) || (dPurse2 != 0.0));

			if ((s == CASHLESS_SOURCE_ECR) && (a == CASHLESS_ACTION_TOPUP))
			{
				bShowLine = TRUE;
			}

			if (TRUE == bShowLine)
			{
				if ((TRUE == bGotLine) && (FALSE == bGotLineThisSource))
				{
					m_Report.WriteLine("");
				}

				CString strSourceToUse = "";
				if ( FALSE == bGotLineThisSource )
				{
					strSourceToUse = strSource;
				}

				CreateLine(strLine, strSourceToUse, strAction, dPurse1, dPurse2);
				m_Report.WriteLine(strLine);
				bGotLine = TRUE;
				bGotLineThisSource = TRUE;
			}
		}
	}

	ShowLines();
	CreateLine(strLine, "<..>Total Additions", "", m_dTotalAdd[1], m_dTotalAdd[2]);
	m_Report.WriteLine(strLine);

	m_Report.WriteLine("");
	m_Report.WriteLine("");

	m_Report.WriteLine("<..>Deductions");
	m_Report.WriteLine("<LI>");

	bGotLine = FALSE;
	for (int s = 0; s < CASHLESS_SOURCE_CNT; s++)
	{
		CString strSource = GetSourceText(s);
		bool bGotLineThisSource = FALSE;

		for (int a = 0; a < CASHLESS_ACTION_CNT; a++)
		{
			// Skip transfers in deductions section
			if (a == CASHLESS_ACTION_TRANSFER)
			{
				continue;
			}

			CString strAction = GetMinusActionText(a);

			double dPurse1 = m_arrayTotals[s][a].m_dPurse1Minus;
			double dPurse2 = m_arrayTotals[s][a].m_dPurse2Minus;

			bool bShowLine = ((dPurse1 != 0.0) || (dPurse2 != 0.0));

			if ((s == CASHLESS_SOURCE_ECR) && (a == CASHLESS_ACTION_SALE))
			{
				bShowLine = TRUE;
			}

			if (TRUE == bShowLine)
			{
				if ((TRUE == bGotLine) && (FALSE == bGotLineThisSource))
				{
					m_Report.WriteLine("");
				}

				CString strSourceToUse = "";
				if ( FALSE == bGotLineThisSource)
				{
					strSourceToUse = strSource;
				}

				CreateLine(strLine, strSourceToUse, strAction, dPurse1, dPurse2);
				m_Report.WriteLine(strLine);
				bGotLine = TRUE;
				bGotLineThisSource = TRUE;

			}
		}
	}

	ShowLines();
	CreateLine(strLine, "<..>Total Deductions", "", m_dTotalSub[1], m_dTotalSub[2]);
	m_Report.WriteLine(strLine);
	
	m_Report.WriteLine("");
	m_Report.WriteLine("");

	m_Report.WriteLine("<..>Transfers");
	m_Report.WriteLine("<LI>");

	{
		double dPurse1 = m_arrayTotals[CASHLESS_SOURCE_PC][CASHLESS_ACTION_TRANSFER].m_dPurse1Plus;
		double dPurse2 = m_arrayTotals[CASHLESS_SOURCE_PC][CASHLESS_ACTION_TRANSFER].m_dPurse2Plus;
		CreateLine(strLine, "<..>Total Xfer In", "", dPurse1, dPurse2);
		m_Report.WriteLine(strLine);
	}

	{
		double dPurse1 = m_arrayTotals[CASHLESS_SOURCE_PC][CASHLESS_ACTION_TRANSFER].m_dPurse1Minus;
		double dPurse2 = m_arrayTotals[CASHLESS_SOURCE_PC][CASHLESS_ACTION_TRANSFER].m_dPurse2Minus;
		CreateLine(strLine, "<..>Total Xfer Out", "", dPurse1, dPurse2);
		m_Report.WriteLine(strLine);
	}


	if (System.GetEnablePurse2Flag() == TRUE)
	{
		if (m_bShowPossibleSpend)
		{
			m_Report.WriteLine("");
			m_Report.WriteLine("");

			m_Report.WriteLine("<..>Daily Refresh Spend");
			m_Report.WriteLine("<LI>");

			CString strLine = "";

			strLine.Format("<..>Actual Spend\t\t%.*lf",
				System.GetDPValue(),
				m_dDailySpend);

			m_Report.WriteLine(strLine);
		}

		strLine.Format("<..>Possible Spend\t\t%.*lf",
			System.GetDPValue(),
			m_dPossibleSpend);

		m_Report.WriteLine(strLine);

		strLine.Format("<..>Possible Database Spend as @ %s\t\t%.*lf",
			CSSDate::GetCurrentDate().GetDate(),
			System.GetDPValue(),
			m_dDatabasePossibleSpend);

		m_Report.WriteLine(strLine);
	}
}

//**************************************************************************

void CReportCashlessPosting::AddColumnText()
{
	m_Report.AddColumn ( "Source", TA_LEFT, 500 );
	m_Report.AddColumn ( "Action", TA_LEFT, 500);
	m_Report.AddColumn ( "Purse1", TA_RIGHT, 300 );

	if ( System.GetEnablePurse2Flag() == TRUE )	
	{
		m_Report.AddColumn ( "Purse2", TA_RIGHT, 300 );
		m_Report.AddColumn ( "Total", TA_RIGHT, 300 );
	}
}

//*******************************************************************

void CReportCashlessPosting::CreateLine(CString& strLine, CString strSource, CString strAction, double dValue1, double dValue2)
{
	//AVOID DISPLAY OF NEGATIVE ZERO
	if (CompareDoubles(dValue1, 0.0, System.GetDPValue()) == 0)
	{
		dValue1 = 0.0;
	}

	if (CompareDoubles(dValue2, 0.0, System.GetDPValue()) == 0)
	{
		dValue2 = 0.0;
	}

	CCSV csv('\t');

	csv.Add(strSource);
	csv.Add(strAction);
	csv.Add(dValue1, System.GetDPValue());

	if (System.GetEnablePurse2Flag() == TRUE)
	{
		csv.Add(dValue2, System.GetDPValue());
		csv.Add(dValue1 + dValue2, System.GetDPValue());
	}
	strLine = csv.GetLine();
}

//*******************************************************************

void CReportCashlessPosting::ShowLines()
{
	CCSV csv('\t');

	csv.Add("");
	csv.Add("");
	csv.Add("<LI>");

	if (System.GetEnablePurse2Flag() == TRUE)
	{
		csv.Add("<LI>");
		csv.Add("<LI>");
	}

	m_Report.WriteLine(csv);
}

//*******************************************************************

void CReportCashlessPosting::CalculateDatabaseSpend()
{
	CWorkingDlg WorkingDlg("Scanning Database");
	WorkingDlg.Create();

	m_dDatabasePossibleSpend = 0;

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NormalNoParams{});
	
	int nChecked = 0;
	CSQLRowAccountFull RowAccount;
	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (WorkingDlg.SetCaption2RecordsChecked(++nChecked, TRUE) == TRUE)
		{
			m_bCancelButton = TRUE;
			break;
		}

		if (RowAccount.GetInactive() == TRUE)
		{
			continue;
		}

		if ((RowAccount.IsCardHotlisted() == TRUE) && (RowAccount.HaveNextUserID() == TRUE))
		{
			continue;
		}

		m_dDatabasePossibleSpend += GetDailyRefreshValue(RowAccount.GetGroupNo());
	}
}

//*******************************************************************

double CReportCashlessPosting::GetDailyRefreshValue(int nGroupNo)
{
	double dResult = 0.0;

	CConsolidatedDoubleByInt item;
	item.m_nKey = nGroupNo;

	int nPos = 0;
	if (m_arrayGroupDailyRefresh.Find(item, nPos) == TRUE)
	{
		m_arrayGroupDailyRefresh.GetAt(nPos, item);
		dResult = item.m_dVal;
	}
	else
	{
		CSQLRowGroup RowGroup;
		RowGroup.SetGroupNo(nGroupNo);

		CSQLRepositoryGroup repoGroup;
		if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if (RowGroup.GetRefreshType() == nREFRESHTYPE_DAILY)
			{
				dResult = RowGroup.GetRefreshValue();
				
				item.m_dVal = dResult;
				m_arrayGroupDailyRefresh.InsertAt(nPos,item);
			}
		}
	}

	return dResult;
}

//*******************************************************************
