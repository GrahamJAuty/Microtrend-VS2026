/**********************************************************************/
#include "CustomerAutoDeleter.h"
#include "CustomerAutoImporterEOD.h"
#include "CustomerExtractor.h"
#include "CustomerTrackTraceDlg.h"
#include "DatabaseCSVArray.h"
#include "ListDataCustomerAccountDlg.h"
#include "ListDataCustomerOneOffDlg.h"
/**********************************************************************/
#include "DataAccessHelpersCustomer.h"
/**********************************************************************/

CDataAccessHelpersCustomer::CDataAccessHelpersCustomer()
{
}

/**********************************************************************/

void CDataAccessHelpersCustomer::EditCustomerAccount( int nLocIdx )
{
	EditCustomerInternal( nLocIdx, NODE_CUSTOMER_ACCOUNT );
}

/**********************************************************************/

void CDataAccessHelpersCustomer::EditCustomerOneOff( int nLocIdx )
{
	EditCustomerInternal( nLocIdx, NODE_CUSTOMER_ONEOFF );
}

/**********************************************************************/

void CDataAccessHelpersCustomer::CustomerTrackTrace( int nLocIdx )
{
	EditCustomerInternal( nLocIdx, NODE_CUSTOMER_TRACKTRACE );
}

/**********************************************************************/

