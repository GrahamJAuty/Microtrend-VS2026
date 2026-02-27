/*********************************************************************/
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/*********************************************************************/
#include "ImportStatsDlg.h"
/*********************************************************************/
#include "..\SmartPay4ManagerBgnd\ImportSimsFile.h"
/*********************************************************************/
static const char* szBLANK = "    --";
/*********************************************************************/

CImportSimsFile::CImportSimsFile ( CSimsData* pSims, CWnd* pParent )
{
	m_pSims = pSims;
	m_pParent = pParent;
	m_strReportTitle = "SIMS Update File";

	Init();
}

/*********************************************************************/

CString CImportSimsFile::GetProgressTitle()
{
	CString strTitle;
	if ( m_ImportFlag.GetImportFlag() == TRUE )	
	{
		strTitle = "Import " + m_strReportTitle;
	}
	else										
	{
		strTitle = "Display " + m_strReportTitle;
	}

	return strTitle;
}

/*********************************************************************/
// Show SIMs Imports

bool CImportSimsFile::ShowImports ( const char* szFilename )
{
	m_ImportFlag.SetImportFlag( FALSE );

	m_strImportFilename	= szFilename;
	if ( m_strImportFilename == "" )
		m_strImportFilename	= m_pSims->m_strImportFilename;		// use default SIMS import filename if not set

	CString strReportLabel	= "R212";
	CString strReportFile = Filenames.GetReportFilename ( strReportLabel );
	CString strParamsFile = Filenames.GetReportParamsFilename ( strReportLabel );

	CRepmanHandler repman ( m_strReportTitle, m_pParent );

	int nErrorNo;
	if ( m_Report.Open ( strReportFile ) == TRUE )				// open report output file
	{
		m_Report.SetStyle1 ( m_strReportTitle );
		AddColumnText();

		nErrorNo = ProcessFile();						// no import reqd					

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

			repman.SetMode ( P_WAIT );
			nErrorNo = repman.DisplayReport ( strReportFile, strParamsFile, strReportLabel );
		}

		repman.DisplayError ( nErrorNo );
		return TRUE;
	}
	else
		repman.DisplayMessage ( m_strError, MB_ICONEXCLAMATION );

	return FALSE;
}

/*********************************************************************/

void CImportSimsFile::AddColumnText()
{
	if ( m_pSims->m_nSIMSUserTextNo > 0 )		m_Report.AddColumn ( "Adno", TA_LEFT, 210 );
	if ( m_pSims->m_nUPNUserTextNo > 0 )		m_Report.AddColumn ( "UPN", TA_LEFT, 300 );
	if ( m_pSims->m_nPIDUserTextNo > 0 )		m_Report.AddColumn ( "Person_id", TA_LEFT, 210 );

	m_Report.AddColumn ( "Account", TA_LEFT, 180 );
	m_Report.AddColumn ( "Forename", TA_LEFT, 260 );
	m_Report.AddColumn ( "Surname", TA_LEFT, 260 );
	m_Report.AddColumn ( "Name", TA_LEFT, 400 );

	if ( m_pSims->m_nDobNo > 0 )				m_Report.AddColumn ( "DOB", TA_LEFT, 219 );
	if ( m_pSims->m_nYearContactNo > 0 )		m_Report.AddColumn ( "Year", TA_LEFT, 200 );
	if ( m_pSims->m_nRegContactNo > 0 )			m_Report.AddColumn ( "Reg", TA_LEFT, 200 );

	m_Report.AddColumn ( "FSM", TA_CENTER, 120 );
	m_Report.AddColumn ( "Sex", TA_CENTER, 90 );

	if ( m_pSims->m_bSaveEmail == TRUE )		m_Report.AddColumn ( "Email", TA_LEFT, 600 );

	m_Report.AddColumn ( "", TA_LEFT, 630 );
}

/*********************************************************************/

