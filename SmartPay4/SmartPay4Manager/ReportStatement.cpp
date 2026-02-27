//******************************************************************
// derived from PurchaseReport
//******************************************************************
// R121 - Statements - all accounts
// R122 - Statement  - single account
//******************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRecordSetAccountUserId.h"
//******************************************************************
#include "ReportStatement.h"
//******************************************************************
#define nINDEX_PURSE1		0
#define nINDEX_PURSE2		1
#define nINDEX_PURSE3		2
//******************************************************************

CReportStatement::CReportStatement ( const char* szAuditFile, const char* szLabel, __int64 nSingleUserID, CWnd* pParent )
	: CPurchaseReport ( szLabel, nSingleUserID, pParent )
{
	m_strAuditFilename = szAuditFile;
	m_bBonusAllowed = FALSE;
	m_nSingleUserID = nSingleUserID;

	ResetStatement();

	Server.Read ( Filenames.GetServerDataFilename() );

	if (Server.RedeemPointsEnabled() == TRUE || Server.RevaluationBonusEnabled() == TRUE || Server.SpendBonusEnabled() == TRUE)
	{
		m_bBonusAllowed = TRUE;
	}
}

//******************************************************************

void CReportStatement::ResetStatement()
{
	for (int nPurse = 0; nPurse < 3; ++nPurse)
	{
		balances.dOpening[nPurse] = 0;
		balances.dAdditions[nPurse] = 0;
		balances.dBonuses[nPurse] = 0;
		balances.dDeductions[nPurse] = 0;
		balances.dClosing[nPurse] = 0;
	}

	CPurchaseReport::Reset();
}

//******************************************************************

void CReportStatement::Show()
{
	int nErrorNo = 0;

	CSmartPayAuditFile auditFile ( m_strAuditFilename );
	if ( ( nErrorNo = auditFile.ValidateFile() ) == nREPORT_NOERROR )
	{
		bool bFirstTime = TRUE;

		while ( TRUE )
		{
			CPurchasePropertySheet propSheet(m_ReportPurchaseHelpers, m_pParent);

			if (TRUE == bFirstTime)
			{
				if ( m_ReportPurchaseHelpers.IsStatementReportSingle() == TRUE)					// single statement report
				{
					CString strCardNo = System.FormatCardNo(m_nSingleUserID, TRUE);
					m_ReportPurchaseHelpers.SetFromCardNo(strCardNo);
					m_ReportPurchaseHelpers.SetToCardNo(strCardNo);					
				}

				bFirstTime = FALSE;
			}

			if (propSheet.DoModal() != IDOK)
			{
				break;
			}

			if ((nErrorNo = CreateReport()) != nREPORT_NOERROR)
			{
				break;
			}
		}
	}

	m_Repman.DisplayError ( nErrorNo );
}

//******************************************************************

