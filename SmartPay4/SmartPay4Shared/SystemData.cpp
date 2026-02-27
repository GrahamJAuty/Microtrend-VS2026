//*******************************************************************
#include "SQLTable_Account\SQLRepositoryAccount.h"
#include "SQLSchema\SQLNamespaces.h"
//*******************************************************************
#include "MembershipDatabase.h"
//*******************************************************************
#include "SystemData.h"
//*******************************************************************
const char* szNOSERVERACCESS = "The system is currently in the process of being backed up!\n\nPlease try later.";
//*******************************************************************

CSystemData::CSystemData()
{
	m_nUserLockCount = 0;
	m_nSystemLockCount = 0;
	m_bSystemForegroundEnabled = FALSE;
	m_pSystemForegroundWnd = NULL;

	//GENERAL
	m_nInterfaceType = nINTERFACE_BIOMETRIC;
	m_nAutoDeletePCtrlType = 0;
	m_bAssumeBioConsent = FALSE;
	m_bAllowPINNumbers = FALSE;
	m_bAllowGroupSets = FALSE;
	m_bUsePasswords = FALSE;
	m_bEnableBackground = FALSE;
	m_nAutoShutdownType = 0;
	m_bAllowGroupShiftDateRange = FALSE;
	RestoreDefaultGroupShiftDurations();
	m_bAllowGroupShiftOnDuty = FALSE;
	//REPORTING
	m_nStartOfWeek = 1;					// 1=Sun, 7=Sat
	m_bEnablePhotoIDList = FALSE;
	m_bKeepLeaversDatabase = TRUE;
	m_strEcrmanPath = "";
	m_bShowPurchaseTexts = FALSE;
	m_nBioConsentTickNo = 0;
	
	for (int n = 0; n < REPORT_ENDTEXT_COUNT; n++)
	{
		m_strReportEndText[n] = "";
	}
		
	//EDITING
	m_bShowTopupButton = TRUE;
	m_bFileEditPurse1 = TRUE;
	m_bFileEditPurse2 = TRUE;
	m_bEnablePurse3 = FALSE;
	m_bFileEditPurse3 = FALSE;
	m_bFileEnablePoints = FALSE;
	m_bFileEditPoints = TRUE;
	m_bFileShowPointsButton = FALSE;
	m_bExternalAccount = FALSE;
	m_bEnableBatchUpdates = FALSE;
	m_bEnableNoAudit = FALSE;
	m_nDefaultAccountEditPage = 0;
	m_bEnableAccountColour = TRUE;
	m_bAccountCancelPrompt = FALSE;
	m_bAuditImportFilter = FALSE;

	//EXPORT
	m_bEnableExportSales = FALSE;				// export of sales \ Topsup transactions to csv
	m_bExportOnExit = FALSE;
	m_bExportOnDelete = FALSE;
	//INTERFACE
	m_nWebPaymentType = nWEBPAYMENT_NONE;
	m_bEnableChartwells = FALSE;
	m_bEnableSims = FALSE;
	m_bEnableAutoImport = FALSE;
	m_bEnableWonde = FALSE;
	m_bImportExtRef1TrimZero = FALSE;
	//EPOS
	m_bExportEposPhoto = FALSE;
	m_strEposPhotoIDFolder = "c:\\vpro\\commsin\\now";
	//NON UI
	m_bCreditOptions = FALSE;
	m_bAuditFileFilter = FALSE;

	m_nDPValue = 2;					// not saved
	
	m_bGotPCId = FALSE;

	m_bDoneSQLFileSize = FALSE;
	m_bDoneSQLShrinkLog = FALSE;
}

//*******************************************************************

void CSystemData::RestoreDefaultGroupShiftDurations()
{
	m_strGroupShiftDurations = "7:14:21:28";
}

//*******************************************************************

