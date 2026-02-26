/**********************************************************************/
#include "..\CommonEcrStock\AutoDeleteCustomerDlg.h" 
#include "..\CommonEcrStock\BackgroundLayoutInfo.h"
#include "..\CommonEcrStock\ButtonDefines.h"
#include "..\CommonEcrStock\ButtonManager.h"
#include "..\CommonEcrStock\ButtonOptions.h"
#include "..\CommonEcrStock\DatabaseValidator.h"
#include "..\CommonEcrStock\LayoutOptionsDlg.h"
#include "..\CommonEcrStock\PluLengthDlg.h"
#include "..\CommonEcrStock\PriceSetItemManager.h"
#include "..\CommonEcrStock\PropertySheetHiddenOptions.h"
#include "..\CommonEcrStock\RebuildHistoryDlg.h"
#include "..\CommonEcrStock\SpecialKeys.h"
/**********************************************************************/
#include "CommsHandshaker.h"
#include "ListDataUpgradeStockDlg.h"
#include "LocationCSVArray.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "MainFrmStockman.h"
/**********************************************************************/
void CMainFrameStockman::OnSpecialKeyA(){ ProcessSpecialKey("A"); }
void CMainFrameStockman::OnSpecialKeyB(){ ProcessSpecialKey("B"); }
void CMainFrameStockman::OnSpecialKeyC(){ ProcessSpecialKey("C"); }
void CMainFrameStockman::OnSpecialKeyD(){ ProcessSpecialKey("D"); }
void CMainFrameStockman::OnSpecialKeyE(){ ProcessSpecialKey("E"); }
void CMainFrameStockman::OnSpecialKeyF(){ ProcessSpecialKey("F"); }
void CMainFrameStockman::OnSpecialKeyG(){ ProcessSpecialKey("G"); }
void CMainFrameStockman::OnSpecialKeyH(){ ProcessSpecialKey("H"); }
void CMainFrameStockman::OnSpecialKeyI(){ ProcessSpecialKey("I"); }
void CMainFrameStockman::OnSpecialKeyJ(){ ProcessSpecialKey("J"); }
void CMainFrameStockman::OnSpecialKeyK(){ ProcessSpecialKey("K"); }
void CMainFrameStockman::OnSpecialKeyL(){ ProcessSpecialKey("L"); }
void CMainFrameStockman::OnSpecialKeyM(){ ProcessSpecialKey("M"); }
void CMainFrameStockman::OnSpecialKeyN(){ ProcessSpecialKey("N"); }
void CMainFrameStockman::OnSpecialKeyO(){ ProcessSpecialKey("O"); }
void CMainFrameStockman::OnSpecialKeyP(){ ProcessSpecialKey("P"); }
void CMainFrameStockman::OnSpecialKeyQ(){ ProcessSpecialKey("Q"); }
void CMainFrameStockman::OnSpecialKeyR(){ ProcessSpecialKey("R"); }
void CMainFrameStockman::OnSpecialKeyS(){ ProcessSpecialKey("S"); }
void CMainFrameStockman::OnSpecialKeyT(){ ProcessSpecialKey("T"); }
void CMainFrameStockman::OnSpecialKeyU(){ ProcessSpecialKey("U"); }
void CMainFrameStockman::OnSpecialKeyV(){ ProcessSpecialKey("V"); }
void CMainFrameStockman::OnSpecialKeyW(){ ProcessSpecialKey("W"); }
void CMainFrameStockman::OnSpecialKeyX(){ ProcessSpecialKey("X"); }
void CMainFrameStockman::OnSpecialKeyY(){ ProcessSpecialKey("Y"); }
void CMainFrameStockman::OnSpecialKeyZ(){ ProcessSpecialKey("Z"); }
void CMainFrameStockman::OnSpecialKey0(){ ProcessSpecialKey("0"); }
void CMainFrameStockman::OnSpecialKey1(){ ProcessSpecialKey("1"); }
void CMainFrameStockman::OnSpecialKey2(){ ProcessSpecialKey("2"); }
void CMainFrameStockman::OnSpecialKey3(){ ProcessSpecialKey("3"); }
void CMainFrameStockman::OnSpecialKey4(){ ProcessSpecialKey("4"); }
void CMainFrameStockman::OnSpecialKey5(){ ProcessSpecialKey("5"); }
void CMainFrameStockman::OnSpecialKey6(){ ProcessSpecialKey("6"); }
void CMainFrameStockman::OnSpecialKey7(){ ProcessSpecialKey("7"); }
void CMainFrameStockman::OnSpecialKey8(){ ProcessSpecialKey("8"); }
void CMainFrameStockman::OnSpecialKey9(){ ProcessSpecialKey("9"); }
/**********************************************************************/

