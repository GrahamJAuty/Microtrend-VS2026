//********************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//********************************************************************
#include "LoyaltyManager.h"
#include "RecSort.h"
#include "RepmanHandler.h"
#include "ReportCashlessPosting.h"
#include "ReportStartingBalance.h"
//********************************************************************
#include "ReportCashlessLiability.h"
//********************************************************************

CReportCashlessLiability::CReportCashlessLiability( const char* szLabel, CWnd* pParent )
	: m_dlgCashlessFilter ( this, pParent )
	, m_ReportInfo( szLabel, TRUE )
{
	m_strReportLabel = szLabel;
	m_pParent = pParent;
	m_bCancelButton = FALSE;
	m_dPurse1Opening = 0.0;
	m_dPurse2Opening = 0.0;
	m_dPurse1Total = 0.0;
	m_dPurse2Total = 0.0;
}

//********************************************************************

void CReportCashlessLiability::Reset()
{
	m_bCancelButton = FALSE;
	m_dPurse1Opening = 0.0;
	m_dPurse2Opening = 0.0;
	m_dPurse1Total = 0.0;
	m_dPurse2Total = 0.0;

	m_arrayCardBalances.RemoveAll();
	m_TotalsPurseTran.Reset();
	m_TotalsPurseXfer.Reset();	
	m_TotalsPurseAdjust.Reset();
}

//********************************************************************

void CReportCashlessLiability::Show()
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

int CReportCashlessLiability::ShowInternal()
{
	Reset();

	CString strReportFile = Filenames.GetReportFilename ( m_strReportLabel );
	CString strParamsFile = Filenames.GetReportParamsFilename ( m_strReportLabel );

	CLoyaltyAuditFile auditFile;						

	{
		int nErrorNo = auditFile.ValidateFile();
		if (nErrorNo != nREPORT_NOERROR)
		{
			return nErrorNo;
		}
	}

	if (m_Report.Open(strReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CString strTitle = m_ReportInfo.GetReportTitle();
	strTitle += m_ReportInfo.GetDateRangeString();

	m_Report.SetStyle1 ( strTitle );

	AddColumnText();

	if (auditFile.OpenReadOnly() == FALSE)	
	{
		m_Report.Close();
		return nREPORT_OPENERROR;
	}

	CWorkingDlg DlgWorking("Creating report");
	DlgWorking.Create();

	//CREATE LIST OF ALL USER IDS IN DATABASE

	CReportConsolidationArray<CSortedInt64WithFlag> arrayUserIDs;

	CSQLRecordSetAccountUserID RecordSetUserID(NULL,
		RSParams_AccountUserId_NormalNoParams{});
	{
		int nChecked = 0;
		__int64 nUserID = 0;
		while (RecordSetUserID.StepSelect(nUserID, TRUE) == TRUE)
		{
			if (DlgWorking.SetCaption2RecordsChecked(++nChecked, TRUE) == TRUE)
			{
				m_bCancelButton = TRUE;
				break;
			}

			CSortedInt64WithFlag entry;
			entry.m_nInt64 = nUserID;
			entry.m_nFlag = 1;
			arrayUserIDs.Consolidate(entry);
		}
	}

	if ( TRUE == m_bCancelButton )
	{
		m_Report.Close();
		auditFile.Close();
		DlgWorking.DestroyWindow();
		return nREPORT_CANCEL;
	}

	CString strDateFrom = m_ReportInfo.GetDateFromString();
	if ((m_ReportInfo.GetCutoffTimeFlag() == TRUE) && (m_ReportInfo.GetCutoffNextDayFlag() == FALSE))
	{
		strDateFrom = m_ReportInfo.GetDatePreviousString();
	}

	CReportConsolidationArray<CSortedIntByInt64> arrayBalancePos;

	if (CReportStartingBalance::FindStartingBalances(
		arrayUserIDs,
		TRUE,
		auditFile,
		m_ReportInfo.GetDateSearchFromString(),
		m_ReportInfo.GetDateSearchToString(),
		strDateFrom,
		m_ReportInfo.GetTimeCutoffString(),
		arrayBalancePos,
		DlgWorking) == FALSE)
	{
		m_bCancelButton = TRUE;
		m_Report.Close();
		auditFile.Close();
		DlgWorking.DestroyWindow();
		return nREPORT_CANCEL;
	}

	CalculateOpeningBalances(auditFile, arrayBalancePos, DlgWorking);

	m_Report.WriteLine ("" );

	if (CalculateUsageFigures(&auditFile, DlgWorking) == FALSE)		// calc whats been added / subtracted in the period
	{
		m_bCancelButton = TRUE;
		m_Report.Close();
		auditFile.Close();
		DlgWorking.DestroyWindow();
		return nREPORT_CANCEL;
	}
	else
	{
		ShowTotals();
	}

	auditFile.Close();
	m_Report.Close();
	DlgWorking.DestroyWindow();

	CRepmanHandler repman ( m_ReportInfo.GetReportTitle(), m_pParent );
	return repman.DisplayReport( strReportFile, strParamsFile );
}

//*******************************************************************

bool CReportCashlessLiability::CalculateOpeningBalances(CLoyaltyAuditFile& AuditFile, CReportConsolidationArray<CSortedIntByInt64>& arrayBalancePos, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Collecting opening balances");

	for (int n = 0; n < arrayBalancePos.GetSize(); n++)
	{
		if (WorkingDlg.SetCaption2RecordsOfTotal(n, arrayBalancePos.GetSize()) == TRUE)
		{
			m_bCancelButton = TRUE;
			return FALSE;
		}

		CSortedIntByInt64 item;
		arrayBalancePos.GetAt(n, item);

		int nFilePos = item.m_nVal;
		if (nFilePos >= 0)
		{
			AuditFile.SetFilePos(nFilePos);
			if (AuditFile.ReadLine(&m_atc) == TRUE)
			{
				bool bCalcNearestBalance = (m_ReportInfo.IsBeforeReportDate(&m_atc) == 1);
				SaveStartingBalanceLine(bCalcNearestBalance);
			}
		}
		else
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(item.m_nKey);
			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				CSSDate date;
				CSSTime time;
				RowAccount.GetLastUsed(date, time);

				if (m_ReportInfo.IsBeforeReportDate(date, time) == 0)
				{
					m_dPurse1Opening += RowAccount.GetPurse1Balance();
					m_dPurse2Opening += RowAccount.GetPurse2Balance();

					CConsolidatedCashlessReconcilePurse CardOpening;
					CardOpening.m_nCardNo = _atoi64(m_atc.GetCardNo());
					CardOpening.m_dPurse1 = RowAccount.GetPurse1Balance();
					CardOpening.m_dPurse2 = RowAccount.GetPurse2Balance();
					m_arrayCardBalances.Consolidate(CardOpening);
				}
			}
		}
	}

	WriteLine("Opening Balances", m_dPurse1Opening, m_dPurse2Opening);

	m_dPurse1Total += m_dPurse1Opening;
	m_dPurse2Total += m_dPurse2Opening;

	return TRUE;
}

