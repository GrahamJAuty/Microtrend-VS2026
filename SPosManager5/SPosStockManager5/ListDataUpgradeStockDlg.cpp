/**********************************************************************/
#include "MessageLogger.h"
#include "ReportHelpers.h"
#include "RepSpawn.h"
#include "StockOptionsIni.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ListDataUpgradeStockDlg.h"
/**********************************************************************/

CSingleListContentHelpersUpgradeStock::CSingleListContentHelpersUpgradeStock( CArray<CUpgradeStockInfo,CUpgradeStockInfo>& arrayUpgrade ) : m_arrayUpgrade( arrayUpgrade )
{
}

/**********************************************************************/

int CSingleListContentHelpersUpgradeStock::GetArraySize()
{
	return m_arrayUpgrade.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersUpgradeStock::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		int nDbIdx = m_arrayUpgrade[nIndex].m_nDbIdx;
		int nPluSize = m_arrayUpgrade[nIndex].m_nPluSize;

		CCSV csv ( '\t' );
		csv.Add( dbDatabase.GetName( nDbIdx ) );
		csv.Add( nPluSize );

		strLine = csv.GetLine();

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataUpgradeStockDlg::CListDataUpgradeStockDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	CWordArray arrayDbIdx;
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int n = 0; n < dbDatabase.GetSize(); n++ )
			arrayDbIdx.Add(n);
	}
	else
		dbDatabase.SortByName( arrayDbIdx );

	for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
	{
		int nDbIdx = arrayDbIdx.GetAt(n);

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );

		if ( DataManager.Stock.GetSize() > 0 )
			continue;

		if ( DataManager.Plu.GetSize() == 0 )
			continue;

		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );
		if ( nEndIdx > nStartIdx )
			continue;

		CUpgradeStockInfo upgrade_info;
		upgrade_info.m_nDbIdx = nDbIdx;
		upgrade_info.m_nPluSize = DataManager.Plu.GetSize();
		m_arrayUpgrade.Add( upgrade_info );
	}

	CDWordArray dw;
	dw.Add( m_arrayUpgrade.GetSize() );
	MessageLogger.LogSystem( 10, 1, TRUE, dw );

	m_bMultiSelect = FALSE;
	m_pContentHelpers = new CSingleListContentHelpersUpgradeStock( m_arrayUpgrade );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataUpgradeStockDlg::~CListDataUpgradeStockDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataUpgradeStockDlg::SetTitle()
{
	SetWindowText ( "Upgrade Ecr Manager Stock" );
}

/**********************************************************************/

void CListDataUpgradeStockDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Upgrade" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.SetWindowText ( "&Check" );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
	m_buttonOK.ShowWindow( SW_HIDE );
	m_buttonCancel.SetWindowText( "Close" );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button4, 368, 107 );
}

/**********************************************************************/

void CListDataUpgradeStockDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.ModifyStyle( 0, LVS_SINGLESEL, 0 );
	m_listData.InsertColumn ( 0, "Database", LVCFMT_LEFT, 340 );
	m_listData.InsertColumn ( 1, "Plu Items", LVCFMT_RIGHT, 140 );
}

/**********************************************************************/

