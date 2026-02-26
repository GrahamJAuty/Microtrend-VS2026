/**********************************************************************/
#include "Consolidation.h"
#include "NodeTypes.h"
#include "ReportConsolidationArray.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "SalesNoteDlg.h"
/**********************************************************************/

CSalesNoteDlg::CSalesNoteDlg( int nSpIdx, int nType, CWnd* pParent)
	: CDialog(CSalesNoteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSalesNoteDlg)
	//}}AFX_DATA_INIT

	m_strFilename = "";

	m_nType = nType;
	m_nSpIdx = nSpIdx;
	CString strPath = dbStockpoint.GetFolderPathSession( m_nSpIdx, nType );
	strPath += "\\";
	
	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strPath + "*.*" );

	CReportConsolidationArray<CSortedStringItem> arrayFiles;
	
	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDots() == TRUE )
			continue;

		if ( finder.IsDirectory() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();

		if ( strFilename.GetLength() != 15 )
			continue;

		if ( ::TestNumeric( strFilename.Left(8) ) == FALSE )
			continue;

		if ( ::TestNumeric( strFilename.Right(6) ) == FALSE )
			continue;

		CSortedStringItem item;
		item.m_strItem = strFilename;
		arrayFiles.Consolidate( item );
	}

	for ( int n = arrayFiles.GetSize() - 1; n >= 0; n-- )
	{
		CSortedStringItem item;
		arrayFiles.GetAt( n, item );
		CString strFilename = item.m_strItem;

		int nYear = atoi( strFilename.Left(4) );
		int nMonth = atoi( strFilename.Mid(4,2) );
		int nDay = atoi( strFilename.Mid(6,2) );

		int nHour = atoi( strFilename.Mid(9,2) );
		int nMinute = atoi( strFilename.Mid(11,2) );
		int nSecond = atoi( strFilename.Right(2) );

		COleDateTime time = COleDateTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );
		if ( time.m_status != COleDateTime::valid )
			continue;

		CSSFile fileSession;
		if ( fileSession.Open( strPath + strFilename, "rb" ) == FALSE )
			continue;

		CString strBuffer;
		fileSession.ReadString( strBuffer );
		CCSV csv( strBuffer );

		if ( csv.GetInt(0) != 1 )
			continue;

		m_arraySessions.Add( strFilename );
	}
}

/**********************************************************************/

void CSalesNoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesNoteDlg)
	DDX_Control(pDX, IDC_COMBO_SESSION, m_comboSession);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesNoteDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesNoteDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSalesNoteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
			
	FillSessionCombo();
	
	CString strTitle;
	strTitle.Format ( "Manual Sales (%s)", dbStockpoint.GetName( m_nSpIdx ) );
	SetWindowText ( strTitle );
		
	return TRUE;	
}

/**********************************************************************/

void CSalesNoteDlg::FillSessionCombo()
{
	m_comboSession.ResetContent();
	m_comboSession.AddString( "New Session" );

	for ( int n = 0; n < m_arraySessions.GetSize(); n++ )
	{
		CString strFilename = m_arraySessions.GetAt(n);
		
		CString strComboText;
		strComboText.Format( "%s/%s/%s at %s:%s",
			(const char*) strFilename.Mid(6,2),
			(const char*) strFilename.Mid(4,2),
			(const char*) strFilename.Left(4),
			(const char*) strFilename.Mid(9,2),
			(const char*) strFilename.Mid(11,2) );
		m_comboSession.AddString( strComboText );
	}

	m_comboSession.SetCurSel(0);
}

/**********************************************************************/

void CSalesNoteDlg::OnOK() 
{
	m_strFilename = "";

	int nSel = m_comboSession.GetCurSel();
	if ( ( nSel >= 1 ) && ( nSel <= m_arraySessions.GetSize() ) )
		m_strFilename = m_arraySessions.GetAt( nSel - 1 );

	EndDialog ( IDOK );
}

/**********************************************************************/

