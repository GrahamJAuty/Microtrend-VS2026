/**********************************************************************/
#include "NetworkCSVArray.h"
#include "SPOSVersions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "WalletStampRuleCSVArray.h"
/**********************************************************************/

CWalletStampRuleCSVRecord::CWalletStampRuleCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CWalletStampRuleCSVRecord::ClearRecord()
{
	m_nRuleID = 1;
	m_nEdition = 1;
	m_bEnable = FALSE;
	m_nAction = 1;
	m_nOfferGroupNo = 0;
	m_nThreshold = 1;
	m_nExpiry = 0;
	m_strDescription = "";
}

/**********************************************************************/

void CWalletStampRuleCSVRecord::ConvertFromCSV(CCSV& csv)
{
	ClearRecord();

	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V4:
		switch (csv.GetInt(0))
		{
		case 1:
			V1ConvertFromCSV_PurchaseControl(csv);
			break;
		}
		break;

	case LOYALTY_TYPE_LOY_V4:
	default:
		switch (csv.GetInt(0))
		{
		case 1:
			V1To3ConvertFromCSV_StampOffer(csv, 1);
			break;

		case 2:
			V1To3ConvertFromCSV_StampOffer(csv, 2);
			break;

		case 3:
			V1To3ConvertFromCSV_StampOffer(csv, 3);
			break;
		}
		break;
	}
}

/**********************************************************************/

void CWalletStampRuleCSVRecord::V1To3ConvertFromCSV_StampOffer(CCSV& csv, int nVer)
{
	int n = 1;
	
	SetRuleID( csv.GetInt(n++) );

	if (nVer >= 2)
	{
		SetEdition(csv.GetInt(n++));
	}

	SetDescription(csv.GetString(n++));
	SetEnableFlag(csv.GetBool(n++));

	if ( nVer >= 3)
	{
		SetAction(csv.GetInt(n++));
	}

	SetOfferGroupNo(csv.GetInt(n++));
	SetThreshold(csv.GetInt(n++));
	SetExpiry(csv.GetInt(n++));
}

/**********************************************************************/

void CWalletStampRuleCSVRecord::V1ConvertFromCSV_PurchaseControl(CCSV& csv)
{
	int n = 1;
	SetRuleID(csv.GetInt(n++));
	SetDescription(csv.GetString(n++));
	SetEnableFlag(csv.GetBool(n++));
	SetOfferGroupNo(csv.GetInt(n++));
	SetThreshold(csv.GetInt(n++));
	SetExpiry(csv.GetInt(n++));
	m_nEdition = 1;
	m_nAction = 3;
}

/**********************************************************************/

void CWalletStampRuleCSVRecord::ConvertToCSV(CCSV& csv )
{
	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V4:
		csv.Add(WALLETSTAMP_VERSION_PURCHASECONTROL);
		csv.Add(m_nRuleID);
		csv.Add(m_strDescription);
		csv.Add(m_bEnable);
		csv.Add(m_nOfferGroupNo);
		csv.Add(m_nThreshold);
		csv.Add(m_nExpiry);
		break;

	case LOYALTY_TYPE_LOY_V4:
	default:
		csv.Add(WALLETSTAMP_VERSION_STAMPOFFER);
		csv.Add(m_nRuleID);
		csv.Add(m_nEdition);
		csv.Add(m_strDescription);
		csv.Add(m_bEnable);
		csv.Add(m_nAction);
		csv.Add(m_nOfferGroupNo);
		csv.Add(m_nThreshold);
		csv.Add(m_nExpiry);
		break;
	}
}

/**********************************************************************/

