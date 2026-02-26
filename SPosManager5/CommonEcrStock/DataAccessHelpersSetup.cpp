/**********************************************************************/
#include "..\CommonEcrStockTray\AutoBackupOptions.h"
/**********************************************************************/
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "AutoBackupProcessor.h"
#include "AutoExpireHistory.h"
#include "CustomExportManager.h"
#include "EmailOptions.h"
#include "LocationCSVArray.h"
#include "LockManager.h"
#include "NetworkTerminalMap.h"
#include "NodeTypes.h"
#include "PCOptions.h"
#include "PosTrayTokenFiles.h"
#include "PropertySheetOptions.h"
#include "PropertySheetSetup.h"
#include "KeyboardHelpers.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DataAccessHelpersSetup.h"
/**********************************************************************/

CDataAccessHelpersSetup::CDataAccessHelpersSetup()
{
}

/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/

void CDataAccessHelpersSetup::EditOptions()
{
	CMainFrameEcrman* pFrame = (CMainFrameEcrman*)m_pParentWnd;

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "change these settings";
	infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Setup options";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = pFrame;

	CLockManager LockManager(infoLock);

	if (LockManager.GetLock() == TRUE)
	{
		CPropertySheetOptions propSheet(TRUE, m_pParentWnd);

		CEcrmanOptions oldEcrmanOptions;
		oldEcrmanOptions.CopyFrom(EcrmanOptions);

		CEmailOptions oldEmailOptions;
		oldEmailOptions.CopyFrom(EmailOptions);

		CPCOptionsClient oldPCOptionsClient;
		oldPCOptionsClient.CopyFrom(PCOptionsClient);

		CPCOptionsHost oldPCOptionsHost;
		oldPCOptionsHost.CopyFrom(PCOptionsHost);

		SysInfo.RememberSettings();
		AutoBackupOptions.RememberSettings();

		int nPropSheetResult = 0;
		{
			CDataManagerInfo info;
			DataManagerNonDb.OpenEmailAddress(DB_READONLY, info);
			nPropSheetResult = propSheet.DoModal();
			DataManagerNonDb.CloseEmailAddress(info);
		}

		if (nPropSheetResult != IDOK)
		{
			EcrmanOptions.CopyFrom(oldEcrmanOptions);
			EmailOptions.CopyFrom(oldEmailOptions);
			PCOptionsClient.CopyFrom(oldPCOptionsClient);
			PCOptionsHost.CopyFrom(oldPCOptionsHost);
			SysInfo.RestoreSettings();
			AutoBackupOptions.RestoreSettings();
			pFrame->Invalidate();
		}
		else
		{
			pFrame->Invalidate();
			if (SysInfo.GotLoyaltyLink())
			{
				if (oldEcrmanOptions.GetPluExportPurchaseHistoryFlag() == FALSE)
					if (EcrmanOptions.GetPluExportPurchaseHistoryFlag() == TRUE)
						CCustomExportManagerSCard CustomExport;
			}

			if (oldEcrmanOptions.GetGlobalDepartmentFlag() == TRUE)
				if (EcrmanOptions.GetGlobalDepartmentFlag() == FALSE)
					::CopyGlobalData(NODE_DEPARTMENT);

			if (oldEcrmanOptions.GetGlobalDeptGroupReportFlag() == TRUE)
				if (EcrmanOptions.GetGlobalDeptGroupReportFlag() == FALSE)
					::CopyGlobalData(NODE_DEPT_GROUP_REPORT);

			if (oldEcrmanOptions.GetGlobalDeptGroupConsolFlag() == TRUE)
				if (EcrmanOptions.GetGlobalDeptGroupConsolFlag() == FALSE)
					::CopyGlobalData(NODE_DEPT_GROUP_CONSOL);

			if (oldEcrmanOptions.GetGlobalDeptGroupEposFlag() == TRUE)
				if (EcrmanOptions.GetGlobalDeptGroupEposFlag() == FALSE)
					::CopyGlobalData(NODE_DEPT_GROUP_EPOS);

			if (oldEcrmanOptions.GetGlobalTaxFlag() == TRUE)
				if (EcrmanOptions.GetGlobalTaxFlag() == FALSE)
					::CopyGlobalData(NODE_TAX);

			if (oldEcrmanOptions.GetGlobalModifierFlag() == TRUE)
				if (EcrmanOptions.GetGlobalModifierFlag() == FALSE)
					::CopyGlobalData(NODE_MODIFIER);

			if (oldEcrmanOptions.GetGlobalAnalysisCategoryFlag() == TRUE)
				if (EcrmanOptions.GetGlobalAnalysisCategoryFlag() == FALSE)
					::CopyGlobalData(NODE_ACAT);

			if (oldEcrmanOptions.GetGlobalPaymentFlag() == TRUE)
				if (EcrmanOptions.GetGlobalPaymentFlag() == FALSE)
					::CopyGlobalData(NODE_PAYMENT);

			switch (oldEcrmanOptions.GetFeaturesServerSettingsType())
			{
			case SERVER_SYS:
				switch (EcrmanOptions.GetFeaturesServerSettingsType())
				{
				case SERVER_DB:		::CopyGlobalData(NODE_SERVER);	break;
				case SERVER_LOC:	::CopyServerData(NODE_SYSTEM);	break;
				}
				break;

			case SERVER_DB:
				switch (EcrmanOptions.GetFeaturesServerSettingsType())
				{
				case SERVER_LOC:	::CopyServerData(NODE_DATABASE);	break;
				}
				break;
			}

			EcrmanOptions.Write();
			EmailOptions.Write();
			PCOptionsClient.Write();
			PCOptionsHost.Write();

			if (AutoBackupOptions.HasScheduleChanged())
			{
				AutoBackupProcessor.ScheduleNextBackup(FALSE);

				if (pFrame->GetBackupResultDlg() != NULL)
				{
					pFrame->GetBackupResultDlg()->UpdateDisplay();
				}
			}

			AutoBackupOptions.Write();
			SysInfo.Write();
			pFrame->SetPasswordMenuAccess();

			AutoExpireHistory.Reset();
			AutoExpireHistory.ExpireReportsNow();

			for (int nDbIdx = 0; nDbIdx <= dbDatabase.GetSize(); nDbIdx++)
			{
				CKeyboardHelpers::RemovePluFile(nDbIdx);
			}

			CPosTrayTokenFiles::CreateSystemUpdateFile();
		}
	}
}

