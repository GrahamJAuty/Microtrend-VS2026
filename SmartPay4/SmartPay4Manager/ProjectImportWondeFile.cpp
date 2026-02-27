/*********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/*********************************************************************/
#include "ImportStatsDlg.h"
/*********************************************************************/
#include "..\SmartPay4ManagerBgnd\ImportWondeFile.h"
/*********************************************************************/
static const char* szBLANK = "-";
/*********************************************************************/

CImportWondeFile::CImportWondeFile( CWondeData* pData, CWnd* pParent )
{
	m_pData = pData;
	m_pParent = pParent;
	m_strReportTitle = "Wonde Update File";
	Init();
}

/*********************************************************************/

CString CImportWondeFile::GetProgressTitle()
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
// Show Wonde Imports

bool CImportWondeFile::ShowImports ( const char* szFilename )
{
	m_ImportFlag.SetImportFlag( FALSE );

	m_strImportFilename	= szFilename;
	if ( m_strImportFilename == "" )
		m_strImportFilename	= m_pData->m_strImportFilename;		// use default Wonde import filename if not set

	CString strReportFile = Filenames.GetReportFilename ( m_strReportLabel );
	CString strParamsFile = Filenames.GetReportParamsFilename ( m_strReportLabel );

	CRepmanHandler repman ( m_strReportTitle, m_pParent );

	int nErrorNo;
	if ( m_Report.Open ( strReportFile ) == TRUE )				// open report output file
	{
		m_Report.SetStyle1 ( m_strReportTitle );
		AddColumnText();

		nErrorNo = ProcessFile();								// no import reqd					

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

			nErrorNo = repman.DisplayReport ( strReportFile, strParamsFile, m_strReportLabel );
		}

		repman.DisplayError ( nErrorNo );
		return TRUE;
	}
	else
		repman.DisplayMessage ( m_strError, MB_ICONEXCLAMATION );

	return FALSE;
}

/*********************************************************************/

void CImportWondeFile::AddColumnText()
{
	m_Report.AddColumn ( "UserID", TA_LEFT, 210 );
	m_Report.AddColumn ( "MIS_ID", TA_LEFT, 180 );
	m_Report.AddColumn ( "Type", TA_LEFT, 180 );
	m_Report.AddColumn ( "UserName", TA_LEFT, 300 );
	m_Report.AddColumn ( "Forename", TA_LEFT, 210 );
	m_Report.AddColumn ( "Surname", TA_LEFT, 210 );

	if ( m_pData->m_bSavePupilEmail == TRUE || m_pData->m_bSaveStaffEmail == TRUE )
	{
		m_Report.AddColumn ( "Email", TA_LEFT, 350 );
		m_Report.AddColumn ( "Email2", TA_LEFT, 350 );
	}
	m_Report.AddColumn ( "AdNo", TA_LEFT, 180 );
	m_Report.AddColumn ( "UPN", TA_LEFT, 300 );
	m_Report.AddColumn ( "DOB", TA_LEFT, 219 );
	m_Report.AddColumn ( "Year", TA_LEFT, 200 );
	m_Report.AddColumn ( "Reg", TA_LEFT, 200 );
	m_Report.AddColumn ( "House", TA_LEFT, 200 );
	m_Report.AddColumn ( "FSM", TA_CENTER, 120 );
	m_Report.AddColumn ( "Gender", TA_LEFT, 180 );
	m_Report.AddColumn ( "Expiry", TA_LEFT, 219 );
	m_Report.AddColumn ( "Dietary", TA_LEFT, 400 );
	m_Report.AddColumn ( "", TA_LEFT, 630 );
}

/*********************************************************************/

