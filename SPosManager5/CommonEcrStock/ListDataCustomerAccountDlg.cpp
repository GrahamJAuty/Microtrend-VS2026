/**********************************************************************/
#include "CustomerActivityChecker.h"
#include "CustomerExtractor.h"
#include "CustomerMoveDlg.h"
#include "CustomerPrintDlg.h"
#include "CustomerSyncDlg.h"
#include "DefEditCustomerDateDlg.h"
#include "LocationCSVArray.h"
#include "PropertySheetCustomer.h"
#include "SQLImporter.h"
/**********************************************************************/
#include "ListDataCustomerAccountDlg.h"
/**********************************************************************/

CSingleListContentHelpersCustomerAccount::CSingleListContentHelpersCustomerAccount()
{
}

/**********************************************************************/

int CSingleListContentHelpersCustomerAccount::GetArraySize()
{
	return DataManagerNonDb.CustomerNameTable.Account.GetSortOrderSize();
}

/**********************************************************************/

int CSingleListContentHelpersCustomerAccount::GetArrayIdxFromListIdx( int nListIdx )
{
	int nArrayIdx = 0;
	
	if ( ( nListIdx >= 0 ) && ( nListIdx < GetArraySize() ) )
		nArrayIdx = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderItem( nListIdx );

	if ( ( nArrayIdx < 0 ) || ( nArrayIdx >= DataManagerNonDb.CustomerAccount.GetSize() ) )
		nArrayIdx = 0;

	return nArrayIdx;
}

/**********************************************************************/

void CSingleListContentHelpersCustomerAccount::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		int nArrayIdx = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderItem( nIndex );

		if ( ( nArrayIdx >= 0 ) && ( nArrayIdx < DataManagerNonDb.CustomerAccount.GetSize() ) )
		{
			CCustomerCSVRecord Customer;
			DataManagerNonDb.CustomerAccount.GetAt( nArrayIdx, Customer );
			Customer.SetDeletionStatus( CUSTOMER_DELETIONSTATUS_DELETENOW );
			DataManagerNonDb.CustomerAccount.SetAt( nArrayIdx, Customer );
		}
	}
}

/**********************************************************************/

