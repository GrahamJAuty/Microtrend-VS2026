//******************************************************************************
#include "resource.h"
//******************************************************************************
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_PluInfo\SQLRowPluInfo.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRecordSetAccountUserId.h"
#include "..\SmartPay4Shared\TLogFile.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//******************************************************************************
#include "MyConsolidateArray.h"
//******************************************************************************
#include "ReportPurchaseHistory.h"
//******************************************************************************
// R104 = Purchase History - All
// R105 = Purchase History - Single
// R121 - Account Statement - All
// R122 - Account Statement  -Single
//******************************************************************************

CPurchaseReport::CPurchaseReport ( const char* szLabel, __int64 nSingleUserID, CWnd* pParent )
	: m_Repman("")
{
	m_ReportPurchaseHelpers.SetReportLabel(szLabel);

	m_bNewPageReqd = FALSE;

	//SINGLE USER ID IS IGNORED IF REPORTING ON ALL ACCOUNTS
	m_nSingleUserID = nSingleUserID;
	m_pParent = pParent;

	m_Repman.SetTitle(m_ReportPurchaseHelpers.GetWindowTitle());
	
	//NO POINTS ON STATEMENT SUMMARY
	if ( m_ReportPurchaseHelpers.IsStatementReport() == TRUE )
	{
		m_bShowPoints = FALSE;
	}
	else
	{
		m_bShowPoints = System.GetActiveEnablePointsFlag();	
	}

	Reset();
}

//******************************************************************************

void CPurchaseReport::Reset()
{
	m_strLastDate = "";
	m_dCardSpendTotal = 0.0;
	m_dValueTotal = 0.0;				// total value of spend
	m_nPointsTotal = 0;					// total points for given spend
}

//******************************************************************************

void CPurchaseReport::ResetReportTotals()
{
	m_dReportTotalValue = 0.0;				// total value of selected plus sold
	m_dReportTotalQty = 0;					// number of selective plus sold
	m_nReportTotalPoints = 0;				// number of accounts buying selective plus
}

//******************************************************************************
// not used by Account Statement reporting ( R121 or R122 )

void CPurchaseReport::Show()
{
	int nErrorNo = 0;
	if (m_ReportPurchaseHelpers.IsPurchaseHistoryReportSingle() == TRUE)
	{
		nErrorNo = DisplayOne();	// single account
	}
	else
	{
		nErrorNo = DisplayAll();	// range of accounts
	}

	m_Repman.DisplayError(nErrorNo);								// display error if any
}

//******************************************************************************