/**********************************************************************/
#else
/**********************************************************************/

void CDataAccessHelpersSetup::EditOptions()
{
	CMainFrameStockman* pFrame = (CMainFrameStockman*) m_pParentWnd;

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "change these settings";
	infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Setup options";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = pFrame;

	CLockManager LockManager( infoLock );

	if ( LockManager.GetLock() == TRUE )
	{
		CPropertySheetOptions propSheet ( TRUE, pFrame );

		CEcrmanOptions oldEcrmanOptions;
		oldEcrmanOptions.CopyFrom( EcrmanOptions );

		CEmailOptions oldEmailOptions;
		oldEmailOptions.CopyFrom( EmailOptions );

		CPCOptionsHost oldPCOptionsHost;
		oldPCOptionsHost.CopyFrom( PCOptionsHost );

		CPCOptionsClient oldPCOptionsClient;
		oldPCOptionsClient.CopyFrom(PCOptionsClient);

		CStockOptions oldStockOptions;
		oldStockOptions.CopyFrom( StockOptions );

		SysInfo.RememberSettings();
		AutoBackupOptions.RememberSettings();

		int nPropSheetResult = 0;
		{
			CDataManagerInfo info;
			DataManagerNonDb.OpenEmailAddress( DB_READONLY, info );
			nPropSheetResult = propSheet.DoModal();
			DataManagerNonDb.CloseEmailAddress( info );
		}

		if (nPropSheetResult != IDOK)
		{
			EcrmanOptions.CopyFrom(oldEcrmanOptions);
			EmailOptions.CopyFrom(oldEmailOptions);
			PCOptionsHost.CopyFrom(oldPCOptionsHost);
			PCOptionsClient.CopyFrom(oldPCOptionsClient);
			StockOptions.CopyFrom(oldStockOptions);
			SysInfo.RestoreSettings();
			AutoBackupOptions.RestoreSettings();
			pFrame->Invalidate();
		}
		else
		{	
			pFrame -> Invalidate();
			if ( SysInfo.GotLoyaltyLink() )
			{
				if ( oldEcrmanOptions.GetPluExportPurchaseHistoryFlag() == FALSE ) 
					if ( EcrmanOptions.GetPluExportPurchaseHistoryFlag() == TRUE )
						CCustomExportManagerSCard CustomExport;
			}

			if ( oldEcrmanOptions.GetGlobalDepartmentFlag() == TRUE )
				if ( EcrmanOptions.GetGlobalDepartmentFlag() == FALSE )
					::CopyGlobalData( NODE_DEPARTMENT );

			if ( oldEcrmanOptions.GetGlobalDeptGroupReportFlag() == TRUE )
				if ( EcrmanOptions.GetGlobalDeptGroupReportFlag() == FALSE )
					::CopyGlobalData( NODE_DEPT_GROUP_REPORT );

			if ( oldEcrmanOptions.GetGlobalDeptGroupConsolFlag() == TRUE )
				if ( EcrmanOptions.GetGlobalDeptGroupConsolFlag() == FALSE )
					::CopyGlobalData( NODE_DEPT_GROUP_CONSOL );

			if ( oldEcrmanOptions.GetGlobalDeptGroupEposFlag() == TRUE )
				if ( EcrmanOptions.GetGlobalDeptGroupEposFlag() == FALSE )
					::CopyGlobalData( NODE_DEPT_GROUP_EPOS );

			if ( oldEcrmanOptions.GetGlobalTaxFlag() == TRUE )
				if ( EcrmanOptions.GetGlobalTaxFlag() == FALSE )
					::CopyGlobalData( NODE_TAX );

			if ( oldEcrmanOptions.GetGlobalModifierFlag() == TRUE )
				if ( EcrmanOptions.GetGlobalModifierFlag() == FALSE )
					::CopyGlobalData( NODE_MODIFIER );

			if ( oldEcrmanOptions.GetGlobalAnalysisCategoryFlag() == TRUE )
				if ( EcrmanOptions.GetGlobalAnalysisCategoryFlag() == FALSE )
					::CopyGlobalData( NODE_ACAT );

			if ( oldEcrmanOptions.GetGlobalPaymentFlag() == TRUE )
				if ( EcrmanOptions.GetGlobalPaymentFlag() == FALSE )
					::CopyGlobalData( NODE_PAYMENT );

			switch( oldEcrmanOptions.GetFeaturesServerSettingsType() )
			{
			case SERVER_SYS:
				switch( EcrmanOptions.GetFeaturesServerSettingsType() )
				{
				case SERVER_DB:		::CopyGlobalData( NODE_SERVER );	break;
				case SERVER_LOC:	::CopyServerData( NODE_SYSTEM );	break;
				}
				break;

			case SERVER_DB:
				switch( EcrmanOptions.GetFeaturesServerSettingsType() )
				{
				case SERVER_LOC:	::CopyServerData( NODE_DATABASE );	break;
				}
				break;
			}

			if ( oldStockOptions.GetGlobalCategoryFlag() == TRUE )
				if ( StockOptions.GetGlobalCategoryFlag() == FALSE )
					::CopyGlobalData( NODE_CATEGORY );

			if ( oldStockOptions.GetGlobalAllowanceFlag() == TRUE )
				if ( StockOptions.GetGlobalAllowanceFlag() == FALSE )
					::CopyGlobalData( NODE_ALLOWANCE );

			if ( StockOptions.GetStockMaxSuppliersPerItem() != oldStockOptions.GetStockMaxSuppliersPerItem() )
			{
				CString strMsg = "";
				CStringArray WarningArray;
					
				if ( StockOptions.GetStockMaxSuppliersPerItem() > oldStockOptions.GetStockMaxSuppliersPerItem() )
				{
					WarningArray.Add ( "Increase Suppliers per Stock Record" );
					WarningArray.Add ( "The maximum number of suppliers for each stock record will be increased" );
					WarningArray.Add ( "" );
					WarningArray.Add ( "Existing supplier information for each record will be retained" );
				}
				else
				{
					WarningArray.Add ( "Decrease Suppliers per Stock Record" );
					WarningArray.Add ( "The maximum number of suppliers for each stock record will be reduced." );
					WarningArray.Add ( "" );
					WarningArray.Add ( "Any extra supplier information for each stock item will be lost." );
				}

				if ( Prompter.Warning( WarningArray ) != IDYES )
					StockOptions.SetStockMaxSuppliersPerItem( oldStockOptions.GetStockMaxSuppliersPerItem() );
				else
				{		
					StatusProgress.Lock( TRUE, "Updating stock records" );
						
					for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
					{
						CWordArray arrayReadWrite;
						arrayReadWrite.Add( NODE_STOCK );

						CWordArray arrayReadOnly;
						arrayReadOnly.Add( NODE_SUPPLIER );

						CDataManagerInfo info;
						if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info ) == TRUE )
						{
							CString str;
							str.Format( "(%d)", dbDatabase.GetAt( nDbIdx ) );
							StatusProgress.SetDialogText( str );

							int nSize = DataManager.Stock.GetSize();

							for ( int nStockIdx = 0; nStockIdx < nSize; nStockIdx++ )
							{
								StatusProgress.SetPos ( nStockIdx, nSize );
								CStockCSVRecord StockRecord;
								DataManager.Stock.GetAt ( nStockIdx, StockRecord );
								StockRecord.TidySuppliers( StockOptions.GetStockMaxSuppliersPerItem() );
								DataManager.Stock.SetAt ( nStockIdx, StockRecord );
							}
						
							DataManager.WriteStock( info, TRUE, TRUE );
							DataManager.CloseDatabase();	
						}
					}
					StatusProgress.Unlock();
				}
			}
				
			EcrmanOptions.Write();
			EmailOptions.Write();
			PCOptionsHost.Write();
			PCOptionsClient.Write();

			if ( AutoBackupOptions.HasScheduleChanged() )
			{
				AutoBackupProcessor.ScheduleNextBackup( FALSE );

				if ( pFrame -> GetBackupResultDlg() != NULL )
					pFrame -> GetBackupResultDlg() -> UpdateDisplay();
			}
			
			AutoBackupOptions.Write();
			StockOptions.Write();
			SysInfo.Write();
			pFrame -> SetPasswordMenuAccess();

			AutoExpireHistory.Reset();
			AutoExpireHistory.ExpireReportsNow();

			for ( int nDbIdx = 0; nDbIdx <= dbDatabase.GetSize(); nDbIdx++ )
				CKeyboardHelpers::RemovePluFile( nDbIdx );

			CPosTrayTokenFiles::CreateSystemUpdateFile();
		}
	}
}

