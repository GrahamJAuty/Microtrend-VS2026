/**********************************************************************/
#include "SPOSVersions.h"
/**********************************************************************/
#include "CascadingMacroCSVArray.h"
/**********************************************************************/

CCascadingMacroCSVRecord::CCascadingMacroCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CCascadingMacroCSVRecord::ClearRecord()
{
	m_nMacroNo = 1;
	m_nParentMacroTypeInternal = 0;
	m_strParentMacroName = "";
	m_strParentMacroData = "";
	m_bLocalMacro = TRUE;
	m_strLocalMacroName = "";
	m_strLocalMacroData = "";
}

/**********************************************************************/

void CCascadingMacroCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetMacroNo( csv.GetInt(0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV ( csv );		break;
	}
}

/**********************************************************************/

void CCascadingMacroCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetLocalMacroName( csv.GetString(2) );
	SetLocalMacroData( csv.GetString(3) );
	SetLocalMacroFlag( csv.GetBool(4) );
	SetParentMacroTypeInternal( csv.GetInt(5) );
	SetParentMacroName( csv.GetString(6) );
	SetParentMacroData( csv.GetString(7) );
}

/**********************************************************************/

void CCascadingMacroCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nMacroNo );
	csv.Add ( CASCADINGMACRO_VERSION );
	csv.Add ( m_strLocalMacroName );
	csv.Add ( m_strLocalMacroData );
	csv.Add ( m_bLocalMacro );
	csv.Add ( m_nParentMacroTypeInternal );
	csv.Add ( m_strParentMacroName );
	csv.Add ( m_strParentMacroData );
}

/**********************************************************************/

int CCascadingMacroCSVRecord::GetParentMacroTypeExternal()
{
	switch( m_nParentMacroTypeInternal )
	{
	case 1:		return NODE_SYSTEM;
	case 2:		return NODE_DATABASE;
	case 3:		return NODE_LOCATION;
	case 4:		return NODE_DBKEYBOARDSET;
	case 0:
	default:	return NODE_PLACEHOLDER;
	}	
}

/**********************************************************************/

void CCascadingMacroCSVRecord::SetMacroNo( int n )
{
	Set ( m_nMacroNo, n, CascadingMacro::MacroNo );
}

/**********************************************************************/

void CCascadingMacroCSVRecord::SetLocalMacroName( const char* sz )
{
	Set ( m_strLocalMacroName, sz, CascadingMacro::MacroName );
}

/**********************************************************************/

void CCascadingMacroCSVRecord::SetLocalMacroData( const char* sz )
{
	Set ( m_strLocalMacroData, sz, CascadingMacro::MacroData );
}

/**********************************************************************/

void CCascadingMacroCSVRecord::SetParentMacroTypeInternal( int n )
{
	Set ( m_nParentMacroTypeInternal, n, CascadingMacro::ParentMacroTypeInternal );
}

/**********************************************************************/

void CCascadingMacroCSVRecord::SetParentMacroTypeExternal( int n )
{
	int nInternal = 0;

	switch( n )
	{
	case NODE_SYSTEM:			nInternal = 1;	break;
	case NODE_DATABASE:			nInternal = 2;	break;
	case NODE_LOCATION:			nInternal = 3;	break;
	case NODE_DBKEYBOARDSET:	nInternal = 4;	break;
	case NODE_PLACEHOLDER:
	default:					nInternal = 0;	break;
	}

	m_nParentMacroTypeInternal = nInternal;
}

/**********************************************************************/

void CCascadingMacroCSVRecord::SetParentMacroName( const char* sz )
{
	Set ( m_strParentMacroName, sz, CascadingMacro::MacroName );
}

/**********************************************************************/

void CCascadingMacroCSVRecord::SetParentMacroData( const char* sz )
{
	Set ( m_strParentMacroData, sz, CascadingMacro::MacroData );
}

/**********************************************************************/

