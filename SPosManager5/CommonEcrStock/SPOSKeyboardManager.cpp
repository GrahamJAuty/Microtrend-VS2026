/**********************************************************************/
#include "DataAccessHelpersPlu.h"
#include "KeyboardHelpers.h"
#include "ListLabelsDlg.h"
#include "SPOSKeyboardDefines.h"
#include "SPOSKeyboardDlg.h"
#include "SPOSKeyboardSQLExporter.h"
#include "SPOSKeyboardSQLImporter.h"
#include "SPOSVersions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SPOSKeyboardManager.h"
/**********************************************************************/

CSPOSKeyboardManagerParams::CSPOSKeyboardManagerParams()
{
	m_nDbIdx = 0;
	m_nLocIdx = 0;
	m_bKbSet = FALSE;
	m_nPriceSetCount = 0;
	m_nSPOSVersion = SPOS_V4;
	m_strKeyboardName = "";
	m_strDataPath = "";
	m_strImageFolder = "";
	m_strSysFilesFolder = "";
	m_strListNameFolder = "";
	m_strSQLFileIn = "";
	m_strSQLFileOut = "";
	m_nPasswordIdx = 0;
}

/**********************************************************************/

CSPOSKeyboardManager::CSPOSKeyboardManager()
{
}

/**********************************************************************/

void CSPOSKeyboardManager::Reset()
{
	m_ColourTable.Reset();
	m_FontTable.Reset();
	m_ImageLibrary.Reset();
	m_KeymapTable.Reset();
	m_ListManager.Reset();
	m_LayoutOptions.Reset();
	m_MessageLogger.Reset();
	m_MultipleKeySelectInfo.Reset();
	m_PluDatabase.Reset();
	m_SPOSVersion.Reset();

	m_ListTable.Reset();
	m_MacroTable.Reset();
	m_FunctionTable.Reset();
	m_PaymentTable.Reset();
	m_ControlTable.Reset();
	m_ModifierTable.Reset();
	m_SvrPresetTable.Reset();
	m_EmptyTable.Reset();

	m_nDbIdx = 0;
	m_nLocIdx = 0;
	m_bKbSet = FALSE;
	m_nPriceSetCount = 0;

	m_bCanDoChangesTask = FALSE;
	m_bCanDeleteBarcode = FALSE;
	m_bCanDeleteStock = FALSE;
	m_bMergedLegacyMacros = FALSE;
}

/**********************************************************************/

