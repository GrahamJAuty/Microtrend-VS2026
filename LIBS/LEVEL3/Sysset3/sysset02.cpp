/**********************************************************************/

#include "SyssetDlg1.h"
#include "SyssetDlg2.h"
#include "SyssetDlg4.h"
#include "local.h"

/**********************************************************************/

#define SYSTEM_EXTEND_DAYS	5			// no of days extension given by secret access code

/**********************************************************************/

void CSysset::DisplayUserList()
{
	CSyssetDlg4 dlg;
	dlg.m_pSysset = this;
	dlg.DoModal();
}

/**********************************************************************/

CString CSysset::DisplayLicenceDetails ( const char* szGroup, const char* szText, int nCodeLen, CWnd* pParent )
{
	CString strCode;

	CString strTitle = "Licence Details";
	CString strComment1 = "If your company details shown above are incorrect";
	CString strComment2 = "Contact SPOS UK who will issue an authorised version.";	
	
	CSyssetDlg1 dlg ( this, "", strTitle, szGroup, szText, strComment1, strComment2, nCodeLen, pParent );

	dlg.m_strUserName = GetField(0);
	dlg.m_strUserAdd1 = GetField(1);
	dlg.m_strUserAdd2 = GetField(2);
	dlg.m_strUserAdd3 = GetField(3);
	dlg.m_strUserAdd4 = GetField(4);
	dlg.m_strUserAdd5 = GetField(5);

	dlg.DoModal();

	strCode = dlg.m_strCode;

	if ( dlg.m_bSecretMode == TRUE )
	{
		CString strTemp = dlg.m_strCode;
		strTemp.MakeLower();

		if ( strTemp == PasswordTexts::Dealer() )
		{
			if (SetDealerExpiry(pParent) == FALSE)
			{
				AfxMessageBox(m_strError);
			}

			strCode = "";
		}
		else if (strTemp == PasswordTexts::Update())
		{
			CString strError = UpdateOnline(CSecurityCode::GetCode(GetSerialNo()), GetUserFilename());

			if (strError == "")
			{
				AfxMessageBox("Your update will take effect\nwhen you restart this program.");
			}
			else
			{
				AfxMessageBox(strError);
			}
		}
	}

	return strCode;
}

/**********************************************************************/

static int xlat[10][10] =
{
	11,2,9,6,1,13,14,3,5,10,		// reg no starting with 0
	2,13,1,9,6,11,10,14,3,5,		// reg no starting with 1
	13,1,6,2,11,9,5,10,14,3,		// reg no starting with 2
	9,2,11,6,13,1,3,5,10,14,		// reg no starting with 3
	6,9,13,11,1,2,14,3,5,10,		// reg no starting with 4
	11,6,2,1,9,13,10,14,3,5,		// reg no starting with 5
	1,11,9,13,2,6,5,10,14,3,		// reg no starting with 6
	13,1,6,9,2,11,3,5,10,14,		// reg no starting with 7
	9,13,1,2,11,6,14,3,5,10,		// reg no starting with 8
	2,11,13,1,6,9,10,14,3,5,		// reg no starting with 9
};

static char AddMod10 ( char cDigit, int nOffset )
{
	int n = (int) ( cDigit - '0' ) + nOffset;
	if ( n >= 10 )
		n -= 10;

	return ( n +'0' );
}

static char Checksum ( const char* szBuf, int nLen )
{
	int nChecksum = 0;

	for ( int i = 0 ; i < nLen ; i++ )
		nChecksum += ( szBuf[i] - '0' );

	return (( nChecksum % 10 ) + '0' );
}

/**********************************************************************/

