/**********************************************************************/
#include "Resource.h"
/**********************************************************************/
#include "AutoBackupProcessor.h"
/**********************************************************************/
#include "BackupResultDlg.h"
/**********************************************************************/

CBackupResultDlg::CBackupResultDlg( CBackupResultDlg** pParentToThis, CWnd* pParent )
	: CSSDialog(CBackupResultDlg::IDD, pParent)
{
	m_pParentToThis = pParentToThis;
	*m_pParentToThis = this;
	m_bFullSize = FALSE;
}

/**********************************************************************/

CBackupResultDlg::~CBackupResultDlg()
{
}

/**********************************************************************/

void CBackupResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_BUTTON_MORE, m_buttonMore );
	DDX_Control( pDX, IDC_STATIC_NEXT, m_staticNext );
	DDX_Control( pDX, IDCANCEL, m_buttonCancel );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CBackupResultDlg, CDialog)
	ON_NOTIFY( LVN_GETDISPINFO, IDC_LIST_RESULT, OnGetDispInfoListResult )
	ON_NOTIFY( LVN_GETDISPINFO, IDC_LIST_NEXT, OnGetDispInfoListNext )
	ON_BN_CLICKED( IDC_BUTTON_MORE, OnButtonMore )
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CBackupResultDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_listResult.SubclassDlgItem ( IDC_LIST_RESULT, this );
	m_listResult.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listResult.InsertColumn ( 0, "Date", LVCFMT_LEFT, 80 );
	m_listResult.InsertColumn ( 1, "Time", LVCFMT_LEFT, 80 );
	m_listResult.InsertColumn ( 2, "Reason", LVCFMT_LEFT, 180 );
	m_listResult.InsertColumn ( 3, "Result", LVCFMT_LEFT, 360 );
	m_listResult.SetItemCountEx(1);
	m_listResult.SetFocus();
	m_listResult.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	m_listResult.SetSelectionMark(0);
	m_listResult.SetCurSel(0);

	m_listNext.SubclassDlgItem ( IDC_LIST_NEXT, this );
	m_listNext.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listNext.InsertColumn ( 0, "Date", LVCFMT_LEFT, 80 );
	m_listNext.InsertColumn ( 1, "Time", LVCFMT_LEFT, 80 );
	m_listNext.InsertColumn ( 2, "Reason", LVCFMT_LEFT, 180 );
	m_listNext.SetItemCountEx(1);
	m_listNext.SetFocus();
	m_listNext.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	m_listNext.SetSelectionMark(0);
	m_listNext.SetCurSel(0);

	CenterWindow();
	return FALSE;
}

/**********************************************************************/

void CBackupResultDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
	switch( nIDCtl )
	{
	case IDC_LIST_RESULT:
		m_listResult.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;

	case IDC_LIST_NEXT:
		m_listNext.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;
	}
}

/**********************************************************************/

void CBackupResultDlg::OnButtonMore()
{
	if ( TRUE == m_bFullSize )
	{
		m_buttonMore.SetWindowText( "More" );
		m_bFullSize = FALSE;
	}
	else
	{
		m_buttonMore.SetWindowText( "Less" );
		m_bFullSize = TRUE;
	}

	SetDialogSize();
	UpdateDisplay();
}

/**********************************************************************/

void CBackupResultDlg::SetDialogSize()
{
	int nOffset = (m_bFullSize) ? 240 : 0;

	if (TRUE == m_bFullSize)
	{
		ResizeDialog(500, 375);
	}

	ResizeControl(&m_listResult, 486, 40 + nOffset);
	MoveControl(&m_buttonMore, 443, 63 + nOffset);
	MoveControl(&m_staticNext, 7, 75 + nOffset);
	MoveControl(&m_listNext, 7, 88 + nOffset);
	MoveControl(&m_buttonCancel, 443, 114 + nOffset);

	if (FALSE == m_bFullSize)
	{
		ResizeDialog(500, 135);
	}
}

/**********************************************************************/

void CBackupResultDlg::UpdateDisplay()
{
	if (FALSE == m_bFullSize)
	{
		m_listResult.SetItemCountEx(1);
	}
	else
	{
		m_listResult.SetItemCountEx(AutoBackupProcessor.GetLastResultLineCount());
	}

	Invalidate();
}

/**********************************************************************/

void CBackupResultDlg::OnGetDispInfoListResult(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		CCSV csv( AutoBackupProcessor.GetLastResultLine( nIndex ) );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CBackupResultDlg::OnGetDispInfoListNext(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		CCSV csv( AutoBackupProcessor.GetNextBackupLine() );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CBackupResultDlg::OnButtonClose()
{
	DestroyWindow();
}

/**********************************************************************/

void CBackupResultDlg::OnOK()
{
	DestroyWindow();
}

/**********************************************************************/

void CBackupResultDlg::OnCancel()
{
	DestroyWindow();
}

/**********************************************************************/

void CBackupResultDlg::PostNcDestroy() 
{
	*m_pParentToThis = NULL;
	delete this;
}

/**********************************************************************/