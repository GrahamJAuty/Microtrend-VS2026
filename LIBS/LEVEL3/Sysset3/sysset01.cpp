/**********************************************************************/

#include "local.h"

/**********************************************************************/
/*	Folder Structure										*/
/**********************************************************************/
/*
	[DataPath]
		+ [Pnn]					(folder for each program)
		|	+ [PCnnnnnnnnnn]		(folder for each pc)
		|	+ [Temp00]			(common folder for all spawned instances)
		|	+ [Temp01]			(folder for each instance)
		|	+ [Temp02] etc
		|
		+ [Sysset]				(folder for use by sysset only)
			+ [Pnn]				(folder for each program)
			|	SysData			(old registration data - pre sysset3)
			|	R01				(new registration data (reg))
			|	R02				(new registration data (user))
			|	S00				(test file to make sure folder can be written to)
			|	S01				(lock file for each instance)
			|	S02 etc
			|	C01				(lock file for each comms session)
			|	C02 etc
			|	+ [PCnnnnnnnnnn]	(folder for each pc)
			|		+ [ws]		{folder for window sizes)
			|			MD5(id)	(file containing window size and position)
			|
			+ [PC]
				MD5(ComputerName)	(file containing: PCnnnnnnnnnn,ComputerName)
*/
/**********************************************************************/

#define VERSION_FULL		0
#define VERSION_UPGRADE		1
#define VERSION_DOWNGRADE	2

/**********************************************************************/
/*														*/
/*	The following data area is modified by running ecrset			*/
/*														*/
/**********************************************************************/

static const char* szData =	// length = 64 exactly
{
	"\x014\x009\x001\x012\x010\x013\x00f\x006\x015"	// ( 9) search string
	"000000"									// ( 6) serial number
	"00000000000000000"							// (17) spare
	"d7b69fa615c0fa1c8c6eab2ce94b0ccb"				// (32) md5 checksum
};

/**********************************************************************/

CSysset::CSysset()
{
	m_nVersionType = VERSION_DOWNGRADE;
	m_strError = "";
	m_nProgram = 0;
	m_nInstanceNo = 0;
	m_nPCIdHost = 0;
	m_nPCIdClient = 0;
	m_bUsersLocked = FALSE;
	m_bSpawned = FALSE;
	m_strDataPath = "";

	m_bNobbledDemo = FALSE;
	m_nDemoTokenLocCount = -1;
	m_nDemoTokenDbCount = -1;
	m_nDemoTokenUserCount = -1;
}

CSysset::~CSysset()
{
	m_UserData.Write();
	m_RegData.Write();
}

/**********************************************************************/

