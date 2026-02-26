/**********************************************************************/
#include "CustomerTrackTraceReport.h"
#include "CustomerVisitDlg.h"
#include "LocationCSVArray.h"
#include "PropertySheetCustomerNoSpin.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "CustomerTrackTraceDlg.h"
/**********************************************************************/

CCustomerTrackTraceDlg::CCustomerTrackTraceDlg( int nLocIdx, CWnd* pParent /*=NULL*/)
	: CSSDialog(CCustomerTrackTraceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomerTrackTraceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nLocIdx = nLocIdx;
	m_strID = "";
	m_strName = "";
	m_strPhone = "";
	m_nPhoneColumnType = 1;
}

/**********************************************************************/

void CCustomerTrackTraceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomerTrackTraceDlg)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName );
	DDX_Control(pDX, IDC_EDIT_PHONE, m_editPhone );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCustomerTrackTraceDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomerTrackTraceDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnButtonFind)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

int CCustomerTrackTraceDlg::GetArraySize()
{
	return DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderSize();
}

/**********************************************************************/

int CCustomerTrackTraceDlg::GetArrayIdxFromListIdx( int nListIdx )
{
	int nArrayIdx = 0;
	
	if ( ( nListIdx >= 0 ) && ( nListIdx < GetArraySize() ) )
	{
		nArrayIdx = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderItem( nListIdx );
	}

	if ( ( nArrayIdx < 0 ) || ( nArrayIdx >= DataManagerNonDb.CustomerOneOff.GetSize() ) )
	{
		nArrayIdx = 0;
	}

	return nArrayIdx;
}

/**********************************************************************/

void CCustomerTrackTraceDlg::SelectListLine ( int nIndex )
{
	m_listCustomers.Invalidate();

	for ( int n = 0; n < m_listCustomers.GetItemCount(); n++ )
	{
		if ( n != nIndex )
			m_listCustomers.SetItemState(n, 0, LVIS_SELECTED);
		else
		{
			m_listCustomers.SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
			m_listCustomers.SetSelectionMark(n);
			m_listCustomers.SetCurSel(n);
		}
	}
}

/**********************************************************************/

BOOL CCustomerTrackTraceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit( IDC_EDIT_ID, SS_NUM, 8, "%s" );
	m_editName.LimitText( Customer::CustomerName1.Max );
	m_editPhone.LimitText( Customer::Phone.Max );

	DataManagerNonDb.CustomerNameTable.OneOff.ClearList();
	DataManagerNonDb.CustomerNameTable.OneOff.BuildListCurrentDb();
	
	m_listCustomers.SubclassDlgItem ( IDC_LIST, this );
	m_listCustomers.InsertColumn ( 0, "ID", LVCFMT_LEFT, 70 );
	m_listCustomers.InsertColumn ( 1, "First Name", LVCFMT_LEFT, 180 );
	m_listCustomers.InsertColumn ( 2, "Last Name", LVCFMT_LEFT, 180 );
	m_listCustomers.InsertColumn ( 3, "Phone1", LVCFMT_LEFT, 180 );
	m_listCustomers.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	m_listCustomers.SetItemCount( DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderSize() );

	return TRUE;
}

/**********************************************************************/

CEdit* CCustomerTrackTraceDlg::GetEditID(){ return GetEdit( IDC_EDIT_ID ); }

/**********************************************************************/

void CCustomerTrackTraceDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST ) 
   {
	   m_listCustomers.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

void CCustomerTrackTraceDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nCustIdx = GetArrayIdxFromListIdx ( pDispInfo->item.iItem );

		if ( ( nCustIdx >= 0 ) && ( nCustIdx < DataManagerNonDb.CustomerOneOff.GetSize() ) )
		{
			CCustomerCSVRecord Customer;
			DataManagerNonDb.CustomerOneOff.GetAt( nCustIdx, Customer );

			CString strText = "";

			switch( pDispInfo->item.iSubItem )
			{
			case 0:
				strText.Format ( "%8.8d", Customer.GetCustomerID() );
				break;

			case 1:
				strText = Customer.GetDisplayName1();
				break;

			case 2:
				strText = Customer.GetDisplayName2();
				break;

			case 3:
				switch( m_nPhoneColumnType )
				{
				case 2:
					strText = Customer.GetPhone2();
					break;

				case 3:
					strText = Customer.GetPhone3();
					break;

				case 1:
				default:
					strText = Customer.GetPhone1();
					break;
				}
			}

			lstrcpy ( pDispInfo->item.pszText, strText );
		}
	}

	*pResult = 0;
}

/**********************************************************************/

void CCustomerTrackTraceDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nColumn = pNMListView->iSubItem;
	
	int nNewSortType = -1;
	int nOldSortType = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderType();

	switch( nColumn )
	{
	case 0:
		nNewSortType = ( CUSTOMER_SORT_ID_INC == nOldSortType ) ? CUSTOMER_SORT_ID_DEC : CUSTOMER_SORT_ID_INC;
		break;

	case 1:
		nNewSortType = ( CUSTOMER_SORT_FIRSTNAME_INC == nOldSortType ) ? CUSTOMER_SORT_FIRSTNAME_DEC : CUSTOMER_SORT_FIRSTNAME_INC;
		break;

	case 2:
		nNewSortType = ( CUSTOMER_SORT_LASTNAME_INC == nOldSortType ) ? CUSTOMER_SORT_LASTNAME_DEC : CUSTOMER_SORT_LASTNAME_INC;
		break;

	case 3:

		m_nPhoneColumnType++;
		
		if ( m_nPhoneColumnType > 3 ) 
		{
			m_nPhoneColumnType = 1;
		}
		
		{
			LVCOLUMN col;
			::ZeroMemory((void *)&col, sizeof(LVCOLUMN));
			col.mask = LVCF_TEXT;

			if (m_listCustomers.GetColumn(3, &col))
			{
				CString strText;
				strText.Format( "Phone%d", m_nPhoneColumnType );

				col.pszText = strText.GetBuffer(strText.GetLength());;
				m_listCustomers.SetColumn(3, &col);
			}
		}
		break;
	}

	if ( nNewSortType != -1 )
	{
		DataManagerNonDb.CustomerNameTable.OneOff.BuildSortOrder( nNewSortType, TRUE, m_strID, m_strName, m_strPhone );
		m_listCustomers.SetItemCount( DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderSize() );
		SelectListLine(0);
	}
	
	m_listCustomers.RedrawWindow();
	*pResult = 0;
}

/**********************************************************************/

void CCustomerTrackTraceDlg::OnButtonApply()
{
	GetEditID() -> GetWindowText( m_strID );
	::TrimSpaces( m_strID, FALSE );

	m_editName.GetWindowText( m_strName );
	::TrimSpaces( m_strName, FALSE );

	m_editPhone.GetWindowText( m_strPhone );
	::TrimSpaces( m_strPhone, FALSE );

	DataManagerNonDb.CustomerNameTable.OneOff.BuildSortOrder( DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderType(), TRUE, m_strID, m_strName, m_strPhone );
	m_listCustomers.SetItemCount( DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderSize() );
	SelectListLine(0);
	
	m_listCustomers.RedrawWindow();
}

/**********************************************************************/

void CCustomerTrackTraceDlg::OnButtonFind() 
{
	int nSel = m_listCustomers.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < GetArraySize() ) )
	{
		int nCustIdx = GetArrayIdxFromListIdx ( nSel );

		if ( ( nCustIdx >= 0 ) && ( nCustIdx < DataManagerNonDb.CustomerOneOff.GetSize() ) )
		{
			CCustomerVisitDlg dlgVisit( nCustIdx, this );

			if ( dlgVisit.DoModal() == IDOK )
			{
				CCustomerTrackTraceReport TrackTraceReport( m_nLocIdx, nCustIdx, dlgVisit.m_nBeforeThis, dlgVisit.m_nAfterThis, dlgVisit.m_nBeforeOther, dlgVisit.m_nAfterOther, dlgVisit.m_nVisit );

				if ( TrackTraceReport.CreateReport() == FALSE )
				{
					Prompter.Error( "Unable to create report" );
				}
				else
				{
					CReportManager ReportManager;
					CFilenameUpdater FnUp( SysFiles::CustomerTrackTracePrm );
					ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
				}
			}
		}
	}	
}

/**********************************************************************/

void CCustomerTrackTraceDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonEdit();
	*pResult = 0;
}

/**********************************************************************/

void CCustomerTrackTraceDlg::OnButtonEdit() 
{
	int nSel = m_listCustomers.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < GetArraySize() ) )
	{
		CPropertySheetCustomerNoSpin propSheet( nSel, m_WarnCancelAfterChange, this );
			
		if ( propSheet.DoModal() == IDOK )
		{
			propSheet.SaveRecord();
		}

		SelectListLine ( nSel );
	}
}

/**********************************************************************/

void CCustomerTrackTraceDlg::OnCancel() 
{
	if ( CListDataDlg::CheckBeforeCancel( FALSE, m_WarnCancelAfterChange.GetFlag() ) == TRUE )
	{
		EndDialog( IDCANCEL );
	}
}

/**********************************************************************/