bool CMainFrameStockman::SpecialKeyValidationCheck()
{
	bool bResult = DatabaseValidator.ValidAtStartup();

	if (FALSE == bResult)
	{
		Prompter.Error("This option is unavailable because the product database\nwas invalid at program startup.");
	}

	return bResult;
}

/**********************************************************************/

void CMainFrameStockman::ProcessSpecialKey( const char* sz )
{
	CString strKey = sz;
	strKey.MakeUpper();

	{
		CString strTest = SpecialKeys.GetHiddenOptionsKey();
		strTest.MakeUpper();
		
		if ( strTest == strKey )
		{
			if (SpecialKeyValidationCheck() == TRUE)
			{
				ProcessHiddenOptions();
			}

			return;
		}
	}

	{
		CString strTest = SpecialKeys.GetRebuildSalesHistoryKey();
		strTest.MakeUpper();
		
		if ( strTest == strKey )
		{
			if (SpecialKeyValidationCheck() == TRUE)
			{
				ProcessRebuildHistory();
			}

			return;
		}
	}

	{
		CString strTest = SpecialKeys.GetPluNumberModeKey();
		strTest.MakeUpper();
		
		if ( strTest == strKey )
		{
			ProcessSetPluLength();
			return;
		}
	}

	{
		CString strTest = SpecialKeys.GetCustomerAutoDeleteKey();
		strTest.MakeUpper();
		
		if ( strTest == strKey )
		{
			if (SpecialKeyValidationCheck() == TRUE)
			{
				ProcessCustomerAutoDelete();
			}

			return;
		}
	}

	{
		CString strTest = SpecialKeys.GetButtonOptionsKey();
		strTest.MakeUpper();
		
		if ( strTest == strKey )
		{
			if (SpecialKeyValidationCheck() == TRUE)
			{
				ProcessSetupLayout();
			}

			return;
		}
	}

	{
		CString strTest = SpecialKeys.GetButtonActionsKey();
		strTest.MakeUpper();
		
		if ( strTest == strKey )
		{
			if (SpecialKeyValidationCheck() == TRUE)
			{
				ProcessSetupLists();
			}

			return;
		}
	}

	{
		CString strTest = SpecialKeys.GetStockUpgradeKey();
		strTest.MakeUpper();
		
		if ( strTest == strKey )
		{
			if (SpecialKeyValidationCheck() == TRUE)
			{
				ProcessEcrmanStock();
			}

			return;
		}
	}
}

/**********************************************************************/