bool CSingleListContentHelpersCustomerAccount::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	int nCustomerIdx = GetArrayIdxFromListIdx( nIndex );
	if ( ( nCustomerIdx >= 0 ) && ( nCustomerIdx < DataManagerNonDb.CustomerAccount.GetSize() ) )
	{
		CCustomerCSVRecord CustomerRecord;
		DataManagerNonDb.CustomerAccount.GetAt ( nCustomerIdx, CustomerRecord );
		
		strLine.Format ( "%8.8d\t%s\t%s\t%.2f\t%s",
			CustomerRecord.GetCustomerID(),
			CustomerRecord.GetDisplayName1(),
			CustomerRecord.GetDisplayName2(),
			CustomerRecord.GetStartBalance(),
			CustomerRecord.GetDisplayDateBalance() );

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CSingleListContentHelpersCustomerAccount::PrepareDeletionLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	int nCustomerIdx = GetArrayIdxFromListIdx( nIndex );
	if ( ( nCustomerIdx >= 0 ) && ( nCustomerIdx < DataManagerNonDb.CustomerAccount.GetSize() ) )
	{
		CCustomerCSVRecord CustomerRecord;
		DataManagerNonDb.CustomerAccount.GetAt ( nCustomerIdx, CustomerRecord );

		CString strActivityDate = "none";

		CString strCustomerDate = CustomerRecord.GetTempDate();
		if ( strCustomerDate.GetLength() == 8 )
		{
			strActivityDate = "";
			strActivityDate += strCustomerDate.Right(2);
			strActivityDate += "/";
			strActivityDate += strCustomerDate.Mid(4,2);
			strActivityDate += "/";
			strActivityDate += strCustomerDate.Left(4);
		}
		
		strLine.Format ( "%8.8d\t%s\t%s\t%s",
			CustomerRecord.GetCustomerID(),
			CustomerRecord.GetDisplayName1(),
			CustomerRecord.GetDisplayName2(),
			(const char*) strActivityDate );

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersCustomerAccount::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>&  arraySelected )
{
	m_bPreventDeletion = FALSE;

	for ( int n = arraySelected.GetSize() - 1; n >= 0; n-- )
	{
		CSortedIntItem item;
		arraySelected.GetAt( n, item );

		if ( ( item.m_nItem >= 0 ) && ( item.m_nItem < GetArraySize() ) )
		{
			int nArrayIdx = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderItem( item.m_nItem );

			if ( ( nArrayIdx >= 0 ) && ( nArrayIdx < DataManagerNonDb.CustomerAccount.GetSize() ) )
			{
				CCustomerCSVRecord CustomerRecord;
				DataManagerNonDb.CustomerAccount.GetAt( nArrayIdx, CustomerRecord );

				if ( CustomerRecord.GetDeletionStatus() == CUSTOMER_DELETIONSTATUS_PROTECTED )
				{
					arraySelected.RemoveAt(n);
					m_bPreventDeletion = TRUE;
				}
			}
		}
	}

	m_bPreventDeletion &= ( arraySelected.GetSize() == 0 );
}

/**********************************************************************/

void CSingleListContentHelpersCustomerAccount::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "ID", LVCFMT_LEFT, 70 );
	list.InsertColumn ( 1, "First Name", LVCFMT_LEFT, 155 );
	list.InsertColumn ( 2, "Last Name", LVCFMT_LEFT, 155 );
	list.InsertColumn ( 3, "Recent Activity", LVCFMT_RIGHT, 100 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataCustomerAccountDlg::CListDataCustomerAccountDlg( int nLocIdx, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_bCheckedActivity = FALSE;
	m_bAdminOptions = FALSE;
	
	m_nLocIdx = nLocIdx;
	DataManagerNonDb.CustomerNameTable.Account.ClearList();
	DataManagerNonDb.CustomerNameTable.Account.BuildListCurrentDb();
	DataManagerNonDb.CustomerNameTable.Account.BuildSortOrder();

	m_pContentHelpers = new CSingleListContentHelpersCustomerAccount();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "Customer" );
	m_pDisplayHelpers -> SetTopPluralText( "Customers" );
	m_pDisplayHelpers -> SetBottomSingleText( "customer" );
	m_pDisplayHelpers -> SetBottomPluralText( "customers" );
}

/**********************************************************************/

CListDataCustomerAccountDlg::~CListDataCustomerAccountDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataCustomerAccountDlg::SetTitle()
{
	CString strTitle = "Customer Accounts";
	
	CString strExtra;
	strExtra.Format( " (%s)", dbLocation.GetName( m_nLocIdx ) );
	strTitle += strExtra;
	
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataCustomerAccountDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "Location\n&Start Date" );
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
	m_button5.EnableWindow( FALSE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.EnableWindow( FALSE );
	m_button6.ShowWindow ( SW_HIDE );
}

/**********************************************************************/

void CListDataCustomerAccountDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "ID", LVCFMT_LEFT, 70 );
	m_listData.InsertColumn ( 1, "First Name", LVCFMT_LEFT, 120 );
	m_listData.InsertColumn ( 2, "Last Name", LVCFMT_LEFT, 120 );
	m_listData.InsertColumn ( 3, "Start Balance", LVCFMT_RIGHT, 80 );
	m_listData.InsertColumn ( 4, "Start Date", LVCFMT_RIGHT, 90 );
}

/**********************************************************************/

void CListDataCustomerAccountDlg::HandleClickHeader( int nColumn )
{
	int nNewSortMode = -1;
	int nOldSortMode = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderType();

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

	DataManagerNonDb.CustomerNameTable.Account.BuildSortOrder( nNewSortMode );
	m_listData.SetItemCountEx( GetArraySize() );
	SelectLine(0);
}

/**********************************************************************/

