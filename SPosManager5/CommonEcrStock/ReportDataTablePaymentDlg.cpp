/**********************************************************************/
#include "CCSVReportLine.h"
#include "RepSpawn.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ReportDataTablePaymentDlg.h"
/**********************************************************************/

CReportDataTablePaymentDlg::CReportDataTablePaymentDlg(CWnd* pParent /*=NULL*/)
	: CReportDataTableDlg( pParent)
{
	m_strTitle = "Select Payment Report Type";
	m_arrayReports.Add( "Programming" );
	m_arrayReports.Add( "Epos Version Check" );
}

/**********************************************************************/

void CReportDataTablePaymentDlg::HandleReport1()
{
	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;

	ReportFile.SetStyle1 ( "Payment Methods" );
		
	ReportFile.AddColumn ( "No", TA_LEFT, 100 );
	ReportFile.AddColumn ( "EPOS Name", TA_LEFT, 400 );
	ReportFile.AddColumn ( "Report Name", TA_LEFT, 400 );
	ReportFile.AddColumn ( "Type", TA_LEFT, 400 );
	ReportFile.AddColumn ( "Report Group", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Forex", TA_LEFT, 180 );
	ReportFile.AddColumn ( "Authority Function", TA_LEFT, 350 );
	ReportFile.AddColumn ( "Limit", TA_RIGHT, 180 );
	ReportFile.AddColumn ( "Tender", TA_LEFT, 180 );
	ReportFile.AddColumn ( "Over Tender", TA_LEFT, 250 );
	ReportFile.AddColumn ( "Change", TA_LEFT, 180 );
	ReportFile.AddColumn ( "Gratuity", TA_LEFT, 180 );
	ReportFile.AddColumn ( "Drawer", TA_LEFT, 180 );
	ReportFile.AddColumn ( "Non Bank", TA_LEFT, 250 );
	ReportFile.AddColumn ( "Receipt", TA_LEFT, 180 );
	ReportFile.AddColumn ( "A/C Allow", TA_LEFT, 250 );
	ReportFile.AddColumn ( "Cashback", TA_LEFT, 250 );
	ReportFile.AddColumn ( "ChangeAsCashback", TA_LEFT, 360 );
	ReportFile.AddColumn ( "Handheld", TA_LEFT, 250 );
	ReportFile.AddColumn ( "InCashLimit", TA_LEFT, 250 );
	ReportFile.AddColumn ( "Parameter", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Text", TA_LEFT, 500 );

	for ( int nPaymentIdx = 0; nPaymentIdx < DataManager.Payment.GetSize(); nPaymentIdx++ )
	{
		CPaymentCSVRecord PaymentRecord;
		DataManager.Payment.GetAt ( nPaymentIdx, PaymentRecord );

		CCSVReportLine csvOut;
		csvOut.AppendInt( PaymentRecord.GetPaymentNo() );
		csvOut.AppendString( PaymentRecord.GetEPOSName() );

		csvOut.AppendString( PaymentRecord.GetDisplayName() );

		CString strReportGroup = DataManager.PaymentGroup.GetGroupDisplayText( PaymentRecord.GetReportGroup() );

		csvOut.AppendString( PaymentRecord.GetDisplayType() );
		csvOut.AppendString( strReportGroup );
		csvOut.AppendInt( PaymentRecord.GetForexNo() );

		CString strFuncName;
		int f = PaymentRecord.GetFunctionNo();
		strFuncName.Format ( "%d Payment %d", f, f + 1 - Payment::FunctionNo.Min );
		csvOut.AppendString( strFuncName );

		csvOut.AppendVal( PaymentRecord.GetLimit() );
		csvOut.AppendYesOrBlank( PaymentRecord.GetTender() );
		csvOut.AppendYesOrBlank( PaymentRecord.GetOverTender() );
		csvOut.AppendYesOrBlank( PaymentRecord.GetChange() );
		csvOut.AppendYesOrBlank( PaymentRecord.GetGratuity() );
		csvOut.AppendYesOrBlank( PaymentRecord.GetDrawer() );
		csvOut.AppendYesOrBlank( PaymentRecord.GetNonBnk() );
		csvOut.AppendYesOrBlank( PaymentRecord.GetReceipt() );
		csvOut.AppendYesOrBlank( PaymentRecord.GetAccountAllow() );

		{
			bool bEFTCashback = FALSE;
			bool bChangeAsCashback = FALSE;

			switch ( PaymentRecord.GetPaymentType() )
			{
			case PAYMENT_TYPE_OTHER:
				bChangeAsCashback = PaymentRecord.GetChangeAsCashbackFlag();
				break;

			case PAYMENT_TYPE_EFT:
			case PAYMENT_TYPE_EFT_CNP:
				bEFTCashback = PaymentRecord.GetEFTCashback();
				break;
			}
			
			csvOut.AppendYesOrBlank( bEFTCashback );
			csvOut.AppendYesOrBlank( bChangeAsCashback );
		}

		csvOut.AppendYesOrBlank( PaymentRecord.GetHandheld() );
		csvOut.AppendYesOrBlank( PaymentRecord.GetCashLimitFlag() );	
		csvOut.AppendString( PaymentRecord.GetExternalParams() );

		CStringArray arrayTexts;
		PaymentRecord.GetPaymentTexts( arrayTexts );

		CString strText = "";
		for ( int n = 0; n < arrayTexts.GetSize(); n++ )
		{
			strText += arrayTexts.GetAt(n);
			strText.TrimRight();
			strText += " ";
		}

		strText.TrimRight();
		csvOut.AppendString( strText );
		
		ReportFile.WriteLine ( csvOut.GetLine() );
	}
	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::PaymentPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}

/**********************************************************************/

void CReportDataTablePaymentDlg::HandleReport2()
{
	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;

	ReportFile.SetStyle1 ( "Epos Version Check for Payment Methods" );
		
	ReportFile.AddColumn ( "No", TA_LEFT, 100 );
	ReportFile.AddColumn ( "Name", TA_LEFT, 400 );
	ReportFile.AddColumn ( "Minimum Version", TA_LEFT, 800 );
	ReportFile.AddColumn ( "Reason", TA_LEFT, 800 );

	for ( int nPaymentIdx = 0; nPaymentIdx < DataManager.Payment.GetSize(); nPaymentIdx++ )
	{
		CPaymentCSVRecord PaymentRecord;
		DataManager.Payment.GetAt ( nPaymentIdx, PaymentRecord );

		CMinimumSPOSVersion Version;
		PaymentRecord.CheckMinimumSPOSVersion( Version );

		CCSV csvOut ( '\t' );
		csvOut.Add ( PaymentRecord.GetPaymentNo() );
		csvOut.Add ( PaymentRecord.GetDisplayName() );
		csvOut.Add ( Version.GetMinVerName() );
		csvOut.Add ( Version.m_strReason );
	
		ReportFile.WriteLine ( csvOut.GetLine() );
	}
	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::PaymentVersionPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}

/**********************************************************************/
