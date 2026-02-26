/**********************************************************************/
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "NodeTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageWizardImport.h"
/**********************************************************************/

CPropPageWizardImport::CPropPageWizardImport() : CSSPropertyPage(CPropPageWizardImport::IDD)
{
	//{{AFX_DATA_INIT(CPropPageWizardImport)
	//}}AFX_DATA_INIT
	m_nLocationType = NODE_SYSTEM;
	m_pSQLImporter = NULL;
	m_nDbIdx = 0;
	m_nLocIdx = 0;
	m_bCreateStock = FALSE;
	m_bSpecialServerMode = FALSE;
	m_bClearExisting = FALSE;

	m_nFileTypeID[0] = IDC_CHECK_FILETYPE1;
	m_nFileTypeID[1] = IDC_CHECK_FILETYPE2;
	m_nFileTypeID[2] = IDC_CHECK_FILETYPE3;
	m_nFileTypeID[3] = IDC_CHECK_FILETYPE4;
	m_nFileTypeID[4] = IDC_CHECK_FILETYPE5;
	m_nFileTypeID[5] = IDC_CHECK_FILETYPE6;
	m_nFileTypeID[6] = IDC_CHECK_FILETYPE7;
	m_nFileTypeID[7] = IDC_CHECK_FILETYPE8;
	m_nFileTypeID[8] = IDC_CHECK_FILETYPE9;
	m_nFileTypeID[9] = IDC_CHECK_FILETYPE10;
	m_nFileTypeID[10] = IDC_CHECK_FILETYPE11;
	m_nFileTypeID[11] = IDC_CHECK_FILETYPE12;
	m_nFileTypeID[12] = IDC_CHECK_FILETYPE13;
	m_nFileTypeID[13] = IDC_CHECK_FILETYPE14;
	m_nFileTypeID[14] = IDC_CHECK_FILETYPE15;
	m_nFileTypeID[15] = IDC_CHECK_FILETYPE16;
	m_nFileTypeID[16] = IDC_CHECK_FILETYPE17;

	for (int n = 0; n < DTW_TICKSPERPAGE; n++)
	{
		m_nFileType[n] = -1;
	}
}

/**********************************************************************/

CPropPageWizardImport::~CPropPageWizardImport()
{
}

/**********************************************************************/