bool CSystemData::Read()
{
//** create folders if required

	CString strPath;
	strPath.Format ( "%s\\SysFiles", (const char*) Sysset.GetProgramPath() );
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	strPath.Format ( "%s\\SysFiles\\Logs", (const char*) Sysset.GetProgramPath() );
	if ( ::ExistSubdirectory ( strPath ) == FALSE )
	{
		::MakeSubdirectory ( strPath );							// check for old log files
		MoveLogFiles ( strPath );								// ..\Sysfiles\?filelog-yyyymmdd.dat -> ..\Sysfiles\Logs\yyyy\?filelog-yyyymmdd.dat
	}

	strPath.Format ( "%s\\PhotoID", (const char*) Sysset.GetProgramPath() );
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	strPath.Format ( "%s\\EOD", (const char*) Sysset.GetProgramPath() );
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	strPath.Format ( "%s\\Imports", (const char*) Sysset.GetProgramPath() );
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	strPath.Format ( "%s\\Imports\\PhotoID", (const char*) Sysset.GetProgramPath() );
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	strPath.Format ( "%s\\Exports", (const char*) Sysset.GetProgramPath() );
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	strPath.Format ( "%s\\Exports\\PhotoID", (const char*) Sysset.GetProgramPath() );
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	strPath.Format ( "%s\\History", (const char*) Sysset.GetProgramPath() );
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	return Rewind();
}

//*******************************************************************

bool CSystemData::Rewind()
{
	bool bReply = FALSE;

	if (::FileExists(Filenames.GetSystemFilenameNew()) == TRUE)
	{
		CSSIniFile file(SS_INI_NORMAL);
		bReply = file.Read(Filenames.GetSystemFilenameNew());
		RewindInternal(file);
	}
	else
	{
		CSSIniFile file(SS_INI_AES);
		bReply = file.Read(Filenames.GetSystemFilenameOld());
		RewindInternal(file);
	}

	return bReply;
}

//*******************************************************************

