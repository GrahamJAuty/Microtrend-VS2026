/**********************************************************************/
#include "DefEditDlg.h"
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
	m_pSearchDlg = NULL;
	m_bSearchPrevious = FALSE;
	m_strSearchText = "";
	m_nSearchColumn = -1;
	m_strSearchPrompt = "";
}

/**********************************************************************/

CListDataDlg::~CListDataDlg()
{
	DeleteSearchDlg();
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
	DDX_Control(pDX, IDC_BUTTON_HIDDEN1, m_buttonHidden1);
	DDX_Control(pDX, IDC_BUTTON_HIDDEN2, m_buttonHidden2);
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
	ON_BN_CLICKED(IDC_BUTTON_HIDDEN1, OnButtonHidden1)
	ON_BN_CLICKED(IDC_BUTTON_HIDDEN2, OnButtonHidden2)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillFocusList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetFocusList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHeaderClicked)
	ON_NOTIFY(HDN_ITEMCLICKW, 0, OnHeaderClicked)
	ON_MESSAGE(WM_APP, OnSearchMessage)
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

void CListDataDlg::DeleteSearchDlg()
{
	if (m_pSearchDlg != NULL)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
}

/**********************************************************************/

void CListDataDlg::OnButton1() 
{ 
	DeleteSearchDlg();
	HandleButton1();	
}

/**********************************************************************/

void CListDataDlg::OnButton2()
{
	DeleteSearchDlg();
	HandleButton2();
}

/**********************************************************************/

void CListDataDlg::OnButton3()
{
	DeleteSearchDlg();
	HandleButton3();
}

/**********************************************************************/

void CListDataDlg::OnButton4()
{
	DeleteSearchDlg();
	HandleButton4();
}

/**********************************************************************/

void CListDataDlg::OnButton5()
{
	DeleteSearchDlg();
	HandleButton5();
}

/**********************************************************************/

void CListDataDlg::OnButton6()
{
	DeleteSearchDlg();
	HandleButton6();
}

/**********************************************************************/

void CListDataDlg::OnButtonHidden1()
{
	DeleteSearchDlg();
	HandleButtonHidden1();
}

/**********************************************************************/

void CListDataDlg::OnButtonHidden2()
{
	DeleteSearchDlg();
	HandleButtonHidden1();
}

/**********************************************************************/

void CListDataDlg::EnableHiddenButton1(CString strText)
{
	CString strAlt = "&";
	m_buttonHidden1.EnableWindow(TRUE);
	m_buttonHidden1.SetWindowText(strAlt + strText);
}

/**********************************************************************/

void CListDataDlg::EnableHiddenButton2(CString strText)
{
	CString strAlt = "&";
	m_buttonHidden2.EnableWindow(TRUE);
	m_buttonHidden2.SetWindowText(strAlt + strText);
}

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
	DeleteSearchDlg();
	HandleDefaultButton();
	*pResult = 0;
}

/**********************************************************************/

void CListDataDlg::HandleClickHeader(int nColumn)
{
	if ((m_nSearchColumn == nColumn) && ( nColumn >= 0 ) && (GetArraySize() > 1))
	{
		if (m_pSearchDlg == NULL)
		{
			m_pSearchDlg = new CListDataSearchDlg(&m_pSearchDlg, m_strSearchPrompt, this);
			m_pSearchDlg->Create(IDD_DEF_EDIT, this);
		}

		m_pSearchDlg->PostMessage(WM_SETFOCUS);
		m_pSearchDlg->LoadSettings(m_bSearchPrevious, m_strSearchText);
	}
}

/**********************************************************************/

long CListDataDlg::OnSearchMessage(WPARAM w, LPARAM l)
{
	HandleSearch();
	return 0l;
}

/**********************************************************************/

void CListDataDlg::HandleSearch()
{
	if (m_pSearchDlg != NULL)
	{
		CString strTextToFind = m_strSearchText;
		strTextToFind.MakeUpper();

		int nSel = GetSafeSelection();
		bool bFound = FALSE;

		if (FALSE == m_bSearchPrevious)
		{
			for (int nTest = nSel + 1; nTest < GetArraySize(); nTest++)
			{
				if (SearchText(nTest, strTextToFind) == TRUE)
				{
					bFound = TRUE;
					break;
				}
			}
		}
		else
		{
			for (int nTest = nSel - 1; nTest >= 0; nTest--)
			{
				if (SearchText(nTest, strTextToFind) == TRUE)
				{
					bFound = TRUE;
					break;
				}
			}
		}

		if (FALSE == bFound)
		{
			MessageBeep(0);
		}
	}
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

	if ( strMsg != "" )
	{
		if ( Prompter.YesNo( strMsg, FALSE ) != IDYES )
			bResult = FALSE;
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

