//$$******************************************************************
#include "SelectImportTypeDlg.h"
//$$******************************************************************

CSelectImportTypeDlg::CSelectImportTypeDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSelectImportTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectImportTypeDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CSelectImportTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectImportTypeDlg)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSelectImportTypeDlg, CSSAutoShutdownDialog)
	//{{AFX_MSG_MAP(CSelectImportTypeDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSelectImportTypeDlg::CSSAutoShutdownPostInitDialog()
{
	m_listImports.SubclassDlgItem ( IDC_LIST, this );
	m_listImports.InsertColumn ( 0, "", LVCFMT_LEFT, 200 );
	m_listImports.ModifyStyle( LVS_OWNERDRAWFIXED, 0, 0 );
	m_listImports.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	FillListBox();

	m_listImports.SetCurSel(0);
	m_listImports.SetFocus();
	m_listImports.Invalidate();
	
	return FALSE;  
}

/**********************************************************************/

void CSelectImportTypeDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listImports.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

void CSelectImportTypeDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnOK();
	*pResult = 0;
}

/**********************************************************************/

void CSelectImportTypeDlg::FillListBox()
{
	m_listImports.DeleteAllItems();

	CStringArray arrayImports;
	arrayImports.Add("Import via User ID");
	arrayImports.Add("Import via Member ID");

	for (int n = 0; n < arrayImports.GetSize(); n++)
	{
		m_listImports.InsertItem(n, "");
		m_listImports.SetItem(n, 0, LVIF_TEXT, arrayImports.GetAt(n), 0, 0, 0, NULL);
	}
}

/**********************************************************************/

void CSelectImportTypeDlg::OnOK()
{
	m_nSel = m_listImports.GetCurSel();

	if ((m_nSel < 0) || (m_nSel > 1))
	{
		m_nSel = 0;
	}

	EndDialog ( IDOK );
}

/**********************************************************************/

void CSelectImportTypeDlg::OnCancel()
{
	EndDialog ( IDCANCEL );
}

/**********************************************************************/
