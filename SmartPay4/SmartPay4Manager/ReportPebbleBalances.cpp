/******************************************************************************/
#include "..\SmartPay4ManagerBgnd\PebbleHelpers.h"
/******************************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/******************************************************************************/
#include "ReportPebbleBalances.h"
/******************************************************************************/

CReportPebbleBalances::CReportPebbleBalances ( CWnd* pParent )
{
	m_pParent = pParent;
	m_strTitle = "Pebble Balance Reconciliation";	
	m_strCommentPadding = "          ";
	Reset();
}

/******************************************************************************/

void CReportPebbleBalances::Reset()
{
	m_dDiscrepancyPlus = 0.0;
	m_dDiscrepancyMinus = 0.0;
	m_dDiscrepancySettled = 0.0;
	m_dPebbleSales = 0.0;
}

/******************************************************************************/

bool CReportPebbleBalances::Show()
{
	int nErrorNo = 0;

	CString strReportLabel = "R228";
	CString strReportFile = Filenames.GetReportFilename(strReportLabel);
	CString strParamsFile = Filenames.GetReportParamsFilename(strReportLabel);

	CRepmanHandler repman(m_strTitle, m_pParent);

	if ((nErrorNo = Create(strReportFile)) == nREPORT_NOERROR)
	{
		nErrorNo = repman.DisplayReport(strReportFile, strParamsFile, "1");
	}

	return repman.DisplayError(nErrorNo);
}

//******************************************************************************