const char* CWalletStampRuleCSVRecord::GetDisplayName()
{
	m_strDisplayName = m_strDescription;
	::TrimSpaces( m_strDisplayName, FALSE );
	
	if (m_strDisplayName == "")
	{
		if (TRUE == m_bEnable)
		{
			switch (SysInfo.GetLoyaltyOrSmartPayType())
			{
			case LOYALTY_TYPE_SMP_V4:
				m_strDisplayName.Format("Purchase Control %d", m_nRuleID);
				break;

			case LOYALTY_TYPE_LOY_V4:
			default:
				m_strDisplayName.Format("Stamp Offer %d", m_nRuleID);
				break;
			}
		}
		else
		{
			m_strDisplayName = "----";
		}
	}

	return m_strDisplayName;
}

/**********************************************************************/

void CWalletStampRuleCSVRecord::SetRuleID( int n )
{
	Set( m_nRuleID, n, WalletStampRule::RuleID );
}

/**********************************************************************/

void CWalletStampRuleCSVRecord::SetEdition(int n)
{
	Set(m_nEdition, n, WalletStampRule::Edition);
}

/**********************************************************************/

void CWalletStampRuleCSVRecord::SetAction(int n)
{
	Set(m_nAction, n, WalletStampRule::Action);
}

/**********************************************************************/

void CWalletStampRuleCSVRecord::SetDescription(const char* sz)
{
	CString str = sz;
	::TrimSpaces(str, FALSE);
	Set(m_strDescription, str, WalletStampRule::Description);
}

/**********************************************************************/

void CWalletStampRuleCSVRecord::SetOfferGroupNo(int n)
{
	Set(m_nOfferGroupNo, n, WalletStampRule::OfferGroupNo);
}

/**********************************************************************/

void CWalletStampRuleCSVRecord::SetThreshold(int n)
{
	Set(m_nThreshold, n, WalletStampRule::Threshold);
}

/**********************************************************************/

void CWalletStampRuleCSVRecord::SetExpiry(int n)
{
	Set(m_nExpiry, n, WalletStampRule::Expiry);
}

/**********************************************************************/

void CWalletStampRuleCSVRecord::CheckMinimumSPOSVersion( CMinimumSPOSVersion& version )
{
	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V4:
		version.m_nMinVer = SPOS_V4_2_1579;
		break;

	case LOYALTY_TYPE_LOY_V4:
	default:
		version.m_nMinVer = SPOS_V4_2_1530;
		break;
	}

	CString strVersionNo = "";
	CNetworkCSVRecord::GetVersionNumberExternal( CONNECTION_TYPE_STANDARD_REALTIME, version.m_nMinVer, strVersionNo );
	
	version.m_strReason.Format ("%s require SPOS %s or later",
		SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4 ? "Purchase Controls" : "Stamp Offers",
		(const char*) strVersionNo );

	version.m_bInvalidData = FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CWalletStampRuleCSVArray::CWalletStampRuleCSVArray()
{
	ValidateArray();
}

/**********************************************************************/

CWalletStampRuleCSVArray::~CWalletStampRuleCSVArray()
{
}

/**********************************************************************/

int CWalletStampRuleCSVArray::GetRuleIDByIndex( int nIdx )
{
	int nRuleID = 0;

	CString strTemp = "";
	if ( ( strTemp = GetAt ( nIdx ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 2 );
		
		switch( csv.GetInt(0) )
		{
		case 1:
			nRuleID = csv.GetInt(1);
			break;
		}
	}
	
	return nRuleID;
}

/**********************************************************************/

bool CWalletStampRuleCSVArray::FindRuleByID( int nRuleID, int& nIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIdx = ( nStart + nEnd ) / 2;
		
		CWalletStampRuleCSVRecord Rule;
		GetAt ( nIdx, Rule );
		int nArrayID = Rule.GetRuleID();
			
		if (nArrayID == nRuleID)
		{
			return TRUE;
		}

		if ( nRuleID < nArrayID )
		{ 
			nEnd = nIdx - 1;
		}
		else
		{
			nStart = nIdx + 1;
		}
	}

	nIdx = nStart;
	return FALSE;
}

/**********************************************************************/

