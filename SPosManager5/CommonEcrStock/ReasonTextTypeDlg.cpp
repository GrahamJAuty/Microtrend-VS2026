/**********************************************************************/
#include "ReasonTextTypeDlg.h"
/**********************************************************************/

CReasonTextTypeDlg::CReasonTextTypeDlg(CWnd* pParent /*=NULL*/)
	: CSSDialog(CReasonTextTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReasonTextTypeDlg)
	//}}AFX_DATA_INIT
	m_nSel = 0;
}

/**********************************************************************/

void CReasonTextTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReasonTextTypeDlg)
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CReasonTextTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CReasonTextTypeDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CReasonTextTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText("Edit Reason Texts");

	m_listTextTypes.SubclassDlgItem ( IDC_LIST, this );
	m_listTextTypes.InsertColumn ( 0, "", LVCFMT_LEFT, 200 );
	m_listTextTypes.ModifyStyle( LVS_OWNERDRAWFIXED | LVS_SINGLESEL, 0, 0 );
	m_listTextTypes.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	FillListBox();

	m_listTextTypes.SetCurSel(m_nSel);
	m_listTextTypes.SetFocus();
	m_listTextTypes.Invalidate();

	m_buttonOK.SetWindowText("Continue");
	m_buttonCancel.SetWindowText("Close");
	
	return FALSE;  
}

/**********************************************************************/

void CReasonTextTypeDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listTextTypes.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

void CReasonTextTypeDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();
	*pResult = 0;
}

/**********************************************************************/

void CReasonTextTypeDlg::FillListBox()
{
	m_listTextTypes.DeleteAllItems();

	CStringArray arrayTypes;
	for (int n = 0; n < REASONTEXT_TYPES; n++)
	{
		arrayTypes.Add(CCascadingReasonCSVArray::GetReasonTypeText(n));
	}
	
	for ( int n = 0; n < arrayTypes.GetSize(); n++ )
	{
		m_listTextTypes.InsertItem ( n, "" );
		m_listTextTypes.SetItem ( n, 0, LVIF_TEXT, arrayTypes.GetAt(n), 0, 0, 0, NULL );
	}
}

/**********************************************************************/

void CReasonTextTypeDlg::OnOK()
{
	m_nSel = m_listTextTypes.GetCurSel();

	if ((m_nSel < 0) || (m_nSel >= REASONTEXT_TYPES))
	{
		m_nSel = 0;
	}

	EndDialog(IDOK);
}

/**********************************************************************/

void CReasonTextTypeDlg::OnCancel()
{
	EndDialog ( IDCANCEL );
}

/**********************************************************************/
