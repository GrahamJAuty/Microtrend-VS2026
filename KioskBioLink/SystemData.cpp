//*******************************************************************
#include "Cardlink.h"
#include "GlobalFunctions.h"
#include "WaitDlg.h"
//*******************************************************************
#include "SystemData.h"
//*******************************************************************
extern CSysset Sysset;
//*******************************************************************
static const char* szDEFAULT_NOTIFYPATH = "C:\\Users\\Public\\Microtrend\\KioskSmartPayLink";
static const char* szDEFAULT_IMPORTFILE = "GCVal.txt";
//*******************************************************************

CSystemData::CSystemData()
{
}

//*******************************************************************

bool CSystemData::Read()
{
	CString strPath = AddSysFilePath("");						// C:\Users\Public\Microtrend\KioskBioLink\P34\SysFiles
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	// see if need to create image folder
	m_strSysFilesImagePath = AddSysFilePath("Images");
	if (::ExistSubdirectory(m_strSysFilesImagePath) == FALSE)
	{
		::MakeSubdirectory(m_strSysFilesImagePath);
	}

	m_strCommandFilename = AddSysFilePath("File0000.dat");
	m_strSystemFilename = AddSysFilePath("File0001.dat");
	m_strPasswordFile = AddSysFilePath("File0002.dat");
	m_strGTFilename = AddSysFilePath("File0003.dat");
	m_strPayLinkLogFilename = AddSysFilePath("File0004.dat");
	m_strPayLinkTTLFilename = AddSysFilePath("File0005.dat");
	m_strOfflineLoyLogFilename = AddSysFilePath("File0006.dat");
	m_strAdminDatabaseFilename = AddSysFilePath("File0007.dat");
	m_strPayLinkServiceFilename = AddSysFilePath("File0008.dat");
	m_strPayLinkLevelFilename = AddSysFilePath("File0009.dat");
	m_strReaderFilename = AddSysFilePath("File0011.dat");
	m_strGTExportFilename = AddSysFilePath("File0012.dat");
	m_strPendingLogFilename = AddSysFilePath("File0101.dat");

	CSSIniFile file(SS_INI_AES);
	file.Read(m_strSystemFilename);
	m_nInterfaceType = file.GetInt("Interface", nINTERFACE_BIOMETRIC);
	m_bShowPhotoID = file.GetBool("ShowPhotoID", m_bShowPhotoID);
	m_bShowPoints = file.GetBool("ShowPoints", m_bShowPoints);
	m_bShowGroup = file.GetBool("ShowGroup", m_bShowGroup);
	m_bShowAvailableSpend = file.GetBool("ShowAS", m_bShowAvailableSpend);
	m_bDisableAlerts = file.GetBool("DisableAlerts", m_bDisableAlerts);
	m_bHideAccountID = file.GetBool("HideAccountID", m_bHideAccountID);
	m_bEnableKeypadEntry = file.GetBool("KeypadEntry", m_bEnableKeypadEntry);
	m_bEnrolmentAllowed = file.GetBool("Enrolment", m_bEnrolmentAllowed);
	m_strEnrolmentPrompt = file.GetString("EnrolPrompt", m_strEnrolmentPrompt);
	m_bSystemPasswordReqd = file.GetBool("PasswordReqd", m_bSystemPasswordReqd);
	m_nTerminalNo = file.GetInt("TerminalNo", m_nTerminalNo);
	m_lPaylinkSerialNo = file.GetLong("PaylinkSerialNo", m_lPaylinkSerialNo);
	m_nRefreshInterval = file.GetInt("RefreshInterval", m_nRefreshInterval);
	m_nEnquiryTimeout = file.GetInt("EnquiryTimeout", m_nEnquiryTimeout);
	m_strLoyReqFolder = file.GetString("LoyReqFolder", m_strLoyReqFolder);
	m_strLoyLogFolder = file.GetString("LoyLogFolder", m_strLoyLogFolder);
	m_strNotifyPath = file.GetString("NotifyPath", szDEFAULT_NOTIFYPATH);
	m_strImportFile = file.GetString("ImportFile", szDEFAULT_IMPORTFILE);
	m_strCardNoText = file.GetString("CardNoText", m_strCardNoText);
	m_bShowDateTime = file.GetBool("ShowDateTime", m_bShowDateTime);
	m_bTopupPurse1 = file.GetBool("TopupPurse1", m_bTopupPurse1);
	m_strPurse1Text = file.GetString("Purse1Text", m_strPurse1Text);
	m_bTopupPurse3 = file.GetBool("TopupPurse3", m_bTopupPurse3);
	m_strPurse3Text = file.GetString("Purse3Text", m_strPurse3Text);
	m_bNoSmallCoinWarning = file.GetBool("NoSmallCoinWarning", m_bNoSmallCoinWarning);
	m_bShowUserIDCancel = file.GetBool("ShowUserIDCancel", m_bShowUserIDCancel);
	m_bActualPINMode = file.GetBool("ActualPINMode", m_bActualPINMode);

	m_strLowCardNo = "1";
	m_strHighCardNo = (m_nInterfaceType == nINTERFACE_MIFAREv1) ? "9999999999" : "999999";
	m_strBlankCardNo = (m_nInterfaceType == nINTERFACE_MIFAREv1) ? "0000000000" : "000000";

	if ( nINTERFACE_PINPAD == m_nInterfaceType )
	{
		m_bEnableKeypadEntry = TRUE;						
	}

	CreateFilenames();									// create system filenames required

	m_timeNextCheck = COleDateTime::GetCurrentTime();	// next time for biometric refresh

	return ::FileExists(m_strSystemFilename);
}

