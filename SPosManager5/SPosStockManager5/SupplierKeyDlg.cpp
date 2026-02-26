/**********************************************************************/
#include "..\CommonStockTray\CustomField.h"
#include "GlobalFunctions.h"
#include "..\CommonStockTray\SupplierCSVArray.h"
/**********************************************************************/
#include "SupplierKeyDlg.h"
/**********************************************************************/

CSupplierKeyDlg::CSupplierKeyDlg( bool bAdd, bool bCustomField, CWnd* pParent )
	: CDialog(CSupplierKeyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSupplierKeyDlg)
	//}}AFX_DATA_INIT
	m_bAdd = bAdd;
	m_bCustomField = bCustomField;
}

/**********************************************************************/

void CSupplierKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSupplierKeyDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_KEY, m_strKey);

	if ( FALSE == m_bCustomField )
		DDV_MaxChars(pDX, m_strKey, Supplier::SupplierKey.Max);
	else
		DDV_MaxChars(pDX, m_strKey, CustomFieldValue::Key.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSupplierKeyDlg, CDialog)
	//{{AFX_MSG_MAP(CSupplierKeyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSupplierKeyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( TRUE == m_bAdd )
	{
		if ( TRUE == m_bCustomField )
			SetWindowText( "Add Custom Field Value" );
		else
			SetWindowText( "Add Supplier" );
	}
	else
	{
		if ( TRUE == m_bCustomField )
			SetWindowText( "Edit Custom Field Value Key" );
		else
			SetWindowText( "Edit Supplier Key" );
	}

	return TRUE;  
}

/**********************************************************************/

void CSupplierKeyDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		::TrimSpaces ( m_strKey, FALSE );

		if ( m_strKey == "" )
			EndDialog ( IDCANCEL );
		else
			EndDialog ( IDOK );
	}
}

/**********************************************************************/

