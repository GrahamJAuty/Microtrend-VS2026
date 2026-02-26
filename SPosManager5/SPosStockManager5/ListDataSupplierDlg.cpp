/**********************************************************************/
#include "..\CommonEcrStock\CSVDBImportStatsDlg.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "SupplierEditDlg.h"
#include "SupplierKeyDlg.h"
/**********************************************************************/
#include "ListDataSupplierDlg.h"
/**********************************************************************/

CSingleListContentHelpersSupplier::CSingleListContentHelpersSupplier( CWordArray& arrayProtected ) : m_arrayProtected( arrayProtected )
{
}

/**********************************************************************/

int CSingleListContentHelpersSupplier::GetArraySize()
{
	return DataManager.Supplier.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersSupplier::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CSupplierCSVRecord SupplierRecord;
		DataManager.Supplier.GetAt ( nIndex, SupplierRecord );

		bool bNew = FALSE;
		int nSuppNo = SupplierRecord.GetSuppNo();
		if ( nSuppNo >= 1 && nSuppNo <= Supplier::SupplierNo.Max )
			if ( m_arrayProtected.GetAt( nSuppNo ) == 0 )
				bNew = TRUE;

		CCSV csv ( '\t' );
		csv.Add( SupplierRecord.GetKey() );
		csv.Add( SupplierRecord.GetName() );
		csv.Add( ( bNew == TRUE ) ? "Yes" : "" );
		
		strLine = csv.GetLine();

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersSupplier::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManager.Supplier.GetSize() ) )
	{
		DataManager.Supplier.DeleteSupplier ( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersSupplier::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>&  arraySelected )
{
	m_bPreventDeletion = FALSE;

	for ( int n = arraySelected.GetSize() - 1; n >= 0; n-- )
	{
		CSortedIntItem item;
		arraySelected.GetAt( n, item );

		int nSuppNo = DataManager.Supplier.GetSuppNo( item.m_nItem );
		if ( ( nSuppNo >= 1 ) && ( nSuppNo <= Supplier::SupplierNo.Max ) )
		{
			if ( m_arrayProtected.GetAt( nSuppNo ) != 0 )
			{
				arraySelected.RemoveAt(n);
				m_bPreventDeletion = TRUE;
			}
		}
	}

	m_bPreventDeletion &= ( arraySelected.GetSize() == 0 );
}

/**********************************************************************/

void CSingleListContentHelpersSupplier::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Supplier Key", LVCFMT_LEFT, 150 );
	list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 270 );
	list.InsertColumn ( 2, "New", LVCFMT_LEFT, 60 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataSupplierDlg::CListDataSupplierDlg( int nSuppNo, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nListSelection = -1;
	m_nInitialSuppNo = nSuppNo;

	for ( int n = 0; n <= Supplier::SupplierNo.Max; n++ )
		m_arrayProtected.Add(0);

	for ( int n = 0; n < DataManager.Supplier.GetSize(); n++ )
	{
		int nSuppNo = DataManager.Supplier.GetSuppNo(n);
		if ( nSuppNo >= 1 && nSuppNo <= Supplier::SupplierNo.Max )
			m_arrayProtected.SetAt( nSuppNo, 1 );
	}

	m_pContentHelpers = new CSingleListContentHelpersSupplier( m_arrayProtected );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "supplier" );
	m_pDisplayHelpers -> SetTopPluralText( "suppliers" );
	m_pDisplayHelpers -> SetBottomSingleText( "supplier" );
	m_pDisplayHelpers -> SetBottomPluralText( "suppliers" );
}

/**********************************************************************/

CListDataSupplierDlg::~CListDataSupplierDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataSupplierDlg::SetTitle()
{
	SetWindowText ( "Setup Stock Supplier Texts" );
}

/**********************************************************************/

void CListDataSupplierDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "&Print" );
	m_button5.SetWindowText ( "&Import" );
	m_button6.ShowWindow ( SW_HIDE );

	m_button4.EnableWindow ( DataManager.Supplier.GetSize() > 0 );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataSupplierDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataSupplierDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	for ( int n = 0; n < DataManager.Supplier.GetSize(); n++ )
		if ( DataManager.Supplier.GetSuppNo(n) == m_nInitialSuppNo )
			SelectLine(n);
}

/**********************************************************************/

void CListDataSupplierDlg::HandleButton1() 
{
	int nSuppNo = DataManager.Supplier.FindFreeNumber();
	if ( nSuppNo == 0 )
	{
		Prompter.Error ( "You have already created the\nmaximum number of suppliers." );
		return;
	}
	
	CSupplierKeyDlg dlgSupplierKey ( TRUE, FALSE, this );
	if ( dlgSupplierKey.DoModal() != IDOK )
		return;

	CString strKey = dlgSupplierKey.m_strKey;
	if ( strKey == "" )
		return;

	int nPos;
	if ( DataManager.Supplier.FindSupplierByKey ( strKey, nPos ) == TRUE )
	{
		CString strMsg = "You have already created a supplier\nwith key ";
		strMsg += strKey;
		Prompter.Error ( strMsg );
		return;
	}

	CSupplierCSVRecord NewSupplier;
	NewSupplier.SetSuppNo ( nSuppNo );
	NewSupplier.SetKey ( strKey );
	NewSupplier.SetName( strKey );
	int nSuppIdx = DataManager.Supplier.AddSupplier ( NewSupplier );
	m_listData.SetItemCountEx ( GetArraySize() );
	
	SelectLine ( nSuppIdx );
	CSupplierEditDlg dlg ( "Add Supplier", nSuppIdx, this );
	dlg.DoModal();

	SelectLine ( dlg.GetSuppIdx() );
	m_button4.EnableWindow ( TRUE );	

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSupplierDlg::HandleButton2() 
{
	int nSuppIdx = GetSafeSelection();
	if ( nSuppIdx == -1 )
		return;

	CSupplierCSVRecord SupplierRecord;
	DataManager.Supplier.GetAt ( nSuppIdx, SupplierRecord );
	CSupplierEditDlg dlg ( "Edit Supplier", nSuppIdx, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		m_WarnCancelAfterChange.SetFlag();
	}

	SelectLine ( dlg.GetSuppIdx() );	
}
		
/**********************************************************************/
	
void CListDataSupplierDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete(this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );

	if ( m_pContentHelpers -> GetPreventDeletionFlag() )
		Prompter.Error( "You cannot delete suppliers that were added\nbefore the start of this editing session." );

	if ( DataManager.Supplier.GetSize() == 0 )
		m_button4.EnableWindow ( FALSE );
}

/**********************************************************************/