//*******************************************************************
// C:\Users\Public\Microtrend\KioskBioLink\P34\SysFiles{\filename}

CString CSystemData::AddSysFilePath(const char* szFilename)
{
	CString strFilename = "";

	if (lstrlen(szFilename) == 0)
	{
		strFilename.Format("%s\\SysFiles",
			(const char*)Sysset.GetProgramPath());
	}
	else
	{
		strFilename.Format("%s\\SysFiles\\%s",
			(const char*)Sysset.GetProgramPath(),
			szFilename);
	}

	return strFilename;
}

//*******************************************************************

void CSystemData::CreateFilenames()
{
	m_strUserIDFilename.Format("%s\\%s",
		(const char*)m_strNotifyPath,
		(const char*)m_strImportFile);

	m_strLoyReqTempFilename.Format("%s\\LOYBIO-%3.3d.TMP",
		(const char*)m_strLoyReqFolder,
		m_nTerminalNo);

	m_strLoyReqFilename.Format("%s\\LOYBIO-%3.3d.REQ",
		(const char*)m_strLoyReqFolder,
		m_nTerminalNo);

	m_strLoyReqReplyFilename.Format("%s\\LOYBIO-%3.3d.RSP",
		(const char*)m_strLoyReqFolder,
		m_nTerminalNo);

	m_strLoyLogTransFilename.Format("%s\\LOYBIO-%3.3d.LOG",
		(const char*)m_strLoyLogFolder,
		m_nTerminalNo);

	m_strLoyLogReplyFilename.Format("%s\\LOYBIO-%3.3d.RSP",
		(const char*)m_strLoyLogFolder,
		m_nTerminalNo);
}

//*******************************************************************