int CPurchaseReport::DisplayAll()
{
	while (TRUE)
	{
		CPurchasePropertySheet propSheet(m_ReportPurchaseHelpers, m_pParent);
		m_Repman.SetParentWindow(&propSheet);

		if (propSheet.DoModal() != IDOK)
		{
			break;
		}

		propSheet.SaveReportPurchaseHelpers();

		if (m_Report.Open( m_ReportPurchaseHelpers.GetReportFilename()) == FALSE)
		{
			return nREPORT_CREATEFAIL;
		}

		Reset();
		ResetReportTotals();										// used when slective plu repotr

		int nLineCount = 0;
		CString strReportTitle = m_ReportPurchaseHelpers.GetPurchaseReportTitle();

		if (m_ReportPurchaseHelpers.GetSelectivePluFlag() == TRUE)
		{
			CFileStringArray filePlu;
			filePlu.Read(Filenames.GetPluPurchaseListFilename(m_ReportPurchaseHelpers.GetReportLabelInternal()));	// selective plu items to report
		
			m_arrayPluNumbers.RemoveAll();
			for (int n = 0; n < filePlu.GetSize(); n++)
			{
				CSortedInt64Item item;
				item.m_nItem = _atoi64(filePlu.GetAt(n));
				m_arrayPluNumbers.Consolidate(item);
			}
		}

		CWorkingDlg WorkingDlg("Filtering Account List for Report");
		WorkingDlg.Create();

		bool bCancelButton = FALSE;
		int nCount = 0;
		CArray<__int64, __int64> arrayUserIDs;

		if (m_ReportPurchaseHelpers.GetReportRangeType() == nREPORT_LIST)
		{
			CSQLRepositoryAccount RepoAccount;
			RepoAccount.GetUserIDList(arrayUserIDs, NULL);

			for (int n = 0; n < arrayUserIDs.GetSize(); n++)
			{
				if (WorkingDlg.SetCaption2RecordsChecked(++nCount) == TRUE)
				{
					bCancelButton = TRUE;
					break;
				}

				__int64 nUserID = arrayUserIDs.GetAt(n);;
				if (m_ReportPurchaseHelpers.CheckUserIDAgainstList(nUserID) == FALSE)
				{
					arrayUserIDs.RemoveAt(n--);
				}
			}
		}
		else
		{
			CPurchaseHistorySearchInfo infoSearch;
			m_ReportPurchaseHelpers.GetSQLSearchInfoPurchase(infoSearch);

			CString strWhere = "";
			CSQLBuilderBind BuilderBind;
			CSQLRepositoryAccount RepoAccount;
			RepoAccount.PreparePurchaseWhere(infoSearch, strWhere, BuilderBind);

			CSQLRecordSetAccountUserID RecordSet(
				NULL,
				RSParams_AccountUserId_NormalByParams{ &BuilderBind, strWhere });
	
			int nCount = 0;

			__int64 nUserID = 0;
			while (RecordSet.StepSelect(nUserID) == TRUE)
			{
				if (WorkingDlg.SetCaption2RecordsChecked(++nCount) == TRUE)
				{
					bCancelButton = TRUE;
					break;
				}

				arrayUserIDs.Add(nUserID);
			}
		}

		if (bCancelButton == FALSE)
		{
			m_Report.SetStyle1(strReportTitle);
			AddColumnText();

			WorkingDlg.SetCaption1("Processing Accounts");

			bool bHaveSeparator = FALSE;
			for (int nIndex = 0; nIndex < arrayUserIDs.GetSize(); nIndex++)
			{
				WorkingDlg.SetCaption2RecordsOfTotal(nIndex + 1, arrayUserIDs.GetSize(), FALSE);

				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserID(arrayUserIDs.GetAt(nIndex));

				CSQLRepositoryAccount RepoAccount;
				if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					if (m_ReportPurchaseHelpers.GetPurchaseReportType() == nPURCHASE_ACCOUNTITEM)
					{
						nLineCount += PrintConsolidateReport(RowAccount, &WorkingDlg, bCancelButton);
					}
					else
					{
						nLineCount += PrintPurchaseReport(RowAccount, &WorkingDlg, bCancelButton);
					}
				}

				if (bCancelButton == TRUE)
				{
					break;
				}
			}
		}

		//if (m_ReportPurchaseHelpers.GetSelectivePluFlag() == TRUE)					// see if selected report on reqd
		{
			PrintReportTotals();													// add report totals
		}

		m_Report.Close();
		WorkingDlg.DestroyWindow();

		if (bCancelButton == FALSE)
		{
			int nErrorNo = nREPORT_NOMATCH;

			CRepmanHandler repman(strReportTitle, m_pParent);
			if (nLineCount > 0)
			{
				nErrorNo = repman.DisplayReport(m_ReportPurchaseHelpers.GetReportFilename(), m_ReportPurchaseHelpers.GetParamsFilename(), m_ReportPurchaseHelpers.GetReportKey());
			}

			repman.DisplayError(nErrorNo);
		}
	}

	return nREPORT_EXIT;
}

//******************************************************************************
// single account report