void CPropPageWizardImport::SetLocationType( int nType )
{
	m_nLocationType = nType;

	for (int n = 0; n < DTW_TICKSPERPAGE; n++)
	{
		m_nFileType[n] = -1;
	}

	bool bServerRestrict = EcrmanOptions.GetFeaturesServerRestrictionFlag();
	bool bServerPreset = EcrmanOptions.GetFeaturesServerPresetFlag();

	int n = 0;
	switch( m_nLocationType )
	{
	case NODE_SERVER:
		if ( SysInfo.IsEcrLiteSystem() == FALSE )
		{
			if ( bServerRestrict || bServerPreset )
			{
				m_nFileType[n++] = NODE_SERVER;
				
				if (bServerRestrict)
				{
					m_nFileType[n++] = NODE_SERVER_RESTRICT;
				}

				if (bServerPreset)
				{
					m_nFileType[n++] = NODE_SVRPRESET_LOCATION;
				}

				m_nFileType[n++] = NODE_FUNCTION_LOCATION;
				m_nFileType[n++] = NODE_TABLE_AREA;
				m_nFileType[n++] = NODE_TABLE_NAME;
				m_nFileType[n++] = NODE_CCMACRO_LOCATION;
			}
		}
		else
		{
			if (bServerRestrict || bServerPreset)
			{
				m_nFileType[n++] = NODE_CCMACRO_LOCATION;
			}
		}
		break;

	case NODE_LOCATION:
		if ( SysInfo.IsEcrLiteSystem() == FALSE )
		{
			if ( FALSE == ( bServerRestrict || bServerPreset ) )
			{
				if (EcrmanOptions.GetFeaturesServerSettingsType() == SERVER_LOC)
				{
					m_nFileType[n++] = NODE_SERVER;
				}
				
				m_nFileType[n++] = NODE_FUNCTION_LOCATION;
				m_nFileType[n++] = NODE_TABLE_AREA;
				m_nFileType[n++] = NODE_TABLE_NAME;
				m_nFileType[n++] = NODE_CCMACRO_LOCATION;
				m_nFileType[n++] = NODE_CCREASON_LOCATION;
			}
		}
		else
		{
			if (FALSE == (bServerRestrict || bServerPreset))
			{
				m_nFileType[n++] = NODE_CCMACRO_LOCATION;
			}
		}
		break;
		
	case NODE_DATABASE:
		m_nFileType[n++] = NODE_PLU;
		m_nFileType[n++] = NODE_BARCODE;

		if ( SysInfo.IsEcrLiteSystem() == FALSE )
		{
			if (EcrmanOptions.GetFeaturesMixMatchFlag() == TRUE)
			{
				m_nFileType[n++] = NODE_MIXMATCH;
			}

			switch (SysInfo.GetLoyaltyOrSmartPayType())
			{
			case LOYALTY_TYPE_LOY_V4:
			case LOYALTY_TYPE_SMP_V4:
				if (EcrmanOptions.GetFeaturesWalletStampRuleFlag() == TRUE)
				{
					m_nFileType[n++] = NODE_WALLETSTAMPRULE;
				}
			}

			if (EcrmanOptions.GetFeaturesMixMatchFlag() == TRUE)
			{
				if (EcrmanOptions.GetFeaturesTimePlanFlag() == TRUE)
				{
					m_nFileType[n++] = NODE_TIMEPLAN;
				}
			}

			if (FALSE == (bServerRestrict || bServerPreset))
			{
				if (EcrmanOptions.GetFeaturesServerSettingsType() == SERVER_DB)
				{
					m_nFileType[n++] = NODE_SERVER;
				}
			}
		}

		if (EcrmanOptions.GetGlobalDepartmentFlag() == FALSE)
		{
			m_nFileType[n++] = NODE_DEPARTMENT;
		}

		if (EcrmanOptions.GetGlobalDeptGroupEposFlag() == FALSE)
		{
			m_nFileType[n++] = NODE_DEPT_GROUP_EPOS;
		}

		if (SysInfo.GetMaxBasePluLen() > 0)
		{
			if (EcrmanOptions.GetGlobalModifierFlag() == FALSE)
			{
				m_nFileType[n++] = NODE_MODIFIER;
			}
		}

		if (EcrmanOptions.GetGlobalAnalysisCategoryFlag() == FALSE)
		{
			m_nFileType[n++] = NODE_ACAT;
		}

		if (EcrmanOptions.GetGlobalPaidInOutFlag() == FALSE)
		{
			m_nFileType[n++] = NODE_PAIDIN_TEXT;
		}

		if (EcrmanOptions.GetGlobalPaymentFlag() == FALSE)
		{
			m_nFileType[n++] = NODE_PAYMENT;
		}

		if (EcrmanOptions.GetGlobalTaxFlag() == FALSE)
		{
			m_nFileType[n++] = NODE_TAX;
		}

		if (SysInfo.IsEcrLiteSystem() == FALSE)
		{
			m_nFileType[n++] = NODE_FUNCTION_DATABASE;
		}

		m_nFileType[n++] = NODE_CCMACRO_DATABASE;

		if (SysInfo.IsEcrLiteSystem() == FALSE)
		{
			m_nFileType[n++] = NODE_CCREASON_DATABASE;
		}

		if ( (SysInfo.GotLoyaltyLink() == TRUE) || ((SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_UNKNOWN)))
		{
			m_nFileType[n++] = NODE_LOYALTY_SCHEME;
		}

		break;
		
	case NODE_SYSTEM:
	default:
		if ( SysInfo.IsEcrLiteSystem() == FALSE )
		{
			if (FALSE == (bServerRestrict || bServerPreset))
			{
				if (EcrmanOptions.GetFeaturesServerSettingsType() == SERVER_SYS)
				{
					m_nFileType[n++] = NODE_SERVER;
				}
			}
		}

		if (EcrmanOptions.GetFeaturesAllergenFlag() == TRUE)
		{
			m_nFileType[n++] = NODE_ALLERGEN;
		}
			
		if (EcrmanOptions.GetGlobalDepartmentFlag() == TRUE)
		{
			m_nFileType[n++] = NODE_DEPARTMENT;
		}

		if (EcrmanOptions.GetGlobalDeptGroupEposFlag() == TRUE)
		{
			m_nFileType[n++] = NODE_DEPT_GROUP_EPOS;
		}

		if (SysInfo.GetMaxBasePluLen() > 0)
		{
			if (EcrmanOptions.GetGlobalModifierFlag() == TRUE)
			{
				m_nFileType[n++] = NODE_MODIFIER;
			}
		}

		if (EcrmanOptions.GetGlobalAnalysisCategoryFlag() == TRUE)
		{
			m_nFileType[n++] = NODE_ACAT;
		}

		if (EcrmanOptions.GetGlobalPaidInOutFlag() == TRUE)
		{
			m_nFileType[n++] = NODE_PAIDIN_TEXT;
		}

		if (EcrmanOptions.GetGlobalPaymentFlag() == TRUE)
		{
			m_nFileType[n++] = NODE_PAYMENT;
		}

		if (EcrmanOptions.GetGlobalTaxFlag() == TRUE)
		{
			m_nFileType[n++] = NODE_TAX;
		}

		if (SysInfo.IsEcrLiteSystem() == FALSE)
		{
			m_nFileType[n++] = NODE_FUNCTION_SYSTEM;
		}

		if (SysInfo.GotLoyaltyLink())
		{
			m_nFileType[n++] = NODE_LOYALTY_SCHEME;
		}

		m_nFileType[n++] = NODE_CCMACRO_SYSTEM;
		
		if (SysInfo.IsEcrLiteSystem() == FALSE)
		{
			m_nFileType[n++] = NODE_CCREASON_SYSTEM;
		}

		break;
	}
}