int CWalletStampRuleCSVArray::FindFirstFreeRuleID()
{	
	int nResult = 0;

	CWalletStampRuleCSVRecord Rule;
	for ( int n = 0; n < GetSize(); n++ )
	{
		GetAt( n, Rule );
		if ( Rule.GetRuleID() > n + 1 )
		{
			nResult = n + 1;
			break;
		}
	}

	if (nResult == 0)
	{
		nResult = GetSize() + 1;
	}

	if (nResult < WalletStampRule::RuleID.Min || nResult > WalletStampRule::RuleID.Max)
	{
		nResult = 0;
	}

	return nResult;
}

/**********************************************************************/

int CWalletStampRuleCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if (nReply != DB_ERR_SHARE)
	{
		if (nMode == DB_READONLY)	// if file does not exist use default values
		{
			nReply = DB_ERR_NONE;
		}
	}

	if (DB_ERR_NONE == nReply)
	{
		ValidateArray();
	}

	return nReply;
}

/**********************************************************************/

void CWalletStampRuleCSVArray::ValidateArray()
{
	int nIdx = 0;
	int nNextIDWanted = 1;
	CWalletStampRuleCSVRecord Rule;
	for (; (nIdx < GetSize()) && (nNextIDWanted <= WalletStampRule::RuleID.Max); nIdx++)
	{
		GetAt(nIdx, Rule);

		int nThisID = Rule.GetRuleID();

		if (nThisID == nNextIDWanted)
		{
			nNextIDWanted++;
			continue;
		}

		if ((nThisID < nNextIDWanted) || (nThisID > WalletStampRule::RuleID.Max))
		{
			break;
		}

		CreatePlaceholderRule(nNextIDWanted++, Rule);
		InsertAt(nIdx, Rule);
	}

	while (nIdx < GetSize())
	{
		RemoveAt(GetSize() - 1);
	}

	while (nNextIDWanted <= WalletStampRule::RuleID.Max)
	{
		CreatePlaceholderRule(nNextIDWanted++, Rule);
		Add(Rule);
	}
}

/**********************************************************************/

void CWalletStampRuleCSVArray::CreatePlaceholderRule(int nID, CWalletStampRuleCSVRecord& Rule)
{
	Rule.SetRuleID(nID);
	Rule.SetEdition(1);
	Rule.SetEnableFlag(FALSE);
	Rule.SetDescription("");
	Rule.SetThreshold(10);
	Rule.SetExpiry(0);
	Rule.SetOfferGroupNo(0);
	Rule.SetAction(1);
}

/**********************************************************************/

void CWalletStampRuleCSVArray::GetComboTexts( int nRuleID, CString& strNum, CString& strName )
{
	strNum.Format( "SO%2.2d", nRuleID );

	int nIdx = 0;
	if ( DataManager.WalletStampRule.FindRuleByID( nRuleID, nIdx ) == TRUE )
	{
		CWalletStampRuleCSVRecord Rule;
		DataManager.WalletStampRule.GetAt( nIdx, Rule );
		strName = Rule.GetDisplayName();		
	}
	else
	{
		strName = "Unknown";
	}
}

/**********************************************************************/

const char* CWalletStampRuleCSVArray::GetTabbedComboText( int nRuleID )
{
	CString strNum, strName;
	GetComboTexts( nRuleID, strNum, strName );

	CCSV csv;
	csv.Add( strNum );
	csv.Add( strName );
	m_strTabbedComboText = csv.GetLine();

	return m_strTabbedComboText;
}

/**********************************************************************/

const char* CWalletStampRuleCSVArray::GetSimpleListText( int nRuleID )
{
	CString strNum, strName;
	GetComboTexts( nRuleID, strNum, strName );

	m_strSimpleListText = "";
	m_strSimpleListText += strNum;
	m_strSimpleListText += "  ";
	m_strSimpleListText += strName;

	return m_strSimpleListText;
}

/**********************************************************************/

