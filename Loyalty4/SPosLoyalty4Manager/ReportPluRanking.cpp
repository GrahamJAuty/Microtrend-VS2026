/******************************************************************************/
#include <math.h>
/******************************************************************************/
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
/******************************************************************************/
#include "AccumulatorPropertySheet.h"
#include "DbExportEditor.h"
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
#include "ReportInfoAccumulator.h"
#include "TLogFile.h"
#include "TLogRecord.h"
/******************************************************************************/
#include "ReportPluRanking.h"
/******************************************************************************/
#define	nRANKBY_QTY		0
#define nRANKBY_VALUE	1
#define nRANKBY_POINTS	2
/******************************************************************************/

CReportPluRanking::CReportPluRanking(CWnd* pParent) : m_ReportInfo("R106")
{
	m_pParent = pParent;
	m_strReportLabel = "R106";
	m_bHaveFractionalQty = FALSE;

	for (int n = 0; n < 3; n++)
	{
		m_dTotalQty[n] = 0.0;
		m_dTotalValue[n] = 0.0;
		m_dTotalPoints[n] = 0.0;
	}
}

/******************************************************************************/

void CReportPluRanking::ResetTotals(int nLevel)
{
	if ((nLevel >= 0) && (nLevel < 3))
	{
		m_dTotalQty[nLevel] = 0.0;
		m_dTotalValue[nLevel] = 0.0;
		m_dTotalPoints[nLevel] = 0.0;
	}
}

//********************************************************************

void CReportPluRanking::AddToTotals(double dQty, double dValue, double dPoints, bool bDiscount)
{
	for (int nLevel = 0; nLevel < 3; nLevel++)
	{
		m_dTotalQty[nLevel] += dQty;
		m_dTotalPoints[nLevel] += dPoints;

		if ( (TRUE == bDiscount) && ( 2 == nLevel ))
		{
			m_dTotalValue[nLevel] -= dValue;
		}
		else
		{
			m_dTotalValue[nLevel] += dValue;
		}
	}
}

//********************************************************************