//*****************************************************************************

void CReportCashlessLiability::SaveStartingBalanceLine ( bool bCalcBalance ) 
{
	if (m_atc.GetApplicationNo() == APPNO_DELETE)
	{
		CSSDate date(m_atc.GetDateFiled());
		if (date < m_ReportInfo.GetDateFrom())
		{
			return;
		}
	}

	double dCardPurse1Opening = m_atc.GetPurse1();
	double dCardPurse2Opening = m_atc.GetPurse2();

	if ( bCalcBalance == TRUE )
	{
		dCardPurse1Opening -= m_atc.GetPurse1Transaction();
		dCardPurse2Opening -= m_atc.GetPurse2Transaction();
	}

	m_dPurse1Opening += dCardPurse1Opening;					// Purse1 total
	m_dPurse2Opening += dCardPurse2Opening;					// Purse2 total

	CConsolidatedCashlessReconcilePurse CardOpening;
	CardOpening.m_nCardNo = _atoi64( m_atc.GetCardNo() );
	CardOpening.m_dPurse1 = dCardPurse1Opening;
	CardOpening.m_dPurse2 = dCardPurse2Opening;
	m_arrayCardBalances.Consolidate( CardOpening );
}

//*****************************************************************************
//*** values added / subtracted from cards in period **************************
//*****************************************************************************

