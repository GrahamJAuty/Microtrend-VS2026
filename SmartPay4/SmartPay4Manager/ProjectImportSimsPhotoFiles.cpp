/*********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/*********************************************************************/
#include "..\SmartPay4ManagerBgnd\ImportSimsPhotoFiles.h"
/*********************************************************************/
#include "ImportStatsDlg.h"
/*********************************************************************/
static const char* szBLANK = "    --";
/*********************************************************************/

CImportSimsPhotoFiles::CImportSimsPhotoFiles( CSimsData* pSims, CWnd* pParent )
{
	m_pSims = pSims;
	m_pParent = pParent;
	m_strReportTitle  = "SIMS Photo Import";
	Init();
}

/*********************************************************************/

bool CImportSimsPhotoFiles::ShowImports()
{
	m_ImportFlag.SetImportFlag( FALSE );

	CString strReportLabel	= "R213";
	CString strReportFile = Filenames.GetReportFilename ( strReportLabel );
	CString strParamsFile = Filenames.GetReportParamsFilename ( strReportLabel );

	CRepmanHandler repman ( m_strReportTitle, m_pParent );

	int nErrorNo;
	if ( m_Report.Open ( strReportFile ) == TRUE )				// open report output file
	{
		m_Report.SetStyle1 ( m_strReportTitle );
		AddColumnText();

		nErrorNo = ProcessPhotos();								// no import reqd					

		AddStatistics();

		m_Report.Close();										// close report file
	}
	else
		nErrorNo = nREPORT_CREATEFAIL;

	if ( m_strError == "" )
	{
		if ( nErrorNo == nREPORT_NOERROR )
		{
			if ( m_nIgnored > 0 )
				repman.DisplayMessage ( szMSG_EXCEPTIONSDETECTED, MB_ICONEXCLAMATION );		// "Exceptions were detected with the import data!\n\nPlease see end of report for details.";

			nErrorNo = repman.DisplayReport ( strReportFile, strParamsFile, strReportLabel );
		}
		repman.DisplayError ( nErrorNo );
		return TRUE;
	}
	else
		repman.DisplayMessage ( m_strError, MB_ICONEXCLAMATION );

	return FALSE;
}

//**********************************************************************

void CImportSimsPhotoFiles::AddColumnText()
{
	m_Report.AddColumn ( m_pSims->GetLocatePhotoByText(), TA_LEFT, 210 );
	m_Report.AddColumn ( "Account", TA_LEFT, 180 );
	m_Report.AddColumn ( "Forename", TA_LEFT, 260 );
	m_Report.AddColumn ( "Surname", TA_LEFT, 260 );
	m_Report.AddColumn ( "PhotoFile", TA_LEFT, 1000 );
}

//**********************************************************************

bool CImportSimsPhotoFiles::ShowImportLine ( const char* szPhotoRef, const char* szCardNo, const char* szPhotoPathname, bool bReplacement )
{
	CString strCardNo	= szCardNo;
	CString strForename = "";
	CString strSurname	= "";

	if ( strCardNo == "" )
		strCardNo = szBLANK ;
	else
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserIDString(szCardNo);

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )	// see if in database
		{
			strForename = RowAccount.GetForename();
			strSurname = RowAccount.GetSurname();
		}

		strCardNo = System.FormatCardNo ( szCardNo );

		if ( bReplacement == FALSE )							// see if previous photo id exists
			strCardNo += " *";									// no - set display flag
	}

	CCSV csv ( '\t' );
	csv.Add ( szPhotoRef );
	csv.Add ( strCardNo );
	csv.Add ( strForename );
	csv.Add ( strSurname );
	csv.Add ( szPhotoPathname );

	return ( m_Report.WriteLine ( csv ) );
}

//**********************************************************************

void CImportSimsPhotoFiles::AddStatistics()
{
	CString strLine;
	m_Report.WriteLine ( "" );

	strLine.Format ( "%d\t<..>Photo files found", m_nRead );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>New photos added *", m_nAdded );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>Existing photos updated", m_nRead - ( m_nAdded + m_nIgnored ) );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>Exceptions found", m_nIgnored );
	m_Report.WriteLine ( strLine );
}

//*******************************************************************

void CImportSimsPhotoFiles::ShowStatistics()
{
	CImportStatsDlgConfig Config;
	Config.m_nLinesRead = m_nRead;
	Config.m_nLinesInvalid = m_nInvalid;
	Config.m_nLinesIgnored = m_nIgnored;
	Config.m_nLinesUsed = m_nUsed;
	Config.m_nRecordsAdded = m_nAdded;
	Config.m_nLinesReadPos = 0;
	Config.m_nLinesInvalidPos = 1;
	Config.m_nLinesIgnoredPos = 2;
	Config.m_nLinesUsedPos = 3;
	Config.m_nRecordsAddedPos = 5;
	Config.m_nRecordsDeletedPos = -1;
	Config.m_nLastLinePos = 5;
	Config.m_strCaption = m_strReportTitle;

	CImportStatsDlg dlg(Config, m_pParent);
	dlg.DoModal();
}

//********************************************************************
