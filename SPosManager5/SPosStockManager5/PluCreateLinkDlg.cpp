/**********************************************************************/
#include "FilterTypes.h"
#include "BarcodePluNoTable.h"
#include "ReportHelpers.h"
#include "StockFilterArray.h"
#include "StockFilterDlg.h"
#include "StockMonitor.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "PluCreateLinkDlg.h"
/**********************************************************************/

CPluCreateLinkDlg::CPluCreateLinkDlg( CPluCSVRecord& PluRecord, CWnd* pParent)
	: CSSDialog(CPluCreateLinkDlg::IDD, pParent), m_PluRecord( PluRecord )
{
	//{{AFX_DATA_INIT(CPluCreateLinkDlg)
	//}}AFX_DATA_INIT
	m_strStockCode = "";
	if ( StockOptions.GetStockAutoCreateFlag() )
		m_strStockCode = PluRecord.GetPluNoString();
}

/**********************************************************************/

void CPluCreateLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluCreateLinkDlg)
	DDX_Control(pDX, IDC_EDIT_STOCKCODE, m_editStockCode);
	DDX_Control(pDX, IDC_STATIC_PLU, m_staticPlu);
	DDX_Control(pDX, IDC_STATIC_DESCRIPTION, m_staticDescription);
	DDX_Control(pDX, IDC_STATIC_DEPT, m_staticDept);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_STOCKCODE, m_strStockCode);
	DDV_MaxChars(pDX, m_strStockCode, Stock::StockCode.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluCreateLinkDlg, CDialog)
	//{{AFX_MSG_MAP(CPluCreateLinkDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluCreateLinkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CReportHelpers ReportHelpers;
	m_staticPlu.SetWindowText( m_PluRecord.GetPluNoString() );
	m_staticDescription.SetWindowText( m_PluRecord.GetReportText() );
	m_staticDept.SetWindowText( DataManager.Department.GetReportTextByDeptNo( m_PluRecord.GetBaseDeptNo()  ) );
	return TRUE;  
}

/**********************************************************************/

void CPluCreateLinkDlg::OnKillFocusStockCode() 
{
	UpdateData( TRUE, FALSE );
	m_strStockCode = BarcodePluNoTable.ConvertStockCode ( m_strStockCode );
	UpdateData( FALSE );
}

/**********************************************************************/

void CPluCreateLinkDlg::OnButtonBrowse() 
{
	CStockFilterArray FilterArray( FILTERTYPE_BROWSE_STOCK );
	CStockFilterDlg dlgBrowse ( FilterArray, this );
		
	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nStockIdx = dlgBrowse.GetStockIdx();
	if ( nStockIdx < 0 )
		return;

	UpdateData( TRUE, FALSE );
	m_strStockCode = DataManager.Stock.GetStockCode ( nStockIdx );
	UpdateData( FALSE );
	
	m_editStockCode.SetSel( 0, -1 );
	m_editStockCode.SetFocus();	
}

/**********************************************************************/

void CPluCreateLinkDlg::OnOK() 
{
	if ( UpdateData ( TRUE, TRUE ) )
	{
		::TrimSpaces ( m_strStockCode, FALSE );
		EndDialog (IDOK);
	}
}

/**********************************************************************/