bool CReportCashlessLiability::CalculateUsageFigures ( CLoyaltyAuditFile* pAudit, CWorkingDlg& DlgWorking)
{
	bool bCancel = FALSE;
	ProcessCardUsage( pAudit, DlgWorking, bCancel );

	if ( FALSE == bCancel )
	{
		double dPurse1 = m_TotalsPurseTran.m_dPurse1Plus;
		double dPurse2 = m_TotalsPurseTran.m_dPurse2Plus;
		
		WriteLine ( "Total Additions" , dPurse1, dPurse2 );

		m_dPurse1Total += dPurse1;
		m_dPurse2Total += dPurse2;

		m_Report.WriteLine("");

		/*****/

		dPurse1 = m_TotalsPurseAdjust.m_dPurse1Plus - m_TotalsPurseAdjust.m_dPurse1Minus;
		dPurse2 = m_TotalsPurseAdjust.m_dPurse2Plus - m_TotalsPurseAdjust.m_dPurse2Minus;

		if ((::CompareDoubles(dPurse1, 0.0, 3) != 0) || (::CompareDoubles(dPurse2, 0.0,3) != 0))
		{
			WriteLine("Total Adjustments", dPurse1, dPurse2);

			m_dPurse1Total += dPurse1;
			m_dPurse2Total += dPurse2;

			m_Report.WriteLine("");
		}

		/*****/
		//NET TRANSFER SHOULD BE ZERO, BUT REPORT IF NOT
		/*****/

		dPurse1 = m_TotalsPurseXfer.m_dPurse1Plus - m_TotalsPurseXfer.m_dPurse1Minus;
		dPurse2 = m_TotalsPurseXfer.m_dPurse2Plus - m_TotalsPurseXfer.m_dPurse2Minus;

		if ((::CompareDoubles(dPurse1, 0.0, 3) != 0) || (::CompareDoubles(dPurse2, 0.0, 3) != 0))
		{
			WriteLine("Total Net Transfer", dPurse1, dPurse2);

			m_dPurse1Total += dPurse1;
			m_dPurse2Total += dPurse2;

			m_Report.WriteLine("");
		}

		/*****/

		dPurse1 = m_TotalsPurseTran.m_dPurse1Minus;
		dPurse2 = m_TotalsPurseTran.m_dPurse2Minus;

		WriteLine ( "Total Deductions", dPurse1, dPurse2 );

		m_dPurse1Total -= dPurse1;
		m_dPurse2Total -= dPurse2;

		m_Report.WriteLine ("" );
	}
	
	return ( FALSE == bCancel );
}

//**********************************************************************

