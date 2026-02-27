/******************************************************************************/
#include "resource.h"
/******************************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLTable_PluInfo\SQLRowPluInfo.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRecordSetAccountUserId.h"
#include "..\SmartPay4Shared\TLogFile.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/******************************************************************************/
#include "ReportPurchaseBreakdown.h"
//******************************************************************************

CPurchaseBreakdownReport::CPurchaseBreakdownReport ( const char* szLabel, CWnd* pParent ) : m_Repman("",NULL)
{
	m_ReportPurchaseHelpers.SetReportLabel(szLabel);

	m_pParent = pParent;
	m_bShowGroupColumn = FALSE;

	m_bShowPoints = System.GetActiveEnablePointsFlag();

	m_strReportName = Filenames.GetReportFilename ( szLabel );
	m_strParamsName = Filenames.GetReportParamsFilename ( szLabel );

	ResetReportTotals();
}

//******************************************************************************

void CPurchaseBreakdownReport::ResetTotals(int n)
{
	m_dTotalQty[n] = 0;
	m_dTotalRefundQty[n] = 0;
	m_dTotalValue[n] = 0;;
	m_nTotalPoints[n] = 0;
	m_nTotalCount[n] = 0;
}

//******************************************************************************

void CPurchaseBreakdownReport::ResetReportTotals()
{
	for (int i = 0; i < 3; i++)			// [0]Group / [1]Orderby / [2] Report totals
	{
		ResetTotals(i);
	}
}

//******************************************************************************

void CPurchaseBreakdownReport::Show()
{
	int nErrorNo = DisplayReport();									// range of accounts
	m_Repman.DisplayError ( nErrorNo );								// display error if any
}

//*******************************************************************

