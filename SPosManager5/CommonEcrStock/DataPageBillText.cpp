/**********************************************************************/
 
/**********************************************************************/
#include "BillAndReceiptTexts.h"
#include "CommsBatchDlg.h"
#include "DownloadDateDlg.h"
//#include "EcrmanOptionsIni.h"
#include "EditBillTextDlg.h"
#include "LIstDataBillTextTerminalDlg.h"
#include "LocationCSVArray.h"
#include "Password.h"
 
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareBillTextPage()
{	
	//m_Filter.PrepareBillTextFilter();
}

/**********************************************************************/

void CPropPageSystemDataList::BuildBillTextList()
{
	RebuildBillTextList();

	m_buttonAction1.SetWindowText ( "&Edit" );
	m_buttonAction2.SetWindowText ( "&Terminals" );
	m_buttonAction3.SetWindowText ( "&Download" );
	m_buttonAction4.ShowWindow ( SW_HIDE );
	m_buttonAction5.ShowWindow ( SW_HIDE );
	m_buttonAction6.ShowWindow ( SW_HIDE );

	if ( PasswordArray.GetEnable( PasswordTicks::CommsDownloads ) == FALSE )
		m_buttonAction3.ShowWindow( SW_HIDE );
	
	SelectLine( m_listLocation, 0 );
}

/**********************************************************************/

void CPropPageSystemDataList::RebuildBillTextList()
{
	m_arrayLocation.RemoveAll();
	m_arrayAction.RemoveAll();
	
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
				AddDatabase( nDbIdx );
	}
	else
	{
		CWordArray arrayDbIdx;
		dbDatabase.SortByName( arrayDbIdx );

		for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
		{
			int nDbIdx = arrayDbIdx.GetAt(n);
			if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
				AddDatabase( nDbIdx );
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationBillText()
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetBillTextAction( LocSelEntity, nAction );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	AddAction( 999999, "System Texts" );
	AddAction( 999998, "Database Texts" );

	int nDbIdx = LocSelEntity.GetDbIdx();

	CArray<int,int> arrayLocIdx;
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );
	else
		dbLocation.SortByName( arrayLocIdx, nDbIdx );

	for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
	{
		int nLocIdx = arrayLocIdx.GetAt(n);
		if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == TRUE )
			AddAction( nLocIdx, dbLocation.GetName( nLocIdx ) );			
	}
	
	ForceSelectAction( nAction );
}

/**********************************************************************/

void CPropPageSystemDataList::GetBillTextAction( CLocationSelectorEntity& LocSelEntity, int& nAction )
{
	LocSelEntity.SetSystemNode();
	nAction = 0;

	{
		int nSel = m_listLocation.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayLocation.GetSize() ) )
			LocSelEntity = m_arrayLocation.GetAt( nSel );
	}

	{
		int nSel = m_listAction.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayAction.GetSize() ) )
			nAction = m_arrayAction.GetAt(nSel);
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectActionBillText()
{
}

/**********************************************************************/

void CPropPageSystemDataList::Button1BillTextPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelEdit;
	GetBillTextAction( LocSelEdit, nAction );
	int nDbIdx = LocSelEdit.GetDbIdx();

	bool bGotSelect = TRUE;

	switch( nAction )
	{
	case 999999:
		LocSelEdit.SetSystemNode();
		bGotSelect = TRUE;
		break;

	case 999998:
		LocSelEdit.SetDatabaseNode( nDbIdx );
		bGotSelect = TRUE;
		break;

	default:
		LocSelEdit.SetLocationNode( nAction );
		bGotSelect = TRUE;
		break;
	}

	if ( TRUE == bGotSelect )
	{
		EditBillReceiptTexts( LocSelEdit );
	}
}

/**********************************************************************/

void CPropPageSystemDataList::EditBillReceiptTexts( CLocationSelectorEntity& LocSelEdit ) 
{
	CBillAndReceiptTexts Texts;
	Texts.SetLocSelEntity( LocSelEdit, FALSE );
	
	CSSFile fileLock;
	if ( fileLock.Open( Texts.GetLockFilename(), "wb" ) == FALSE )
	{
		MessageBox ( "This text file is currently being modified by another user.\n\nPlease try again later.", "Access Denied", MB_ICONEXCLAMATION | MB_OK );
	}
	else
	{
		
		Texts.Read();
		
		CEditBillTextDlg dlg( LocSelEdit, Texts, this );
		
		if ( dlg.DoModal() == IDOK )
		{
			if ( ( dlg.GetUseParentFlag() == FALSE ) || ( LocSelEdit.GetConType() == NODE_SYSTEM ) )
				Texts.Write();
			else
				Texts.DeleteFile();
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button2BillTextPage() 
{
	int nLocIdx;
	CLocationSelectorEntity LocSelSelect;
	GetBillTextAction( LocSelSelect, nLocIdx );
	
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < dbLocation.GetSize() ) )
	{
		if ( dbLocation.GetTerminalCount( nLocIdx ) >= 1 )
		{
			CListDataBillTextTerminalDlg dlg( nLocIdx, this );
			dlg.DoModal();
		}
		else
		{
			Prompter.Error( "You have not defined any terminals for this location." );
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button3BillTextPage()
{
	int nAction;
	CLocationSelectorEntity LocSelSelect;
	GetBillTextAction( LocSelSelect, nAction );
	int nDbIdx = LocSelSelect.GetDbIdx();

	CArray<int,int> arrayLocIdx;

	switch( nAction )
	{
	case 999999:
		{
			for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
				arrayLocIdx.Add( nLocIdx );
		}
		break;

	case 999998:
		dbLocation.GetDatabaseLocIdxSet( LocSelSelect.GetDbIdx(), arrayLocIdx );
		break;

	default:
		arrayLocIdx.Add( nAction );
		break;
	}

	if ( arrayLocIdx.GetSize() > 0 )
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask( DN_RECEIPT, arrayLocIdx );
		CString strDate = "";

		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );

			CString strTitle;
			TaskHeader.SetName( "Download Receipt and Bill Texts" );
			strTitle.Format ( "Epos Import Date (%s)", TaskHeader.GetName() );

			CDownloadDateDlg dlgDate( strTitle, m_pParentWnd );
			if ( dlgDate.RunIfNeeded() == FALSE )
				return;

			if ( dlgDate.m_bImmediate == TRUE )
				TaskHeader.SetImportType( IMPORT_IMMEDIATE );
			else
			{
				TaskHeader.SetImportType( IMPORT_SPECIFIED );
				strDate = dlgDate.m_strDate;
			}

			TaskDatabase.SetEditableTaskHeader( TaskHeader );
		}
	
		CKeyboardMap KeyboardMap;
		CCommsBatchDlg dlgComms( TaskDatabase, strDate, FALSE, KeyboardMap, -1, 0, 0, this );
		dlgComms.DoModal();
	}
}

/**********************************************************************/