void CReportPluRanking::Show()
{
	bool bCancelButton = FALSE;
	while (FALSE == bCancelButton)
	{
		m_arrayPluNumbers.RemoveAll();
		m_arrayPluSales.RemoveAll();
		m_arrayPluDescription.RemoveAll();

		m_ReportInfo.Reset();

		{
			CAccumulatorPropertySheet psReport(m_strReportLabel, TRUE, m_pParent);
			psReport.SetReportInfo(&m_ReportInfo);

			if (psReport.DoModal() == IDCANCEL)
			{
				break;
			}
		}

		CString strTitle = m_ReportInfo.GetReportTitle();

		//CREATE ARRAY OF PLU NUMBERS
		{
			CFileStringArray arrayPLUs;

			CString strMsg = "";
			CString strPluList = Filenames.GetPluDatabaseListFilename(m_ReportInfo.GetDbNo());
			if (arrayPLUs.Read(strPluList) == FALSE)
			{
				if (SolutionGlobalFunctions::FileExists(strPluList) == TRUE)
				{
					strMsg = "Cannot open the product selection list file";
				}
			}
			else if (arrayPLUs.GetSize() == 0)
			{
				strMsg = "No products have been selected for reporting";
			}

			if (strMsg != "")
			{
				Prompter.Error(strMsg, strTitle);
				continue;
			}

			for (int n = 0; n < arrayPLUs.GetSize(); n++)
			{
				CSortedInt64Item item;
				item.m_nItem = _atoi64(arrayPLUs.GetAt(n));
				m_arrayPluNumbers.Consolidate(item);
			}
		}

		CDbExportEditor editor(FALSE, NULL);
		if (editor.EditLabel(m_strReportLabel, strTitle) == FALSE)
		{
			continue;
		}

		CDbReportHandler dbReporter{};
		if (dbReporter.Read(m_strReportLabel) == FALSE)
		{
			continue;
		}

		CString strReportFile = dbReporter.GetReportFilename();

		if (m_Report.Open(strReportFile) == FALSE)
		{
			CRepmanHandler repman(strTitle, m_pParent);
			repman.DisplayError(nREPORT_CREATEFAIL);
			continue;
		}

		CWorkingDlg progress("Creating UserID List");
		progress.Create();

		//CREATE ARRAY OF USER IDS
		CStringArray arrayUserIDs;
		{
			CString strWhere = "";
			CSQLBuilderBind BuilderBind{};
			CSQLRepositoryAccount repoAccount;
			repoAccount.PrepareExportWhere(dbReporter, -1, strWhere, BuilderBind);

			CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ExportByParams{ &BuilderBind, strWhere });
			RecordSet.m_strSort = dbReporter.GetSQLOrderBy();
			
			CSQLRowAccountFull RowAccount;
			while (RecordSet.StepSelectAll(RowAccount) == TRUE)
			{
				if (progress.IncrementRecordsChecked(10,TRUE) == TRUE)
				{
					bCancelButton = TRUE;
					break;
				}

				if (dbReporter.NonSQLFilter(RowAccount) == TRUE)
				{
					arrayUserIDs.Add(RowAccount.GetUserIDString());
				}
			}
		}

		if (TRUE == bCancelButton)
		{
			continue;
		}

		m_Report.SetStyle1(strTitle);
		AddColumnText(dbReporter);

		bool bNewPageReqd = FALSE;
		m_bHaveFractionalQty = FALSE;

		LONGLONG LastProgressUpdate = 0;

		ResetTotals(0);
		ResetTotals(1);
		ResetTotals(2);

		progress.SetCaption1("Consolidating Sales by Account");

		for (int x = 0; x < arrayUserIDs.GetSize(); x++)
		{
			LONGLONG ProgressTime = GetTickCount64();

			if (ProgressTime > LastProgressUpdate + 100)
			{
				if (progress.SetCaption2RecordsOfTotal(x, arrayUserIDs.GetSize()) == TRUE)
				{
					bCancelButton = TRUE;
					break;
				}

				LastProgressUpdate = ProgressTime;
			}

			CString strUserID = arrayUserIDs.GetAt(x);
			Consolidate(strUserID, progress, bCancelButton);
		}

		int nCurrentBlockStart = 0;
		__int64 nCurrentPluNo = -1;
		int nPluCount = 0;

		progress.SetCaption1("Ranking sales by PLU item");

		for (int n = 0; n < m_arrayPluSales.GetSize(); n++)
		{
			CPluRankingReportItem item;
			m_arrayPluSales.GetAt(n, item);

			if (item.m_nPluNo != nCurrentPluNo)
			{
				progress.SetCaption2RecordsOfUpToTotal(++nPluCount, m_arrayPluNumbers.GetSize(), FALSE);

				if (nCurrentPluNo != -1)
				{
					OutputRanking(nCurrentBlockStart, n - 1, dbReporter, (0 == nCurrentPluNo));

					if (TRUE == m_ReportInfo.GetNewPageFlag())
					{
						m_Report.WriteLine("<NEWPAGE>");
					}
					else
					{
						m_Report.WriteLine("");
						m_Report.WriteLine("");
					}
				}

				nCurrentPluNo = item.m_nPluNo;
				nCurrentBlockStart = n;

				CString strEcrText = "";
				if (0 != nCurrentPluNo)				// ".."
				{
					CSortedInt64AndStringItem itemPluDesc;
					itemPluDesc.m_nItem = nCurrentPluNo;
					itemPluDesc.m_strItem = "";

					int nPos = 0;
					m_arrayPluDescription.Find(itemPluDesc, nPos);

					while (nPos < m_arrayPluDescription.GetSize())
					{
						m_arrayPluDescription.GetAt(nPos, itemPluDesc);
						if (itemPluDesc.m_nItem == nCurrentPluNo)
						{
							if (strEcrText != "")
							{
								strEcrText += ", ";
							}

							strEcrText += itemPluDesc.m_strItem;
						}
						else
						{
							break;
						}

						nPos++;
					}

					if (strEcrText == "")
					{
						strEcrText = "Unknown";
					}
				}
				else
				{
					strEcrText = szDISCOUNT_ECRTEXT;			// "Discount"
				}

				AddPluDetails(item.m_nPluNo, strEcrText);
			}
		}

		if (m_arrayPluSales.GetSize() > 0)
		{
			OutputRanking(nCurrentBlockStart, m_arrayPluSales.GetSize() - 1, dbReporter, (0 == nCurrentPluNo));
		}

		m_Report.WriteLine("");
		ShowTotals(2);

		m_Report.Close();
		progress.DestroyWindow();

		if (FALSE == bCancelButton)
		{
			CRepmanHandler repman(strTitle, m_pParent);
			repman.SetupSaveAs(dbReporter.GetSaveAsType(), dbReporter.GetSaveAsFile());
			int nErrorNo = repman.DisplayReport(strReportFile, dbReporter.GetParamsFilename(), dbReporter.GetReportKey());
			repman.DisplayError(nErrorNo);
		}
	}
}

