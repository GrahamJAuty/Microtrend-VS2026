//**********************************************************************
#include "..\SmartPay4Shared\PaymentAuditReport.h"
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//**********************************************************************

CPaymentAuditReport::CPaymentAuditReport ( const char* szAuditFile, CWnd* pParent ) :
	m_dlgReport("R135")
	, m_Repman("", &m_dlgReport)
{
	m_strAuditFilename = szAuditFile;
	m_pParent = pParent;
	m_strReportLabel = "R135";
	m_bSummaryReqd = FALSE; //CAN BE SET EXTERNALLY

	m_eposPayments.Read();
	m_eposPaymentGroups.Read();
	m_Repman.SetTitle ( m_dlgReport.GetReportTitle() );
}

//**********************************************************************

void CPaymentAuditReport::Show()
{
	if ( m_eposPayments.GetRecordCount() == 0 )					//29-03-2017 - v1.14d
	{
		Prompter.Error( "Ecr\\Stock Manager EPOS Payment Type details not found!", m_dlgReport.GetReportTitle() );
		return;
	}
	
	if ( m_eposPaymentGroups.GetRecordCount() == 0 )			//29-03-2017 - v1.14d
	{
		Prompter.Error( "Ecr\\Stock Manager EPOS Payment Group details not found!", m_dlgReport.GetReportTitle() );
		return;
	}

	CEposPaymentAuditFile auditFile ( m_strAuditFilename );
	auditFile.Open();
	auditFile.GetFirstDate ( m_dlgReport.m_dateAllFrom );		// try & get first date in audit file
	auditFile.Close();

	while (m_dlgReport.DoModal() != IDCANCEL)
	{
		m_bSummaryReqd = (m_dlgReport.m_bSummaryReqd != 0);
		Reset(nTOTAL_CARD);
		Reset(nTOTAL_REPORT);
		ResetPaymentGroups();

		if (m_Repman.DisplayError(CreateReport()) == FALSE)	// exit on a fatal error
		{
			break;
		}
	}
}

//********************************************************************

bool CPaymentAuditReport::CheckSaveLine( CCSV& csv )
{
	return ( m_bSummaryReqd == TRUE ) ? TRUE : m_Report.WriteLine( csv );
}

//********************************************************************

int CPaymentAuditReport::CreateReport()
{
	if (m_Report.Open(m_dlgReport.GetReportFilename()) == FALSE)
		return nREPORT_CREATEFAIL;

	CEposPaymentAuditFile auditFile(m_strAuditFilename);
	if (auditFile.Open() == FALSE)							// reopen source file
	{
		m_Report.Close();										// close report file
		return nREPORT_OPENERROR;
	}

	int nErrorNo = nREPORT_NOERROR;

	CWorkingDlg dlgWorking(szMSG_CREATINGREPORT, TRUE);
	dlgWorking.Create();

	CString strTitle = m_dlgReport.GetReportTitle();
	strTitle += m_dlgReport.GetDateString();
	m_Report.SetStyle1(strTitle);

	AddColumnText();

	auditFile.JumpIn(m_dlgReport.m_dateFrom.GetDate());

	switch (m_dlgReport.m_nOrderBy)
	{
	case nORDERBY_DATE:
		nErrorNo = ReportByDate(&auditFile, &dlgWorking);
		break;

	case nORDERBY_USERID:
		nErrorNo = ReportByCardNo(&auditFile, &dlgWorking);
		break;

	case nORDERBY_TYPE:
		nErrorNo = ReportByPayment(&auditFile, &dlgWorking);
		break;
	}

	dlgWorking.DestroyWindow();

	m_Report.Close();
	auditFile.Close();										// close audit file open

	if (nErrorNo == nREPORT_NOERROR)
	{
		nErrorNo = m_Repman.DisplayReport(m_dlgReport.GetReportFilename(), m_dlgReport.GetParamsFilename(), m_dlgReport.GetReportKey(), &m_dlgReport);
	}

	return nErrorNo;
}

//*****************************************************************