static CString GetRandom ( int nSerialNo, int nProgram, int nEnableNo )
{
	CString strRandom;

	strRandom = "0000000000000000";		// 16 digits

	srand ( ::GetTickCount() );

	strRandom.SetAt (  0, (char) ( rand() % 10 ) + '0' );	// random 1
	strRandom.SetAt (  4, (char) ( rand() % 10 ) + '0' );	// random 2
	strRandom.SetAt (  8, (char) ( rand() % 10 ) + '0' );	// random 3

	CString strCode;
	strCode.Format ( "%6.6d%2.2d%2.2d", nSerialNo, nProgram, nEnableNo );

	int r1 = strRandom[0] - '0';		// random 1
	int r2 = strRandom[4] - '0';		// random 2
	int r3 = strRandom[8] - '0';		// random 3

	for ( int i = 0 ; i < 10 ; i++ )
	{
		strCode.SetAt ( i, AddMod10 ( strCode[i], i ) );		// add position

		if (( i & 1 ) == 0 )							// even position
			strCode.SetAt ( i, AddMod10 ( strCode[i], r2 ) );	// add random 2
		else											// odd position
			strCode.SetAt ( i, AddMod10 ( strCode[i], r3 ) );	// add random 3

		strRandom.SetAt ( xlat[r1][i], strCode[i] );			// reorder using random 1
	}

	strRandom.SetAt (  7, Checksum(strRandom, 7) );	// check digit 1
	strRandom.SetAt ( 12, Checksum(strRandom,12) );	// check digit 2
	strRandom.SetAt ( 15, Checksum(strRandom,15) );	// check digit 3

	return strRandom;
}

/**********************************************************************/