void CSystemData::RewindInternal(CSSIniFile& file)
{
	//GENERAL
	SetInterfaceType(file.GetInt(SystemDataFields::Interface, nINTERFACE_BIOMETRIC));
	SetAutoDeletePCtrlType(file.GetInt(SystemDataFields::AutoDeletePCtrlType, 0));
	SetAssumeBioConsentFlag(file.GetInt(SystemDataFields::AssumeBioConsent, m_bAssumeBioConsent));
	SetAllowPINNumbersFlag(file.GetBool(SystemDataFields::AllowPINNumbers, m_bAllowPINNumbers));
	SetAllowGroupSetsFlag(file.GetBool(SystemDataFields::GroupSets, m_bAllowGroupSets));
	SetUsePasswordsFlag(file.GetBool(SystemDataFields::Passwords, m_bUsePasswords));
	SetEnableBackgroundFlag(file.GetBool(SystemDataFields::Background, m_bEnableBackground));
	SetAutoShutdownType(file.GetInt(SystemDataFields::AST, 0));
	SetAllowGroupShiftDateRangeFlag(file.GetBool(SystemDataFields::AllowGroupShiftDateRange, m_bAllowGroupShiftDateRange));
	SetGroupShiftDurations(file.GetString(SystemDataFields::GroupShiftDurations, m_strGroupShiftDurations));
	SetAllowGroupShiftOnDutyFlag(file.GetBool(SystemDataFields::AllowGroupShiftOnDuty, m_bAllowGroupShiftOnDuty));
	//REPORTING
	SetStartOfWeek(file.GetInt(SystemDataFields::StartOfWeek, m_nStartOfWeek));
	SetEnablePhotoIDListFlag(file.GetBool(SystemDataFields::PhotoIDList, m_bEnablePhotoIDList));
	SetKeepLeaversDatabaseFlag(file.GetBool(SystemDataFields::LeaversDbase, m_bKeepLeaversDatabase));
	SetEcrmanPath(file.GetString(SystemDataFields::EcrmanPath, m_strEcrmanPath));
	SetShowPurchaseTextsFlag(file.GetBool(SystemDataFields::PurchaseTexts, m_bShowPurchaseTexts));
	SetBioConsentTickNo(file.GetInt(SystemDataFields::BioConsentTickNo, 0));

	for (int n = 0; n < REPORT_ENDTEXT_COUNT; n++)
	{
		SetReportEndText(n, file.GetString(GetReportEndTextLabel(n + 1), m_strReportEndText[n]));
	}

	//EDITING
	SetShowTopupButtonFlag(file.GetBool(SystemDataFields::CashButton, m_bShowTopupButton));
	SetFileEditPurse1Flag(file.GetBool(SystemDataFields::Purse1, m_bFileEditPurse1));
	SetFileEditPurse2Flag(file.GetBool(SystemDataFields::Purse2, m_bFileEditPurse2));
	SetEnablePurse3Flag(file.GetBool(SystemDataFields::Purse3Required, m_bEnablePurse3));
	SetFileEditPurse3Flag(file.GetBool(SystemDataFields::Purse3, m_bFileEditPurse3));
	SetFileEnablePointsFlag(file.GetBool(SystemDataFields::PointsRequired, m_bFileEnablePoints));
	SetFileEditPointsFlag(file.GetBool(SystemDataFields::Points, m_bFileEditPoints));
	SetFileShowPointsButtonFlag(file.GetBool(SystemDataFields::PointsButton, m_bFileShowPointsButton));
	SetExternalAccountFlag(file.GetBool(SystemDataFields::ExternalAccount, FALSE));
	SetEnableBatchUpdatesFlag(file.GetBool(SystemDataFields::BatchUpdates, m_bEnableBatchUpdates));
	SetEnableNoAuditFlag(file.GetBool(SystemDataFields::NoAudit, m_bEnableNoAudit));
	SetDefaultAccountEditPage(file.GetInt(SystemDataFields::EditPage, 0));
	SetEnableAccountColourFlag(file.GetBool(SystemDataFields::EnableAccountColour, m_bEnableAccountColour));
	SetAccountCancelPromptFlag(file.GetBool(SystemDataFields::AccountCancelPrompt, m_bAccountCancelPrompt));
	SetAuditImportFilterFlag(file.GetBool(SystemDataFields::AuditImportFilter, m_bAuditImportFilter));
	//EXPORT
	SetEnableExportSalesFlag(file.GetBool(SystemDataFields::ExportSales, m_bEnableExportSales));
	SetExportOnExitFlag(file.GetBool(SystemDataFields::ExportOnExit, m_bExportOnExit));
	SetExportOnDeleteFlag(file.GetBool(SystemDataFields::ExportOnDelete, m_bExportOnDelete));
	//INTERFACE
	SetWebPaymentType(file.GetInt(SystemDataFields::WebPayment, m_nWebPaymentType));
	SetEnableChartwellsFlag(file.GetBool(SystemDataFields::Chartwells, m_bEnableChartwells));
	SetEnableSimsFlag(file.GetBool(SystemDataFields::EnableSims, m_bEnableSims));
	SetEnableAutoImportFlag(file.GetBool(SystemDataFields::AutoImport, m_bEnableAutoImport));

	{
		int n = file.GetInt(SystemDataFields::EnableWonde, -1);

		switch (n)
		{
		case 0:
			SetEnableWondeFlag(FALSE);
			break;

		case 1:
			SetEnableWondeFlag(TRUE);
			break;

		default:
			SetEnableWondeFlag(file.GetBool(SystemDataFields::EnableGroupcall, m_bEnableWonde));
			break;
		}
	}

	SetImportExtRef1TrimZeroFlag(file.GetBool(SystemDataFields::ImportExtRef1TrimZero, m_bImportExtRef1TrimZero));

	//EPOS
	SetEposPhotoIDFolder(file.GetString(SystemDataFields::EposPhotoFolder, m_strEposPhotoIDFolder));
	SetExportEposPhotoFlag(file.GetBool(SystemDataFields::EposPhotoExport, m_bExportEposPhoto));

	m_strLowCardNo = "1";
	m_strHighCardNo = (m_nInterfaceType == nINTERFACE_MIFAREv1) ? "9999999999" : "999999";
	m_strBlankCardNo = (m_nInterfaceType == nINTERFACE_MIFAREv1) ? "0000000000" : "000000";

	//NON UI
	SetCreditOptionsFlag(file.GetBool(SystemDataFields::CreditOptions, m_bCreditOptions));
	SetAuditFileFilterFlag(file.GetBool(SystemDataFields::AuditFileFilter, m_bAuditFileFilter));

	// normally gets auto created when system first registered
	// but not if run via a shortcut across a network
	{
		CString strPath = Sysset.GetPCPathHost();
		if (::ExistSubdirectory(strPath) == FALSE)
		{
			// create {datapath}\P46\pcnnnnnnnnnn
			::MakeSubdirectory(strPath);
		}
	}

	{
		CString strPath = Sysset.GetPCPathClient();

		if (strPath != Sysset.GetPCPathHost())
		{
			if (::ExistSubdirectory(strPath) == FALSE)
			{
				::MakeSubdirectory(strPath);
			}
		}
	}
}

