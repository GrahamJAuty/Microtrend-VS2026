/**********************************************************************/
#include "DateRangeHelpers.h"
#include "FNBDatabaseOptions.h"
#include "PosTrayFNBOptionsDlg.h"
/**********************************************************************/
#include "PropPagePosTrayOptionsFNB.h"
/**********************************************************************/

CPropPagePosTrayOptionsFNB::CPropPagePosTrayOptionsFNB() : CSSPropertyPage(CPropPagePosTrayOptionsFNB::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePosTrayOptionsFNB)
	//}}AFX_DATA_INIT
	m_pArrayFNBInfo = NULL;
}

/**********************************************************************/

CPropPagePosTrayOptionsFNB::~CPropPagePosTrayOptionsFNB()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsFNB::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePosTrayOptionsFNB)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePosTrayOptionsFNB, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePosTrayOptionsFNB)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePosTrayOptionsFNB::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 1, "Database", LVCFMT_LEFT, 350 );
	m_listData.InsertColumn ( 2, "FTP Folder", LVCFMT_LEFT, 200 );
	m_listData.SetItemCountEx ( m_pArrayFNBInfo -> GetSize() );
	m_listData.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listData.SetCurSel(0);

	return TRUE;  
}

/**********************************************************************/

void CPropPagePosTrayOptionsFNB::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listData.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsFNB::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsFNB::OnSetActive() 
{
	GetRecordData();
	return ( CPropertyPage::OnSetActive() != 0 );
}

/**********************************************************************/

void CPropPagePosTrayOptionsFNB::GetRecordData()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsFNB::SaveRecord()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsFNB::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

bool CPropPagePosTrayOptionsFNB::PrepareLine( int nIndex )
{
	if ( nIndex < 0 || nIndex >= m_pArrayFNBInfo -> GetSize() )
		return FALSE;

	CPosTrayFNBInfo info;
	m_pArrayFNBInfo -> GetAt( nIndex, info );
	
	CString strExport = "";
	if ( TRUE == info.m_bEnable )
		strExport += info.m_strFolder;
	else
		strExport = "-----";
	
	CCSV csv( '\t' );
	csv.Add( info.m_strDbName );
	csv.Add( strExport );

	m_strDisplayLine = csv.GetLine();
	
	return TRUE;
}

/**********************************************************************/

int CPropPagePosTrayOptionsFNB::GetSafeSelection()
{
	return GetSafeSelection ( 0, m_pArrayFNBInfo -> GetSize() - 1 );
}

/**********************************************************************/

int CPropPagePosTrayOptionsFNB::GetSafeSelection( int nMin, int nMax )
{
	int nSel = m_listData.GetCurSel();
	if ( ( nSel < nMin ) || ( nSel > nMax ) || ( nSel >= m_listData.GetItemCount() ) )
		nSel = -1;
	
	return nSel;
}

/**********************************************************************/

void CPropPagePosTrayOptionsFNB::SelectLine ( int nIndex )
{
	int nSize = m_listData.GetItemCount();

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listData.SetCurSel ( nIndex );
	else 
		m_listData.SetCurSel ( nSize - 1 );
	
	m_listData.Invalidate();
}

/**********************************************************************/

void CPropPagePosTrayOptionsFNB::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButtonEdit();
	*pResult = 0;
}

/**********************************************************************/

void CPropPagePosTrayOptionsFNB::OnButtonEdit()
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	CPosTrayFNBInfo info;
	m_pArrayFNBInfo -> GetAt( nIndex, info );

	CFNBDatabaseOptions Options;
	Options.Read( info.m_nDbIdx, info.m_bModified );

	CPosTrayFNBOptionsDlg dlg( info.m_nDbIdx, Options, this );

	if ( dlg.DoModal() == IDOK )
	{
		info.m_bEnable = Options.GetEnable();
		info.m_strFolder = Options.GetFTPFolder();
		info.m_bModified = TRUE;
		m_pArrayFNBInfo -> SetAt( nIndex, info );
		Options.Write( info.m_nDbIdx, TRUE );

		COleDateTime dateNextExport;
		dlg.GetDateNextExport( dateNextExport );
		CDateRangeHelpers DateRangeHelpers;
		DateRangeHelpers.AddDaysToTime( dateNextExport, -1 );
		Options.SetLastExportDate( info.m_nDbIdx, dateNextExport, TRUE );

		m_listData.Invalidate();
		SelectLine( nIndex );
	}
}

/**********************************************************************/
