/**********************************************************************/
#include "DateRangeHelpers.h"
#include "S4LabourLocationOptions.h"
#include "PosTrayS4LabourFTPDlg.h"
#include "PosTrayS4LabourLocationDlg.h"
/**********************************************************************/
#include "PropPagePosTrayOptionsS4Labour.h"
/**********************************************************************/

CPropPagePosTrayOptionsS4Labour::CPropPagePosTrayOptionsS4Labour() : CSSPropertyPage(CPropPagePosTrayOptionsS4Labour::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePosTrayOptionsS4Labour)
	//}}AFX_DATA_INIT
	m_pArrayS4LabourInfo = NULL;
}

/**********************************************************************/

CPropPagePosTrayOptionsS4Labour::~CPropPagePosTrayOptionsS4Labour()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsS4Labour::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePosTrayOptionsS4Labour)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePosTrayOptionsS4Labour, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePosTrayOptionsS4Labour)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_FTP, OnButtonFTP)
	ON_BN_CLICKED(IDC_BUTTON_LOCATION, OnButtonLocation)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePosTrayOptionsS4Labour::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 1, "Location Name", LVCFMT_LEFT, 350 );
	m_listData.InsertColumn ( 2, "Location ID", LVCFMT_LEFT, 200 );
	m_listData.SetItemCountEx ( m_pArrayS4LabourInfo -> GetSize() );
	m_listData.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listData.SetCurSel(0);

	return TRUE;  
}

/**********************************************************************/

void CPropPagePosTrayOptionsS4Labour::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listData.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsS4Labour::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsS4Labour::OnSetActive() 
{
	GetRecordData();
	return ( CPropertyPage::OnSetActive() != 0 );
}

/**********************************************************************/

void CPropPagePosTrayOptionsS4Labour::GetRecordData()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsS4Labour::SaveRecord()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsS4Labour::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

bool CPropPagePosTrayOptionsS4Labour::PrepareLine( int nIndex )
{
	if ( nIndex < 0 || nIndex >= m_pArrayS4LabourInfo -> GetSize() )
		return FALSE;

	CPosTrayS4LabourInfo info;
	m_pArrayS4LabourInfo -> GetAt( nIndex, info );
	
	CString strExport = "";
	if ( TRUE == info.m_bEnable )
		strExport += info.m_strLocID;
	else
		strExport = "-----";
	
	CCSV csv( '\t' );
	csv.Add( info.m_strLocName );
	csv.Add( strExport );

	m_strDisplayLine = csv.GetLine();
	
	return TRUE;
}

/**********************************************************************/

int CPropPagePosTrayOptionsS4Labour::GetSafeSelection()
{
	return GetSafeSelection ( 0, m_pArrayS4LabourInfo -> GetSize() - 1 );
}

/**********************************************************************/

int CPropPagePosTrayOptionsS4Labour::GetSafeSelection( int nMin, int nMax )
{
	int nSel = m_listData.GetCurSel();
	if ( ( nSel < nMin ) || ( nSel > nMax ) || ( nSel >= m_listData.GetItemCount() ) )
		nSel = -1;
	
	return nSel;
}

/**********************************************************************/

void CPropPagePosTrayOptionsS4Labour::SelectLine ( int nIndex )
{
	int nSize = m_listData.GetItemCount();

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listData.SetCurSel ( nIndex );
	else 
		m_listData.SetCurSel ( nSize - 1 );
	
	m_listData.Invalidate();
}

/**********************************************************************/

void CPropPagePosTrayOptionsS4Labour::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButtonLocation();
	*pResult = 0;
}

/**********************************************************************/

void CPropPagePosTrayOptionsS4Labour::OnButtonFTP()
{
	CPosTrayS4LabourFTPDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/

void CPropPagePosTrayOptionsS4Labour::OnButtonLocation()
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	CPosTrayS4LabourInfo info;
	m_pArrayS4LabourInfo -> GetAt( nIndex, info );

	CS4LabourLocationOptions Options;
	Options.Read( info.m_nLocIdx, info.m_bModified );

	CPosTrayS4LabourLocationDlg dlg( info.m_nLocIdx, Options, this );

	if ( dlg.DoModal() == IDOK )
	{
		info.m_bEnable = Options.GetEnable();
		info.m_strLocID = Options.GetLocID();
		info.m_bModified = TRUE;
		m_pArrayS4LabourInfo -> SetAt( nIndex, info );
		Options.Write( info.m_nLocIdx, TRUE );

		COleDateTime dateNextExport;
		dlg.GetDateNextExport( dateNextExport );
		CDateRangeHelpers DateRangeHelpers;
		DateRangeHelpers.AddDaysToTime( dateNextExport, -1 );
		Options.SetLastExportDate( info.m_nLocIdx, dateNextExport, TRUE );

		m_listData.Invalidate();
		SelectLine( nIndex );
	}
}

/**********************************************************************/