//*******************************************************************

CString CSystemData::GetReportEndTextLabel( int n )
{
	CString strLabel = "";
	strLabel.Format( "%s%d", 
		SystemDataFields::ReportEndText,
		n );

	return strLabel;
}

//*******************************************************************

CString CSystemData::GetReportEndText( int n )
{
	CString strText = "";
	if ( ( n >= 1 ) && ( n <= REPORT_ENDTEXT_COUNT ) )
	{
		strText = m_strReportEndText[n-1];
	}
	return strText;
}

//*******************************************************************

void CSystemData::SetReportEndText( int n, CString str )
{
	if ( ( n >= 1 ) && ( n <= REPORT_ENDTEXT_COUNT ) )
	{
		m_strReportEndText[n-1] = str;
	}
}

//*******************************************************************

void CSystemData::SetDefaultAccountEditPage(int n)
{
	if ((n >= 0) && (n <= 1))
	{
		m_nDefaultAccountEditPage = n;
	}
}

//*******************************************************************

void CSystemData::SetBioConsentTickNo(int n)
{
	if ((n >= 0) && (n <= 4))
	{
		m_nBioConsentTickNo = n;
	}
}

//*******************************************************************

void CSystemData::SetAutoDeletePCtrlType(int n)
{
	if ((n >= 0) && (n <= 8))
	{
		m_nAutoDeletePCtrlType = n;
	}
}

//*******************************************************************