/**********************************************************************/
#endif
/**********************************************************************/

void CDataAccessHelpersSetup::SetupSystem()
{
#ifdef STOCKMAN_SYSTEM
	CMainFrameStockman* pFrame = (CMainFrameStockman*) m_pParentWnd;
#else
	CMainFrameEcrman* pFrame = (CMainFrameEcrman*) m_pParentWnd;
#endif

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "change these settings";
	infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Setup locations";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = pFrame;

	CLockManager LockManager( infoLock );

	if ( LockManager.GetLock() == TRUE )
	{
		NetworkTerminalMap.BuildMap();
		CPropertySheetSetup propSheet( m_pParentWnd );
		propSheet.DoModal();
		propSheet.SaveSystem();
		NetworkTerminalMap.BuildMap();
		pFrame -> Housekeeping();
		pFrame -> CheckNetworks();

#ifdef STOCKMAN_SYSTEM
		//FORCE THE SYSTEM TO NOTICE NEW STOCKPOINTS
		SystemMapFull.CreateSiteMap();
#endif

		{
			CCustomExportManagerBase Export;

			switch (SysInfo.GetLoyaltyOrSmartPayType())
			{
			case LOYALTY_TYPE_SMP_V4:
				Export.WriteEposTerminalFile();
				break;

			case LOYALTY_TYPE_LOY_V4:
				Export.WriteDbInfoFile();
				break;
			}
		}
	}
}

/**********************************************************************/