void CCascadingMacroCSVRecord::CheckMinimumSPOSVersion( CMinimumSPOSVersion& version )
{
	version.m_nMinVer = SPOS_V4;
	version.m_strReason = "";
	version.m_bInvalidData = FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCascadingMacroCSVArray::CCascadingMacroCSVArray() 
{
}

/**********************************************************************/

CCascadingMacroCSVArray::~CCascadingMacroCSVArray() 
{
}

/**********************************************************************/

void CCascadingMacroCSVArray::Reset()
{
	RemoveAt( 0, GetSize() );
}

/**********************************************************************/

int CCascadingMacroCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}
	
	int nLastMacroNo = 0;
	int nValidRecords = 0;
	
	CCascadingMacroCSVRecord Macro;
	for ( ; nValidRecords < GetSize(); nValidRecords++ )
	{
		int nMacroNo = GetMacroNo( nValidRecords );
		if ( ( nMacroNo <= nLastMacroNo ) || ( nMacroNo > CascadingMacro::MacroNo.Max ) )
			break;

		nLastMacroNo = nMacroNo;
	}

	while ( GetSize() > nValidRecords )
		RemoveAt( GetSize() - 1 );

	return nReply;
}

/**********************************************************************/

bool CCascadingMacroCSVArray::FindMacroByNumber( int nMacroNo, int& nMacroIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nMacroIdx = ( nStart + nEnd ) / 2;
		
		int nArrayNo = GetMacroNo(nMacroIdx);
			
		if ( nMacroNo == nArrayNo )
			return TRUE;

		if ( nMacroNo < nArrayNo )
			nEnd = nMacroIdx - 1;
		else
			nStart = nMacroIdx + 1;
	}

	nMacroIdx = nStart;
	return FALSE;
}

/**********************************************************************/

int CCascadingMacroCSVArray::FindFirstFreeNumber()
{
	int nResult = 0;

	for ( int n = 0; n < GetSize(); n++ )
	{
		if ( GetMacroNo(n) > n + 1 )
		{
			nResult = n + 1;
			break;
		}
	}

	if ( nResult == 0 )
		nResult = GetSize() + 1;

	if ( nResult < CascadingMacro::MacroNo.Min || nResult > CascadingMacro::MacroNo.Max )
		nResult = 0;

	return nResult;
}

/**********************************************************************/

