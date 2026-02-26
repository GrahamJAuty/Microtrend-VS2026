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
#include "LocationCSVArray.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "MainFrmEcrman.h"
/**********************************************************************/
void CMainFrameEcrman::OnSpecialKeyA(){ ProcessSpecialKey("A"); }
void CMainFrameEcrman::OnSpecialKeyB(){ ProcessSpecialKey("B"); }
void CMainFrameEcrman::OnSpecialKeyC(){ ProcessSpecialKey("C"); }
void CMainFrameEcrman::OnSpecialKeyD(){ ProcessSpecialKey("D"); }
void CMainFrameEcrman::OnSpecialKeyE(){ ProcessSpecialKey("E"); }
void CMainFrameEcrman::OnSpecialKeyF(){ ProcessSpecialKey("F"); }
void CMainFrameEcrman::OnSpecialKeyG(){ ProcessSpecialKey("G"); }
void CMainFrameEcrman::OnSpecialKeyH(){ ProcessSpecialKey("H"); }
void CMainFrameEcrman::OnSpecialKeyI(){ ProcessSpecialKey("I"); }
void CMainFrameEcrman::OnSpecialKeyJ(){ ProcessSpecialKey("J"); }
void CMainFrameEcrman::OnSpecialKeyK(){ ProcessSpecialKey("K"); }
void CMainFrameEcrman::OnSpecialKeyL(){ ProcessSpecialKey("L"); }
void CMainFrameEcrman::OnSpecialKeyM(){ ProcessSpecialKey("M"); }
void CMainFrameEcrman::OnSpecialKeyN(){ ProcessSpecialKey("N"); }
void CMainFrameEcrman::OnSpecialKeyO(){ ProcessSpecialKey("O"); }
void CMainFrameEcrman::OnSpecialKeyP(){ ProcessSpecialKey("P"); }
void CMainFrameEcrman::OnSpecialKeyQ(){ ProcessSpecialKey("Q"); }
void CMainFrameEcrman::OnSpecialKeyR(){ ProcessSpecialKey("R"); }
void CMainFrameEcrman::OnSpecialKeyS(){ ProcessSpecialKey("S"); }
void CMainFrameEcrman::OnSpecialKeyT(){ ProcessSpecialKey("T"); }
void CMainFrameEcrman::OnSpecialKeyU(){ ProcessSpecialKey("U"); }
void CMainFrameEcrman::OnSpecialKeyV(){ ProcessSpecialKey("V"); }
void CMainFrameEcrman::OnSpecialKeyW(){ ProcessSpecialKey("W"); }
void CMainFrameEcrman::OnSpecialKeyX(){ ProcessSpecialKey("X"); }
void CMainFrameEcrman::OnSpecialKeyY(){ ProcessSpecialKey("Y"); }
void CMainFrameEcrman::OnSpecialKeyZ(){ ProcessSpecialKey("Z"); }
void CMainFrameEcrman::OnSpecialKey0(){ ProcessSpecialKey("0"); }
void CMainFrameEcrman::OnSpecialKey1(){ ProcessSpecialKey("1"); }
void CMainFrameEcrman::OnSpecialKey2(){ ProcessSpecialKey("2"); }
void CMainFrameEcrman::OnSpecialKey3(){ ProcessSpecialKey("3"); }
void CMainFrameEcrman::OnSpecialKey4(){ ProcessSpecialKey("4"); }
void CMainFrameEcrman::OnSpecialKey5(){ ProcessSpecialKey("5"); }
void CMainFrameEcrman::OnSpecialKey6(){ ProcessSpecialKey("6"); }
void CMainFrameEcrman::OnSpecialKey7(){ ProcessSpecialKey("7"); }
void CMainFrameEcrman::OnSpecialKey8(){ ProcessSpecialKey("8"); }
void CMainFrameEcrman::OnSpecialKey9(){ ProcessSpecialKey("9"); }
/**********************************************************************/

bool CMainFrameEcrman::SpecialKeyValidationCheck()
{
	bool bResult = DatabaseValidator.ValidAtStartup();

	if (FALSE == bResult)
	{
		Prompter.Error("This option is unavailable because the product database\nwas invalid at program startup.");
	}

	return bResult;
}

/**********************************************************************/