int CPurchaseReport::DisplayOne()
{
	bool bFirstTime = TRUE;

	while ( TRUE)
	{
		CPurchasePropertySheet propSheet(m_ReportPurchaseHelpers, m_pParent);

		if (TRUE == bFirstTime)
		{
			CString strSingleUserID = System.FormatCardNo(m_nSingleUserID, TRUE);
			m_ReportPurchaseHelpers.SetFromCardNo(strSingleUserID);
			m_ReportPurchaseHelpers.SetToCardNo(strSingleUserID);
			bFirstTime = FALSE;
		}

		if (propSheet.DoModal() != IDOK)
		{
			break;
		}

		if (m_Report.Open(m_ReportPurchaseHelpers.GetReportFilename()) == FALSE)
		{
			return nREPORT_CREATEFAIL;
		}

		Reset();
		ResetReportTotals();
		int nLineCount = 0;
		CString strReportTitle = m_ReportPurchaseHelpers.GetPurchaseReportTitle();

		if (m_ReportPurchaseHelpers.GetSelectivePluFlag() == TRUE)
		{
			CFileStringArray filePlu;
			filePlu.Read(Filenames.GetPluPurchaseListFilename(m_ReportPurchaseHelpers.GetReportLabelInternal()));	// selective plu items to report

			m_arrayPluNumbers.RemoveAll();
			for (int n = 0; n < filePlu.GetSize(); n++)
			{
				CSortedInt64Item item;
				item.m_nItem = _atoi64(filePlu.GetAt(n));
				m_arrayPluNumbers.Consolidate(item);
			}
		}

		CWorkingDlg WorkingDlg ( szMSG_CREATINGREPORT );
		WorkingDlg.Create();
		
		bool bCancelButton = FALSE;

		m_Report.SetStyle1 ( strReportTitle );
		AddColumnText();

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(m_nSingleUserID);

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if (m_ReportPurchaseHelpers.GetPurchaseReportType() == nPURCHASE_ACCOUNTITEM)
			{
				nLineCount = PrintConsolidateReport(RowAccount, &WorkingDlg, bCancelButton);
			}
			else
			{
				nLineCount = PrintPurchaseReport(RowAccount, &WorkingDlg, bCancelButton);
			}
		}

		m_Report.Close();
		WorkingDlg.DestroyWindow();

		if ( bCancelButton == FALSE )
		{
			int nErrorNo = nREPORT_NOTRANSACTIONS;

			CRepmanHandler repman ( strReportTitle, m_pParent );
			if (nLineCount > 0)
			{
				nErrorNo = repman.DisplayReport(m_ReportPurchaseHelpers.GetReportFilename(), m_ReportPurchaseHelpers.GetParamsFilename(), m_ReportPurchaseHelpers.GetReportKey());
			}

			repman.DisplayError ( nErrorNo );
		}
	}
	return nREPORT_EXIT;
}

//******************************************************************************
// Statement sales for a single card for a given period

int CPurchaseReport::AddStatementPurchases(CSQLRowAccountFull& RowAccount, bool bRowOK)
{
	Reset();
	bool bCancelButton = FALSE;

	int nLineCount = 0;

	if (TRUE == bRowOK)
	{
		if (m_ReportPurchaseHelpers.GetPurchaseReportType() == nPURCHASE_ACCOUNTITEM)
		{
			nLineCount = PrintConsolidateReport(RowAccount, NULL, bCancelButton);
		}
		else
		{
			nLineCount = PrintPurchaseReport(RowAccount, NULL, bCancelButton);
		}
	}

	return nLineCount;
}

//******************************************************************************

