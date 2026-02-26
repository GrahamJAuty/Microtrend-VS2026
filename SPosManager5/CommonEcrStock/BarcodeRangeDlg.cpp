/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "BarcodeCSVArray.h"
//#include "globalfunctions.h"
/**********************************************************************/
#include "BarcodeRangeDlg.h"
/**********************************************************************/

CBarcodeRangeDlg::CBarcodeRangeDlg( const char* szTitle, CWnd* pParent )
	: CSSDialog(CBarcodeRangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarcodeRangeDlg)
	//}}AFX_DATA_INIT
	m_strTitle = szTitle;
}

/**********************************************************************/

void CBarcodeRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeRangeDlg)
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_BARCODENO_FROM, m_strBarcodeFrom);
	DDV_IntegerString(pDX, m_strBarcodeFrom, "1", Super.BarcodeEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_BARCODENO_TO, m_strBarcodeTo );
	DDV_IntegerString(pDX, m_strBarcodeTo, "1", Super.BarcodeEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CBarcodeRangeDlg, CDialog)
	//{{AFX_MSG_MAP(CBarcodeRangeDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_FROM, OnKillFocusBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_TO, OnKillFocusBarcodeNo)
	ON_BN_CLICKED(IDC_STATIC_FROM, OnStaticFrom)
	ON_BN_CLICKED(IDC_STATIC_TO, OnStaticTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CBarcodeRangeDlg::GetEditBarcodeFrom() { return GetEdit( IDC_EDIT_BARCODENO_FROM ); }
CEdit* CBarcodeRangeDlg::GetEditBarcodeTo() { return GetEdit( IDC_EDIT_BARCODENO_TO ); }
/**********************************************************************/

BOOL CBarcodeRangeDlg::OnInitDialog() 
{
	::FormatBarcodeNo( m_nBarcodeFrom, m_strBarcodeFrom );
	::FormatBarcodeNo( m_nBarcodeTo, m_strBarcodeTo );

	CDialog::OnInitDialog();	
	SetWindowText ( m_strTitle );
	SubclassEdit ( IDC_EDIT_BARCODENO_FROM, SS_NUM, Super.MaxBarcodeLen(), "%s" );
	SubclassEdit ( IDC_EDIT_BARCODENO_TO, SS_NUM, Super.MaxBarcodeLen(), "%s" );
	return TRUE;  
}

/**********************************************************************/

void CBarcodeRangeDlg::OnKillFocusBarcodeNo() 
{
	UpdateData( TRUE, FALSE );
	m_strBarcodeFrom = BarcodePluNoTable.ConvertBarcode( m_strBarcodeFrom );
	m_strBarcodeTo = BarcodePluNoTable.ConvertBarcode( m_strBarcodeTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CBarcodeRangeDlg::OnStaticFrom() 
{
	CString strStart;
	::FormatBarcodeNo( 1, strStart );
	GetEditBarcodeFrom() -> SetWindowText ( strStart );
}

/**********************************************************************/

void CBarcodeRangeDlg::OnStaticTo() 
{
	CString strEnd;
	::FormatBarcodeNo( Super.MaxBarcodeInt(), strEnd );
	GetEditBarcodeTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CBarcodeRangeDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusBarcodeNo();
		m_nBarcodeFrom = _atoi64( m_strBarcodeFrom );
		m_nBarcodeTo = _atoi64( m_strBarcodeTo );
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