void CDataAccessHelpersCustomer::EditCustomerInternal( int nLocIdx, int nActionType )
{
	CDataManagerInfo info;
	
	if ( DataManagerNonDb.OpenCustomerAll( DB_READWRITE, nLocIdx, info ) == FALSE )
		Prompter.ShareError( info );
	else
	{
		DataManagerNonDb.CustomerAccount.LoadHeaderStartDateRecord();
		DataManagerNonDb.CustomerAccount.MoveDeletionsToHelperArray();
		DataManagerNonDb.CustomerOneOff.LoadHeaderStartDateRecord();
		DataManagerNonDb.CustomerOneOff.MoveDeletionsToHelperArray();

		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		
		CString strDateNow;
		strDateNow.Format( "%4.4d%2.2d%2.2d",
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay() );

		CString strSyncDate = "";
		bool bGotSyncDate = FALSE;

		if ( DataManagerNonDb.CustomerAccount.GetSize() != 0 )
		{
			CSSFile fileDate;
			CFilenameUpdater FnUp( SysFiles::CustomerSyncDate, nLocIdx );
			if ( fileDate.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
			{
				fileDate.ReadString( strSyncDate );
				fileDate.Close();
			}
	
			if ( strSyncDate.GetLength() == 8 )
			{
				if ( ::TestNumeric( strSyncDate ) == TRUE )
				{
					COleDateTime dateSync = COleDateTime(
						atoi( strSyncDate.Left(4) ),
						atoi( strSyncDate.Mid(4,2) ),
						atoi( strSyncDate.Right(2) ),
						0, 0, 0 );

					if ( COleDateTime::valid == dateSync.m_status )
					{
						dateSync -= COleDateTimeSpan( 7, 0, 0, 0 );

						strSyncDate.Format( "%4.4d%2.2d%2.2d",
							dateSync.GetYear(),
							dateSync.GetMonth(),
							dateSync.GetDay() );

						bGotSyncDate = TRUE;
					}
				}
			}
		}

		if ( FALSE == bGotSyncDate )
			strSyncDate = "20110101";

		CCustomerCSVArray arrayTemp;
		CCustomerExtractor Extractor( nLocIdx, strSyncDate, strDateNow, arrayTemp );

		int nAdded = 0;
		int nRemoved = 0;
		int nFirstNewID = 0;
		DataManagerNonDb.CustomerAccount.Sync( arrayTemp, DataManagerNonDb.CustomerOneOff, TRUE, FALSE, nAdded, nRemoved, nFirstNewID, TRUE );

		bool bSyncOK = TRUE;

		DataManagerNonDb.CustomerAccount.SaveHeaderStartDateRecord();
		DataManagerNonDb.CustomerAccount.MoveDeletionsToMainArray();
		DataManagerNonDb.CustomerOneOff.SaveHeaderStartDateRecord();
		DataManagerNonDb.CustomerOneOff.MoveDeletionsToMainArray();
		
		if ( nAdded != 0 )
			if ( DataManagerNonDb.WriteCustomerAll( info ) == TRUE )
				bSyncOK = FALSE;

		DataManagerNonDb.CustomerAccount.LoadHeaderStartDateRecord();
		DataManagerNonDb.CustomerAccount.MoveDeletionsToHelperArray();
		DataManagerNonDb.CustomerOneOff.LoadHeaderStartDateRecord();
		DataManagerNonDb.CustomerOneOff.MoveDeletionsToHelperArray();

		if ( TRUE == bSyncOK )
		{
			CSSFile fileDate;
			CFilenameUpdater FnUp( SysFiles::CustomerSyncDate, nLocIdx );
			if ( fileDate.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
				fileDate.WriteLine( strDateNow );
		}

		CustomerAutoImporterEOD.ImportCustomerExternal( nLocIdx );
		CustomerAutoDeleterOneOff.DeleteCustomerExternal( nLocIdx );
		CustomerAutoDeleterAccount.DeleteCustomerExternal( nLocIdx );

		bool bSaveChanges = FALSE;
		bool bAdminOptions = FALSE;

		switch( nActionType )
		{
		case NODE_CUSTOMER_ONEOFF:
			{
				DataManagerNonDb.CustomerOneOff.ClearMoveToOtherArrayFlags();
				CListDataCustomerOneOffDlg dlg( nLocIdx, m_pParentWnd );
				bSaveChanges = ( dlg.DoModal() == IDOK );
				bAdminOptions = dlg.GetAdminOptionsFlag();
			}
			break;

		case NODE_CUSTOMER_ACCOUNT:
			{
				DataManagerNonDb.CustomerAccount.ClearMoveToOtherArrayFlags();
				CListDataCustomerAccountDlg dlg( nLocIdx, m_pParentWnd );
				bSaveChanges = ( dlg.DoModal() == IDOK );
				bAdminOptions = dlg.GetAdminOptionsFlag();
			}
			break;

		case NODE_CUSTOMER_TRACKTRACE:
			{
				DataManagerNonDb.CustomerAccount.ClearMoveToOtherArrayFlags();
				CCustomerTrackTraceDlg dlg( nLocIdx, m_pParentWnd );
				bSaveChanges = ( dlg.DoModal() == IDOK );
			}
			break;
		}

		if ( TRUE == bSaveChanges )
		{
			if ( TRUE == bAdminOptions )
			{
				if ( NODE_CUSTOMER_ONEOFF == nActionType )
				{
					DataManagerNonDb.CustomerOneOff.MoveCustomers( DataManagerNonDb.CustomerAccount );
				}
				else
				{
					DataManagerNonDb.CustomerAccount.MoveCustomers( DataManagerNonDb.CustomerOneOff );
				}
			}

			DataManagerNonDb.CustomerAccount.SaveHeaderStartDateRecord();
			DataManagerNonDb.CustomerAccount.MoveDeletionsToMainArray();
			DataManagerNonDb.CustomerOneOff.SaveHeaderStartDateRecord();
			DataManagerNonDb.CustomerOneOff.MoveDeletionsToMainArray();

			if ( DataManagerNonDb.WriteCustomerAll( info ) == FALSE )
			{
				Prompter.WriteError( info );
			}

			DataManagerNonDb.CustomerAccount.LoadHeaderStartDateRecord();
			DataManagerNonDb.CustomerAccount.MoveDeletionsToHelperArray();
			DataManagerNonDb.CustomerOneOff.LoadHeaderStartDateRecord();
			DataManagerNonDb.CustomerOneOff.MoveDeletionsToHelperArray();
		}

		DataManagerNonDb.CloseCustomerAll( info );
	}
}

/**********************************************************************/