int CReportStatement::CreateReport()
{
	ResetStatement();

	if (m_Report.Open(m_ReportPurchaseHelpers.GetReportFilename()) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CSmartPayAuditFile auditFile ( m_strAuditFilename );
	if ( auditFile.Open() == FALSE )							// reopen source file - already validated
	{
		m_Report.Close();										// close report file
		return nREPORT_OPENERROR;
	}

//
	CWorkingDlg dlgWorking ( szMSG_CREATINGREPORT, TRUE );
	dlgWorking.Create();

	m_Report.SetStyle1 ( m_ReportPurchaseHelpers.GetPurchaseReportTitle() );
	AddColumnText();
		
	auditFile.JumpIn ( m_ReportPurchaseHelpers.GetDateFrom().GetDate() );

	int nErrorNo = ReportByCardNo ( &auditFile, &dlgWorking );

	dlgWorking.DestroyWindow();

	m_Report.Close();
	auditFile.Close();										// close audit file open

	if (nErrorNo == nREPORT_NOERROR)
	{
		nErrorNo = m_Repman.DisplayReport(m_ReportPurchaseHelpers.GetReportFilename(), m_ReportPurchaseHelpers.GetParamsFilename(), m_ReportPurchaseHelpers.GetReportKey(), m_pParent);
	}

	return nErrorNo;
}

//******************************************************************

int CReportStatement::ReportByCardNo ( CSmartPayAuditFile* pAudit, CWorkingDlg* pDlgWorking )
{
	CSQLAuditRecord atc;
	CReportConsolidationArray<CReportStatementAccountInfo> arrayUserIDs;

	bool bCancelButton = FALSE;

	while ( TRUE )
	{
		if (pDlgWorking->SetCaption2PercentDone(pAudit->GetPercentPosition() / 2, TRUE) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		int nStartFilePos = pAudit->GetFilePos();			// position in file to start from
		if (pAudit->ReadLine(&atc) == FALSE)
		{
			break;
		}

		if (atc.m_dateFiled < m_ReportPurchaseHelpers.GetDateFrom())
		{
			continue;
		}

		if (atc.m_dateFiled > m_ReportPurchaseHelpers.GetDateTo())
		{
			continue;
		}

		if (m_ReportPurchaseHelpers.GetTimeRangeSetFlag() == TRUE)
		{
			if (atc.m_timeFiled < m_ReportPurchaseHelpers.GetTimeFrom())
			{
				continue;
			}

			if (atc.m_timeFiled > m_ReportPurchaseHelpers.GetTimeTo())
			{
				continue;
			}
		}

		bool bIsValid = FALSE;
		
		__int64 nUserID = _atoi64(atc.GetUserID());
		if (m_ReportPurchaseHelpers.GetReportRangeType() == nREPORT_LIST)
		{
			bIsValid = (m_ReportPurchaseHelpers.CheckUserIDAgainstList(nUserID));
		}
		else
		{
			CPurchaseHistorySearchInfo infoSearch;
			m_ReportPurchaseHelpers.GetSQLSearchInfoPurchase(infoSearch, nUserID);

			CString strWhere = "";
			CSQLBuilderBind BuilderBind;
			CSQLRepositoryAccount RepoAccount;
			RepoAccount.PreparePurchaseWhere(infoSearch, strWhere, BuilderBind);

			CSQLRecordSetAccountUserID RecordSet(
				NULL,
				RSParams_AccountUserId_NormalByParams{ &BuilderBind, strWhere });

			__int64 nUserID = 0;
			if (RecordSet.StepSelect(nUserID) == TRUE)
			{
				bIsValid = TRUE;
			}
		}

		if (TRUE == bIsValid)
		{
			CReportStatementAccountInfo info;
			info.m_nUserID = nUserID;
			info.m_nFilePos = nStartFilePos;
			arrayUserIDs.Consolidate(info);
		}
	}

	if (bCancelButton == TRUE)
	{
		return nREPORT_CANCEL;
	}

// no lines found in audit file 

	if ( arrayUserIDs.GetSize() == 0 )
	{
		if ( m_ReportPurchaseHelpers.IsStatementReportSingle() == TRUE)										// see if single statement
		{
			return SingleInactiveBalance(pAudit, pDlgWorking);	//  try & work out single account starting balance from audit
		}
		else
		{
			return nREPORT_NOTRANSACTIONS;
		}
	}

//
	CString strLastUserID = "";

	int nCount = 0;
	for ( int nIndex = 0 ; nIndex < arrayUserIDs.GetSize() ; nIndex++ )
	{
		if (pDlgWorking->SetCaption2PercentDone(50 + ((nIndex * 50L) / arrayUserIDs.GetSize()), TRUE) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		CReportStatementAccountInfo accountInfo;
		arrayUserIDs.GetAt(nIndex, accountInfo);
		
		pAudit->SetFilePos ( accountInfo.m_nFilePos );

		if ( pAudit->ReadLine ( &atc ) == TRUE )
		{
			CString strUserID = atc.GetUserID();

			if ( nIndex == 0 )										// on first time round loop
			{
				strLastUserID = strUserID;
				GetStartingBalance ( &atc );						//  save balance line if line is not a balance line
			}
			else
			{
				if ( strUserID != strLastUserID )					// change of main label
				{
					if ( SaveStatement ( _atoi64(strLastUserID) ) == TRUE )	// display if required
					{
						++nCount;
						NewPage();									// save new page if required
					}

					strLastUserID = strUserID;
					ResetStatement();
					GetStartingBalance( &atc );						//  save balance line if line is not a balance line
				}
			}

			ProcessAuditLine ( &atc );
		}
	}

	if (SaveStatement(_atoi64(strLastUserID)) == TRUE)					// display if required
	{
		++nCount;
	}

	int nErrorNo = ( nCount == 0 ) ? nREPORT_NOTRANSACTIONS : nREPORT_NOERROR;

	return ( bCancelButton == TRUE ) ? nREPORT_CANCEL : nErrorNo;
}

//******************************************************************

void CReportStatement::GetStartingBalance(CSQLAuditRecord* pAtc)
{
	balances.dOpening[nINDEX_PURSE1] = pAtc->GetCurrentPurse1Total() - pAtc->GetTransactionPurse1Total();;
	balances.dOpening[nINDEX_PURSE2] = pAtc->GetCurrentPurse2Balance() - pAtc->GetTransactionPurse2Balance();
	balances.dOpening[nINDEX_PURSE3] = pAtc->GetCurrentPurse3Total() - pAtc->GetTransactionPurse3Total();;
}

//******************************************************************

void CReportStatement::ProcessAuditLine(CSQLAuditRecord* pAtc)
{
	if (m_ReportPurchaseHelpers.CheckTerminalFilter(pAtc->GetFolderSet(), pAtc->GetTerminalNo()) == TRUE)
	{
		double dValue1 = pAtc->GetTransactionPurse1Total();		// purse1 transaction / cash taken
		double dValue2 = pAtc->GetTransactionPurse2Balance();	// purse2 transaction / bonus cash
		double dValue3 = pAtc->GetTransactionPurse3Total();	// purse3 transaction / cash taken
		int nAppNo = pAtc->GetApplicationNo();

		bool bHavePurse1 = TRUE;									// assume have Purse1 transaction data
		bool bHavePurse2 = TRUE;									// assume have Purse2 transaction data
		bool bHavePurse3 = TRUE;									// assume have Purse3 transaction data
		bool bIsBonus = FALSE;

		switch (nAppNo)
		{
		case APPNO_INSERT:
		case APPNO_DELETE:
		case APPNO_DELETELEAVER:
		case APPNO_HOTLIST:
		case APPNO_IMPORTHOTLIST:
		case APPNO_IMPORTDELETE:
		case APPNO_UNHOTLIST:
		case APPNO_HOTLISTREPLACE:
		case APPNO_IMPORTREPLACE:
		case APPNO_BATCHDELETE:
		case APPNO_BATCHLEAVER:
		case APPNO_IMPORTLEAVER:
		case APPNO_BALANCE:
		case APPNO_IMPORTUPDATE:
		case APPNO_IMPORTINSERT:
		case APPNO_IMPORTCUSTOM:
		case APPNO_INSERTRECORDSIMS:
		case APPNO_INSERTRECORDPPAY:
		case APPNO_INSERTRECORDWISEPAY:
		case APPNO_ADDPOINTS:						// add to card purse
		case APPNO_REVALUEBONUSPOINTS:
		case APPNO_REWARDBONUSPOINTS:
		case APPNO_REFUNDREWARDBONUSPOINTS:
		case APPNO_WPOPENINGBALANCE:
		case APPNO_BIOREG:
		case APPNO_BIOUNREG:
			bHavePurse1 = FALSE;					// Purse1 transaction
			bHavePurse2 = FALSE;					// Purse2 transaction
			bHavePurse3 = FALSE;					// Purse3 transaction
			break;

		case APPNO_ADDCASH:							// add cash to Purse
		case APPNO_EDIT:
		case APPNO_XFEROUT:
		case APPNO_XFERIN:
		case APPNO_HOTXFEROUT:
		case APPNO_HOTXFERIN:
		case APPNO_IMPORTTOPUP:
		case APPNO_REFUNDSALE:						// negative sale
		case APPNO_SALE:
		case APPNO_PARENTPAY:						// ParentPay import
		case APPNO_WISEPAY:							// WisePay import	
		case APPNO_TUCASI:							// Tucasi(Soap) import	
		case APPNO_TUCASIv2:						// Tucasi(Json) import
		case APPNO_SQUID:							// sQuid import
		case APPNO_SCHOOLCOMMS:						// Schoolcomms import
		case APPNO_PARENTMAIL:						// ParentMail import
		case APPNO_BROMCOM:
		case APPNO_PEBBLE_TOPUP:
		case APPNO_PEBBLE_TOPUP_REFUND:
		case APPNO_PEBBLE_ORDER:
		case APPNO_PEBBLE_ORDER_REFUND:
			break;

		case APPNO_REFRESH:
			bHavePurse1 = FALSE;					// purse1 trans
			bHavePurse3 = FALSE;					// Purse3 transaction
			break;

		case APPNO_REDEEM:							// points redeemed for bonus cash
		case APPNO_BATCHREDEEM:						// points redeemed for bonus cash
			bIsBonus = TRUE;
			break;

		case APPNO_REVALUEBONUSCASH:
		case APPNO_REWARDBONUSCASH:
		case APPNO_REFUNDREWARDBONUSCASH:
			bIsBonus = TRUE;
			bHavePurse2 = FALSE;					// purse2
			bHavePurse3 = FALSE;					// Purse3 transaction
			dValue1 = dValue2;						// bonus value saved in Purse2Transaction field
			dValue2 = 0.0;							// clear dvalue2
			break;
		}

		if (bHavePurse1 == TRUE) 					// Purse1 transaction column
		{
			if (bIsBonus == TRUE)
			{
				balances.dBonuses[nINDEX_PURSE1] += dValue1;
			}
			else
			{
				if (dValue1 < 0.0)
				{
					balances.dDeductions[nINDEX_PURSE1] += dValue1;
				}
				else
				{
					balances.dAdditions[nINDEX_PURSE1] += dValue1;
				}
			}
		}

		if (bHavePurse2 == TRUE)					// Purse2 transaction column
		{
			if (bIsBonus == TRUE)
			{
				balances.dBonuses[nINDEX_PURSE2] += dValue2;
			}
			else
			{
				if (dValue2 < 0.0)
				{
					balances.dDeductions[nINDEX_PURSE2] += dValue2;
				}
				else
				{
					balances.dAdditions[nINDEX_PURSE2] += dValue2;
				}
			}
		}

		if (bHavePurse3 == TRUE)					// Purse3 transaction column
		{
			if (bIsBonus == TRUE)
			{
				balances.dBonuses[nINDEX_PURSE3] += dValue3;
			}
			else
			{
				if (dValue3 < 0.0)
				{
					balances.dDeductions[nINDEX_PURSE3] += dValue3;
				}
				else
				{
					balances.dAdditions[nINDEX_PURSE3] += dValue3;
				}
			}
		}
	}

	// Overwrite last Purse Balances

	balances.dClosing[nINDEX_PURSE1] = pAtc->GetCurrentPurse1Total();
	balances.dClosing[nINDEX_PURSE2] = pAtc->GetCurrentPurse2Balance();
	balances.dClosing[nINDEX_PURSE3] = pAtc->GetCurrentPurse3Total();
}

//******************************************************************

void CReportStatement::AddColumnBalanceLine()
{
	CCSV csv ( '\t' );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "Purse1" );
	csv.Add ( "Purse2" );
	csv.Add ( "Purse3" );
	csv.Add ( "" );							// comment column

	m_Report.WriteLine ( csv );
	m_Report.WriteLine ( "" );
}

//******************************************************************

void CReportStatement::AddBalanceLine(int nIndex)
{
	if (nIndex == 2 && m_bBonusAllowed == FALSE)						// see if need a bonus line
	{
		return;
	}

	CString strText = "";
	bool bCommentAllowed = FALSE;

	double dPurse1 = 0.0, dPurse2 = 0.0, dPurse3 = 0.0;

	switch (nIndex)
	{
	case 0:
		strText = "<..>Opening Balance";
		dPurse1 = balances.dOpening[nINDEX_PURSE1];
		dPurse2 = balances.dOpening[nINDEX_PURSE2];
		dPurse3 = balances.dOpening[nINDEX_PURSE3];
		break;

	case 1:
		strText = "<..>Additions";
		bCommentAllowed = TRUE;
		dPurse1 = balances.dAdditions[nINDEX_PURSE1];
		dPurse2 = balances.dAdditions[nINDEX_PURSE2];
		dPurse3 = balances.dAdditions[nINDEX_PURSE3];
		break;

	case 2:
		strText = "<..>Bonuses";
		bCommentAllowed = TRUE;
		dPurse1 = balances.dBonuses[nINDEX_PURSE1];
		dPurse2 = balances.dBonuses[nINDEX_PURSE2];
		dPurse3 = balances.dBonuses[nINDEX_PURSE3];
		break;

	case 3:
		strText = "<..>Deductions";
		bCommentAllowed = TRUE;
		dPurse1 = balances.dDeductions[nINDEX_PURSE1];
		dPurse2 = balances.dDeductions[nINDEX_PURSE2];
		dPurse3 = balances.dDeductions[nINDEX_PURSE3];
		break;

	case 4:
		strText = "<..>Closing Balance";
		dPurse1 = balances.dClosing[nINDEX_PURSE1];
		dPurse2 = balances.dClosing[nINDEX_PURSE2];
		dPurse3 = balances.dClosing[nINDEX_PURSE3];
		break;
	}

	CCSV csvLine('\t');

	csvLine.Add(strText);
	csvLine.Add("");
	csvLine.Add(::FormatDoubleValue(dPurse1));
	csvLine.Add(::FormatDoubleValue(dPurse2));

	if (System.GetEnablePurse3Flag() == TRUE)
	{
		csvLine.Add(::FormatDoubleValue(dPurse3));
	}
	else
	{
		csvLine.Add("--");
	}

	if ( m_ReportPurchaseHelpers.GetTerminalListType() == 1)				// see if do partial terminal list
	{
		if (bCommentAllowed == TRUE)
		{
			CCSV csv;
			for (int i = 0; i < m_ReportPurchaseHelpers.TNoListGetSize(); i++)
			{
				csv.Add(m_ReportPurchaseHelpers.TNoListGetItem(i));
			}

			CString strComment = "";

			strComment.Format("TNo : %s",
				(const char*)csv.GetLine());

			csvLine.Add(strComment);
		}
	}

	m_Report.WriteLine(csvLine);
}

//******************************************************************

bool CReportStatement::AnythingToReport()
{
	if (balances.dOpening[nINDEX_PURSE1] != balances.dClosing[nINDEX_PURSE1])
	{
		return TRUE;
	}

	if (balances.dOpening[nINDEX_PURSE2] != balances.dClosing[nINDEX_PURSE2])
	{
		return TRUE;
	}

	if (balances.dOpening[nINDEX_PURSE3] != balances.dClosing[nINDEX_PURSE3])
	{
		return TRUE;
	}

	if (balances.dAdditions[nINDEX_PURSE1] != 0)
	{
		return TRUE;
	}

	if (balances.dAdditions[nINDEX_PURSE2] != 0)
	{
		return TRUE;
	}

	if (balances.dAdditions[nINDEX_PURSE3] != 0)
	{
		return TRUE;
	}

	if (balances.dDeductions[nINDEX_PURSE1] != 0)
	{
		return TRUE;
	}

	if (balances.dDeductions[nINDEX_PURSE2] != 0)
	{
		return TRUE;
	}

	if (balances.dDeductions[nINDEX_PURSE3] != 0)
	{
		return TRUE;
	}

	return FALSE;
}

//******************************************************************

bool CReportStatement::SaveStatement(__int64 nUserID)
{
	if (m_ReportPurchaseHelpers.IsStatementReportAll() == TRUE)			
	{
		if (AnythingToReport() == FALSE)
		{
			return FALSE;
		}
	}

	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nUserID);
	CSQLRepositoryAccount RepoAccount;
	bool bRowOK = (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE);

	AddCardDetails(RowAccount);		// add user details at top of report

	AddColumnBalanceLine();				// Purse1 / Purse2  Purse 3 column heading

	AddBalanceLine(0);				// opening balance
	AddBalanceLine(1);				// additions
	AddBalanceLine(2);				// bonuses
	AddBalanceLine(3);				// deductions
	AddBalanceLine(4);				// closing

	m_Report.WriteLine("");

	AddColumnStatementLine();						// add statement purchase column header line

	int nCount = AddStatementPurchases(RowAccount, bRowOK);	// add itemised \ summary purchase item lines
	AddEndStatementLine(nCount);

	// get ready for next page

	if (m_ReportPurchaseHelpers.GetNewPageFlag() == TRUE)
	{
		m_bNewPageReqd = TRUE;
	}
	else
	{
		m_Report.WriteLine("");
	}

	return TRUE;
}

//******************************************************************

void CReportStatement::AddColumnText()
{
	m_bNewPageReqd = FALSE;

	m_Report.AddColumn("", TA_LEFT, 200);				//
	m_Report.AddColumn("", TA_LEFT, 200);				// Date
	m_Report.AddColumn("", TA_RIGHT, 180);				// Time \ purse1
	m_Report.AddColumn("", TA_RIGHT, 180);				// Qty  \ purse2
	m_Report.AddColumn("", TA_RIGHT, 180);				// Value \purse3

	if (m_ReportPurchaseHelpers.GetPurchaseReportType() == nPURCHASE_ITEMISED)	// Itemised Description
	{
		m_Report.AddColumn("", TA_LEFT, 800);
	}
	else
	{
		m_Report.AddColumn("", TA_LEFT, 200);			// TNo comment column
	}
}

//******************************************************************

void CReportStatement::AddColumnStatementLine()
{
	CCSV csv ( '\t' );
	if (m_ReportPurchaseHelpers.GetPurchaseReportType() == nPURCHASE_ACCOUNTITEM )
	{
		csv.Add ( "Purchases" );
		csv.Add ( "Item" );
		csv.Add ( "Qty" );
		csv.Add ( "Value");
	}
	else
	{
		csv.Add ( "Purchases" );
		csv.Add ( "Date" );
		csv.Add ( "Time" );
		csv.Add ( "Qty" );
		csv.Add ( "Value" );
		csv.Add ( "Item" );
	}
	m_Report.WriteLine ( csv );
}

//******************************************************************

void CReportStatement::AddEndStatementLine ( int nCount )
{
	CCSV csv ( '\t' );

	if (0 == nCount)
	{
		csv.Add("<..>No sales");
	}
	else
	{
		csv.Add("<..>End of report");
	}

	m_Report.WriteLine ( csv );
}

//******************************************************************
// no actual audit lines found - so find previous balances as start & end balance

int CReportStatement::SingleInactiveBalance(CSmartPayAuditFile* pAudit, CWorkingDlg* pDlgWorking)
{
	CString strBeginDate = "";
	strBeginDate.Format("%2.2d/%2.2d/%4.4d", 
		m_ReportPurchaseHelpers.GetDateFrom().GetDay(),
		m_ReportPurchaseHelpers.GetDateFrom().GetMonth(),
		m_ReportPurchaseHelpers.GetDateFrom().GetYear() - 1);
	pAudit->JumpIn(strBeginDate);								// go back one year from requested start

	CSQLAuditRecord atcRecord;
	CSSDate dateBegin(strBeginDate);

	bool bHaveData = FALSE;
	bool bCancelButton = FALSE;
	CString strCardNo = m_ReportPurchaseHelpers.GetFromCardNo();				// get single card number

	while (TRUE)
	{
		if (pDlgWorking->SetCaption2PercentDone(pAudit->GetPercentPosition(),TRUE) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		if (pAudit->ReadLine(&atcRecord) == FALSE)
		{
			break;
		}

		int nSourceType = atcRecord.GetSourceType();

		CString strAuditDate = "";
		switch (atcRecord.GetSourceType())
		{
		case AUDIT_POS:
		case AUDIT_KIOSKLINK:
		case AUDIT_VENDING:
		case AUDIT_EXT:
			strAuditDate = atcRecord.GetDateLastUsed();
			break;

		default:
			strAuditDate = atcRecord.GetDateFiled();
			break;
		}

		CSSDate dateAudit(strAuditDate);
		if (dateAudit.IsSet() == FALSE || dateAudit < dateBegin)		// date 1 year before reference date
		{
			continue;
		}

		if (dateAudit > m_ReportPurchaseHelpers.GetDateTo())							// passed required end date
		{
			break;
		}

		if (strCardNo == atcRecord.GetUserID())
		{
			if (m_ReportPurchaseHelpers.CheckTerminalFilter(atcRecord.GetFolderSet(),atcRecord.GetTerminalNo()) == TRUE)	// check is in terminal number range
			{
				balances.dOpening[nINDEX_PURSE1] = atcRecord.GetCurrentPurse1Total();		// save as opening balance
				balances.dOpening[nINDEX_PURSE2] = atcRecord.GetCurrentPurse2Balance();
				balances.dOpening[nINDEX_PURSE3] = atcRecord.GetCurrentPurse3Total();

				balances.dClosing[nINDEX_PURSE1] = atcRecord.GetCurrentPurse1Total();		// same same as closing balance
				balances.dClosing[nINDEX_PURSE2] = atcRecord.GetCurrentPurse2Balance();
				balances.dClosing[nINDEX_PURSE3] = atcRecord.GetCurrentPurse3Total();

				bHaveData = TRUE;
			}
		}
	}

	if (bCancelButton == TRUE)
	{
		return nREPORT_CANCEL;
	}

	int nErrorNo = nREPORT_NOTRANSACTIONS;					// assume no data

	if (bHaveData == TRUE)
	{
		if (SaveStatement(_atoi64(strCardNo)) == TRUE)			// will force a  display
		{
			nErrorNo = nREPORT_NOERROR;
		}
	}

	return nErrorNo;
}

//******************************************************************
