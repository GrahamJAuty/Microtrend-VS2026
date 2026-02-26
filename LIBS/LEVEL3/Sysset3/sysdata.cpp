/**********************************************************************/
#include "local.h"
#include "fields.h"
/**********************************************************************/
/*
struct NewSysData
{
	char SearchString[9];		// ( 9) search string
	char SerialNo[6];			// ( 6) serial number
	char spare[17];			// (17) spare
	char Checksum[32];			// (32) md5 checksum
};
*/
/**********************************************************************/
/*	LEGACY EMBEDDED SERIAL NUMBER										
/**********************************************************************/

CLegacyEmbeddedSerialNo::CLegacyEmbeddedSerialNo()
{
	m_nSerialNo = 0;
}

/**********************************************************************/

bool CLegacyEmbeddedSerialNo::SetData(const char* szData)
{
	CString strData = szData;

	if (strData.GetLength() == SYSSET_DATA_LEN)		// check length
	{
		if (strData.Mid(0, 9) == GetMatchString())		// check match string
		{
			if (CSSMD5::CalcMD5(strData.Mid(0, 32)) == strData.Mid(32, 32))	// check md5
			{
				m_nSerialNo = atoi(strData.Mid(9, 6));
				return TRUE;
			}
		}
	}
	return FALSE;
}

/**********************************************************************/

CString CLegacyEmbeddedSerialNo::GetData()
{
	CString strData = "";
	strData.Format ( "%s%6.6d00000000000000000", GetMatchString(), m_nSerialNo );
	strData += CSSMD5::CalcMD5(strData);
	return strData;
}

/**********************************************************************/

void CLegacyEmbeddedSerialNo::SetSerialNo(int nSerialNo)
{
	if ((nSerialNo >= 0) && (nSerialNo <= 999999))
	{
		m_nSerialNo = nSerialNo;
	}
}

/**********************************************************************/

const char* CLegacyEmbeddedSerialNo::GetMatchString()
{
//	return "\x013\x008\x001\x012\x010\x013\x00f\x006\x015";
	return "\x014\x009\x001\x012\x010\x013\x00f\x006\x015";
}

/**********************************************************************/
/*	UPDATE TOKEN												*/
/**********************************************************************/

CUpdateTokenData::CUpdateTokenData()
	: CSSIniFile(SS_INI_HEX)
{
}

int CUpdateTokenData::GetDealerCode() { return GetInt ( szDealerCode, 0 ); }
int CUpdateTokenData::GetSerialNo() { return GetInt ( szSerialNo, 0 ); }
CString CUpdateTokenData::GetVersion() { return GetString ( szVersion, "" ); }
int CUpdateTokenData::GetIsUpdateFlag() { return GetInt(szIsUpdate, 0); }

void CUpdateTokenData::SetDealerCode ( int n ) { Set ( szDealerCode, n ); }
void CUpdateTokenData::SetSerialNo ( int n ) { Set ( szSerialNo, n ); }
void CUpdateTokenData::SetVersion ( const char* sz ) { Set ( szVersion, sz ); }
void CUpdateTokenData::SetIsUpdateFlag ( int n ) { Set ( szIsUpdate, n ); }

/**********************************************************************/
/*	USER.DAT												*/
/**********************************************************************/

CUserData::CUserData()
	: CSSIniFile ( SS_INI_HEX )
{
}

/**********************************************************************/

CString CUserData::GetField ( int nIndex )
{
	CString strField;

	switch ( nIndex )
	{
	case 0:	strField = GetString ( szUserName, "" );	break;
	case 1:	strField = GetString ( szAdd1, "" );		break;
	case 2:	strField = GetString ( szAdd2, "" );		break;
	case 3:	strField = GetString ( szAdd3, "" );		break;
	case 4:	strField = GetString ( szAdd4, "" );		break;
	case 5:	strField = GetString ( szAdd5, "" );		break;
	}

	return strField;
}

void CUserData::SetField ( int nIndex, const char* szData )
{
	switch ( nIndex )
	{
	case 0:	Set ( szUserName, szData );	break;
	case 1:	Set ( szAdd1, szData );		break;
	case 2:	Set ( szAdd2, szData );		break;
	case 3:	Set ( szAdd3, szData );		break;
	case 4:	Set ( szAdd4, szData );		break;
	case 5:	Set ( szAdd5, szData );		break;
	}
} 

/**********************************************************************/

