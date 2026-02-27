//*******************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_PebblePendingTransaction\SQLRecordSetPebblePendingTransaction.h"
#include "..\SmartPay4Shared\SQLTable_PebblePendingTransaction\SQLRowPebblePendingTransaction.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//*******************************************************************
#include "PebblePaymentExceptionReport.h"
//*******************************************************************

CPebblePaymentExceptionReport::CPebblePaymentExceptionReport(const char* szLabel, CWnd* pParent)
{
	m_strReportLabel = szLabel;
	m_pParent = m_pParent;
	m_dTotalValue = 0;
	
	m_strReportTitle = "Pebble Payment Outstanding Exceptions";
}

//*******************************************************************

void CPebblePaymentExceptionReport::Show()
{
	int nErrorNo = 0;
	CString strReportFile = Filenames.GetReportFilename ( m_strReportLabel );
	CString strParamsFile = Filenames.GetReportParamsFilename ( m_strReportLabel );

	CRepmanHandler repman ( m_strReportTitle, m_pParent );

	if ((nErrorNo = Create(strReportFile)) == nREPORT_NOERROR)
	{
		nErrorNo = repman.DisplayReport(strReportFile, strParamsFile, m_strReportLabel);
	}

	repman.DisplayError ( nErrorNo );
}

//*******************************************************************

int CPebblePaymentExceptionReport::Create(const char* szReportFile)
{
	if (m_Report.Open(szReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	bool bCancelButton = FALSE;

	m_Report.SetStyle1 ( m_strReportTitle );
	AddColumnText();

	CSQLRecordSetPebblePendingTransaction RecordSetPendingTran(nullptr,
		RSParams_PebblePendingTransaction_NormalNoParams{},
		TRUE);
	
	CWorkingDlg WorkingDlg("Import Pebble Transactions");
	WorkingDlg.Create();
	WorkingDlg.SetCaption1("Processing Pending Transactions");
	int nProcessedSoFar = 0;

	CSQLRowPebblePendingTransaction RowPendingTran;
	while (RecordSetPendingTran.StepSelectAll(RowPendingTran) == TRUE)
	{
		WorkingDlg.SetCaption2RecordsChecked(++nProcessedSoFar, FALSE);
		SaveLine(RowPendingTran);
	}

	ShowTotalValue();

	m_Report.Close();

	return nREPORT_NOERROR;
}

//**********************************************************************

void CPebblePaymentExceptionReport::AddColumnText()
{
	m_Report.AddColumn(PebblePendingTransaction::DateTime.Label, TA_LEFT, 500);
	m_Report.AddColumn(PebblePendingTransaction::TransactionID.Label, TA_LEFT, 700);
	m_Report.AddColumn(PebblePendingTransaction::MemberID.Label, TA_LEFT, 700);
	m_Report.AddColumn("Pebble Name", TA_LEFT, 500);
	m_Report.AddColumn(PebblePendingTransaction::Amount.Label, TA_RIGHT, 300);
}

//**********************************************************************

void CPebblePaymentExceptionReport::SaveLine(CSQLRowPebblePendingTransaction& RowTran)
{
	CCSV csv ( '\t' );
	
	CString strDate = "";
	CString strSQLDate = RowTran.GetDateTime();

	if (strSQLDate.GetLength() == 14)
	{
		strDate += strSQLDate.Mid(6, 2);
		strDate += "/";
		strDate += strSQLDate.Mid(4, 2);
		strDate += "/";
		strDate += strSQLDate.Left(4);
		strDate += " @ ";
		strDate += strSQLDate.Mid(8,2);
		strDate += ":";
		strDate += strSQLDate.Mid(10, 2);
		strDate += ":";
		strDate += strSQLDate.Right(2);
	}

	double dAmount = ((double)RowTran.GetAmount()) / 100.0;
	
	csv.Add(strDate);
	csv.Add(RowTran.GetTransactionID());
	csv.Add(RowTran.GetMemberID());
	csv.Add(RowTran.GetName());
	csv.Add(dAmount, 2);
	
	m_dTotalValue += dAmount;

	m_Report.WriteLine ( csv );
}

//**********************************************************************

void CPebblePaymentExceptionReport::ShowTotalValue()
{
	CCSV csv ( '\t' );
	csv.Add("");
	csv.Add("");
	csv.Add("");
	csv.Add("");
	csv.Add(m_dTotalValue, 2);

	m_Report.WriteLine ( "\t\t\t\t<LI>" );
	m_Report.WriteLine ( csv );
}

//**********************************************************************
