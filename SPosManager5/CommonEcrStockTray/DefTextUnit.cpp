/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "DefTextUnit.h"
/**********************************************************************/

CDefTextUnit::CDefTextUnit( int nType ) : CSharedStringArray() 
{
	m_nType = nType;
}

/**********************************************************************/

int CDefTextUnit::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedStringArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;

		if ( GetSize() == 0 )
			SetDefaults();
	}

	return nReply;
}

/**********************************************************************/

const char* CDefTextUnit::GetText ( int nIndex )
{
	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		m_strText = GetAt ( nIndex );
		return m_strText;
	}
	return "Unknown";
}

/**********************************************************************/

void CDefTextUnit::SetText ( int nIndex, const char* szText )
{
	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CString strText = szText;
		::TrimSpaces ( strText, FALSE );
		
		if ( strText == "" )
			strText = "Box";

		SetAt ( nIndex, strText );
	}
}

/**********************************************************************/

void CDefTextUnit::SetDefaults()
{
	RemoveAt( 0, GetSize() );

	switch ( m_nType )
	{
	case NODE_STOCKUNIT:
		Add ( "Bottle" );
		Add ( "Can" );
		Add ( "Box" );
		Add ( "Tin" );
		break;

	case NODE_SUBUNIT:
		Add ( "Glass" );
		Add ( "Measure" );
		break;

	case NODE_ADJUSTTEXT:
	case NODE_CUSTOM_FIELD_SET:
		break;

	case NODE_PMSCLOSURE:
		Add( "Cleaning" );
		Add( "Maintenance" );
		break;

	case NODE_PLUUNIT:
	default:
		Add ( "per 10g" );
		Add ( "per 100g" );
		Add ( "per 10ml" );
		Add ( "per 100ml" );
		Add ( "per 750ml" );
		Add ( "per 50kg" );
		Add ( "per 1000kg" );
		Add ( "per can" );
		Add ( "per bottle" );
		Add ( "per item" );
		break;
	}
}

/**********************************************************************/

void CDefTextUnit::AddUnit ( const char* szText )
{
	Add ( szText );
}

/**********************************************************************/

void CDefTextUnit::RemoveUnit ( int nIndex )
{
	if ( nIndex >= 0 && nIndex < GetSize() )
		RemoveAt ( nIndex );
}

/**********************************************************************/

void CDefTextUnit::CopyFrom( CDefTextUnit& Source )
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < Source.GetSize(); n++ )
		Add( Source.GetAt(n) );
}

/**********************************************************************/

void CDefTextUnit::SwapWithNext( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() - 1 ) )
	{
		CString str = GetAt( nIdx );
		SetAt( nIdx, GetAt( nIdx + 1 ) );
		SetAt( nIdx + 1, str );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CDefTextSystemCustomFieldSet::CDefTextSystemCustomFieldSet() : CDefTextUnit( NODE_CUSTOM_FIELD_SET )
{
}

/**********************************************************************/