static BOOL TestRandomSystem ( const char* szRandom, const char* szCode )
{
	if ( strlen(szRandom) == 16 && strlen(szCode) == 8 )
	{
		CString strAccessCode = szRandom;
		CString strMD5 = CSSMD5::CalcMD5(strAccessCode).MakeUpper();
		CString strUserCode;

		for ( int i = 0 ; i < 8 ; i++ )
		{
			char c = strMD5.GetAt(i);
			int j = ( c >= 'A' && c <= 'F' ) ? c-0x37 : c-0x30;
			c = char ( j % 10 ) + '0';
			strUserCode += c;
		}

		if ( strcmp ( strUserCode, szCode ) == 0 )
			return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

static int TestRandomDealer ( const char* szRandom, const char* szCode )
{
	if ( strlen(szRandom) == 16 && strlen(szCode) == 8 )
	{
		CString strAccessCode = szRandom;
		CString strMD5 = CSSMD5::CalcMD5(strAccessCode).MakeUpper();
		CString strUserCode;

		for ( int i = 0 ; i < 8 ; i++ )
		{
			char c = strMD5.GetAt(i);
			int j = ( c >= 'A' && c <= 'F' ) ? c-0x37 : c-0x30;
			c = char ( j % 10 ) + '0';
			strUserCode += c;
		}

		if ( strUserCode[0] != szCode[0] )	return -1;
		if ( strUserCode[1] != szCode[1] )	return -1;
		if ( strUserCode[2] != szCode[2] )	return -1;
		if ( strUserCode[3] != szCode[3] )	return -1;

		if ( strUserCode[5] != szCode[5] )	return -1;
		if ( strUserCode[6] != szCode[6] )	return -1;
		if ( strUserCode[7] != szCode[7] )	return -1;

		return szCode[4] - '0';			// 5th digit of code
	}

	return -1;
}

/**********************************************************************/

bool CSysset::RegisterSoftware ( CWnd* pParent )
{
	int nDaysRemaining;

	CString strExpiryDate = m_RegData.GetExpiryDate();

	if ( strExpiryDate == "000000" )		// software has been registered
	{
		return TRUE;
	}
	else if ( strExpiryDate == "999999" )	// software has not been registered
	{
		nDaysRemaining = 999999;
	}
	else								// expiry date has been set
	{
		int nDay = atoi ( strExpiryDate.Mid(0,2) );
		int nMonth = atoi ( strExpiryDate.Mid(2,2) );
		int nYear = atoi ( strExpiryDate.Mid(4,2) ) + 2000;

		COleDateTime tNow = COleDateTime::GetCurrentTime();
		COleDateTime tExpiry ( nYear, nMonth, nDay, tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond() );

		COleDateTimeSpan ts = tExpiry - tNow;
		nDaysRemaining = ts.GetDays();
	}

	CString strTitle = "Software Registration";
	CString strGroup = "Access Code";
	CString strText = "";

	CString strComment1;
	CString strComment2;

	if ( nDaysRemaining == 1 )
	{
		strComment1 = "You have 1 day remaining to register your software";
	}
	else if ( nDaysRemaining >= 2 && nDaysRemaining <= 7 )
	{
		strComment1.Format ( "You have %d days remaining before your software licence expires", nDaysRemaining );
	}
	else
	{
		strComment1 = "Your software has not been registered";
	}

	int nSyssetEnable;

	if ( m_RegData.GetExpiryType() == SYSSET_EXPIRY_DEALER )	// dealer expiry date
	{
		nSyssetEnable = SYSSET_ENABLE_DEALER_EXPIRY;

		CString strPhone = m_RegData.GetDealerPhone();
		strPhone.TrimLeft ( ' ' );
		strPhone.TrimRight ( ' ' );

		if ( strPhone == "" )	// does not contain any digits
		{
			strComment2 = CMessageTexts::ContactDealerAccessCode();			
		}
		else
		{
			strComment2 = "Contact your Dealer on ";
			strComment2 += strPhone;
			strComment2 += " quoting the access code";
		}
	}
	else												// system expiry date
	{
		nSyssetEnable = SYSSET_ENABLE_PROGRAM;
		strComment2 = CMessageTexts::ContactDealerAccessCode();
	}

	//CHECK FOR SPECIAL DEALER DEMO TOKEN
	{
		bool bWantDemoToken = FALSE;
		bool bGotDemoToken = FALSE;

		CheckDemoToken(bWantDemoToken, bGotDemoToken);

		if (TRUE == bWantDemoToken)
		{
			return bGotDemoToken;
		}
	}

	CString strRandom = GetRandom ( GetSerialNo(), m_nProgram, nSyssetEnable );

	int nLoopCount = 3;			// allow 3 attempts with the same access code

	for (int i = 0; i < nLoopCount; i++)
	{
		CSyssetDlg1 dlg(this, strRandom, strTitle, strGroup, strText, strComment1, strComment2, 10, pParent);

		dlg.m_strUserName = GetField(0);
		dlg.m_strUserAdd1 = GetField(1);
		dlg.m_strUserAdd2 = GetField(2);
		dlg.m_strUserAdd3 = GetField(3);
		dlg.m_strUserAdd4 = GetField(4);
		dlg.m_strUserAdd5 = GetField(5);

		dlg.DoModal();

		if (dlg.m_bSecretMode == TRUE)
		{
			CString strTemp = dlg.m_strCode;
			strTemp.MakeLower();

			if (strTemp == PasswordTexts::Dealer())
			{
				if (nSyssetEnable == SYSSET_ENABLE_PROGRAM)
				{
					if (nDaysRemaining == 999999)	// dont allow secret access if expiry date set
					{
						m_RegData.SetSystemExpiry(SYSTEM_EXTEND_DAYS);	// now + 5 days
						if (m_RegData.Write() == TRUE)
						{
							AfxMessageBox("The registration period has been extended by 5 days");
							return TRUE;
						}
						m_strError = CMessageTexts::CannotWriteRegFile();
						return FALSE;
					}
				}

				return SetDealerExpiry(pParent);	// dealer expiry dialog
			}
		}

		CString strCode = "";

		for (int n = 0; n < dlg.m_strCode.GetLength(); n++)	// remove spaces
		{
			char c = dlg.m_strCode.GetAt(n);

			if (c != ' ')
				strCode += c;
		}

		if (nSyssetEnable == SYSSET_ENABLE_PROGRAM)
		{
			if (strCode == "")
			{
				// nDaysRemaining can never be greater than 5 unless someone changed the
				// date on the computer to the future before using the secret access code
				// to add 5 days

				if (nDaysRemaining > 0 && nDaysRemaining <= SYSTEM_EXTEND_DAYS)
				{
					return TRUE;
				}
				else
				{
					m_strError = CMessageTexts::SoftwareNotRegistered();
					return FALSE;
				}
			}
			else
			{
				if (TestRandomSystem(strRandom, strCode) == TRUE)
				{
					m_RegData.SetSystemExpiry("000000");	// system has been registered
					if (m_RegData.Write() == TRUE)
					{
						return TRUE;
					}
					else
					{
						m_strError = CMessageTexts::CannotWriteRegFile();
						return FALSE;
					}
				}

				m_strError = "Incorrect Code";
			}
		}
		else if (nSyssetEnable == SYSSET_ENABLE_DEALER_EXPIRY)
		{
			if (strCode == "")
			{
				if (nDaysRemaining <= 0 || nDaysRemaining == 999999)
				{
					m_strError = CMessageTexts::SoftwareNotRegistered();
					return FALSE;
				}
				else
				{
					return TRUE;
				}
			}
			else
			{
				bool bWriteRegistry = TRUE;

				switch (TestRandomDealer(strRandom, strCode))
				{
				case 0:
					m_RegData.SetDealerExpiry("000000");
					break;	// system has been registered
				
				case 1:	
					m_RegData.SetDealerExpiry(7);
					break;	// extend 7 days

				case 2:
					m_RegData.SetDealerExpiry(14);
					break;	// extend 14 days

				case 3:	
					m_RegData.SetDealerExpiry(30);
					break;	// extend 30 days

				default:	
					m_strError = "Incorrect Code";
					bWriteRegistry = FALSE;
					break;
				}

				if (bWriteRegistry == TRUE)
				{
					if (m_RegData.Write() == TRUE)
					{
						return TRUE;
					}
					else
					{
						m_strError = CMessageTexts::CannotWriteRegFile();
						return FALSE;
					}
				}
			}
		}
		else
		{
			m_strError = "Unknown enable";
			return FALSE;
		}

		if (i < (nLoopCount - 1))			// dont want 2 message boxes
		{
			AfxMessageBox(m_strError);
		}
	}

	m_strError = "Too many attempts";

	return FALSE;
}

/**********************************************************************/

void CSysset::CheckDemoToken(bool& bWantDemoToken, bool& bGotDemoToken)
{
	bWantDemoToken = FALSE;
	bGotDemoToken = FALSE;

	int nSerialNo = m_UserData.GetSerialNo();
	if (((GetExternalLink() & 1024) != 1024) || (nSerialNo < 9001) || (nSerialNo > 9099))
	{
		return;
	}

	bWantDemoToken = TRUE;

	CSSIniFile fileToken(SS_INI_AES);

	// .\\DEMOTOKEN_*.DAT
	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile(".\\DEMOTOKEN_*.DAT");

	CString strTokenFilename = "";

	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if ((FileFinder.IsDots() == FALSE) && (FileFinder.IsDirectory() == FALSE))
		{
			strTokenFilename = FileFinder.GetFileName();
			break;
		}
	}

	if ((strTokenFilename == "") || (_access(strTokenFilename, 0) != 0))
	{
		//Token file not found
		m_strError = "Token file not found";
		m_UserData.SetField(4, "");
		return;
	}

	if (fileToken.Read(strTokenFilename) == FALSE)
	{
		//Unable to read token file
		m_strError = "Unable to read token file";
		m_UserData.SetField(4, "");
		return;
	}

	//Invalid data in token file<space>
	CString strInvalid = "Invalid data in token file ";

	CString strDealer = fileToken.GetString("F05", "");
	if (strDealer.GetLength() < 11)
	{
		//(1)
		m_strError = strInvalid + " (01)";
		m_UserData.SetField(4, "");
		return;
	}

	CString strNobble = fileToken.GetString("F07", "");
	if (strNobble.GetLength() != 11)
	{
		//(7)
		m_strError = strInvalid + " (02)";
		m_UserData.SetField(4, "");
		return;
	}

	CString strExpire = fileToken.GetString("F11", "");
	if (strExpire.GetLength() != 16)
	{
		//(2)
		m_strError = strInvalid + " (03)";
		m_UserData.SetField(4, "");
		return;
	}

	CString strVersion = fileToken.GetString("F14", "");
	if (strVersion.GetLength() != 13)
	{
		//(3)
		m_strError = strInvalid + " (04)";
		m_UserData.SetField(4, "");
		return;
	}

	CString strCustom = fileToken.GetString("F17", "");
	if (strCustom.GetLength() > 10)
	{
		CCSV csvCustom(strCustom.Mid(10));
		for (int n = 0; n < csvCustom.GetSize(); n++)
		{
			CString strField = csvCustom.GetString(n);
			strField.MakeUpper();
			if (strField.Left(4) == "LOC=")
			{
				int n = atoi(strField.Mid(4));
				if ((n >= 1) && (n <= 999))
				{
					m_nDemoTokenLocCount = n;
				}
			}
			else if (strField.Left(3) == "DB=")
			{
				int n = atoi(strField.Mid(3));
				if ((n >= 1) && (n <= 99))
				{
					m_nDemoTokenDbCount = n;
				}
			}
			else if (strField.Left(5) == "USER=")
			{
				int n = atoi(strField.Mid(5));
				if ((n >= 1) && (n <= 10))
				{
					m_nDemoTokenUserCount = n;
				}
			}
		}
	}

	CString strPrograms = fileToken.GetString("F19", "");
	if (strPrograms.GetLength() < 12)
	{
		//(4)
		m_strError = strInvalid + " (05)";
		m_UserData.SetField(4, "");
		return;
	}

	CString strDate = strExpire.Right(6);
	int nYear = atoi(strDate.Left(4));
	int nMonth = atoi(strDate.Right(2));

	COleDateTime timeExpire = COleDateTime(nYear, nMonth, 1, 0, 0, 0);

	if (timeExpire.GetStatus() != COleDateTime::valid)
	{
		//(5)
		m_strError = strInvalid + " (06)";
		m_UserData.SetField(4, "");
		return;
	}

	if (COleDateTime::GetCurrentTime() + COleDateTimeSpan(250, 0, 0, 0) < timeExpire)
	{
		//(6)
		m_strError = strInvalid + " (07)";
		m_UserData.SetField(4, "");
		return;
	}

	if (COleDateTime::GetCurrentTime() > timeExpire)
	{
		//Token has expired
		m_strError = "Token has expired";
		m_UserData.SetField(4, "");
		return;
	}

	strVersion = strVersion.Right(3);
	if (atoi(strVersion) > 1)
	{
		//Token requires newer software version
		m_strError = "Token requires newer software version";
		m_UserData.SetField(4, "");
		return;
	}

	bool bFoundProgramNo = FALSE;
	{
		CCSV csv(strPrograms.Right(strPrograms.GetLength() - 10));
		for (int n = 0; n < csv.GetSize(); n++)
		{
			if (csv.GetInt(n) == m_UserData.GetProgramNo())
			{
				bFoundProgramNo = TRUE;
				break;
			}
		}
	}

	if (FALSE == bFoundProgramNo)
	{
		//Token does not grant access to this program
		m_strError = "Token does not grant access to this program";
		m_UserData.SetField(4, "");
		return;
	}

	m_UserData.SetField(0, strDealer.Right(strDealer.GetLength() - 10));

	// Expires 01
	CString strPrefix = "Expires 01";

	CString strDateField;
	strDateField.Format("%s/%s/%s",
		(const char*)strPrefix,
		(const char*)strDate.Right(2),
		(const char*)strDate.Left(4));

	m_UserData.SetField(4, strDateField);

	m_bNobbledDemo = (strNobble.Right(1) == "1");

	bGotDemoToken = TRUE;
}

/**********************************************************************/

void CSysset::CheckRegToken(int& nEmbeddedSerialNo, CString strPath, CString strVersion, CString& strRegTokenError)
{
	nEmbeddedSerialNo = 0;
	strRegTokenError = "";

	CString strFilenamePattern = "";
	strFilenamePattern.Format("%sREGTOKEN_%2.2d_*.DAT", 
		(const char*)strPath,
		m_nProgram);
			
	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile(strFilenamePattern);
	
	CString strTokenFilename = "";
	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if ((FileFinder.IsDots() == FALSE) && (FileFinder.IsDirectory() == FALSE))
		{
			strTokenFilename = FileFinder.GetFileName();
			break;
		}
	}

	strTokenFilename.MakeUpper();

	//NO TOKEN FILE = UNCOMMITTED EXE WITH NO REPLY TEXT
	if ( strTokenFilename == "" )
	{
		return;
	}

	//INVALID FILENAME = UNCOMMITTED EXE WITH NO RE{PLY TEXT
	if ( strTokenFilename.GetLength() != 31 ) 
	{
		return;
	}

	if ( TestNumeric(strTokenFilename.Mid(12,6)) == false )	
	{
		return;
	}

	if (strTokenFilename.Mid(18, 1) != "_")
	{
		return;
	}

	if (TestNumeric(strTokenFilename.Mid(19, 8)) == FALSE)
	{
		return;
	}

	if ( strTokenFilename.Right(4) != ".DAT")
	{
		return;
	}

	CString strTokenFullPath = strPath + strTokenFilename;

	if (_access(strTokenFullPath, 0) != 0)
	{
		PrepareRegTokenGeneralError(strRegTokenError, strTokenFilename, "Unable to access registration token file.");
		return;
	}

	CSSIniFile fileToken(SS_INI_AES);
	if (fileToken.Read(strTokenFullPath) == FALSE)
	{
		PrepareRegTokenGeneralError(strRegTokenError, strTokenFilename, "Unable to read registration token file.");
		return;
	}

	//PROGRAM NO
	CString strProgramNo = fileToken.GetString("F07", "");
	if (strProgramNo.GetLength() != 12)
	{
		PrepareRegTokenInvalidDataError(strRegTokenError, strTokenFilename, 1);
		return;
	}

	if ( m_nProgram != atoi(strProgramNo.Right(2)) )
	{
		PrepareRegTokenInvalidDataError(strRegTokenError, strTokenFilename, 2);
		return;
	}

	if ( strTokenFilename.Mid(9,2) != strProgramNo.Right(2) )
	{
		PrepareRegTokenInvalidDataError(strRegTokenError, strTokenFilename, 3);
		return;
	}

	//SERIAL NO
	CString strSerialNo = fileToken.GetString("F03", "");
	if (strSerialNo.GetLength() != 16)
	{
		PrepareRegTokenInvalidDataError(strRegTokenError, strTokenFilename, 4);
		return;
	}

	if (strSerialNo.Right(6) != strTokenFilename.Mid(12, 6))
	{
		PrepareRegTokenInvalidDataError(strRegTokenError, strTokenFilename, 5);
		return;
	}

	int nSerialNo = atoi(strSerialNo.Right(6));

	if (0 == nSerialNo)
	{
		PrepareRegTokenInvalidDataError(strRegTokenError, strTokenFilename, 6);
		return;
	}

	//DATE
	CString strDate = fileToken.GetString("F15", "");
	if (strDate.GetLength() != 18)
	{
		PrepareRegTokenInvalidDataError(strRegTokenError, strTokenFilename, 7);	
		return;
	}

	if (strDate.Right(8) != strTokenFilename.Mid(19, 8))
	{
		PrepareRegTokenInvalidDataError(strRegTokenError, strTokenFilename, 8);
		return;
	}

	COleDateTime timeToken(
		atoi(strDate.Mid(10, 4)),
		atoi(strDate.Mid(14, 2)),
		atoi(strDate.Mid(16, 2)),
		0, 0, 0);

	if (timeToken.m_status != COleDateTime::valid )
	{
		PrepareRegTokenInvalidDataError(strRegTokenError, strTokenFilename, 9);
		return;
	}

	//VERSION
	CString strTokenVersion = fileToken.GetString("F10", "");
	if ( strTokenVersion.GetLength() < 11 )
	{
		PrepareRegTokenInvalidDataError(strRegTokenError, strTokenFilename, 10);
		return;
	}

	strTokenVersion = strTokenVersion.Mid(10);
	if ( strTokenVersion != strVersion )
	{
		PrepareRegTokenInvalidDataError(strRegTokenError, strTokenFilename, 11);
		nEmbeddedSerialNo = 0;
	}
	else if ( timeToken + COleDateTimeSpan(92, 0, 0, 0) < COleDateTime::GetCurrentTime() )
	{
		//NO ERROR TEXT FOR EXPIRED TOKEN
		nEmbeddedSerialNo = 0;
	}
	else
	{
		nEmbeddedSerialNo = nSerialNo;
	}
}

