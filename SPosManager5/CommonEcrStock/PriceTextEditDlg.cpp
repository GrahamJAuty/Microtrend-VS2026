/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "PriceTextCSVArray.h"
/**********************************************************************/
#include "PriceTextEditDlg.h"
/**********************************************************************/

CPriceTextEditDlg::CPriceTextEditDlg(CWnd* pParent)
	: CSSDialog(CPriceTextEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPriceTextEditDlg)
	//}}AFX_DATA_INIT
	m_editTextID[0] = IDC_EDIT_TEXT1;
	m_editTextID[1] = IDC_EDIT_TEXT2;
	m_editTextID[2] = IDC_EDIT_TEXT3;
	m_editTextID[3] = IDC_EDIT_TEXT4;
	m_editTextID[4] = IDC_EDIT_TEXT5;
	m_editTextID[5] = IDC_EDIT_TEXT6;
	m_editTextID[6] = IDC_EDIT_TEXT7;
	m_editTextID[7] = IDC_EDIT_TEXT8;
	m_editTextID[8] = IDC_EDIT_TEXT9;
	m_editTextID[9] = IDC_EDIT_TEXT10;

	for ( int i = 0; i < 10; i++ )
		m_strText[i] = DataManager.PriceText.GetReportText(i);
}

/**********************************************************************/

void CPriceTextEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPriceTextEditDlg)
	//}}AFX_DATA_MAP

	for ( int i = 0; i < 10; i++ )
	{
		DDX_Text(pDX, m_editTextID[i], m_strText[i]);
		DDV_MaxChars(pDX, m_strText[i], MAX_PRICETEXT_LEN );
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPriceTextEditDlg, CDialog)
	//{{AFX_MSG_MAP(CPriceTextEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPriceTextEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	CString strTitle = "Setup Price Level Texts ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), TRUE ); 
	SetWindowText ( strTitle );

	return TRUE;  
}

/**********************************************************************/

void CPriceTextEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		for ( int i = 0; i < 10; i++ )
			DataManager.PriceText.SetReportText ( i, m_strText[i] );
		
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
