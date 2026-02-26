/**********************************************************************/
#include "GroupCSVArrayEpos.h"
/**********************************************************************/

CGroupCSVArrayEpos::CGroupCSVArrayEpos() : CGroupCSVArrayBase()
{
	m_bCheckedSportsBookerGroupNo = FALSE;
	m_nSportsBookerGroupNo = 0;
}

/**********************************************************************/

int CGroupCSVArrayEpos::Open( const char* szFilename, int nMode )
{
	m_bCheckedSportsBookerGroupNo = FALSE;
	m_nSportsBookerGroupNo = 0;

	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	if ( GetSize() != 0 )
	{
		CGroupCSVRecordEpos Group;
		GetAt( 0, Group );
			
		if (Group.GetGroupNo() == 0)
		{
			RemoveAt(0);
		}
	}

	CGroupCSVRecordEpos Group;
	Group.SetEposText( "PROMO" );
	Group.SetRepText( DealerFlags.GetGroupZeroText() );
	Group.SetGroupNo(0);
	Group.SetSageNominalCode(0);
	InsertAt( 0, Group );
	
	return nReply;
}

/**********************************************************************/

void CGroupCSVArrayEpos::GetGroupInfo( int nIndex, CBaseGroupInfo& info )
{
	int nResult = 0;
	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		CGroupCSVRecordEpos Group;
		GetAt( nIndex, Group );
		info.m_nGroupNo = Group.GetGroupNo();
		info.m_strReportText = Group.GetReportText();
		info.m_strDefText = Group.GetDefaultText();
	}
}

/**********************************************************************/

void CGroupCSVArrayEpos::AddPMSGroup( int nGroupNo, const char* szText )
{
	CGroupCSVRecordEpos GroupRecord;
	GroupRecord.SetGroupNo(nGroupNo);
	GroupRecord.SetEposText( szText );
			
	int nGroupIdx;
	if( FindGroupByNumber( nGroupNo, nGroupIdx ) == FALSE )
		InsertAt( nGroupIdx, GroupRecord );
	else
		SetAt( nGroupIdx, GroupRecord );
}

/**********************************************************************/

bool CGroupCSVArrayEpos::GetListDataLine( int nIndex, CString& strLine )
{
	bool bResult = FALSE;
	m_strListDataLine = "";

	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		CGroupCSVRecordEpos GroupRecord;
		GetAt( nIndex, GroupRecord );

		strLine.Format ( "%d\t%s\t%s",
			GroupRecord.GetGroupNo(),
			GroupRecord.GetEposText(),
			GroupRecord.GetRepText() );

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CGroupCSVArrayEpos::RememberSettings()
{
	m_arraySaved.RemoveAll();
	for ( int n = 0; n < GetSize(); n++ )
		m_arraySaved.Add( GetAt(n) );
}

/**********************************************************************/

void CGroupCSVArrayEpos::RestoreSettings()
{
	RemoveAt( 0, GetSize() );
	for ( int n = 0; n < m_arraySaved.GetSize(); n++ )
	{
		CCSV csv( m_arraySaved.GetAt(n) );
		
		CGroupCSVRecordEpos Group;
		Group.ConvertFromCSV( csv );

		Add( Group );
	}
	m_arraySaved.RemoveAll();
}

/**********************************************************************/

int CGroupCSVArrayEpos::GetSportsBookerGroupNo()
{
	if ( FALSE == m_bCheckedSportsBookerGroupNo )
	{
		m_nSportsBookerGroupNo = 0;
		for ( int nGroupIdx = 0; nGroupIdx < GetSize(); nGroupIdx++ )
		{
			CGroupCSVRecordEpos GroupRecord;
			GetAt( nGroupIdx, GroupRecord );

			if ( GroupRecord.GetSportsBookerFlag() == TRUE )
			{
				m_nSportsBookerGroupNo = GroupRecord.GetGroupNo();
				break;
			}
		}
	}

	m_bCheckedSportsBookerGroupNo = TRUE;
	return m_nSportsBookerGroupNo;
}

/**********************************************************************/

void CGroupCSVArrayEpos::FixSportsBookerGroupNo()
{
	if ( TRUE == m_bCheckedSportsBookerGroupNo )
	{
		for ( int nGroupIdx = 0; nGroupIdx < GetSize(); nGroupIdx++ )
		{
			CGroupCSVRecordEpos GroupRecord;
			GetAt( nGroupIdx, GroupRecord );
			GroupRecord.SetSportsBookerFlag( GroupRecord.GetGroupNo() == m_nSportsBookerGroupNo );
			SetAt( nGroupIdx, GroupRecord );
		}
	}
}

/**********************************************************************/

void CGroupCSVArrayEpos::SetSportsBookerGroupNo( int nGroupNo )
{
	if ( ( nGroupNo >= EposGroup::GroupNo.Min ) &&( nGroupNo <= EposGroup::GroupNo.Max ) )
	{
		m_bCheckedSportsBookerGroupNo = TRUE;
		m_nSportsBookerGroupNo = nGroupNo;
	}
}

/**********************************************************************/