/**********************************************************************/

void CSysset::PrepareRegTokenInvalidDataError(CString& strError, CString strTokenFilename, int nErrorNum)
{
	strError.Format("Invalid data in registration token file (%d)\n\n%s", 
		nErrorNum,
		(const char*)strTokenFilename);
}

/**********************************************************************/

void CSysset::PrepareRegTokenGeneralError(CString& strError, CString strTokenFilename, const CString strReason)
{
	strError.Format("%s\n\n%s",
		(const char*)strReason,
		(const char*)strTokenFilename);
}

/**********************************************************************/

bool CSysset::TestNumeric(const CString str)
{
	for (int n = 0; n < str.GetLength(); n++)
	{
		char c = str.GetAt(n);
		if ((c < '0') || (c > '9'))
		{
			return false;
		}
	}
	return true;
}

/**********************************************************************/

bool CSysset::MayHaveLegacyEmbeddedSerialNo()
{
	switch (m_nProgram)
	{
	case SYSSET_KIOSKBIOLINK:
	case SYSSET_ECRMANSPOSV4:
	case SYSSET_STKMANSPOSV4:
	case SYSSET_LOYALTY3:
	case SYSSET_LOYALTY4:
	case SYSSET_SMARTPAY2:
	case SYSSET_SMARTPAY4:
		return false;

	default:
		return true;
	}
}