int CPurchaseBreakdownReport::DisplayReport()
{
	while (TRUE)
	{
		CPurchaseBreakdownPropertySheet propSheet(m_ReportPurchaseHelpers, m_pParent);

		m_Repman.SetParentWindow(&propSheet);
		m_Repman.SetTitle(m_ReportPurchaseHelpers.GetWindowTitle());

		if (propSheet.DoModal() != IDOK)
		{
			return nREPORT_EXIT;
		}

		propSheet.SaveReportPurchaseHelpers();

		if (m_Report.Open(m_strReportName) == FALSE)
		{
			return nREPORT_CREATEFAIL;
		}

		int nLineCount = 0;
		CString strReporTitle = m_ReportPurchaseHelpers.GetBreakdownReportTitle();

		m_bShowGroupColumn = FALSE;
		if ((m_ReportPurchaseHelpers.GetSortByGroupFlag() == TRUE) || (m_ReportPurchaseHelpers.GetSingleGroupNo() != 0))
		{
			m_bShowGroupColumn = TRUE;
		}

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

		CString strFilename = Filenames.GetTempReportPath(m_ReportPurchaseHelpers.GetReportLabelInternal(), "dbs");
		CFileRecycler::SendToRecycleBin(strFilename);								// clear if found

		m_dbSales.Open(strFilename, DB_READWRITE);

		CWorkingDlg WorkingDlg("Filtering Account List for Report");
		WorkingDlg.Create();

		bool bCancelButton = FALSE;

		if (m_ReportPurchaseHelpers.GetReportRangeType() == nREPORT_LIST)
		{
			CArray<__int64, __int64> arrayIDs;
			CSQLRepositoryAccount RepoAccount;
			RepoAccount.GetUserIDList(arrayIDs, NULL);

			int nCount = 0;
			for (int n = 0; n < arrayIDs.GetSize(); n++)
			{
				__int64 nUserID = arrayIDs.GetAt(n);

				if (WorkingDlg.SetCaption2RecordsChecked(++nCount) == TRUE)
				{
					bCancelButton = TRUE;
					break;
				}

				if (m_ReportPurchaseHelpers.CheckUserIDAgainstList(nUserID) == TRUE)
				{
					CSQLRowAccountFull RowAccount;
					RowAccount.SetUserID(nUserID);

					if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
					{
						ExtractPurchases(RowAccount, &WorkingDlg, bCancelButton);
					}
				}
			}
		}
		else
		{
			CPurchaseHistorySearchInfo infoSearch;
			m_ReportPurchaseHelpers.GetSQLSearchInfoBreakdown(infoSearch);

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

				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserID(nUserID);

				if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					if (m_ReportPurchaseHelpers.NonSQLFilterBreakdown(RowAccount) == TRUE)
					{
						ExtractPurchases(RowAccount, &WorkingDlg, bCancelButton);
					}
				}
			}
		}

		if (bCancelButton == FALSE)
		{
			ResetReportTotals();

			m_Report.SetStyle1(m_ReportPurchaseHelpers.GetBreakdownReportTitle());
			AddColumnText();

			int nLastGroupNo = 0;
			CString strLastOrderBy = "";

			for (int n = 0; n < m_dbSales.GetRecordCount(); n++)
			{
				m_dbSales.MoveTo(n);

				CString strOrderBy = m_dbSales.GetOrderBy();
				int nGroupNo = m_dbSales.GetGroupNo();
				double dQty = m_dbSales.GetQty();
				double dRefundQty = m_dbSales.GetRefundQty();
				double dValue = m_dbSales.GetValue();
				int nPoints = m_dbSales.GetPoints();
				int nCount = m_dbSales.GetCount();

				if (n == 0)										// on first time
				{
					strLastOrderBy = strOrderBy;
					nLastGroupNo = nGroupNo;
				}

				if (strOrderBy != strLastOrderBy)
				{
					ShowGroupTotals(nLastGroupNo);				// Group totals
					ShowTotals(1, strLastOrderBy);				// Orderby totals

					if (m_ReportPurchaseHelpers.GetAllNewPageFlags() != 0)
					{
						m_Report.WriteLine("<NEWPAGE>,1");
					}
					else
					{
						m_Report.WriteLine("");
					}

					strLastOrderBy = strOrderBy;
					nLastGroupNo = nGroupNo;
				}

				if (nLastGroupNo != nGroupNo)
				{
					ShowGroupTotals(nLastGroupNo);					// Group totals
					m_Report.WriteLine("");
					nLastGroupNo = nGroupNo;
				}

				CCSV csv('\t');

				if (m_ReportPurchaseHelpers.GetInfoTextIndex() > 0)
				{
					csv.Add(strOrderBy);
				}

				if (m_bShowGroupColumn == TRUE)
				{
					csv.Add(nGroupNo);
				}

				csv.Add(m_dbSales.GetPluNo());
				csv.Add(m_dbSales.GetText());
				csv.Add(nCount);
				csv.Add(::FormatQty(dRefundQty));
				csv.Add(::FormatQty(dQty));
				csv.Add(dValue, System.GetDPValue());

				if (m_bShowPoints == TRUE)
				{
					csv.Add(nPoints);
				}

				if (m_Report.WriteLine(csv) == TRUE)
				{
					++nLineCount;
					m_nTotalCount[0] += nCount;
					m_dTotalQty[0] += dQty;
					m_dTotalRefundQty[0] += dRefundQty;
					m_dTotalValue[0] += dValue;
					m_nTotalPoints[0] += nPoints;
				}
			}

			if (nLineCount > 0)
			{
				ShowGroupTotals(nLastGroupNo);		// Group totals if any
				ShowTotals(1, strLastOrderBy);				// Orderby totals if any
				ShowTotals(2, "Report");				// Orderby totals
			}
		}

		m_dbSales.Close();
		m_Report.Close();
		WorkingDlg.DestroyWindow();

		if (bCancelButton == FALSE)
		{
			int nErrorNo = nREPORT_NOMATCH;

			CRepmanHandler repman(strReporTitle, m_pParent);
			if (nLineCount > 0)
			{
				nErrorNo = repman.DisplayReport(m_strReportName, m_strParamsName, m_ReportPurchaseHelpers.GetReportKey());
			}

			repman.DisplayError(nErrorNo);
		}
	}

	return nREPORT_EXIT;
}

//*****************************************************************************/

void CPurchaseBreakdownReport::ShowGroupTotals ( int nGroupNo )
{
	CString strGroupName;

	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(nGroupNo);

	CSQLRepositoryGroup RepoGroup;
	if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		strGroupName.Format("Group %2.2d", nGroupNo);
	}
	else
	{
		strGroupName = RowGroup.GetGroupName();
	}

	ShowTotals ( 0, strGroupName );
}

