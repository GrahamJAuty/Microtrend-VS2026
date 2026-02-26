/**********************************************************************/

#include "fields.h"

#define SYSSET_DATA_LEN 400
#define SYSSET_EXPIRY_SYSTEM	0
#define SYSSET_EXPIRY_DEALER	1

#define MES	CSysset::DecodeMessage

/**********************************************************************/
/*	private structure for CSysset use only!						*/
/**********************************************************************/

struct OldSysData
{
	char SearchString[9];		// ( 9) search string
	char Field1[32];			// (32) user name
	char Field2[32];			// (32) address 1
	char Field3[32];			// (32) address 2
	char Field4[32];			// (32) address 3
	char Field5[32];			// (32) dealer name
	char Field6[32];			// (32) licence no
	char UserId[12];			// (12) user id number
	char ExpiryDate[7];			// ( 7) expiry date mm/yyyy
	char MaxLocations[4];		// ( 4) max locations sites
	char SmartcardCustNo[6];		// ( 6) smartcard cust no
	char spare1[1];			// ( 1) check box demo (no longer used)
	char CheckScheduler[1];		// ( 1) check box scheduler
	char CheckSmartcard[1];		// ( 1) check box smartcard
	char CheckMealDeal[1];		// ( 1) check box meal deal (was pluxfer)
	char spare2[1];			// ( 1) check box stock (no longer used)
	char spare3[1];			// ( 1) check box t1tlog (no longer used)
	char CheckGcr500[1];		// ( 1) check box gcr500
	char spare4[1];			// ( 1) check box remote (no longer used)
	char spare5[1];			// ( 1) check box t1 plu (no longer used)
	char CheckBarman[1];		// ( 1) check box barman
	char CheckCoin[1];			// ( 1) check box coin
	char CheckPhotoId[1];		// ( 1) check box photo id
	char CheckStockExport[1];	// ( 1) check box stock export (was simple stock)
	char CheckNetworkComms[1];	// ( 1) check box network comms (0=not allowed, 1=allowed)
	char CheckCustomer[1];		// ( 1) check box customer
	char CheckSmartFormat[1];	// ( 1) check box smartcard format
	char spare6[16];			// (16) spare check boxes
	char MaxDatabases[4];		// ( 4) max databases
	char MaxStockpoints[4];		// ( 4) max stock points
	char MaxRemoteLocations[4];	// ( 4) max remote locations
	char SystemType[1];			// ( 1) system type (0=retail/1=wet or 0=smartcard/1=smprep)
	char MaxEcrModels[2];		// ( 2) max ecr models
	char ProgramNo[2];			// ( 2) program number (SYSSET_ECRMAN etc)
	char SerialNo[6];			// ( 6) serial number
	char MaxUsers[2];			// ( 2) max users
	char MaxComms[2];			// ( 2) max comms sessions
	char spare7[95];			// (95) spare
	char Random1[6];			// ( 6) random 1
	char Checksum[4];			// ( 4) checksum
	char Random2[6];			// ( 6) random 2
};

/**********************************************************************/
/*	private class for CSysset use only!						*/
/**********************************************************************/

class COldSysData
{
public:
	COldSysData();
	void SetData ( const char* szData );
	CString GetData();
	CString GetField ( int nIndex );				// 0 to 5
	CString GetUserID();
	CString GetExpiry();
	CString GetExpiryDate();
	int GetMaxLocations();
	int GetSmartcardCustNo();
	bool IsSchedulerSystem();
	bool IsSmartcardSystem();
	bool IsGcr500System();
	bool IsBarmanSystem();
	bool IsPhotoIDSystem();
	bool IsStockExportSystem();
	bool IsSmartcardFormat();
	bool IsMealDealSystem();
	int GetMaxDatabases();
	int GetMaxStockPoints();
	int GetMaxRemoteLocations();
	int GetSystemType();
	bool IsRetailStockSystem();
	bool IsWetStockSystem();
	int GetMaxEcrModels();
	int GetProgramNo();
	int GetSerialNo();
	int GetMaxUsers();
	int GetMaxComms();

	bool IsChecksumOk();
	int CalcChecksum();

private:
	CString Decode ( char* buf, int nWidth );

private:
	OldSysData m_Data;
	bool m_bSetOk;
};