/**********************************************************************/

int CSysset::CheckDealerUpdateToken(int nDealerCode, const char* szMinTokenDate)
{
	CSSIniFile fileToken(SS_INI_AES);

	// .\\DEMOTOKEN_*.DAT
	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile(".\\UPDATETOKEN_*.DAT");

	CString strTokenFilename = "";

	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if ((FileFinder.IsDots() == FALSE) && (FileFinder.IsDirectory() == FALSE))
		{
			strTokenFilename = FileFinder.GetFileName();
			break;
		}
	}

	if ((strTokenFilename == "") || (_access(strTokenFilename, 0) != 0))
	{
		return 1;
	}

	if (fileToken.Read(strTokenFilename) == FALSE)
	{
		return 2;
	}

	{
		CString strTokenDealerCode = fileToken.GetString("F03", "");
		if (strTokenDealerCode.GetLength() < 11)
		{
			return 3;
		}

		int nTokenDealerCode = atoi(strTokenDealerCode.Mid(10));
		if (nTokenDealerCode != nDealerCode)
		{
			return 4;
		}
	}

	{
		CString strCheckDate = fileToken.GetString("F12", "");
		if (strCheckDate.GetLength() != 18)
		{
			return 5;
		}

		CString strTokenDate = strCheckDate.Right(8);
		CString strMinTokenDate = szMinTokenDate;

		if (strTokenDate < strMinTokenDate)
		{
			return 6;
		}

		COleDateTime dateNow = COleDateTime::GetCurrentTime();
		CString strDateNow = dateNow.Format("%Y%m%d");

		if (strTokenDate > strDateNow)
		{
			return 7;
		}

		COleDateTime dateTwoWeeksAgo = dateNow - COleDateTimeSpan(14, 0, 0, 0);
		CString strDateTwoWeeksAgo = dateTwoWeeksAgo.Format("%Y%m%d");

		if (strTokenDate < strDateTwoWeeksAgo)
		{
			return 8;
		}
	}

	return 0;
}

