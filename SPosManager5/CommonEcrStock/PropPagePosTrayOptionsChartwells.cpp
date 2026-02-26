/**********************************************************************/
#include "..\CommonEcrStockTray\ChartwellsOptions.h"
/**********************************************************************/
#include "DateRangeHelpers.h"
#include "PosTrayChartwellsOptionsDlg.h"
/**********************************************************************/
#include "PropPagePosTrayOptionsChartwells.h"
/**********************************************************************/

CPropPagePosTrayOptionsChartwells::CPropPagePosTrayOptionsChartwells() : CSSPropertyPage(CPropPagePosTrayOptionsChartwells::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePosTrayOptionsFNB)
	//}}AFX_DATA_INIT
	m_pArrayChartwellsInfo = NULL;
}

/**********************************************************************/

CPropPagePosTrayOptionsChartwells::~CPropPagePosTrayOptionsChartwells()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsChartwells::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePosTrayOptionsChartwells)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePosTrayOptionsChartwells, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePosTrayOptionsChartwells)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePosTrayOptionsChartwells::OnInitDialog()
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 1, "FTP Server", LVCFMT_LEFT, 550 );
	m_listData.SetItemCountEx ( m_pArrayChartwellsInfo -> GetSize() );
	m_listData.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listData.SetCurSel(0);

	return TRUE;  
}

/**********************************************************************/

void CPropPagePosTrayOptionsChartwells::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listData.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsChartwells::OnKillActive()
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsChartwells::OnSetActive()
{
	GetRecordData();
	return ( CPropertyPage::OnSetActive() != 0 );
}

/**********************************************************************/

void CPropPagePosTrayOptionsChartwells::GetRecordData()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsChartwells::SaveRecord()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsChartwells::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLine( nIndex );
		CCSV csv ( m_strDisplayLine, '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

bool CPropPagePosTrayOptionsChartwells::PrepareLine(int nIndex)
{
	if ((nIndex < 0) || (nIndex >= m_pArrayChartwellsInfo->GetSize()))
	{
		return FALSE;
	}

	CPosTrayChartwellsInfo info;
	m_pArrayChartwellsInfo->GetAt(nIndex, info);

	CCSV csv('\t');
	csv.Add(info.m_strServer);
	m_strDisplayLine = csv.GetLine();

	return TRUE;
}

/**********************************************************************/

int CPropPagePosTrayOptionsChartwells::GetSafeSelection()
{
	return GetSafeSelection ( 0, m_pArrayChartwellsInfo -> GetSize() - 1 );
}

/**********************************************************************/

int CPropPagePosTrayOptionsChartwells::GetSafeSelection(int nMin, int nMax)
{
	int nSel = m_listData.GetCurSel();
	if ((nSel < nMin) || (nSel > nMax) || (nSel >= m_listData.GetItemCount()))
	{
		nSel = -1;
	}

	return nSel;
}

/**********************************************************************/

void CPropPagePosTrayOptionsChartwells::SelectLine(int nIndex)
{
	int nSize = m_listData.GetItemCount();

	if ((nIndex >= 0) && (nIndex < nSize))
	{
		m_listData.SetCurSel(nIndex);
	}
	else
	{
		m_listData.SetCurSel(nSize - 1);
	}

	m_listData.Invalidate();
}

/**********************************************************************/

void CPropPagePosTrayOptionsChartwells::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult)
{	
	OnButtonEdit();
	*pResult = 0;
}

/**********************************************************************/

void CPropPagePosTrayOptionsChartwells::OnButtonEdit()
{
	int nIndex = GetSafeSelection();
	if (nIndex == -1)
	{
		return;
	}

	CPosTrayChartwellsInfo info;
	m_pArrayChartwellsInfo -> GetAt( nIndex, info );

	ChartwellsOptions.Read( info.m_bModified );

	CPosTrayChartwellsOptionsDlg dlg( ChartwellsOptions, this );

	if ( dlg.DoModal() == IDOK )
	{
		info.m_bEnable = ChartwellsOptions.GetEnable();
		info.m_strServer = ChartwellsOptions.GetFTPServer();
		info.m_bModified = TRUE;
		m_pArrayChartwellsInfo -> SetAt( nIndex, info );
		ChartwellsOptions.Write( TRUE );

		COleDateTime dateNextExport;
		dlg.GetDateNextExport( dateNextExport );
		CDateRangeHelpers DateRangeHelpers;
		DateRangeHelpers.AddDaysToTime( dateNextExport, -1 );
		ChartwellsOptions.SetLastExportDate( dateNextExport, TRUE );

		m_listData.Invalidate();
		SelectLine( nIndex );
	}
}

/**********************************************************************/
