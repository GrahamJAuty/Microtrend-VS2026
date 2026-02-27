//$$******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//$$******************************************************************
#include "WebPaymentReporter.h"
//$$******************************************************************

CWebPaymentReporter::CWebPaymentReporter(const char* szLabel, int nWebPaymentType, const char* szFilename, CWnd* pParent)
{
	m_pParent = pParent;
	m_nWebPaymentType = nWebPaymentType;
	m_strFilename = szFilename;
	m_strReportLabel = szLabel;
	m_bGotSubtotal = FALSE;

	if (m_strReportLabel == "R203")
	{
		switch (System.GetWebPaymentType())
		{
		case nWEBPAYMENT_PEBBLE:
			m_strTitle = "Pebble Transaction Import Exceptions";
			break;

		default:
			m_strTitle.Format("%s WebPayment Import Exceptions",
				(const char*)System.GetWebPaymentDisplayName());
			break;
		}
	}
	else
	{
		switch (System.GetWebPaymentType())
		{
		case nWEBPAYMENT_PEBBLE:
			m_strTitle = "Pebble Transaction Import Lines";
			break;

		default:
			m_strTitle.Format("%s WebPayment Import Lines",
				(const char*)System.GetWebPaymentDisplayName());
			break;
		}
	}

	// default texts are ParentPay's texts
	m_arrayColumns.Add(CWebPaymentReporterColumn(TRUE, "PaymentID", TA_LEFT, 210));
	m_arrayColumns.Add(CWebPaymentReporterColumn(TRUE, "ServiceID", TA_LEFT, 210));
	m_arrayColumns.Add(CWebPaymentReporterColumn(TRUE, "PaidAmount", TA_RIGHT, 300));
	m_arrayColumns.Add(CWebPaymentReporterColumn(TRUE, "PaidDate", TA_LEFT, 450));
	m_arrayColumns.Add(CWebPaymentReporterColumn(FALSE, "ReceiptCode", TA_LEFT, 360));
	m_arrayColumns.Add(CWebPaymentReporterColumn(TRUE, "TransRef", TA_LEFT, 240));
	m_arrayColumns.Add(CWebPaymentReporterColumn(TRUE, "Description", TA_LEFT, 240));
	m_arrayColumns.Add(CWebPaymentReporterColumn(TRUE, "PupilID", TA_LEFT, 180));
	m_arrayColumns.Add(CWebPaymentReporterColumn(TRUE, "Name", TA_LEFT, 450));
	m_arrayColumns.Add(CWebPaymentReporterColumn(FALSE, "PupilDOB", TA_LEFT, 330));

	switch (m_nWebPaymentType)
	{
	case nWEBPAYMENT_PARENTPAY:
		m_arrayColumns[4].m_bShow = TRUE;					// ReceiptCode
		break;

	case nWEBPAYMENT_WISEPAY:
		m_arrayColumns[1].m_strLabel = "ProductID"; 		// ServiceID
		m_arrayColumns[5].m_bShow = FALSE;					// TransRef
		m_arrayColumns[6].m_bShow = FALSE;					// Description
		break;

	case nWEBPAYMENT_TUCASI:
		m_arrayColumns[1].m_bShow = FALSE;					// ServiceID							
		m_arrayColumns[5].m_bShow = FALSE;					// TransRef
		break;

	case nWEBPAYMENT_SCHOOLCOMMS:
		m_arrayColumns[1].m_bShow = FALSE;					// ServiceID							
		m_arrayColumns[5].m_strLabel = "BatchID";	 		// TransRef
		m_arrayColumns[6].m_bShow = FALSE;					// Description
		break;

	case nWEBPAYMENT_TUCASIv2:
		m_arrayColumns[1].m_strLabel = "ItemNo";	 		// ServiceID
		m_arrayColumns[5].m_bShow = FALSE;					// TransRef
		m_arrayColumns[7].m_strLabel = "AccountID";	 		// PupilID
		m_arrayColumns[8].m_strLabel = "AccountName";	 	// PupilName
		break;

	case nWEBPAYMENT_SQUID:
		m_arrayColumns[1].m_strLabel = "Seq.No";	 		// ServiceID
		m_arrayColumns[3].m_bShow = FALSE;					// Date
		m_arrayColumns[5].m_bShow = FALSE;					// TransRef
		m_arrayColumns[6].m_bShow = FALSE;					// Description
		m_arrayColumns[7].m_strLabel = "purseAccountNumber";// PupilID
		m_arrayColumns[8].m_strLabel = "Name";				// PupilName	
		break;

	case nWEBPAYMENT_PARENTMAIL:
		m_arrayColumns[1].m_bShow = FALSE;					// ServiceID							
		m_arrayColumns[5].m_bShow = FALSE;					// TransRef
		m_arrayColumns[6].m_bShow = FALSE;					// Description
		m_arrayColumns[8].m_strLabel = "Name";				// PupilName
		break;

	case nWEBPAYMENT_BROMCOM:
		m_arrayColumns[1].m_bShow = FALSE;					// ServiceID							
		m_arrayColumns[5].m_bShow = FALSE;					// TransRef
		m_arrayColumns[6].m_bShow = FALSE;					// Description
		break;

	case nWEBPAYMENT_PEBBLE:
		m_arrayColumnOrder.RemoveAll();
		m_arrayColumnOrder.Add(5); //TRANSREF
		m_arrayColumnOrder.Add(7); //PUPIL ID
		m_arrayColumnOrder.Add(3); //DATE
		m_arrayColumnOrder.Add(8); //PUPIL NAME
		m_arrayColumnOrder.Add(2); //AMOUNT
		m_arrayColumnOrder.Add(6); //DESCRIPTIOM
		
		//ADJUST SETTINGS FOR VISIBLE COLUMNS
		m_arrayColumns[5].m_strLabel = "Pebble TransactionID";
		m_arrayColumns[5].m_nWidth = 700;
		m_arrayColumns[7].m_strLabel = "Pebble MemberID";
		m_arrayColumns[7].m_nWidth = 700;
		m_arrayColumns[3].m_strLabel = "Date";
		
		if (m_strReportLabel == "R203")
		{
			m_arrayColumns[8].m_strLabel = "Pebble Name";
		}
		else
		{
			m_arrayColumns[8].m_strLabel = "SmartPay Name";
		}

		m_arrayColumns[2].m_strLabel = "Amount";
		m_arrayColumns[6].m_strLabel = "Type";

		//FLAG HIDDEN COLUMNS
		m_arrayColumns[0].m_bShow = FALSE;
		m_arrayColumns[1].m_bShow = FALSE;
		m_arrayColumns[4].m_bShow = FALSE;
		m_arrayColumns[9].m_bShow = FALSE;

		break;
	}

	//PEBBLE HAS CUSTOM COLUMN ORDER
	if ( m_nWebPaymentType != nWEBPAYMENT_PEBBLE )
	{
		m_arrayColumnOrder.RemoveAll();
		for (int n = 0; n < m_arrayColumns.GetSize(); n++)
		{
			if (TRUE == m_arrayColumns[n].m_bShow)
			{
				m_arrayColumnOrder.Add(n);
			}
		}
	}

	ResetTotal(0);
}