void CListDataUpgradeStockDlg::HandleButton1() 
{
	int nSel = GetSafeSelection();

	if ( nSel < 0 || nSel >= m_arrayUpgrade.GetSize() )
		return;

	CDWordArray dw;
	int nDbIdx = m_arrayUpgrade[nSel].m_nDbIdx;
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );
	dw.Add( nDbNo );

	CStringArray arrayWarning;

	CString strTitle;
	strTitle.Format ( "Upgrade Ecr Manager Stock (%s)", dbDatabase.GetName( nDbIdx ) );
	arrayWarning.Add( strTitle );

	arrayWarning.Add( "One stock item will be created for each stock-enabled Plu." );
	arrayWarning.Add( "" );
	arrayWarning.Add( "Ecr Manager recipes will be converted." );
	arrayWarning.Add( "" );
	arrayWarning.Add( "Apparent and Minimum stock levels will be converted." );
	arrayWarning.Add( "" );
	arrayWarning.Add( "Cost prices and supplier information will be converted." );
	arrayWarning.Add( "" );
	arrayWarning.Add( "Delivery levels and audits will NOT be converted." );

	if ( Prompter.Warning( arrayWarning ) != IDYES )
	{
		Prompter.Info( "The Ecr Manager stock upgrade was cancelled.\n\nYour database has not been changed." );
		MessageLogger.LogSystem( 10, 2, FALSE, dw );
		return;
	}

	//MUST DO THIS BEFORE READING DATA
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );
	
	if ( nEndIdx < nStartIdx )
	{
		if ( dbStockpoint.AddStockpoint( nDbNo ) < 0 )
		{
			Prompter.Error ( "Unable to create a new stockpoint\n\nThe Ecr Manager stock upgrade cannot continue." );
			MessageLogger.LogSystem( 10, 3, FALSE, dw );
			return;
		}
	}

	//MUST DO THIS AFTER CREATING NEW STOCKPOINT
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_PLU );
	arrayReadWrite.Add( NODE_STOCK );
	arrayReadWrite.Add( NODE_SUPPLIER );

	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_MODIFIER );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	int nOldSize = DataManager.Supplier.GetSize();
	int nResult = UpgradeSystemInternal( nDbIdx, info );
	DataManager.CloseDatabase();

	switch( nResult )
	{
	case 0:
		Prompter.Info ( "The stcck upgrade was completed successfully" );

		m_arrayUpgrade.RemoveAt( nSel );
		m_listData.SetItemCountEx( m_arrayUpgrade.GetSize() );
		SelectLine( nSel );

		dw.Add( DataManager.Plu.GetSize() );
		dw.Add( DataManager.Stock.GetSize() );
		dw.Add( DataManager.Supplier.GetSize() - nOldSize );

		MessageLogger.LogSystem( 10, 4, FALSE, dw );
		
		break;

	case 1:
		Prompter.WriteError ( info );
		break;
	}
}

/**********************************************************************/

