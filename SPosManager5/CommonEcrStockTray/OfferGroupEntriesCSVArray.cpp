/**********************************************************************/
#include "..\CommonEcrStock\CommsExportBase.h"
#include "DepartmentCSVArray.h"
#include "SPOSVersions.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "OfferGroupEntriesCSVArray.h"
/**********************************************************************/

COfferGroupEntriesCSVRecord::COfferGroupEntriesCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void COfferGroupEntriesCSVRecord::ClearRecord()
{
	m_nGroupNo = 1;
	m_nEntryType = 1;
	m_nEntryPluNo = 1;
	m_nEntryDeptNo = 1;
	m_nEntryGroupNo = 1;
	m_nModifierFlags = 1;
}

/**********************************************************************/

void COfferGroupEntriesCSVRecord::MakeDummy( int nGroup )
{
	m_nGroupNo = nGroup;
	m_nEntryType = 0;
	m_nEntryPluNo = 0;
	m_nEntryDeptNo = 0;
	m_nEntryGroupNo = 0;
	m_nModifierFlags = 1;
}

/**********************************************************************/

void COfferGroupEntriesCSVRecord::MakeDummy( int nGroup, int nType )
{
	m_nGroupNo = nGroup;
	m_nEntryType = nType;
	m_nEntryPluNo = 0;
	m_nEntryDeptNo = 0;
	m_nEntryGroupNo = 0;
	m_nModifierFlags = 1;
}

/**********************************************************************/

void COfferGroupEntriesCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord(); 

	switch ( csv.GetInt(0) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	case 2:	V2ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void COfferGroupEntriesCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetGroupNo( csv.GetInt(1) );
	
	int nEntryType = csv.GetInt(2);
	switch( nEntryType )
	{
	case OFFERENTRY_PLU:	
	case OFFERENTRY_PLU_EXCLUDE:	
		SetEntryPluNo( nEntryType, csv.GetInt64(3) );	
		break;

	case OFFERENTRY_DEPT:
	case OFFERENTRY_DEPT_SAMEPLU:
		SetEntryDeptNo( nEntryType, csv.GetInt(3) );	
		break;

	case OFFERENTRY_GROUP:	
		SetEntryGroupNo( csv.GetInt(3) );	
		break;
	}
	
	SetModifierFlags( csv.GetInt(4) );
}

/**********************************************************************/

void COfferGroupEntriesCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	SetGroupNo( csv.GetInt(1) );
	
	int nEntryType = csv.GetInt(2);
	switch( nEntryType )
	{
	case OFFERENTRY_PLU:
	case OFFERENTRY_PLU_EXCLUDE:
		SetEntryPluNo( nEntryType, csv.GetInt64(3) );	
		break;

	case OFFERENTRY_DEPT:
	case OFFERENTRY_DEPT_SAMEPLU:
		SetEntryDeptNo( nEntryType, csv.GetInt(4) );	
		break;

	case OFFERENTRY_GROUP:	
		SetEntryGroupNo( csv.GetInt(5) );	
		break;
	}
	
	SetModifierFlags( csv.GetInt(6) );
}

/**********************************************************************/

void COfferGroupEntriesCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( OFFERGROUPENTRIES_VERSION );
	csv.Add ( m_nGroupNo );
	csv.Add ( m_nEntryType );
	csv.Add ( m_nEntryPluNo );
	csv.Add ( m_nEntryDeptNo );
	csv.Add ( m_nEntryGroupNo );
	csv.Add ( m_nModifierFlags );
}

/**********************************************************************/

void COfferGroupEntriesCSVRecord::SetGroupNo( int n )
{
	Set ( m_nGroupNo, n, OfferGroupEntries::GroupNo );
}

/**********************************************************************/

void COfferGroupEntriesCSVRecord::SetModifierFlags( int n )
{
	if ( SysInfo.GetMaxBasePluLen() != 0 )
		Set ( m_nModifierFlags, n, OfferGroupEntries::ModifierFlags );
}

/**********************************************************************/

void COfferGroupEntriesCSVRecord::SetSingleModifierFlag( int n )
{
	if ( SysInfo.GetMaxBasePluLen() != 0 )
	{
		switch( n )
		{
		case 0:	m_nModifierFlags |= 1;	break;
		case 1:	m_nModifierFlags |= 2;	break;
		case 2:	m_nModifierFlags |= 4;	break;
		case 3:	m_nModifierFlags |= 8;	break;
		case 4:	m_nModifierFlags |= 16;	break;
		case 5:	m_nModifierFlags |= 32;	break;
		case 6:	m_nModifierFlags |= 64;	break;
		case 7:	m_nModifierFlags |= 128;	break;
		case 8:	m_nModifierFlags |= 256;	break;
		case 9:	m_nModifierFlags |= 512;	break;
		}
	}
}