//$$******************************************************************

void CWebPaymentReporter::ResetTotal(int n)
{
	m_dPaidTotal[n] = 0.0;
}

//$$******************************************************************

void CWebPaymentReporter::Show(int nMode)
{
	int nErrorNo = 0;
	CString strReportFile = Filenames.GetReportFilename(m_strReportLabel);
	CString strParamsFile = Filenames.GetReportParamsFilename(m_strReportLabel);

	CRepmanHandler repman(m_strTitle, m_pParent);
	repman.SetMode(nMode);

	if ((nErrorNo = Create(strReportFile)) == nREPORT_NOERROR)
	{
		nErrorNo = repman.DisplayReport(strReportFile, strParamsFile, m_strReportLabel);
	}

	repman.DisplayError(nErrorNo);
}

//$$******************************************************************

int CWebPaymentReporter::Create(const char* szReportFile)
{
	bool bReply = FALSE;

	CSSFile file;
	if (file.Open(m_strFilename, "rb") == FALSE)		// try and open audit file
	{
		return nREPORT_NOSOURCE;							// no source data found
	}

	if (m_Report.Open(szReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CWorkingDlg WorkingDlg(szMSG_CREATINGREPORT,TRUE);
	WorkingDlg.Create();

	m_Report.SetStyle1(m_strTitle);
	AddColumnText();

	bool bCancelButton = FALSE;

	CWebPaymentLogRecord record;
	while (record.ReadLine(&file) == TRUE)
	{
		if (WorkingDlg.SetCaption2PercentDone(file.GetPercentPosition(),TRUE) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		ShowReportLine(&record);
	}

	SaveTotal(0);

	if (TRUE == m_bGotSubtotal)
	{
		SaveTotal(1);
	}

	m_Report.Close();
	WorkingDlg.DestroyWindow();

	return (bCancelButton == TRUE) ? nREPORT_CANCEL : nREPORT_NOERROR;
}

//$$******************************************************************

void CWebPaymentReporter::AddReportField(CCSV& csv, __int64 n)
{
	csv.Add(n);
}

//$$******************************************************************

void CWebPaymentReporter::AddReportField(CCSV& csv, CString str)
{
	csv.Add(str);
}

//$$******************************************************************

void CWebPaymentReporter::AddReportField( CCSV& csv, double d, int nDPs)
{
	csv.Add(d,nDPs);
}

//$$******************************************************************

bool CWebPaymentReporter::ShowReportLine(CWebPaymentLogRecord* pRecord)
{
	CCSV csv('\t');

	if (pRecord->IsHeaderLine() == TRUE)
	{
		// display any previous paid totals 
		if (SaveTotal(0) == TRUE)
		{
			m_bGotSubtotal = TRUE;
		}

		CString strBuf = "";
		strBuf.Format("<..>%s  @  %s",
			pRecord->GetHeaderDate(),
			pRecord->GetHeaderTime());

		csv.Add(strBuf);
		m_Report.WriteLine(csv);

		return m_Report.WriteLine("<LI>");
	}
	else
	{
		double dPaidTotal = pRecord->GetPaidAmount();

		m_dPaidTotal[0] += dPaidTotal;
		m_dPaidTotal[1] += dPaidTotal;

		for (int n = 0; n < m_arrayColumnOrder.GetSize(); n++)
		{
			int nCol = m_arrayColumnOrder.GetAt(n);

			if ((nCol >= 0) && (nCol < m_arrayColumns.GetSize()))
			{
				switch (nCol)
				{
				case 0:
					AddReportField(csv, pRecord->GetPaymentID());
					break;

				case 1:
					AddReportField(csv, pRecord->GetServiceID());
					break;

				case 2:
					AddReportField(csv, dPaidTotal, 2);
					break;

				case 3:
					AddReportField(csv, pRecord->GetPaidDate());
					break;

				case 4:
					AddReportField(csv, pRecord->GetReceiptCode());
					break;

				case 5:
					AddReportField(csv, pRecord->GetTransactionRef());
					break;

				case 6:
					AddReportField(csv, pRecord->GetNotes());
					break;

				case 7:
					AddReportField(csv, pRecord->GetPupilID());
					break;

				case 8:
					AddReportField(csv, pRecord->GetPupilName());
					break;

				case 9:
					AddReportField(csv, pRecord->GetDOB());
					break;

				default:
					csv.Add("");
					break;
				}
			}
		}

		CString strPrefix = pRecord->GetPrefix();					// first entry on line

		if (m_strReportLabel == "R203")
		{
			csv.Add(strPrefix);					// exception
		}
		else
		{
			csv.Add(System.FormatCardNo(strPrefix));
		}

		return m_Report.WriteLine(csv);
	}
}

//$$******************************************************************

void CWebPaymentReporter::AddReportColumn(CWebPaymentReporterColumn& Column)
{
	m_Report.AddColumn(Column.m_strLabel, Column.m_nAlign, Column.m_nWidth);
}

//$$******************************************************************

void CWebPaymentReporter::AddColumnText()
{
	for (int n = 0; n < m_arrayColumnOrder.GetSize(); n++)
	{
		int nCol = m_arrayColumnOrder.GetAt(n);

		if ( ( nCol >= 0 ) && ( nCol < m_arrayColumns.GetSize()))
		{ 
			AddReportColumn(m_arrayColumns[nCol]);
		}
	}

	if (m_strReportLabel == "R203")
	{
		m_Report.AddColumn("Exception", TA_LEFT, 600);
	}
	else
	{
		m_Report.AddColumn("UserID", TA_LEFT, 210);
	}
}

//$$******************************************************************

bool CWebPaymentReporter::SaveTotal(int nIndex)
{
	if ((0 == nIndex) && (0.0 == m_dPaidTotal[nIndex]))
	{
		return FALSE;
	}

	int nTotalPos = -1;
	for (int n = 0; n < m_arrayColumnOrder.GetSize(); n++)
	{
		if (m_arrayColumnOrder.GetAt(n) == 2)
		{
			nTotalPos = n;
			break;
		}
	}

	if (-1 == nTotalPos)
	{
		return FALSE;
	}

	CCSV csv('\t');
	CString strTabLine = "";

	if (nTotalPos != 0)
	{
		csv.Add("Total");
		strTabLine += "\t";
	}

	for (int n = 1; n < nTotalPos; n++)
	{
		csv.Add("");
		strTabLine += "\t";
	}

	csv.Add(m_dPaidTotal[nIndex], 2);
	strTabLine += "<LI>";

	m_Report.WriteLine(strTabLine);
	m_Report.WriteLine(csv);
	m_Report.WriteLine("");

	ResetTotal(nIndex);					

	return TRUE;
}

//$$******************************************************************
