/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSReportAccountFind.h"
/**********************************************************************/
#include "PMSAccountFindReportDlg.h"
/**********************************************************************/

CPMSAccountFindReportDlg::CPMSAccountFindReportDlg( CArray<CPMSAccountFindInfo,CPMSAccountFindInfo>& arrayInfo, CArray<int,int>& arrayFilter, bool bUseFilter, int nType, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPMSAccountFindReportDlg::IDD, pParent), 
	m_arrayInfo( arrayInfo ), 
	m_arrayFilter( arrayFilter )
{
	//{{AFX_DATA_INIT(CPMSAccountFindReportDlg)
	//}}AFX_DATA_INIT
	m_bUseFilter = bUseFilter;
	m_nType = nType;
	SetDefaults();

	m_nSubType = 0;
	m_bAllowInvoiceSort = FALSE;

	switch( m_nType )
	{
	case IDM_PMSMENUBAR_FIND_BOOKING:	
		m_nSubType = 1;
		m_bAllowInvoiceSort = FALSE;
		break;	

	case IDM_PMSMENUBAR_CHECK_IN:				
		m_nSubType = 2;
		m_bAllowInvoiceSort = FALSE;
		break;

	case IDM_PMSMENUBAR_CHECK_OUT_ACTIVE:	
		m_nSubType = 3;
		m_bAllowInvoiceSort = TRUE;
		break;

	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT:		
		m_nSubType = 4;	
		m_bAllowInvoiceSort = TRUE;
		break;
		
	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT:	
		m_nSubType = 5;
		m_bAllowInvoiceSort = TRUE;
		break;
	
	case IDM_PMSMENUBAR_GUEST_ACCOUNTS:				
		m_nSubType = 6;	
		m_bAllowInvoiceSort = TRUE;
		break;

	case IDM_PMSMENUBAR_BATCH_INVOICE:				
		m_nSubType = 7;	
		m_bAllowInvoiceSort = TRUE;
		break;

	case IDM_PMSMENUBAR_CANCELLED_BOOKING:
		m_nSubType = 8;
		m_bAllowInvoiceSort = FALSE;
		break;
	}
}

/**********************************************************************/

void CPMSAccountFindReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSAccountFindReportDlg)
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO_SORT, m_nSortType);
	DDX_Check(pDX, IDC_CHECK_BALANCE, m_bBalance);
	DDX_Check(pDX, IDC_CHECK_LINES, m_bLines);
	DDX_Check(pDX, IDC_CHECK_ROOM, m_bRoom);
	DDX_Control(pDX, IDC_RADIO4, m_radioInvoice);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSAccountFindReportDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSAccountFindReportDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSAccountFindReportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strBuffer;
	bool bGotFile = FALSE;

	if ( FALSE == m_bAllowInvoiceSort )
	{
		m_radioInvoice.EnableWindow( FALSE );
		m_radioInvoice.ShowWindow( SW_HIDE );
	}

	CSSFile fileConfig;
	CFilenameUpdater FnUp( SysFiles::PMSAccountFindDef, 0, m_nSubType );
	if ( fileConfig.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		if ( fileConfig.ReadString ( strBuffer ) == TRUE )
		{
			bGotFile = TRUE;
			
			CCSVPMSAccountFindKey csv ( strBuffer );

			m_nSortType = csv.GetSortType();
			m_bRoom = csv.GetRoomFlag();
			m_bBalance = csv.GetBalanceFlag();
			m_bLines = csv.GetLinesFlag();

			int nMaxSortType = ( TRUE == m_bAllowInvoiceSort ) ? 3 : 2;
			
			if ( ( m_nSortType < 0 ) || ( m_nSortType > nMaxSortType ) )
				m_nSortType = 0;
			
			UpdateData ( FALSE );
		}
	}
		
	if ( bGotFile == FALSE )
		OnButtonDefault();

	return TRUE;  
}

/**********************************************************************/

void CPMSAccountFindReportDlg::SetDefaults()
{
	m_nSortType = 0;
	m_bRoom = FALSE;
	m_bBalance = FALSE;
	m_bLines = FALSE;
}

/**********************************************************************/

void CPMSAccountFindReportDlg::OnButtonDefault() 
{
	SetDefaults();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPMSAccountFindReportDlg::OnButtonSave() 
{
	UpdateData( TRUE, FALSE );

	CSSFile fileConfig;
	CFilenameUpdater FnUp( SysFiles::PMSAccountFindDef, 0, m_nSubType );
	if ( fileConfig.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		fileConfig.WriteLine ( GetCSVLine() );
}

/**********************************************************************/

const char* CPMSAccountFindReportDlg::GetCSVLine()
{
	CCSV csv;	
	csv.Add( m_nSortType );
	csv.Add( m_bRoom );
	csv.Add( m_bBalance );
	csv.Add( m_bLines );
	m_strFieldSelect = csv.GetLine();
	return m_strFieldSelect;
}

/**********************************************************************/

void CPMSAccountFindReportDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CPMSReportAccountFind Report( m_arrayInfo, m_arrayFilter, m_bUseFilter );
		Report.SetSortType( m_nSortType );
		Report.SetRoomFlag( m_bRoom != 0 );
		Report.SetBalanceFlag( m_bBalance != 0 );
		Report.SetLinesFlag( m_bLines != 0 );
		Report.SetInvoiceNumFlag( ( m_bAllowInvoiceSort ) || ( 3 == m_nSortType ) );
		Report.SortAccounts();
		Report.CreateReport();
	}
}

/**********************************************************************/
#endif
/**********************************************************************/