/**********************************************************************/

bool CPropPageWizardImport::IsRelevant()
{
	return ( m_nFileType[0] != -1 );
}

/**********************************************************************/

void CPropPageWizardImport::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageWizardImport)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_DESTINATION, m_staticDestination);
	DDX_Control(pDX, IDC_STATIC_TYPES, m_staticTypes);
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	DDX_Control(pDX, IDC_COMBO_SERVER, m_comboServer);
	DDX_Control(pDX, IDC_CHECK_STOCK, m_checkStock);
	DDX_Control(pDX, IDC_CHECK_CLEAR, m_checkClear);

	for (int n = 0; n < DTW_TICKSPERPAGE; n++)
	{
		DDX_Control(pDX, m_nFileTypeID[n], m_checkFileType[n]);
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageWizardImport, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageWizardImport)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_CHECK_FILETYPE1, OnCheckFileType1 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE2, OnCheckFileType2 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE3, OnCheckFileType3 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE4, OnCheckFileType4 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE5, OnCheckFileType5 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE6, OnCheckFileType6 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE7, OnCheckFileType7 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE8, OnCheckFileType8 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE9, OnCheckFileType9 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE10, OnCheckFileType10 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE11, OnCheckFileType11 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE12, OnCheckFileType12 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE13, OnCheckFileType13 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE14, OnCheckFileType14 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE15, OnCheckFileType15 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE16, OnCheckFileType16 )
	ON_BN_CLICKED( IDC_CHECK_FILETYPE17, OnCheckFileType17 )
	ON_BN_CLICKED( IDC_BUTTON_ALL, OnButtonAll )
	ON_BN_CLICKED( IDC_BUTTON_NONE, OnButtonNone )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageWizardImport::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

