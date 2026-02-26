/**********************************************************************/
#include "CustomExportManager.h"
#include "FileRemover.h"
#include "KeyboardHelpers.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
/**********************************************************************/
#include "PropertySheetWizardImport.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetWizardImport, CPropertySheet)
/**********************************************************************/

CPropertySheetWizardImport::CPropertySheetWizardImport( CSQLImporter& SQLImporter, CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent), m_SQLImporter( SQLImporter )
{	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
	m_Page1.SetLocationType( NODE_SYSTEM );
	m_Page2.SetLocationType( NODE_DATABASE );
	m_Page3.SetLocationType( NODE_LOCATION );
	m_Page4.SetLocationType( NODE_SERVER );

	if ( m_Page1.IsRelevant() == TRUE )
	{
		AddPage( &m_Page1 );
		m_Page1.m_psp.dwFlags |= PSP_USETITLE;
		m_Page1.m_psp.pszTitle = "System";
		m_Page1.SetSQLImporter( &m_SQLImporter );
	}

	if ( m_Page2.IsRelevant() == TRUE )
	{
		AddPage( &m_Page2 );
		m_Page2.m_psp.dwFlags |= PSP_USETITLE;
		m_Page2.m_psp.pszTitle = "Database";
		m_Page2.SetSQLImporter( &m_SQLImporter );
	}

	bool bGotLinkedLoc = FALSE;
	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{
		int nDbIdx;
		int nDbNo = dbLocation.GetDbNo( nLocIdx );
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
		{
			bGotLinkedLoc = TRUE;
			break;
		}
	}

	if ( TRUE == bGotLinkedLoc )
	{
		if ( m_Page3.IsRelevant() == TRUE )
		{
			AddPage( &m_Page3 );
			m_Page3.m_psp.dwFlags |= PSP_USETITLE;
			m_Page3.m_psp.pszTitle = "Location";
			m_Page3.SetSQLImporter( &m_SQLImporter );
		}
		else if ( m_Page4.IsRelevant() == TRUE )
		{
			AddPage( &m_Page4 );
			m_Page4.m_psp.dwFlags |= PSP_USETITLE;
			m_Page4.m_psp.pszTitle = "Location";
			m_Page4.SetSQLImporter( &m_SQLImporter );
		}
	}

	SetTitle ( "Import Wizard Data" );

	m_bImportedData = FALSE;
}

/**********************************************************************/

