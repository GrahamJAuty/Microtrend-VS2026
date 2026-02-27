/******************************************************************************/
// Low Points Usage
// Low Spend Usage
// Points Ranking
// Spend Ranking
// Active Cards
// Inactive Cards
/******************************************************************************/
#include "..\SmartPay4ManagerBgnd\DbReportFields.h"
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
/******************************************************************************/
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_BioRegister\SQLRepositoryBioRegister.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/******************************************************************************/
#include "AccumulatorPropertySheet.h"
#include "DbExportEditor.h"
/******************************************************************************/
#include "ReportAccumulator.h"
/******************************************************************************/

CReportAccumulator::CReportAccumulator(CAccumulatorReportInfo& ReportInfo, const char* szAuditFile, CWnd* pParent)
	: m_ReportInfo(ReportInfo),
	m_auditFile(szAuditFile)
{
	m_strReportLabel = m_ReportInfo.GetReportLabel();
	m_pParent = pParent;
	m_bAuditFileChecked = FALSE;

	// get report type from report label
	m_nReportType = m_ReportInfo.GetReportType();

	for (int n = 0; n < nREPORT_MAX; n++)
	{
		ClearTotals(n);
	}
}

//******************************************************************************

void CReportAccumulator::ClearTotals(int nIndex)
{
	m_nReportCount[nIndex] = 0;
	m_dReportValue[nIndex] = 0.0;
	m_nReportQty[nIndex] = 0;
}

//******************************************************************************
// Validate audit file

bool CReportAccumulator::Show()
{
	int nErrorNo = nREPORT_NOERROR;

	if (m_bAuditFileChecked == FALSE)						// see if audit has been check yet
	{
		if (m_auditFile.Open() == FALSE)					// try & open audit file
		{
			nErrorNo = nREPORT_NOSOURCE;					// no source data found
		}
		else
		{
			CSSDate date;
			m_bAuditFileChecked = m_auditFile.GetFirstDate(date);	// try & get first date in audit file
			m_auditFile.Close();							// lose audit file

			if (m_bAuditFileChecked == FALSE)					// see if have first date in audit file
			{
				nErrorNo = nREPORT_INVALIDSOURCE;				// exit - invalid source
			}
		}
	}

	// get user added fields

	if (nErrorNo == nREPORT_NOERROR)						// see if any error so far
	{
		{
			CAccumulatorPropertySheet psReport(m_ReportInfo.GetReportLabel(), FALSE, NULL);
			psReport.SetReportInfo(&m_ReportInfo);

			if (psReport.DoModal() == IDCANCEL)
			{
				return FALSE;
			}
		}

		CDbExportEditor editor{};
		if (editor.EditExportLabel(m_strReportLabel) == FALSE)
		{
			return FALSE;
		}

		if (m_DbReporter.Read(m_strReportLabel) == FALSE)
		{
			return TRUE;
		}
	}

	// setup repman

	CRepmanHandler repman(m_DbReporter.GetReportTitle(), m_pParent);
	if (nErrorNo == nREPORT_NOERROR)							// see if any errors so far
	{
		CString strReportFile = m_DbReporter.GetReportFilename();

		if ((nErrorNo = Create(strReportFile)) == nREPORT_NOERROR)
		{
			repman.SetupSaveAs(m_DbReporter.GetSaveAsType(), m_DbReporter.GetSaveAsFile());
			nErrorNo = repman.DisplayReport(strReportFile, m_DbReporter.GetParamsFilename(), m_DbReporter.GetReportKey());
		}
	}

	return repman.DisplayError(nErrorNo);
}

//*******************************************************************