CString CUserData::GetExpiryDate()
{
	CString strExpiry = GetExpiry();
	strExpiry.TrimRight ( ' ' );

	CString strExpiryDate = "";
	if ( strExpiry.GetLength() > 0 )
		strExpiryDate = "01/" + strExpiry;			/* make 01/mm/yyyy */

	return strExpiryDate;
}

/**********************************************************************/

bool CUserData::IsRetailStockSystem()
{
	return ( GetSystemType() == 0 ) ? TRUE : FALSE;
}

bool CUserData::IsWetStockSystem()
{
	return ( GetSystemType() == 1 ) ? TRUE : FALSE;
}

/**********************************************************************/

CString CUserData::GetUserID() { return GetString ( szDealer, "" ); }
CString CUserData::GetExpiry() { return GetString ( szExpiry, "" ); }
int CUserData::GetMaxLocations() { return GetInt ( szMaxSites, 0 ); }
int CUserData::GetSmartcardCustNo() { return GetInt ( szCustNo, 0 ); }
bool CUserData::IsSchedulerSystem() { return GetBool ( szScheduler, FALSE ); }
bool CUserData::IsSmartcardSystem() { return GetBool ( szSmartcard, FALSE ); }
bool CUserData::IsGcr500System() { return GetBool ( szGcrReport, FALSE ); }
bool CUserData::IsBarmanSystem() { return GetBool ( szBarcode, FALSE ); }
bool CUserData::IsPhotoIDSystem() { return GetBool ( szPhotoID, FALSE ); }
bool CUserData::IsStockExportSystem() { return GetBool ( szExportStock, FALSE ); }
bool CUserData::IsSmartcardFormat() { return GetBool ( szCardFormat, FALSE ); }
bool CUserData::IsMealDealSystem() { return GetBool ( szMealDeal, FALSE ); }
int CUserData::GetMaxDatabases() { return GetInt ( szMaxDatabases, 0 ); }
int CUserData::GetMaxStockPoints() { return GetInt ( szMaxStockPoints, 0 ); }
int CUserData::GetMaxRemoteLocations() { return GetInt ( szMaxRemoteSites, 0 ); }
int CUserData::GetSystemType() { return GetInt ( szSystemType, 0 ); }
int CUserData::GetMaxEcrModels() { return GetInt ( szMaxEcrModels, 0 ); }
int CUserData::GetProgramNo() { return GetInt ( szProgramNo, 0 ); }
int CUserData::GetSerialNo() { return GetInt ( szSerialNo, 0 ); }
int CUserData::GetMaxUsers() { return GetInt ( szMaxUsers, 0 ); }
int CUserData::GetMaxComms() { return GetInt ( szMaxSessions, 0 ); }
int CUserData::GetTLogConvert() { return GetInt ( szTLogConvert, 0 ); }
int CUserData::GetPMS() { return GetInt ( szPMS, 0 ); }
int CUserData::GetExternalLink() { return GetInt ( szExternalLink, 0 ); }

/**********************************************************************/

void CUserData::SetUserID ( const char* szText ) { Set ( szDealer, szText ); }
void CUserData::SetExpiry ( const char* szDate ) { Set ( szExpiry, szDate ); }
void CUserData::SetMaxLocations ( int nMax ) { Set ( szMaxSites, nMax ); }
void CUserData::SetSmartcardCustNo ( int nCustNo ) { Set ( szCustNo, nCustNo ); }
void CUserData::SetSchedulerSystem ( bool b ) { Set ( szScheduler, b ); }
void CUserData::SetSmartcardSystem ( bool b ) { Set ( szSmartcard, b ); }
void CUserData::SetGcr500System ( bool b ) { Set ( szGcrReport, b ); }
void CUserData::SetBarmanSystem ( bool b ) { Set ( szBarcode, b ); }
void CUserData::SetPhotoIDSystem ( bool b ) { Set ( szPhotoID, b ); }
void CUserData::SetStockExportSystem ( bool b ) { Set ( szExportStock, b ); }
void CUserData::SetSmartcardFormat ( bool b ) { Set ( szCardFormat, b ); }
void CUserData::SetMealDealSystem ( bool b ) { Set ( szMealDeal, b ); }
void CUserData::SetMaxDatabases ( int nMax ) { Set ( szMaxDatabases, nMax ); }
void CUserData::SetMaxStockPoints ( int nMax ) { Set ( szMaxStockPoints, nMax ); }
void CUserData::SetMaxRemoteLocations ( int nMax ) { Set ( szMaxRemoteSites, nMax ); }
void CUserData::SetSystemType ( int n ) { Set ( szSystemType, n ); }
void CUserData::SetMaxEcrModels ( int nMax ) { Set ( szMaxEcrModels, nMax ); }
void CUserData::SetProgramNo ( int n ) { Set ( szProgramNo, n ); }
void CUserData::SetSerialNo ( int n ) { Set ( szSerialNo, n ); }
void CUserData::SetMaxUsers ( int nMax ) { Set ( szMaxUsers, nMax ); }
void CUserData::SetMaxComms ( int nMax ) { Set ( szMaxSessions, nMax ); }
void CUserData::SetTLogConvert ( int n ) { Set ( szTLogConvert, n ); }
void CUserData::SetPMS ( int n ) { Set ( szPMS, n ); }
void CUserData::SetExternalLink ( int n ) { Set ( szExternalLink, n ); }

