//$$******************************************************************
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
//$$******************************************************************
#include "ImportStatsDlg.h"
//$$******************************************************************

CImportStatsDlg::CImportStatsDlg(CWnd* pParent)
	: CSSDialog(CImportStatsDlg::IDD, pParent)
{
	m_nLinesRead = 0;
	m_nLinesInvalid = 0;
	m_nLinesIgnored = 0;
	m_nLinesUsed = 0;
	m_nRecordsAdded = 0;
	m_strExceptionFilename = "";
}

//$$******************************************************************

void CImportStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_EXCEPTION, m_buttonException);
	DDX_Control(pDX, IDC_EDIT_LINESREAD, m_editLinesRead);
	DDX_Control(pDX, IDC_EDIT_LINESINVALID, m_editLinesInvalid);
	DDX_Control(pDX, IDC_EDIT_LINESIGNORED, m_editLinesIgnored);
	DDX_Control(pDX, IDC_EDIT_LINESUSED, m_editLinesUsed);
	DDX_Control(pDX, IDC_EDIT_RECORDSADDED, m_editRecordsAdded);
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CImportStatsDlg, CSSDialog)
	ON_BN_CLICKED(IDC_BUTTON_EXCEPTION, OnButtonException)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CImportStatsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetEditBoxInt(m_editLinesRead, m_nLinesRead);
	SetEditBoxInt(m_editLinesInvalid, m_nLinesInvalid);
	SetEditBoxInt(m_editLinesIgnored, m_nLinesIgnored);
	SetEditBoxInt(m_editLinesUsed, m_nLinesUsed);
	SetEditBoxInt(m_editRecordsAdded, m_nRecordsAdded);

	if (m_nLinesInvalid == 0)
	{
		m_buttonException.ShowWindow(SW_HIDE);
	}

	return TRUE;
}

//*******************************************************************

void CImportStatsDlg::OnButtonException()
{
	CSSFile file;
	if ( file.Open ( m_strExceptionFilename, "rb" ) == TRUE )
	{
		CString strReportName = Filenames.GetReportFilename ( "R901" );

		CReportFile report;
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
			strLine.Format ( "<..>Number of invalid lines found = %d",  m_nLinesInvalid );
			report.WriteLine ( strLine );

			report.Close();
			file.Close();

			CRepmanHandler repman ( strTitle, this );
			int nErrorNo = repman.DisplayReport( strReportName, "" );
			repman.DisplayError ( nErrorNo );
		}
	}
}

//$$******************************************************************