#ifdef STOCKMAN_SYSTEM

	if ( NODE_DATABASE == m_nLocationType )
	{
		m_checkStock.SetCheck( StockOptions.GetStockAutoCreateFlag() );
		m_checkStock.EnableWindow( FALSE );
	}
	else
	{
		m_checkStock.SetCheck( FALSE );
		m_checkStock.EnableWindow( FALSE );
		m_checkStock.ShowWindow( SW_HIDE );
	}

#else

	m_checkStock.SetCheck( FALSE );
	m_checkStock.EnableWindow( FALSE );
	m_checkStock.ShowWindow( SW_HIDE );

#endif

	if (NODE_SERVER == m_nLocationType)
	{
		m_checkClear.SetWindowText("Clear existing data for imported server file");
	}

	if ( ( NODE_SERVER == m_nLocationType ) && ( SERVER_LOC != EcrmanOptions.GetFeaturesServerSettingsType() ) )
	{
		m_bSpecialServerMode = TRUE;
		ShowAndEnableWindow( &m_comboDatabase, FALSE );

		MoveControl( &m_comboServer, 57, 26, 300, 12 );
		MoveControl( &m_staticDestination, 7, 7, 365, 67 );
		MoveControl( &m_staticTypes, 7, 80, 365, 262 );
		MoveControl( &m_checkClear, 21, 52 );
		
		for ( int n = 0; n < 10; n++ )
		{
			MoveControl( &m_checkFileType[n], 21, 96 + ( 15 * n ) );
		}
		
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(180);
		m_comboServer.SetTabStops( arrayStops );
		
		if ( SERVER_DB == EcrmanOptions.GetFeaturesServerSettingsType() )
		{
			int nSortType = EcrmanOptions.GetReportsLocationSortType();

			CWordArray arrayDbIdx;
			if (nSortType != 0)
			{
				dbDatabase.SortByName(arrayDbIdx);
			}

			int nSize = ( 0 == nSortType ) ? dbDatabase.GetSize() : arrayDbIdx.GetSize();
			
			int nPos = 0;
			for ( int n = 0; n < nSize; n++ )
			{
				int nDbIdx = ( 0 == nSortType ) ? n : arrayDbIdx.GetAt(n);

				CString strDatabase;
				strDatabase.Format( "%s Servers", dbDatabase.GetName( nDbIdx ) );

				CArray<int,int> arrayLocIdx;
				dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );

				if (2 == nSortType)
				{
					dbLocation.SortLocListByName(arrayLocIdx);
				}

				for ( int l = 0; l < arrayLocIdx.GetSize(); l++ )
				{
					int nLocIdx = arrayLocIdx.GetAt ( l );
					
					CCSV csv;
					csv.Add( strDatabase );
					csv.Add( dbLocation.GetName( nLocIdx ) );
					m_comboServer.AddItem( csv.GetLine() );
					m_comboServer.SetItemData( nPos++, nLocIdx );
				}
			}
		}
		else
		{
			for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
			{
				CCSV csv;
				csv.Add( "Global Servers" );
				csv.Add( dbLocation.GetName( nLocIdx ) );
				m_comboServer.AddItem( csv.GetLine() );
				m_comboServer.SetItemData( nLocIdx, nLocIdx );
			}
		}

		m_comboServer.SetCurSel(0);
	}
	else
	{
		m_bSpecialServerMode = FALSE;
		ShowAndEnableWindow( &m_comboServer, FALSE );

		CWordArray arrayTypes;

		switch( m_nLocationType )
		{
		case NODE_DATABASE:
			arrayTypes.Add ( NODE_DATABASE );
			break;

		case NODE_LOCATION:
		case NODE_SERVER:
			arrayTypes.Add ( NODE_LOCATION );
			break;

		case NODE_SYSTEM:
		default:
			arrayTypes.Add ( NODE_SYSTEM );
			m_LocationSelector.SetForceSystemFlag( TRUE );
			break;
		}

		m_LocationSelector.SetForceGroupByDbFlag( NODE_SERVER == m_nLocationType );
		m_LocationSelector.BuildList( arrayTypes );
		m_LocationSelector.FillLocationCombo ( &m_comboDatabase );
	}

	int nCount1 = 0;
	int nCount2 = 0;
	int nCount3 = 0;
	int nCount4 = 0;
	CString strText = "";

	for ( int n = 0; n < DTW_TICKSPERPAGE; n++ )
	{
		switch( m_nFileType[n] )
		{
		case NODE_PLU:
			nCount1 = m_pSQLImporter -> GetBasePluCount();
			nCount2 = m_pSQLImporter -> GetModifierPluCount();

			if (nCount2 == 0)
			{
				strText.Format("Import Plu Records (%d found)", nCount1);
			}
			else
			{
				strText.Format("Import Plu Records (%d + %d modifiers)", nCount1, nCount2);
			}

			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 + nCount2 > 0 );
			break;

		case NODE_BARCODE:
			nCount1 = m_pSQLImporter -> GetBarcodeCount();
			strText.Format ( "Import Barcode Records (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_MIXMATCH:
			{
				nCount1 = m_pSQLImporter -> GetMixMatchCount();
				nCount2 = m_pSQLImporter -> GetOfferGroupCount();
				nCount3 = m_pSQLImporter -> GetOfferEntryCount();
				nCount4 = m_pSQLImporter -> GetPromotionCount();

				CString strOffer = ( nCount1 == 1 ) ? "offer" : "offers";
				CString strGroup = ( nCount2 == 1 ) ? "group" : "groups";
				CString strEntry = ( nCount3 == 1 ) ? "entry" : "entries";
				CString strPromo = ( nCount4 == 1 ) ? "promo" : "promos";

				strText.Format( "Import Mix Match Records (%d %s, %d %s, %d %s, %d %s)",
					nCount1,
					(const char*) strOffer,
					nCount2,
					(const char*) strGroup,
					nCount3,
					(const char*) strEntry,
					nCount4,
					(const char*) strPromo
					);

				m_checkFileType[n].SetWindowText( strText );
				m_checkFileType[n].EnableWindow( nCount1 + nCount2 + nCount3 + nCount4 > 0 );
			}
			break;

		case NODE_WALLETSTAMPRULE:
			{
				nCount1 = m_pSQLImporter->GetWalletStampRuleCount();
				nCount2 = m_pSQLImporter->GetOfferGroupCount();
				nCount3 = m_pSQLImporter->GetOfferEntryCount();
		
				CString strOffer = (nCount1 == 1) ? "offer" : "offers";
				CString strGroup = (nCount2 == 1) ? "group" : "groups";
				CString strEntry = (nCount3 == 1) ? "entry" : "entries";

				CString strType = "";
				switch (SysInfo.GetLoyaltyOrSmartPayType())
				{
				case LOYALTY_TYPE_SMP_V4:
					strType = "Purchase Control";
					break;

				case LOYALTY_TYPE_LOY_V4:
				default:
					strType = "Stamp Offer";
					break;
				}
				strText.Format("Import %s Records (%d %s, %d %s, %d %s)",
					(const char*)strType,
					nCount1,
					(const char*)strOffer,
					nCount2,
					(const char*)strGroup,
					nCount3,
					(const char*)strEntry
				);

				m_checkFileType[n].SetWindowText(strText);
				m_checkFileType[n].EnableWindow(nCount1 + nCount2 + nCount3 > 0);
			}
			break;

		case NODE_TIMEPLAN:
			{
				nCount1 = m_pSQLImporter -> GetTimePlanCount();
				nCount2 = m_pSQLImporter -> GetTimePlanEntryCount();
				
				CString strPlan = ( nCount1 == 1 ) ? "plan" : "plans";
				CString strEntry = ( nCount2 == 1 ) ? "entry" : "entries";
				
				strText.Format( "Import Time Plan Records (%d %s, %d %s)",
					nCount1,
					(const char*) strPlan,
					nCount2,
					(const char*) strEntry );

				m_checkFileType[n].SetWindowText( strText );
				m_checkFileType[n].EnableWindow( nCount1 + nCount2 > 0 );
			}
			break;

		case NODE_PAYMENT:
			nCount1 = m_pSQLImporter -> GetPayMethCount();
			strText.Format ( "Import Payment Records (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_MODIFIER:
			nCount1 = m_pSQLImporter -> GetModifierCount();
			strText.Format ( "Import Modifier Records (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_ACAT:
			nCount1 = m_pSQLImporter -> GetCategoryCount();
			strText.Format ( "Import Analysis Category Records (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_PAIDIN_TEXT:
			nCount1 = m_pSQLImporter -> GetPaidInOutTextCount();
			strText.Format ( "Import Paid In / Out Texts (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_SERVER:
			nCount1 = m_pSQLImporter->GetServerCount();
			nCount2 = m_pSQLImporter->GetServerLogonCount();

			if (EcrmanOptions.GetFeaturesServerLogonFlag() == FALSE)
			{
				strText.Format("Import Server Records (%d found)", nCount1);
			}
			else
			{
				strText.Format("Import Server Records (%d server%s, %d logon%s)", 
					nCount1,
					(1 == nCount1) ? "" : "s",
					nCount2,
					(1 == nCount2) ? "" : "s" );
			}

			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 + nCount2 > 0 );
			break;

		case NODE_SERVER_RESTRICT:
			nCount1 = m_pSQLImporter -> GetServerRestrictCount();
			strText.Format ( "Import Server Restriction Records (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_SVRPRESET_LOCATION:
			nCount1 = m_pSQLImporter -> GetServerPresetCount();
			strText.Format ( "Import Server Preset Records (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_DEPARTMENT:
			nCount1 = m_pSQLImporter -> GetDeptCount();
			strText.Format ( "Import Department Records (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_DEPT_GROUP_EPOS:
			nCount1 = m_pSQLImporter -> GetGroupCount();
			strText.Format ( "Import Group Records (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_ALLERGEN:
			nCount1 = m_pSQLImporter -> GetAllergenCount();
			strText.Format ( "Import Allergen Records (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;
	
		case NODE_TAX:
			nCount1 = m_pSQLImporter -> GetTaxRateCount();
			strText.Format ( "Import %s Rates (%d found)", EcrmanOptions.GetTaxBandString(), nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_LOYALTY_SCHEME:
			nCount1 = m_pSQLImporter -> GetLoyaltySchemeCount();
			strText.Format ( "Import Loyalty Schemes (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_TABLE_AREA:
			nCount1 = m_pSQLImporter -> GetTableAreaCount();
			strText.Format ( "Import Table Areas (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_TABLE_NAME:
			nCount1 = m_pSQLImporter -> GetTableNameCount();
			strText.Format ( "Import Table Names (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_FUNCTION_SYSTEM:
		case NODE_FUNCTION_DATABASE:
		case NODE_FUNCTION_LOCATION:
			nCount1 = m_pSQLImporter -> GetFunctionCount();
			strText.Format ( "Import Function Authority Levels (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_CCMACRO_SYSTEM:
		case NODE_CCMACRO_DATABASE:
		case NODE_CCMACRO_LOCATION:
			nCount1 = m_pSQLImporter -> GetCCMacroCount();
			strText.Format ( "Import Macro Definitions (%d found)", nCount1 );
			m_checkFileType[n].SetWindowText( strText );
			m_checkFileType[n].EnableWindow( nCount1 > 0 );
			break;

		case NODE_CCREASON_SYSTEM:
		case NODE_CCREASON_DATABASE:
		case NODE_CCREASON_LOCATION:
			nCount1 = m_pSQLImporter->GetCCReasonCount();
			strText.Format("Import Reason Texts (%d found)", nCount1);
			m_checkFileType[n].SetWindowText(strText);
			m_checkFileType[n].EnableWindow(nCount1 > 0);
			break;
	
		case -1:
		default:
			m_checkFileType[n].ShowWindow( SW_HIDE );
			m_checkFileType[n].EnableWindow( FALSE );
			break;
		}
	}

	SetRecordControls();
	return TRUE;  
}

/**********************************************************************/
void CPropPageWizardImport::OnCheckFileType1()	{ ToggleFileType(0); }
void CPropPageWizardImport::OnCheckFileType2()	{ ToggleFileType(1); }
void CPropPageWizardImport::OnCheckFileType3()	{ ToggleFileType(2); }
void CPropPageWizardImport::OnCheckFileType4()	{ ToggleFileType(3); }
void CPropPageWizardImport::OnCheckFileType5()	{ ToggleFileType(4); }
void CPropPageWizardImport::OnCheckFileType6()	{ ToggleFileType(5); }
void CPropPageWizardImport::OnCheckFileType7()	{ ToggleFileType(6); }
void CPropPageWizardImport::OnCheckFileType8()	{ ToggleFileType(7); }
void CPropPageWizardImport::OnCheckFileType9()	{ ToggleFileType(8); }
void CPropPageWizardImport::OnCheckFileType10()	{ ToggleFileType(9); }
void CPropPageWizardImport::OnCheckFileType11()	{ ToggleFileType(10); }
void CPropPageWizardImport::OnCheckFileType12()	{ ToggleFileType(11); }
void CPropPageWizardImport::OnCheckFileType13()	{ ToggleFileType(12); }
void CPropPageWizardImport::OnCheckFileType14()	{ ToggleFileType(13); }
void CPropPageWizardImport::OnCheckFileType15() { ToggleFileType(14); }
void CPropPageWizardImport::OnCheckFileType16() { ToggleFileType(15); }
void CPropPageWizardImport::OnCheckFileType17() { ToggleFileType(16); }
/**********************************************************************/

void CPropPageWizardImport::ToggleFileType( int n )
{
	if (NODE_PLU == m_nFileType[n])
	{
		m_checkStock.EnableWindow(m_checkFileType[n].GetCheck() != 0);
	}
}

/**********************************************************************/

void CPropPageWizardImport::OnButtonAll()
{
	for ( int n = 0; n < DTW_TICKSPERPAGE; n++ )
	{
		if ( m_checkFileType[n].IsWindowEnabled() == TRUE )
		{
			m_checkFileType[n].SetCheck( TRUE );
			ToggleFileType(n);
		}
	}
}

/**********************************************************************/

void CPropPageWizardImport::OnButtonNone()
{
	for ( int n = 0; n < DTW_TICKSPERPAGE; n++ )
	{
		if ( m_checkFileType[n].IsWindowEnabled() == TRUE )
		{
			m_checkFileType[n].SetCheck( FALSE );
			ToggleFileType(n);
		}
	}
}

/**********************************************************************/

BOOL CPropPageWizardImport::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageWizardImport::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageWizardImport::GetRecordData()
{
}

/**********************************************************************/

void CPropPageWizardImport::SaveRecord()
{
	if ( FALSE == m_bSpecialServerMode )
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		switch( m_nLocationType )
		{
		case NODE_DATABASE:
			m_nDbIdx = LocSelEntity.GetEntityIdx();
			m_bCreateStock = ( m_checkStock.GetCheck() != 0 ); 
			break;

		case NODE_LOCATION:
		case NODE_SERVER:
			m_nLocIdx = LocSelEntity.GetEntityIdx();
			break;
		}
	}
	else
	{
		int nSel = m_comboServer.GetCurSel();
		m_nLocIdx = m_comboServer.GetItemData( nSel );
	}

	m_bClearExisting = ( m_checkClear.GetCheck() != 0 );

	m_arrayFileTypes.RemoveAll();

	for (int n = 0; n < DTW_TICKSPERPAGE; n++)
	{
		if (m_checkFileType[n].GetCheck() != 0)
		{
			m_arrayFileTypes.Add(m_nFileType[n]);
		}
	}
}

/**********************************************************************/

void CPropPageWizardImport::GetFileTypeTicks( CWizardImportInfo& info )
{
	for ( int n = 0; n < m_arrayFileTypes.GetSize(); n++ )
	{
		int nFileType = m_arrayFileTypes.GetAt(n);
		int nAction = ( m_bClearExisting ? WIZARD_IMPORT_CLEAR : WIZARD_IMPORT_MERGE );

		switch( nFileType )
		{
		case NODE_BARCODE:				info.m_nBarcode = nAction;			break;
		case NODE_DEPARTMENT:			info.m_nDept = nAction;				break;
		case NODE_ACAT:					info.m_nCategory = nAction;			break;
		case NODE_PAIDIN_TEXT:			info.m_nPaidInOutText = nAction;	break;
		case NODE_DEPT_GROUP_EPOS:		info.m_nGroup = nAction;			break;
		case NODE_ALLERGEN:				info.m_nAllergen = nAction;			break;
		case NODE_MIXMATCH:				info.m_nMixMatch2 = nAction;		break;
		case NODE_WALLETSTAMPRULE:		info.m_nWalletStampRule = nAction;	break;
		case NODE_TIMEPLAN:				info.m_nTimePlan = nAction;			break;
		case NODE_MODIFIER:				info.m_nModifier = nAction;			break;
		case NODE_PAYMENT:				info.m_nPayment = nAction;			break;
		
		case NODE_SERVER:	
			info.m_nServer = nAction;	

			if (EcrmanOptions.GetFeaturesServerLogonFlag() == TRUE)
			{
				info.m_nServerLogon = nAction;
			}

			break;
		
		case NODE_SERVER_RESTRICT:		info.m_nServerRestrict = nAction;	break;
		case NODE_SVRPRESET_LOCATION:	info.m_nServerPreset = nAction;		break;
		case NODE_PLU:					info.m_nPlu = nAction;				break;
		case NODE_TAX:					info.m_nTaxRate = nAction;			break;
		case NODE_LOYALTY_SCHEME:		info.m_nLoyaltyScheme = nAction;	break;
		case NODE_TABLE_AREA:			info.m_nTableArea = nAction;		break;
		case NODE_TABLE_NAME:			info.m_nTableName = nAction;		break;
		case NODE_FUNCTION_SYSTEM:		info.m_nFunctionSystem = nAction;	break;
		case NODE_FUNCTION_DATABASE:	info.m_nFunctionDatabase = nAction;	break;
		case NODE_FUNCTION_LOCATION:	info.m_nFunctionLocation = nAction;	break;
		case NODE_CCMACRO_SYSTEM:		info.m_nCCMacroSystem = nAction;	break;
		case NODE_CCMACRO_DATABASE:		info.m_nCCMacroDatabase = nAction;	break;
		case NODE_CCMACRO_LOCATION:		info.m_nCCMacroLocation = nAction;	break;
		case NODE_CCREASON_SYSTEM:		info.m_nCCReasonSystem = nAction;	break;
		case NODE_CCREASON_DATABASE:	info.m_nCCReasonDatabase = nAction;	break;
		case NODE_CCREASON_LOCATION:	info.m_nCCReasonLocation = nAction;	break;
		}
	}

#ifdef STOCKMAN_SYSTEM
	if (WIZARD_IMPORT_NONE != info.m_nPlu)
	{
		if (TRUE == m_bCreateStock)
		{
			if (NODE_DATABASE == m_nLocationType)
			{
				info.m_nStock = WIZARD_IMPORT_MERGE;
			}
		}
	}
#endif
}

/**********************************************************************/

void CPropPageWizardImport::SetRecordControls()
{
}

/**********************************************************************/
