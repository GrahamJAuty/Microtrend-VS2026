//*******************************************************************
#include "..\SmartPay4ManagerBgnd\BromComImport.h"
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_BromComPendingOrder\SQLRecordSetBromComPendingOrder.h"
#include "..\SmartPay4Shared\SQLTable_BromComPendingOrder\SQLRowBromComPendingOrder.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//*******************************************************************
#include "BromComPaymentExceptionReport.h"
//*******************************************************************

CBromComPaymentExceptionReport::CBromComPaymentExceptionReport(const char* szLabel, CWnd* pParent)
{
	m_strReportLabel = szLabel;
	m_pParent = m_pParent;
	m_dTotalValue = 0;
	
	m_strReportTitle = "Bromcom Payment Outstanding Exceptions";
}

//*******************************************************************

void CBromComPaymentExceptionReport::Show()
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

int CBromComPaymentExceptionReport::Create(const char* szReportFile)
{
	if (m_Report.Open(szReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	bool bCancelButton = FALSE;

	m_Report.SetStyle1 ( m_strReportTitle );
	AddColumnText();

	CSQLRecordSetBromComPendingOrder RecordSetPendingOrder(NULL, RSParams_BromComPendingOrder_NormalNoParams{});
	
	CWorkingDlg WorkingDlg("Import BromCom Orders");
	WorkingDlg.Create();
	WorkingDlg.SetCaption1("Processing Pending Orders");
	int nProcessedSoFar = 0;

	CSQLRowBromComPendingOrder RowPendingOrder;
	while (RecordSetPendingOrder.StepSelectAll(RowPendingOrder) == TRUE)
	{
		WorkingDlg.SetCaption2RecordsChecked(++nProcessedSoFar, FALSE);
		
		CReportConsolidationArray<CConsolidatedDoubleByInt> arrayItemDelta;
		CBromComImport::GetItemDelta(RowPendingOrder, arrayItemDelta);

		for (int n = 0; n < arrayItemDelta.GetSize(); n++)
		{
			CConsolidatedDoubleByInt item;
			arrayItemDelta.GetAt(n, item);
			SaveLine(RowPendingOrder, item);
		}
	}

	ShowTotalValue();

	m_Report.Close();

	return nREPORT_NOERROR;
}

//**********************************************************************

void CBromComPaymentExceptionReport::AddColumnText()
{
	m_Report.AddColumn(BromComPendingOrder::LastModified.Label, TA_LEFT, 500);
	m_Report.AddColumn(BromComPendingOrder::OrderID.Label, TA_LEFT, 250);
	m_Report.AddColumn(BromComPendingOrder::Status.Label, TA_LEFT, 250);
	m_Report.AddColumn(BromComPendingItem::PersonID.Label, TA_LEFT, 250);
	m_Report.AddColumn("Pending Amount", TA_RIGHT, 350);
}

//**********************************************************************

void CBromComPaymentExceptionReport::SaveLine(CSQLRowBromComPendingOrder& RowOrder, CConsolidatedDoubleByInt& item)
{
	CCSV csv ( '\t' );
	
	CString strDate = "";
	
	CString strSQLDate = RowOrder.GetLastModified();
	if (strSQLDate.GetLength() != 19)
	{
		strDate += strSQLDate.Mid(8, 2);
		strDate += "/";
		strDate += strSQLDate.Mid(5, 2);
		strDate += "/";
		strDate += strSQLDate.Left(4);
		strDate += " @ ";
		strDate += strSQLDate.Mid(11, 8);
	}
	
	csv.Add(strDate);
	csv.Add(RowOrder.GetOrderID());
	csv.Add(RowOrder.GetStatus());
	csv.Add(item.m_nKey);
	csv.Add(item.m_dVal,2);
	
	m_dTotalValue += item.m_dVal;

	m_Report.WriteLine ( csv );
}

//**********************************************************************

void CBromComPaymentExceptionReport::ShowTotalValue()
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
