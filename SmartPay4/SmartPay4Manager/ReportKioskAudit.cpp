/**********************************************************************/
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/**********************************************************************/
#include "AuditReportDlg.h"
/**********************************************************************/
#include "ReportKioskAudit.h"
/**********************************************************************/
#define nTOTAL_KIOSK	0
#define nTOTAL_REPORT	1
/**********************************************************************/

CReportKioskAudit::CReportKioskAudit ( const char* szLabel, CWnd* pParent )
	: m_dlgReport( szLabel )
	, m_Repman("Kiosk Audit Report", &m_dlgReport)
{
	m_pParent = pParent;
}

//*******************************************************************

void CReportKioskAudit::ResetTotals ( int nIndex )
{
	m_dTotalAddedPurse1[nIndex] = 0.0;				// Purse1 column
	m_dTotalAddedPurse3[nIndex] = 0.0;				// Purse3 column
}

//*******************************************************************

void CReportKioskAudit::Show()
{
	int nErrorNo;
	if ((nErrorNo = Create()) != nREPORT_NOERROR)
	{
		m_Repman.DisplayError(nErrorNo);
	}
}

//*******************************************************************

int CReportKioskAudit::Create()
{
	int nErrorNo = nREPORT_NOERROR;
	CSmartPayAuditFile auditFile ( Filenames.GetAuditFilename() );
	if ((nErrorNo = auditFile.ValidateFile()) != nREPORT_NOERROR)
	{
		return nErrorNo;
	}

	while ( m_dlgReport.DoModal() == IDOK )
	{
		if (m_Report.Open(m_dlgReport.GetReportFilename()) == FALSE)
		{
			return nREPORT_CREATEFAIL;
		}
		else
		{
			if ( auditFile.Open() == FALSE )						// reopen source file
			{
				m_Report.Close();									// close report file
				return nREPORT_OPENERROR;
			}
		}

		CWorkingDlg dlgWorking ( szMSG_CREATINGREPORT, TRUE );
		dlgWorking.Create();

		CString strTitle = m_dlgReport.GetReportTitle();
		strTitle += m_dlgReport.GetDateString(); 

		m_Report.SetStyle1 ( strTitle );
		AddColumnText( );
		
		ResetTotals ( nTOTAL_KIOSK );
		ResetTotals ( nTOTAL_REPORT	);

		int nErrorNo = ReportByKiosk ( &auditFile, &dlgWorking );

		dlgWorking.DestroyWindow();

		m_Report.Close();
		auditFile.Close();										// close audit file open

		if (nErrorNo == nREPORT_NOERROR)
		{
			nErrorNo = m_Repman.DisplayReport(m_dlgReport.GetReportFilename(), m_dlgReport.GetParamsFilename(), m_dlgReport.GetReportKey(), &m_dlgReport);
		}

		m_Repman.DisplayError ( nErrorNo );
	}

	return nREPORT_NOERROR;
}

//*******************************************************************

