/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "PluDeletePrintDlg.h"
/**********************************************************************/

CPluDeletePrintDlg::CPluDeletePrintDlg( bool bDeleteItems, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPluDeletePrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPluDeletePrintDlg)
	//}}AFX_DATA_INIT
	m_bLines = TRUE;
	m_bMarked = TRUE;
	m_bStockCode = TRUE;
	m_bPrice = TRUE;
	m_bEmpty = TRUE;
	/*****/
	m_bDeleteItems = bDeleteItems;
}

/**********************************************************************/

void CPluDeletePrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluDeletePrintDlg)
	DDX_Control(pDX, IDC_CHECK_LINES, m_checkLines);
	DDX_Control(pDX, IDC_CHECK_MARKED, m_checkMarked);
	DDX_Control(pDX, IDC_CHECK_STOCKCODE, m_checkStockCode);
	DDX_Control(pDX, IDC_CHECK_PRICE, m_checkPrice);
	DDX_Control(pDX, IDC_CHECK_EMPTY, m_checkEmpty);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluDeletePrintDlg, CDialog)
	//{{AFX_MSG_MAP(CPluDeletePrintDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluDeletePrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_checkLines.SetCheck( m_bLines != 0 );
	m_checkMarked.SetCheck( m_bMarked != 0 );
	m_checkStockCode.SetCheck( m_bStockCode != 0 );
	m_checkPrice.SetCheck( m_bPrice != 0 );
	m_checkEmpty.SetCheck( m_bEmpty != 0 );

	if ( SysInfo.IsStockSystem() == FALSE )
	{
		m_checkStockCode.ShowWindow( SW_HIDE );
		MoveControl( &m_checkPrice, 7, 45 );
		MoveControl( &m_checkEmpty, 7, 59 );
		MoveControl( &m_buttonSave, 7, 99 );
		MoveControl( &m_buttonOK, 117, 99 );
		MoveControl( &m_buttonCancel, 172, 99 );
		ResizeDialog( 229, 120 );
	}

	if ( TRUE == m_bDeleteItems )
	{
		SetWindowText( "Print Plu Deletion List" );
		m_checkMarked.SetWindowText( "Show items that are not marked for deletion" );
	}
	else
	{
		SetWindowText( "Print Inactive Items List" );
		m_checkMarked.SetWindowText( "Show items that are to be retained as active" );
	}

	return TRUE;  
}

/**********************************************************************/

void CPluDeletePrintDlg::OnButtonSave()
{
	CSSFile fileOptions;
	CFilenameUpdater FnUp( SysFiles::PluDeleteDef );
	if ( fileOptions.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CCSV csv;
		csv.Add( 1 );
		csv.Add( m_checkLines.GetCheck() != 0 );
		csv.Add( m_checkMarked.GetCheck() != 0 );
		csv.Add( m_checkStockCode.GetCheck() != 0 );
		csv.Add( m_checkPrice.GetCheck() != 0 );
		csv.Add( m_checkEmpty.GetCheck() != 0 );
		fileOptions.WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/

void CPluDeletePrintDlg::OnOK() 
{
	m_bLines = ( m_checkLines.GetCheck() != 0 );
	m_bMarked = ( m_checkMarked.GetCheck() != 0 );
	m_bStockCode = ( m_checkStockCode.GetCheck() != 0 );
	m_bPrice = ( m_checkPrice.GetCheck() != 0 );
	m_bEmpty = ( m_checkEmpty.GetCheck() != 0 );
	EndDialog( IDOK );
}

/**********************************************************************/