int CListDataUpgradeStockDlg::UpgradeSystemInternal( int nDbIdx, CDataManagerInfo& info )
{
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );
	int nSpNo = dbStockpoint.GetSpNo( nStartIdx );

	CEcrmanSupplierCSVArray EcrSuppliers;
	CFilenameUpdater FnUp ( SysFiles::EcrmanSupplier );
	EcrSuppliers.Open( FnUp.GetFilenameToUse(), DB_READONLY );
	
	int nOldSize = DataManager.Supplier.GetSize();

	StatusProgress.Lock( TRUE, "Upgrading stock data" );
	for ( int nPluIdx = 0; nPluIdx < DataManager.Plu.GetSize(); nPluIdx++ )
	{
		StatusProgress.SetPos( nPluIdx, DataManager.Plu.GetSize() );

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );

		if ( PluRecord.GetNonStockInternalFlag() == TRUE )
			continue;

		if ( PluRecord.GotEcrmanRecipeTableArray() == TRUE )
			continue;

		CString strPluNo = PluRecord.GetPluNoString();

		CStockCSVRecord StockRecord;
		StockRecord.SetStockCode( strPluNo );
		StockRecord.SetDescription( ReportHelpers.GetReportText( PluRecord ) );
		StockRecord.SetCategory( PluRecord.GetBaseDeptNo() );

		int nPluSuppNo = PluRecord.GetSupplierNo();
		CString strPluSuppName = "";

		int nPluSuppIdx;
		if ( EcrSuppliers.FindSupplierByNumber( nPluSuppNo, nPluSuppIdx ) == TRUE )
		{
			CEcrmanSupplierCSVRecord EcrSupplier;
			EcrSuppliers.GetAt( nPluSuppIdx, EcrSupplier );
			strPluSuppName = EcrSupplier.GetSuppName();
		}

		::TrimSpaces( strPluSuppName, FALSE );
		if ( strPluSuppName == "" )
			strPluSuppName.Format ( "Supplier %d", nPluSuppNo );

		CString strSuppKey;
		strSuppKey.Format ( "%4.4d", nPluSuppNo );

		int nStockSuppNo = 0;

		int nPos;
		if ( DataManager.Supplier.FindSupplierByKey ( strSuppKey, nPos ) == FALSE )
		{
			nStockSuppNo = DataManager.Supplier.FindFreeNumber();

			if ( nStockSuppNo != 0 )
			{
				CSupplierCSVRecord NewSupplier;
				NewSupplier.SetSuppNo ( nStockSuppNo );
				NewSupplier.SetKey ( strSuppKey );
				NewSupplier.SetName( strPluSuppName );
				DataManager.Supplier.AddSupplier ( NewSupplier );
			}
		}
		else
		{
			CSupplierCSVRecord Supplier;
			DataManager.Supplier.GetAt( nPos, Supplier );
			nStockSuppNo = Supplier.GetSuppNo();
		}

		if ( nStockSuppNo != 0 )
		{
			StockRecord.SetSuppNo( 0, nStockSuppNo );
			StockRecord.SetSuppRef( 0, PluRecord.GetSupplierRef() );
			StockRecord.SetWrapper( 0, "" );
			StockRecord.SetDUItems( 0, 1 );
			StockRecord.SetDUItemSize( 0, 1.0 );
			StockRecord.SetCost( 0, PluRecord.GetPurchaseCost() );
			StockRecord.TidySuppliers( StockOptions.GetStockMaxSuppliersPerItem() );
		}

		int nStockIdx;
		DataManager.Stock.InsertRecord( StockRecord, nStockIdx );

		CRecipeCSVRecord Recipe;
		Recipe.SetRecipePluNo( strPluNo );
		Recipe.AddStockCode( strPluNo, 1.0 );
		DataManager.Plu.SetRecipe( nPluIdx, Recipe );
		
		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
		StockLevels.SetOpeningQty( PluRecord.GetApparentLevel() );
		StockLevels.SetMinimumLevelQty( PluRecord.GetMinimumLevel() );
		DataManager.Stock.SetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
	}
	StatusProgress.Unlock();
	
	//NOW DO THE RECIPES
	StatusProgress.Lock( TRUE, "Checking for Recipes" );
	for ( int nPluIdx = 0; nPluIdx < DataManager.Plu.GetSize(); nPluIdx++ )
	{
		StatusProgress.SetPos( nPluIdx, DataManager.Plu.GetSize() );

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );

		CReportConsolidationArray<CEcrmanRecipeItem> arrayItems;
		PluRecord.GetEcrmanRecipeTableArray( arrayItems );
		
		CRecipeCSVRecord RecipeRecord;
		RecipeRecord.SetRecipePluNo( PluRecord.GetPluNoString() );

		for ( int n = 0; n < arrayItems.GetSize(); n++ )
		{
			CEcrmanRecipeItem infoItem;
			arrayItems.GetAt( n, infoItem );

			CPluCSVRecord PluRecipeItem;
			PluRecipeItem.SetPluNo( infoItem.m_nPluNo );

			int nStockIdx;
			CString strStockCode = PluRecipeItem.GetPluNoString();
			if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == TRUE )
			{
				if ( infoItem.m_dStockQty > 0.0 )
				{
					int nRecipeItemIdx;
					if ( RecipeRecord.FindStockCode( strStockCode, nRecipeItemIdx ) == FALSE )
						RecipeRecord.AddStockCode( strStockCode, infoItem.m_dStockQty );
				}
			}
		}

		if ( RecipeRecord.GetSize() > 0 )
			DataManager.Plu.SetRecipe( nPluIdx, RecipeRecord );
	}
	StatusProgress.Unlock();
	
	if ( DataManager.WriteStock( info, TRUE, TRUE ) == FALSE )
		return 1;

	if ( DataManager.WritePlu( info ) == FALSE )
		return 1;

	if ( nOldSize != DataManager.Supplier.GetSize() )
	{
		if ( DataManager.WriteSupplier( info ) == FALSE )
			return 1;
	}
	
	return 0;
}

/**********************************************************************/

