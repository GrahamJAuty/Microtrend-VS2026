/**********************************************************************/
 
/**********************************************************************/
#include "CloneDatabaseDlg.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "KeyboardSetCSVArray.h"
#include "NetworkCSVArray.h"
#include "Password.h"
 
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareKeyboardPage()
{	
}

/**********************************************************************/

void CPropPageSystemDataList::BuildKeyboardList()
{
	m_arrayLocation.RemoveAll();
	m_arrayAction.RemoveAll();

	int nLastDbIdx = -1;

	bool bShowDatabase = ( EcrmanOptions.GetFeaturesDatabaseEditingType() != 2 );
	for ( int nMapIdx = 0; nMapIdx < m_pHelpers -> m_Keyboard.GetKeyboardMapSize(); nMapIdx++ )
	{
		CKeyboardMapInfo info;
		m_pHelpers -> m_Keyboard.GetKeyboardMapInfo( nMapIdx, info );

		if ( TRUE == bShowDatabase )
		{
			if ( info.m_nDbIdx != nLastDbIdx )
			{
				AddDatabase( info.m_nDbIdx );
				nLastDbIdx = info.m_nDbIdx;
			}
		}

		if ( KBD_MAP_SET == info.m_nEntryType )
			AddKeyboardMapSet( nMapIdx, info.m_nKbsIdx, bShowDatabase );

		if ( KBD_MAP_LOC == info.m_nEntryType )
			AddKeyboardMapLoc( nMapIdx, info.m_nLocIdx, bShowDatabase );
	}
		
	m_buttonAction1.SetWindowText ( "Edit" );
	m_buttonAction2.SetWindowText ( "Clone" );
	m_buttonAction3.SetWindowText ( "Delete" );
	m_buttonAction4.ShowWindow( SW_HIDE );
	m_buttonAction5.SetWindowText ( "Download" );	
	m_buttonAction6.SetWindowText ( "Name" );

	SelectLine( m_listLocation, 0 );
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationKeyboard()
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetKeyboardAction( LocSelEntity, nAction );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	if ( LocSelEntity.GetConType() == NODE_DBKEYBOARDSET )
	{
		CKeyboardMapInfo infoMap;
		m_pHelpers -> m_Keyboard.GetKeyboardMapInfo( LocSelEntity.GetEntityIdx(), infoMap );

		CDataNodeInfoKeyboard infoNode;
		m_pHelpers -> m_Keyboard.ConvertMapInfoToNodeInfo( infoMap, infoNode );

		CString strSPOSVersion;
		CNetworkCSVRecord::GetTreeNameExternal( CONNECTION_TYPE_STANDARD_NONE, infoNode.m_nSPOSVersion, strSPOSVersion, TRUE );
		AddAction( 9999, strSPOSVersion );

		for ( int nKbNo = 1; nKbNo <= EcrmanOptions.GetFeaturesKeyboardsPerLocation(); nKbNo++ )
		{
			CString str = "--> ";
			infoNode.m_nKbNo = nKbNo;
			str += m_pHelpers -> m_Keyboard.GetKeyboardLabel( infoNode );
			AddAction( NODE_KEYBOARD1 + nKbNo - 1, str );
		}
	}
	else
	{
		m_buttonAction1.EnableWindow( FALSE );
		m_buttonAction2.EnableWindow( FALSE );
		m_buttonAction3.EnableWindow( FALSE );
		m_buttonAction5.EnableWindow( FALSE );
		m_buttonAction6.EnableWindow( FALSE );
	}

	ForceSelectAction( nAction );
}

/**********************************************************************/

void CPropPageSystemDataList::GetKeyboardAction( CLocationSelectorEntity& LocSelEntity, int& nAction )
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

