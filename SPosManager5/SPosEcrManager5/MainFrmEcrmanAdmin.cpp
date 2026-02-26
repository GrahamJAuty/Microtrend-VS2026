/**********************************************************************/
#include "..\CommonEcrStock\BarcodeDeleteDlg.h"
#include "..\CommonEcrStock\ClearHistoryDlgEcrman.h"
#include "CommsHandshaker.h"
#include "LockManager.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "..\CommonEcrStock\PasswordListDlg.h"
#include "..\CommonEcrStock\PluDeleteUnusedDlg.h" 
#include "..\CommonEcrStock\SelectSingleDatabaseDlg.h"
/**********************************************************************/
#include "MainFrmEcrman.h"
/**********************************************************************/

void CMainFrameEcrman::OnAdminPasswords() 
{
	if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "edit passwords" );
	else
	{
		PasswordArray.RememberSettings();

		CPasswordListDlg dlg ( this );
		dlg.m_bLogPasswords = EcrmanOptions.GetPasswordsLogFlag();

		if ( dlg.DoModal() == IDOK )
		{
			PasswordArray.Write();
			EcrmanOptions.SetPasswordsLogFlag ( dlg.m_bLogPasswords != 0 );
			EcrmanOptions.Write();
		}
		else
			PasswordArray.RestoreSettings();

		DataManager.UnlockAllUsers();
	}
}

/**********************************************************************/

void CMainFrameEcrman::OnAdminDeleteReports() 
{
	if ( dbLocation.GetSize() == 0 )
	{
		ShowLocationError();
		return;
	}

	if ( PasswordArray.CheckLocationAccessLoc() == FALSE )
	{
		Prompter.LocationAccessError();
		return;
	}

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "delete reports";
	infoLock.m_nCommsAction = COMMS_ACTION_DISABLE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Delete reports";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = this;

	CLockManager LockManager( infoLock );

	if ( LockManager.GetLock() == TRUE )
	{
		CClearHistoryDlgEcrman dlgClear ( this );
		dlgClear.DoModal();
	}
}

/**********************************************************************/

void CMainFrameEcrman::OnAdminDeletePluUnsold() 
{
	CPluDeleteUnusedDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/

void CMainFrameEcrman::OnAdminDeleteBarcodes() 
{
	bool bContinue = TRUE;
	
	while( TRUE == bContinue )
	{
		bContinue = FALSE;

		int nDbIdx = 0;
		if ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_SYSTEM )
			nDbIdx = PasswordArray.GetLocationAccessDbIdx();
		else if ( dbDatabase.GetSize() > 1 )
		{
			CSelectSingleDatabaseDlg dlg ( "Delete Unlinked Barcodes", nDbIdx, this );
			
			if ( dlg.DoModal() != IDOK )
				return;

			nDbIdx = dlg.GetDbIdx();
			bContinue = TRUE;
		}

		CDataManagerInfo info;
		
		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_BARCODE );

		if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info, FALSE ) == TRUE )
		{
			CArray<int,int> arrayBarcodeIdx;
			DataManager.Barcode.GetUnlinkedBarcodeList( arrayBarcodeIdx );
			int nUnlinked = arrayBarcodeIdx.GetSize();

			int nMessage = 0;

			if ( 0 == nUnlinked )
				nMessage = 1;
			else
			{
				CBarcodeDeleteDlg dlgDelete( arrayBarcodeIdx, this );
				
				if ( dlgDelete.DoModal() == IDOK )
				{
					DataManager.Barcode.DeleteBarcodes( arrayBarcodeIdx );
					
					if ( DataManager.WriteBarcode( info ) == TRUE )
					{
						CDWordArray dw;
						dw.Add( 4 );
						dw.Add(nUnlinked);
						dw.Add(arrayBarcodeIdx.GetSize());
						dw.Add(DataManager.Barcode.GetSize() );
						MessageLogger.LogSystem( 11, 1, TRUE, dw );
					}
					else
						nMessage = 3;
				}
			}

			DataManager.CloseDatabase();

			switch( nMessage )
			{
			case 1:
				Prompter.Info( "No unlinked barcodes were found" );
				break;

			case 2:
				Prompter.Info( "Barcodes were deleted as requested" );
				break;

			case 3:
				Prompter.WriteError( info );
				break;
			}

		}
	}
}

/**********************************************************************/