void CSystemData::Write()
{
	CSSIniFile file(SS_INI_AES);

	file.Set("Interface", m_nInterfaceType);
	file.Set("ShowPhotoID", m_bShowPhotoID);
	file.Set("ShowPoints", m_bShowPoints);
	file.Set("ShowGroup", m_bShowGroup);
	file.Set("ShowAS", m_bShowAvailableSpend);
	file.Set("DisableAlerts", m_bDisableAlerts);
	file.Set("HideAccountID", m_bHideAccountID);
	file.Set("KeypadEntry", GetEnableKeypadEntryFlag());
	file.Set("Enrolment", m_bEnrolmentAllowed);
	file.Set("EnrolPrompt", m_strEnrolmentPrompt);
	file.Set("PasswordReqd", m_bSystemPasswordReqd);
	file.Set("TerminalNo", m_nTerminalNo);
	file.Set("PaylinkSerialNo", m_lPaylinkSerialNo);
	file.Set("RefreshInterval", m_nRefreshInterval);
	file.Set("EnquiryTimeout", m_nEnquiryTimeout);
	file.Set("LoyReqFolder", m_strLoyReqFolder);
	file.Set("LoyLogFolder", m_strLoyLogFolder);
	file.Set("NotifyPath", m_strNotifyPath);
	file.Set("ImportFile", m_strImportFile);
	file.Set("CardNoText", m_strCardNoText);
	file.Set("ShowDateTime", m_bShowDateTime);
	file.Set("TopupPurse1", m_bTopupPurse1);
	file.Set("Purse1Text", m_strPurse1Text);
	file.Set("TopupPurse3", m_bTopupPurse3);
	file.Set("Purse3Text", m_strPurse3Text);
	file.Set("NoSmallCoinWarning", m_bNoSmallCoinWarning);
	file.Set("ShowUserIDCancel", m_bShowUserIDCancel);
	file.Set("ActualPINMode", m_bActualPINMode);

	file.Write(m_strSystemFilename);

	CreateFilenames();									// re-create system filenames in case Folder names changed
}

//*******************************************************************

bool CSystemData::RefreshBiometrics(int nRetries)
{
	return RefreshBiometrics(nRetries, "");
}

//*******************************************************************

bool CSystemData::RefreshBiometrics(const char* szNotifyPath)
{
	return RefreshBiometrics(5, szNotifyPath);
}

//*******************************************************************

bool CSystemData::RefreshBiometrics(int nRetries, const char* szNotifyPath)
{
	CString strPath = szNotifyPath;

	if (strPath == "")								// may be passed by SetupDlg
	{
		strPath = m_strNotifyPath;
	}

	CString strFilename = "";
	strFilename.Format("%s\\GcValRefresh.txt",
		(const char*)strPath);

	CSSFile file;
	bool bReply = file.Open(strFilename, "wb");			// try create empty file
	file.Close();

	if (bReply == FALSE)
	{
		return FALSE;
	}

	for (int nCount = 0; nCount < nRetries; nCount++)
	{
		Sleep(500);											// wait a bit

		if (::FileExists(strFilename) == FALSE)		// file gone - biometrics must be working
		{
			return TRUE;
		}
	}

	::SendToRecycleBin(strFilename);
	return FALSE;
}

//*******************************************************************

bool CSystemData::AutoRefreshBiometrics(int nRetries, CWnd* pParent)
{
	bool bReply = TRUE;
	if (COleDateTime::GetCurrentTime() >= m_timeNextCheck)
	{
		CWaitDlg waitDlg("Refreshing biometric database", "Please wait", pParent);
		waitDlg.PumpMessages();

		bReply = RefreshBiometrics(nRetries);
		m_timeNextCheck = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, System.m_nRefreshInterval, 0);
	}

	return bReply;
}

//*******************************************************************

void CSystemData::SetPaylinkSerialNo(long lSerialNo)
{
	if (lSerialNo >= 0)
	{
		m_lPaylinkSerialNo = lSerialNo;
	}
}

//*******************************************************************

int CSystemData::GetUserIDLength()
{
	return m_strHighCardNo.GetLength();
}

//*******************************************************************

__int64  CSystemData::GetMaxCardNo()
{
	return _atoi64(m_strHighCardNo);
}

//*******************************************************************

CString CSystemData::FormatCardNo(__int64 nCardNo, bool bPadOut)
{
	CString strCardNo = "";
	strCardNo.Format("%I64d", nCardNo);

	if (TRUE == bPadOut)
	{
		::AddLeading(strCardNo, GetUserIDLength(), '0');
	}

	return strCardNo;
}

//*******************************************************************

CString CSystemData::FormatCardNo(const char* szCardNo, bool bPadOut)
{
	CString strCardNo = szCardNo;

	if (TRUE == bPadOut)
	{
		::AddLeading(strCardNo, GetUserIDLength(), '0');
	}

	return strCardNo;
}

//*******************************************************************

bool CSystemData::GetEnableKeypadEntryFlag()
{
	if (nINTERFACE_PINPAD == m_nInterfaceType)
	{
		return TRUE;
	}
	else
	{
		return m_bEnableKeypadEntry;
	}
}

//*******************************************************************