/**********************************************************************/

bool COfferGroupEntriesCSVRecord::GetSingleModifierFlag( int n )
{
	if ( SysInfo.GetMaxBasePluLen() != 0 )
	{
		switch( n )
		{
		case 0:	return ( ( m_nModifierFlags & 1 ) != 0 );
		case 1:	return ( ( m_nModifierFlags & 2 ) != 0 );
		case 2:	return ( ( m_nModifierFlags & 4 ) != 0 );
		case 3:	return ( ( m_nModifierFlags & 8 ) != 0 );
		case 4:	return ( ( m_nModifierFlags & 16 ) != 0 );
		case 5:	return ( ( m_nModifierFlags & 32 ) != 0 );
		case 6:	return ( ( m_nModifierFlags & 64 ) != 0 );
		case 7:	return ( ( m_nModifierFlags & 128 ) != 0 );
		case 8:	return ( ( m_nModifierFlags & 256 ) != 0 );
		case 9:	return ( ( m_nModifierFlags & 512 ) != 0 );
		default:	return FALSE;
		}
	}
	else
	{
		switch( n )
		{
		case 0:		return TRUE;
		default:	return FALSE;
		}
	}	
}

/**********************************************************************/

void COfferGroupEntriesCSVRecord::SetEntryPluNo( int nEntryType, __int64 nPluNo )
{
	switch ( nEntryType )
	{
	case OFFERENTRY_PLU:
	case OFFERENTRY_PLU_EXCLUDE:
		m_nEntryType = nEntryType;
		Set( m_nEntryPluNo, nPluNo, OfferGroupEntries::EntryPluNo );
		break;
	}
}

/**********************************************************************/

void COfferGroupEntriesCSVRecord::SetEntryDeptNo( int nEntryType, int nDeptNo )
{
	switch ( nEntryType )
	{
	case OFFERENTRY_DEPT:
	case OFFERENTRY_DEPT_SAMEPLU:
		m_nEntryType = nEntryType;
		Set( m_nEntryDeptNo, nDeptNo, OfferGroupEntries::EntryDeptNo );
		break;
	}
}

/**********************************************************************/

void COfferGroupEntriesCSVRecord::SetEntryGroupNo( int nGroupNo )
{
	m_nEntryType = OFFERENTRY_GROUP;
	Set( m_nEntryGroupNo, nGroupNo, OfferGroupEntries::EntryGroupNo );
}

/**********************************************************************/

const char* COfferGroupEntriesCSVRecord::GetEntryLookupKey()
{
	m_strKey = "";

	switch( m_nEntryType )
	{
	case OFFERENTRY_PLU:
	case OFFERENTRY_PLU_EXCLUDE:
		m_strKey.Format( "%3.3d%3.3d%14.14I64d",
			m_nGroupNo, 
			m_nEntryType, 
			m_nEntryPluNo );
		break;

	case OFFERENTRY_DEPT:
	case OFFERENTRY_DEPT_SAMEPLU:
		m_strKey.Format( "%3.3d%3.3d%3.3d",
			m_nGroupNo, 
			m_nEntryType, 
			m_nEntryDeptNo );
		break;

	case OFFERENTRY_GROUP:
		m_strKey.Format( "%3.3d%3.3d%3.3d",
			m_nGroupNo, 
			m_nEntryType, 
			m_nEntryGroupNo );
		break;

	default:
		m_strKey.Format( "%3.3d%3.3d000",
			m_nGroupNo, 
			m_nEntryType );
		break;
	}
	
	return m_strKey;
}

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

void COfferGroupEntriesCSVRecord::CheckMinimumSPOSVersion( CMinimumSPOSVersion& version, __int64& nSharpPluNo, bool& bModifiable )
{
	version.m_nMinVer = SPOS_V4;
	version.m_strReason = "";
	version.m_bInvalidData = FALSE;
	
	nSharpPluNo = 0;
	bModifiable = FALSE;
	
	int nEntryType = GetEntryType();

	switch( nEntryType )
	{
	case OFFERENTRY_PLU:
	case OFFERENTRY_PLU_EXCLUDE:
		{
			__int64 nSSPluNo = GetEntryPluNo();
			CCommsExportBase::CheckPluNo( SPOS_V4, nSSPluNo, nSharpPluNo, bModifiable );
		}
		break;
	}

	switch( nEntryType )
	{
	case OFFERENTRY_PLU_EXCLUDE:
		version.m_nMinVer = SPOS_V4_682;
		version.m_strReason = "PLU (Exclude) List";
		return;

	case OFFERENTRY_DEPT_SAMEPLU:
		version.m_nMinVer = SPOS_V4_682;
		version.m_strReason = "Department (Same PLU) List";
		return;
	}
}