bool CSystemData::Write()
{
#ifndef SYSTEMTYPE_EMAILBATCHSEND

	if (m_bEnableExportSales == TRUE)
	{
		CString strPath = System.GetWebPaymentPath("", nWEBPAYMENT_EXPORTSALES);	// creates folder if not found
		if (::ExistSubdirectory(strPath) == FALSE)
		{
			::MakeSubdirectory(strPath);
		}
	}

#endif

	CSSIniFile file(SS_INI_NORMAL);

	//GENERAL
	file.Set(SystemDataFields::Interface, m_nInterfaceType);
	file.Set(SystemDataFields::AutoDeletePCtrlType, m_nAutoDeletePCtrlType);
	file.Set(SystemDataFields::AssumeBioConsent, m_bAssumeBioConsent);
	file.Set(SystemDataFields::AllowPINNumbers, m_bAllowPINNumbers);
	file.Set(SystemDataFields::GroupSets, m_bAllowGroupSets);
	file.Set(SystemDataFields::Passwords, m_bUsePasswords);
	file.Set(SystemDataFields::Background, m_bEnableBackground);
	file.Set(SystemDataFields::AST, m_nAutoShutdownType);
	file.Set(SystemDataFields::AllowGroupShiftDateRange, m_bAllowGroupShiftDateRange);
	file.Set(SystemDataFields::GroupShiftDurations, m_strGroupShiftDurations);
	file.Set(SystemDataFields::AllowGroupShiftOnDuty, m_bAllowGroupShiftOnDuty);
	//REPORTING
	file.Set(SystemDataFields::StartOfWeek, m_nStartOfWeek);
	file.Set(SystemDataFields::PhotoIDList, m_bEnablePhotoIDList);
	file.Set(SystemDataFields::LeaversDbase, m_bKeepLeaversDatabase);
	file.Set(SystemDataFields::EcrmanPath, m_strEcrmanPath);
	file.Set(SystemDataFields::PurchaseTexts, m_bShowPurchaseTexts);
	file.Set(SystemDataFields::BioConsentTickNo, m_nBioConsentTickNo);
	
	for (int n = 0; n < REPORT_ENDTEXT_COUNT; n++)
	{
		file.Set(GetReportEndTextLabel(n + 1), m_strReportEndText[n]);
	}

	//EDITING
	file.Set(SystemDataFields::CashButton, m_bShowTopupButton);
	file.Set(SystemDataFields::Purse1, m_bFileEditPurse1);
	file.Set(SystemDataFields::Purse2, m_bFileEditPurse2);
	file.Set(SystemDataFields::Purse3Required, m_bEnablePurse3);
	file.Set(SystemDataFields::Purse3, m_bFileEditPurse3);
	file.Set(SystemDataFields::PointsRequired, m_bFileEnablePoints);
	file.Set(SystemDataFields::Points, m_bFileEditPoints);
	file.Set(SystemDataFields::PointsButton, m_bFileShowPointsButton);
	file.Set(SystemDataFields::ExternalAccount, m_bExternalAccount);
	file.Set(SystemDataFields::BatchUpdates, m_bEnableBatchUpdates);
	file.Set(SystemDataFields::NoAudit, m_bEnableNoAudit);
	file.Set(SystemDataFields::EditPage, m_nDefaultAccountEditPage);
	file.Set(SystemDataFields::EnableAccountColour, m_bEnableAccountColour);
	file.Set(SystemDataFields::AccountCancelPrompt, m_bAccountCancelPrompt);
	file.Set(SystemDataFields::AuditImportFilter, m_bAuditImportFilter);
	//EXPORT
	file.Set(SystemDataFields::ExportSales, m_bEnableExportSales);
	file.Set(SystemDataFields::ExportOnExit, m_bExportOnExit);
	file.Set(SystemDataFields::ExportOnDelete, m_bExportOnDelete);
	//INTERFACE
	file.Set(SystemDataFields::WebPayment, m_nWebPaymentType);
	file.Set(SystemDataFields::Chartwells, m_bEnableChartwells);
	file.Set(SystemDataFields::EnableSims, m_bEnableSims);
	file.Set(SystemDataFields::AutoImport, m_bEnableAutoImport);
	file.Set(SystemDataFields::EnableWonde, m_bEnableWonde);
	file.Set(SystemDataFields::ImportExtRef1TrimZero, m_bImportExtRef1TrimZero);
	//EPOS
	file.Set(SystemDataFields::EposPhotoFolder, m_strEposPhotoIDFolder);
	file.Set(SystemDataFields::EposPhotoExport, m_bExportEposPhoto);
	//NON UI
	file.Set(SystemDataFields::CreditOptions, m_bCreditOptions);
	file.Set(SystemDataFields::AuditFileFilter, m_bAuditFileFilter);

	return file.Write(Filenames.GetSystemFilenameNew());
}

//*******************************************************************

int CSystemData::GetUserIDLength()		
{	
	return m_strHighCardNo.GetLength();	
}

//*******************************************************************

__int64  CSystemData::GetMaxCardNo()	
{	
	return _atoi64 ( m_strHighCardNo );	
} 

//*******************************************************************

CString CSystemData::FormatCardNo ( __int64 nCardNo, bool bPadOut  )
{
	CString strCardNo = FormatInt64Value ( nCardNo );

	if (bPadOut == TRUE)
	{
		::AddLeading(strCardNo, GetUserIDLength(), '0');
	}
	
	return strCardNo;
}

//*************************************************************************************