int CPurchaseReport::PrintPurchaseReport(CSQLRowAccountFull& RowAccount, CWorkingDlg* pProgress, bool& bCancelButton)
{
	CTLogFile tlogFile(RowAccount.GetUserIDString());
	if (tlogFile.Open() == FALSE)
	{
		return 0;
	}

	tlogFile.JumpIn(m_ReportPurchaseHelpers.GetDateFrom().GetDate());
	CTLogRecord tlogRecord;

	Reset();												// reset report totals
	int nLineCount = 0;

	if (pProgress != NULL)
	{
		pProgress->SetCaption1("Processing T-Log Records");
	}

	while (tlogFile.ReadLine(&tlogRecord) == TRUE)
	{
		if (pProgress != NULL)
		{
			if (pProgress->SetCaption2PercentDone(tlogFile.GetPercentPosition()))
			{
				bCancelButton = TRUE;
				break;
			}
		}

		if (tlogRecord.m_date < m_ReportPurchaseHelpers.GetDateFrom())
		{
			continue;
		}

		if (m_ReportPurchaseHelpers.GetTimeRangeSetFlag() == TRUE)
		{
			if (tlogRecord.m_time < m_ReportPurchaseHelpers.GetTimeFrom() || tlogRecord.m_time > m_ReportPurchaseHelpers.GetTimeTo())
			{
				continue;
			}
		}

		if (m_ReportPurchaseHelpers.CheckTerminalFilter(tlogRecord.GetFolderSet(), tlogRecord.GetTerminalNo()) == FALSE)
		{
			continue;
		}

		if (tlogRecord.m_date > m_ReportPurchaseHelpers.GetDateTo())
		{
			break;
		}

		if (m_ReportPurchaseHelpers.GetSelectivePluFlag() == TRUE)					// see if selected report on reqd
		{
			// not selected for reporting on
			if (ValidateSale(&tlogRecord) == FALSE)
			{
				continue;
			}
		}

		NewPage();

		if ( m_ReportPurchaseHelpers.IsStatementReport() == TRUE)
		{
			nLineCount += PrintStatementRecord(&tlogRecord);				// print purchases on statement report
		}
		else
		{
			if (nLineCount == 0)											// See if need first line for purchase history
			{
				AddCardDetails(RowAccount);
			}

			nLineCount += PrintPurchaseRecord(&tlogRecord);
		}
	}

	tlogFile.Close();

	if (TRUE == bCancelButton)
	{
		return 0;
	}

	if (nLineCount > 0)													// see if have a last purchase history to prinbr
	{
		PrintPurchaseTotals();
	}

	if (pProgress != NULL)
	{
		pProgress->SetCaption2("");
	}

	return nLineCount;
}

//******************************************************************************
// print purchase histort report line

int CPurchaseReport::PrintPurchaseRecord(CTLogRecord* pTLogRecord)
{
	int nLineCount = 0;

	CString strDate = pTLogRecord->GetDate();
	if (m_strLastDate == strDate)								// same date as previous sale
	{
		strDate = "";
	}

	for (int nIndex = 0; nIndex < pTLogRecord->GetItemCounter(); nIndex++)
	{
		CCSV csv('\t');
		csv.Add(strDate);									// Sale date

		if (nLineCount == 0)									// only do on first printed lines
		{														// may not get used if selected reporting used & plu is not a selected one
			csv.Add(pTLogRecord->GetTime());

			CString strTNo = "";
			if (pTLogRecord->GetTerminalNo() != 0)
			{
				CString strReceiptNo = pTLogRecord->GetReceiptNo();
				if (strReceiptNo != "")						// Version4 format onwards has Receipt number
				{
					strTNo.Format("%d/%s",
						pTLogRecord->GetTerminalNo(),
						(const char*)strReceiptNo);
				}
				else
				{ // Pre version4
					strTNo.Format("%d/%4.4d",
						pTLogRecord->GetTerminalNo(),
						pTLogRecord->GetCCNo());
				}
			}
			csv.Add(strTNo);
			csv.Add(pTLogRecord->GetTotalSpend(), System.GetDPValue());
		}
		else
		{
			csv.Add("");										// time
			csv.Add("");										// TNo
			csv.Add("");										// spend
		}

		pTLogRecord->ExtractItemData(nIndex);				// extract sales item

		int nDeptNo = 0;
		CString strPluNo = szNODATA;
		CString strDeptNo = szNODATA;
		double dQty = pTLogRecord->GetItemLineQty();
		double dValue = pTLogRecord->GetItemLineVal();

		if ( (dValue == 0.0) && m_ReportPurchaseHelpers.GetExcludeZeroPriceFlag() == TRUE)
		{
			continue;
		}

		switch (pTLogRecord->GetItemType())
		{
		case nTLOGTYPE_PLU:
		case nTLOGTYPE_DISC_ITEM:
			strPluNo = CSQLRowPluInfo::FormatPluNo(pTLogRecord->GetItemNo());		// plu no
			break;

		case nTLOGTYPE_PLU2:
			strPluNo = CSQLRowPluInfo::FormatPluNo(pTLogRecord->GetItemNo());		// plu no
			nDeptNo = pTLogRecord->GetItemDeptNo();						// deptNo
			break;

		case nTLOGTYPE_DEPT:
			nDeptNo = pTLogRecord->GetItemDeptNo();						// Dept No
			break;

		case nTLOGTYPE_DISC_SUBT:
			break;														// discount
		}

		if (m_ReportPurchaseHelpers.GetSelectivePluFlag() == TRUE)				// see if selected report reqd
		{
			if (ValidatePlu(strPluNo) == FALSE)						// see if need to report on it
			{
				continue;
			}
		}

		// valid line to add to report

		if (nLineCount == 0)											// check for first line printed
		{
			strDate = "";												// clear date incase more items to print
			m_dCardSpendTotal += pTLogRecord->GetTotalSpend();			// save card total
		}

		if (nDeptNo != 0)
		{
			strDeptNo.Format("%3.3d", nDeptNo);
		}

		csv.Add(strDeptNo);
		csv.Add(strPluNo);
		csv.Add(pTLogRecord->GetItemText());
		csv.Add(pTLogRecord->GetItemTax());

		csv.Add(::FormatQty(dQty));
		m_dReportTotalQty += dQty;

		csv.Add(dValue, System.GetDPValue());
		m_dValueTotal += dValue;
		m_dReportTotalValue += dValue;

		if (m_bShowPoints == TRUE)
		{
			int nPoints = pTLogRecord->GetItemPoints();
			csv.Add(nPoints);
			m_nPointsTotal += nPoints;
			m_nReportTotalPoints += nPoints;
		}

		if (m_Report.WriteLine(csv) == TRUE)
		{
			++nLineCount;
		}
	}

	if (nLineCount > 0)											// if printed any lines
	{
		m_strLastDate = pTLogRecord->GetDate();						// set last date
	}

	return nLineCount;
}