void CReportCashlessLiability::ProcessCardUsage( CLoyaltyAuditFile* pAudit, CWorkingDlg& DlgWorking, bool& bCancel )
{
	bCancel = FALSE;

	DlgWorking.SetCaption1("Processing Card Usage");

	pAudit->JumpIn ( m_ReportInfo.GetDatePreviousString() );
	while (pAudit->ReadLine(&m_atc) == TRUE)
	{
		if (DlgWorking.SetCaption2PercentDone(pAudit->GetPercentPosition(), TRUE) == TRUE)
		{
			m_bCancelButton = TRUE;
			bCancel = TRUE;
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

		if (m_ReportInfo.IsValidReportDate(strDate, strTime))
		{
			ProcessLine();
		}
	}
}

//**********************************************************************

void CReportCashlessLiability::ProcessLine() 
{
	int nAppNo = m_atc.GetApplicationNo();

	switch (m_atc.GetSourceType())
	{
	case AUDIT_SERVER:
	case AUDIT_PC:
	case AUDIT_POS:
	case AUDIT_EXT:
		break;

	default:
		return;
	}

	double dPurse1Amount = m_atc.GetPurse1Transaction();
	double dPurse2Amount = m_atc.GetPurse2Transaction();

	switch (m_atc.GetApplicationNo())
	{
	case APPNO_SALE:
		m_TotalsPurseTran.ConsolidateMinus(1, dPurse1Amount);
		m_TotalsPurseTran.ConsolidateMinus(2, dPurse2Amount);
		AdjustCardBalance();
		break;

	case APPNO_REFUNDSALE:
	case APPNO_MANUAL_REFUND:
		if ( m_ReportInfo.GetRefundProcessType() == 1)
		{
			m_TotalsPurseTran.ConsolidateMinus(1, dPurse1Amount);
			m_TotalsPurseTran.ConsolidateMinus(2, dPurse2Amount);
		}
		else
		{
			m_TotalsPurseTran.ConsolidatePlus(1, dPurse1Amount);
			m_TotalsPurseTran.ConsolidatePlus(2, dPurse2Amount);
		}
		AdjustCardBalance();
		break;

	case APPNO_EDIT:
	case APPNO_BATCHUPDATE:
	case APPNO_SUBCASH:
	case APPNO_ADDGIFT:
	case APPNO_ADDGIFTX:
	case APPNO_REVALUEBONUSCASH:
	case APPNO_PURSE1_MANTOPUP:
	case APPNO_PURSE2_MANTOPUP:
	case APPNO_PURSE1_IMPTOPUP:
	case APPNO_PURSE2_IMPTOPUP:
		m_TotalsPurseTran.Consolidate(1, dPurse1Amount);
		m_TotalsPurseTran.Consolidate(2, dPurse2Amount);
		AdjustCardBalance();
		break;

	case APPNO_REDEEM:
	case APPNO_BATCHREDEEM:
		m_TotalsPurseTran.ConsolidatePlus(1, dPurse1Amount);
		m_TotalsPurseTran.ConsolidatePlus(2, dPurse2Amount);
		AdjustCardBalance();
		break;

	case APPNO_MANUAL_SPEND:
		m_TotalsPurseTran.ConsolidateMinus(1, dPurse1Amount);
		AdjustCardBalance();
		break;

	case APPNO_REWARDBONUSCASH:
	case APPNO_DEPTBONUSCASH:
		m_TotalsPurseTran.ConsolidatePlus(1, dPurse1Amount);
		m_TotalsPurseTran.ConsolidatePlus(2, dPurse2Amount);
		AdjustCardBalance();
		break;

	case APPNO_REFUNDREWARDBONUSCASH:
	case APPNO_REFUNDDEPTBONUSCASH:
		m_TotalsPurseTran.ConsolidateMinus(1, dPurse1Amount);
		m_TotalsPurseTran.ConsolidateMinus(2, dPurse2Amount);
		AdjustCardBalance();
		break;

	case APPNO_REFRESH:
		m_TotalsPurseTran.ConsolidatePlus(2, dPurse2Amount);
		AdjustCardBalance();
		break;

	case APPNO_XFERIN:
	case APPNO_XFEROUT:
	case APPNO_HOTXFERIN:
	case APPNO_HOTXFEROUT:
		m_TotalsPurseXfer.Consolidate(1, dPurse1Amount);
		m_TotalsPurseXfer.Consolidate(2, dPurse2Amount);
		AdjustCardBalance();
		break;

	default:
		{
			int nPurseNo = 0;
			int nTopupNo = 0;
			CAuditReportTexts::GetAddCashIndexByAppNo(m_atc.GetApplicationNo(), nPurseNo, nTopupNo);

			if ( ( 1 == nPurseNo ) || ( 2 == nPurseNo ) )
			{
				m_TotalsPurseTran.Consolidate(1, dPurse1Amount);
				m_TotalsPurseTran.Consolidate(2, dPurse2Amount);
				AdjustCardBalance();
			}
		}
		break;
	}
}

//*******************************************************************

void CReportCashlessLiability::AdjustCardBalance()
{
	CConsolidatedCashlessReconcilePurse CardBalance;
	CardBalance.m_nCardNo = _atoi64(m_atc.GetCardNo());

	int nPos = 0;
	if (m_arrayCardBalances.Find(CardBalance, nPos) == TRUE)
	{
		m_arrayCardBalances.GetAt(nPos, CardBalance);
	}

	double dAdjust = m_atc.GetPurse1() - (m_atc.GetPurse1Transaction() + CardBalance.m_dPurse1);
	if (::CompareDoubles(dAdjust, 0.0, 3) != 0)
	{
		int x = 2;
	}

	m_TotalsPurseAdjust.Consolidate(1, m_atc.GetPurse1() - (m_atc.GetPurse1Transaction() + CardBalance.m_dPurse1));
	m_TotalsPurseAdjust.Consolidate(2, m_atc.GetPurse2() - (m_atc.GetPurse2Transaction() + CardBalance.m_dPurse2));

	CardBalance.m_dPurse1 = m_atc.GetPurse1();
	CardBalance.m_dPurse2 = m_atc.GetPurse2();
	m_arrayCardBalances.Consolidate(CardBalance);
}

//*******************************************************************

void CReportCashlessLiability::AddColumnText()
{
	m_Report.AddColumn("", TA_LEFT, 500);
	m_Report.AddColumn("Purse1", TA_RIGHT, 300);
	if (System.GetEnablePurse2Flag() == TRUE)
	{
		m_Report.AddColumn("Purse2", TA_RIGHT, 300);
		m_Report.AddColumn("Total", TA_RIGHT, 300);
	}
}

//*****************************************************************************

void CReportCashlessLiability::WriteLine(const char* szText, double dValue1, double dValue2)
{
	CCSV csv('\t');

	csv.Add(szText);
	csv.Add(dValue1, System.GetDPValue());
	if (System.GetEnablePurse2Flag() == TRUE)
	{
		csv.Add(dValue2, System.GetDPValue());
		csv.Add(dValue1 + dValue2, System.GetDPValue());
	}

	m_Report.WriteLine(csv.GetLine());
}

//*****************************************************************************

void CReportCashlessLiability::ShowLines()
{
	CCSV csv('\t');

	csv.Add("");
	csv.Add("<LI>");
	if (System.GetEnablePurse2Flag() == TRUE)
	{
		csv.Add("<LI>");
		csv.Add("<LI>");
	}

	m_Report.WriteLine(csv);
}

//*****************************************************************************

void CReportCashlessLiability::ShowTotals()
{
	ShowLines();
	WriteLine("Liability Total", m_dPurse1Total, m_dPurse2Total);
}

//*****************************************************************************