//*****************************************************************************/

void CPurchaseBreakdownReport::ExtractPurchases(CSQLRowAccountFull& RowAccount, CWorkingDlg* pDlgProgress, bool& bCancelButton)
{
	CString strOrderby = RowAccount.GetInfo(m_ReportPurchaseHelpers.GetInfoTextIndex());
	if (m_ReportPurchaseHelpers.GetInfoTextIndex() > 0)
	{
		if ((m_ReportPurchaseHelpers.GetSkipOnBlankFlag() == TRUE) && strOrderby == "")
		{
			return;
		}
	}

	CTLogFile tlogFile(RowAccount.GetUserIDString());
	if (tlogFile.Open() == FALSE)
		return;

	int nGroupNo = (m_bShowGroupColumn == TRUE) ? RowAccount.GetGroupNo() : 0;

	tlogFile.JumpIn(m_ReportPurchaseHelpers.GetDateFrom().GetDate());

	CTLogRecord tlogRecord;
	while (tlogFile.ReadLine(&tlogRecord) == TRUE)
	{
		if (pDlgProgress != NULL)
		{
			if (pDlgProgress->SetCaption2PercentDone(tlogFile.GetPercentPosition()) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}
		}

		if (tlogRecord.m_date < m_ReportPurchaseHelpers.GetDateFrom())
			continue;

		if (tlogRecord.m_date > m_ReportPurchaseHelpers.GetDateTo())
			break;

		if (m_ReportPurchaseHelpers.GetTimeRangeSetFlag() == TRUE)
		{
			if (m_ReportPurchaseHelpers.GetContinuousFlag() == FALSE)
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
						continue;
				}

				if (tlogRecord.m_date == m_ReportPurchaseHelpers.GetDateTo())
				{
					if (tlogRecord.m_time > m_ReportPurchaseHelpers.GetTimeTo())
						continue;
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

			if (tlogRecord.GetItemLineVal() == 0 && (m_ReportPurchaseHelpers.GetAllExcludeZeroPriceFlags() != 0))
			{
				continue;
			}

			CString strText = szNODATA;
			CString strPluNo = szNODATA;

			int nItemType = tlogRecord.GetItemType();	

			switch (nItemType)
			{
			case nTLOGTYPE_PLU:
			case nTLOGTYPE_PLU2:
			case nTLOGTYPE_DISC_ITEM:
				strText = tlogRecord.GetItemText();
				strPluNo = CSQLRowPluInfo::FormatPluNo(tlogRecord.GetItemNo());	// plu no
				break;

			case nTLOGTYPE_DEPT:																// Dept No
			case nTLOGTYPE_DISC_SUBT:
				break;													// discount
			}

			CString strTextPrefix = (nTLOGTYPE_DISC_ITEM == nItemType) ? "1" : "0";

			if (strPluNo != szNODATA)
			{
				if (m_ReportPurchaseHelpers.GetSelectivePluFlag() == TRUE)					// see if selected report reqd
				{
					if (ValidatePlu(strPluNo) == FALSE)						// see if need to report on it
					{
						continue;
					}
				}

				CString strKey = m_dbSales.MakeKey(strOrderby, nGroupNo, strTextPrefix + strText);
				if (m_dbSales.FindRecord(strKey) == FALSE)
				{
					m_dbSales.AddBlankRecord(strKey);
				}

				m_dbSales.SetOrderBy(strOrderby);
				m_dbSales.SetGroupNo(nGroupNo);
				m_dbSales.SetPluNo(strPluNo);
				m_dbSales.SetText(strText);
				m_dbSales.IncCount();

				if (tlogRecord.GetItemType() != nTLOGTYPE_DISC_ITEM)
				{
					if (tlogRecord.GetItemLineQty() < 0)
					{
						m_dbSales.AddRefundQty(-(tlogRecord.GetItemLineQty()));
					}
					else
					{
						m_dbSales.AddQty(tlogRecord.GetItemLineQty());
					}
				}

				m_dbSales.AddValue(tlogRecord.GetItemLineVal());
				m_dbSales.AddPoints(tlogRecord.GetItemPoints());
			}
		}
	}

	tlogFile.Close();
}

//*******************************************************************