int CReportPebbleBalances::Create ( CString strReportFile )
{
	if (m_Report.Open(strReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CWorkingDlg dlgWorking("Creating Pebble Balance Report", TRUE);
	dlgWorking.Create();

	m_Report.SetStyle1 ( m_strTitle );
	AddColumnText();
		
	Reset();
	bool bCancel = FALSE;
	bool bGotData = FALSE;
	int nChecked = 0;

	CSQLRepositoryPebbleConfig PebbleConfig;
	PebbleConfig.LoadRow(NULL);

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NoParams{}, FALSE);
	CSQLRowAccountFull RowAccount;

	//NOTE : THIS IS ACTUALLY BEING USED AS AN UNSORTED BUFFER
	CReportConsolidationArray<CSortedIntAndStringItem> arrayBuffer;

	while( RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (dlgWorking.SetCaption2RecordsChecked(++nChecked, TRUE) == TRUE)
		{
			bCancel = TRUE;
			break;
		}
	
		int nErrorFlag = 0;

		CString strMemberID = RowAccount.GetMemberID();
		if (strMemberID != "")
		{
			bGotData = TRUE;

			double dSmartPayBalance = RowAccount.GetPurse1Balance();

			CCSV csvLine('\t');
			csvLine.Add(RowAccount.GetUserIDString());
			csvLine.Add(RowAccount.GetAccountDisplayName());
			csvLine.Add(dSmartPayBalance, 2);

			CArray<CPebblePurseBuffer, CPebblePurseBuffer> arrayPurses;
			CString strError = pPebbleHelpers->GetPurses(PebbleConfig, strMemberID, arrayPurses);

			if (strError != "")
			{
				AddNoDataMarker(csvLine);
				csvLine.Add("");
				csvLine.Add("");
				csvLine.Add("");
				csvLine.Add("");
				csvLine.Add( m_strCommentPadding + "Unable to check Pebble purses");
				nErrorFlag = 1;
			}
			else
			{
				bool bGotPebbleBalance = FALSE;
				bool bGotPebbleSales = FALSE;
				double dPebbleBalance = 0.0;
				double dPebbleSales = 0.0;

				for (int n = 0; n < arrayPurses.GetSize(); n++)
				{
					if (arrayPurses[n].m_strType == "cash")
					{
						bGotPebbleBalance = TRUE;
						dPebbleBalance = (atof(arrayPurses[n].m_strBalance)) / 100.0;
					}
					else if (arrayPurses[n].m_strType == "sales")
					{
						dPebbleSales = (atof(arrayPurses[n].m_strBalance)) / 100.0;
						bGotPebbleSales = (CompareDoubles(dPebbleSales, 0.0, 2) != 0);
					}
				}

				if (FALSE == bGotPebbleBalance)
				{
					AddNoDataMarker(csvLine);
					csvLine.Add("");
					csvLine.Add("");
					csvLine.Add("");
					csvLine.Add("");
					csvLine.Add(m_strCommentPadding + "No Pebble cash purse found");
					nErrorFlag = 1;
				}
				else
				{
					csvLine.Add(dPebbleBalance, 2);

					double dLineDiscrepancy = dPebbleBalance - dSmartPayBalance;	

					switch (CompareDoubles(dLineDiscrepancy, 0.0, 2))
					{
					case 1:
						csvLine.Add(dLineDiscrepancy, 2);
						csvLine.Add("");
						m_dDiscrepancyPlus += dLineDiscrepancy;
						break;

					case -1:
						csvLine.Add("");
						csvLine.Add(dLineDiscrepancy * (-1), 2);
						m_dDiscrepancyMinus -= dLineDiscrepancy;
						break;

					default:
						csvLine.Add("");
						csvLine.Add("");
						break;
					}

					if (TRUE == bGotPebbleSales)
					{
						csvLine.Add(dPebbleSales, 2);
						m_dPebbleSales += dPebbleSales;
					}
					else
					{
						m_dDiscrepancySettled += dLineDiscrepancy;
						csvLine.Add("");
					}

					if (TRUE == bGotPebbleSales)
					{
						csvLine.Add("--");
						nErrorFlag = 1;
					}
					else
					{
						if (CompareDoubles(dLineDiscrepancy, 0.0, 2) != 0)
						{
							csvLine.Add(dLineDiscrepancy, 2);
							nErrorFlag = 1;
						}
						else
						{
							csvLine.Add("");
						}
					}
				}
			}

			//NOTE : BUFFER LINE WITHOUT SORTING
			CSortedIntAndStringItem BufferItem;
			BufferItem.m_nItem = nErrorFlag;
			BufferItem.m_strItem = csvLine.GetLine();
			arrayBuffer.DirectAdd(BufferItem);
		}
	}

	dlgWorking.DestroyWindow();

	if (FALSE == bCancel)
	{
		bool bGotLine = FALSE;
		for (int n = 0; n < arrayBuffer.GetSize(); n++)
		{
			CSortedIntAndStringItem item;
			arrayBuffer.GetAt(n, item);

			if (item.m_nItem != 0)
			{
				m_Report.WriteLine(item.m_strItem);
				bGotLine = TRUE;
			}
		}

		if (TRUE == bGotLine)
		{
			ShowTotals();
			m_Report.WriteLine("");
			m_Report.WriteLine("");
			m_Report.WriteLine("");
			m_Report.WriteLine("");
		}

		for (int n = 0; n < arrayBuffer.GetSize(); n++)
		{
			CSortedIntAndStringItem item;
			arrayBuffer.GetAt(n, item);
			m_Report.WriteLine(item.m_strItem);
		}

		ShowTotals();
	}

	m_Report.Close();
	
	if (TRUE == bCancel)
	{
		return nREPORT_CANCEL;
	}
	if (FALSE == bGotData)
	{
		return nREPORT_NOMATCH;
	}

	return nREPORT_NOERROR;
}

/*****************************************************************************/

void CReportPebbleBalances::ShowTotals()
{
	m_Report.WriteLine("\t\t\t\t<LI>\t<LI>\t<LI>\t<LI>");

	{
		CCSV csvTotal('\t');
		csvTotal.Add("");
		csvTotal.Add("");
		csvTotal.Add("");
		csvTotal.Add("");
		csvTotal.Add(m_dDiscrepancyPlus, 2);
		csvTotal.Add(m_dDiscrepancyMinus, 2);
		csvTotal.Add(m_dPebbleSales, 2);
		csvTotal.Add(m_dDiscrepancySettled, 2);
		m_Report.WriteLine(csvTotal.GetLine());
	}

	m_Report.WriteLine("");
	m_Report.WriteLine("\t\t\t\t\t<LI>");

	{
		CCSV csvTotal('\t');
		csvTotal.Add("");
		csvTotal.Add("");
		csvTotal.Add("");
		csvTotal.Add("");
		csvTotal.Add("");
		csvTotal.Add(m_dDiscrepancyPlus - m_dDiscrepancyMinus, 2);
		m_Report.WriteLine(csvTotal.GetLine());
	}
}

/*****************************************************************************/

void CReportPebbleBalances::AddNoDataMarker(CCSV& Csv)
{
	Csv.Add(szNODATA);
}

//**********************************************************************

void CReportPebbleBalances::AddColumnText()
{
	m_Report.AddColumn("UserID", TA_LEFT, 300);
	m_Report.AddColumn("Name", TA_LEFT, 600);
	m_Report.AddColumn("SmartPay Purse1", TA_RIGHT, 360);
	m_Report.AddColumn("Pebble Cash Purse", TA_RIGHT, 360);
	m_Report.AddColumn("Pebble Var(+)", TA_RIGHT, 360);
	m_Report.AddColumn("Pebble Var(-)", TA_RIGHT, 360);
	m_Report.AddColumn("Pebble Sale Purse", TA_RIGHT, 400);
	m_Report.AddColumn("Pebble Var(settled)", TA_RIGHT, 400);
	m_Report.AddColumn("", TA_LEFT, 700);
}

//*****************************************************************************