/**********************************************************************/

static int TestExpiryDate ( CString& strExpiry )		// ddmmyy
{
	CString strDate = "20";			// convert to yyyymmdd
	strDate += strExpiry.Mid(4,2);
	strDate += strExpiry.Mid(2,2);
	strDate += strExpiry.Mid(0,2);

	CString strNow = COleDateTime::GetCurrentTime().Format ( "%Y%m%d" );	// yyyymmdd

	return strcmp ( strDate, strNow );		// 0 (today), >0 (future), <0 (past)
}

/**********************************************************************/

bool CSysset::SetDealerExpiry ( CWnd* pParent )
{
	CString strExpiry = m_RegData.GetExpiryDate();

	if ( strExpiry == "000000" || m_RegData.GetExpiryType() == SYSSET_EXPIRY_DEALER )	// system has been registered or is dealer expiry
	{
		if ( strExpiry != "999999" )		// 999999 should never occur
		{
			CString strOrigExpiry = strExpiry;
			CString strOrigPhone = m_RegData.GetDealerPhone();

			if ( strExpiry == "000000" )	// software has been registered
				strExpiry = COleDateTime::GetCurrentTime().Format ( "%d%m%y" ); // today (ddmmyy)

			CSyssetDlg2 dlg ( pParent );

			dlg.m_strExpiry = strExpiry;
			dlg.m_strDealerPhone = strOrigPhone;

			if ( dlg.DoModal() == IDOK )
			{
				CString strPhone = dlg.m_strDealerPhone;
				strPhone.TrimLeft ( ' ' );
				strPhone.TrimRight ( ' ' );
				bool bWriteRegistry = FALSE;

				if ( strPhone != strOrigPhone )		// phone number has changed
				{
					m_RegData.SetDealerPhone ( strPhone );
					bWriteRegistry = TRUE;
				}
				int nCondition = TestExpiryDate ( dlg.m_strExpiry );

				if ( nCondition >= 0 )				// today or future date
				{
					if (nCondition == 0)			// today
					{
						strExpiry = "000000";		// clear expiry
					}
					else
					{
						// future date (ddmmyy)
						strExpiry = dlg.m_strExpiry;	
					}

					if ( strExpiry != strOrigExpiry )	// expiry has changed
					{
						m_RegData.SetDealerExpiry ( strExpiry );
						bWriteRegistry = TRUE;
					}
					else
					{
						return TRUE;
					}
				}

				if ( bWriteRegistry == TRUE )
				{
					if (m_RegData.Write() == TRUE)
					{
						return TRUE;
					}
					else
					{
						m_strError = CMessageTexts::CannotWriteRegFile();	
						return FALSE;
					}
				}
			}
			else			// cancel
			{
				if (strOrigExpiry == "000000")		// software has been registered
				{
					return TRUE;
				}

				if (TestExpiryDate(strExpiry) > 0)	// future date
				{
					return TRUE;
				}
			}
		}
	}

	m_strError = CMessageTexts::SoftwareNotRegistered();

	return FALSE;
}

/**********************************************************************/
