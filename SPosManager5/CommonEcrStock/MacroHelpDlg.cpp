/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "MacroHelpDlg.h"
/**********************************************************************/

CMacroHelpDlg::CMacroHelpDlg( CMacroHelpDlg** pParentToThis, CWnd* pParent )
	: CSSDialog(CMacroHelpDlg::IDD, pParent)
{
	m_pParentToThis = pParentToThis;
	*m_pParentToThis = this;
	m_nTabPage = 0;
	
	m_arrayMacro.Add("Fn,Function");
	m_arrayMacro.Add("Mn,Macro");
	m_arrayMacro.Add("Wn,Preset Server");
	m_arrayMacro.Add("Zn,Modifier");
	m_arrayMacro.Add("Cn,Control");
	m_arrayMacro.Add("Ln,List");
	m_arrayMacro.Add("Yn,Payment Method");
	m_arrayMacro.Add("n,Number");
	m_arrayMacro.Add("XC,Clear");
	m_arrayMacro.Add("XY,Yes");
	m_arrayMacro.Add("XN,No");
	m_arrayMacro.Add("XE,Enter");
	m_arrayMacro.Add("XO,OK");
	m_arrayMacro.Add("Q,Clear / Quit");

	m_arrayControl.Add("1,Next Page");
	m_arrayControl.Add("2,Previous Page");
	m_arrayControl.Add("4,Exit List");
	m_arrayControl.Add("5,List Enter");
	m_arrayControl.Add("6,List Page 1");
	m_arrayControl.Add("7,Go to List Page");
	m_arrayControl.Add("10,Exit Button");
	m_arrayControl.Add("20,Home Button");
	m_arrayControl.Add("30,Message Box left Button");
	m_arrayControl.Add("31,Message Box middle Button");
	m_arrayControl.Add("32,Message Box right Button");
	m_arrayControl.Add("50,Beep off in Macro");
	m_arrayControl.Add("51,Beep on in Macro");
	m_arrayControl.Add("52,Beep once in Macro");
	m_arrayControl.Add("60,EXIT button disable");
	m_arrayControl.Add("61,EXIT button enable");
	m_arrayControl.Add("70,Restore keypad data");
	m_arrayControl.Add("71,Restore keypad data (by character)");
	m_arrayControl.Add("80,Recalculate MixMatch");
}

/**********************************************************************/

CMacroHelpDlg::~CMacroHelpDlg()
{
}

/**********************************************************************/

void CMacroHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SELECT, m_TabSelect);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CMacroHelpDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose )
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SELECT, OnTcnSelchangeTabs)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_MACRO, OnGetDispInfoListMacro )
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_FUNCTION, OnGetDispInfoListFunction)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_CONTROL, OnGetDispInfoListControl)
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CMacroHelpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_TabSelect.InsertItem(0, "Macro");
	m_TabSelect.InsertItem(1, "Functions");
	m_TabSelect.InsertItem(2, "Control Operations");

	PrepareList(m_listMacro, IDC_LIST_MACRO, m_arrayMacro.GetSize(), "Tag", "Usage", 100, 255);
	PrepareList(m_listFunction, IDC_LIST_FUNCTION, m_FunctionAuthTable.GetMacroFunctionCount(), "No", "Function", 50, 295);
	PrepareList(m_listControl, IDC_LIST_CONTROL, m_arrayControl.GetSize(), "No", "Operation", 50, 305);

	SelectTab();
	CenterWindow();

	return FALSE;
}

/**********************************************************************/

void CMacroHelpDlg::PrepareList(CSSListCtrlVirtual& list, int nID, int nCount, CString strColumn1, CString strColumn2, int nColWidth1, int nColWidth2)
{
	list.SubclassDlgItem(nID, this);
	MoveControl(&list, 7, 54, 257, 335);

	list.SetFixedForegroundColour(0x01);
	list.SetFixedBackgroundColour(0xFFFF00);
	list.SetBoldMode(TRUE);
	list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	list.InsertColumn(0, strColumn1, LVCFMT_LEFT, nColWidth1);
	list.InsertColumn(1, strColumn2, LVCFMT_LEFT, nColWidth2);
	list.SetItemCountEx(nCount);
	list.SetFocus();
	list.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	list.SetSelectionMark(0);
	list.SetCurSel(0);
	list.Invalidate();
}

/**********************************************************************/

void CMacroHelpDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
	switch( nIDCtl )
	{
	case IDC_LIST_MACRO:
		m_listMacro.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;

	case IDC_LIST_FUNCTION:
		m_listFunction.AdjustRowHeight(lpMeasureItemStruct, 10);
		break;

	case IDC_LIST_CONTROL:
		m_listControl.AdjustRowHeight(lpMeasureItemStruct, 10);
		break;
	}
}

/**********************************************************************/

void CMacroHelpDlg::OnTcnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	m_nTabPage = m_TabSelect.GetCurSel();
	if ((m_nTabPage < 0) || (m_nTabPage > 2))
	{
		m_nTabPage = 0;
	}

	SelectTab();
}

/**********************************************************************/

void CMacroHelpDlg::SelectTab()
{
	ShowAndEnableWindow(&m_listMacro, (0 == m_nTabPage));
	ShowAndEnableWindow(&m_listFunction, (1 == m_nTabPage));
	ShowAndEnableWindow(&m_listControl, (2 == m_nTabPage));
}

/**********************************************************************/

void CMacroHelpDlg::OnGetDispInfoListMacro(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ((nIndex >= 0) && (nIndex < m_arrayMacro.GetSize()))
		{
			CCSV csv(m_arrayMacro.GetAt(nIndex));
			lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
		}
	}
	*pResult = 0;
}

/**********************************************************************/

void CMacroHelpDlg::OnGetDispInfoListFunction(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;

		if ((nIndex >= 0) && (nIndex < m_FunctionAuthTable.GetMacroFunctionCount()))
		{
			CFunctionAuthMiniInfo infoMini;
			m_FunctionAuthTable.GetFunction(nIndex, infoMini);

			CCSV csv;
			csv.Add(infoMini.m_nFuncNo);
			csv.Add(infoMini.m_strName);

			lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
		}
	}
	*pResult = 0;
}

/**********************************************************************/

void CMacroHelpDlg::OnGetDispInfoListControl(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;

		if ((nIndex >= 0) && (nIndex < m_arrayControl.GetSize()))
		{
			CCSV csv(m_arrayControl.GetAt(nIndex));
			lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
		}
	}
	*pResult = 0;
}

/**********************************************************************/

void CMacroHelpDlg::OnButtonClose()
{
	DestroyWindow();
}

/**********************************************************************/

void CMacroHelpDlg::OnOK()
{
	DestroyWindow();
}

/**********************************************************************/

void CMacroHelpDlg::OnCancel()
{
	DestroyWindow();
}

/**********************************************************************/

void CMacroHelpDlg::PostNcDestroy() 
{
	*m_pParentToThis = NULL;
	delete this;
}

/**********************************************************************/