void CListDataUpgradeStockDlg::HandleButton4() 
{
	int nSel = GetSafeSelection();

	if ( nSel < 0 || nSel >= m_arrayUpgrade.GetSize() )
		return;

	int nDbIdx = m_arrayUpgrade[nSel].m_nDbIdx;
	
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
	
	CDWordArray dw;
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );
	dw.Add( nDbNo );
	MessageLogger.LogSystem( 10, 5, FALSE, dw );

	CEcrmanSupplierCSVArray EcrSuppliers;
	CFilenameUpdater FnUp ( SysFiles::EcrmanSupplier );
	EcrSuppliers.Open( FnUp.GetFilenameToUse(), DB_READONLY );
	
	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		CString strTitle;
		strTitle.Format ( "Ecr Manager Stock Summary (%s)", dbDatabase.GetName( nDbIdx ) );

		ReportFile.SetStyle1 ( strTitle );
		ReportFile.AddColumn ( "PluNo", TA_LEFT, 380 );
		ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
		ReportFile.AddColumn ( "Supplier", TA_LEFT, 350 );
		ReportFile.AddColumn ( "Reference", TA_LEFT, 350 );
		ReportFile.AddColumn ( "Cost", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "Apparent", TA_RIGHT, 200 );
		ReportFile.AddColumn ( "Minimum", TA_RIGHT, 200 );

		CReportHelpers ReportHelpers;
		
		int nNonStockItem = 0;
		CStringArray arrayRecipes;
		
		StatusProgress.Lock( TRUE, "Creating report" );
		int nSize = DataManager.Plu.GetSize();
		for ( int nPluIdx = 0; nPluIdx < nSize; nPluIdx++ )
		{
			StatusProgress.SetPos( nPluIdx, nSize );

			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );

			CReportConsolidationArray<CEcrmanRecipeItem> arrayRecipeItems;
			PluRecord.GetEcrmanRecipeTableArray( arrayRecipeItems );

			if ( arrayRecipeItems.GetSize() != 0 )
			{
				CCSV csv ( '\t' );
				csv.Add( PluRecord.GetPluNoString() );
				csv.Add( ReportHelpers.GetReportText( PluRecord ) );
				csv.Add( "Recipe" );

				int nItems = arrayRecipeItems.GetSize();
				if ( 1 == nItems )
					csv.Add( "(1 item)" );
				else
				{
					CString str;
					str.Format( "(%d items)", nItems );
					csv.Add( str );
				}

				arrayRecipes.Add( csv.GetLine() );
			}
			else if ( PluRecord.GetNonStockInternalFlag() == FALSE )
			{
				CCSV csv ( '\t' );
				csv.Add( PluRecord.GetPluNoString() );
				csv.Add( ReportHelpers.GetReportText( PluRecord ) );
			
				int nPluSuppNo = PluRecord.GetSupplierNo();
			
				CString strPluSuppName = "";

				int nPluSuppIdx;
				if ( EcrSuppliers.FindSupplierByNumber( nPluSuppNo, nPluSuppIdx ) == TRUE )
				{
					CEcrmanSupplierCSVRecord EcrSupplier;
					EcrSuppliers.GetAt( nPluSuppIdx, EcrSupplier );
					strPluSuppName = EcrSupplier.GetSuppName();
				}

				::TrimSpaces( strPluSuppName, FALSE );
				if ( strPluSuppName == "" )
					strPluSuppName.Format ( "Supplier %d", nPluSuppNo );
			
				csv.Add( strPluSuppName );
				csv.Add( PluRecord.GetSupplierRef() );
				csv.Add( PluRecord.GetPurchaseCost(), SysInfo.GetDPValue() );
				csv.Add( PluRecord.GetApparentLevel(), SysInfo.GetDPQty() );
				csv.Add( PluRecord.GetMinimumLevel(), SysInfo.GetDPQty() );

				ReportFile.WriteLine ( csv.GetLine() );
			}
			else
			{
				nNonStockItem++;
				continue;
			}
		}
		StatusProgress.Unlock();

		if ( arrayRecipes.GetSize() != 0 )
		{
			ReportFile.WriteLine( "" );

			for ( int n = 0; n < arrayRecipes.GetSize(); n++ )
				ReportFile.WriteLine( arrayRecipes.GetAt(n) );
		}

		switch( nNonStockItem )
		{
		case 0:
			break;

		case 1:
			ReportFile.WriteLine( "" );
			ReportFile.WriteLine( "<..>The Plu database also contains 1 non stock item" );
			break;

		default:
			{
				CString strMsg;
				strMsg.Format ( "<..>The Plu database also contains %d non stock items", nNonStockItem );
				ReportFile.WriteLine( "" );
				ReportFile.WriteLine( strMsg );
			}
			break;
		}

		ReportFile.WriteLine( "" );
		ReportFile.Close();

		CReportManager ReportManager;
		CFilenameUpdater FnUp( SysFiles::EcrmanUpgrade );
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
}

/**********************************************************************/