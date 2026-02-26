/**********************************************************************/
#include "CustomerMoveDlg.h"
#include "CustomerPrintDlg.h"
#include "CustomerSyncDlg.h"
#include "LocationCSVArray.h"
#include "PropertySheetCustomer.h" 
#include "SQLImporter.h"
/**********************************************************************/
#include "ListDataCustomerOneOffDlg.h"
/**********************************************************************/

CSingleListContentHelpersCustomerOneOff::CSingleListContentHelpersCustomerOneOff()
{
}

/**********************************************************************/

int CSingleListContentHelpersCustomerOneOff::GetArraySize()
{
	return DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderSize();
}

/**********************************************************************/

int CSingleListContentHelpersCustomerOneOff::GetArrayIdxFromListIdx( int nListIdx )
{
	int nArrayIdx = 0;
	
	if ( ( nListIdx >= 0 ) && ( nListIdx < GetArraySize() ) )
		nArrayIdx = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderItem( nListIdx );

	if ( ( nArrayIdx < 0 ) || ( nArrayIdx >= DataManagerNonDb.CustomerOneOff.GetSize() ) )
		nArrayIdx = 0;

	return nArrayIdx;
}

/**********************************************************************/

void CSingleListContentHelpersCustomerOneOff::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		int nArrayIdx = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderItem( nIndex );

		if ( ( nArrayIdx >= 0 ) && ( nArrayIdx < DataManagerNonDb.CustomerOneOff.GetSize() ) )
		{
			CCustomerCSVRecord Customer;
			DataManagerNonDb.CustomerOneOff.GetAt( nArrayIdx, Customer );
			Customer.SetDeletionStatus( CUSTOMER_DELETIONSTATUS_DELETENOW );
			DataManagerNonDb.CustomerOneOff.SetAt( nArrayIdx, Customer );
		}
	}
}

/**********************************************************************/

bool CSingleListContentHelpersCustomerOneOff::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	int nCustomerIdx = GetArrayIdxFromListIdx( nIndex );
	if ( ( nCustomerIdx >= 0 ) && ( nCustomerIdx < DataManagerNonDb.CustomerOneOff.GetSize() ) )
	{
		CCustomerCSVRecord CustomerRecord;
		DataManagerNonDb.CustomerOneOff.GetAt ( nCustomerIdx, CustomerRecord );
		
		strLine.Format ( "%8.8d\t%s\t%s\t%.2f\t%s",
			CustomerRecord.GetCustomerID(),
			CustomerRecord.GetDisplayName1(),
			CustomerRecord.GetDisplayName2() );

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersCustomerOneOff::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "ID", LVCFMT_LEFT, 70 );
	list.InsertColumn ( 1, "First Name", LVCFMT_LEFT, 205 );
	list.InsertColumn ( 2, "Last Name", LVCFMT_LEFT, 205 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataCustomerOneOffDlg::CListDataCustomerOneOffDlg( int nLocIdx, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nLocIdx = nLocIdx;
	m_bAdminOptions = FALSE;
	
	DataManagerNonDb.CustomerNameTable.OneOff.ClearList();
	DataManagerNonDb.CustomerNameTable.OneOff.BuildListCurrentDb();
	DataManagerNonDb.CustomerNameTable.OneOff.BuildSortOrder();

	m_pContentHelpers = new CSingleListContentHelpersCustomerOneOff();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "Customer" );
	m_pDisplayHelpers -> SetTopPluralText( "Customers" );
	m_pDisplayHelpers -> SetBottomSingleText( "customer" );
	m_pDisplayHelpers -> SetBottomPluralText( "customers" );
}

/**********************************************************************/