int CReportAccumulator::Create(const char* szReportFile)
{
	if (m_Report.Open(szReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	int nReply = CompileReport();							// make report
	m_Report.Close();

	return nReply;
}

//*******************************************************************

int CReportAccumulator::CompileReport()
{
	m_DbReporter.ClearTotals();
	for (int i = 0; i < nREPORT_MAX; i++)					// clear SortOn \ Group \ Report totals
	{
		ClearTotals(i);
	}

	bool bCancelButton = FALSE;
	CWorkingDlg WorkingDlg("Processing Account Records for Report");
	WorkingDlg.Create();

	CReportConsolidationArray<CSortedDbReportItem> ConsolArray(FALSE, m_DbReporter.GetSortOn() == nSORTONUSERID ? 1 : 0);

	// require when < threshold requested
	if (TRUE == m_ReportInfo.GetNoZeroSkipFlag())	
	{
		CString strWhere = "";
		CSQLBuilderBind BuilderBind{};
		CSQLRepositoryAccount RepoAccount;
		RepoAccount.PrepareExportWhere(m_DbReporter, -1, strWhere, BuilderBind);

		CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ByParams{ &BuilderBind, strWhere });
		RecordSet.m_strSort = m_DbReporter.GetSQLOrderBy();
		
		int nCount = 0;

		CSQLRowAccountFull RowAccount;
		while (RecordSet.StepSelectAll(RowAccount) == TRUE)
		{
			if (m_DbReporter.NonSQLFilter(RowAccount) == TRUE)
			{
				CSortedDbReportItem item;
				m_DbReporter.GetSortItem(RowAccount, item, TRUE);
				item.m_nQty = 1;
				item.m_dVal = 0.0;
				ConsolArray.Consolidate(item);
			}

			if (WorkingDlg.SetCaption2RecordsChecked(++nCount) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}
		}

		if (TRUE == bCancelButton)
		{
			return nREPORT_CANCEL;
		}
	}

	if (m_auditFile.Open() == FALSE)							// reopen audit source file
	{
		return nREPORT_OPENERROR;
	}

	CString strLastDate = "";
	CString strCaption = "Checking Audit Records";
	WorkingDlg.SetCaption1(strCaption);
	int nRecordsChecked = 0;

	m_auditFile.JumpIn(m_ReportInfo.GetDateFromString());
	while (m_auditFile.ReadLine(&m_atc) == TRUE)
	{
		{
			CString strDate = m_atc.GetCSVDateFiled();
			if (strDate != strLastDate)
			{
				CString strDisplayDate = "";

				if (strDate.GetLength() == 8)
				{
					strDisplayDate.Format("%s/%s/%s",
						(const char*)strDate.Left(2),
						(const char*)strDate.Mid(2, 2),
						(const char*)strDate.Right(4));
				}
				else
				{
					strDisplayDate = strDate;
				}

				CString str = "";
				str += strCaption;
				str += " ";
				str += strDisplayDate;
				WorkingDlg.SetCaption1(str);
				strLastDate = strDate;
			}
		}

		if (WorkingDlg.SetCaption2RecordsChecked(++nRecordsChecked) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		if (m_ReportInfo.IsValid(&m_DbReporter, &m_atc) == TRUE)		// see if date in range
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(_atoi64(m_atc.GetUserID()));
			RowAccount.SetGroupNo(m_atc.GetGroupNo());

			CSortedDbReportItem item;
			m_DbReporter.GetSortItem(RowAccount, item, FALSE);

			switch (m_nReportType)
			{
			case nACCUMULATE_LOWPOINTS:
			case nACCUMULATE_RANKPOINTS:
				item.m_nQty = 1;
				item.m_dVal = (double)m_atc.GetTransactionPoints();
				ConsolArray.Consolidate(item);
				break;

			case nACCUMULATE_LOWSPEND:
			case nACCUMULATE_RANKSPEND:
				item.m_nQty = 1;
				item.m_dVal = -(m_atc.GetTransactionTotal());
				ConsolArray.Consolidate(item);
				break;

			case nACCUMULATE_ACTIVE:
			case nACCUMULATE_INACTIVE:
				item.m_nQty = 1;
				item.m_dVal = 1.0;
				ConsolArray.Consolidate(item);
				break;
			}
		}
	}

	m_auditFile.Close();

	if ((ConsolArray.GetSize() == 0) || (TRUE == bCancelButton))
	{
		return nREPORT_NOMATCH;
	}

	m_nReportCount[nREPORT_TOTAL] = 0;

	m_Report.SetStyle1(m_DbReporter.GetReportTitle(m_ReportInfo.GetThresholdText()));
	AddColumnText();

	if (m_nReportType == nACCUMULATE_RANKPOINTS || m_nReportType == nACCUMULATE_RANKSPEND)
	{
		WorkingDlg.SetCaption1("Preparing Ranking Positions");

		CReportConsolidationArray<CSortedDbReportRankingItem> ConsolArrayRanking(FALSE, m_DbReporter.GetSortOn() == nSORTONUSERID ? 1 : 0);

		CString strRankingLabel = "";

		for (int nConsolIndex = 0; nConsolIndex < ConsolArray.GetSize(); nConsolIndex++)
		{
			WorkingDlg.SetCaption2PercentDone((nConsolIndex * 100L) / ConsolArray.GetSize(), FALSE);

			CSortedDbReportItem item;
			ConsolArray.GetAt(nConsolIndex, item);

			if (m_ReportInfo.CheckThreshold(item.m_dVal) == TRUE)
			{
				CSortedDbReportRankingItem rankingItem;
				rankingItem.m_nRanking = int(floor(item.m_dVal * 100.0));
				rankingItem.m_nGroupNo = item.m_nGroupNo;
				rankingItem.m_strSortField = item.m_strSortField;
				rankingItem.m_nUserID = item.m_nUserID;
				rankingItem.m_nConsolIndex = nConsolIndex;
				ConsolArrayRanking.Consolidate(rankingItem);
			}
		}

		WorkingDlg.SetCaption1("Preparing Report");

		double dLastValue = -999999.99;
		int nRank = 0;
		int nCount = 0;

		for (int nRankingIndex = 0; nRankingIndex < ConsolArrayRanking.GetSize(); nRankingIndex++)
		{
			if (WorkingDlg.SetCaption2PercentDone((nRankingIndex * 100) / ConsolArrayRanking.GetSize()) == TRUE)
			{
				break;
			}

			CSortedDbReportRankingItem rankingItem;
			ConsolArrayRanking.GetAt(nRankingIndex, rankingItem);

			CSortedDbReportItem item;
			ConsolArray.GetAt(rankingItem.m_nConsolIndex, item);

			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(item.m_nUserID);

			CSQLRepositoryAccount RepoAccount;
			bool bExist = (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE);
			bool bAllow = TRUE;

			if (FALSE == bExist)
			{
				bAllow = m_DbReporter.GetUnknownReqd();
			}
			else
			{
				CString strWhere = "";
				CSQLBuilderBind BuilderBind{};
				RepoAccount.PrepareExportWhere(m_DbReporter, RowAccount.GetUserID(), strWhere, BuilderBind);

				CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ByParams{ &BuilderBind, strWhere });
				RecordSet.m_strSort = m_DbReporter.GetSQLOrderBy();
				
				CSQLRowAccountFull RowTest;
				if (RecordSet.StepSelectAll(RowTest) == TRUE)
				{
					bAllow = m_DbReporter.NonSQLFilter(RowAccount, TRUE);
				}
				else
				{
					bAllow = FALSE;
				}
			}

			// invalid records may have been added from audit file
			// fiddle need to check for filters that would usually apply via SQL
			if (TRUE == bAllow)
			{
				if (item.m_dVal == dLastValue)
				{
					++nCount;
				}
				else
				{
					nRank = nRank + nCount + 1;

					if ((m_ReportInfo.GetTop100ChartSize() != 0) && (nRank > m_ReportInfo.GetTop100ChartSize()))
					{
						break;
					}

					dLastValue = item.m_dVal;
					nCount = 0;
				}

				SaveReportLine(RowAccount, bExist, item.m_nQty, item.m_dVal, nRank);
			}
		}

		SaveReportTotal(TRUE);
	}
	else											// non ranked report
	{
		WorkingDlg.SetCaption1("Preparing Report");

		int nLastGroupNo = -1;
		CString strLastSortOn = "";

		for (int nConsolIndex = 0; nConsolIndex < ConsolArray.GetSize(); nConsolIndex++)
		{
			if (WorkingDlg.SetCaption2PercentDone((nConsolIndex * 100) / ConsolArray.GetSize()) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}

			CSortedDbReportItem item;
			ConsolArray.GetAt(nConsolIndex, item);

			if (m_ReportInfo.CheckThreshold(item.m_dVal) == TRUE)
			{
				int nGroupNo = item.m_nGroupNo;
				__int64 nUserID = item.m_nUserID;
				CString strSortOn = item.m_strSortField;

				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserID(item.m_nUserID);

				CSQLRepositoryAccount RepoAccount;
				bool bExist = (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE);
				bool bAllow = TRUE;

				if (FALSE == bExist)
				{
					bAllow = m_DbReporter.GetUnknownReqd();
				}
				else
				{
					CString strWhere = "";
					CSQLBuilderBind BuilderBind{};
					RepoAccount.PrepareExportWhere(m_DbReporter, RowAccount.GetUserID(), strWhere, BuilderBind);

					CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ByParams{ &BuilderBind, strWhere });
					RecordSet.m_strSort = m_DbReporter.GetSQLOrderBy();
					
					CSQLRowAccountFull RowTest;
					if (RecordSet.StepSelectAll(RowTest) == TRUE)
					{
						bAllow = m_DbReporter.NonSQLFilter(RowAccount, TRUE);
					}
					else
					{
						bAllow = FALSE;
					}

				}

				if (TRUE == bAllow)
				{
					if ((m_DbReporter.GetSortOn() != nSORTONUSERID) && (m_DbReporter.GetTotalise() == TRUE))
					{
						if (nConsolIndex == 0)
						{
							strLastSortOn = strSortOn;							// only need for first entry
						}

						if (strLastSortOn != strSortOn)
						{
							SaveSortOnTotal(strLastSortOn);
							strLastSortOn = strSortOn;
							m_DbReporter.ClearTotals(nREPORT_SORTON);			// clear nSortOn totals
							ClearTotals(nREPORT_SORTON);
						}
					}

					if (m_DbReporter.GetSortByGroup() == TRUE)
					{
						if (nConsolIndex == 0)
						{
							nLastGroupNo = nGroupNo;							// only need for first entry
						}

						if (nLastGroupNo != nGroupNo)
						{
							SaveGroupTotal(nLastGroupNo);
							nLastGroupNo = nGroupNo;
							m_DbReporter.ClearTotals(nREPORT_GROUP);			// clear nGROUP totals //04/10/2016 - v1.13x
							ClearTotals(nREPORT_GROUP);
						}
					}

					SaveReportLine(RowAccount, bExist, item.m_nQty, item.m_dVal);
				}
			}
		}

		if ((m_DbReporter.GetSortOn() != nSORTONUSERID) && (m_DbReporter.GetTotalise() == TRUE))
		{
			SaveSortOnTotal(strLastSortOn);
		}

		if (m_DbReporter.GetSortByGroup() == TRUE)
		{
			SaveGroupTotal(nLastGroupNo);			// save last group if any
		}

		SaveReportTotal();
	}

	return (m_nReportCount[nREPORT_TOTAL] == 0) ? nREPORT_NOMATCH : nREPORT_NOERROR;
}