void CListDataCustomerAccountDlg::HandleButton1()
{
	CDefEditCustomerDateDlg dlg( m_nLocIdx, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		m_WarnCancelAfterChange.SetFlag();
	}

	if ( FALSE == m_bAdminOptions )
	{
		if ( dlg.GetAdminOptionsFlag() == TRUE )
		{
			DataManagerNonDb.CustomerAccount.ClearMoveToOtherArrayFlags();
			ShowAndEnableWindow( &m_button5, TRUE );
			ShowAndEnableWindow( &m_button6, TRUE );
			m_bAdminOptions = TRUE;
		}
	}
}

/**********************************************************************/

void CListDataCustomerAccountDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	CPropertySheetCustomer propSheet( IDC_SPIN, FALSE, nIndex, m_WarnCancelAfterChange, this );
	propSheet.SetListDataDlg(this);

	if ( propSheet.DoModal() == IDOK )
		propSheet.SaveRecord();

	if ( FALSE == m_bAdminOptions )
	{
		if ( propSheet.GetAdminEnabledFlag() == TRUE )
		{
			DataManagerNonDb.CustomerAccount.ClearMoveToOtherArrayFlags();
			ShowAndEnableWindow( &m_button5, TRUE );
			ShowAndEnableWindow( &m_button6, TRUE );
			m_bAdminOptions = TRUE;
		}
	}

	SelectLine ( propSheet.GetIndex() );
}

/**********************************************************************/

void CListDataCustomerAccountDlg::HandleButton3() 
{
	if ( DataManagerNonDb.CustomerAccount.GetSize() == 0 )
	{
		Prompter.Error ( "You must create some customers\nbefore you can print this report." );
		return;
	}

	CCustomerPrintDlg dlgDateRange( m_nLocIdx, FALSE, this );
	dlgDateRange.SetTitle( "Print Customer List" );
	dlgDateRange.SetRangeText( "Customer Creation Date" );
	dlgDateRange.DoModal();
}

/**********************************************************************/	

void CListDataCustomerAccountDlg::HandleButton4()
{
	if ( m_bCheckedActivity == FALSE )
	{
		CCustomerActivityChecker Checker( m_nLocIdx );
		m_bCheckedActivity = TRUE;
	}

	m_pDisplayHelpers -> HandleDelete( this );

	if ( m_pContentHelpers -> GetPreventDeletionFlag() == TRUE )
	{
		CString strMsg = "";
		strMsg += "All of the selected customers are currently\n";
		strMsg += "flagged as in use and cannot be deleted.";
		Prompter.Error( strMsg );
	}
	else
	{
		CString strDate;
		COleDateTime dateNow = COleDateTime::GetCurrentTime();
		strDate.Format( "%4.4d%2.2d%2.2d", dateNow.GetYear(), dateNow.GetMonth(), dateNow.GetDay() );

		bool bDeletedItem = FALSE;
		for ( int nCustomerIdx = DataManagerNonDb.CustomerAccount.GetSize() - 1; nCustomerIdx >= 0; nCustomerIdx-- )
		{
			CCustomerCSVRecord Customer;
			DataManagerNonDb.CustomerAccount.GetAt( nCustomerIdx, Customer );
			
			if ( Customer.GetDeletionStatus() == CUSTOMER_DELETIONSTATUS_DELETENOW )
			{
				DataManagerNonDb.CustomerAccount.RemoveAt( nCustomerIdx );
				DataManagerNonDb.CustomerAccount.FlagDeletion( Customer.GetCustomerID(), strDate );
				bDeletedItem = TRUE;
			}
		}

		if ( TRUE == bDeletedItem )
		{
			int nSortOrder = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderType();
			DataManagerNonDb.CustomerNameTable.Account.ClearList();
			DataManagerNonDb.CustomerNameTable.Account.BuildListCurrentDb();
			DataManagerNonDb.CustomerNameTable.Account.BuildSortOrder( nSortOrder );

			m_listData.SetItemCountEx( GetArraySize() );
			m_pDisplayHelpers -> SelectDatabaseLine( m_pDisplayHelpers -> GetFirstDeletedItem() );

			m_WarnCancelAfterChange.SetFlag();
		}
	}
}

/**********************************************************************/

