/**********************************************************************/
#include "BarcodePluNoTable.h"
//#include "DataManager.h"
//#include "globalfunctions.h"
#include "NodeTypes.h"
#include "BarcodePluNoTable.h"
/**********************************************************************/
#include "PluSearchDlg.h"
/**********************************************************************/

CPluSearchDlg::CPluSearchDlg( int nMode, CWnd* pParent )
	: CSSDialog(CPluSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPluSearchDlg)
	//}}AFX_DATA_INIT
	m_nPluNo = 0;
	m_nBarcodeNo = 0;
	m_nMode = nMode;
}

/**********************************************************************/

void CPluSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluSearchDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RADIO_PLU, m_radioPlu);
	DDX_Control(pDX, IDC_RADIO_BARCODE, m_radioBarcode);

	if ( NODE_BARCODE == m_nMode )
	{
		DDX_IntegerString(pDX, IDC_EDIT_BARCODENO, m_strBarcodeNo);
		DDV_IntegerString(pDX, m_strBarcodeNo, "1", Super.BarcodeEnd() );
	}
	else
	{
		DDX_IntegerString(pDX, IDC_EDIT_PLUNO, m_strPluNo);
		DDV_IntegerString(pDX, m_strPluNo, "1", Super.PluEnd() );
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CPluSearchDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_PLU, OnRadioPlu)
	ON_BN_CLICKED(IDC_RADIO_BARCODE, OnRadioBarcode)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO,OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO,OnKillFocusBarcodeNo)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluSearchDlg::OnInitDialog() 
{
	::FormatPluNo( m_nPluNo, m_strPluNo );
	::FormatBarcodeNo( m_nBarcodeNo, m_strBarcodeNo );

	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_BARCODENO, SS_NUM, Super.MaxBarcodeLen(), "%s" );

	switch( m_nMode )
	{
	case NODE_BARCODE:
		OnRadioBarcode();
		break;

	default:
		OnRadioPlu();
		break;
	}

	return FALSE;  
}

/**********************************************************************/
CEdit* CPluSearchDlg::GetEditPlu(){ return GetEdit( IDC_EDIT_PLUNO ); }
CEdit* CPluSearchDlg::GetEditBarcode(){ return GetEdit( IDC_EDIT_BARCODENO ); }
/**********************************************************************/

void CPluSearchDlg::OnRadioPlu()
{
	m_radioPlu.SetCheck( TRUE );
	m_radioBarcode.SetCheck( FALSE );
	GetEditPlu() -> EnableWindow( TRUE );
	GetEditBarcode() -> EnableWindow( FALSE );
	m_nMode = NODE_PLU;

	GetEditPlu() -> SetFocus();
	GetEditPlu() -> SetSel( 0, -1 );
}

/**********************************************************************/

void CPluSearchDlg::OnRadioBarcode()
{
	m_radioPlu.SetCheck( FALSE );
	m_radioBarcode.SetCheck( TRUE );
	GetEditPlu() -> EnableWindow( FALSE );
	GetEditBarcode() -> EnableWindow( TRUE );
	m_nMode = NODE_BARCODE;

	GetEditBarcode() -> SetFocus();
	GetEditBarcode() -> SetSel( 0, -1 );
}

/**********************************************************************/

void CPluSearchDlg::OnKillFocusPluNo()
{
	UpdateData( TRUE, FALSE );
	m_strPluNo = BarcodePluNoTable.ConvertPlu( m_strPluNo );
	GetEditPlu() -> SetWindowText( m_strPluNo );
}

/**********************************************************************/

void CPluSearchDlg::OnKillFocusBarcodeNo()
{
	UpdateData( TRUE, FALSE );
	m_strBarcodeNo = BarcodePluNoTable.ConvertBarcode( m_strBarcodeNo );
	GetEditBarcode() -> SetWindowText( m_strBarcodeNo );
}

/**********************************************************************/

void CPluSearchDlg::OnOK()
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		OnKillFocusPluNo();
		OnKillFocusBarcodeNo();
		m_nPluNo = _atoi64( m_strPluNo );
		m_nBarcodeNo = _atoi64( m_strBarcodeNo );
		EndDialog( IDOK );
	}
}

/**********************************************************************/