/*****************************************************************************/

bool CReportAccumulator::SaveReportLine(CSQLRowAccountFull& RowAccount, bool bExists, int nQty, double dValue, int nPos)
{
	// -1 to adjust consol array count, 1st entry forced due to non zero skip
	if (TRUE == m_ReportInfo.GetNoZeroSkipFlag())
	{
		--nQty;					
	}

	m_DbReporter.ResetSpecialFields();

	switch (m_nReportType)
	{
	case nACCUMULATE_RANKPOINTS:
	case nACCUMULATE_RANKSPEND:
		m_DbReporter.AddSpecialField(nPos);
		break;
	}

	switch (m_nReportType)
	{
	case nACCUMULATE_LOWPOINTS:
	case nACCUMULATE_RANKPOINTS:
		m_DbReporter.AddSpecialField(nQty);
		m_DbReporter.AddSpecialField(dValue, 0);
		break;

	case nACCUMULATE_LOWSPEND:
	case nACCUMULATE_RANKSPEND:
		m_DbReporter.AddSpecialField(nQty);
		m_DbReporter.AddSpecialField(dValue, System.GetDPValue());
		break;
	}

	CCSV csv('\t');

	m_DbReporter.AppendDbFieldsToCSVLine(RowAccount, &csv, bExists);

	++m_nReportCount[nREPORT_SORTON];
	++m_nReportCount[nREPORT_GROUP];
	++m_nReportCount[nREPORT_TOTAL];

	m_nReportQty[nREPORT_SORTON] += nQty;
	m_nReportQty[nREPORT_GROUP] += nQty;
	m_dReportValue[nREPORT_GROUP] += dValue;

	m_nReportQty[nREPORT_SORTON] += nQty;
	m_nReportQty[nREPORT_TOTAL] += nQty;
	m_dReportValue[nREPORT_TOTAL] += dValue;

	return m_Report.WriteLine(csv);
}

