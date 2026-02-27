//$$******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "ImportStatsDlg.h"
//$$******************************************************************

CImportStatsDlgConfig::CImportStatsDlgConfig()
{
	m_strCaption = "";
	m_strExceptionFilename = "";
	m_nLinesRead = 0;
	m_nLinesInvalid = 0;
	m_nLinesIgnored = 0;
	m_nLinesUsed = 0;
	m_nRecordsAdded = 0;
	m_nRecordsDeleted = 0;
	m_nLinesReadPos = 0;
	m_nLinesInvalidPos = 1;
	m_nLinesIgnoredPos = 2;
	m_nLinesUsedPos = 3;
	m_nRecordsAddedPos = 4;
	m_nRecordsDeletedPos = 5;
	m_nLastLinePos = 5;
}

//$$******************************************************************

CImportStatsDlg::CImportStatsDlg( CImportStatsDlgConfig& config, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CImportStatsDlg::IDD, pParent),
	m_Config(config)
{
}

//$$******************************************************************

void CImportStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LINESREAD, m_editLinesRead);
	DDX_Control(pDX, IDC_EDIT_LINESINVALID, m_editLinesInvalid);
	DDX_Control(pDX, IDC_EDIT_LINESIGNORED, m_editLinesIgnored);
	DDX_Control(pDX, IDC_EDIT_LINESUSED, m_editLinesUsed);
	DDX_Control(pDX, IDC_EDIT_RECORDSADDED, m_editRecordsAdded);
	DDX_Control(pDX, IDC_EDIT_RECORDSDELETED, m_editRecordsDeleted);
	DDX_Control(pDX, IDC_STATIC_LINESREAD, m_staticLinesRead);
	DDX_Control(pDX, IDC_STATIC_LINESINVALID, m_staticLinesInvalid);
	DDX_Control(pDX, IDC_STATIC_LINESIGNORED, m_staticLinesIgnored);
	DDX_Control(pDX, IDC_STATIC_LINESUSED, m_staticLinesUsed);
	DDX_Control(pDX, IDC_STATIC_RECORDSADDED, m_staticRecordsAdded);
	DDX_Control(pDX, IDC_STATIC_RECORDSDELETED, m_staticRecordsDeleted);
	DDX_Control(pDX, IDC_BUTTON_EXCEPTION, m_buttonException);
	DDX_Control(pDX, IDOK, m_buttonOK);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CImportStatsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_EXCEPTION, OnButtonException)
END_MESSAGE_MAP()

//$$******************************************************************

void CImportStatsDlg::ShowStat(int nRow, CStatic& label, CEdit& edit, int nVal)
{
	if ((nRow >= 0) && (nRow <= 5))
	{
		MoveControl(&label, 7, 13 + (22 * nRow));
		MoveControl(&edit, 140, 13 + (22 * nRow));
		SetEditBoxInt(edit, nVal);
	}
	else
	{
		ShowAndEnableWindow(&label, FALSE);
		ShowAndEnableWindow(&edit, FALSE);
	}
}

//$$******************************************************************

BOOL CImportStatsDlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText ( m_Config.m_strCaption );

	{
		bool bExceptions = FALSE;
		if (m_Config.m_strExceptionFilename != "")
		{
			bExceptions = (::FileExists(m_Config.m_strExceptionFilename));
		}

		ShowAndEnableWindow(&m_buttonException, bExceptions);
	}

	ShowStat(m_Config.m_nLinesReadPos, m_staticLinesRead, m_editLinesRead, m_Config.m_nLinesRead);
	ShowStat(m_Config.m_nLinesInvalidPos, m_staticLinesInvalid, m_editLinesInvalid, m_Config.m_nLinesInvalid);
	ShowStat(m_Config.m_nLinesIgnoredPos, m_staticLinesIgnored, m_editLinesIgnored, m_Config.m_nLinesIgnored);
	ShowStat(m_Config.m_nLinesUsedPos, m_staticLinesUsed, m_editLinesUsed, m_Config.m_nLinesUsed);
	ShowStat(m_Config.m_nRecordsAddedPos, m_staticRecordsAdded, m_editRecordsAdded, m_Config.m_nRecordsAdded);
	ShowStat(m_Config.m_nRecordsDeletedPos, m_staticRecordsDeleted, m_editRecordsDeleted, m_Config.m_nRecordsDeleted);
	
	if ((m_Config.m_nLastLinePos >= 0) && (m_Config.m_nLastLinePos < 5))
	{
		MoveControl(&m_buttonException, 7, 48 + ( 22 * m_Config.m_nLastLinePos));
		MoveControl(&m_buttonOK, 190, 48 + (22 * m_Config.m_nLastLinePos));
		ResizeDialog(247, 69 + (22 * m_Config.m_nLastLinePos));
	}

	return TRUE;
}

//$$******************************************************************

void CImportStatsDlg::OnButtonException()
{
	CSSFile file;
	if ( file.Open ( m_Config.m_strExceptionFilename, "rb" ) == TRUE )
	{
		CString strReportName = Filenames.GetReportFilename ( "R901" );

		CReportFileWithEndText report;
		if ( report.Open ( strReportName ) == TRUE )
		{
			CString strTitle = "Database Import Exceptions";

			report.SetStyle1 ( strTitle );
			report.AddColumn ( "LineNo", TA_LEFT, 200 );
			report.AddColumn ( "Data", TA_LEFT, 2000 );

			CCSV csvData;
			while ( file.Read ( csvData ) == TRUE )
			{
				CCSV csvLine ( '\t' );

				csvLine.Add ( csvData.GetString(0) );
				csvLine.Add ( csvData.GetString(1) ); 
				report.WriteLine ( csvLine );
			}

			report.WriteLine ( "" );

			CString strLine;
			strLine.Format ( "<..>Number of invalid lines found = %d",  m_Config.m_nLinesInvalid );
			report.WriteLine ( strLine );

			report.Close();
			file.Close();

			CRepmanHandler repman ( strTitle, this );
			int nErrorNo = repman.DisplayReport ( strReportName );
			repman.DisplayError ( nErrorNo );
		}
	}
}

//$$******************************************************************