void CPurchaseBreakdownReport::AddColumnText()
{
	if (m_ReportPurchaseHelpers.GetInfoTextIndex() > 0)				// see if OrdetBy is not 'Don't care'
	{
		m_Report.AddColumn(m_ReportPurchaseHelpers.GetOrderByLabel(), TA_LEFT, 300);
	}

	if (m_bShowGroupColumn == TRUE)
	{
		m_Report.AddColumn("Grp", TA_LEFT, 93);				// only show group column if required
	}

	m_Report.AddColumn("PluNo", TA_LEFT, 300);
	m_Report.AddColumn("Description", TA_LEFT, 500);
	m_Report.AddColumn("Trans", TA_RIGHT, 210);
	m_Report.AddColumn("Refunds", TA_RIGHT, 210);
	m_Report.AddColumn("QtySold", TA_RIGHT, 219);
	m_Report.AddColumn("Value", TA_RIGHT, 249);

	if (m_bShowPoints == TRUE)
	{
		m_Report.AddColumn("Points", TA_RIGHT, 210);
	}
}

//*******************************************************************

void CPurchaseBreakdownReport::ShowTotals(int n, const char* szText)
{
	double dQty = m_dTotalQty[n];
	double dValue = m_dTotalValue[n];
	double dRefundQty = m_dTotalRefundQty[n];

	CString strTitle = "";
	strTitle.Format("<..>%s Total", szText);

	switch (n)
	{
	case 0:
	case 1:
		if ((dQty == 0) && (dValue == 0) && (dRefundQty == 0))
		{
			return;
		}
		break;
	}

	bool bShowTotal = TRUE;

	switch (n)
	{
	case 0:
		if (m_ReportPurchaseHelpers.GetSortByGroupFlag() == FALSE)		// Group total not reqd if not sorting by group
		{
			bShowTotal = FALSE;
		}
		break;

	case 1:
		if (m_ReportPurchaseHelpers.GetSingleGroupNo() != 0)		// OrderBy total not reqd single group report
		{
			bShowTotal = FALSE;
		}
		break;

	default:
		break;
	}

	CCSV csv('\t');
	CString strTabLine = "";

	if (m_ReportPurchaseHelpers.GetInfoTextIndex() > 0)
	{
		csv.Add("");								// Orderby - only show if not 'Don't care'
		strTabLine = "\t";
	}

	if (m_bShowGroupColumn == TRUE)
	{
		csv.Add("");								// Group - only show column if required
		strTabLine = "\t";
	}

	strTabLine += "\t\t<LI>\t<LI>\t<LI>\t<LI>";

	csv.Add("");									// PluNo
	csv.Add(strTitle);							// desc
	csv.Add(m_nTotalCount[n]);					// count
	csv.Add(::FormatQty(dRefundQty));			// refund qty
	csv.Add(::FormatQty(dQty));				// sold qty
	csv.Add(dValue, System.GetDPValue());			// value 

	if (m_bShowPoints == TRUE)
	{
		csv.Add(m_nTotalPoints[n]);
		strTabLine += "\t<LI>";
	}

	if (bShowTotal == TRUE)
	{
		m_Report.WriteLine(strTabLine);
		m_Report.WriteLine(csv);
	}

	if (n == 0)									// group total
	{
		m_dTotalQty[1] += m_dTotalQty[0];		// add to order by total
		m_dTotalRefundQty[1] += m_dTotalRefundQty[0];
		m_dTotalValue[1] += m_dTotalValue[0];
		m_nTotalPoints[1] += m_nTotalPoints[0];
		m_nTotalCount[1] += m_nTotalCount[0];
		ResetTotals(0);
	}
	else if (n == 1)								// orderby total
	{
		m_dTotalQty[2] += m_dTotalQty[1];		// ad to report totals
		m_dTotalRefundQty[2] += m_dTotalRefundQty[1];
		m_dTotalValue[2] += m_dTotalValue[1];
		m_nTotalPoints[2] += m_nTotalPoints[1];
		m_nTotalCount[2] += m_nTotalCount[1];
		ResetTotals(1);
	}
}

//*************************************************************************

bool CPurchaseBreakdownReport::ValidatePlu ( const char* szSalePluNo )
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

//*******************************************************************