void CListDataCustomerAccountDlg::HandleButton5()
{
	if ( TRUE == m_bAdminOptions )
	{
		if ( DataManagerNonDb.CustomerAccount.GetSize() == 0 )
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

			if ( m_bCheckedActivity == FALSE )
			{
				CCustomerActivityChecker Checker( m_nLocIdx );
				m_bCheckedActivity = TRUE;
			}

			CCustomerMoveDlg dlgMove( m_nLocIdx, FALSE, arrayCustomerIdx, this );
			if ( dlgMove.DoModal() == IDOK )
			{
				m_WarnCancelAfterChange.SetFlag();
			}
		}
	}
}

/**********************************************************************/

void CListDataCustomerAccountDlg::HandleButton6()
{
	if ( TRUE == m_bAdminOptions )
	{
		CArray<int,int> arrayIDs;
		CCustomerSyncDlg dlgSync( FALSE, arrayIDs, this );

		if ( dlgSync.DoModal() == IDOK )
		{
			int nOldSelect = GetSafeSelection();

			int nSelectedID = 0;

			int nCustomerIdx = m_pContentHelpers -> GetArrayIdxFromListIdx( nOldSelect );
			if ( ( nCustomerIdx < 0 ) || ( nCustomerIdx >= DataManagerNonDb.CustomerAccount.GetSize() ) )	
				nSelectedID = DataManagerNonDb.CustomerAccount.GetCustomerID( nCustomerIdx );

			bool bChanged = FALSE;
			
			switch( dlgSync.m_nAction )
			{
			//ADD FROM LIST OF IDS
			case 0:
				{
					int nOldSize = DataManagerNonDb.CustomerAccount.GetSize();
					DataManagerNonDb.CustomerAccount.AddFromIDList( arrayIDs, DataManagerNonDb.CustomerOneOff, FALSE );
					bChanged = ( DataManagerNonDb.CustomerAccount.GetSize() != nOldSize );
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

			//IMPORT FROM CASHRSP
			case 2:
				{
					bool bImport = ( dlgSync.m_bImport != 0 );
					bool bRemove = ( dlgSync.m_bRemove != 0 );

					if ( ( TRUE == bImport ) || ( TRUE == bRemove ) )
					{
						CString strDateNow;
						COleDateTime dateNow = COleDateTime::GetCurrentTime();
						strDateNow.Format( "%4.4d%2.2d%2.2d", dateNow.GetYear(), dateNow.GetMonth(), dateNow.GetDay() );

						CCustomerCSVArray arrayTemp;
						CCustomerExtractor Extractor( m_nLocIdx, "20110101", strDateNow, arrayTemp );

						int nAdded = 0;
						int nRemoved = 0;
						int nFirstNewID = 0;
						DataManagerNonDb.CustomerAccount.Sync( arrayTemp, DataManagerNonDb.CustomerOneOff, bImport, bRemove, nAdded, nRemoved, nFirstNewID, FALSE );

						bChanged = ( ( nAdded != 0 ) || ( nRemoved != 0 ) );
					}
				}
				break;
			}

			if ( TRUE == bChanged )
			{
				int nSortOrder = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderType();
				DataManagerNonDb.CustomerNameTable.Account.ClearList();
				DataManagerNonDb.CustomerNameTable.Account.BuildListCurrentDb();
				DataManagerNonDb.CustomerNameTable.Account.BuildSortOrder( nSortOrder );
				m_listData.SetItemCountEx ( GetArraySize() );
				SelectLineByID ( nSelectedID );
				m_WarnCancelAfterChange.SetFlag();
			}
		}
	}
}

/**********************************************************************/

void CListDataCustomerAccountDlg::SelectLineByID( int nID )
{
	for ( int n = 0; n < GetArraySize(); n++ )
	{
		int nCustomerIdx = m_pContentHelpers -> GetArrayIdxFromListIdx(n);
		if ( DataManagerNonDb.CustomerAccount.GetCustomerID( nCustomerIdx ) == nID )
		{
			SelectLine(n);
			return;
		}
	}

	SelectLine(0);
}

/**********************************************************************/