//******************************************************************************
// print statement purchase items column = " Purchases, Date, Time, Qty, Value, Item "

int CPurchaseReport::PrintStatementRecord(CTLogRecord* pTLogRecord)
{
	int nLineCount = 0;

	for (int nIndex = 0; nIndex < pTLogRecord->GetItemCounter(); nIndex++)
	{
		CCSV csv('\t');

		csv.Add("");

		if (m_strLastDate == pTLogRecord->GetDate())
		{
			csv.Add("");
		}
		else
		{
			m_strLastDate = pTLogRecord->m_date.GetDate();
			csv.Add(m_strLastDate);
		}

		if (nIndex == 0)
		{
			csv.Add(pTLogRecord->GetTime());
		}
		else
		{
			csv.Add("");
		}

		pTLogRecord->ExtractItemData(nIndex);

		double dValue = pTLogRecord->GetItemLineVal();

		if ((dValue == 0.0) && (m_ReportPurchaseHelpers.GetExcludeZeroPriceFlag() == TRUE))
		{
			continue;
		}

		csv.Add(::FormatQty(pTLogRecord->GetItemLineQty()));
		csv.Add(dValue, System.GetDPValue());
		csv.Add(pTLogRecord->GetItemText());

		if (m_Report.WriteLine(csv) == TRUE)
		{
			++nLineCount;
		}
	}

	return nLineCount;
}

//******************************************************************************
// Purchase history consoliated items
// Statement print for consoliaded item, columnns= " Purchases, Item , Qty, Value "