//********************************************************************

void CReportPluRanking::Consolidate(CString strUserID, CWorkingDlg& dlgWorking, bool& bCheckCancel)
{
	CTLogFile tlogFile(strUserID);
	if (tlogFile.Open() == TRUE)
	{
		tlogFile.JumpIn(m_ReportInfo.GetDateFrom().GetDate());
		CTLogRecord tlogRecord;

		while (tlogFile.ReadLine(&tlogRecord) == TRUE)
		{
			bCheckCancel = dlgWorking.CheckCancelButton();
			if (TRUE == bCheckCancel)
			{
				break;
			}

			if (tlogRecord.m_date < m_ReportInfo.GetDateFrom())
			{
				continue;
			}

			if (m_ReportInfo.GetTimeRangeFlag() == TRUE)
			{
				if (tlogRecord.m_time < m_ReportInfo.GetTimeFrom() || tlogRecord.m_time > m_ReportInfo.GetTimeTo())
				{
					continue;
				}
			}

			if (tlogRecord.m_date > m_ReportInfo.GetDateTo())
			{
				break;
			}

			if (m_ReportInfo.CheckTerminalFilter(tlogRecord.GetDbNo(),tlogRecord.GetFolderSet(),tlogRecord.GetTerminalNo()) == FALSE )
			{
				continue;
			}

			for (int nIndex = 0; nIndex < tlogRecord.GetItemCounter(); nIndex++)
			{
				tlogRecord.ExtractItemData(nIndex);
				int nItemType = tlogRecord.GetItemType();

				CString strPluToFind = "";

				switch (nItemType)
				{
				case nTLOGTYPE_PLU:
				case nTLOGTYPE_DISC_ITEM:
					strPluToFind = tlogRecord.GetItemNo();
					break;

				case nTLOGTYPE_DISC_SUBT:
					strPluToFind = szDISCOUNT_PLUNO;
					break;
				}

				if (strPluToFind == "")
				{
					continue;
				}

				bool bIncludeItem = FALSE;
				__int64 nPluNo = _atoi64(strPluToFind);

				{
					int nPos = 0;
					CSortedInt64Item itemPlu;
					itemPlu.m_nItem = nPluNo;
					if (m_arrayPluNumbers.Find(itemPlu, nPos) == TRUE)
					{
						bIncludeItem = TRUE;
					}
					else if (m_ReportInfo.GetIncludeUnknownItemsFlag() == TRUE)
					{
						bool bKnown = FALSE;

						//CHECK FOR EXACT MATCH WITH LITERAL PLU NUMBER (NO MODIFIER)
						CSQLRowPluInfo RowPluInfo;
						RowPluInfo.SetDbNo(m_ReportInfo.GetDbNo());
						RowPluInfo.SetPluNo(itemPlu.m_nItem);
						
						CSQLRepositoryPluInfo repoInfo;
						if (repoInfo.SelectRow(RowPluInfo, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
						{
							if (RowPluInfo.GetModType() == 3)
							{
								bKnown = TRUE;
							}
						}

						if (FALSE == bKnown)
						{
							//CHECK FOR BASE PLU WITH MODIFIER DIGIT
							RowPluInfo.SetPluNo(itemPlu.m_nItem * 10);
							if (repoInfo.SelectRow(RowPluInfo, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
							{
								if (RowPluInfo.GetModType() == 1)
								{
									bKnown = TRUE;
								}
							}
						}

						if (FALSE == bKnown)
						{
							bIncludeItem = TRUE;
						}
					}
				}

				if (TRUE == bIncludeItem)
				{
					CPluRankingReportItem PluSale;

					if (strPluToFind == szDISCOUNT_PLUNO)
					{
						PluSale.m_nPluNo = 0;
					}
					else
					{
						PluSale.m_nPluNo = nPluNo;
					}

					PluSale.m_strUserID = strUserID;

					if (nTLOGTYPE_DISC_ITEM == nItemType)
					{
						PluSale.m_dQty = 0.0;
					}
					else
					{
						PluSale.m_dQty = tlogRecord.GetItemLineQty();
					}

					PluSale.m_dValue = tlogRecord.GetItemLineVal();
					PluSale.m_dPoints = (double)tlogRecord.GetItemPoints();
					m_arrayPluSales.Consolidate(PluSale);

					if (nTLOGTYPE_DISC_ITEM == nItemType)
					{
						PluSale.m_dQty = 0.0;
					}

					if (FALSE == m_bHaveFractionalQty)
					{
						if (PluSale.m_dQty > 0.0)
						{
							double n = 0.0;
							if (modf(PluSale.m_dQty, &n) != 0.0)
							{
								m_bHaveFractionalQty = TRUE;
							}
						}
					}

					if (nTLOGTYPE_DISC_ITEM != nItemType)
					{
						if (PluSale.m_nPluNo != 0)
						{
							CString strText = tlogRecord.GetItemText();
							SolutionGlobalFunctions::TrimSpacesFromString(strText, FALSE);

							if (strText != "")
							{
								CSortedInt64AndStringItem itemPluDesc;
								itemPluDesc.m_nItem = PluSale.m_nPluNo;
								itemPluDesc.m_strItem = strText;
								m_arrayPluDescription.Consolidate(itemPluDesc);
							}
						}
					}
				}
			}
		}
		tlogFile.Close();
	}
}

//********************************************************************

void CReportPluRanking::OutputRanking(int nStartIdx, int nEndIdx, CDbReportHandler& ReportHandler, bool bDiscount)
{
	CReportConsolidationArray<CPluRankingUserSort> arrayRanking;

	for (int n = nStartIdx; n <= nEndIdx; n++)
	{
		CPluRankingReportItem itemSales;
		m_arrayPluSales.GetAt(n, itemSales);

		double dRankValue = 0.0;

		switch (m_ReportInfo.GetRankByType())
		{
		case nRANKBY_QTY:
			dRankValue = itemSales.m_dQty;
			break;

		case nRANKBY_VALUE:
			dRankValue = itemSales.m_dValue;

			if (TRUE == bDiscount)
			{
				dRankValue *= -1;
			}

			break;

		case nRANKBY_POINTS:
			dRankValue = itemSales.m_dPoints;
			break;
		}

		CString strRankLabel = "";
		strRankLabel.Format("%.0f", dRankValue * 100.0);
		::AddLeading(strRankLabel, 20, '0');

		CPluRankingUserSort itemRanking;
		itemRanking.m_strInfoLabel = "";
		itemRanking.m_strRankLabel = strRankLabel;
		itemRanking.m_nIndex = n;
		arrayRanking.Consolidate(itemRanking);
	}

	int nRank = 0;
	int nCount = 0;
	double dLastVal1 = -1.0;

	ResetTotals(0);
	ResetTotals(1);
	CString strLastRankLabel = "";
	CString strLastInfoLabel = "";
	bool bGotSubtotal = FALSE;

	for (int n = 0; n < arrayRanking.GetSize(); n++)
	{
		CPluRankingUserSort itemRanking;
		arrayRanking.GetAt(n, itemRanking);

		if (strLastInfoLabel != itemRanking.m_strInfoLabel)
		{
			if (n != 0)
			{
				ShowTotals(0);
				ResetTotals(0);
				m_Report.WriteLine("");
				bGotSubtotal = TRUE;
			}

			nRank = 1;
			nCount = 0;
			strLastRankLabel = itemRanking.m_strRankLabel;
			strLastInfoLabel = itemRanking.m_strInfoLabel;
		}
		else if (strLastRankLabel == itemRanking.m_strRankLabel)
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

			strLastRankLabel = itemRanking.m_strRankLabel;
			nCount = 0;
		}

		CPluRankingReportItem itemSales;
		m_arrayPluSales.GetAt(itemRanking.m_nIndex, itemSales);

		double dValue = itemSales.m_dValue;
		::RoundDoubleValue(dValue);
		
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserIDString(itemSales.m_strUserID);

		CSQLRepositoryAccount repoAccount;
		repoAccount.SelectRow(RowAccount, NULL);
		SaveReportLine(RowAccount, nRank, itemSales.m_dQty, dValue, itemSales.m_dPoints, bDiscount, ReportHandler);
	}

	ShowTotals(1);

	if (TRUE == bGotSubtotal)
	{
		m_Report.WriteLine("");
		ShowTotals(1);
		ResetTotals(1);
	}
}

//********************************************************************

void CReportPluRanking::SaveReportLine(CSQLRowAccountFull& RowAccount, int nRank, double dQty, double dValue, double dPoints, bool bDiscount, CDbReportHandler& ReportHandler)
{
	double dLineValue = dValue;
	if (TRUE == bDiscount)
	{
		dLineValue *= -1;
	}

	CCSV csv('\t');
	csv.Add(nRank);
	csv.Add(System.FormatCardNo(RowAccount.GetUserIDString()));
	csv.Add(RowAccount.GetSurname());
	csv.Add(RowAccount.GetGroupNo());

	int nDps = (m_bHaveFractionalQty == TRUE) ? 2 : 0;
	CString strQty = "";
	strQty.Format("%.*f", nDps, dQty);

	switch (m_ReportInfo.GetRankByType())
	{
	case nRANKBY_QTY:
		csv.Add(strQty);
		csv.Add(dLineValue, System.GetDPValue());
		csv.Add(dPoints, 0);
		break;

	case nRANKBY_VALUE:
		csv.Add(dLineValue, System.GetDPValue());
		csv.Add(strQty);
		csv.Add(dPoints, 0);
		break;

	case nRANKBY_POINTS:
		csv.Add(dPoints, 0);
		csv.Add(strQty);
		csv.Add(dLineValue, System.GetDPValue());
		break;
	}

	ReportHandler.AppendDbFieldsToCSVLine(RowAccount, &csv, TRUE);

	AddToTotals(dQty, dLineValue, dPoints, bDiscount);

	m_Report.WriteLine(csv);
}

//*******************************************************************

void CReportPluRanking::AddColumnText ( CDbReportHandler& ReportHandler )
{
	m_Report.AddColumn ( "Rank", TA_LEFT, 150 );
	m_Report.AddColumn ( System.GetCardNoText(), TA_LEFT, 162 );
	m_Report.AddColumn ( "Name", TA_LEFT, 357 );
	m_Report.AddColumn ( "Grp", TA_RIGHT, 93 );

	switch ( m_ReportInfo.GetRankByType() )
	{
	case nRANKBY_QTY:		
		m_Report.AddColumn ( "Quantity", TA_RIGHT, 200);				
		m_Report.AddColumn ( "Value", TA_RIGHT, 249 );					
		m_Report.AddColumn ( "Points", TA_RIGHT, 249 );					
		break;

	case nRANKBY_VALUE:		
		m_Report.AddColumn ( "Value", TA_RIGHT, 249 );
		m_Report.AddColumn ( "Quantity", TA_RIGHT, 200);		
		m_Report.AddColumn ( "Points", TA_RIGHT, 249 );
		break;

	case nRANKBY_POINTS:	
		m_Report.AddColumn ( "Points", TA_RIGHT, 249 );					
		m_Report.AddColumn ( "Quantity", TA_RIGHT, 200);				
		m_Report.AddColumn ( "Value", TA_RIGHT, 249 );				
		break;
	}

	ReportHandler.CreateReportHeader ( &m_Report );
}

//********************************************************************

void CReportPluRanking::ShowTotals(int nLevel)
{
	if ((nLevel < 0) || (nLevel > 2))
	{
		return;
	}

	CString strQty = "";
	int nDps = m_bHaveFractionalQty ? 2 : 0;
	strQty.Format("%.*f", nDps, m_dTotalQty[nLevel]);

	CCSV csv('\t');
	csv.Add("");								// Rank
	csv.Add("");								// cardNo
	csv.Add(2 == nLevel ? "Report Totals" : "");// Username
	csv.Add("");								// Group;

	switch (m_ReportInfo.GetRankByType())
	{
	case nRANKBY_QTY:
		csv.Add(strQty);
		csv.Add(m_dTotalValue[nLevel], System.GetDPValue());
		csv.Add(m_dTotalPoints[nLevel], 0);
		break;

	case nRANKBY_VALUE:
		csv.Add(m_dTotalValue[nLevel], System.GetDPValue());
		csv.Add(strQty);
		csv.Add(m_dTotalPoints[nLevel], 0);
		break;

	case nRANKBY_POINTS:
		csv.Add(m_dTotalPoints[nLevel], 0);
		csv.Add(strQty);
		csv.Add(m_dTotalValue[nLevel], System.GetDPValue());
		break;
	}

	CString strLine = "\t\t\t\t<LI>\t<LI>\t<LI>";
	
	m_Report.WriteLine(strLine);
	m_Report.WriteLine(csv);
}

//****************************************************************

void CReportPluRanking::AddPluDetails(__int64 nPluNo, CString strEcrText)
{
	CString strHeader = "";
	
	if (0 == nPluNo)
	{
		strHeader = "<..>Discount";
	}
	else
	{
		strHeader.Format("<..>Plu %I64d - %s", 
			nPluNo, 
			(const char*) strEcrText);
	}
	
	m_Report.WriteLine ( strHeader );
	m_Report.WriteLine ( "<LI>" );
}

//*******************************************************************