CString CSystemData::FormatCardNo ( const char* szCardNo, bool bPadOut )
{
	CString strCardNo = szCardNo;

	if (bPadOut == TRUE)
	{
		::AddLeading(strCardNo, GetUserIDLength(), '0');
	}

	return strCardNo;
}

//*************************************************************************************

bool CSystemData::IsValidCardNo ( const char* szCardNo )
{
	return ::IsStringNumeric ( szCardNo, 1, GetUserIDLength() );
}

//*******************************************************************

CString CSystemData::GetWebPaymentDisplayName ( int nWebPaymentType )
{
	CString strName = "Unknown";

	if (nWebPaymentType == nWEBPAYMENT_NONE)
	{
		nWebPaymentType = m_nWebPaymentType;
	}

	switch ( nWebPaymentType )
	{
	case nWEBPAYMENT_PARENTPAY:		strName = "ParentPay";		break;
	case nWEBPAYMENT_TUCASI:		strName = "Tucasi";			break;
	case nWEBPAYMENT_WISEPAY:		strName = "WisePay";		break;
	case nWEBPAYMENT_PARENTMAIL:	strName = "ParentMail";		break;
	case nWEBPAYMENT_SCHOOLCOMMS:	strName = "Schoolcomms";	break;
	case nWEBPAYMENT_SQUID:			strName = "sQuid";			break;
	case nWEBPAYMENT_BUCKINGHAM:	strName = "Buckingham";		break;
	case nWEBPAYMENT_TUCASIv2:		strName = "Tucasi";			break;
	case nWEBPAYMENT_BROMCOM:		strName = "BromCom";		break;
	case nWEBPAYMENT_PEBBLE:		strName = "Pebble";			break;
	}
	return strName;
}

//*************************************************************************************
#ifndef SYSTEMTYPE_EMAILBATCHSEND
//*************************************************************************************

CString CSystemData::GetWebPaymentPath ( const char* szFilename, int nWebPaymentType )
{
	CString strFilename = Filenames.GetWebPaymentPath ( szFilename, nWebPaymentType );
	return strFilename;
}

//*************************************************************************************

CString CSystemData::TranslateMiFareNo(const char* szMiFareNo, int nMiFareDbNo)
{
	CString strLocateOn = (nMiFareDbNo == 2) ? Account::ExternalRef2.Label : Account::ExternalRef1.Label;

	CSQLRepositoryAccount RepoAccount;
	__int64 nUserID = RepoAccount.LookupIndex(strLocateOn, szMiFareNo, NULL).GetSQLResult();

	CString strUserID = "";

	if (nUserID != 0)
	{
		strUserID = FormatInt64Value(nUserID);
	}

	return strUserID;
}

//*******************************************************************

void CSystemData::ValidateGroupShiftDurations(CString strIn, CString& strOut, CString& strError)
{
	strOut = "";
	strError = "";

	CReportConsolidationArray<CSortedIntItem> arrayDurations;

	CCSV csvIn(strIn, ':');

	for (int n = 0; n < csvIn.GetSize(); n++)
	{
		CString str = csvIn.GetString(n);

		if (str == "")
		{
			continue;
		}

		if (::TestNumeric(str) == FALSE)
		{
			strError = "Please enter only numbers, separated by colons.";
			return;
		}

		str.TrimLeft("0");
		bool bRangeFail = ((str.GetLength() < 1) || (str.GetLength() > 3));

		int nDuration = 0;
		if (FALSE == bRangeFail)
		{
			nDuration = atoi(str);
			bRangeFail = (nDuration > MAX_GROUPSHIFT_DURATION);
		}
		
		if (TRUE == bRangeFail)
		{
			strError.Format( "Group shift durations must be from 1 to %d days.",
				MAX_GROUPSHIFT_DURATION);

			return;
		}

		CSortedIntItem item;
		item.m_nItem = atoi(str);
		arrayDurations.Consolidate(item);

		if (arrayDurations.GetSize() > 10)
		{
			strError = "Please specify a maximum of 10 shift durations, separated by colons.";
			return;
		}
	}

	CCSV csvOut(':');
	for (int n = 0; n < arrayDurations.GetSize(); n++)
	{
		CSortedIntItem item;
		arrayDurations.GetAt(n, item);
		csvOut.Add(item.m_nItem);
	}

	strOut = csvOut.GetLine();
}

