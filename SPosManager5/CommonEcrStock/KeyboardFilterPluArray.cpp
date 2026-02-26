/**********************************************************************/
 
/**********************************************************************/
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "KeyboardFilterPluArray.h"
/**********************************************************************/

CKeyboardFilterPluArray::CKeyboardFilterPluArray(void)
{
	m_CurrentLoc.m_nNodeType = NODE_SYSTEM;
	m_CurrentLoc.m_nEntityIdx = 0;
	m_CurrentLoc.m_strKeyboards = "";
	m_nCurrentKbNo = 1;
}

/**********************************************************************/

void CKeyboardFilterPluArray::SelectKeyboard( CKeyboardFilterSelectorLoc& Loc, int nKbNo )
{
	if ( ( Loc.m_nNodeType != m_CurrentLoc.m_nNodeType ) || ( Loc.m_nEntityIdx != m_CurrentLoc.m_nEntityIdx ) || ( nKbNo != m_nCurrentKbNo ) )
	{
		m_CurrentLoc.m_nNodeType = Loc.m_nNodeType;
		m_CurrentLoc.m_nEntityIdx = Loc.m_nEntityIdx;
		m_nCurrentKbNo = nKbNo;
		
		m_arrayPluNo.RemoveAll();

		if ( Loc.m_nNodeType != NODE_SYSTEM )
		{
			CString strPath = "";
			if ( Loc.m_nNodeType == NODE_LOCATION )
				strPath += dbLocation.GetFolderPathDataKeyboard( Loc.m_nEntityIdx, nKbNo );
			else
				strPath += dbKeyboardSet.GetFolderPathKeyboard( Loc.m_nEntityIdx, nKbNo );

			strPath += "\\PluNum.dat";
		
			CSSFile fileNum;
			fileNum.Open( strPath, "rb" );
			
			__int64 nLastPluNo = 0; 
			
			CString strBuffer;
			while ( fileNum.ReadString( strBuffer ) == TRUE )
			{
				__int64 nPluNo = _atoi64( strBuffer );
				
				if ( nPluNo > nLastPluNo )
				{
					CSortedInt64Item item;
					item.m_nItem = nPluNo;
					m_arrayPluNo.DirectAdd( item );
					nLastPluNo = nPluNo;
				}
				else
					break;
			}
		}
	}
}

/**********************************************************************/

void CKeyboardFilterPluArray::GetSelection( CKeyboardFilterSelectorLoc& Loc, int& nKbNo )
{ 
	Loc.m_nNodeType = m_CurrentLoc.m_nNodeType;	
	Loc.m_nEntityIdx = m_CurrentLoc.m_nEntityIdx ;
	nKbNo = m_nCurrentKbNo;		
}

/**********************************************************************/

bool CKeyboardFilterPluArray::CheckPluNo( __int64 nPluNo )
{
	CSortedInt64Item item;

	bool bResult = FALSE;
	if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == FALSE )
	{
		int nPos;
		item.m_nItem = nPluNo;
		bResult = m_arrayPluNo.Find( item, nPos );
	}
	else
	{
		int nPos1;
		item.m_nItem = nPluNo * 10;
		if ( m_arrayPluNo.Find( item, nPos1 ) == TRUE )
			bResult = TRUE;
		else
		{
			int nPos2;
			item.m_nItem += 10;
			m_arrayPluNo.Find( item, nPos2 );		
			bResult = ( nPos2 != nPos1 );
		}
	}

	return bResult;
}

/**********************************************************************/
