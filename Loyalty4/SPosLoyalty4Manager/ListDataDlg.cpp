/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

CListDataDlg::CListDataDlg(CWnd* pParent)
	: CSSDialog(CListDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListDataDlg)
	//}}AFX_DATA_INIT
	m_bWarnCancelAfterDownload = FALSE;
	m_bMultiSelect = TRUE;
	m_bEnableColour = FALSE;
	m_pDisplayHelpers = NULL;
	m_pContentHelpers = NULL;
	m_nListSelection = 0;
}

/**********************************************************************/

void CListDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListDataDlg)
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	DDX_Control(pDX, IDC_BUTTON3, m_button3);
	DDX_Control(pDX, IDC_BUTTON4, m_button4);
	DDX_Control(pDX, IDC_BUTTON5, m_button5);
	DDX_Control(pDX, IDC_BUTTON6, m_button6);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CListDataDlg, CDialog)
	//{{AFX_MSG_MAP(CListDataDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillFocusList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetFocusList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHeaderClicked) 
	ON_NOTIFY(HDN_ITEMCLICKW, 0, OnHeaderClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CListDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	SetTitle();
	SetButtonTexts();
	SetColumns();
	PrepareList();
	return FALSE;  
}

/**********************************************************************/

int CListDataDlg::GetArraySize()
{
	return m_pContentHelpers -> GetArraySize();
}

/**********************************************************************/

void CListDataDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST ) 
   {
	   m_listData.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

void CListDataDlg::PrepareList()
{
	m_pDisplayHelpers -> PrepareDatabaseList( m_bMultiSelect, m_bEnableColour );
}

/**********************************************************************/

void CListDataDlg::SelectLine ( int nIndex )
{
	m_pDisplayHelpers -> SelectDatabaseLine( nIndex );
}

/**********************************************************************/
void CListDataDlg::OnButton1() { HandleButton1(); }
void CListDataDlg::OnButton2() { HandleButton2(); }
void CListDataDlg::OnButton3() { HandleButton3(); }
void CListDataDlg::OnButton4() { HandleButton4(); }
void CListDataDlg::OnButton5() { HandleButton5(); }
void CListDataDlg::OnButton6() { HandleButton6(); }
/**********************************************************************/

int CListDataDlg::GetSafeSelection()
{
	return GetSafeSelection ( 0, GetArraySize() - 1 );
}

/**********************************************************************/

int CListDataDlg::GetSafeSelection( int nMin, int nMax )
{
	int nSel = m_listData.GetCurSel();
	if ((nSel < nMin) || (nSel > nMax) || (nSel >= m_listData.GetItemCount()))
	{
		nSel = -1;
	}
	
	return nSel;
}

/**********************************************************************/

bool CListDataDlg::PrepareLine( int nIndex )
{
	return m_pContentHelpers -> PrepareLine( nIndex, m_strDisplayLine );
}

/**********************************************************************/

void CListDataDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLine(nIndex);
		CCSV csv(m_strDisplayLine, '\t');
		lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));

		if (TRUE == m_bEnableColour)
		{
			m_listData.SetColour(nIndex, m_pContentHelpers->GetListLineColour());
		}
	}
	*pResult = 0;
}

/**********************************************************************/

void CListDataDlg::OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	
	if (phdn->iButton == 0)
	{
		HandleClickHeader(phdn->iItem);
	}

	*pResult = 0;
}

/**********************************************************************/

void CListDataDlg::OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( GetDefaultButtonID() );
	*pResult = 0;
}

/**********************************************************************/

void CListDataDlg::OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	SetDefID ( IDOK );
	*pResult = 0;
}

/**********************************************************************/

void CListDataDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	HandleDefaultButton();
	*pResult = 0;
}

/**********************************************************************/

void CListDataDlg::OnOK() 
{
	if (HandleOK() == TRUE)
	{
		EndDialog(IDOK);
	}
}

/**********************************************************************/

bool CListDataDlg::CheckBeforeCancel( bool bWarnDownload, bool bWarnChange )
{
	bool bResult = TRUE;

	CString strMsg = "";
	
	if ( TRUE == bWarnDownload )
	{
		strMsg += "You have attempted to download changes to an EPOS terminal.\n\n";
		strMsg += "Are you sure you want to cancel these changes in ";
		strMsg += AfxGetAppName();
		strMsg += ".";
	}
	else if ( TRUE == bWarnChange )
	{
		strMsg += "Are you sure you wish to cancel any changes that you\n";
		strMsg += "have made since opening this list.\n";
	}

	if (strMsg != "")
	{
		if (Prompter.YesNo(strMsg, FALSE) != IDYES)
		{
			bResult = FALSE;
		}
	}

	return bResult;
}

/**********************************************************************/

bool CListDataDlg::HandleOK()
{
	m_nListSelection = m_listData.GetCurSel();
	return TRUE;
}

/**********************************************************************/

void CListDataDlg::OnCancel() 
{
	if (HandleCancel())
	{
		EndDialog(IDCANCEL);
	}
}

/**********************************************************************/