int CPurchaseReport::PrintConsolidateReport(CSQLRowAccountFull& RowAccount, CWorkingDlg* pProgress, bool& bCancelButton)
{
	CTLogFile tlogFile(RowAccount.GetUserIDString());
	if (tlogFile.Open() == FALSE)
	{
		return 0;
	}

	int nPoints = 0;
	double dQty = 0.0, dValue = 0.0;

	CTLogRecord tlogRecord;
	CMyConsolidateArray consolArray(TRUE, TRUE);		// ascending ignorecase

	if (pProgress != NULL)
	{
		pProgress->SetCaption1("Processing T-Log Records");
	}

	tlogFile.JumpIn(m_ReportPurchaseHelpers.GetDateFrom().GetDate());

	while (tlogFile.ReadLine(&tlogRecord) == TRUE)
	{
		if (pProgress != NULL)
		{
			if (pProgress->SetCaption2PercentDone(tlogFile.GetPercentPosition()) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}
		}

		if (tlogRecord.m_date < m_ReportPurchaseHelpers.GetDateFrom())
		{
			continue;
		}

		if (tlogRecord.m_date > m_ReportPurchaseHelpers.GetDateTo())
		{
			break;
		}

		if (m_ReportPurchaseHelpers.GetTimeRangeSetFlag() == TRUE)
		{
			if (m_ReportPurchaseHelpers.GetContinuousFlag() == FALSE )
			{
				if (tlogRecord.m_time < m_ReportPurchaseHelpers.GetTimeFrom() || tlogRecord.m_time > m_ReportPurchaseHelpers.GetTimeTo())
				{
					continue;
				}
			}
			else
			{
				if (tlogRecord.m_date == m_ReportPurchaseHelpers.GetDateFrom())
				{
					if (tlogRecord.m_time < m_ReportPurchaseHelpers.GetTimeFrom())
					{
						continue;
					}
				}

				if (tlogRecord.m_date == m_ReportPurchaseHelpers.GetDateTo())
				{
					if (tlogRecord.m_time > m_ReportPurchaseHelpers.GetTimeTo())
					{
						continue;
					}
				}
			}
		}

		if (m_ReportPurchaseHelpers.CheckTerminalFilter(tlogRecord.GetFolderSet(), tlogRecord.GetTerminalNo()) == FALSE)
		{
			continue;
		}

		for (int nIndex = 0; nIndex < tlogRecord.GetItemCounter(); nIndex++)
		{
			tlogRecord.ExtractItemData(nIndex);
			consolArray.Add(
				tlogRecord.GetItemText(), 
				tlogRecord.GetItemLineQty(), 
				tlogRecord.GetItemLineVal(), 
				(double)tlogRecord.GetItemPoints());
		}
	}

	tlogFile.Close();

	if (bCancelButton == TRUE)
	{
		return 0;
	}

	int nLineCount = 0;
	if (consolArray.GetSize() > 0)							// see if any data to print
	{
		Reset();												// reset report totals

		NewPage();


		if ( m_ReportPurchaseHelpers.IsStatementReport() == FALSE )
		{
			AddCardDetails(RowAccount);
		}

		if (pProgress != NULL)
		{
			pProgress->SetCaption1("Creating Report");
		}

		for (int nIndex = 0; nIndex < consolArray.GetSize(); nIndex++)
		{
			if (pProgress != NULL)
			{
				if (pProgress->SetCaption2PercentDone((nIndex * 100) / consolArray.GetSize()) == TRUE)
				{
					bCancelButton = TRUE;
					break;
				}
			}

			dQty = consolArray.GetValue1(nIndex);			// v1.02i - FIX : Purchase History summary columns wrong way round
			dValue = consolArray.GetValue2(nIndex);
			nPoints = (int)consolArray.GetValue3(nIndex);

			if (dQty != 0.0 || dValue != 0.0 || nPoints != 0)
			{
				CCSV csv('\t');

				if ( m_ReportPurchaseHelpers.IsStatementReport() == TRUE)
				{
					csv.Add("");
				}

				csv.Add(consolArray.GetLabel(nIndex));

				csv.Add(::FormatQty(dQty));
				csv.Add(dValue, System.GetDPValue());

				if (m_bShowPoints == TRUE)
				{
					csv.Add(nPoints);
				}

				m_dValueTotal += dValue;
				m_nPointsTotal += nPoints;

				if (m_Report.WriteLine(csv) == TRUE)
				{
					++nLineCount;
				}
			}
		}

		PrintConsolidateTotals();

		NewPage();
	}

	if (pProgress != NULL)
	{
		pProgress->SetCaption2("");
	}

	return nLineCount;
}