/**********************************************************************/
/*	private class for CSysset use only!						*/
/**********************************************************************/

class COldSysExtra
{
public:
	COldSysExtra();
	void SetExtra ( const char* szExtra );

	CString GetExpiryDate();
	int GetExpiryType();

private:
	CString m_strExtra;
};

/**********************************************************************/

class CRegFile : public CSharedStringArray
{
public:
	CRegFile();
	~CRegFile();
	int Open ( const char* szFilename, int nMode );
	bool Close();

	CString GetUser();
	CString GetExtra();
	CString GetDealer();

	bool TestChecksum();

	static void Delete ( const char* szFilename );

private:
	CString DecodeString ( const char* szBuf );
	CString CalcChecksum();
	CString GetChecksum();

private:
	CString m_strFilename;
	int m_nMode;
	bool m_bChecksumOk;
};

/**********************************************************************/
/*														*/
/*	convert	SYSSET\Pnn\SysData								*/
/*	to		SYSSET\Pnn\R01 (reg)							*/
/*	and		SYSSET\Pnn\R02 (user)							*/
/*														*/
/**********************************************************************/

bool CSysset::ConvertOldToNew()
{
	CString strData;
	CString strExtra;
	CString strDealer;

	CString strOldRegFilename;
	strOldRegFilename.Format("%s%s\\P%2.2d\\%s", (const char*)GetDataPath(), (const char*)CFilenameTexts::Sysset(), m_nProgram, (const char*)CFilenameTexts::OldRegFilename());

	if ( _access ( strOldRegFilename, 0 ) == -1 )	// file does not exist (nothing to convert)
		return TRUE;

	CRegFile regFileOld;

	if ( regFileOld.Open ( strOldRegFilename, DB_READONLY ) == DB_ERR_NONE )
	{
		if ( regFileOld.TestChecksum() == TRUE )
		{
			strData = regFileOld.GetUser();
			strExtra = regFileOld.GetExtra();
			strDealer = regFileOld.GetDealer();
		}
		regFileOld.Close();
	}
	else
		return FALSE;

	if ( strData.GetLength() == SYSSET_DATA_LEN )
	{
		COldSysData sysData;
		COldSysExtra sysExtra;

		sysData.SetData ( strData );
		if ( sysData.IsChecksumOk() == FALSE )
			return FALSE;

		sysExtra.SetExtra ( strExtra );

		int nSerialNo = sysData.GetSerialNo();

		CSSIniFile userFile ( SS_INI_HEX );
		userFile.Set ( szSerialNo, nSerialNo );
		userFile.Set ( szUserName, sysData.GetField ( 0 ) );
		userFile.Set ( szAdd1, sysData.GetField ( 1 ) );
		userFile.Set ( szAdd2, sysData.GetField ( 2 ) );
		userFile.Set ( szAdd3, sysData.GetField ( 3 ) );
		userFile.Set ( szAdd4, sysData.GetField ( 4 ) );
		userFile.Set ( szAdd5, sysData.GetField ( 5 ) );
		userFile.Set ( szDealer, sysData.GetUserID() );
		userFile.Set ( szExpiry, sysData.GetExpiry() );
		userFile.Set ( szMaxSites, sysData.GetMaxLocations() );
		userFile.Set ( szCustNo, sysData.GetSmartcardCustNo() );
		userFile.Set ( szScheduler, sysData.IsSchedulerSystem() );
		userFile.Set ( szSmartcard, sysData.IsSmartcardSystem() );
		userFile.Set ( szCardFormat, sysData.IsSmartcardFormat() );
		userFile.Set ( szMealDeal, sysData.IsMealDealSystem() );
		userFile.Set ( szGcrReport, sysData.IsGcr500System() );
		userFile.Set ( szBarcode, sysData.IsBarmanSystem() );
		userFile.Set ( szPhotoID, sysData.IsPhotoIDSystem() );
		userFile.Set ( szExportStock, sysData.IsStockExportSystem() );
		userFile.Set ( szMaxDatabases, sysData.GetMaxDatabases() );
		userFile.Set ( szMaxStockPoints, sysData.GetMaxStockPoints() );
		userFile.Set ( szMaxRemoteSites, sysData.GetMaxRemoteLocations() );
		userFile.Set ( szSystemType, sysData.GetSystemType() );
		userFile.Set ( szMaxEcrModels, sysData.GetMaxEcrModels() );
		userFile.Set ( szProgramNo, sysData.GetProgramNo() );
		userFile.Set ( szMaxUsers, sysData.GetMaxUsers() );
		userFile.Set ( szMaxSessions, sysData.GetMaxComms() );
		userFile.Write ( GetUserFilename() );

		CSSIniFile regFile ( SS_INI_HEX );
		regFile.Set ( szSerialNo, nSerialNo );
		regFile.Set ( szExpiryDate, sysExtra.GetExpiryDate() );
		regFile.Set ( szExpiryType, sysExtra.GetExpiryType() );
		regFile.Set ( szDealerPhone, strDealer );
		regFile.Write ( GetRegFilename() );

		regFileOld.Delete ( strOldRegFilename );	// delete original file
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/
/*	SysData												*/
/**********************************************************************/

#define CODECHAR 0x1a

/**********************************************************************/

COldSysData::COldSysData() : m_Data{}
{
	m_bSetOk = FALSE;		// SetData has not been called
}

/**********************************************************************/

void COldSysData::SetData ( const char* szData )
{
	int nLen = strlen(szData);

	if ( nLen == SYSSET_DATA_LEN )
	{
		for ( int i = 0 ; i < nLen ; i++ )
			((char*)&m_Data)[i] = szData[i];

		m_bSetOk = TRUE;
	}
}

/**********************************************************************/

CString COldSysData::GetData()
{
	CString strBuf ( (char*)&m_Data, sizeof(m_Data) );
	return strBuf;
}

/**********************************************************************/

bool COldSysData::IsChecksumOk()
{
	if ( m_bSetOk == TRUE )
	{
		CString strBuf ( m_Data.Checksum, sizeof(m_Data.Checksum) );

		int nChecksum = atoi ( strBuf );

		if ( nChecksum == CalcChecksum() )
			return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

CString COldSysData::GetField ( int nIndex )
{
	CString strField;

	switch ( nIndex )
	{
	case 0:	strField = Decode ( m_Data.Field1, sizeof(m_Data.Field1) );	break;
	case 1:	strField = Decode ( m_Data.Field2, sizeof(m_Data.Field2) );	break;
	case 2:	strField = Decode ( m_Data.Field3, sizeof(m_Data.Field3) );	break;
	case 3:	strField = Decode ( m_Data.Field4, sizeof(m_Data.Field4) );	break;
	case 4:	strField = Decode ( m_Data.Field5, sizeof(m_Data.Field5) );	break;
	case 5:	strField = Decode ( m_Data.Field6, sizeof(m_Data.Field6) );	break;
	}

	return strField;
}

/**********************************************************************/

CString COldSysData::GetUserID()
{
	return Decode ( m_Data.UserId, sizeof(m_Data.UserId) );
}

/**********************************************************************/

CString COldSysData::GetExpiry()
{
	return Decode ( m_Data.ExpiryDate, sizeof(m_Data.ExpiryDate) );
}

CString COldSysData::GetExpiryDate()
{
	CString strExpiry = GetExpiry();
	strExpiry.TrimRight ( ' ' );

	CString strExpiryDate = "";
	if ( strExpiry.GetLength() > 0 )
		strExpiryDate = "01/" + strExpiry;			/* make 01/mm/yyyy */

	return strExpiryDate;
}

/**********************************************************************/

int COldSysData::GetMaxLocations()
{
	return atoi ( Decode ( m_Data.MaxLocations, sizeof(m_Data.MaxLocations) ) );
}

/**********************************************************************/

int COldSysData::GetSmartcardCustNo()
{
	return atoi ( Decode ( m_Data.SmartcardCustNo, sizeof(m_Data.SmartcardCustNo) ) );
}

/**********************************************************************/

bool COldSysData::IsSchedulerSystem()
{
	return ( Decode ( m_Data.CheckScheduler, sizeof(m_Data.CheckScheduler) ) == "0" ) ? FALSE : TRUE;
}

/**********************************************************************/

bool COldSysData::IsSmartcardSystem()
{
	return ( Decode ( m_Data.CheckSmartcard, sizeof(m_Data.CheckSmartcard) ) == "0" ) ? FALSE : TRUE;
}

/**********************************************************************/

bool COldSysData::IsGcr500System()
{
	return ( Decode ( m_Data.CheckGcr500, sizeof(m_Data.CheckGcr500) ) == "0" ) ? FALSE : TRUE;
}

/**********************************************************************/

bool COldSysData::IsBarmanSystem()
{
	return ( Decode ( m_Data.CheckBarman, sizeof(m_Data.CheckBarman) ) == "0" ) ? FALSE : TRUE;
}

/**********************************************************************/

bool COldSysData::IsPhotoIDSystem()
{
	return ( Decode ( m_Data.CheckPhotoId, sizeof(m_Data.CheckPhotoId) ) == "0" ) ? FALSE : TRUE;
}

/**********************************************************************/

bool COldSysData::IsStockExportSystem()
{
	return ( Decode ( m_Data.CheckStockExport, sizeof(m_Data.CheckStockExport) ) == "0" ) ? FALSE : TRUE;
}

/**********************************************************************/

bool COldSysData::IsSmartcardFormat()
{
	return ( Decode ( m_Data.CheckSmartFormat, sizeof(m_Data.CheckSmartFormat) ) == "0" ) ? FALSE : TRUE;
}

/**********************************************************************/

bool COldSysData::IsMealDealSystem()
{
	return ( Decode ( m_Data.CheckMealDeal, sizeof(m_Data.CheckMealDeal) ) == "0" ) ? FALSE : TRUE;
}

/**********************************************************************/

int COldSysData::GetMaxDatabases()
{
	return atoi ( Decode ( m_Data.MaxDatabases, sizeof(m_Data.MaxDatabases) ) );
}

/**********************************************************************/

int COldSysData::GetMaxStockPoints()
{
	return atoi ( Decode ( m_Data.MaxStockpoints, sizeof(m_Data.MaxStockpoints) ) );
}

/**********************************************************************/

int COldSysData::GetMaxRemoteLocations()
{
	return atoi ( Decode ( m_Data.MaxRemoteLocations, sizeof(m_Data.MaxRemoteLocations) ) );
}

/**********************************************************************/

int COldSysData::GetSystemType()
{
	return atoi ( Decode ( m_Data.SystemType, sizeof(m_Data.SystemType) ) );
}

bool COldSysData::IsRetailStockSystem()
{
	return ( GetSystemType() == 0 ) ? TRUE : FALSE;
}

bool COldSysData::IsWetStockSystem()
{
	return ( GetSystemType() == 1 ) ? TRUE : FALSE;
}

/**********************************************************************/

int COldSysData::GetMaxEcrModels()
{
	return atoi ( Decode ( m_Data.MaxEcrModels, sizeof(m_Data.MaxEcrModels) ) );
}

/**********************************************************************/

int COldSysData::GetProgramNo()
{
	return atoi ( Decode ( m_Data.ProgramNo, sizeof(m_Data.ProgramNo) ) );
}

/**********************************************************************/

int COldSysData::GetSerialNo()
{
	return atoi ( Decode ( m_Data.SerialNo, sizeof(m_Data.SerialNo) ) );
}

/**********************************************************************/

int COldSysData::GetMaxUsers()
{
	return atoi ( Decode ( m_Data.MaxUsers, sizeof(m_Data.MaxUsers) ) );
}

/**********************************************************************/

int COldSysData::GetMaxComms()
{
	return atoi ( Decode ( m_Data.MaxComms, sizeof(m_Data.MaxComms) ) );
}

/**********************************************************************/

int COldSysData::CalcChecksum()
{
	int nChecksum = 0;

	for ( int i = 0 ; i < offsetof(OldSysData,Checksum) ; i++ )
		nChecksum += (((char*)&m_Data)[i] * (i+1));

	nChecksum %= 10000;		/* limit range to 0-9999 */

	return nChecksum;
}

/**********************************************************************/
/*	Private routines										*/
/**********************************************************************/

CString COldSysData::Decode ( char* buf, int nWidth )
{
	CString str;

	for ( int i = nWidth-1 ; i >= 0 ; i-- )
		str += (char) ( buf[i] ^ CODECHAR );

	str.TrimRight ( ' ' );
	return str;
}

/**********************************************************************/
/*	SysExtra												*/
/**********************************************************************/

#define TOTAL_LEN	10

/**********************************************************************/

static const char* szDefaultExtra =	// length = 10
{
	"010"	// ( 0,3) total number of bytes (0 - 999)
	"999999"	// ( 3,6) expiry date (ddmmyy) 000000=registered 999999=not registered
	"0"		// ( 9,1) expiry type (0=system, 1=dealer)
};

/**********************************************************************/

static bool Validate(const char* szBuf)
{
	CString strBuf = szBuf;

	if (strBuf.GetLength() == TOTAL_LEN)
	{
		if (atoi(strBuf.Left(3)) == TOTAL_LEN)	// valid block length
		{
			return TRUE;
		}
	}

	return FALSE;		// invalid data block;
}

/**********************************************************************/

COldSysExtra::COldSysExtra()
{
	m_strExtra = szDefaultExtra;		// system has not been registered
}

/**********************************************************************/

void COldSysExtra::SetExtra(const char* szExtra)
{
	if (Validate(szExtra) == TRUE)
	{
		m_strExtra = szExtra;
	}
	else
	{
		m_strExtra = szDefaultExtra;	// system has not been registered
	}
}

/**********************************************************************/

CString COldSysExtra::GetExpiryDate()
{
	return m_strExtra.Mid(3, 6);
}

/**********************************************************************/

int COldSysExtra::GetExpiryType()
{
	return atoi(m_strExtra.Mid(9, 1));
}

/**********************************************************************/
/*	RegFile												*/
/**********************************************************************/

#define IDX_USER		0		// included in checksum
#define IDX_EXTRA		1		// included in checksum
#define IDX_USERID		2		// included in checksum
#define IDX_CHECKSUM	3
#define IDX_DEALER		4
#define MAX_LINES		5		// change this if any more lines added

/**********************************************************************/

CRegFile::CRegFile()
{
	m_nMode = DB_READONLY;
	m_strFilename = "";
	m_bChecksumOk = FALSE;
}

/**********************************************************************/

CRegFile::~CRegFile()
{
	Close();
}

/**********************************************************************/

int CRegFile::Open(const char* szFilename, int nMode)
{
	m_strFilename = szFilename;
	m_nMode = nMode;

	if (nMode == DB_READWRITE)
	{
		DWORD dwAttributes = GetFileAttributes(szFilename);

		if (dwAttributes != 0xffffffff)
		{
			SetFileAttributes(szFilename, (dwAttributes & ~(FILE_ATTRIBUTE_READONLY)));
		}
	}

	int nReply;

	if ((nReply = CSharedStringArray::Open(szFilename, nMode, 10)) != DB_ERR_SHARE)
	{
		for (int i = GetSize(); i < MAX_LINES; i++)	// make up shortfall
		{
			Add("");							// with default values
		}

		if (nMode == DB_READONLY)
		{
			nReply = DB_ERR_NONE;
		}
	}

	return nReply;
}

/**********************************************************************/

bool CRegFile::Close()
{
	bool bReply;

	//	PutChecksum ( CalcChecksum() );

	if ((bReply = CSharedStringArray::Close()) == TRUE)	// write array to disk
	{
		if (m_nMode == DB_READWRITE)
		{
			DWORD dwAttributes = GetFileAttributes(m_strFilename);

			if (dwAttributes != 0xffffffff)
			{
				SetFileAttributes(m_strFilename, dwAttributes | (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN));
			}

			m_nMode = DB_READONLY;
		}
	}

	return bReply;
}

/**********************************************************************/

void CRegFile::Delete(const char* szFilename)
{
	DWORD dwAttributes = GetFileAttributes(szFilename);

	if (dwAttributes != 0xffffffff)
	{
		SetFileAttributes(szFilename, (dwAttributes & ~(FILE_ATTRIBUTE_READONLY)));
	}

	remove(szFilename);
}

/**********************************************************************/

CString CRegFile::GetUser()
{
	return DecodeString(GetAt(IDX_USER));
}

/**********************************************************************/

CString CRegFile::GetExtra()
{
	return DecodeString(GetAt(IDX_EXTRA));
}

/**********************************************************************/

CString CRegFile::GetChecksum()
{
	return DecodeString(GetAt(IDX_CHECKSUM));
}

/**********************************************************************/

CString CRegFile::GetDealer()
{
	return GetAt(IDX_DEALER);
}

/**********************************************************************/

bool CRegFile::TestChecksum()
{
	if (GetChecksum() == CalcChecksum())
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

static int CalcLineChecksum(const char* szBuf)
{
	int nChecksum = 0;

	for (int i = 0; i < (int)strlen(szBuf); i++)
	{
		nChecksum += (int)szBuf[i];
	}

	return nChecksum;
}

/**********************************************************************/

CString CRegFile::CalcChecksum()
{
	int nChecksum = 3945;	// random starting value

	nChecksum += CalcLineChecksum(GetAt(IDX_USER));
	nChecksum += CalcLineChecksum(GetAt(IDX_EXTRA));
	nChecksum += CalcLineChecksum(GetAt(IDX_USERID));

	CString strChecksum = "";
	strChecksum.Format("%d", nChecksum);
	return strChecksum;
}

/**********************************************************************/

static CString ReorderString1(const char* szBuf)
{
	CString strBuf = "";

	int nLen = strlen(szBuf);

	for (int i = 0; i < nLen; i += 2)	// get all evens
	{
		strBuf += szBuf[i];
	}

	for (int i = 1; i < nLen; i += 2)	// get all odds
	{
		strBuf += szBuf[i];
	}

	return strBuf;
}

/**********************************************************************/

static CString ReorderString2(const char* szBuf)
{
	CString strBuf = "";

	int nLen = strlen(szBuf);	// there must be an even number of characters

	for (int i = 0; i < nLen / 2; i++)
	{
		strBuf += szBuf[i];
		strBuf += szBuf[nLen / 2 + i];
	}

	return strBuf;
}

/**********************************************************************/

static CString Convert2CharsToAscii(const char* szBuf)
{
	CString strAscii = "";

	for (int i = 0; i < (int)strlen(szBuf); i += 2)
	{
		BYTE c = ((szBuf[i] - 'a') << 4);	// a to p
		c |= (szBuf[i + 1] - 'a');			// a to p
		strAscii += c;
	}

	return strAscii;
}

/**********************************************************************/

static CString Expand(const char* szBuf)
{
	CString strBuf = "";
	CString strCount = "";

	for (int i = 0; i < (int)strlen(szBuf); i++)
	{
		char c = szBuf[i];

		if (c >= 'a' && c <= 'p')		// normal character
		{
			if (strCount != "")
			{
				int nCount = atoi(strCount);	// get number of times char is repeated

				for (int n = 0; n < nCount; n++)
				{
					strBuf += c;
				}

				strCount = "";			// clear count
			}
			else
			{
				strBuf += c;
			}
		}
		else if (c >= 'A' && c <= 'P')	// double character
		{
			strBuf += (char)(c | 0x20);		// convert to lowercase
			strBuf += (char)(c | 0x20);		// convert to lowercase
		}
		else if (c >= '0' && c <= '9')	// numeric
		{
			strCount += c;
		}
	}

	return strBuf;
}

/**********************************************************************/

CString CRegFile::DecodeString(const char* szBuf)
{
	CString strDecode = "";

	if (strlen(szBuf) > 0)
	{
		CString strBuf = Expand(szBuf);

		if ((strBuf.GetLength() & 1) == 0)	// there must be an even number of characters
		{
			strBuf = ReorderString2(strBuf);	// change order of characters
			strBuf = Convert2CharsToAscii(strBuf);

			int nLen = strBuf.GetLength();

			if (nLen >= 3)
			{
				int nMaxChar = strBuf[0];
				int nMinChar = strBuf[nLen - 1];

				if (nMinChar <= nMaxChar)
				{
					for (int i = 1; i < nLen - 1; i++)
					{
						if (strBuf[i] >= nMinChar && strBuf[i] <= nMaxChar)
						{
							int n1 = strBuf[i];
							int n2 = (i == 1) ? strBuf[0] : strDecode[i - 2];

							int n = (n1 - nMinChar) + (n2 - nMinChar);

							if (n > (nMaxChar - nMinChar))
							{
								n = n - (nMaxChar - nMinChar + 1);
							}

							strDecode += (char)(n + nMinChar);
						}
						else		// character outside permitted range found
						{
							strDecode = "";	// return blank string
							break;
						}
					}
				}
			}
		}
	}

	return strDecode;
}

/**********************************************************************/