void CMainFrameStockman::ProcessHiddenOptions() 
{
	if ( PasswordArray.CheckLocationAccessSystem() == FALSE )
		return;

	if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "access these options" );
	else
	{
		bool bOldPriceSetFlag = DealerFlags.GetPriceSetExcludeByDefaultFlag();
		
		CStringArray arrayBuffer;
		DealerFlags.WriteToArray( arrayBuffer );

		CPropertySheetHiddenOptions propSheet( this );
		
		if ( propSheet.DoModal() != IDOK )
		{
			DealerFlags.ReadFromArray( arrayBuffer );
		}
		else
		{
			if ( DealerFlags.WriteToFile() == FALSE )
			{
				DealerFlags.ReadFromArray( arrayBuffer );
				Prompter.Error( "Unable to write dealer flags" );
			}
			else
				DealerFlags.ProcessLocationSelectorOrder();
		}

		MessageLogger.LogSystem( 12, 2, TRUE, DealerFlags.GetLogFileWritesFlag() ? 1 : 0 );

		if ( DealerFlags.GetPriceSetExcludeByDefaultFlag() != bOldPriceSetFlag )
		{
			CPriceSetItemManager TempPriceSetItemManager;
			TempPriceSetItemManager.ConvertIncludeExcludeFiles();
		}

		m_MenuChanger.ShowItem( IDM_FILE_WRITE_LOG, DealerFlags.GetLogFileWritesFlag() );
		m_MenuChanger.ShowItem( IDM_REPORTS_BRITANNIA, DealerFlags.GetBritanniaHotelsExportFlag() );
		m_MenuChanger.BuildMenu( GetMenu() );
		DrawMenuBar();

		SetTitleBarText();

		SetPasswordMenuAccess();
		DataManager.UnlockAllUsers();
	}
}

/**********************************************************************/

void CMainFrameStockman::ProcessRebuildHistory()
{
	if ( dbLocation.GetSize() == 0 )
		return;

	if ( dbNetwork.ValidateNetworks( FALSE, TRUE, TRUE, TRUE ) == FALSE )
		return;

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "rebuild history files";
	infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Rebuild sales history";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = this;

	CLockManager LockManager( infoLock );

	if ( LockManager.GetLock() == TRUE )
	{
		CRebuildHistoryDlg dlg( this );
		dlg.DoModal();
	}
}

/**********************************************************************/

void CMainFrameStockman::ProcessSetPluLength()
{
	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "change Plu number configuration";
	infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Adjust Plu length settings";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = this;

	bool bChange = FALSE;
	{
		CLockManager LockManager( infoLock );

		if ( LockManager.GetLock() == TRUE )
		{
			CPluLengthDlg dlg( this );
			bChange = ( dlg.DoModal() == IDOK );
		}
	}

	if ( TRUE == bChange )
	{
		CommsHandshaker.PauseComms();
		SysInfo.GetMaxBasePluLen( TRUE );
		PostMessage ( WM_CLOSE );
	}
}

/**********************************************************************/

void CMainFrameStockman::ProcessCustomerAutoDelete()
{
	if ( dbLocation.GetSize() == 0 )
		return;

	if ( dbNetwork.ValidateNetworks( FALSE, TRUE, TRUE, TRUE ) == FALSE )
		return;

	if ( ( EcrmanOptions.GetFeaturesCustomerOneOffAutoDeleteFlag() == FALSE ) && ( EcrmanOptions.GetFeaturesCustomerAccountAutoDeleteFlag() == FALSE ) )
		return;

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "auto delete customers";
	infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Auto Delete Customers";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = this;

	CLockManager LockManager( infoLock );

	if ( LockManager.GetLock() == TRUE )
	{
		CAutoDeleteCustomerDlg dlg( this );
		dlg.DoModal();
	}
}

/**********************************************************************/

void CMainFrameStockman::ProcessSetupLayout()
{
	if ( PasswordArray.IsMasterPassword() == TRUE )
	{
		if ( BUTMGR_MODE_USER == ButtonManager.GetButtonManagerMode() )
		{
			if ( DataManager.LockAllUsers() == FALSE )
				Prompter.LockError( "edit button layout" );
			else
			{
				ButtonManager.SetButtonManagerMode( BUTMGR_MODE_EDIT_LAYOUT );
				BackgroundLayoutInfo.CreateLayoutFolders();
				BackgroundLayoutInfo.TempImageModeOn();
				ButtonOptions.RememberSettings();
				RefreshLayout();

				CLayoutOptionsDlg dlg( this );
				if ( dlg.DoModal() == IDOK )
				{
					BackgroundLayoutInfo.TempImageModeOff(TRUE);
					ButtonOptions.Write();
				}
				else
				{
					BackgroundLayoutInfo.TempImageModeOff(FALSE);
					ButtonOptions.RestoreSettings();
				}

				ButtonManager.SetButtonManagerMode( BUTMGR_MODE_USER );
				RefreshLayout();

				DataManager.UnlockAllUsers();
			}
		}
	}
}

