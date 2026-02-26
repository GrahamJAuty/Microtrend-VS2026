/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManagerNonDb.h"
#include "LocationCSVArray.h"
/**********************************************************************/
#include "ServerSelector.h"
/**********************************************************************/

CServerSelector::CServerSelector( CEdit& edit, CSSComboBox& combo ) : m_EditBox ( edit ), m_ComboBox ( combo )
{
	m_nServerNo = 0;
}

/**********************************************************************/

void CServerSelector::FillServerCombo( int nServerNo, int nStartPos, int nEndPos )
{
	m_ComboBox.ResetContent();

	for ( int n = nStartPos; n < nEndPos; n++ )
	{
		CServerSelectionInfo infoServer;
		DataManagerNonDb.ServerNameTable.GetServerStatus( n, infoServer );

		int nInsertionPoint = m_ComboBox.AddString( infoServer.m_strServerName );
		m_ComboBox.SetItemData ( nInsertionPoint, ( WORD ) infoServer.m_nServerNo );
	}

	m_ComboBox.InsertString( 0, "None" );
	m_ComboBox.SetItemData( 0, 0 );
		
	bool bFound = FALSE;
	for ( int nIndex = 0; nIndex < m_ComboBox.GetCount(); nIndex++ )
	{
		if ( m_ComboBox.GetItemData ( nIndex ) == (WORD) nServerNo )
		{
			m_ComboBox.SetCurSel ( nIndex );
			m_nServerNo = nServerNo;
			bFound = TRUE;
			break;
		}
	}

	if ( bFound == FALSE ) 
	{
		CString strServer;
		strServer.Format( "Server %d", nServerNo );
		m_ComboBox.InsertString ( 1, strServer );
		m_ComboBox.SetItemData ( 1, nServerNo  );
		m_ComboBox.SetCurSel(1);
		m_nServerNo = nServerNo;
	}

	UpdateServerEditBox();
}

/**********************************************************************/

void CServerSelector::FillServerComboSystem( int nServerNo )
{
	FillServerCombo( nServerNo, 0, DataManagerNonDb.ServerNameTable.GetSize() );
}

/**********************************************************************/

void CServerSelector::FillServerComboDatabase( int nDbIdx, int nServerNo )
{
	CServerSelectionInfo infoServer;
	infoServer.m_nDbNo = dbDatabase.GetDbNo( nDbIdx );	
	infoServer.m_nLocNo = 0;
	infoServer.m_nNwkNo = 0;
	infoServer.m_nServerNo = 0;

	int nStartPos;
	DataManagerNonDb.ServerNameTable.FindServer( infoServer, nStartPos );

	int nEndPos;
	infoServer.m_nDbNo++;
	DataManagerNonDb.ServerNameTable.FindServer( infoServer, nEndPos );

	FillServerCombo( nServerNo, nStartPos, nEndPos );
}

/**********************************************************************/

void CServerSelector::FillServerComboLocation( int nLocIdx, int nServerNo )
{
	CServerSelectionInfo infoServer;
	infoServer.m_nDbNo = dbLocation.GetDbNo( nLocIdx );	
	infoServer.m_nLocNo = dbLocation.GetLocNo( nLocIdx );
	infoServer.m_nNwkNo = dbLocation.GetNetworkNo( nLocIdx );
	infoServer.m_nServerNo = 0;

	int nStartPos;
	DataManagerNonDb.ServerNameTable.FindServer( infoServer, nStartPos );

	int nEndPos;
	infoServer.m_nLocNo++;
	DataManagerNonDb.ServerNameTable.FindServer( infoServer, nEndPos );

	FillServerCombo( nServerNo, nStartPos, nEndPos );
}

/**********************************************************************/

bool CServerSelector::UpdateServerCombo( int nNewServerNo )
{
	for ( int nIndex = 0; nIndex <= m_ComboBox.GetCount(); nIndex++ )
	{
		if ( m_ComboBox.GetItemData ( nIndex ) == ( WORD ) nNewServerNo )
		{
			m_ComboBox.SetCurSel ( nIndex );
			m_nServerNo = nNewServerNo;
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/

void CServerSelector::UpdateServerEditBox()
{
	CString strServerNo = "";
	if ( ( m_nServerNo >= 0 ) && ( m_nServerNo <= Server::ServerNo.Max ) )
		strServerNo.Format ( "%d", m_nServerNo );

	m_EditBox.SetWindowText ( strServerNo );
}

/**********************************************************************/

void CServerSelector::SelectServerFromCombo() 
{
	int nIndex = m_ComboBox.GetCurSel();
	
	if ( nIndex != CB_ERR )
	{
		int nNewServerNo = m_ComboBox.GetItemData ( nIndex );

		if ( nNewServerNo != m_nServerNo )
		{
			m_nServerNo = nNewServerNo;
			UpdateServerEditBox();
		}
	}
}

/**********************************************************************/

void CServerSelector::SelectServerFromEditBox( bool bUseCombo ) 
{
	CString strServerNo;
	m_EditBox.GetWindowText( strServerNo );
	::TrimSpaces ( strServerNo, FALSE );

	if ( strServerNo == "" )
	{
		UpdateServerEditBox();
	}
	else
	{
		int nNewServerNo = atoi ( strServerNo );
		
		if ( nNewServerNo != m_nServerNo )
		{
			if ( TRUE == bUseCombo )
			{
				if ( UpdateServerCombo( nNewServerNo ) == FALSE )
				{
					CString strServer;
					strServer.Format( "Server %d", nNewServerNo );
					m_ComboBox.InsertString ( 1, strServer );
					m_ComboBox.SetItemData ( 1, nNewServerNo  );
					m_ComboBox.SetCurSel(1);
					m_nServerNo = nNewServerNo;
				}
			}
			else
			{
				if ( ( nNewServerNo >= 0 ) && ( nNewServerNo <= Server::ServerNo.Max ) )
					m_nServerNo = nNewServerNo;
			}

			UpdateServerEditBox();
		}
	}
}

/**********************************************************************/