//******************************************************************************
// not used by statement purchases ( R121 or R122 )

void CPurchaseReport::AddColumnText()
{
	m_bNewPageReqd = FALSE;

	if (m_ReportPurchaseHelpers.GetPurchaseReportType() == nPURCHASE_ITEMISED)
	{
		m_Report.AddColumn("Date", TA_LEFT, 219);
		m_Report.AddColumn("Time", TA_LEFT, 120);
		m_Report.AddColumn("TNo", TA_LEFT, 120);
		m_Report.AddColumn("TTL Spend", TA_RIGHT, 240);
		m_Report.AddColumn("Dp", TA_CENTER, 100);
		m_Report.AddColumn("Plu", TA_LEFT, 270);
		m_Report.AddColumn("Description", TA_LEFT, 500);
		m_Report.AddColumn("Tax", TA_CENTER, 90);
		m_Report.AddColumn("Quantity", TA_RIGHT, 200);
		m_Report.AddColumn("Value", TA_RIGHT, 240);

		if (m_bShowPoints == TRUE)
		{
			m_Report.AddColumn("Points", TA_RIGHT, 180);
		}
	}
	else						// summary report
	{
		m_Report.AddColumn("Description", TA_LEFT, 500);
		m_Report.AddColumn("Quantity", TA_RIGHT, 200);
		m_Report.AddColumn("Value", TA_RIGHT, 249);

		if (m_bShowPoints == TRUE)
		{
			m_Report.AddColumn("Points", TA_RIGHT, 249);
		}
	}
}

//******************************************************************************

void CPurchaseReport::PrintPurchaseTotals()
{
	if (m_ReportPurchaseHelpers.IsStatementReport() == TRUE)
	{
		return;
	}

	/*
	if (m_ReportPurchaseHelpers.GetSelectivePluFlag() == TRUE)					// see if selected report on reqd
	{
		m_Report.WriteLine("");
		return;															// just do a report total
	}
	*/

	CString strTabLine = "\t\t\t<LI>\t\t\t\t\t\t<LI>";

	CCSV csv('\t');
	csv.Add("<..>Total");							// date
	csv.Add("");										// time
	csv.Add("");										// TNo
	csv.Add(m_dCardSpendTotal, System.GetDPValue());	// card spend
	csv.Add("");										// Dept
	csv.Add("");										// Plu No.
	csv.Add("");										// description
	csv.Add("");										// Tax
	csv.Add("");										// qty
	csv.Add(m_dValueTotal, System.GetDPValue());		// value

	if (m_bShowPoints == TRUE)
	{
		csv.Add(m_nPointsTotal);						// points
		strTabLine += "\t<LI>";
	}

	m_Report.WriteLine(strTabLine);
	m_Report.WriteLine(csv);

	// get ready for next page

	if (m_ReportPurchaseHelpers.GetNewPageFlag() == TRUE)
	{
		m_bNewPageReqd = TRUE;
	}
	else
	{
		m_Report.WriteLine("");
	}
}

//******************************************************************************

void CPurchaseReport::PrintReportTotals()
{
	CString strTabLine = "\t\t\t\t\t\t\t\t<LI>\t<LI>";

	CCSV csv('\t');
	csv.Add("<..>Report Total");							// date
	csv.Add("");											// time
	csv.Add("");											// TNo
	csv.Add("");											// card spend
	csv.Add("");											// Dept
	csv.Add("");											// Plu No.
	csv.Add("");											// description
	csv.Add("");											// Tax
	csv.Add(::FormatQty(m_dReportTotalQty));			// qty
	csv.Add(m_dReportTotalValue, System.GetDPValue());	// value

	if (m_bShowPoints == TRUE)
	{
		csv.Add(m_nReportTotalPoints);					// points
		strTabLine += "\t<LI>";
	}

	m_Report.WriteLine(strTabLine);
	m_Report.WriteLine(csv);
}

//******************************************************************************