void CSPOSKeyboardManager::ProcessKeyboard( CWnd* pParentWnd, CSPOSKeyboardManagerParams& Params )
{
	Reset();

	m_nDbIdx = Params.m_nDbIdx;
	m_nLocIdx = Params.m_nLocIdx;
	m_bKbSet = Params.m_bKbSet;
	m_nPriceSetCount = Params.m_nPriceSetCount;

	CString strSlash = "\\";

	m_MessageLogger.Initialise( Super.FileWriteLogNew(), SysInfo.IsStockSystem(), Params.m_nPasswordIdx );

	m_LayoutOptions.SetDataPath( Params.m_strDataPath );
	m_LayoutOptions.SetKeyboardName( Params.m_strKeyboardName );
	m_LayoutOptions.SetSQLInFilename( Params.m_strSQLFileIn );
	m_LayoutOptions.SetSQLOutFilename( Params.m_strDataPath + strSlash + Params.m_strSQLFileOut );
	m_SPOSVersion.SetSPOSVersion( Params.m_nSPOSVersion );
	m_LayoutOptions.SetFilename ( Params.m_strDataPath + "\\options.dat" );
	m_LayoutOptions.SetImageFolder( Params.m_strImageFolder );
	m_LayoutOptions.SetMaxBasePluLen ( SysInfo.GetMaxBasePluLen() );
	m_ColourTable.SetFilename( Params.m_strDataPath + "\\colours.txt" );
	m_LayoutOptions.SetReportFolder( Sysset.GetTempPath() );
	m_LayoutOptions.SetReportSettingsFolder( Params.m_strSysFilesFolder );
	m_LayoutOptions.SetPluTextOrder( EcrmanOptions.GetFeaturesKeyboardTextOrder() );

	m_FunctionTable.SetTextType( Params.m_strDataPath, KEY_TYPE_FUNCTION );
	m_ListTable.SetTextType( Params.m_strDataPath, KEY_TYPE_LIST );
	m_MacroTable.SetTextType( Params.m_strDataPath, KEY_TYPE_MACRO );
	m_PaymentTable.SetTextType( Params.m_strDataPath, KEY_TYPE_PAYMENT );
	m_ControlTable.SetTextType( Params.m_strDataPath, KEY_TYPE_CONTROL );
	m_ModifierTable.SetTextType( Params.m_strDataPath, KEY_TYPE_MODIFIER );
	m_SvrPresetTable.SetTextType( Params.m_strDataPath, KEY_TYPE_SVRPRESET );
	m_EmptyTable.SetTextType( Params.m_strDataPath, KEY_TYPE_EMPTY );

	m_ColourTable.Read();
	m_LayoutOptions.Read();
	m_FunctionTable.Read();
	m_ListTable.Read();
	m_MacroTable.Read();
	m_PaymentTable.Read();
	m_ControlTable.Read();
	m_ModifierTable.Read();
	m_SvrPresetTable.Read();
	m_EmptyTable.Read();

	CString strKeyboardMacroFilename = Params.m_strDataPath + "\\File0173.dat";
	DataManagerNonDb.SetKeyboardMacroFilename( strKeyboardMacroFilename );
	CreatePluItemList();

	m_ListManager.SetListNameExportFolder ( Params.m_strListNameFolder );

	CreateSubdirectory( Params.m_strDataPath );

	CSPOSKeyboardSQLImporter SQLImporter;

	CCSV csv( m_LayoutOptions.GetSQLInFilename() );
	for ( int n = 0; n < csv.GetSize(); n++ )
	{
		CString strFilename = "";
		strFilename += Params.m_strDataPath;
		strFilename += strSlash;
		strFilename += csv.GetString(n);
	
		SQLImporter.ImportFile( strFilename, TRUE, TRUE, TRUE );

		if ( SQLImporter.GotMacros() == TRUE )
		{
			CDataManagerInfo infoDM;
			if ( DataManagerNonDb.OpenCCMacroKeyboard( DB_READWRITE, infoDM ) == TRUE )
			{
				SQLImporter.MergeMacroBufferKeyboard(TRUE);

				if ( DataManagerNonDb.WriteCCMacroKeyboard( infoDM ) == TRUE )
				{
					SPOSKeyboardManager.SetMergedLegacyMacrosFlag();
				}

				DataManagerNonDb.CloseCCMacroKeyboard( infoDM );
			}
		}
		else
		{
			SPOSKeyboardManager.SetMergedLegacyMacrosFlag();
		}

		break;
	}

	m_ListManager.WriteListNames();
	CreateActiveMacroList();

	CSPOSKeyboardDlg dlgKeyboard( pParentWnd );
	dlgKeyboard.DoModal();
	
	HandlePluChanges( pParentWnd );

	DataManager.CloseDatabase();
	Reset();
}

/**********************************************************************/