int CReportKioskAudit::ReportByKiosk(CSmartPayAuditFile* pAudit, CWorkingDlg* pWorkingDlg)
{
	CRecordSorter sorter;
	CSQLAuditRecord atc;

	pAudit->JumpIn(m_dlgReport.m_dateFrom.GetDate());

	int nReply = nREPORT_NOERROR;
	while (TRUE)
	{
		if (pWorkingDlg->SetCaption2PercentDone(pAudit->GetPercentPosition() / 2, TRUE) == TRUE)
		{
			nReply = nREPORT_CANCEL;
			break;
		}

		int nStartFilePos = pAudit->GetFilePos();			// position in file to start from
		if (pAudit->ReadLine(&atc) == FALSE)
		{
			break;
		}

		if (m_dlgReport.IsValid(&atc) == TRUE)
		{
			CString strLabel = "";
			strLabel.Format("%6.6d", atc.GetTerminalNo());
			sorter.Add(strLabel, nStartFilePos);
		}
	}

	if (nReply != nREPORT_NOERROR)
	{
		return nReply;
	}

	if (sorter.GetSize() == 0)
	{
		return nREPORT_NOMATCH;
	}

	int nTNo = 0;
	int nLastTNo = 0;
	int nLineCount = 0;

	for (int nIndex = 0; nIndex < sorter.GetSize(); nIndex++)
	{
		if (pWorkingDlg->SetCaption2PercentDone(50 + ((nIndex * 50L) / sorter.GetSize()), TRUE) == TRUE)
		{
			nReply = nREPORT_CANCEL;
			break;
		}

		pAudit->SetFilePos(sorter.GetRecordNo(nIndex));
		if (pAudit->ReadLine(&atc) == TRUE)
		{
			nTNo = atc.GetTerminalNo();					// get Kiosk Terminal No.

			if (nIndex == 0)							// on first time round loop
			{
				nLastTNo = nTNo;
			}
			else
			{
				if (nTNo != nLastTNo)				// change of Kiosk TNo
				{
					if (nLineCount > 0)
					{
						if (m_dlgReport.m_bSummaryReport == TRUE)
						{
							SaveSummaryTotals(nLastTNo);
						}
						else
						{
							SaveTotals(nTOTAL_KIOSK, nLastTNo);
						}

						if (m_dlgReport.m_bNewPage == TRUE)
						{
							m_Report.WriteLine("<NEWPAGE>,1");
						}
					}

					nLineCount = 0;
					nLastTNo = nTNo;
					ResetTotals(nTOTAL_KIOSK);
				}
			}

			CSQLRepositoryAccount RepoAccount;
			bool bExists = (RepoAccount.RowExists(_atoi64(atc.GetUserID()), NULL).GetSQLResult() != 0);

			if (SaveLine(&atc, bExists) == TRUE)
			{
				++nLineCount;
			}
		}
	}

	if (nReply == nREPORT_NOERROR)
	{
		if (nLineCount > 0)
		{
			if (m_dlgReport.m_bSummaryReport == TRUE)
			{
				SaveSummaryTotals(nTNo);
			}
			else
			{
				SaveTotals(nTOTAL_KIOSK, nTNo);
			}
		}

		if (m_dlgReport.m_bSingleKiosk == FALSE)
		{
			if (m_dlgReport.m_bSummaryReport == TRUE)
			{
				SaveSummaryTotals();
			}
			else
			{
				if (m_dlgReport.m_bNewPage == TRUE)
				{
					m_Report.WriteLine("<NEWPAGE>,1");
				}

				SaveTotals(nTOTAL_REPORT);
			}
		}
	}

	return nReply;
}

/**********************************************************************/

void CReportKioskAudit::AddColumnText()
{
	if (m_dlgReport.m_bSummaryReport == TRUE)
	{
		m_Report.AddColumn("Kiosk", TA_LEFT, 300);
		m_Report.AddColumn("Purse1", TA_RIGHT, 220);
		if (System.GetEnablePurse3Flag() == TRUE)
		{
			m_Report.AddColumn("Purse3", TA_RIGHT, 210);
		}
	}
	else
	{
		m_Report.AddColumn("Audit Date", TA_LEFT, 190);
		m_Report.AddColumn("", TA_LEFT, 120);
		m_Report.AddColumn("UserID", TA_LEFT, 210);
		m_Report.AddColumn("Surname", TA_LEFT, 300);
		m_Report.AddColumn("Grp", TA_LEFT, 93);
		m_Report.AddColumn("Source", TA_LEFT, 230);
		m_Report.AddColumn("CCNo", TA_LEFT, 120);
		m_Report.AddColumn("Purse1", TA_RIGHT, 220);
		m_Report.AddColumn("P1.Bal", TA_RIGHT, 180);

		if (System.GetEnablePurse3Flag() == TRUE)
		{
			m_Report.AddColumn("Purse3", TA_RIGHT, 220);
			m_Report.AddColumn("P3.Bal", TA_RIGHT, 180);
		}
	}
}

//**********************************************************************

