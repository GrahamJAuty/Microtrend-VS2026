/**********************************************************************/
 
/**********************************************************************/
 
/**********************************************************************/
#include "BarcodeTableDefaultsDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CBarcodeTableDefaultsDlg, CDialog)
/**********************************************************************/

CBarcodeTableDefaultsDlg::CBarcodeTableDefaultsDlg(CWnd* pParent)
	: CDialog(CBarcodeTableDefaultsDlg::IDD, pParent)
{
	m_nBarcodeAction = 0;
	m_nPluAction = 0;
}

/**********************************************************************/

CBarcodeTableDefaultsDlg::~CBarcodeTableDefaultsDlg()
{
}

/**********************************************************************/

void CBarcodeTableDefaultsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BARCODE, m_comboBarcode);
	DDX_Control(pDX, IDC_COMBO_PLU, m_comboPlu);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CBarcodeTableDefaultsDlg, CDialog)
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CBarcodeTableDefaultsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_comboBarcode.AddString( "No Change" );
	m_comboBarcode.AddString( "Restore Defaults" );
	m_comboBarcode.SetCurSel(0);

	m_comboPlu.AddString( "No Change" );
	m_comboPlu.AddString( "Restore Defaults" );
	m_comboPlu.AddString( "Copy Barcode Settings" );
	m_comboPlu.SetCurSel(0);

	return TRUE;
}

/**********************************************************************/

void CBarcodeTableDefaultsDlg::OnOK()
{
	m_nBarcodeAction = m_comboBarcode.GetCurSel();
	m_nPluAction = m_comboPlu.GetCurSel();
	EndDialog( IDOK );
}

/**********************************************************************/