bool CImportSimsFile::ShowImportLine ( const char* szCardNo )
{
	CString strCardNo	= szCardNo;
	CString strAdno		= m_simsFile.GetAdno();
	CString strUPN		= m_simsFile.GetUPN();
	CString strPID		= m_simsFile.GetPID();

	if ( strAdno == "" )	strAdno = szBLANK;
	if ( strUPN == "" )		strUPN = szBLANK;
	if ( strPID == "" )		strPID = szBLANK;

	if ( strCardNo == "" )										// see if valid account number
	{
		++m_nIgnored;												// will be ignored
		strCardNo = szBLANK;
	}
	else
	{
		bool bOk = TRUE;
		
		CSQLRepositoryAccount RepoAccount;
		bool bExists = (RepoAccount.RowExists(_atoi64(strCardNo), NULL).GetSQLResult() != 0);
		
		if ( bExists == FALSE && m_nImportMethod == nIMPORT_METHOD_EXISTONLY )	// see if must exist
			bOk = FALSE;

		else if ( bExists == TRUE && m_nImportMethod == nIMPORT_METHOD_NEWONLY )		// only new recod allowed
			bOk = FALSE;
	
		if ( bOk == TRUE )
		{
			++m_nUsed;												// inc number of valid lines 
			if ( bExists == FALSE )
			{
				++m_nAdded;											// will be new record
				strCardNo += " *";									// not in database - set display flag
			}
		}
		else
		{
			++m_nSkipped;											// will be ignored
			strCardNo += " ~";										// set display flag
		}

	}

	CCSV csv ( '\t' );
	if ( m_pSims->m_nSIMSUserTextNo > 0 )		csv.Add ( strAdno );
	if ( m_pSims->m_nUPNUserTextNo > 0 )		csv.Add ( strUPN );		// 0=not defined, 1=Member, 2-9=Info1-8 
	if ( m_pSims->m_nPIDUserTextNo > 0 )		csv.Add ( strPID );

	strCardNo.TrimLeft ( '0' );										// remove leading 0's may have been added by add new record
	csv.Add ( strCardNo );

	csv.Add ( m_simsFile.GetForename() );
	csv.Add ( m_simsFile.GetSurname() );
	csv.Add ( m_simsFile.GetCardName() );

	if ( m_pSims->m_nDobNo > 0 )				csv.Add ( m_simsFile.GetDOB() );
	if ( m_pSims->m_nYearContactNo > 0 )		csv.Add ( m_simsFile.GetYear() );
	if ( m_pSims->m_nRegContactNo > 0 )			csv.Add ( m_simsFile.GetReg() );

	if ( m_simsFile.IsFSM() == TRUE )			csv.Add ( "Y" );
	else										csv.Add ( "-" );

	csv.Add ( m_simsFile.GetGender() );

	if ( m_pSims->m_bSaveEmail == TRUE )		csv.Add ( m_simsFile.GetEmail() );

	csv.Add ( m_strExceptionComment );

	return ( m_Report.WriteLine ( csv ) );
}

/*********************************************************************/

void CImportSimsFile::AddStatistics()
{
	CString strLine;
	m_Report.WriteLine ( "" );

	strLine.Format ( "%d\t<..>Lines read", m_nRead );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>Lines skipped ~", m_nSkipped );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>New accounts * ( added by '%s' )", m_nAdded,  m_pSims->GetNewIntakeAddedByText() );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>Existing accounts ( located by '%s' )", m_nRead - ( m_nAdded + m_nIgnored + m_nSkipped ), m_pSims->GetLocateByText(m_pSims->m_nLocateBy) );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>Exceptions found", m_nIgnored );
	m_Report.WriteLine ( strLine );
}

/*********************************************************************/

void CImportSimsFile::ShowStatistics()
{
	CImportStatsDlgConfig Config;
	Config.m_nLinesRead = m_nRead;
	Config.m_nLinesIgnored = m_nIgnored + m_nSkipped;
	Config.m_nLinesUsed = m_nUsed;
	Config.m_nRecordsAdded = m_nAdded;
	Config.m_nLinesReadPos = 0;
	Config.m_nLinesIgnoredPos = 1;
	Config.m_nLinesUsedPos = 2;
	Config.m_nRecordsAddedPos = 4;
	Config.m_nLinesInvalidPos = -1;
	Config.m_nRecordsDeletedPos = -1;
	Config.m_nLastLinePos = 4;
	Config.m_strCaption = m_strReportTitle;

	CImportStatsDlg dlg(Config);
	dlg.DoModal();
}

/*********************************************************************/
