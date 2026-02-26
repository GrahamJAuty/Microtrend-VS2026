/**********************************************************************/
#include "Password.h"
#include "RepSpawn.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ReportDataTableServerDlg.h"
/**********************************************************************/

CReportDataTableServerDlg::CReportDataTableServerDlg(int nServerDbIdx, int nServerLocIdx, int nTerminalLocIdx, CWnd* pParent /*=NULL*/)
	: CReportDataTableDlg(pParent)
{
	m_strTitle = "Select Server Report Type";
	m_arrayReports.Add("Programming by Server Number");
	m_arrayReports.Add("Programming by Sort Order");
	m_arrayReports.Add("Epos Version Check");
	m_nServerDbIdx = nServerDbIdx;
	m_nServerLocIdx = nServerLocIdx;
	m_nTerminalLocIdx = nTerminalLocIdx;
}

/**********************************************************************/

void CReportDataTableServerDlg::HandleReport1()
{
	CreateServerListReport(FALSE);
}

/**********************************************************************/

void CReportDataTableServerDlg::HandleReport2()
{
	CreateServerListReport(TRUE);
}

/**********************************************************************/

void CReportDataTableServerDlg::CreateServerListReport( bool bSortOrder )
{
	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;

	DataManagerNonDb.Server.CreateServerReport( m_nServerDbIdx, m_nServerLocIdx, m_nTerminalLocIdx, bSortOrder, ReportFile );
	ReportFile.Close();

	CString strParamsFile = "";

	if ( PasswordArray.GetEnable( PasswordTicks::EditServerDetails ) == TRUE )
	{
		CFilenameUpdater FnUp ( SysFiles::ServerPrm );
		strParamsFile = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp ( SysFiles::ServerNamePrm );
		strParamsFile = FnUp.GetFilenameToUse();
	}

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), strParamsFile, "", this );
}

/**********************************************************************/

void CReportDataTableServerDlg::HandleReport3()
{
	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;

	ReportFile.SetStyle1 ( "Epos Version Check for Servers" );
		
	ReportFile.AddColumn ( "No", TA_LEFT, 100 );
	ReportFile.AddColumn ( "Receipt Name", TA_LEFT, 400 );
	ReportFile.AddColumn ( "Full Name", TA_LEFT, 400 );
	ReportFile.AddColumn ( "Minimum Version", TA_LEFT, 800 );
	ReportFile.AddColumn ( "Reason", TA_LEFT, 800 );

	for ( int nServerIdx = 0; nServerIdx < DataManagerNonDb.Server.GetSize(); nServerIdx++ )
	{
		CServerCSVRecord ServerRecord;
		DataManagerNonDb.Server.GetAt ( nServerIdx, ServerRecord );

		CMinimumSPOSVersion Version;
		ServerRecord.CheckMinimumSPOSVersion( Version );

		CCSV csvOut ( '\t' );
		csvOut.Add ( ServerRecord.GetServerNo() );
		csvOut.Add ( ServerRecord.GetReceiptName() );
		csvOut.Add ( ServerRecord.GetFullName() );
		csvOut.Add ( Version.GetMinVerName() );
		csvOut.Add ( Version.m_strReason );
	
		ReportFile.WriteLine ( csvOut.GetLine() );
	}
	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::ServerVersionPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}

/**********************************************************************/