int CPaymentAuditReport::ReportByDate ( CEposPaymentAuditFile* pAudit, CWorkingDlg* pDlgWorking )
{
	int nCount = 0;
	bool bCancelButton = FALSE;

	CPaymentAuditRecord atc;
	while ( pAudit->ReadLine ( &atc) == TRUE )
	{
		if (pDlgWorking -> SetCaption2PercentDone(pAudit->GetPercentPosition(), TRUE) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}
		
		if (m_dlgReport.IsValid(&atc) == TRUE)
		{
			if (SaveLine(&atc) == TRUE)
			{
				++nCount;
			}
		}
	}

	int nErrorNo = nREPORT_NOERROR;

	if ( nCount > 0 )
	{
		if ( m_bSummaryReqd == FALSE )
		{
			SaveTotals ( nTOTAL_REPORT, "" );

			m_Report.WriteLine ( "" );
			m_Report.WriteLine ( "" );
		}

		SaveGroupTotals();
	}
	else
	{
		nErrorNo = nREPORT_NOMATCH;
	}

	return ( bCancelButton == TRUE ) ? nREPORT_CANCEL : nErrorNo;
}

//**********************************************************************

int CPaymentAuditReport::ReportByCardNo(CEposPaymentAuditFile* pAudit, CWorkingDlg* pDlgWorking)
{
	CRecordSorter sorter;
	CPaymentAuditRecord atc;

	bool bCancelButton = FALSE;

	while (TRUE)
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

		if (m_dlgReport.IsValid(&atc) == TRUE)
		{
			sorter.Add(System.FormatCardNo(atc.GetUserID()), nStartFilePos);
		}
	}

	if (TRUE == bCancelButton)
	{
		return nREPORT_CANCEL;
	}

	if (sorter.GetSize() == 0)
	{
		return nREPORT_NOMATCH;
	}

	int nLineCount = 0;
	CString strLastCardNo = "";

	for (int nIndex = 0; nIndex < sorter.GetSize(); nIndex++)
	{
		if (pDlgWorking->SetCaption2PercentDone(50 + ((nIndex * 50L) / sorter.GetSize()), TRUE) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		pAudit->SetFilePos(sorter.GetRecordNo(nIndex));
		if (pAudit->ReadLine(&atc) == TRUE)
		{
			CString strCardNo = System.FormatCardNo(atc.GetUserID());

			if (0 == nIndex)								// on first time round loop
			{
				strLastCardNo = strCardNo;
			}
			else
			{
				if (strCardNo != strLastCardNo)			// change of main label
				{
					if (nLineCount > 0)
					{
						SaveTotals(nTOTAL_CARD, strLastCardNo);
					}

					nLineCount = 0;
					strLastCardNo = strCardNo;
					Reset(nTOTAL_CARD);
				}
			}

			if (SaveLine(&atc) == TRUE)
			{
				++nLineCount;
			}
		}
	}

	if (nLineCount > 0)
	{
		SaveTotals(nTOTAL_CARD, strLastCardNo);
	}

	SaveTotals(nTOTAL_REPORT, "");

	return (bCancelButton == TRUE) ? nREPORT_CANCEL : nREPORT_NOERROR;
}

//****************************************************************

int CPaymentAuditReport::ReportByPayment(CEposPaymentAuditFile* pAudit, CWorkingDlg* pDlgWorking)
{
	CRecordSorter sorter;
	CPaymentAuditRecord atc;

	bool bCancelButton = FALSE;

	while (TRUE)
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

		if (m_dlgReport.IsValid(&atc) == TRUE)
		{
			CString strLabel = "";
			strLabel.Format("%3.3d", atc.GetPaymentType());
			sorter.Add(strLabel, nStartFilePos);
		}
	}

	if (TRUE == bCancelButton)
	{
		return nREPORT_CANCEL;
	}

	if (sorter.GetSize() == 0)
	{
		return nREPORT_NOMATCH;
	}

	CString strType = "";
	int nLineCount = 0;
	int nLastPaymentType = 0;

	for ( int nIndex = 0 ; nIndex < sorter.GetSize() ; nIndex++ )
	{
		if (pDlgWorking->SetCaption2PercentDone(50 + ((nIndex * 50L) / sorter.GetSize()), TRUE) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		pAudit->SetFilePos ( sorter.GetRecordNo(nIndex) );
		if (pAudit->ReadLine(&atc) == TRUE)
		{
			int nPaymentType = atc.GetPaymentType();

			if (0 == nIndex)								// on first time round loop
			{
				nLastPaymentType = nPaymentType;
			}
			else
			{
				if (nPaymentType != nLastPaymentType)			// change of main label
				{
					if (nLineCount > 0)
					{
						strType.Format("%d", nLastPaymentType);
						SaveTotals(nTOTAL_CARD, strType);
					}

					nLineCount = 0;
					nLastPaymentType = nPaymentType;
					Reset(nTOTAL_CARD);
				}
			}

			if (SaveLine(&atc) == TRUE)
			{
				++nLineCount;
			}
		}
	}

	if ( nLineCount > 0 )
	{
		strType.Format ( "%d", nLastPaymentType );
		SaveTotals ( nTOTAL_CARD, strType );
	}

	SaveTotals( nTOTAL_REPORT, "" );

	m_Report.WriteLine ( "" );
	m_Report.WriteLine ( "" );
	SaveGroupTotals();

	return ( bCancelButton == TRUE ) ? nREPORT_CANCEL : nREPORT_NOERROR;
}