void CPropPageSystemDataList::SelectActionKeyboard()
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetPriceSetAction( LocSelEntity, nAction );

	if ( 9999 == nAction )
	{
		m_buttonAction1.EnableWindow( FALSE );	//EDIT
		m_buttonAction2.EnableWindow( TRUE );	//CLONE
		m_buttonAction3.EnableWindow( TRUE );	//DELETE
		m_buttonAction5.EnableWindow( TRUE );	//DOWNLOAD
		m_buttonAction6.EnableWindow( FALSE );	//NAME
	}
	else
	{
		int nKbNo = m_pHelpers -> m_Keyboard.GetKbNoByNodeType( nAction );

		if ( 0 == nKbNo )
		{
			m_buttonAction1.EnableWindow( FALSE );	//EDIT
			m_buttonAction2.EnableWindow( FALSE );	//CLONE
			m_buttonAction3.EnableWindow( FALSE );	//DELETE
			m_buttonAction5.EnableWindow( FALSE );	//DOWNLOAD
			m_buttonAction6.EnableWindow( FALSE );	//NAME
		}
		else
		{
			CKeyboardMapInfo infoMap;
			m_pHelpers -> m_Keyboard.GetKeyboardMapInfo( LocSelEntity.GetEntityIdx(), infoMap );

			CDataNodeInfoKeyboard infoNode;
			m_pHelpers -> m_Keyboard.ConvertMapInfoToNodeInfo( infoMap, infoNode );
			infoNode.m_nKbNo = nKbNo;			
			bool bFile = m_pHelpers -> m_Keyboard.KeyboardInUse( infoNode );

			m_buttonAction1.EnableWindow( TRUE );	//EDIT
			m_buttonAction2.EnableWindow( bFile );	//CLONE
			m_buttonAction3.EnableWindow( bFile );	//DELETE
			m_buttonAction5.EnableWindow( bFile );	//DOWNLOAD
			m_buttonAction6.EnableWindow( bFile );	//NAME
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button1KeyboardPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetKeyboardAction( LocSelEntity, nAction );

	int nKbNo = m_pHelpers -> m_Keyboard.GetKbNoByNodeType( nAction );

	if ( 0 != nKbNo )
	{
		CKeyboardMapInfo infoMap;
		m_pHelpers -> m_Keyboard.GetKeyboardMapInfo( LocSelEntity.GetEntityIdx(), infoMap );

		CDataNodeInfoKeyboard infoNode;
		m_pHelpers -> m_Keyboard.ConvertMapInfoToNodeInfo( infoMap, infoNode );
		infoNode.m_nKbNo = nKbNo;

		if ( m_pHelpers -> m_Keyboard.EditKeyboard( infoNode ) == TRUE )
			SelectLocationKeyboard();
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button2KeyboardPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetKeyboardAction( LocSelEntity, nAction );

	CKeyboardMapInfo infoMap;
	m_pHelpers -> m_Keyboard.GetKeyboardMapInfo( LocSelEntity.GetEntityIdx(), infoMap );

	CDataNodeInfoKeyboard infoNode;
	m_pHelpers -> m_Keyboard.ConvertMapInfoToNodeInfo( infoMap, infoNode );

	bool bAttempt = FALSE;

	if ( 9999 == nAction )
	{
		infoNode.m_nKbNo = 0;
		bAttempt = TRUE;
	}
	else
	{
		int nKbNo = m_pHelpers -> m_Keyboard.GetKbNoByNodeType( nAction );

		if ( 0 != nKbNo )
		{
			infoNode.m_nKbNo = nKbNo;
			bAttempt = TRUE;
		}
	}

	if ( TRUE == bAttempt )
	{
		if ( DataManager.LockAllUsers() == FALSE )
			Prompter.LockError( "clone keyboards" );
		else
		{
			int nResult = m_pHelpers -> m_Keyboard.CloneKeyboards( infoNode );
			DataManager.UnlockAllUsers();
			SelectLocationKeyboard();
			m_pHelpers -> m_Keyboard.ShowCloneResult( nResult );
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button3KeyboardPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetKeyboardAction( LocSelEntity, nAction );

	CKeyboardMapInfo infoMap;
	m_pHelpers -> m_Keyboard.GetKeyboardMapInfo( LocSelEntity.GetEntityIdx(), infoMap );

	CDataNodeInfoKeyboard infoNode;
	m_pHelpers -> m_Keyboard.ConvertMapInfoToNodeInfo( infoMap, infoNode );

	int nResult = 0;
	bool bAttempted = FALSE;

	if ( 9999 == nAction )
	{
		infoNode.m_nKbNo = 0;
		nResult = m_pHelpers -> m_Keyboard.DeleteKeyboards( infoNode );
		bAttempted = TRUE;
	}
	else
	{
		int nKbNo = m_pHelpers -> m_Keyboard.GetKbNoByNodeType( nAction );

		if ( 0 != nKbNo )
		{
			infoNode.m_nKbNo = nKbNo;
			nResult = m_pHelpers -> m_Keyboard.DeleteKeyboards( infoNode );
			bAttempted = TRUE;
		}
	}

	if ( TRUE == bAttempted )
	{
		SelectLocationKeyboard();
		m_pHelpers -> m_Keyboard.ShowDeletionResult( nResult );
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button5KeyboardPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetKeyboardAction( LocSelEntity, nAction );

	CKeyboardMapInfo infoMap;
	m_pHelpers -> m_Keyboard.GetKeyboardMapInfo( LocSelEntity.GetEntityIdx(), infoMap );

	CDataNodeInfoKeyboard infoNode;
	m_pHelpers -> m_Keyboard.ConvertMapInfoToNodeInfo( infoMap, infoNode );

	bool bAttempt = FALSE;

	if ( 9999 == nAction )
	{
		infoNode.m_nKbNo = 0;
		bAttempt = TRUE;
	}
	else
	{
		int nKbNo = m_pHelpers -> m_Keyboard.GetKbNoByNodeType( nAction );

		if ( 0 != nKbNo )
		{
			infoNode.m_nKbNo = nKbNo;
			bAttempt = TRUE;
		}
	}

	if ( TRUE == bAttempt )
		m_pHelpers -> m_Keyboard.DownloadKeyboard( infoNode );
}

/**********************************************************************/

void CPropPageSystemDataList::Button6KeyboardPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetKeyboardAction( LocSelEntity, nAction );

	int nKbNo = m_pHelpers -> m_Keyboard.GetKbNoByNodeType( nAction );

	if ( 0 != nKbNo )
	{
		CKeyboardMapInfo infoMap;
		m_pHelpers -> m_Keyboard.GetKeyboardMapInfo( LocSelEntity.GetEntityIdx(), infoMap );

		CDataNodeInfoKeyboard infoNode;
		m_pHelpers -> m_Keyboard.ConvertMapInfoToNodeInfo( infoMap, infoNode );
		infoNode.m_nKbNo = nKbNo;

		if ( m_pHelpers -> m_Keyboard.EditNames( infoNode ) == TRUE )
			SelectLocationKeyboard();
	}
}

/**********************************************************************/