CListDataCustomerOneOffDlg::~CListDataCustomerOneOffDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataCustomerOneOffDlg::SetTitle()
{
	CString strTitle = "One Off Customers";
	
	CString strExtra;
	strExtra.Format( " (%s)", dbLocation.GetName( m_nLocIdx ) );
	strTitle += strExtra;
	
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataCustomerOneOffDlg::SetButtonTexts()
{
	m_button2.SetWindowText ( "&Edit\nCustomer" );
	m_button3.SetWindowText ( "&Print\nList" );
	m_button4.SetWindowText ( "&Delete\nCustomers" );
	m_button5.SetWindowText ( "&Move\nCustomers" );
	m_button6.SetWindowText ( "&Add\nCustomers" );
	
	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 42 );
	MoveControl( &m_button3, 368, 77 );
	MoveControl( &m_button4, 368, 137 );
	MoveControl( &m_button5, 368, 172 );
	MoveControl( &m_button6, 368, 207 );

	m_button1.EnableWindow( FALSE );
	m_button1.ShowWindow ( SW_HIDE );
	m_button5.EnableWindow( FALSE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.EnableWindow( FALSE );
	m_button6.ShowWindow ( SW_HIDE );
}

/**********************************************************************/

void CListDataCustomerOneOffDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataCustomerOneOffDlg::HandleClickHeader( int nColumn )
{
	int nNewSortMode = -1;
	int nOldSortMode = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderType();

	switch( nColumn )
	{
	case 0:
		
		if ( CUSTOMER_SORT_ID_INC == nOldSortMode )
			nNewSortMode = CUSTOMER_SORT_ID_DEC;
		else
			nNewSortMode = CUSTOMER_SORT_ID_INC;
		
		break;

	
	case 1:

		if ( CUSTOMER_SORT_FIRSTNAME_INC == nOldSortMode )
			nNewSortMode = CUSTOMER_SORT_FIRSTNAME_DEC;
		else
			nNewSortMode = CUSTOMER_SORT_FIRSTNAME_INC;
		
		break;

	case 2:

		if ( CUSTOMER_SORT_LASTNAME_INC == nOldSortMode )
			nNewSortMode = CUSTOMER_SORT_LASTNAME_DEC;
		else
			nNewSortMode = CUSTOMER_SORT_LASTNAME_INC;
		
		break;

	default:
		return;
	}

	DataManagerNonDb.CustomerNameTable.OneOff.BuildSortOrder( nNewSortMode );
	m_listData.SetItemCountEx( GetArraySize() );
	SelectLine(0);
}

/**********************************************************************/

void CListDataCustomerOneOffDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	CPropertySheetCustomer propSheet( IDC_SPIN, TRUE, nIndex, m_WarnCancelAfterChange, this );
	propSheet.SetListDataDlg(this);

	if ( propSheet.DoModal() == IDOK )
		propSheet.SaveRecord();

	if ( FALSE == m_bAdminOptions )
	{
		if ( propSheet.GetAdminEnabledFlag() == TRUE )
		{
			DataManagerNonDb.CustomerOneOff.ClearMoveToOtherArrayFlags();
			ShowAndEnableWindow( &m_button5, TRUE );
			ShowAndEnableWindow( &m_button6, TRUE );
			m_bAdminOptions = TRUE;
		}
	}

	SelectLine ( propSheet.GetIndex() );
}

/**********************************************************************/

void CListDataCustomerOneOffDlg::HandleButton3() 
{
	if ( DataManagerNonDb.CustomerOneOff.GetSize() == 0 )
	{
		Prompter.Error ( "You must create some customers\nbefore you can print this report." );
		return;
	}

	CCustomerPrintDlg dlgDateRange( m_nLocIdx, TRUE, this );
	dlgDateRange.SetTitle( "Print Customer List" );
	dlgDateRange.SetRangeText( "Customer Creation Date" );
	dlgDateRange.DoModal();
}

/**********************************************************************/