/**********************************************************************/

void CReportAccumulator::SaveSortOnTotal(const char* szSortOnText)
{
	if (m_nReportCount[nREPORT_SORTON] > 0)
	{
		CString strSortOnText = szSortOnText;
		strSortOnText.TrimLeft(' ');
		strSortOnText.TrimRight(' ');
		SaveTotals(nREPORT_SORTON, strSortOnText);
	}
}

/**********************************************************************/

void CReportAccumulator::SaveGroupTotal(int nGroupNo)
{
	if (m_nReportCount[nREPORT_GROUP] > 0)
	{
		CString strGroupName = "";

		CSQLRowGroupFull RowGroup;
		RowGroup.SetGroupNo(nGroupNo);

		CSQLRepositoryGroup RepoGroup;
		if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			strGroupName.Format("Group %2.2d", nGroupNo);
		}
		else
		{
			strGroupName = RowGroup.GetGroupName();
		}
		SaveTotals(nREPORT_GROUP, strGroupName);
	}
}

/**********************************************************************/

void CReportAccumulator::SaveReportTotal(bool bRankReport)
{
	SaveTotals(nREPORT_TOTAL, "Report", bRankReport);
}

/**********************************************************************/

void CReportAccumulator::SaveTotals(int nIndex, const char* szText, bool bRankReport)
{
	CCSV csv('\t');
	CCSV csvTabLine('\t');

	int nTextPos = 0;
	if (bRankReport == TRUE)
	{
		nTextPos = 1;
	}

	CString strTotalText = "";
	strTotalText.Format("%s Total", szText);

	m_DbReporter.ResetSpecialFields();

	switch (m_nReportType)
	{
	case nACCUMULATE_RANKPOINTS:
	case nACCUMULATE_RANKSPEND:
		m_DbReporter.AddSpecialField("");
		break;
	}

	switch (m_nReportType)
	{
	case nACCUMULATE_LOWPOINTS:
	case nACCUMULATE_RANKPOINTS:
		m_DbReporter.AddSpecialField(m_nReportQty[nIndex]);
		m_DbReporter.AddSpecialField(m_dReportValue[nIndex], 0);
		break;

	case nACCUMULATE_LOWSPEND:
	case nACCUMULATE_RANKSPEND:
		m_DbReporter.AddSpecialField(m_nReportQty[nIndex]);
		m_DbReporter.AddSpecialField(m_dReportValue[nIndex], System.GetDPValue());
		break;
	}

	m_DbReporter.CreateTotalLine(nTextPos, strTotalText, &csv, &csvTabLine, nIndex, m_nReportCount[nIndex]);

	m_Report.WriteLine(csvTabLine);
	m_Report.WriteLine(csv);
	m_Report.WriteLine("");
}

/**************************************************************************/

void CReportAccumulator::AddColumnText()
{
	switch (m_nReportType)
	{
	case nACCUMULATE_RANKPOINTS:
	case nACCUMULATE_RANKSPEND:
		m_DbReporter.AddSpecialColumn(DbReportFieldsSpecial::AccuRank);
		break;
	}

	m_DbReporter.AddOrderByColumnText();

	switch (m_nReportType)
	{
	case nACCUMULATE_LOWPOINTS:
	case nACCUMULATE_RANKPOINTS:
		m_DbReporter.AddSpecialColumn(DbReportFieldsSpecial::AccuTrans);
		m_DbReporter.AddSpecialColumn(DbReportFieldsSpecial::AccuPoints);
		break;

	case nACCUMULATE_LOWSPEND:
	case nACCUMULATE_RANKSPEND:
		m_DbReporter.AddSpecialColumn(DbReportFieldsSpecial::AccuTrans);
		m_DbReporter.AddSpecialColumn(DbReportFieldsSpecial::AccuSpend);
		break;
	}

	m_DbReporter.CreateReportHeader(&m_Report);
}

//*****************************************************************************