int CCascadingMacroCSVArray::GetMacroNo( int nMacroIdx )
{
	int nMacroNo = 0;

	CString strTemp;
	if ( ( strTemp = GetAt ( nMacroIdx ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		nMacroNo = csv.GetInt(0);
	}

	return nMacroNo;
}

/**********************************************************************/

void CCascadingMacroCSVArray::LoadParentMacrosSystem()
{
	CLocationSelectorEntity ParentEntity;
	ParentEntity.SetSystemNode();
	LoadParentMacros( ParentEntity );
}

/**********************************************************************/

void CCascadingMacroCSVArray::LoadParentMacrosDatabase( int nDbIdx )
{
	CLocationSelectorEntity ParentEntity;
	ParentEntity.SetDatabaseNode( nDbIdx );
	LoadParentMacros( ParentEntity );
}

/**********************************************************************/

void CCascadingMacroCSVArray::LoadParentMacrosLocation( int nLocIdx )
{
	CLocationSelectorEntity ParentEntity;
	ParentEntity.SetLocationNode( nLocIdx );
	LoadParentMacros( ParentEntity );
}

/**********************************************************************/

void CCascadingMacroCSVArray::LoadParentMacrosKeyboard()
{
	CLocationSelectorEntity ParentEntity;
	ParentEntity.SetKeyboardMapNode(0);
	LoadParentMacros( ParentEntity );
}

/**********************************************************************/

void CCascadingMacroCSVArray::LoadParentMacros( CLocationSelectorEntity& LocSelEntity )
{
	CCascadingMacroCSVArray* pParentArray = NULL;
	int nParentType = LocSelEntity.GetConType();
	
	CDataManagerInfo infoDM;

	switch( nParentType )
	{
	case NODE_SYSTEM:
		DataManagerNonDb.OpenCCMacroSystem( DB_READONLY, infoDM );
		pParentArray = &DataManagerNonDb.CCMacroSystem;
		break;

	case NODE_DATABASE:
		DataManagerNonDb.OpenCCMacroDatabase( LocSelEntity.GetDbIdx(), DB_READONLY, infoDM );
		pParentArray = &DataManagerNonDb.CCMacroDatabase;
		break;

	case NODE_LOCATION:
		DataManagerNonDb.OpenCCMacroLocation( LocSelEntity.GetEntityIdx(), DB_READONLY, infoDM );
		pParentArray = &DataManagerNonDb.CCMacroLocation;
		break;

	case NODE_DBKEYBOARDSET:
		DataManagerNonDb.OpenCCMacroKeyboard( DB_READONLY, infoDM );
		pParentArray = &DataManagerNonDb.CCMacroKeyboard;
		break;
	}

	if ( NULL == pParentArray )
		return;

	for ( int n = 0; n < pParentArray -> GetSize(); n++ )
	{
		CCascadingMacroCSVRecord ParentMacro;
		pParentArray -> GetAt( n, ParentMacro );

		if ( ParentMacro.GetLocalMacroFlag() == TRUE )
		{
			CCascadingMacroCSVRecord LocalMacro;
			bool bNewLocalMacro = FALSE;
			bool bUseParentMacro = FALSE;

			int nLocalMacroIdx;
			if ( FindMacroByNumber( ParentMacro.GetMacroNo(), nLocalMacroIdx ) == FALSE )
			{
				bNewLocalMacro = TRUE;
				bUseParentMacro = TRUE;
				LocalMacro.SetMacroNo( ParentMacro.GetMacroNo() );
				LocalMacro.SetLocalMacroFlag( FALSE );
				LocalMacro.SetLocalMacroName( "" );
				LocalMacro.SetLocalMacroData( "" );
			}
			else
			{
				bNewLocalMacro = FALSE;
				GetAt( nLocalMacroIdx, LocalMacro );
				bUseParentMacro = ( LocalMacro.GetParentMacroTypeExternal() == NODE_PLACEHOLDER );
			}

			if ( TRUE == bUseParentMacro )
			{
				LocalMacro.SetParentMacroTypeExternal( nParentType );
				LocalMacro.SetParentMacroName( ParentMacro.GetLocalMacroName() );
				LocalMacro.SetParentMacroData( ParentMacro.GetLocalMacroData() );
				
				if ( FALSE == bNewLocalMacro )
				{
					SetAt( nLocalMacroIdx, LocalMacro );
				}
				else
				{
					InsertAt( nLocalMacroIdx, LocalMacro );				  
				}
			}
		}
	}

	switch( nParentType )
	{
	case NODE_SYSTEM:
		DataManagerNonDb.CloseCCMacroSystem( infoDM );
		break;

	case NODE_DATABASE:
		DataManagerNonDb.CloseCCMacroDatabase( infoDM );
		break;

	case NODE_LOCATION:
		DataManagerNonDb.CloseCCMacroLocation( infoDM );
		break;

	case NODE_DBKEYBOARDSET:
		DataManagerNonDb.CloseCCMacroKeyboard( infoDM );
		break;
	}
}

/**********************************************************************/

void CCascadingMacroCSVArray::PrepareForWrite()
{
	for ( int nMacroIdx = GetSize() - 1; nMacroIdx >= 0; nMacroIdx-- )
	{
		CCascadingMacroCSVRecord Macro;
		GetAt( nMacroIdx, Macro );

		if ( Macro.GetLocalMacroFlag() == FALSE )
		{
			RemoveAt( nMacroIdx );
		}
		else
		{
			Macro.SetParentMacroTypeExternal( NODE_PLACEHOLDER );
			Macro.SetParentMacroName( "" );
			Macro.SetParentMacroData( "" );
			SetAt( nMacroIdx, Macro );
		}
	}
}

/**********************************************************************/

const char* CCascadingMacroCSVArray::GetMacroNameFromNumber( int nMacroNo )
{
	int nMacroIdx;
	if ( FindMacroByNumber( nMacroNo, nMacroIdx ) == TRUE )
	{
		CCascadingMacroCSVRecord Macro;
		GetAt( nMacroIdx, Macro );
		m_strMacroName = Macro.GetParentMacroName();
	}
	else
	{
		m_strMacroName = "Unknown";
	}

	return m_strMacroName;
}

/**********************************************************************/
