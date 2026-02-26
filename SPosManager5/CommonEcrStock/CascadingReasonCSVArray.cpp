/**********************************************************************/
#include "SPOSVersions.h"
/**********************************************************************/
#include "CascadingReasonCSVArray.h"
/**********************************************************************/

CCascadingReasonCSVRecord::CCascadingReasonCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CCascadingReasonCSVRecord::ClearRecord()
{
	m_nReasonNo = 1;
	m_nParentReasonConLevelInternal = 0;
	m_strParentReasonText = "";
	m_bParentManualEntry = FALSE;
	m_bLocalReason = TRUE;
	m_strLocalReasonText = "";
	m_bLocalManualEntry = FALSE;
}

/**********************************************************************/

void CCascadingReasonCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetReasonNo( csv.GetInt(0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV ( csv );		break;
	}
}

/**********************************************************************/

void CCascadingReasonCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetLocalReasonText( csv.GetString(2) );
	SetLocalManualEntryFlag( csv.GetBool(3) );
	SetLocalReasonFlag( csv.GetBool(4) );
	SetParentReasonConLevelInternal( csv.GetInt(5) );
	SetParentReasonText( csv.GetString(6) );
	SetParentManualEntryFlag( csv.GetBool(7) );
}

/**********************************************************************/

void CCascadingReasonCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nReasonNo );
	csv.Add ( CASCADINGREASON_VERSION );
	csv.Add ( m_strLocalReasonText );
	csv.Add ( m_bLocalManualEntry );
	csv.Add ( m_bLocalReason );
	csv.Add ( m_nParentReasonConLevelInternal );
	csv.Add ( m_strParentReasonText );
	csv.Add ( m_bParentManualEntry );
}

/**********************************************************************/

int CCascadingReasonCSVRecord::GetParentReasonConLevelExternal()
{
	switch( m_nParentReasonConLevelInternal )
	{
	case 1:		return NODE_SYSTEM;
	case 2:		return NODE_DATABASE;
	case 0:
	default:	return NODE_PLACEHOLDER;
	}	
}

/**********************************************************************/

void CCascadingReasonCSVRecord::SetReasonNo( int n )
{
	Set ( m_nReasonNo, n, CascadingReason::ReasonNo );
}

/**********************************************************************/

void CCascadingReasonCSVRecord::SetLocalReasonText( const char* sz )
{
	Set ( m_strLocalReasonText, sz, CascadingReason::ReasonText );
}

/**********************************************************************/

void CCascadingReasonCSVRecord::SetLocalManualEntryFlag( bool b )
{
	m_bLocalManualEntry = b;
}

/**********************************************************************/

void CCascadingReasonCSVRecord::SetParentReasonConLevelInternal( int n )
{
	Set ( m_nParentReasonConLevelInternal, n, CascadingReason::ParentReasonConLevelInternal );
}

/**********************************************************************/

void CCascadingReasonCSVRecord::SetParentReasonConLevelExternal( int n )
{
	int nInternal = 0;

	switch( n )
	{
	case NODE_SYSTEM:			nInternal = 1;	break;
	case NODE_DATABASE:			nInternal = 2;	break;
	case NODE_PLACEHOLDER:
	default:					nInternal = 0;	break;
	}

	m_nParentReasonConLevelInternal = nInternal;
}

/**********************************************************************/

void CCascadingReasonCSVRecord::SetParentReasonText( const char* sz )
{
	Set ( m_strParentReasonText, sz, CascadingReason::ReasonText );
}

/**********************************************************************/

void CCascadingReasonCSVRecord::SetParentManualEntryFlag( bool b )
{
	m_bParentManualEntry = b;
}

/**********************************************************************/

void CCascadingReasonCSVRecord::CheckMinimumSPOSVersion( CMinimumSPOSVersion& version )
{
	version.m_nMinVer = SPOS_V4;
	version.m_strReason = "";
	version.m_bInvalidData = FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCascadingReasonCSVArray::CCascadingReasonCSVArray() 
{
	m_nReasonType = REASONTEXT_TYPE_ITEMVOID;
}

/**********************************************************************/

CCascadingReasonCSVArray::~CCascadingReasonCSVArray() 
{
}

/**********************************************************************/

void CCascadingReasonCSVArray::Reset()
{
	RemoveAt( 0, GetSize() );
}

/**********************************************************************/

int CCascadingReasonCSVArray::Open(const char* szFilename, int nMode)
{
	RemoveAt(0, GetSize());

	int nReply = CSharedCSVArray::Open(szFilename, nMode);

	if (nReply != DB_ERR_SHARE)
	{
		if (nMode == DB_READONLY)	// if file does not exist use default values
		{
			nReply = DB_ERR_NONE;
		}
	}

	int nLastReasonNo = 0;
	int nValidRecords = 0;

	CCascadingReasonCSVRecord Reason;
	for (; nValidRecords < GetSize(); nValidRecords++)
	{
		int nReasonNo = GetReasonNo(nValidRecords);
		if ((nReasonNo <= nLastReasonNo) || (nReasonNo > CascadingReason::ReasonNo.Max))
		{
			break;
		}

		nLastReasonNo = nReasonNo;
	}

	while (GetSize() > nValidRecords)
	{
		RemoveAt(GetSize() - 1);
	}

	return nReply;
}

/**********************************************************************/

bool CCascadingReasonCSVArray::FindReasonByNumber( int nReasonNo, int& nReasonIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nReasonIdx = ( nStart + nEnd ) / 2;
		
		int nArrayNo = GetReasonNo(nReasonIdx);
			
		if ( nReasonNo == nArrayNo )
			return TRUE;

		if ( nReasonNo < nArrayNo )
			nEnd = nReasonIdx - 1;
		else
			nStart = nReasonIdx + 1;
	}

	nReasonIdx = nStart;
	return FALSE;
}