/**********************************************************************/
/*	REG.DAT												*/
/**********************************************************************/

CRegData::CRegData()
	: CSSIniFile ( SS_INI_HEX )
{
}

/**********************************************************************/

void CRegData::SetSerialNo ( int nSerialNo )
{
	if ( nSerialNo >= 0 && nSerialNo <= 999999 )
		Set ( szSerialNo, nSerialNo );
}

/**********************************************************************/
/*														*/
/*	expiry date routines									*/
/*														*/
/*	000000 = system has been registered						*/
/*	999999 = system has not beeen registered					*/
/*	ddmmyy = expiry date									*/
/*														*/
/**********************************************************************/

void CRegData::SetSystemExpiry ( int nDays )
{
	SetExpiry ( nDays, SYSSET_EXPIRY_SYSTEM );
}

void CRegData::SetSystemExpiry ( const char* szDate )		// ddmmyy
{
	SetExpiryDate ( szDate );
	SetExpiryType ( SYSSET_EXPIRY_SYSTEM );
}

/**********************************************************************/

void CRegData::SetDealerExpiry ( int nDays )
{
	SetExpiry ( nDays, SYSSET_EXPIRY_DEALER );
}

void CRegData::SetDealerExpiry ( const char* szDate )		// ddmmyy
{
	SetExpiryDate ( szDate );
	SetExpiryType ( SYSSET_EXPIRY_DEALER );
}

/**********************************************************************/

void CRegData::SetExpiryDate ( const char* szDate )
{
	int nLen = strlen(szDate);

	if ( nLen == 6 )
		Set ( szExpiryDate, szDate );
}

/**********************************************************************/

void CRegData::SetExpiryType ( int nType )
{
	Set ( szExpiryType, nType );
}

/**********************************************************************/

void CRegData::SetExpiry ( int nDays, int nType )
{
	if ( nDays >= 0 )
	{
		COleDateTimeSpan tsExtend ( (long) nDays, 0, 0, 0 );
		COleDateTime tNow = COleDateTime::GetCurrentTime();
		COleDateTime tExtend = tNow + tsExtend;

		CString strExpiry;
		strExpiry = tExtend.Format ( "%d%m%y" );	// ddmmyy

		SetExpiryDate ( strExpiry );
		SetExpiryType ( nType );
	}
}

/**********************************************************************/

void CRegData::SetDealerPhone(const char* szPhone)
{
	CString strBuf = szPhone;

	for (int i = 0; i < strBuf.GetLength(); i++)
	{
		if (strchr(SYSSET_VALID_TELEPHONE, strBuf[i]) == NULL)
		{
			return;
		}
	}

	CString strTemp = "";		// do not allow our phone number to be used

	for (int i = 0; i < strBuf.GetLength(); i++)
	{
		char c = strBuf[i];

		if (c >= '0' && c <= '9')		// only allow digits
		{
			strTemp += c;
		}
	}

	if (strTemp.Find("7538572675") == -1)	// "7538572675"
	{
		Set(szDealerPhone, strBuf);		// ok - not our phone number
	}
}

/**********************************************************************/

CString CRegData::GetExpiryDate() { return GetString ( szExpiryDate, "999999" ); }
int CRegData::GetExpiryType() { return GetInt ( szExpiryType, 0 ); }
int CRegData::GetSerialNo() { return GetInt ( szSerialNo, 0 ); }
CString CRegData::GetDealerPhone() { return GetString ( szDealerPhone, "" ); }

/**********************************************************************/
