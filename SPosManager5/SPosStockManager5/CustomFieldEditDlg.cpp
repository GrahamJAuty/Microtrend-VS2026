/**********************************************************************/
 
/**********************************************************************/
 
#include "SupplierKeyDlg.h"
/**********************************************************************/
#include "CustomFieldEditDlg.h"
/**********************************************************************/

CCustomFieldEditDlg::CCustomFieldEditDlg( const char* szTitle, int nValueIdx, CWnd* pParent)
	: CSSDialog(CCustomFieldEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomFieldEditDlg)
	m_strKey = _T("");
	//}}AFX_DATA_INIT
	m_nValueIdx = nValueIdx;
	DataManager.GetActiveCustomField() -> GetAt ( m_nValueIdx, m_ValueRecord );

	m_strTitle = szTitle;
	m_strKey = m_ValueRecord.GetKey();
	m_strOldKey = m_strKey;
	m_strName = m_ValueRecord.GetName();
}

/**********************************************************************/

void CCustomFieldEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomFieldEditDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, CustomFieldValue::Name.Max);
	DDX_Text(pDX, IDC_EDIT_REFERENCE, m_strKey);
	DDV_MaxChars(pDX, m_strKey, CustomFieldValue::Key.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCustomFieldEditDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomFieldEditDlg)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCustomFieldEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText ( m_strTitle );
	return TRUE;  
}

/**********************************************************************/

void CCustomFieldEditDlg::OnButtonSet() 
{
	UpdateData( TRUE, FALSE );

	CSupplierKeyDlg dlgKey ( FALSE, TRUE, this );
	dlgKey.m_strKey = m_strKey;
	if ( dlgKey.DoModal() != IDOK )
		return;

	CString strKey = dlgKey.m_strKey;
	if ( strKey == "" )
		return;

	if ( strKey == m_strKey )
		return;

	int nPos;
	if ( DataManager.GetActiveCustomField() -> FindValueByKey ( strKey, nPos ) == TRUE )
	{
		CString strMsg = "You have already created an Custom Field value\nwith key ";	
		strMsg += strKey;
		Prompter.Error ( strMsg );
		return;
	}

	m_strKey = strKey;
	UpdateData ( FALSE );
}

/**********************************************************************/

void CCustomFieldEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_ValueRecord.SetKey ( m_strKey );
		m_ValueRecord.SetName ( m_strName );
		
		if ( m_strKey == m_strOldKey )
			DataManager.GetActiveCustomField() -> SetAt ( m_nValueIdx, m_ValueRecord );
		else
		{
			DataManager.GetActiveCustomField() -> DeleteValue ( m_nValueIdx );
			m_nValueIdx = DataManager.GetActiveCustomField() -> AddValue ( m_ValueRecord );
		}
		
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