//**********************************************************************

void CPaymentAuditReport::AddColumnText()
{
	if ( m_bSummaryReqd == FALSE )
	{
		m_Report.AddColumn ( "Audit Date", TA_LEFT, 210 );
		m_Report.AddColumn ( "", TA_LEFT, 120 );
		m_Report.AddColumn ( "UserID", TA_LEFT, 210 );
		m_Report.AddColumn ( "Name", TA_LEFT, 300 );
		m_Report.AddColumn ( "Grp", TA_LEFT, 93 );
		m_Report.AddColumn ( "Last Used", TA_LEFT, 210 );
		m_Report.AddColumn ( "", TA_LEFT, 120 );
		m_Report.AddColumn ( "Op", TA_CENTER, 105 );
		m_Report.AddColumn ( "TNo", TA_CENTER, 105 );
		m_Report.AddColumn ( "Receipt", TA_CENTER, 216 );
		m_Report.AddColumn ( "Type", TA_LEFT, 120 );
		m_Report.AddColumn ( "Description", TA_LEFT, 300 );
	}
	else
	{
		m_Report.AddColumn("", TA_LEFT, 300);
	}

	m_Report.AddColumn ( "Purse1", TA_RIGHT, 230 );
	m_Report.AddColumn ( "Purse2", TA_RIGHT, 230 );

	if (System.GetEnablePurse3Flag() == TRUE)
	{
		m_Report.AddColumn("Purse3", TA_RIGHT, 230);
	}

	m_Report.AddColumn ( "Other", TA_RIGHT, 230 );
}

//**********************************************************************

CString CPaymentAuditReport::SkipColumns ( CCSV *pCsv )
{
	CString strTabLine = "\t\t\t\t\t\t\t\t\t\t\t\t<LI>\t<LI>\t<LI>";

	pCsv->Add ( "" );				// Audit time
	pCsv->Add ( "" );				// user id 
	pCsv->Add ( "" );				// name
	pCsv->Add ( "" );				// group 
	pCsv->Add ( "" );				// last used date
	pCsv->Add ( "" );				// last used time
	pCsv->Add ( "" );				// operator
	pCsv->Add ( "" );				// terminal
	pCsv->Add ( "" );				// transaction
	pCsv->Add ( "" );				// type
	pCsv->Add ( "" );				// description

	return strTabLine;
}

//**********************************************************************