//*************************************************************************************
#endif
//*************************************************************************************

bool CSystemData::IsMiFareID ( const char* szUserID )
{
	bool bReply = FALSE;

	if ( m_nInterfaceType == nINTERFACE_MIFAREv2 || System.GetInterfaceType() == nINTERFACE_DUAL )				// UserID = MiFare card number
	{
		CString strUserID = szUserID;
		if ( strUserID.GetLength() > 6 )						// if 6 digit or less iID = AccountID
			bReply = TRUE;
	}
	return bReply;
}

//*************************************************************************************

CString CSystemData::GetEcrmanPath()
{
	CString strPath = m_strEcrmanPath;

	if (strPath == "")
	{
		int nProgramNo = 0;

		if (::FileExists("SPosEcrManager.exe") == TRUE)
		{
			nProgramNo = SYSSET_ECRMANSPOSV4;
		}
		else if (::FileExists("SPosStockManager.exe") == TRUE)
		{
			nProgramNo = SYSSET_STKMANSPOSV4;
		}
		else if (::FileExists("Ecrx500v3.exe") == TRUE)
		{
			nProgramNo = SYSSET_ECRMANX500V3;
		}
		else if (::FileExists("Stkx500v3.exe") == TRUE)
		{
			nProgramNo = SYSSET_STKMANX500V3;	// "Pnn"
		}

		if (nProgramNo != 0)
		{
			strPath.Format("%s%s", (const char*)Sysset.GetDataPath(), (const char*)Sysset.GetProgramFolder(nProgramNo));
		}
	}

	return strPath;
}

//*******************************************************************

CString CSystemData::GetEposTaxRateFilename()
{
	CString strPath = "";
	strPath.Format ( "%s\\Db001\\File2017.dat", (const char*) GetEcrmanPath() );
	return strPath;
}

//*******************************************************************

CString CSystemData::GetEposPaymentTypeFilename()
{
	CString strPath = "";
	strPath.Format ( "%s\\Db001\\smfile01.dat", (const char*) GetEcrmanPath() );
	return strPath;
}

//*******************************************************************

CString CSystemData::GetEposPaymentGroupFilename()
{
	CString strPath = "";
	strPath.Format ( "%s\\Db001\\smfile02.dat", (const char*) GetEcrmanPath() );
	return strPath;
}

//*******************************************************************

CString CSystemData::GetEposAllergyTextsFilename()
{
	CString strPath = "";
	strPath.Format ( "%s\\Db001\\smfile03.dat", (const char*) GetEcrmanPath() );
	return strPath;
}

//*******************************************************************
//  move 	// ..\Sysfiles\sfilelog-yyyymmdd.dat -> ..\Sysfiles\Logs\yyyy\sfilelog-yyyymmdd.dat
//  move 	// ..\Sysfiles\pfilelog-yyyymmdd.dat -> ..\Sysfiles\Logs\yyyy\pfilelog-yyyymmdd.dat

void CSystemData::MoveLogFiles ( const char* szDestPath )
{
	CString strMask;
	strMask.Format ( "%s\\SysFiles\\?filelog-????????.dat", (const char*) Sysset.GetProgramPath() );

	CFileFind FileFinder;	
	BOOL bWorking = FileFinder.FindFile ( strMask );

	while ( bWorking )   
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		CString strFilename = FileFinder.GetFileName();
		CString strYear = strFilename.Mid ( 9, 4 );						// yyyy

		if ( ::IsStringNumeric ( strYear ) == TRUE )					// check have a year
		{
			CString strYearPath;
			strYearPath.Format ( "%s\\%s", szDestPath, (const char*) strYear );

			if (::ExistSubdirectory(strYearPath) == FALSE)
			{
				::MakeSubdirectory(strYearPath);
			}

			CString strSource = FileFinder.GetFilePath();
			CString strDestination;	strDestination.Format ( "%s\\%s", (const char*) strYearPath, (const char*) strFilename );

			::MoveFile ( strSource, strDestination );
		}
	}
}