/**********************************************************************/

int CCascadingReasonCSVArray::FindFirstFreeNumber()
{
	int nResult = 0;

	for (int n = 0; n < GetSize(); n++)
	{
		if (GetReasonNo(n) > n + 1)
		{
			nResult = n + 1;
			break;
		}
	}

	if (nResult == 0)
	{
		nResult = GetSize() + 1;
	}

	if ((nResult < CascadingReason::ReasonNo.Min) || (nResult > CascadingReason::ReasonNo.Max))
	{
		nResult = 0;
	}

	return nResult;
}

/**********************************************************************/

int CCascadingReasonCSVArray::GetReasonNo( int nReasonIdx )
{
	int nReasonNo = 0;

	CString strTemp;
	if ( ( strTemp = GetAt ( nReasonIdx ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		nReasonNo = csv.GetInt(0);
	}

	return nReasonNo;
}

/**********************************************************************/

void CCascadingReasonCSVArray::LoadParentReasonsSystem()
{
	CLocationSelectorEntity ParentEntity;
	ParentEntity.SetSystemNode();
	LoadParentReasons( ParentEntity);
}

/**********************************************************************/

void CCascadingReasonCSVArray::LoadParentReasonsDatabase( int nDbIdx )
{
	CLocationSelectorEntity ParentEntity;
	ParentEntity.SetDatabaseNode( nDbIdx );
	LoadParentReasons( ParentEntity );
}

/**********************************************************************/

void CCascadingReasonCSVArray::LoadParentReasons( CLocationSelectorEntity& LocSelEntity )
{
	CCascadingReasonCSVArray* pParentArray = NULL;
	int nParentType = LocSelEntity.GetConType();
	
	CDataManagerInfo infoDM;

	switch( nParentType )
	{
	case NODE_SYSTEM:
		DataManagerNonDb.OpenCCReasonSystem( DB_READONLY, m_nReasonType, infoDM );
		pParentArray = &DataManagerNonDb.CCReasonSystem;
		break;

	case NODE_DATABASE:
		DataManagerNonDb.OpenCCReasonDatabase( LocSelEntity.GetDbIdx(), DB_READONLY, m_nReasonType, infoDM );
		pParentArray = &DataManagerNonDb.CCReasonDatabase;
		break;
	}

	if (NULL == pParentArray)
	{
		return;
	}

	for ( int n = 0; n < pParentArray -> GetSize(); n++ )
	{
		CCascadingReasonCSVRecord ParentReason;
		pParentArray -> GetAt( n, ParentReason );

		if ( ParentReason.GetLocalReasonFlag() == TRUE )
		{
			CCascadingReasonCSVRecord LocalReason;
			bool bNewLocalReason = FALSE;
			bool bUseParentReason = FALSE;

			int nLocalReasonIdx;
			if ( FindReasonByNumber( ParentReason.GetReasonNo(), nLocalReasonIdx ) == FALSE )
			{
				bNewLocalReason = TRUE;
				bUseParentReason = TRUE;
				LocalReason.SetReasonNo( ParentReason.GetReasonNo() );
				LocalReason.SetLocalReasonFlag( FALSE );
				LocalReason.SetLocalReasonText( "" );
				LocalReason.SetLocalManualEntryFlag( FALSE );
			}
			else
			{
				bNewLocalReason = FALSE;
				GetAt( nLocalReasonIdx, LocalReason );
				bUseParentReason = ( LocalReason.GetParentReasonConLevelExternal() == NODE_PLACEHOLDER );
			}

			if ( TRUE == bUseParentReason )
			{
				LocalReason.SetParentReasonConLevelExternal( nParentType );
				LocalReason.SetParentReasonText( ParentReason.GetLocalReasonText() );
				LocalReason.SetParentManualEntryFlag( ParentReason.GetLocalManualEntryFlag() );
				
				if ( FALSE == bNewLocalReason )
				{
					SetAt( nLocalReasonIdx, LocalReason );
				}
				else
				{
					InsertAt( nLocalReasonIdx, LocalReason );				  
				}
			}
		}
	}

	switch( nParentType )
	{
	case NODE_SYSTEM:
		DataManagerNonDb.CloseCCReasonSystem( infoDM );
		break;

	case NODE_DATABASE:
		DataManagerNonDb.CloseCCReasonDatabase( infoDM );
		break;
	}
}

/**********************************************************************/

void CCascadingReasonCSVArray::PrepareForWrite()
{
	for ( int nReasonIdx = GetSize() - 1; nReasonIdx >= 0; nReasonIdx-- )
	{
		CCascadingReasonCSVRecord Reason;
		GetAt( nReasonIdx, Reason );

		if ( Reason.GetLocalReasonFlag() == FALSE )
		{
			RemoveAt( nReasonIdx );
		}
		else
		{
			Reason.SetParentReasonConLevelExternal( NODE_PLACEHOLDER );
			Reason.SetParentReasonText( "" );
			Reason.SetParentManualEntryFlag( FALSE );
			SetAt( nReasonIdx, Reason );
		}
	}
}

/**********************************************************************/

const char* CCascadingReasonCSVArray::GetReasonTextFromNumber( int nReasonNo )
{
	int nReasonIdx;
	if ( FindReasonByNumber( nReasonNo, nReasonIdx ) == TRUE )
	{
		CCascadingReasonCSVRecord Reason;
		GetAt( nReasonIdx, Reason );
		m_strReasonText = Reason.GetParentReasonText();
	}
	else
	{
		m_strReasonText = "";
	}

	return m_strReasonText;
}

/**********************************************************************/

CString CCascadingReasonCSVArray::GetReasonTypeText(int n)
{
	CString strResult = "";

	switch (n)
	{
	case REASONTEXT_TYPE_ITEMVOID:
		strResult = "Item Void";
		break;

	case REASONTEXT_TYPE_TRANVOID:
		strResult = "Transaction Void";
		break;

	case REASONTEXT_TYPE_REFUND:
		strResult = "Refund";
		break;

	case REASONTEXT_TYPE_VIP:
		strResult = "VIP";
		break;

	case REASONTEXT_TYPE_PROMO:
		strResult = "Promo";
		break;

	case REASONTEXT_TYPE_WASTAGE:
		strResult = "Wastage";
		break;

	case REASONTEXT_TYPE_NOSALE:
		strResult = "No Sale";
		break;

	case REASONTEXT_TYPE_VOIDSURCHARGE:
		strResult = "Void Surcharge";
		break;

	default:
		strResult = "Unknown";
		break;
	}

	return strResult;
}

/**********************************************************************/

CString CCascadingReasonCSVArray::GetSQLLabelByReasonType(int n)
{
	CString strResult = "";

	switch (n)
	{
	case REASONTEXT_TYPE_ITEMVOID:
		strResult = "VOIDS";
		break;

	case REASONTEXT_TYPE_TRANVOID:
		strResult = "TXNVOIDS";
		break;

	case REASONTEXT_TYPE_REFUND:
		strResult = "REFUNDS";
		break;

	case REASONTEXT_TYPE_VIP:
		strResult = "VIPS";
		break;

	case REASONTEXT_TYPE_PROMO:
		strResult = "PROMOS";
		break;

	case REASONTEXT_TYPE_WASTAGE:
		strResult = "WASTAGE";
		break;

	case REASONTEXT_TYPE_NOSALE:
		strResult = "NOSALES";
		break;

	case REASONTEXT_TYPE_VOIDSURCHARGE:
		strResult = "VOIDSURCHARGE";
		break;

	default:
		strResult = "UNKNOWN";
		break;
	}

	return strResult;
}

/**********************************************************************/

CString CCascadingReasonCSVArray::GetCfgTillLabelByReasonType(int n, bool bSwitchItemVoid)
{
	CString strResult = "";

	switch (n)
	{
	case REASONTEXT_TYPE_ITEMVOID:
		if (DealerFlags.GetReasonTicksVoidAfterFlag() != bSwitchItemVoid)
		{
			strResult = "[ReasonVoidAfter]";
		}
		else
		{
			strResult = "[ReasonVoid]";
		}
		break;

	case REASONTEXT_TYPE_TRANVOID:
		strResult = "[ReasonTxnVoid]";
		break;

	case REASONTEXT_TYPE_REFUND:
		strResult = "[ReasonRefund]";
		break;

	case REASONTEXT_TYPE_VIP:
		strResult = "[ReasonVIP]";
		break;

	case REASONTEXT_TYPE_PROMO:
		strResult = "[ReasonPromo]";
		break;

	case REASONTEXT_TYPE_WASTAGE:
		strResult = "[ReasonWastage]";
		break;

	case REASONTEXT_TYPE_NOSALE:
		strResult = "[ReasonNoSale]";
		break;

	case REASONTEXT_TYPE_VOIDSURCHARGE:
		strResult = "[ReasonVoidSurcharge]";
		break;

	default:
		strResult = "UNKNOWN";
		break;
	}

	return strResult;
}

/**********************************************************************/

int CCascadingReasonCSVArray::GetReasonTypeBySQLLabel(CString strLabel)
{
	int nResult = -1;

	strLabel.MakeUpper();

	for (int n = 0; n < REASONTEXT_TYPES; n++)
	{
		CString strTest = GetSQLLabelByReasonType(n);
		if (strTest == strLabel)
		{
			nResult = n;
			break;
		}
	}

	return nResult;
}

/**********************************************************************/