void CMainFrameEcrman::ProcessSpecialKey( const char* sz )
{
	CString strKey = sz;
	strKey.MakeUpper();

	{
		CString strTest = SpecialKeys.GetHiddenOptionsKey();
		strTest.MakeUpper();
		
		if ( strTest == strKey )
		{
			ProcessHiddenOptions();
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
}

/**********************************************************************/

void CMainFrameEcrman::ProcessHiddenOptions()
{
	if (PasswordArray.CheckLocationAccessSystem() == FALSE)
	{
		return;
	}

	if (DataManager.LockAllUsers() == FALSE)
	{
		Prompter.LockError("access these options");
	}
	else
	{
		bool bOldPriceSetFlag = DealerFlags.GetPriceSetExcludeByDefaultFlag();

		CStringArray arrayBuffer;
		DealerFlags.WriteToArray(arrayBuffer);

		CPropertySheetHiddenOptions propSheet(this);

		if (propSheet.DoModal() != IDOK)
		{
			DealerFlags.ReadFromArray(arrayBuffer);
		}
		else
		{
			if (DealerFlags.WriteToFile() == FALSE)
			{
				DealerFlags.ReadFromArray(arrayBuffer);
				Prompter.Error("Unable to write dealer flags");
			}
			else
			{
				DealerFlags.ProcessLocationSelectorOrder();
			}
		}

		MessageLogger.LogSystem(12, 2, TRUE, DealerFlags.GetLogFileWritesFlag() ? 1 : 0);

		if (DealerFlags.GetPriceSetExcludeByDefaultFlag() != bOldPriceSetFlag)
		{
			CPriceSetItemManager TempPriceSetItemManager;
			TempPriceSetItemManager.ConvertIncludeExcludeFiles();
		}

		m_MenuChanger.ShowItem(IDM_FILE_WRITE_LOG, DealerFlags.GetLogFileWritesFlag());
		m_MenuChanger.ShowItem(IDM_REPORTS_BRITANNIA, FALSE);
		m_MenuChanger.BuildMenu(GetMenu());
		DrawMenuBar();

		SetTitleBarText();

		DataManager.UnlockAllUsers();
	}
}

/**********************************************************************/

void CMainFrameEcrman::ProcessRebuildHistory()
{
	if (dbLocation.GetSize() == 0)
	{
		return;
	}

	if (dbNetwork.ValidateNetworks(FALSE, TRUE, TRUE, TRUE) == FALSE)
	{
		return;
	}

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "rebuild history files";
	infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Rebuild sales history";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = this;

	CLockManager LockManager(infoLock);

	if (LockManager.GetLock() == TRUE)
	{
		CRebuildHistoryDlg dlg(this);
		dlg.DoModal();
	}
}

/**********************************************************************/

void CMainFrameEcrman::ProcessSetPluLength()
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

void CMainFrameEcrman::ProcessCustomerAutoDelete()
{
	if (dbLocation.GetSize() == 0)
	{
		return;
	}

	if (dbNetwork.ValidateNetworks(FALSE, TRUE, TRUE, TRUE) == FALSE)
	{
		return;
	}

	if ((EcrmanOptions.GetFeaturesCustomerOneOffAutoDeleteFlag() == FALSE) && (EcrmanOptions.GetFeaturesCustomerAccountAutoDeleteFlag() == FALSE))
	{
		return;
	}

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

void CMainFrameEcrman::ProcessSetupLayout()
{
	if ((PasswordArray.IsMasterPassword() == TRUE) && (SysInfo.IsEcrLiteSystem() == FALSE))
	{
		if (BUTMGR_MODE_USER == ButtonManager.GetButtonManagerMode())
		{
			if (DataManager.LockAllUsers() == FALSE)
			{
				Prompter.LockError("edit button options");
			}
			else
			{
				ButtonManager.SetButtonManagerMode(BUTMGR_MODE_EDIT_LAYOUT);
				BackgroundLayoutInfo.CreateLayoutFolders();
				BackgroundLayoutInfo.TempImageModeOn();
				ButtonOptions.RememberSettings();
				RefreshLayout();

				CLayoutOptionsDlg dlg(this);
				if (dlg.DoModal() == IDOK)
				{
					BackgroundLayoutInfo.TempImageModeOff(TRUE);
					ButtonOptions.Write();
				}
				else
				{
					BackgroundLayoutInfo.TempImageModeOff(FALSE);
					ButtonOptions.RestoreSettings();
				}

				ButtonManager.SetButtonManagerMode(BUTMGR_MODE_USER);
				RefreshLayout();

				DataManager.UnlockAllUsers();
			}
		}
	}
}

/**********************************************************************/

void CMainFrameEcrman::ProcessSetupLists()
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
						if (DataManager.LockAllUsers() == FALSE)
						{
							Prompter.LockError("edit button lists");
						}
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
				if (ButtonManager.CheckButtonLibraryChanges() == TRUE)
				{
					int nResult = Prompter.YesNoCancel("Button lists have been modified - save changes");
					if (nResult != IDCANCEL)
					{
						if (nResult == IDYES)
						{
							ButtonManager.TempImageModeOff(TRUE);
							ButtonManager.WriteButtonActions();
						}
						else
						{
							ButtonManager.TempImageModeOff(FALSE);
							ButtonManager.RestoreButtonLibrary();
						}
					}
					else
					{
						bExitListMode = FALSE;
					}
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