void CSPOSKeyboardManager::HandlePluChanges( CWnd* pParentWnd )
{
	CDataAccessHelpersPlu DataAccessPlu;

	if ( TRUE == m_bCanDoChangesTask )
		DataAccessPlu.RunPluChangesTask( m_nDbIdx );

#ifdef STOCKMAN_SYSTEM
	if ( TRUE == m_bCanDeleteStock )
		DataAccessPlu.AutoDeleteStock();
#endif

	if ( TRUE == m_bCanDeleteBarcode )
		DataAccessPlu.AutoDeleteBarcode();

	//CHECK FOR CHANGED LABELS
	if ( ( EcrmanOptions.GetLabelsShelfAutoBatchFlag() && EcrmanOptions.GetLabelsShelfPromptBatchFlag() ) == FALSE )
		return;

	CDataManagerInfo info2;
	if ( DataManager.OpenShelfLabels( DB_READWRITE, info2 ) == FALSE )
	{
		Prompter.ShareError( info2 );
		return;
	}

	DataManager.ShelfLabels.CreateShelfChangesList();
	
	if ( DataManager.ShelfLabels.GetSize() == 0 )
		return;

	CString strMsg = "One or more Plu records has changed since you last printed\n";      
	strMsg += "shelf edge labels for this database.\n\n";

	if ( EcrmanOptions.GetPluPriceSetsFlag() )
	{
		strMsg += "You should also check any price sets that are linked to this\n";
		strMsg += "database to see if updated labels need to be printed.\n\n";
		strMsg += "Do you wish to print the updated shelf edge labels for the\n";
		strMsg += "master Plu database."; 
	}
	else
		strMsg += "Do you wish to print updated labels now.";

	if ( Prompter.YesNo( strMsg, TRUE ) != IDYES )
		return;

	CListLabelsDlg dlgLabels ( FALSE, FALSE, TRUE, DataManager.ShelfLabels, "", pParentWnd );

	int nResult = dlgLabels.DoModal();
	//m_pParentWnd -> GetParent() -> RedrawWindow();
	
	if ( nResult == IDOK )
	{
		DataManager.ShelfLabels.UpdateShelfChangesList();
		
		CDataManagerInfo info2;
		if ( DataManager.WriteShelfLabels( info2 ) == FALSE )
			Prompter.WriteError( info2 );
	}
}

/**********************************************************************/

void CSPOSKeyboardManager::PrepareDownload( const char* szSourcePath, const char* szKbdFilename, const char* szTempFilename, int nSPOSVersion, bool bClearMacros )
{
	Reset();

	CString strPath = szSourcePath;
	strPath += "\\";

	CString strSourceFile = strPath + szKbdFilename;
	CString strDestFile = strPath + szTempFilename;
	CString strColourFile = strPath + "\\colours.txt";

	CString strKeyboardMacroFilename = strPath + "\\File0173.dat";
	DataManagerNonDb.SetKeyboardMacroFilename( strKeyboardMacroFilename );

	m_ColourTable.SetFilename( strColourFile );
	m_ColourTable.Read();

	CSPOSKeyboardSQLImporter SQLImporter;
	m_SPOSVersion.SetSPOSVersion( nSPOSVersion );
	SQLImporter.ImportFile( strSourceFile, TRUE, TRUE, TRUE );

	CreateActiveMacroList();
	SQLImporter.MergeMacroBufferActive();

	CSPOSKeyboardSQLExporter SQLExporter( nSPOSVersion );
	SQLExporter.ExportFileForDownload( strDestFile, bClearMacros );

	Reset();
}

/**********************************************************************/

void CSPOSKeyboardManager::CreatePluItemList()
{
	CString strPluFilename = "";
	CKeyboardHelpers::CreatePluFile ( m_nDbIdx, m_nLocIdx, strPluFilename, m_bKbSet );
	m_PluDatabase.Read( strPluFilename );
}

/**********************************************************************/

void CSPOSKeyboardManager::CreateActiveMacroList()
{
	DataManagerNonDb.CCMacroActive.Reset();
	DataManagerNonDb.CCMacroActive.LoadParentMacrosKeyboard();
	
	if ( FALSE == m_bKbSet )
	{
		DataManagerNonDb.CCMacroActive.LoadParentMacrosLocation(m_nLocIdx);
	}

	DataManagerNonDb.CCMacroActive.LoadParentMacrosDatabase( m_nDbIdx );
	DataManagerNonDb.CCMacroActive.LoadParentMacrosSystem();
}

/**********************************************************************/

void CSPOSKeyboardManager::MergeParentMacros()
{
	if ( FALSE == GetKbSetFlag() )
	{
		DataManagerNonDb.CCMacroKeyboard.LoadParentMacrosLocation( SPOSKeyboardManager.GetLocIdx() );
	}
	DataManagerNonDb.CCMacroKeyboard.LoadParentMacrosDatabase(SPOSKeyboardManager.GetDbIdx());
	DataManagerNonDb.CCMacroKeyboard.LoadParentMacrosSystem();	
}

/**********************************************************************/