/**********************************************************************/

void CMainFrameStockman::ProcessSetupLists()
{	
		if ( PasswordArray.IsMasterPassword() == TRUE )
	{
		if ( ButtonOptions.GetLayoutEnable() == TRUE )
		{
			if ( BUTMGR_MODE_USER == ButtonManager.GetButtonManagerMode() )
			{
				if ( BackgroundLayoutInfo.ValidateLayout() == TRUE )
				{
					ButtonManager.RememberButtonLibrary();
					ButtonManager.TempImageModeOn();
					ButtonManager.CalculateButtonRects( TRUE );

					if ( ButtonManager.GetButtonCount() != 0 )
					{
						//**MUST** UNLOCK THIS LATER WHEN LEAVING LIST EDIT MODE
						if ( DataManager.LockAllUsers() == FALSE )
							Prompter.LockError( "edit button lists" );
						else
						{
							ButtonManager.SetButtonManagerMode( BUTMGR_MODE_EDIT_LIST );
							ButtonManager.LoadCurrentDisplayList();

							CRect rcMax(0, 0, BackgroundLayoutInfo.GetWindowSizeRequestX(), BackgroundLayoutInfo.GetWindowSizeRequestY() );
							ShowWindow( SW_RESTORE );
							SetWindowPos( NULL, 0, 0, rcMax.Width(), rcMax.Height(), SWP_NOZORDER | SWP_NOACTIVATE );
							GetActiveView() -> Invalidate();
						}
					}
				}
			}
			else
			{
				if ( ButtonManager.GetButtonEditDlg() != NULL )
				{
					delete( ButtonManager.GetButtonEditDlg() );
					ButtonManager.SetButtonEditDlg( NULL );
				}

				bool bExitListMode = TRUE;
				if ( ButtonManager.CheckButtonLibraryChanges() == TRUE )
				{
					int nResult = Prompter.YesNoCancel( "Button lists have been modified - save changes" );
					if ( nResult != IDCANCEL )
					{
						if ( nResult == IDYES )
						{
							ButtonManager.TempImageModeOff( TRUE );
							ButtonManager.WriteButtonActions();
						}
						else
						{
							ButtonManager.TempImageModeOff( FALSE );
							ButtonManager.RestoreButtonLibrary();
						}
					}
					else
						bExitListMode = FALSE;
				}

				if ( TRUE == bExitListMode )
				{
					//RELEASE LOCK THAT WAS APPLIED WHEN ENTERING LIST EDIT MODE
					DataManager.UnlockAllUsers();

					ButtonManager.SetButtonManagerMode( BUTMGR_MODE_USER );
					ButtonManager.GoHome();
					
					CRect rcMax(0, 0, BackgroundLayoutInfo.GetWindowSizeRequestX(), BackgroundLayoutInfo.GetWindowSizeRequestY() );
					ShowWindow( SW_RESTORE );
					SetWindowPos( NULL, 0, 0, rcMax.Width(), rcMax.Height(), SWP_NOZORDER | SWP_NOACTIVATE );
					GetActiveView() -> Invalidate();
				}
			}
		}
	}
}

/**********************************************************************/

void CMainFrameStockman::ProcessEcrmanStock() 
{
	if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "access these options" );
	else
	{
		CListDataUpgradeStockDlg dlg( this );
		if ( dlg.IsRelevant() == TRUE )
			dlg.DoModal();	

		DataManager.UnlockAllUsers();
	}
}

/**********************************************************************/