//*******************************************************************

void CSystemData::SetAutoShutdownType(int n)
{
	if ((n >= 0) && (n <= 6))
	{
		m_nAutoShutdownType = n;
	}
}

//*******************************************************************

bool CSystemData::LockAllUsers()
{
	bool bResult = FALSE;

	if (m_nUserLockCount > 0)
	{
		m_nUserLockCount++;
		bResult = TRUE;
	}
	else if (Sysset.LockAllUsers() == TRUE)
	{
		m_nUserLockCount = 1;
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CSystemData::UnlockAllUsers()
{
	if (m_nUserLockCount > 1)
	{
		m_nUserLockCount--;
	}
	else
	{
		Sysset.UnlockAllUsers();
		m_nUserLockCount = 0;
	}
}

/**********************************************************************/

void CSystemData::SystemLock()
{
	if (m_nSystemLockCount == 0)
	{
#ifndef _DEBUG
		m_bSystemForegroundEnabled = FALSE;
		m_pSystemForegroundWnd = CWnd::GetSafeOwner(CWnd::GetForegroundWindow());
		
		if (m_pSystemForegroundWnd != NULL)
		{
			m_bSystemForegroundEnabled = (m_pSystemForegroundWnd->IsWindowEnabled() != 0);
			m_pSystemForegroundWnd->EnableWindow(FALSE);
		}
#endif
	}

	m_nSystemLockCount++;
}

/**********************************************************************/

void CSystemData::SystemUnlock()
{
	if (m_nSystemLockCount == 0)
	{
		return;
	}

	if (--m_nSystemLockCount == 0)
	{
#ifndef _DEBUG
		if ((m_pSystemForegroundWnd != NULL) && (m_bSystemForegroundEnabled == TRUE))
		{
			m_pSystemForegroundWnd->EnableWindow(TRUE);
		}
#endif
		m_pSystemForegroundWnd = NULL;
		m_bSystemForegroundEnabled = FALSE;
	}
}

/**********************************************************************/

bool CSystemData::GetActiveEnablePointsFlag()
{
	if (nWEBPAYMENT_PEBBLE == m_nWebPaymentType)
	{
		return FALSE;
	}
	else
	{
		return m_bFileEnablePoints;
	}
}

/**********************************************************************/

bool CSystemData::GetActiveEditPointsFlag()
{
	if (nWEBPAYMENT_PEBBLE == m_nWebPaymentType)
	{
		return FALSE;
	}
	else
	{
		return m_bFileEditPoints;
	}
}

/**********************************************************************/

bool CSystemData::GetActiveShowPointsButtonFlag()
{
	if (nWEBPAYMENT_PEBBLE == m_nWebPaymentType)
	{
		return FALSE;
	}
	else
	{
		return m_bFileShowPointsButton;
	}
}

/**********************************************************************/

bool CSystemData::GetActiveEditPurse1Flag()
{
	if (nWEBPAYMENT_PEBBLE == m_nWebPaymentType)
	{
		return FALSE;
	}
	else
	{
		return m_bFileEditPurse1;
	}
}

/**********************************************************************/

bool CSystemData::GetActiveEditPurse2Flag()
{
	if (nWEBPAYMENT_PEBBLE == m_nWebPaymentType)
	{
		return FALSE;
	}
	else
	{
		return m_bFileEditPurse2;
	}
}

/**********************************************************************/

bool CSystemData::GetActiveEditPurse3Flag()
{
	if (nWEBPAYMENT_PEBBLE == m_nWebPaymentType)
	{
		return FALSE;
	}
	else
	{
		return m_bFileEditPurse3;
	}
}

/**********************************************************************/