void CPaymentAuditReport::SaveTotals(int n, const char* szText)
{
	bool bShowOther = TRUE;
	bool bShowCashless = TRUE;
	CString strTitle = "<..>Total";

	if (n == nTOTAL_CARD)
	{
		int nType = 0;
		CString strName = "";

		if (m_dlgReport.m_nOrderBy == nORDERBY_TYPE)
		{
			nType = atoi(szText);
			m_eposPayments.FindRecord(nType);
			strTitle.Format("<..>(T%2.2d) %s", nType, m_eposPayments.GetText());
		}

		else if (m_dlgReport.m_nOrderBy == nORDERBY_USERID)
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserIDString(szText);

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				strName = RowAccount.GetUsername();
			}
			else
			{
				strName = "unknown";
			}
		}

		if (m_bSummaryReqd == TRUE)
		{
			if (m_dlgReport.m_nOrderBy == nORDERBY_TYPE)
			{
				if (20 == nType)
				{
					bShowOther = FALSE;
				}
				else
				{
					bShowCashless = FALSE;
				}
			}
			else if (nORDERBY_USERID == m_dlgReport.m_nOrderBy)
			{
				strTitle.Format("<..>(%s) %s", szText, strName);
			}
		}
		else
		{
			if (nORDERBY_USERID == m_dlgReport.m_nOrderBy)
			{
				strTitle.Format("<..>Total %s", strName);
			}
		}
	}
	else
	{
		strTitle = "<..>Report Total";
	}

	CCSV csv('\t');
	CString strTabLine = "\t<LI>\t<LI>\t<LI>";					// assume summary report

	csv.Add(strTitle);

	if (m_bSummaryReqd == FALSE)
	{
		strTabLine = SkipColumns(&csv);						// skip over none display columns
	}

	if (TRUE == bShowCashless)
	{
		csv.Add(::FormatDoubleValue(m_PaymentSale[n].m_dPurse1));
	}
	else
	{
		csv.Add("--");							// no purse1
	}

	if (TRUE == bShowCashless)
	{
		csv.Add(::FormatDoubleValue(m_PaymentSale[n].m_dPurse2));
	}
	else
	{
		csv.Add("--");							// no purse2
	}

	if ( TRUE == System.GetEnablePurse3Flag() )
	{
		strTabLine += "\t<LI>";

		if (TRUE == bShowCashless)
		{
			csv.Add(::FormatDoubleValue(m_PaymentSale[n].m_dPurse3));
		}
		else
		{
			csv.Add("--");						// no purse3
		}
	}

	if (TRUE == bShowOther)
	{
		csv.Add(::FormatDoubleValue(m_PaymentSale[n].m_dOther));
	}
	else
	{
		csv.Add("--");						// no other
	}

	if (TRUE == m_bSummaryReqd)
	{
		if (nTOTAL_REPORT == n)
		{
			m_Report.WriteLine(strTabLine);
		}

		m_Report.WriteLine(csv.GetLine());
	}
	else
	{
		m_Report.WriteLine ( strTabLine );
		m_Report.WriteLine ( csv.GetLine() );

		if (nTOTAL_CARD == n)
		{
			m_Report.WriteLine("");
		}
	}
}

//**********************************************************************
// NOTE: Similar lines in EOD Reporting 
void CPaymentAuditReport::SaveGroupTotals()
{
	CEPosPaymentGroupSale Totals;

	CString strTitle;

	for (int i = 0; i < m_GroupSale.GetSize(); i++)
	{
		CEPosPaymentGroupSale GroupSale;
		m_GroupSale.GetAt(i, GroupSale);

		strTitle.Format("<..>(G%2.2d) %s",
			GroupSale.m_nGroupNo,
			(const char*)GroupSale.m_strGroupText);

		SaveGroupTotal(strTitle, GroupSale.m_dPurse1, GroupSale.m_dPurse2, GroupSale.m_dPurse3, GroupSale.m_dOther);

		Totals.Add(GroupSale);
	}

	SaveGroupTotal("", Totals.m_dPurse1, Totals.m_dPurse2, Totals.m_dPurse3, Totals.m_dOther);
}

//**********************************************************************

void CPaymentAuditReport::SaveGroupTotal(const char* szTitle, double dValue1, double dValue2, double dValue3, double dOther)
{
	CCSV csv('\t');
	CString strTabLine = "\t<LI>\t<LI>\t<LI>";					// assume summary report
	bool m_bTabLineReqd = FALSE;

	CString strTitle = szTitle;

	if (strTitle == "")										// see if report total
	{
		strTitle = "<..>Report Total";
		m_bTabLineReqd = TRUE;
	}

	csv.Add(strTitle);

	if (FALSE == m_bSummaryReqd)
	{
		strTabLine = SkipColumns(&csv);						// skip over none display columns
	}

	csv.Add(::FormatDoubleValue(dValue1));					// purse1
	csv.Add(::FormatDoubleValue(dValue2));					// purse2

	if (System.GetEnablePurse3Flag() == TRUE)
	{
		strTabLine += "\t<LI>";
		csv.Add(::FormatDoubleValue(dValue3));				// purse3
	}

	csv.Add(::FormatDoubleValue(dOther));					// other

	if (m_bTabLineReqd == TRUE)
	{
		m_Report.WriteLine(strTabLine);
	}

	m_Report.WriteLine(csv.GetLine());
}

//**********************************************************************