bool CImportWondeFile::ShowImportLine ( const char* szUserID )
{
	CString strUserID = szUserID;

	if ( strUserID == "" )
	{
		++m_nSkipped;
		strUserID  += " ~";											// set display flag
	}
	else
	{
		if ( ::IsStringNumeric ( strUserID ) == TRUE )
		{
			if ( m_wondeDbase.GetTypeNo() == nWONDETYPE_STAFF && m_pData->m_bDisableStaffImport == TRUE )
			{
				++m_nSkipped;
				strUserID  += " ~";									// set display flag
			}
			else
			{
				bool bOk = TRUE;
				int nError = 0;

				CSQLRepositoryAccount RepoAccount;
				bool bExists = (RepoAccount.RowExists(_atoi64(strUserID), NULL).GetSQLResult() != 0);

				if ( bExists == FALSE && m_nImportMethod == nIMPORT_METHOD_EXISTONLY )	// see if must exist
					bOk = FALSE;

				else if ( bExists == TRUE && m_nImportMethod == nIMPORT_METHOD_NEWONLY )// only new recod allowed
					bOk = FALSE;
	
				if ( bOk == TRUE )
				{
					++m_nUsed;											// inc number of valid lines 
					if ( bExists == FALSE )
					{
						++m_nAdded;										// will be new record
						strUserID  += " *";								// not in database - set display flag
					}
				}
				else
				{
					++m_nSkipped;										// will be ignored
					strUserID  += " ~";									// set display flag
				}
			}
		}
		else
		{
			++m_nIgnored;												// ignore
			strUserID += " !!";											// not numeric
		}
	}

	CCSV csv ( '\t' );
	AddReportField ( &csv, strUserID );
	AddReportField ( &csv, m_wondeDbase.GetMISID() );
	AddReportField ( &csv, m_wondeDbase.GetType() );				// Pupil / Staff
	AddReportField ( &csv, m_wondeDbase.GetUserName() );
	AddReportField ( &csv, m_wondeDbase.GetForename() );
	AddReportField ( &csv, m_wondeDbase.GetSurname() );

	if ( m_pData->m_bSavePupilEmail == TRUE || m_pData->m_bSaveStaffEmail == TRUE )
	{
		AddReportField ( &csv, m_wondeDbase.GetEmail() );
		AddReportField ( &csv, m_wondeDbase.GetEmail2() );
	}

	AddReportField ( &csv, m_wondeDbase.GetAdNo() );
	AddReportField ( &csv, m_wondeDbase.GetUPN() );
	AddReportField ( &csv, m_wondeDbase.GetDOB() );
	AddReportField ( &csv, m_wondeDbase.GetYear() );
	AddReportField ( &csv, m_wondeDbase.GetReg() );
	AddReportField ( &csv, m_wondeDbase.GetHouse() );

	if ( m_wondeDbase.IsFSM() == TRUE )		csv.Add ( "Y" );
	else										csv.Add ( szBLANK );

	AddReportField ( &csv, m_wondeDbase.GetGender() );
	AddReportField ( &csv, m_wondeDbase.GetOutDate() );			// expiry
	AddReportField ( &csv, m_wondeDbase.GetDietaryNeeds() );	// expiry

	csv.Add ( m_strExceptionComment );

	return ( m_Report.WriteLine ( csv ) );
}

/*********************************************************************/

void CImportWondeFile::AddReportField ( CCSV* pCsv, const char* szData )
{
	CString strField = szData;
	if ( strField == "" )	pCsv->Add ( szBLANK );
	else					pCsv->Add ( strField );
}

/*********************************************************************/

void CImportWondeFile::AddStatistics()
{
	CString strLine;
	m_Report.WriteLine ( "" );

	strLine.Format ( "%d\t<..>Lines read", m_nRead );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>Lines skipped ( ~ )", m_nSkipped );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>New accounts added ( * )", m_nAdded );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>Existing accounts updated", m_nRead - ( m_nAdded + m_nIgnored + m_nSkipped ) );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>Exceptions found ( !! )", m_nIgnored );
	m_Report.WriteLine ( strLine );
}

/*********************************************************************/

void CImportWondeFile::ShowStatistics()
{
	CImportStatsDlgConfig Config;
	Config.m_nLinesRead = m_nRead;
	Config.m_nLinesIgnored = m_nIgnored;
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

	CImportStatsDlg dlg(Config, m_pParent);
	dlg.DoModal();
}

/*********************************************************************/