/**********************************************************************/
#endif
/**********************************************************************/


/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

COfferGroupEntriesCSVArray::COfferGroupEntriesCSVArray() 
{
}

/**********************************************************************/

COfferGroupEntriesCSVArray::~COfferGroupEntriesCSVArray() 
{
}

/**********************************************************************/

int COfferGroupEntriesCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	return nReply;
}

/**********************************************************************/

void COfferGroupEntriesCSVArray::Validate()
{
	CString strLastKey = "";

	for ( int n = 0; n < GetSize(); n++ )
	{
		COfferGroupEntriesCSVRecord record;
		GetAt( n, record );

		CString strThisKey = record.GetEntryLookupKey();

		if ( strThisKey <= strLastKey )
		{
			RemoveAt( n, GetSize() - n );
			break;
		}

		strLastKey = strThisKey;
	}
}

/**********************************************************************/

bool COfferGroupEntriesCSVArray::FindEntryKey( COfferGroupEntriesCSVRecord& record, int& nIndex )
{
	CString strKey = record.GetEntryLookupKey();

	int nStart = 0;
	int nEnd = GetSize() - 1;

	while( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;

		COfferGroupEntriesCSVRecord arrayRecord;
		GetAt( nIndex, arrayRecord );
		CString strArrayKey = arrayRecord.GetEntryLookupKey();

		if ( strArrayKey < strKey )
			nStart = nIndex + 1;
		
		else if ( strArrayKey > strKey )
			nEnd = nIndex - 1;

		else
			return TRUE;
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

bool COfferGroupEntriesCSVArray::InsertRecord( COfferGroupEntriesCSVRecord& record, int& nIndex )
{
	if ( FindEntryKey( record, nIndex ) == FALSE )
	{
		InsertAt( nIndex, record );
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

void COfferGroupEntriesCSVArray::PurgeGroup( int nGroupNo )
{
	int nIdx1, nIdx2;
	GetGroupRange( nGroupNo, nIdx1, nIdx2 );

	if ( nIdx2 >= nIdx1 )
		RemoveAt( nIdx1, ( nIdx2 - nIdx1 ) + 1 );
}

/**********************************************************************/

void COfferGroupEntriesCSVArray::GetGroupRange( int nGroupNo, int& nIdx1, int& nIdx2 )
{
	COfferGroupEntriesCSVRecord record1, record2;
	record1.MakeDummy( nGroupNo );
	record2.MakeDummy( nGroupNo + 1 );

	FindEntryKey( record1, nIdx1 );
	FindEntryKey( record2, nIdx2 );

	nIdx2--;
}

/**********************************************************************/

void COfferGroupEntriesCSVArray::GetGroupSubRange( int nGroupNo, int nType, int& nIdx1, int& nIdx2 )
{
	COfferGroupEntriesCSVRecord record1, record2;
	record1.MakeDummy( nGroupNo, nType );
	record2.MakeDummy( nGroupNo, nType + 1 );
	
	FindEntryKey( record1, nIdx1 );
	FindEntryKey( record2, nIdx2 );

	nIdx2--;
}

/**********************************************************************/

bool COfferGroupEntriesCSVArray::CheckPluNo( int nFilterNo, CSQLPluNoInfo& infoPlu )
{
	bool bResult = TRUE;

	if ( nFilterNo != 0 )
	{
		bResult = FALSE;

		COfferGroupEntriesCSVRecord entry;
		entry.SetGroupNo( nFilterNo );
		entry.SetEntryPluNo( OFFERENTRY_PLU, infoPlu.m_nBasePluNo );

		int nIdx;
		if ( FindEntryKey( entry, nIdx ) == TRUE )
		{
			GetAt( nIdx, entry );
			bResult = entry.GetSingleModifierFlag( infoPlu.m_nModifier );
		}
	}

	return bResult;
}

/**********************************************************************/

bool COfferGroupEntriesCSVArray::IsFilterEmpty( int nFilterNo )
{
	int nIdx1, nIdx2;

	COfferGroupEntriesCSVRecord entry;
	entry.SetGroupNo( nFilterNo );
	entry.SetEntryPluNo( OFFERENTRY_PLU, 0 );
	FindEntryKey( entry, nIdx1 );

	entry.SetGroupNo( nFilterNo + 1 );
	FindEntryKey( entry, nIdx2 );

	return ( nIdx1 == nIdx2 );
}

/**********************************************************************/