bool CSysset::IsNobbledDemo()
{
	if (FALSE == m_bNobbledDemo)
	{
		return FALSE;
	}

	int nSerialNumber = GetSerialNo();
	if ((nSerialNumber < 9001) || (nSerialNumber > 9099))
	{
		return FALSE;
	}

	if ((GetExternalLink() & 1024) == 0)
	{
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

int CSysset::GetRegTokenOrLegacyEmbeddedSerialNo(int nProgramNo, CString strPath, CString strVersion)
{
	int nProgramTemp = m_nProgram;

	m_nProgram = nProgramNo;

	int nResult = 0;

	if (MayHaveLegacyEmbeddedSerialNo() == FALSE)
	{
		CString strRegTokenError = "";
		CheckRegToken(nResult, strPath, strVersion, strRegTokenError);
	}
	else
	{
		CLegacyEmbeddedSerialNo embed;
		if (embed.SetData(szData) == TRUE)
		{
			nResult = embed.GetSerialNo();
		}
	}

	m_nProgram = nProgramTemp;

	return nResult;
}

/**********************************************************************/

bool CSysset::TestOtherProgram( int nProgram )
{
	m_nVersionType = VERSION_DOWNGRADE;
	m_strError = "";
	m_nProgram = nProgram;
	m_nInstanceNo = 0;
	m_nPCIdHost = 0;
	m_nPCIdClient = 0;
	m_bSpawned = FALSE;
	m_strDataPath = "";

	CSSFile file;
	if ( file.Open ( CFilenameTexts::DatapathFilename(), "rb" ) == TRUE )
	{
		CString strDataPath;
		if ( file.ReadString ( strDataPath ) == TRUE )
		{
			m_strDataPath = strDataPath;
		}

		file.Close();
	}

	if ( m_strDataPath != "" )	// m_strDataPath can be "" or MUST include trailing backslash eg "c:\fred\"
	{
		if ( m_strDataPath.Right(1) == "\\" )		// got trailing backslash
		{
			if ( ExistSubdirectory ( m_strDataPath ) == FALSE )
			{
				m_strError = CMessageTexts::DataPathNotExist();
				return FALSE;
			}
		}
		else
		{
			m_strError = CMessageTexts::DataPathInvalid();
			return FALSE;
		}
	}

	CString strUserFilename = GetUserFilename();
	CString strRegFilename = GetRegFilename();

	bool bExistReg = ( _access ( strRegFilename, 0 ) == 0 ) ? TRUE : FALSE;	// R01
	bool bExistUser = ( _access ( strUserFilename, 0 ) == 0 ) ? TRUE : FALSE;	// R02

	if ( bExistUser == FALSE || bExistReg == FALSE )		// R01 or R02 not found (both must exist)
	{
		m_strError = CMessageTexts::UnregisteredSoftware();
		return FALSE;
	}

	if ( m_RegData.Read ( strRegFilename ) == FALSE )
	{
		m_strError = CMessageTexts::RegistrationFileCorrupted();
		return FALSE;
	}

	if ( m_UserData.Read ( strUserFilename ) == FALSE )
	{
		m_strError = CMessageTexts::UserFileCorrupted();
		return FALSE;
	}

	if ( m_RegData.GetSerialNo() != m_UserData.GetSerialNo() )	// serial numbers in both files must match (RnnFile:UpdateFile Rnnnnnn:Unnnnnn)
	{
		m_strError.Format( "Serial number does not match (R%6.6d : U%6.6d)",
			m_RegData.GetSerialNo(), 
			m_UserData.GetSerialNo());

		return FALSE;
	}

	if ( nProgram != m_UserData.GetProgramNo() )
	{
		m_strError = "Unknown program type";
		return FALSE;
	}

	if ( file.Open ( GetShareFilename(0), "wb" ) == TRUE )	// test that the shared folder is writable
	{
		file.Close();
		remove ( GetShareFilename(0) );	// delete S00 file
	}
	else
	{
		m_strError = "Unable to write to the Data path";
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

bool CSysset::TestRegistrationMainExe(int nProgram, CWnd* pParent, bool& bCanCreatePCId, const char* szVersion, const char* szMinTokenDate, bool bSpawned, int nDataPathNo)
{
	return TestRegistrationInternal(nProgram, pParent, bCanCreatePCId, szVersion, szMinTokenDate, bSpawned, nDataPathNo, FALSE, TRUE);
}

/**********************************************************************/

bool CSysset::TestRegistrationModule(int nProgram, CWnd* pParent, bool& bCanCreatePCId, bool bSpawned, int nDataPathNo)
{
	return TestRegistrationInternal(nProgram, pParent, bCanCreatePCId, "", "", bSpawned, nDataPathNo, TRUE, FALSE);
}

/**********************************************************************/

bool CSysset::TestRegistrationInternal ( int nProgram, CWnd* pParent, bool& bCanCreatePCId, const char* szVersion, const char* szMinTokenDate, bool bSpawned, int nDataPathNo, bool bWaiveUpdateToken, bool bAllowRegToken )
{
	m_nVersionType = VERSION_DOWNGRADE;
	m_strError = "";
	m_nProgram = nProgram;
	m_nInstanceNo = 0;
	m_nPCIdHost = 0;
	m_nPCIdClient = 0;
	m_bSpawned = bSpawned;
	m_strDataPath = "";

	int nSerialNoFromFile = 0;

	switch (m_nProgram)
	{
	case SYSSET_LOYALTY4:
	case SYSSET_SMARTPAY4:
	{
		CSSFile file;
		if (file.Open("SerialNo.dat", "rb") == TRUE)
		{
			CString strBuffer;
			if (file.ReadLine(strBuffer) == TRUE)
			{
				nSerialNoFromFile = atoi(strBuffer);
			}
			file.Close();
		}
		else
		{
			m_strError = "Serial no file not found";
			return FALSE;
		}

		if ((nSerialNoFromFile < 9000) || (nSerialNoFromFile > 999999))
		{
			m_strError = "Invalid Serial no from file";
			return FALSE;
		}

		char buffer[MAX_PATH];

		CString strKeyName = "software\\microtrend\\";

		if (SYSSET_SMARTPAY4 == nProgram)
		{
			strKeyName += "SmartPay4";
		}
		else
		{
			strKeyName += "Loyalty4";
		}

		CString strSubKeyName = "";
		strSubKeyName.Format("DataPath_%6.6d", nSerialNoFromFile);

		CRegKey RegKey{};
		int nResult1 = RegKey.Open(HKEY_LOCAL_MACHINE, strKeyName, KEY_QUERY_VALUE);
		if (nResult1 != ERROR_SUCCESS)
		{
			m_strError.Format("Unable to access registry key (1:%d)", nResult1);
			return FALSE;
		}

		ULONG nBufSize = MAX_PATH;
		int nResult2 = RegKey.QueryStringValue(strSubKeyName, buffer, &nBufSize);
		if (nResult2 != ERROR_SUCCESS)
		{
			m_strError.Format("Unable to access registry key (2:%d)", nResult2);
			return FALSE;
		}

		m_strDataPath = buffer;
	}	
	break;

	default:
		{
			CSSFile file;
			if (file.Open(CFilenameTexts::DatapathFilename(), "rb") == TRUE)
			{
				int i = 0;
				CString strDataPath;
				while (file.ReadString(strDataPath) == TRUE)
				{
					if (i++ == nDataPathNo)		// get line corresponding to nDataPathNo
					{
						m_strDataPath = strDataPath;
						break;
					}
				}

				file.Close();
			}
		}
		break;
	}

	if ( m_strDataPath != "" )	// m_strDataPath can be "" or MUST include trailing backslash eg "c:\fred\"
	{
		if ( m_strDataPath.Right(1) == "\\" )		// got trailing backslash
		{
			if ( ExistSubdirectory ( m_strDataPath ) == FALSE )
			{
				m_strError = CMessageTexts::DataPathNotExist();
				return FALSE;
			}
		}
		else
		{
			m_strError = CMessageTexts::DataPathInvalid();
			return FALSE;
		}
	}

	CLegacyEmbeddedSerialNo embed;

	if (MayHaveLegacyEmbeddedSerialNo() == TRUE)
	{
		if (embed.SetData(szData) == FALSE)
		{
			m_strError = "Corrupted program file";
			return FALSE;
		}
	}

	if ( ConvertOldToNew() == FALSE )
	{
		m_strError = "Cannot convert corrupted registration file";
		return FALSE;
	}

	if (SetPCId(bCanCreatePCId) == FALSE)	// error message supplied by function
	{
		return FALSE;
	}

	CString strUserFilename = GetUserFilename();
	CString strRegFilename = GetRegFilename();
	CString strUpdateTokenFilename = GetUpdateTokenFilename();

	bool bExistReg = ( _access ( strRegFilename, 0 ) == 0 ) ? TRUE : FALSE;	// R01
	bool bExistUser = ( _access ( strUserFilename, 0 ) == 0 ) ? TRUE : FALSE;	// R02
	bool bExistUpdateToken = (_access(strUpdateTokenFilename, 0) == 0) ? TRUE : FALSE;	// R03

	//EMBEDDED SERIAL NUMBER IS BEING MOVED FROM EXE TO A REGISTRATION TOKEN
	//THIS IS TO ALLOW FOR CODE SIGNING OF A CONSTANT EXE FOR EACH VERSION
	int nEmbeddedSerialNo = 0;
	bool bWriteInternalUpdateToken = FALSE;
	CString strRegTokenError = "";

	if ((MayHaveLegacyEmbeddedSerialNo() == FALSE) && (TRUE == bAllowRegToken))
	{
		CheckRegToken(nEmbeddedSerialNo, "", szVersion, strRegTokenError);
		
		if ((strRegTokenError == "") && (nEmbeddedSerialNo != 0))
		{
			bWriteInternalUpdateToken = TRUE;
		}
	}
	else
	{
		nEmbeddedSerialNo = embed.GetSerialNo();
	}

	//IF WE HAVE AN EMBEDDED SERIAL NUMBER AND A SERIALNO.DAT NUMBER THEN CHECK THEY AGREE
	if ((nSerialNoFromFile != 0) && (nEmbeddedSerialNo != 0))
	{
		if (nSerialNoFromFile != nEmbeddedSerialNo)
		{
			m_strError.Format ( "Serial number does not match (P%6.6d : F%6.6d)", 
				nEmbeddedSerialNo, 
				nSerialNoFromFile);

			return FALSE;
		}
	}

	if ( nEmbeddedSerialNo != 0 )						// full version
	{
		if (CreateSystemFolders(m_nProgram) == FALSE)
		{
			return FALSE;
		}

		if ( m_RegData.Read ( strRegFilename ) == FALSE )
		{
			m_strError = CMessageTexts::RegistrationFileCorrupted();
			return FALSE;
		}

		if (bExistReg == FALSE)	// R01 (reg) may not exist yet on new system
		{
			m_RegData.SetSerialNo(nEmbeddedSerialNo);
		}

		if ( nEmbeddedSerialNo != m_RegData.GetSerialNo() )		
		{
			m_strError.Format("Serial number does not match (P%6.6d : R%6.6d)",
				nEmbeddedSerialNo,
				m_RegData.GetSerialNo());

			return FALSE;
		}

		if (LoadUpdateFile(nEmbeddedSerialNo, strUserFilename) == FALSE)
		{
			return FALSE;
		}

		if ( _access ( strUserFilename, 0 ) != 0 )
		{
			m_strError = "Cannot find user file";
			return FALSE;
		}

		if ( m_UserData.Read ( strUserFilename ) == FALSE )
		{
			m_strError = CMessageTexts::UserFileCorrupted();
			return FALSE;
		}

		if ( nEmbeddedSerialNo != m_UserData.GetSerialNo() )	
		{
			m_strError.Format("Serial number does not match (P%6.6d : U%6.6d)",
				nEmbeddedSerialNo,
				m_UserData.GetSerialNo());

			return FALSE;
		}

		if (TRUE == bWriteInternalUpdateToken)
		{
			MakeInternalUpdateToken(szVersion, strUpdateTokenFilename);
		}

		m_nVersionType = VERSION_FULL;
	}
	else						// upgrade
	{
		if (bExistUser == FALSE || bExistReg == FALSE)		// R01 or R02 not found (both must exist)
		{
			if (strRegTokenError != "")
			{
				m_strError = strRegTokenError;
			}
			else
			{
				m_strError = CMessageTexts::UnregisteredSoftware();
			}

			return FALSE;
		}

		if (m_RegData.Read(strRegFilename) == FALSE)
		{
			m_strError = CMessageTexts::RegistrationFileCorrupted();
			return FALSE;
		}

		if (LoadUpdateFile(m_RegData.GetSerialNo(), strUserFilename) == FALSE)
		{
			return FALSE;
		}

		if (m_UserData.Read(strUserFilename) == FALSE)
		{
			m_strError = CMessageTexts::UserFileCorrupted();
			return FALSE;
		}

		if (m_RegData.GetSerialNo() != m_UserData.GetSerialNo())
		{
			m_strError.Format("Serial number does not match (R%6.6d : U%6.6d)",
				m_RegData.GetSerialNo(),
				m_UserData.GetSerialNo());

			return FALSE;
		}

		if (nSerialNoFromFile != 0)
		{
			if (m_RegData.GetSerialNo() != nSerialNoFromFile)
			{
				m_strError.Format("Serial number does not match (R%6.6d : F%6.6d)",
					m_RegData.GetSerialNo(),
					nSerialNoFromFile);

				return FALSE;
			}
		}

		if (FALSE == bWaiveUpdateToken)
		{
			// FUNCTION SETS m_nVersionType TO UPGRADE OR FULL IF RETURNING TRUE
			if (ValidateUpdateToken(szVersion, szMinTokenDate, bExistUpdateToken, strUpdateTokenFilename) == FALSE)
			{
				return FALSE;
			}
		}
		else
		{
			m_nVersionType = VERSION_UPGRADE;
		}
	}

	if ( nProgram != m_UserData.GetProgramNo() )
	{
		m_strError = "Unknown program type";
		return FALSE;
	}

	if ( HasSoftwareExpired() == TRUE )
	{
		// "Software has expired (Serial No. nnnnnn)"

		CString strSerialNo;
		strSerialNo.Format ( "%6.6d", m_UserData.GetSerialNo() );

		m_strError = "Software has expired (Serial No. ";
		m_strError += strSerialNo;							// "nnnnnn"
		m_strError += ")";
		return FALSE;
	}

	CSSFile file{};
	if ( file.Open ( GetShareFilename(0), "wb" ) == TRUE )	// test that the shared folder is writable
	{
		file.Close();
		remove ( GetShareFilename(0) );	// delete S00 file
	}
	else
	{
		m_strError = "Unable to write to the Data Path";
		return FALSE;
	}

	if ( bSpawned == FALSE )					// normal program
	{
		if ( TestMaxUsers() == FALSE )		// calculates instance number
			return FALSE;

		if ( CreateTempFolder() == FALSE )		// uses instance number ( 1 to maxusers )
			return FALSE;

		return RegisterSoftware ( pParent );
	}
	else									// spawned program
	{
//		spawned programs must not call LockAllUsers() and UnlockAllUsers() functions
//		spawned programs share a common temp folder Pnn\TEMP00
//		internal: spawned programs must not call CLockArray::LockUser() function

		if ( CreateTempFolder() == FALSE )		// uses instance number = 0
			return FALSE;

		{
			bool bWantDemoToken = FALSE;
			bool bGotDemoToken = FALSE;

			CheckDemoToken(bWantDemoToken, bGotDemoToken);

			if (TRUE == bWantDemoToken)
			{
				return bGotDemoToken;
			}
		}

		return TRUE;
	}
}

/**********************************************************************/

CString CSysset::GetVersion ( const char* szVersion )
{
	CString strVersion = szVersion;

	switch ( m_nVersionType )
	{
	case VERSION_FULL:
		break;

	case VERSION_UPGRADE:
		strVersion += " (upgrade)";
		break;

	default:
		strVersion += " (downgrade)";
		break;
	}

	return strVersion;
}

/**********************************************************************/

int CSysset::GetDealerCode()
{
	CString strDealerName = m_UserData.GetUserID();
	strDealerName.MakeUpper();

	//SHARPSOFT OR DEMO
	if ((strDealerName == "SHARPSOFT") || (strDealerName.Left(4) == "DEMO"))
	{
		return 1;
	}
	//SHARP
	if (strDealerName == "SHARP")
	{
		return 2;
	}
	//ACR
	if (strDealerName == "ACR")
	{
		return 3;
	}
	//CCM
	if (strDealerName == "CCM")
	{
		return 4;
	}
	//INDEPENDENT
	if (strDealerName == "INDEPENDENT")
	{
		return 5;
	}
	//A&K
	if (strDealerName == "A&K")
	{
		return 6;
	}
	//KE
	if (strDealerName == "KE")
	{
		return 7;
	}
	//POSSOL
	if (strDealerName == "POSSOL")
	{
		return 8;
	}
	//ISLAND
	if (strDealerName == "ISLAND")
	{
		return 9;
	}
	//CUSKEN
	if (strDealerName == "CUSKEN")
	{
		return 10;
	}
	//EASYPOS
	if (strDealerName == "EASYPOS")
	{
		return 11;
	}
	//MERSEYSIDE
	if (strDealerName == "MERSEYSIDE")
	{
		return 12;
	}
	//RADLEYS
	if (strDealerName == "RADLEYS")
	{
		return 13;
	}
	//SELECT
	if (strDealerName == "SELECT")
	{
		return 14;
	}
	//RICE OFFICE
	if (strDealerName == "RICE")
	{
		return 15;
	}
	//CRS
	if (strDealerName == "CRS")
	{
		return 16;
	}
	//SBS
	if (strDealerName == "SBS")
	{
		return 17;
	}
	//NORCASH
	if (strDealerName == "NORCASH")
	{
		return 18;
	}
	//VBR
	//if (strDealerName == "")
	//{
	//	return 19;
	//}
	//CLASSIC
	if (strDealerName == "CLASSIC")
	{
		return 20;
	}
	//CRYSTAL
	if (strDealerName == "CRYSTAL")
	{
		return 21;
	}
	//BUSINESS
	if (strDealerName == "BUSINESS")
	{
		return 22;
	}
	//CRT
	if (strDealerName == "CRT")
	{
		return 23;
	}
	//VYRAV
	if (strDealerName == "VYRAV")
	{
		return 24;
	}
	//NDCR
	if (strDealerName == "NDCR")
	{
		return 25;
	}
	//CCR
	if (strDealerName == "CCR")
	{
		return 26;
	}
	//LANGLEY
	//if (strDealerName == "")
	//{
	//	return 27;
	//}
	//MATRIX
	//	if (strDealerName == "")
	//{
	//	return 28;
	//}
	//GSSYSTEMS
	if (strDealerName == "GSSYSTEMS")
	{
		return 29;
	}

	return 0;
}

/**********************************************************************/

void CSysset::MakeInternalUpdateToken(CString strVersion, CString strUpdateTokenFilename)
{
	if (_access(strUpdateTokenFilename, 0) != 0)
	{
		CUpdateTokenData updateTokenData;
		updateTokenData.SetSerialNo(m_RegData.GetSerialNo());
		updateTokenData.SetVersion(strVersion);
		updateTokenData.SetDealerCode(GetDealerCode());
		updateTokenData.SetIsUpdateFlag(0);
		updateTokenData.Write(strUpdateTokenFilename);
	}
}

/**********************************************************************/

bool CSysset::ValidateUpdateToken( CString strVersion, CString strMinTokenDate, bool bExistUpdateToken, CString strUpdateTokenFilename )
{
	int nDealerCode = GetDealerCode();

	//INITIAL CHECK FOR EXISTING VALID UPDATE TOKEN
	if (bExistUpdateToken == TRUE)
	{
		CUpdateTokenData updateTokenData;
		if (updateTokenData.Read(strUpdateTokenFilename) == TRUE)
		{
			if (updateTokenData.GetSerialNo() == m_RegData.GetSerialNo())
			{
				CString strTokenVersion = updateTokenData.GetVersion();
				if (strTokenVersion ==  strVersion)
				{
					if (nDealerCode == updateTokenData.GetDealerCode())
					{
						m_nVersionType = (updateTokenData.GetIsUpdateFlag() == 1) ? VERSION_UPGRADE : VERSION_FULL;
						return TRUE;
					}
				}
			}
		}
	}

	//IF NO EXISTING VALID INTERNAL TOKEN THEN SEE IF WE HAVE A DEALER TOKEN
	{
		int nTokenCheck = CheckDealerUpdateToken(nDealerCode, strMinTokenDate);
		
		if ( nTokenCheck != 0 )
		{
			//UNABLE TO REGISTER UPDATE
			m_strError = "Unable to register update";
			m_strError += "\n\n";

			switch (nTokenCheck)
			{
			case 1:
				m_strError += "Update token file not found";
				break;

			case 2:
				m_strError += "Unable to read update token file";
				break;

			case 3:
			case 4:
			case 5:
				m_strError += "Invalid update token file";
				break;

			case 6:
			case 7:
			case 8:
				m_strError += "Update token file expired";
				break;
			}

			switch (nTokenCheck)
			{
			case 3:
			case 6:
				m_strError += " (1)";
				break;

			case 4:
			case 7:
				m_strError += " (2)";
				break;

			case 5:
			case 8:
				m_strError += " (3)";
				break;
			}

			return FALSE;
		}
	}
	
	//CREATE NEW INTERNAL TOKEN
	{
		CUpdateTokenData updateTokenData;
		updateTokenData.SetSerialNo(m_RegData.GetSerialNo());
		updateTokenData.SetVersion(strVersion);
		updateTokenData.SetDealerCode(nDealerCode);
		updateTokenData.SetIsUpdateFlag(1);
		updateTokenData.Write(strUpdateTokenFilename);
	}
	
	//CHECK NEW INTERNAL TOKEN
	{
		CUpdateTokenData updateTokenData;
		if (updateTokenData.Read(strUpdateTokenFilename) == TRUE)
		{
			if (updateTokenData.GetSerialNo() == m_RegData.GetSerialNo())
			{
				CString strTokenVersion = updateTokenData.GetVersion();
				if (strTokenVersion == strVersion)
				{
					if (nDealerCode == updateTokenData.GetDealerCode())
					{
						m_nVersionType = VERSION_UPGRADE;
						return TRUE;
					}
				}
			}
		}
	}

	m_strError = "Unable to register update\n\nMissing or invalid instance update token";
	return FALSE;
}

/**********************************************************************/
/*	GET routines											*/
/**********************************************************************/

CString CSysset::GetField ( int n )	{ return m_UserData.GetField(n); }
CString CSysset::GetUserID()			{ return m_UserData.GetUserID(); }
int CSysset::GetSmartcardCustNo()		{ return m_UserData.GetSmartcardCustNo(); }
int CSysset::GetSystemType()			{ return m_UserData.GetSystemType(); }

int CSysset::GetMaxLocations()
{
	if ((m_nDemoTokenLocCount >= 1) && (m_nDemoTokenLocCount <= 999))
	{
		return m_nDemoTokenLocCount;
	}
	else
	{
		return m_UserData.GetMaxLocations();
	}
}

int CSysset::GetMaxRemoteLocations()	{ return m_UserData.GetMaxRemoteLocations(); }

int CSysset::GetMaxDatabases()
{
	if ((m_nDemoTokenDbCount >= 1) && (m_nDemoTokenDbCount <= 99))
	{
		return m_nDemoTokenDbCount;
	}
	else
	{
		return m_UserData.GetMaxDatabases();
	}
}

int CSysset::GetMaxStockPoints()		{ return m_UserData.GetMaxStockPoints(); }
int CSysset::GetMaxEcrModels()		{ return m_UserData.GetMaxEcrModels(); }
int CSysset::GetSerialNo()			{ return m_UserData.GetSerialNo(); }

int CSysset::GetMaxUsers()
{
	if ((m_nDemoTokenUserCount >= 1) && (m_nDemoTokenUserCount <= 10))
	{
		return m_nDemoTokenUserCount;
	}
	else
	{
		return m_UserData.GetMaxUsers();
	}
}

int CSysset::GetMaxComms()			{ return m_UserData.GetMaxComms(); }
int CSysset::GetTLogConvert()			{ return m_UserData.GetTLogConvert(); }
int CSysset::GetPMS()				{ return m_UserData.GetPMS(); }
int CSysset::GetExternalLink()		{ return m_UserData.GetExternalLink(); }
bool CSysset::IsSchedulerSystem()		{ return m_UserData.IsSchedulerSystem(); }
bool CSysset::IsStockExportSystem()	{ return m_UserData.IsStockExportSystem(); }
bool CSysset::IsSmartcardSystem()		{ return m_UserData.IsSmartcardSystem(); }
bool CSysset::IsGcr500System()		{ return m_UserData.IsGcr500System(); }
bool CSysset::IsPhotoIDSystem()		{ return m_UserData.IsPhotoIDSystem(); }
bool CSysset::IsBarmanSystem()		{ return m_UserData.IsBarmanSystem(); }
bool CSysset::IsRetailStockSystem()	{ return m_UserData.IsRetailStockSystem(); }
bool CSysset::IsWetStockSystem()		{ return m_UserData.IsWetStockSystem(); }
bool CSysset::IsSmartcardFormat()		{ return m_UserData.IsSmartcardFormat(); }
bool CSysset::IsMealDealSystem()		{ return m_UserData.IsMealDealSystem(); }

/**********************************************************************/
/*	private routines										*/
/**********************************************************************/

static CString EnvGetString(const char* szLabel)
{
	CString strValue = "";

	CString strLabel = "";
	strLabel.Format("%%%s%%", szLabel);	// add percent around label

	char buf[256];
	DWORD dwSize = sizeof(buf);
	if (::ExpandEnvironmentStrings(strLabel, buf, dwSize) > 0)
	{
		strValue = buf;

		if (strValue == strLabel)	// label not found
		{
			strValue = "";
		}
	}

	return strValue;
}

/**********************************************************************/

static bool EnvIsLocal()
{
	CString strSessionName = EnvGetString("SESSIONNAME");
	return  (strSessionName.CompareNoCase("Console") == 0) ? TRUE : FALSE;	// "Console" true = local
}

/**********************************************************************/

static void LegacyEnvGetUserDetails(CString& strHostName, CString& strClientName)
{
	strHostName = EnvGetString("COMPUTERNAME");
	strClientName = EnvGetString("CLIENTNAME");
}

/**********************************************************************/

static IdentityInfo GetIdentityInfo()
{
	IdentityInfo info;

	// --- UserName ---
	TCHAR userName[UNLEN + 1];
	DWORD size = UNLEN + 1;
	if (GetUserName(userName, &size))
	{
		info.m_strUserName = userName;
	}
	else
	{
		info.m_strUserName = ""; // optional, can be empty
	}

	// --- ComputerName (critical) ---
	TCHAR compName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD compSize = MAX_COMPUTERNAME_LENGTH + 1;
	if (GetComputerNameEx(ComputerNamePhysicalDnsHostname, compName, &compSize))
	{
		info.m_strComputerName = compName;
	}
	else if (GetComputerName(compName, &compSize))
	{
		info.m_strComputerName = compName;
	}
	else
	{
		info.m_strComputerName = ""; // must not be empty in practice
	}

	// --- SessionName (informational) ---
	TCHAR sessionName[256];
	DWORD sessSize = GetEnvironmentVariable(_T("SESSIONNAME"), sessionName, 256);
	if ((sessSize > 0) && (sessSize < 256))
	{
		info.m_strSessionName = sessionName;
	}
	else
	{
		info.m_strSessionName = ""; // safe to leave empty
	}

	// --- ClientName (informational) ---
	TCHAR clientName[256];
	DWORD clientSize = GetEnvironmentVariable(_T("CLIENTNAME"), clientName, 256);
	if ((clientSize > 0) && (clientSize < 256))
	{
		info.m_strClientName = clientName;
	}
	else
	{
		info.m_strClientName = ""; // safe to leave empty
	}

	return info;
}

/**********************************************************************/

int CSysset::GetPCIdFromComputerName(CString strComputerName, bool bLegacy)
{
	int nResult = 0;

	CString strFilename = "";
	strFilename += GetSyssetComputerPath();	// <DataPath>\Sysset\PC
	strFilename += "\\";	

	CString strMD5Seed = strComputerName;	

	if (FALSE == bLegacy)
	{
		strFilename += "N_";
		strMD5Seed.MakeUpper();
	}
	
	strFilename += CSSMD5::CalcMD5(strMD5Seed);	

	CSSFile file;
	if (file.Open(strFilename, "rb") == TRUE)
	{
		CString strBuf;
		if (file.ReadString(strBuf) == TRUE)		// PCnnnnnnnnnn,ComputerName
		{
			CCSV csv(strBuf);
			CString strPCId = csv.GetString(0);
			if ((strPCId.GetLength() == 12) && (strPCId.Left(2) == "PC"))	// PCnnnnnnnnnn
			{
				nResult = atoi(strPCId.Right(10));
			}
		}
		file.Close();
	}

	return nResult;
}

/**********************************************************************/

bool CSysset::WritePCIdFile(bool bClient)
{
	// SetPCId() is called before system folders are created (CreateSystemFolders)
	// so we need to create the folder structure to store the PCId

	CString strPath = GetSyssetPath();					// <DataPath>\Sysset
	if (MakeSubdirectory(strPath) != 0)					// create folder (if it doesn't exist)
	{
		m_strError = "Cannot create system folder";
		return FALSE;
	}

	strPath = GetSyssetComputerPath();					// <DataPath>\Sysset\PC
	if (MakeSubdirectory(strPath) != 0)					// create folder (if it doesn't exist)
	{
		m_strError = "Cannot create system folder";
		return FALSE;
	}

	CString strFilename = "";
	CString strComputerName = bClient ? m_IdentityInfo.m_strClientName : m_IdentityInfo.m_strComputerName;	

	{
		CString strMD5Seed = strComputerName;
		strMD5Seed.MakeUpper();

		strFilename += GetSyssetComputerPath();	// <DataPath>\Sysset\PC
		strFilename += "\\N_";
		strFilename += CSSMD5::CalcMD5(strMD5Seed);
	}

	CSSFile file;
	if (file.Open(strFilename, "wb") == TRUE)	// <DataPath\>Sysset\PC\xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	{
		CCSV csv;
		csv.Add(GetPCFolder(bClient ? m_nPCIdClient : m_nPCIdHost));	// PCnnnnnnnnnn
		csv.Add(strComputerName);										// ComputerName

		if (file.WriteLine(csv.GetLine()) == FALSE)	// PCnnnnnnnnnn,ComputerName
		{
			m_strError = CMessageTexts::CannotWritePCId();
			return FALSE;
		}
		file.Close();
	}
	else
	{
		m_strError = CMessageTexts::CannotWritePCId();
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

bool CSysset::SetPCId(bool& bCanCreatePCId)
{
	m_nPCIdHost = 0;
	m_nPCIdClient = 0;	
	bool bGotLegacyIds = FALSE;

	//The CanCreatePCID reference will be set to TRUE if we have a PCId
	//at the end of this function.
	bool bWantCreatePCId = bCanCreatePCId;
	bCanCreatePCId = FALSE;

	m_IdentityInfo = GetIdentityInfo();
	m_nPCIdHost = GetPCIdFromComputerName(m_IdentityInfo.m_strComputerName, FALSE);

	if ( m_IdentityInfo.m_strClientName == "" )
	{
		m_IdentityInfo.m_strClientName = m_IdentityInfo.m_strComputerName;
	}

	if ( m_IdentityInfo.m_strClientName == m_IdentityInfo.m_strComputerName )
	{
		m_nPCIdClient = m_nPCIdHost;
	}
	else
	{
		m_nPCIdClient = GetPCIdFromComputerName(m_IdentityInfo.m_strClientName, FALSE);
	}

	if ((0 == m_nPCIdClient) || (0 == m_nPCIdHost))
	{
		CString strLegacyHostName = "";
		CString strLegacyClientName = "";
		LegacyEnvGetUserDetails(strLegacyHostName, strLegacyClientName);

		if (0 == m_nPCIdHost)
		{
			m_nPCIdHost = GetPCIdFromComputerName(strLegacyHostName, TRUE);
		}

		if ( m_IdentityInfo.m_strClientName == m_IdentityInfo.m_strComputerName )
		{
			m_nPCIdClient = m_nPCIdHost;
		}
		else
		{
			if (0 == m_nPCIdClient)
			{
				m_nPCIdClient = GetPCIdFromComputerName(strLegacyClientName, TRUE);
			}
		}

		bGotLegacyIds = TRUE;
	};

	//RETURN TRUE IF GOT PCId
	if ((m_nPCIdHost != 0) && (m_nPCIdClient != 0))
	{
		if ( (TRUE == bGotLegacyIds) && (TRUE == bWantCreatePCId) )
		{
			if (WritePCIdFile(FALSE) == FALSE)	// host
			{
				bCanCreatePCId = FALSE;
				return FALSE;
			}

			if (m_IdentityInfo.m_strClientName != m_IdentityInfo.m_strComputerName)
			{
				if (WritePCIdFile(TRUE) == FALSE) // client
				{
					bCanCreatePCId = FALSE;
					return FALSE;
				}
			}
		}

		bCanCreatePCId = TRUE;
		return TRUE;
	}

	//RETURN TRUE IF DON'T NEED TO CREATE PCId
	if (FALSE == bWantCreatePCId)
	{
		bCanCreatePCId = FALSE;
		return TRUE;
	}

	//TO GET HERE, WE MUST NOT HAVE LEGACY OR NEW PCIds for HOST or CLIENT or BOTH 	
	bool bWriteHostFile = FALSE;
	bool bWriteClientFile = FALSE;

	if (0 == m_nPCIdHost)
	{
		// get current time (seconds since 1970)
		time_t lTime;
		time(&lTime);		

		// limit range to avoid problems with negative numbers
		m_nPCIdHost = (int)(lTime & 0x3fffffff);	
		bWriteHostFile = TRUE;
	}

	if ((0 == m_nPCIdClient) && (m_IdentityInfo.m_strClientName != m_IdentityInfo.m_strComputerName))
	{
		if (FALSE == bWriteHostFile)			
		{
			// get current time (seconds since 1970)
			time_t lTime;
			time(&lTime);	

			// limit range to avoid problems with negative numbers
			m_nPCIdClient = (int)(lTime & 0x3fffffff);	
		}
		else
		{
			m_nPCIdClient = m_nPCIdHost + 1;
		}

		bWriteClientFile = TRUE;
	}

	if ( m_IdentityInfo.m_strClientName == m_IdentityInfo.m_strComputerName)
	{
		m_nPCIdClient = m_nPCIdHost;
		bWriteClientFile = FALSE;
	}

	if ( TRUE == bWriteHostFile)
	{
		if (WritePCIdFile(FALSE) == FALSE)	// host pc id
		{
			bCanCreatePCId = FALSE;
			return FALSE;
		}
	}

	if ( TRUE == bWriteClientFile)
	{
		if (WritePCIdFile(TRUE) == FALSE) // client pc id
		{
			bCanCreatePCId = FALSE;
			return FALSE;
		}
	}

	bCanCreatePCId = TRUE;
	return TRUE;
}

/**********************************************************************/

bool CSysset::CreateSystemFolders ( int nProgram )
{
	m_nProgram = nProgram;

	CStringArray arrayPaths;
	arrayPaths.Add(GetProgramPath());			// <DataPath>\Pnn

	CString strHostPath = GetPCPathHost();		// <DataPath>\Pnn\PCnnnnnnnnnn	
	CString strClientPath = GetPCPathClient();	// <DataPath>\Pnn\PCnnnnnnnnnn	

	arrayPaths.Add(strHostPath);

	if ( ( strClientPath != strHostPath) && ( strClientPath != "" ) )
	{
		arrayPaths.Add(strClientPath);
	}

	arrayPaths.Add(GetSyssetPath());			// <DataPath>\Sysset
	arrayPaths.Add(GetSyssetComputerPath());	// <DataPath>\Sysset\PC
	arrayPaths.Add(GetSyssetProgramPath());		// <DataPath>\Sysset\Pnn

	strHostPath = GetSyssetPCPathHost();		// <DataPath>\Sysset\Pnn\PCnnnnnnnnnn
	strClientPath = GetSyssetPCPathClient();	// <DataPath>\Sysset\Pnn\PCnnnnnnnnnn

	arrayPaths.Add(strHostPath);

	if ( ( strClientPath != strHostPath) && ( strClientPath != "" ) )
	{
		arrayPaths.Add(strClientPath);
	}

	arrayPaths.Add(GetSyssetWindowSizePath());			// <DataPath>\Sysset\Pnn\PCnnnnnnnnnn\ws

	for ( int i = 0; i < arrayPaths.GetCount(); i++ )
	{
		CString strPath = arrayPaths.GetAt(i);
		if ( MakeSubdirectory ( strPath ) != 0 )		// create folder (if it doesn't exist)
		{
			m_strError = "Cannot create system folder";	
			return FALSE;
		}
	}

	return TRUE;
}

/**********************************************************************/

bool CSysset::CreateTempFolder()
{
	CString strPath = GetTempPath();			// <DataPath>\Pnn\Tempnn
	if ( MakeSubdirectory ( strPath ) != 0 )	// create folder (if it doesn't exist)
	{
		m_strError = "Cannot create temp folder";
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/
/*	Folders												*/
/**********************************************************************/

CString CSysset::GetProgramFolder ( int nProgram )
{
	CString strPath;
	strPath.Format ( "P%2.2d", nProgram );		// Pnn
	return strPath;
}

CString CSysset::GetPCFolder ( int nPCId )
{
	CString strPath;
	strPath.Format ( "PC%10.10d", nPCId );		// PCnnnnnnnnnn
	return strPath;
}

CString CSysset::GetTempFolder ( int nInstNo )
{
	CString strPath;
	strPath.Format ( "Temp%2.2d", nInstNo );	// Tempnn
	return strPath;
}

/**********************************************************************/
/*	Paths												*/
/**********************************************************************/

CString CSysset::GetDataPath()
{
	return m_strDataPath;		// can be "" or must include trailing backslash eg "c:\fred\"
}

CString CSysset::GetProgramPath()
{
	CString strPath;
	strPath.Format ( "%s%s", (const char*) GetDataPath(), (const char*) GetProgramFolder(m_nProgram) );		// <DataPath>\Pnn
	return strPath;
}

CString CSysset::GetPCPathHost()
{
	CString strPath;
	strPath.Format ( "%s\\%s", (const char*) GetProgramPath(), (const char*) GetPCFolder(m_nPCIdHost) );			// <DataPath>\Pnn\PCnnnnnnnnnn
	return strPath;
}

CString CSysset::GetPCPathClient()
{
	CString strPath;
	strPath.Format ( "%s\\%s", (const char*) GetProgramPath(), (const char*) GetPCFolder(m_nPCIdClient) );			// <DataPath>\Pnn\PCnnnnnnnnnn
	return strPath;
}

CString CSysset::GetTempPath()
{
	CString strPath;
	strPath.Format ( "%s\\%s", (const char*) GetProgramPath(), (const char*) GetTempFolder(m_nInstanceNo) );	// <DataPath>\Pnn\Tempnn
	return strPath;
}

/**********************************************************************/
/*	Paths (Sysset specific)									*/
/**********************************************************************/

CString CSysset::GetSyssetPath()
{
	CString strPath = "";
	strPath.Format("%s%s", (const char*)GetDataPath(), (const char*)CFilenameTexts::Sysset());		// <DataPath>\Sysset
	return strPath;
}

/**********************************************************************/

CString CSysset::GetSyssetComputerPath()
{
	CString strPath = "";
	strPath.Format ( "%s\\PC", (const char*) GetSyssetPath() );				// <DataPath>\Sysset\PC
	return strPath;
}

/**********************************************************************/

CString CSysset::GetSyssetProgramPath()
{
	CString strPath = "";
	strPath.Format("%s\\%s", (const char*)GetSyssetPath(), (const char*)GetProgramFolder(m_nProgram));	// <DataPath>\Sysset\Pnn
	return strPath;
}

/**********************************************************************/

CString CSysset::GetSyssetPCPathHost()
{
	CString strPath = "";
	strPath.Format ( "%s\\%s", (const char*) GetSyssetProgramPath(), (const char*) GetPCFolder(m_nPCIdHost) );		// <DataPath>\Sysset\Pnn\PCnnnnnnnnnn
	return strPath;
}

/**********************************************************************/

CString CSysset::GetSyssetPCPathClient()
{
	CString strPath = "";
	strPath.Format("%s\\%s", (const char*)GetSyssetProgramPath(), (const char*)GetPCFolder(m_nPCIdClient));		// <DataPath>\Sysset\Pnn\PCnnnnnnnnnn
	return strPath;
}

/**********************************************************************/

CString CSysset::GetSyssetWindowSizePath()
{
	CString strPath = "";
	strPath.Format("%s\\ws", (const char*)GetSyssetPCPathClient());			// <DataPath>\Sysset\Pnn\PCnnnnnnnnnn\ws
	return strPath;
}

/**********************************************************************/
/*	Filenames												*/
/**********************************************************************/

CString CSysset::GetShareFilename ( int nInstNo )
{
	CString strFilename = "";
	strFilename.Format ( "%s\\S%2.2d", (const char*) GetSyssetProgramPath(), nInstNo );		// <DataPath>\Sysset\Pnn\Snn
	return strFilename;
}

/**********************************************************************/

CString CSysset::GetProgUserFilename(int nInstNo)
{
	CString strFilename = "";
	strFilename.Format("%s\\PG%2.2d", (const char*)GetSyssetProgramPath(), nInstNo);		// <DataPath>\Sysset\Pnn\Snn
	return strFilename;
}

/**********************************************************************/

CString CSysset::GetCommsFilename(int nHandle)
{
	CString strFilename = "";
	strFilename.Format("%s\\C%2.2d", (const char*)GetSyssetProgramPath(), nHandle);		// <DataPath>\Sysset\Pnn\Cnn
	return strFilename;
}

/**********************************************************************/

CString CSysset::GetUserFilename()
{
	CString strFilename = "";
	strFilename.Format("%s\\%s", (const char*)GetSyssetProgramPath(), (const char*)CFilenameTexts::UserFilename());		// <DataPath>\Sysset\Pnn\R02
	return strFilename;
}

/**********************************************************************/

CString CSysset::GetRegFilename()
{
	CString strFilename = "";
	strFilename.Format("%s\\%s", (const char*)GetSyssetProgramPath(), (const char*)CFilenameTexts::RegFilename());	 // <DataPath>\Sysset\Pnn\R01
	return strFilename;
}

/**********************************************************************/

CString CSysset::GetUpdateTokenFilename()
{
	CString strFilename = "";
	strFilename.Format("%s\\%s", (const char*)GetSyssetProgramPath(), (const char*)CFilenameTexts::UpdateTokenFilename());		// <DataPath>\Sysset\Pnn\R03
	return strFilename;
}

/**********************************************************************/

bool CSysset::LoadUpdateFile ( int nSerialNo, const char* szUserFilename )
{
	CString strUpdateFilename = "";
	strUpdateFilename.Format ( CFilenameTexts::UpdateFilename(), nSerialNo );	// "u%6.6d.dat"
	if ( _access ( strUpdateFilename, 0 ) == 0 )
	{
		{
			CSSIniFile file ( SS_INI_HEX );
			if ( file.Read ( strUpdateFilename ) == TRUE )
			{
				file.Write ( szUserFilename );
			}
			else
			{
				m_strError = "Cannot copy update file";
				return FALSE;
			}
		}
		remove ( strUpdateFilename );		// delete update file after object destroyed
	}
	return TRUE;
}

/**********************************************************************/

bool CSysset::HasSoftwareExpired()
{
	CString strExpiry = m_UserData.GetExpiryDate();	// dd/mm/yyyy
	if (strExpiry.GetLength() > 0)
	{
		CSSDate dateExpiry(strExpiry);

		CSSDate dateCurrent;
		dateCurrent.SetCurrentDate();				// get today's date

		if (dateCurrent >= dateExpiry)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/

bool CSysset::TestMaxUsers()
{
	int nMaxUsers = m_UserData.GetMaxUsers();	// max users really means max instances

	for (int i = 1; i <= nMaxUsers; i++)	// try to delete in case file has been left hanging around
	{
		remove(GetShareFilename(i));		// wont be deleted if file really is still being used
	}

	for (int i = 1; i <= nMaxUsers; i++)
	{
		if (m_LockArray.LockUser(GetShareFilename(i), m_IdentityInfo, GetPCFolder(m_nPCIdHost), GetPCFolder(m_nPCIdClient)) == TRUE)
		{
			m_nInstanceNo = i;		// found lowest unused instance number
			return TRUE;
		}
	}

	m_strError = "Too many users";

	return FALSE;
}

/**********************************************************************/

int CSysset::RequestComms()
{
	int nMaxComms = m_UserData.GetMaxComms();	// max simultaneous comms sessions

	for (int i = 1; i <= nMaxComms; i++)
	{
		if (m_CommsLockArray.Lock(GetCommsFilename(i)) == TRUE)
		{
			return i;		// found lowest unused comms handle
		}
	}

	m_strError = "Too many comms sessions";

	return 0;
}

/**********************************************************************/

void CSysset::ReleaseComms(int nHandle)
{
	m_CommsLockArray.Unlock(GetCommsFilename(nHandle));
}

/**********************************************************************/

void CSysset::CreateProgUserFile(CString strUserName)
{
	CSSFile fileUser;
	if (fileUser.Open(GetShareFilename(m_nInstanceNo), "rb") == TRUE)
	{
		CString strInfo = "";
		fileUser.ReadString(strInfo);
		fileUser.Close();

		CSSFile filePG;
		if (filePG.Open(GetProgUserFilename(m_nInstanceNo), "wb") == TRUE)
		{
			filePG.WriteLine(strInfo);
			filePG.WriteLine(strUserName);
			filePG.Close();
		}
	}
}

/**********************************************************************/

//e.g. V4.23.C05 => 40231205
int CSysset::ConvertStringVersionToInt(const CString strVersion)
{ 
	// Expected format: V4.23.C05
	// Output: 40231205 (V4=4, .23=23, .C05=12 05)

	if (strVersion.GetLength() < 5)
	{
		return 0;
	}

	if (strVersion[0] != 'V')
	{
		return 0;
	}

	if (TestNumeric(strVersion.Mid(1, 1)) == FALSE)
	{
		return 0;
	}

	if (strVersion[2] != '.')
	{
		return 0;
	}

	if (TestNumeric(strVersion.Mid(3, 2)) == FALSE)
	{
		return 0;
	}

	int nMajor = atoi(strVersion.Mid(1, 1));
	int nMinor = atoi(strVersion.Mid(3, 2));

	if ( strVersion.GetLength() == 5 )
	{
		return (nMajor * 1000000) + (nMinor * 10000);
	}

	if (strVersion.GetLength() != 9)
	{
		return 0;
	}

	if (strVersion[5] != '.')
	{
		return 0;
	}

	int nPatch = 0;
	CString strPatch = strVersion.Mid(6);
	if (TestNumeric(strPatch) == TRUE)		
	{
		nPatch = atoi(strPatch);
	}
	else
	{
		char chPatchLetter = strPatch[0];
		if ((chPatchLetter < 'A') || (chPatchLetter > 'C'))
		{
			return 0;
		}

		nPatch = 1000 + ((int)(chPatchLetter - 'A') * 100); // A=1000, B=1100, C=1200

		if (TestNumeric(strPatch.Mid(1, 2)) == FALSE)
		{
			return 0;
		}

		nPatch += atoi(strPatch.Mid(1, 2));	
	}

	return (nMajor * 1000000) + (nMinor * 10000) + nPatch;
}

/**********************************************************************/

//e.g. 40231205 => V4.23.C05
CString CSysset::ConvertIntVersionToString(const int nVersion)
{
	int nMajor = (nVersion / 1000000) % 10;
	int nMinor = (nVersion / 10000) % 100;
	int nPatch = nVersion % 10000;
	
	CString strVersion = "";

	if (nPatch <= 1299)
	{
		strVersion.Format("V%1.1d.%2.2d", nMajor, nMinor);

		if (nPatch >= 1000)
		{
			int nPatchLetterIndex = (nPatch / 100) - 10; // A=0, B=1, C=2
			int nPatchNumber = nPatch % 100;
			char chPatchLetter = 'A' + nPatchLetterIndex;
			strVersion.AppendFormat(".%c%2.2d", chPatchLetter, nPatchNumber);
		}
		else if (nPatch > 0)
		{
			strVersion.AppendFormat(".%3.3d", nPatch);
		}
	}
	
	return strVersion;
}

/**********************************************************************/