bool CReportKioskAudit::SaveLine(CSQLAuditRecord* pAtc, bool bCardExists)
{
	CCSV csv('\t');
	csv.Add(pAtc->GetDateFiled());
	csv.Add(pAtc->GetTimeFiled());

	CString strCardNo = System.FormatCardNo(pAtc->GetUserID());
	if (bCardExists == FALSE)	strCardNo += " *";
	csv.Add(strCardNo);

	csv.Add(pAtc->GetUserName());
	csv.Add(pAtc->GetGroupNo());
	csv.Add(CAuditReportTexts::GetReportSourceText(pAtc->GetSourceType(), pAtc->GetApplicationNo(), pAtc->GetTerminalNo(), pAtc->GetAuditLineExtraText()));
	csv.Add(pAtc->GetCCNoShort());

	double dValue = pAtc->GetTransactionPurse1Total();

	csv.Add(dValue, System.GetDPValue());			// purse1 transaction / cash taken
	csv.Add(pAtc->GetCurrentPurse1Total(), System.GetDPValue());

	m_dTotalAddedPurse1[nTOTAL_KIOSK] += dValue;
	m_dTotalAddedPurse1[nTOTAL_REPORT] += dValue;

	if (System.GetEnablePurse3Flag() == TRUE)
	{
		double dValue = pAtc->GetTransactionPurse3Total();

		csv.Add(dValue, System.GetDPValue());			// purse3transaction / cash taken
		csv.Add(pAtc->GetCurrentPurse3Total(), System.GetDPValue());

		m_dTotalAddedPurse3[nTOTAL_KIOSK] += dValue;
		m_dTotalAddedPurse3[nTOTAL_REPORT] += dValue;
	}

	if (m_dlgReport.m_bSummaryReport == FALSE)
	{
		return m_Report.WriteLine(csv);
	}

	return TRUE;
}

//**********************************************************************

void CReportKioskAudit::SaveTotals ( int nIndex, int nKioskNo )
{
	CCSV csvAdded ( '\t' );
	CString strTabLine = "\t\t\t\t\t\t";

	CString strText = "<..>Report Total";
	if (nIndex == nTOTAL_KIOSK)
	{
		strText.Format("Kiosk %d Total", nKioskNo);
	}

	csvAdded.Add ( "" );							// date
	csvAdded.Add ( "" );							// time
	csvAdded.Add ( "" );							// card no
	csvAdded.Add ( strText );						// CardName
	csvAdded.Add ( "" );							// GroupNo
	csvAdded.Add ( "" );							// Source
	csvAdded.Add ( "" );							// CCNo

	strTabLine += "\t<LI>\t";
	csvAdded.Add ( ::FormatDoubleValue(m_dTotalAddedPurse1[nIndex]) );
	csvAdded.Add ( "" );							// bal p1

	if ( System.GetEnablePurse3Flag() == TRUE )
	{
		strTabLine += "\t<LI>\t";
		csvAdded.Add ( ::FormatDoubleValue(m_dTotalAddedPurse3[nIndex]) );
		csvAdded.Add ( "" );						// bal p3
	}

	m_Report.WriteLine ( strTabLine );
	m_Report.WriteLine ( csvAdded );

	if (nIndex == nTOTAL_KIOSK)
	{
		m_Report.WriteLine("");
	}
}

//**********************************************************************

void CReportKioskAudit::SaveSummaryTotals ( int nKioskNo )
{
	CCSV csvAdded ( '\t' );

	if (nKioskNo == 0)							// report total
	{
		m_Report.WriteLine("\t<LI>");
		csvAdded.Add("<..>Report Total");
		csvAdded.Add(::FormatDoubleValue(m_dTotalAddedPurse1[nTOTAL_REPORT]));

		if (System.GetEnablePurse3Flag() == TRUE)
		{
			csvAdded.Add(::FormatDoubleValue(m_dTotalAddedPurse3[nTOTAL_REPORT]));
		}
	}
	else											// Kiosk total
	{
		CString strText;
		strText.Format ( "Kiosk %d Total", nKioskNo );
		csvAdded.Add ( strText );
		csvAdded.Add ( ::FormatDoubleValue(m_dTotalAddedPurse1[nTOTAL_KIOSK]) );

		if (System.GetEnablePurse3Flag() == TRUE)
		{
			csvAdded.Add(::FormatDoubleValue(m_dTotalAddedPurse3[nTOTAL_KIOSK]));
		}
	}
	m_Report.WriteLine ( csvAdded );
}


//*****************************************************************************