void CListDataCustomerOneOffDlg::HandleButton4()
{
	m_pDisplayHelpers -> HandleDelete( this );

	CString strDate;
	COleDateTime dateNow = COleDateTime::GetCurrentTime();
	strDate.Format( "%4.4d%2.2d%2.2d", dateNow.GetYear(), dateNow.GetMonth(), dateNow.GetDay() );

	bool bDeletedItem = FALSE;
	for ( int nCustomerIdx = DataManagerNonDb.CustomerOneOff.GetSize() - 1; nCustomerIdx >= 0; nCustomerIdx-- )
	{
		CCustomerCSVRecord Customer;
		DataManagerNonDb.CustomerOneOff.GetAt( nCustomerIdx, Customer );
		
		if ( Customer.GetDeletionStatus() == CUSTOMER_DELETIONSTATUS_DELETENOW )
		{
			DataManagerNonDb.CustomerOneOff.RemoveAt( nCustomerIdx );
			DataManagerNonDb.CustomerAccount.FlagDeletion( Customer.GetCustomerID(), strDate );
			bDeletedItem = TRUE;
		}
	}

	if ( TRUE == bDeletedItem )
	{
		int nSortOrder = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderType();
		DataManagerNonDb.CustomerNameTable.OneOff.ClearList();
		DataManagerNonDb.CustomerNameTable.OneOff.BuildListCurrentDb();
		DataManagerNonDb.CustomerNameTable.OneOff.BuildSortOrder( nSortOrder );

		m_listData.SetItemCountEx( GetArraySize() );
		m_pDisplayHelpers -> SelectDatabaseLine( m_pDisplayHelpers -> GetFirstDeletedItem() );

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataCustomerOneOffDlg::HandleButton5()
{
	if ( TRUE == m_bAdminOptions )
	{
		if ( DataManagerNonDb.CustomerOneOff.GetSize() == 0 )
			Prompter.Error ( "You must create some customers\nbefore you can use this option." );
		else
		{
			CArray<int,int> arrayCustomerIdx;

			for ( int n = 0; n < GetArraySize(); n++ )
			{
				int nCustomerIdx = m_pContentHelpers -> GetArrayIdxFromListIdx( n );

				CCustomerCSVRecord Customer;
				DataManagerNonDb.CustomerAccount.GetAt( nCustomerIdx, Customer );
				arrayCustomerIdx.Add( nCustomerIdx );
			}

			CCustomerMoveDlg dlgMove( m_nLocIdx, TRUE, arrayCustomerIdx, this );
			if ( dlgMove.DoModal() == IDOK )
			{
				m_WarnCancelAfterChange.SetFlag();
			}
		}
	}
}

/**********************************************************************/

void CListDataCustomerOneOffDlg::HandleButton6()
{
	if ( TRUE == m_bAdminOptions )
	{
		CArray<int,int> arrayIDs;
		CCustomerSyncDlg dlgSync( TRUE, arrayIDs, this );

		if ( dlgSync.DoModal() == IDOK )
		{
			int nOldSelect = GetSafeSelection();

			int nSelectedID = 0;

			int nCustomerIdx = m_pContentHelpers -> GetArrayIdxFromListIdx( nOldSelect );
			if ( ( nCustomerIdx < 0 ) || ( nCustomerIdx >= DataManagerNonDb.CustomerOneOff.GetSize() ) )	
				nSelectedID = DataManagerNonDb.CustomerOneOff.GetCustomerID( nCustomerIdx );

			bool bChanged = FALSE;
			
			switch( dlgSync.m_nAction )
			{
			//ADD FROM LIST OF IDS
			case 0:
				{
					int nOldSize = DataManagerNonDb.CustomerOneOff.GetSize();
					DataManagerNonDb.CustomerOneOff.AddFromIDList( arrayIDs, DataManagerNonDb.CustomerAccount, TRUE );
					bChanged = ( DataManagerNonDb.CustomerOneOff.GetSize() != nOldSize );
				}
				break;

			//IMPORT FROM SQL FILE
			case 1:
				{
					CFileDialog dlgFile ( TRUE, "sql", "database.sql", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, "SQL Import Files (*.sql)|*.sql||", NULL );
					dlgFile.m_ofn.lpstrTitle = "Import Data Transfer Wizard File";
					dlgFile.m_ofn.lpstrInitialDir = ".\\";
		
					if ( dlgFile.DoModal() == IDOK )
					{
						CSQLImporter SQLImporter( dlgFile.GetPathName() );
						SQLImporter.ImportCustomersOnly( FALSE );
						bChanged = TRUE;
					}
				}
				break;
			}

			if ( TRUE == bChanged )
			{
				int nSortOrder = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderType();
				DataManagerNonDb.CustomerNameTable.OneOff.ClearList();
				DataManagerNonDb.CustomerNameTable.OneOff.BuildListCurrentDb();
				DataManagerNonDb.CustomerNameTable.OneOff.BuildSortOrder( nSortOrder );
				m_listData.SetItemCountEx ( GetArraySize() );
				SelectLineByID ( nSelectedID );
				m_WarnCancelAfterChange.SetFlag();
			}
		}
	}
}

/**********************************************************************/

void CListDataCustomerOneOffDlg::SelectLineByID( int nID )
{
	for ( int n = 0; n < GetArraySize(); n++ )
	{
		int nCustomerIdx = m_pContentHelpers -> GetArrayIdxFromListIdx(n);
		if ( DataManagerNonDb.CustomerOneOff.GetCustomerID( nCustomerIdx ) == nID )
		{
			SelectLine(n);
			return;
		}
	}

	SelectLine(0);
}

/**********************************************************************/