void CPurchaseReport::PrintConsolidateTotals()
{
	if ( m_ReportPurchaseHelpers.IsStatementReport() == TRUE )
	{
		return;
	}

	CString strTabLine = "\t\t<LI>";

	CCSV csv('\t');
	csv.Add("<..>Total");							// desc
	csv.Add("");										// qty
	csv.Add(m_dValueTotal, System.GetDPValue());

	if (m_bShowPoints == TRUE)
	{
		csv.Add(m_nPointsTotal);
		strTabLine += "\t<LI>";
	}

	m_Report.WriteLine(strTabLine);
	m_Report.WriteLine(csv);
}

//******************************************************************************

void CPurchaseReport::NewPage()
{
	if (m_bNewPageReqd == TRUE)
	{
		m_Report.WriteLine("<NEWPAGE>,1");
		m_bNewPageReqd = FALSE;
	}
}

//******************************************************************************

void CPurchaseReport::AddCardDetails(CSQLRowAccountFull& RowAccount)
{
	CString strName = "";
	CString strUserName = RowAccount.GetUsername();
	CString strForename = RowAccount.GetForename();
	CString strSurname = RowAccount.GetSurname();

	if (strForename != "")
	{
		strName = strForename;
	}

	if (strSurname != "")
	{
		if (strForename != "")
		{
			strName += " ";
		}

		strName += strSurname;
	}

	CString strBuf = "";
	if (strName != strUserName && strName != "")			// see if username = forename + surname
	{
		strBuf.Format("  ( %s )",
			(const char*)strName);
	}

	NewPage();												// set new page if requred

	CString strHeader = "";
	strHeader.Format("<..>UserID %s",
		(const char*)System.FormatCardNo(RowAccount.GetUserID(), TRUE));

	if (m_ReportPurchaseHelpers.GetShowAccountNameFlag() == TRUE)
	{
		CString strTmp = "";
		strTmp.Format("  - %s%s",
			(const char*)strUserName,
			(const char*)strBuf);
		strHeader += strTmp;
	}

	m_Report.WriteLine(strHeader);
	m_Report.WriteLine("<LI>");
}

//******************************************************************************
// see if purchase sale contain required plu items

bool CPurchaseReport::ValidateSale(CTLogRecord* pTLogRecord)
{
	bool bReply = FALSE;										// asuume not match

	for (int nIndex = 0; nIndex < pTLogRecord->GetItemCounter(); nIndex++)
	{
		pTLogRecord->ExtractItemData(nIndex);

		CString strPluNo = szNODATA;
		double dValue = pTLogRecord->GetItemLineVal();

		if ( (dValue == 0.0) && (m_ReportPurchaseHelpers.GetExcludeZeroPriceFlag() == TRUE))
		{
			continue;
		}

		switch (pTLogRecord->GetItemType())
		{
		case nTLOGTYPE_PLU:
		case nTLOGTYPE_PLU2:
		case nTLOGTYPE_DISC_ITEM:
			strPluNo = CSQLRowPluInfo::FormatPluNo(pTLogRecord->GetItemNo());		// plu no
			break;

		case nTLOGTYPE_DEPT:											// Dept No
		case nTLOGTYPE_DISC_SUBT:
			break;														// discount
		}

		if ((bReply = ValidatePlu(strPluNo)) == TRUE)
		{
			break;
		}
	}

	return bReply;
}

//******************************************************************************

bool CPurchaseReport::ValidatePlu(const char* szSalePluNo)
{
	bool bReply = FALSE;								
	
	CString strSalePluNo = szSalePluNo;
	if (strSalePluNo != szNODATA)
	{
		CSQLRowPluInfo RowAdjust;
		RowAdjust.SetPluNo(_atoi64(strSalePluNo));
		
		CSortedInt64Item item;
		item.m_nItem = _atoi64(RowAdjust.GetMicrotrendBasePluNo(FALSE));

		int nPos = 0;
		bReply = m_arrayPluNumbers.Find(item, nPos);
	}

	return bReply;
}

//******************************************************************************