CPropertySheetWizardImport::~CPropertySheetWizardImport()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetWizardImport, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetWizardImport)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetWizardImport::ImportWizardData()
{
	int nDbIdx = m_Page2.GetDbIdx();
	int nLocIdx = m_Page3.GetLocIdx();

	if ( m_Page1.IsRelevant() == TRUE )
		m_Page1.GetFileTypeTicks( m_infoImport );
	
	if ( m_Page2.IsRelevant() == TRUE )
		m_Page2.GetFileTypeTicks( m_infoImport );
	
	if ( m_Page3.IsRelevant() == TRUE )
		m_Page3.GetFileTypeTicks( m_infoImport );

	if ( m_Page4.IsRelevant() == TRUE )
	{
		m_Page4.GetFileTypeTicks( m_infoImport );
		nLocIdx = m_Page4.GetLocIdx();
	}

	m_SQLImporter.Reset();
	m_SQLImporter.SetWantBarcodeFlag( m_infoImport.m_nBarcode != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantDeptFlag( m_infoImport.m_nDept != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantCategoryFlag( m_infoImport.m_nCategory != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantPaidInOutTextFlag( m_infoImport.m_nPaidInOutText != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantGroupFlag( m_infoImport.m_nGroup != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantAllergenFlag( m_infoImport.m_nAllergen != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantMixMatchFlag( m_infoImport.m_nMixMatch2 != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantWalletStampRuleFlag(m_infoImport.m_nWalletStampRule != WIZARD_IMPORT_NONE);
	m_SQLImporter.SetWantTimePlanFlag( m_infoImport.m_nTimePlan != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantModifierFlag( m_infoImport.m_nModifier != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantPayMethFlag( m_infoImport.m_nPayment != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantServerFlag( m_infoImport.m_nServer != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantServerLogonFlag(m_infoImport.m_nServerLogon != WIZARD_IMPORT_NONE);
	m_SQLImporter.SetWantServerRestrictFlag( m_infoImport.m_nServerRestrict != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantPluFlag( m_infoImport.m_nPlu != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetCreateStockFlag( m_infoImport.m_nStock != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantTaxRateFlag( m_infoImport.m_nTaxRate != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantLoyaltySchemeFlag( m_infoImport.m_nLoyaltyScheme != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantTableAreaFlag( m_infoImport.m_nTableArea != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantTableNameFlag( m_infoImport.m_nTableName != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantFunctionSystemFlag( m_infoImport.m_nFunctionSystem != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantFunctionDatabaseType( m_infoImport.m_nFunctionDatabase );
	m_SQLImporter.SetWantFunctionLocationType( m_infoImport.m_nFunctionLocation );
	m_SQLImporter.SetWantCCMacroSystemFlag( m_infoImport.m_nCCMacroSystem != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantCCMacroDatabaseFlag( m_infoImport.m_nCCMacroDatabase != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantCCMacroLocationFlag( m_infoImport.m_nCCMacroLocation != WIZARD_IMPORT_NONE );
	m_SQLImporter.SetWantCCReasonSystemFlag(m_infoImport.m_nCCReasonSystem != WIZARD_IMPORT_NONE);
	m_SQLImporter.SetWantCCReasonDatabaseFlag(m_infoImport.m_nCCReasonDatabase != WIZARD_IMPORT_NONE);
	m_SQLImporter.SetWantCCReasonLocationFlag(m_infoImport.m_nCCReasonLocation != WIZARD_IMPORT_NONE);

	if ( m_SQLImporter.GetFileTypeCount() == 0 )
	{
		Prompter.Error ( "You have not selected any file types to import" );
		return;
	}

	CWordArray arrayFileTypes;
	
	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nBarcode )		
		arrayFileTypes.Add( NODE_BARCODE );
	
	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nDept )		
		arrayFileTypes.Add( NODE_DEPARTMENT );
	
	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nGroup )		
		arrayFileTypes.Add( NODE_DEPT_GROUP_EPOS );

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nPayment )
		arrayFileTypes.Add( NODE_PAYMENT );
		
	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nCategory )
		arrayFileTypes.Add( NODE_ACAT );

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nPaidInOutText )
	{
		arrayFileTypes.Add( NODE_PAIDIN_TEXT );
		arrayFileTypes.Add( NODE_PAIDOUT_TEXT );
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nModifier )
		arrayFileTypes.Add( NODE_MODIFIER );

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nPlu )			
	{
		arrayFileTypes.Add( NODE_PLU );
#ifdef STOCKMAN_SYSTEM
		arrayFileTypes.Add( NODE_STOCK );
#endif
	}
		
	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nMixMatch2 )
	{
		arrayFileTypes.Add( NODE_MIXMATCH );
		arrayFileTypes.Add( NODE_PROMOTION );
	}

	if (WIZARD_IMPORT_NONE != m_infoImport.m_nWalletStampRule)
	{
		arrayFileTypes.Add(NODE_WALLETSTAMPRULE);
	}

	if ((WIZARD_IMPORT_NONE != m_infoImport.m_nMixMatch2) || (WIZARD_IMPORT_NONE != m_infoImport.m_nWalletStampRule))
	{
		arrayFileTypes.Add(NODE_OFFER_GROUP);
		arrayFileTypes.Add(NODE_OFFER_ENTRY);
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nTimePlan )
	{
		arrayFileTypes.Add( NODE_TIMEPLAN );
		arrayFileTypes.Add( NODE_TIMEPLAN_ENTRY );
	}

	if (WIZARD_IMPORT_NONE != m_infoImport.m_nTaxRate)
	{
		arrayFileTypes.Add(NODE_TAX);
	}

	if (WIZARD_IMPORT_NONE != m_infoImport.m_nLoyaltyScheme)
	{
		arrayFileTypes.Add(NODE_LOYALTY_SCHEME);
	}

	if ( arrayFileTypes.GetSize() > 0 )
	{
		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayFileTypes, info, FALSE ) == FALSE )
		{
			Prompter.ShareError( info );
			return;
		}
	}

	CDataManagerInfo info;

	CWordArray arrayNonDbFileTypes;

	if (WIZARD_IMPORT_NONE != m_infoImport.m_nAllergen)
	{
		if (DataManagerNonDb.OpenAllergen(DB_READWRITE, info) == FALSE)
		{
			DataManager.CloseDatabase();
			Prompter.ShareError(info);
			return;
		}
		else
		{
			arrayNonDbFileTypes.Add(NODE_ALLERGEN);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nTableArea )
	{
		if ( DataManagerNonDb.OpenTableAreas( DB_READWRITE, nLocIdx, info ) == FALSE )
		{
			DataManagerNonDb.CloseFiles( arrayNonDbFileTypes );
			DataManager.CloseDatabase();	
			Prompter.ShareError( info );
			return;
		}
		else
		{
			arrayNonDbFileTypes.Add(NODE_TABLE_AREA);
		}
	}

	if (WIZARD_IMPORT_NONE != m_infoImport.m_nTableName)
	{
		if (DataManagerNonDb.OpenTableNames(DB_READWRITE, nLocIdx, info) == FALSE)
		{
			DataManagerNonDb.CloseFiles(arrayNonDbFileTypes);
			DataManager.CloseDatabase();
			Prompter.ShareError(info);
			return;
		}
		else
		{
			arrayNonDbFileTypes.Add(NODE_TABLE_NAME);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nFunctionSystem )
	{
		if ( DataManagerNonDb.OpenFunctionSystem( DB_READWRITE, info ) == FALSE )
		{
			DataManagerNonDb.CloseFiles( arrayNonDbFileTypes );
			DataManager.CloseDatabase();	
			Prompter.ShareError( info );
			return;
		}
		else
		{
			arrayNonDbFileTypes.Add(NODE_FUNCTION_SYSTEM);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nFunctionDatabase )
	{
		if ( DataManagerNonDb.OpenFunctionDatabase( nDbIdx, DB_READWRITE, info ) == FALSE )
		{
			DataManagerNonDb.CloseFiles( arrayNonDbFileTypes );
			DataManager.CloseDatabase();	
			Prompter.ShareError( info );
			return;
		}
		else
		{
			arrayNonDbFileTypes.Add(NODE_FUNCTION_DATABASE);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nFunctionLocation )
	{
		if ( DataManagerNonDb.OpenFunctionLocation( nLocIdx, DB_READWRITE, info ) == FALSE )
		{
			DataManagerNonDb.CloseFiles( arrayNonDbFileTypes );
			DataManager.CloseDatabase();	
			Prompter.ShareError( info );
			return;
		}
		else
		{
			arrayNonDbFileTypes.Add(NODE_FUNCTION_LOCATION);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nCCMacroSystem )
	{
		if ( DataManagerNonDb.OpenCCMacroSystem( DB_READWRITE, info ) == FALSE )
		{
			DataManagerNonDb.CloseFiles( arrayNonDbFileTypes );
			DataManager.CloseDatabase();	
			Prompter.ShareError( info );
			return;
		}
		else
		{
			arrayNonDbFileTypes.Add(NODE_CCMACRO_SYSTEM);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nCCMacroDatabase )
	{
		if ( DataManagerNonDb.OpenCCMacroDatabase( nDbIdx, DB_READWRITE, info ) == FALSE )
		{
			DataManagerNonDb.CloseFiles( arrayNonDbFileTypes );
			DataManager.CloseDatabase();	
			Prompter.ShareError( info );
			return;
		}
		else
		{
			arrayNonDbFileTypes.Add(NODE_CCMACRO_DATABASE);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nCCMacroLocation )
	{
		if ( DataManagerNonDb.OpenCCMacroLocation( nLocIdx, DB_READWRITE, info ) == FALSE )
		{
			DataManagerNonDb.CloseFiles( arrayNonDbFileTypes );
			DataManager.CloseDatabase();	
			Prompter.ShareError( info );
			return;
		}
		else
		{
			arrayNonDbFileTypes.Add(NODE_CCMACRO_LOCATION);
		}
	}

	if (WIZARD_IMPORT_NONE != m_infoImport.m_nCCReasonSystem)
	{
		if (DataManagerNonDb.OpenCCReasonSystem(DB_READWRITE, REASONTEXT_TYPE_ITEMVOID, info) == FALSE)
		{
			DataManagerNonDb.CloseFiles(arrayNonDbFileTypes);
			DataManager.CloseDatabase();
			Prompter.ShareError(info);
			return;
		}
		else
		{
			arrayNonDbFileTypes.Add(NODE_CCREASON_SYSTEM);
		}
	}

	if (WIZARD_IMPORT_NONE != m_infoImport.m_nCCReasonDatabase)
	{
		if (DataManagerNonDb.OpenCCReasonDatabase(nDbIdx, DB_READWRITE, REASONTEXT_TYPE_ITEMVOID, info) == FALSE)
		{
			DataManagerNonDb.CloseFiles(arrayNonDbFileTypes);
			DataManager.CloseDatabase();
			Prompter.ShareError(info);
			return;
		}
		else
		{
			arrayNonDbFileTypes.Add(NODE_CCREASON_DATABASE);
		}
	}

	if (WIZARD_IMPORT_NONE != m_infoImport.m_nCCReasonLocation)
	{
		if (DataManagerNonDb.OpenCCReasonLocation(nLocIdx, DB_READWRITE, REASONTEXT_TYPE_ITEMVOID, info) == FALSE)
		{
			DataManagerNonDb.CloseFiles(arrayNonDbFileTypes);
			DataManager.CloseDatabase();
			Prompter.ShareError(info);
			return;
		}
		else
		{
			arrayNonDbFileTypes.Add(NODE_CCREASON_LOCATION);
		}
	}

	if ( ( WIZARD_IMPORT_NONE != m_infoImport.m_nServer ) || ( WIZARD_IMPORT_NONE != m_infoImport.m_nServerRestrict ) )
	{
		int nServerDbIdx = nDbIdx;
		int nServerLocIdx = nLocIdx;

		switch( EcrmanOptions.GetFeaturesServerSettingsType() )
		{
		case SERVER_SYS:
			nServerDbIdx = -1;
			nServerLocIdx = -1;
			break;

		case SERVER_DB:
			if ( m_Page4.IsRelevant() == TRUE )
			{
				int nDbNo = dbLocation.GetDbNo( nLocIdx );
				dbDatabase.FindDatabaseByNumber( nDbNo, nServerDbIdx );
			}
			nServerLocIdx = -1;
			break;
		}
		
		if ( ( WIZARD_IMPORT_NONE != m_infoImport.m_nServer ) ||
			(WIZARD_IMPORT_NONE != m_infoImport.m_nServerLogon) || 
			(WIZARD_IMPORT_NONE != m_infoImport.m_nServerRestrict) )
		{
			int nMode = (WIZARD_IMPORT_NONE != m_infoImport.m_nServer) ? DB_READWRITE : DB_READONLY;
			
			if ( DataManagerNonDb.OpenServer( nMode, nServerDbIdx, nServerLocIdx, info ) == FALSE )
			{
				DataManagerNonDb.CloseFiles( arrayNonDbFileTypes );
				DataManager.CloseDatabase();	
				Prompter.ShareError( info );
				return;
			}
			else
			{
				arrayNonDbFileTypes.Add(NODE_SERVER);
			}
		}

		if (WIZARD_IMPORT_NONE != m_infoImport.m_nServerLogon)
		{
			if (DataManagerNonDb.OpenServerLogon(DB_READWRITE, nServerDbIdx, nServerLocIdx, info) == FALSE)
			{
				DataManagerNonDb.CloseFiles(arrayNonDbFileTypes);
				DataManager.CloseDatabase();
				Prompter.ShareError(info);
				return;
			}
			else
			{
				arrayNonDbFileTypes.Add(NODE_SERVERLOGON);
			}
		}
		
		if ( WIZARD_IMPORT_NONE != m_infoImport.m_nServerRestrict )
		{
			if ( DataManagerNonDb.OpenServerRestriction( DB_READWRITE, nLocIdx, info ) == FALSE )
			{
				DataManagerNonDb.CloseFiles( arrayNonDbFileTypes );
				DataManager.CloseDatabase();	
				Prompter.ShareError( info );
				return;
			}
			else
			{
				arrayNonDbFileTypes.Add( NODE_SERVER_RESTRICT );
				DataManagerNonDb.ServerRestriction.RemoveAll();
			}
		}
	}

	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
	
	if ( ( WIZARD_IMPORT_NONE != m_infoImport.m_nMixMatch2 ) || ( WIZARD_IMPORT_NONE != m_infoImport.m_nWalletStampRule ) )
	{
		CWordArray arrayOfferGroups;
		m_SQLImporter.GetOfferGroupList( arrayOfferGroups );

		DataManager.OfferGroupEntry.Validate();

		for (int n = 0; n < arrayOfferGroups.GetSize(); n++)
		{
			DataManager.OfferGroupEntry.PurgeGroup(arrayOfferGroups.GetAt(n));
		}
	}

	//CLEAR DATA IF REQUESTED
	if (WIZARD_IMPORT_CLEAR == m_infoImport.m_nBarcode)
	{
		DataManager.Barcode.RemoveAt(0, DataManager.Barcode.GetSize());
	}

	if (WIZARD_IMPORT_CLEAR == m_infoImport.m_nDept)
	{
		DataManager.Department.RemoveAt(0, DataManager.Department.GetSize());
	}

	if (WIZARD_IMPORT_CLEAR == m_infoImport.m_nGroup)
	{
		DataManager.EposGroup.RemoveAt(0, DataManager.EposGroup.GetSize());
	}

	if (WIZARD_IMPORT_CLEAR == m_infoImport.m_nAllergen)
	{
		DataManagerNonDb.Allergen.RemoveAt(0, DataManagerNonDb.Allergen.GetSize());
	}

	if (WIZARD_IMPORT_CLEAR == m_infoImport.m_nPayment)
	{
		DataManager.Payment.RemoveAt(0, DataManager.Payment.GetSize());
	}

	if (WIZARD_IMPORT_CLEAR == m_infoImport.m_nLoyaltyScheme)
	{
		DataManager.LoyaltyScheme.RemoveAt(0, DataManager.LoyaltyScheme.GetSize());
	}

	if ( WIZARD_IMPORT_CLEAR == m_infoImport.m_nCategory )
	{
		DataManager.AnalysisCategory.RemoveAt( 0, DataManager.AnalysisCategory.GetSize() );
		DataManager.AnalysisCategory.FillArray();
	}

	if ( WIZARD_IMPORT_CLEAR == m_infoImport.m_nPaidInOutText )
	{
		DataManager.PaidInText.RemoveAt( 0, DataManager.PaidInText.GetSize() );
		DataManager.PaidInText.FillArray();
		DataManager.PaidOutText.RemoveAt( 0, DataManager.PaidOutText.GetSize() );
		DataManager.PaidOutText.FillArray();
	}

	if ( WIZARD_IMPORT_CLEAR == m_infoImport.m_nModifier )
	{
		for ( int nMod = 1; nMod <= MODIFIER_COUNT; nMod++ )
		{
			DataManager.Modifier.SetMultiplierForQty( nMod, 1.0 );
			DataManager.Modifier.SetMultiplierForPoints( nMod, 1.0 );
		}
	}

	if ( WIZARD_IMPORT_CLEAR == m_infoImport.m_nTaxRate )
	{
		for ( int nTaxRate = 1; nTaxRate <= MAX_TAX_BANDS; nTaxRate++ )
		{
			CTaxRateInfo TaxRateInfo;
			DataManager.TaxRates.GetTaxRateInfo( nTaxRate, TaxRateInfo );
			TaxRateInfo.m_dTaxRate = 0.0;
			DataManager.TaxRates.SetTaxRateInfo( nTaxRate, TaxRateInfo );
		}
	}

	if ( WIZARD_IMPORT_CLEAR == m_infoImport.m_nTableArea )
	{
		DataManagerNonDb.TableAreas.RemoveAt( 0, DataManagerNonDb.TableAreas.GetSize() );
	}

	if ( WIZARD_IMPORT_CLEAR == m_infoImport.m_nTableName )
	{
		DataManagerNonDb.TableNames.RemoveAt( 0, DataManagerNonDb.TableNames.GetSize() );
	}

	if ( WIZARD_IMPORT_CLEAR == m_infoImport.m_nMixMatch2 )
	{
		DataManager.MixMatch.RemoveAt( 0, DataManager.MixMatch.GetSize() );
		DataManager.Promotion.RemoveAt( 0, DataManager.Promotion.GetSize() );
	}

	if (WIZARD_IMPORT_CLEAR == m_infoImport.m_nWalletStampRule)
	{
		DataManager.WalletStampRule.RemoveAt(0, DataManager.WalletStampRule.GetSize());
	}

	if ( (WIZARD_IMPORT_CLEAR == m_infoImport.m_nMixMatch2) || (WIZARD_IMPORT_CLEAR == m_infoImport.m_nWalletStampRule) )
	{
		DataManager.OfferGroup.RemoveAt(0, DataManager.OfferGroup.GetSize());
		DataManager.OfferGroupEntry.RemoveAt(0, DataManager.OfferGroupEntry.GetSize());
	}

	if ( WIZARD_IMPORT_CLEAR == m_infoImport.m_nTimePlan )
	{
		DataManager.TimePlan.RemoveAt( 0, DataManager.TimePlan.GetSize() );
		DataManager.TimePlanEntry.RemoveAt( 0, DataManager.TimePlanEntry.GetSize() );
	}

	if ( WIZARD_IMPORT_CLEAR == m_infoImport.m_nPlu )
	{
		int nPluIdx = DataManager.Plu.GetSize();

		while( --nPluIdx >= 0 )
		{
			DataManager.ResourceWrapperPlu.FlagDeletedPlu( DataManager.Plu.GetPluNoInt( nPluIdx ) );
			DataManager.ListKeyImageWrapperPlu.FlagDeletedPlu( DataManager.Plu.GetPluNoInt( nPluIdx ) );
			DataManager.Plu.RemoveAt( nPluIdx );
		}
	}

	if (WIZARD_IMPORT_CLEAR == m_infoImport.m_nServer)
	{
		DataManagerNonDb.Server.RemoveAt(0, DataManagerNonDb.Server.GetSize());
	}

	if (WIZARD_IMPORT_CLEAR == m_infoImport.m_nServerLogon)
	{
		DataManagerNonDb.ServerLogon.RemoveAt(0, DataManagerNonDb.ServerLogon.GetSize());
	}

	if ( WIZARD_IMPORT_CLEAR == m_infoImport.m_nServerPreset )
	{
		int nNetWkIdx = 0;
		if (dbNetwork.FindNetworkByNumber(dbLocation.GetNetworkNo(nLocIdx), nNetWkIdx) == FALSE)
		{
			nNetWkIdx = 0;
		}

		for ( int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount( nLocIdx ); nTermIdx++ )
		{
			int nTNo = dbLocation.GetTNo( nLocIdx, nTermIdx );
			CFilenameUpdater FnUp ( SysFiles::ServerPreset, nNetWkIdx, nTNo, FNUP_FAMILY_TERM );
			CFileRemover FileRemover( FnUp.GetFilenameToUse() );
		}
	}

	if (WIZARD_IMPORT_CLEAR == m_infoImport.m_nCCMacroSystem)
	{
		DataManagerNonDb.CCMacroSystem.RemoveAt(0, DataManagerNonDb.CCMacroSystem.GetSize());
	}

	if (WIZARD_IMPORT_CLEAR == m_infoImport.m_nCCMacroDatabase)
	{
		DataManagerNonDb.CCMacroDatabase.RemoveAt(0, DataManagerNonDb.CCMacroDatabase.GetSize());
	}

	if (WIZARD_IMPORT_CLEAR == m_infoImport.m_nCCMacroLocation)
	{
		DataManagerNonDb.CCMacroLocation.RemoveAt(0, DataManagerNonDb.CCMacroLocation.GetSize());
	}

	m_SQLImporter.SetClearReasonSystemFlag(WIZARD_IMPORT_CLEAR == m_infoImport.m_nCCReasonSystem);
	m_SQLImporter.SetClearReasonDatabaseFlag(WIZARD_IMPORT_CLEAR == m_infoImport.m_nCCReasonDatabase);
	m_SQLImporter.SetClearReasonLocationFlag(WIZARD_IMPORT_CLEAR == m_infoImport.m_nCCReasonLocation);

	//IMPORT AND SAVE DATA
	m_SQLImporter.SetServerPresetLocIdx( m_Page4.GetLocIdx() );
	m_SQLImporter.ImportFile( TRUE, nDbIdx, nLocIdx );

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nBarcode )
	{
		if (DataManager.WriteBarcode(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nDept )
	{
		if (DataManager.WriteDepartment(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nGroup )
	{
		if (DataManager.WriteEposGroup(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nAllergen )
	{
		if (DataManagerNonDb.WriteAllergen(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else
		{
			DataManagerNonDb.Allergen.WriteSMAllergenFile();
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nPayment )
	{
		if ( DataManager.WritePayment( info ) == TRUE )
		{
			DataManager.Payment.WriteSMPaymentFile();
			DataManager.Payment.WriteSMPaymentGroupFile();
		}
		else
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nCategory )
	{
		if (DataManager.WriteAnalysisCategory(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nPaidInOutText )
	{
		if (DataManager.WritePaidInText(info) == FALSE)
		{
			Prompter.WriteError(info);
		}

		if (DataManager.WritePaidOutText(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nModifier )
	{
		if (DataManager.WriteModifier(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nTaxRate )
	{
		if (DataManager.WriteTaxRates(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nLoyaltyScheme )
	{
		if (DataManager.WriteLoyaltyScheme(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nTableArea )
	{
		if (DataManagerNonDb.WriteTableAreas(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nTableName )
	{
		if (DataManagerNonDb.WriteTableNames(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nMixMatch2 )
	{
		if (DataManager.WriteMixMatch(info) == FALSE)
		{
			Prompter.WriteError(info);
		}

		if (DataManager.WritePromotion(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if (WIZARD_IMPORT_NONE != m_infoImport.m_nWalletStampRule)
	{
		if (DataManager.WriteWalletStampRule(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( (WIZARD_IMPORT_NONE != m_infoImport.m_nMixMatch2) || (WIZARD_IMPORT_NONE != m_infoImport.m_nWalletStampRule) )
	{
		if (DataManager.WriteOfferGroup(info) == FALSE)
		{
			Prompter.WriteError(info);
		}

		if (DataManager.WriteOfferGroupEntry(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nTimePlan )
	{
		if (DataManager.WriteTimePlan(info) == FALSE)
		{
			Prompter.WriteError(info);
		}

		if (DataManager.WriteTimePlanEntry(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nPlu )		
	{
		if (DataManager.WritePlu(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if ( WIZARD_IMPORT_CLEAR == m_infoImport.m_nPlu )
		{
			DataManager.ResourceWrapperPlu.SaveDatabaseChanges();
			DataManager.ListKeyImageWrapperPlu.SaveDatabaseChanges();
		}

		CKeyboardHelpers::RemovePluFile( nDbIdx );

#ifdef STOCKMAN_SYSTEM			
		if (DataManager.WriteStock(info, TRUE, TRUE) == FALSE)
		{
			Prompter.WriteError(info);
		}
#endif

		CCustomExportManagerPlu Export;
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nServer )
	{
		if (DataManagerNonDb.WriteServer(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if (WIZARD_IMPORT_NONE != m_infoImport.m_nServerLogon)
	{
		if (DataManagerNonDb.WriteServerLogon(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nServerRestrict )
	{
		if (DataManagerNonDb.WriteServerRestriction(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nFunctionSystem )
	{
		if (DataManagerNonDb.WriteFunctionSystem(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nFunctionDatabase )
	{
		if (DataManagerNonDb.WriteFunctionDatabase(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nFunctionLocation )
	{
		if (DataManagerNonDb.WriteFunctionLocation(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nCCMacroSystem )
	{
		if (DataManagerNonDb.WriteCCMacroSystem(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nCCMacroDatabase )
	{
		if (DataManagerNonDb.WriteCCMacroDatabase(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if ( WIZARD_IMPORT_NONE != m_infoImport.m_nCCMacroLocation )
	{
		if (DataManagerNonDb.WriteCCMacroLocation(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if (WIZARD_IMPORT_NONE != m_infoImport.m_nCCReasonSystem)
	{
		if (DataManagerNonDb.WriteCCReasonSystem(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if (WIZARD_IMPORT_NONE != m_infoImport.m_nCCReasonDatabase)
	{
		if (DataManagerNonDb.WriteCCReasonDatabase(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	if (WIZARD_IMPORT_NONE != m_infoImport.m_nCCReasonLocation)
	{
		if (DataManagerNonDb.WriteCCReasonLocation(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	DataManagerNonDb.CloseFiles( arrayNonDbFileTypes );
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();

	m_bImportedData = TRUE;
	DataManager.CloseDatabase();
}

/**********************************************************************/