void CListDataSupplierDlg::HandleButton4()  
{
	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		ReportFile.SetStyle1 ( "Supplier List" );
		ReportFile.AddColumn ( "Supplier Key", TA_LEFT, 350 );
		ReportFile.AddColumn ( "Supplier", TA_LEFT, 350 );
		ReportFile.AddColumn ( "Address", TA_LEFT, 400 );
		ReportFile.AddColumn ( "Postcode", TA_LEFT, 200 );
		ReportFile.AddColumn ( "Phone", TA_LEFT, 200 );
		ReportFile.AddColumn ( "Fax", TA_LEFT, 200 );
		ReportFile.AddColumn ( "Email", TA_LEFT, 200 );
		ReportFile.AddColumn ( "Account No", TA_LEFT, 200 );
		ReportFile.AddColumn ( "FAO", TA_LEFT, 350 );
		
		for ( int nSuppIdx = 0; nSuppIdx < DataManager.Supplier.GetSize(); nSuppIdx++ )
		{
			CCSV csv ( '\t' );
			
			CSupplierCSVRecord SupplierRecord;
			DataManager.Supplier.GetAt ( nSuppIdx, SupplierRecord );

			csv.Add ( SupplierRecord.GetKey() );
			csv.Add ( SupplierRecord.GetName() );

			CString strAddress;
			strAddress.Format ( "%s %s %s %s %s",
				SupplierRecord.GetAddress1(),
				SupplierRecord.GetAddress2(),
				SupplierRecord.GetAddress3(),
				SupplierRecord.GetAddress4(),
				SupplierRecord.GetAddress5() );

			csv.Add ( strAddress );
			csv.Add ( SupplierRecord.GetPostcode() );
			csv.Add ( SupplierRecord.GetPhone() );
			csv.Add ( SupplierRecord.GetFax() ) ;
			csv.Add ( SupplierRecord.GetEmail() );
			csv.Add ( SupplierRecord.GetAccountNo() );
			csv.Add ( SupplierRecord.GetFAO() );
			ReportFile.WriteLine ( csv.GetLine() );
			ReportFile.WriteLine ( "<LI>" );
		}
		ReportFile.Close();

		CReportManager ReportManager;
		CFilenameUpdater FnUp( SysFiles::SupplierPrm );
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
}

/**********************************************************************/

void CListDataSupplierDlg::HandleButton5()
{
	CString strImportFolder = "";
	CFilenameUpdater FnUp( SysFiles::SupplierImport );
	
	{
		CSSFile fileImportFolder;
		if ( fileImportFolder.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
			fileImportFolder.ReadString( strImportFolder );
	}

	CString strTitle = "Import Supplier CSV File";
	CFileDialog dlgFilename ( TRUE, "csv", "supplier", OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFilename.m_ofn.lpstrTitle = strTitle;
	dlgFilename.m_ofn.lpstrInitialDir = strImportFolder;
	
	if ( dlgFilename.DoModal() != IDOK )
		return;

	CString strFilename = dlgFilename.GetPathName();

	CSSImportFile fileImport ( CSupplierCSVRecord::GetHeader() );
	if ( fileImport.Open ( strFilename, "rb" ) == FALSE )
	{
		Prompter.Error( "Unable to open import file" );
		return;
	}

	int nTotalLines = 0;
	int nAdded = 0;
	int nModified = 0;
	int nIgnored = 0;

	StatusProgress.Lock();

	CString strBuffer;
	while ( fileImport.ReadData ( strBuffer ) == TRUE )
	{	
		StatusProgress.SetPos ( fileImport );
		
		int nResult = DataManager.Supplier.ImportLine( fileImport, strBuffer );
		
		switch ( nResult )
		{
		case IMPORT_LINE_ADDED:		
			nTotalLines++;
			nAdded++;
			break;

		case IMPORT_LINE_MODIFIED:	
			nTotalLines++;
			nModified++;
			break;
				
		case IMPORT_LINE_IGNORED:
			nTotalLines++;
			nIgnored++;
			break;
		}
	}
	fileImport.Close();
	
	StatusProgress.Unlock();

	CString strPath = dlgFilename.GetPathName();
	CString strFile = dlgFilename.GetFileName();

	if ( strPath.GetLength() >= strFile.GetLength() )
	{
		strPath = strPath.Left ( strPath.GetLength() - strFile.GetLength() );

		CSSFile fileImportFolder;
		if ( fileImportFolder.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
			fileImportFolder.WriteLine( strPath );
	}

	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( 0 );

	CCSVDBImportStatsDlg dlg( NODE_SUPPLIER, this );
	dlg.m_nTotalLines = nTotalLines;
	dlg.m_nIgnoredLines = nIgnored;
	dlg.m_nRecordsAdded = nAdded;
	dlg.m_nRecordsModified = nModified;
	dlg.DoModal();

	m_button4.EnableWindow ( DataManager.Supplier.GetSize() > 0 );

	if ( ( nAdded > 0 ) || ( nModified > 0 ) )
	{
		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/
