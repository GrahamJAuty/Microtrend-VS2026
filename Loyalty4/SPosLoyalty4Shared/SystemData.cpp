//$$******************************************************************
#include "SQLNamespaces.h"
#include "SQLTable_Account\SQLRepositoryAccount.h"
//$$******************************************************************

const char* szNOSERVERACCESS = "The system is currently in the process of being backed up!\n\nPlease try later.";

//$$******************************************************************

CSystemData::CSystemData()
{
	m_nUserLockCount = 0;

	//GENERAL TAB
	SetInterfaceType(nINTERFACE_SWIPEv1);	// magswipe short number format
	m_strCardNoPrefix = "";					// moved from ServerData	
	m_bCheckDigit = FALSE;					// moved from ServerData
	m_bShowQRInfo = TRUE;
	m_bEnablePurse2 = FALSE;
	m_bPurse2PendingRefresh = FALSE;
	m_nSignOnMethod = 0;					// 0= Disabled, 1= via Password, 2= via Card
	m_bExternalAccount = FALSE;
	m_bSrvImportUnknown = TRUE;
	m_bPaxtonMode = FALSE;
	m_bQRCodeEmbedFlag = FALSE;
	m_strQRCodeEmbedPrefix = "";
	
	//EDITING TAB
	m_nListRefreshRate = 0;
	m_nDefaultAccountEditPage = 0;
	m_bShowButtonPurse1Topup = TRUE;
	m_bShowButtonPurse2Topup = FALSE;
	m_bShowButtonPurse1Spend = FALSE;
	m_bShowButtonPointsTopup = TRUE;
	m_bShowButtonPointsRedeem = TRUE;
	m_bShowButtonManualBonus = FALSE;
	m_bEditPurse1 = FALSE;
	m_bEditPurse2 = FALSE;
	m_bEditPoints = FALSE;
	m_bEditCashSpend = FALSE;
	m_bEnableBatchUpdates = FALSE;
	m_bBatchUpdatePoints = FALSE;
	m_bBatchUpdatePurse1 = FALSE;
	m_bBatchUpdatePurse2 = FALSE;
	m_bBatchUpdateSpendTD = FALSE;
	m_bContextMenuPurse1Topup = TRUE;
	m_bContextMenuPurse2Topup = FALSE;
	m_bAccountFindValid = TRUE;
	m_bAccountFindExpired = FALSE;
	m_bAccountFindHotlisted = FALSE;
	m_bAccountFindAlertSet = TRUE;
	m_bAccountFindInactive = FALSE;
	m_bAccountFindUsername = TRUE;
	m_bAccountFindForename = TRUE;
	m_bAccountFindSurname = TRUE;
	m_bAccountFindAddress = FALSE;
	m_bAccountFindPostcode = FALSE;
	m_bAccountFindPhone = FALSE;
	m_bAccountFindInfo = FALSE;
	m_bEnableAccountColour = TRUE;
	m_bAccountPreload = TRUE;
	m_bAccountCancelPrompt = FALSE;

	//EXPORT TAB
	m_bExportOnExit = FALSE;
	m_bExportOnDelete = FALSE;

	//REPORTING TAB
	m_nStartOfWeek = 1;					// 1=Sun, 7=Sat
	m_bEnablePhotoIDList = FALSE;
	m_bShowAuditPurchases = FALSE;	
	
	m_nDPFactor = 100;				// not saved
	m_nDPValue = 2;					// not saved
	m_bGotPCId = FALSE;
	m_bDoneSQLFileSize = FALSE;
	m_bDoneSQLShrinkLog = FALSE;
}

//$$******************************************************************

bool CSystemData::Read()
{
//** create folders if required

	CString strPath;
	strPath.Format ( "%s\\SysFiles", 
		(const char*) Sysset.GetProgramPath() );

	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	strPath.Format ( "%s\\History",
		(const char*) Sysset.GetProgramPath() );

	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	strPath.Format ( "%s\\Imports",
		(const char*) Sysset.GetProgramPath() );

	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	strPath.Format ( "%s\\Exports", 
		(const char*) Sysset.GetProgramPath() );

	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

// read system data

	bool bReadOldIniFile = FALSE;

	if (::FileExists(Filenames.GetSystemFilenameNew()) == FALSE)		// see if have new file
	{
		if (::FileExists(Filenames.GetSystemFilenameOld1()) == FALSE)	// if have old file
		{
			ConvertOldCSVFile();
			Write();
		}
		else
		{
			bReadOldIniFile = TRUE;
		}
	}

	bool bReply = FALSE;
	if (TRUE == bReadOldIniFile)
	{
		CSSIniFile file(SS_INI_AES);
		bReply = file.Read(Filenames.GetSystemFilenameOld1());
		RewindInternal(file,FALSE);

		if (TRUE == bReply)
		{
			Write();
		}
	}
	else
	{
		CSSIniFile file(SS_INI_NORMAL);
		bReply = file.Read(Filenames.GetSystemFilenameNew());
		RewindInternal(file,TRUE);
	}

// create any new folders

	if (HaveBarman() == FALSE)							// if no barman
	{
		m_bEnablePhotoIDList = FALSE;						// no reporting allowed
	}

	strPath.Format ( "%s\\PhotoID",
		(const char*) Sysset.GetProgramPath() );

	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}
	
	return bReply;
}

//$$******************************************************************

void CSystemData::ConvertOldCSVFile()
{
	CSSFile file;
	if (file.Open(Filenames.GetSystemFilenameOld2(), "rb") == TRUE)		// old csv file
	{
		CCSV csv;
		if (file.Read(csv) == TRUE)
		{
			if (csv.IsEmpty(0) == FALSE)	SetEditPurse1Flag(csv.GetBool(0));
			if (csv.IsEmpty(1) == FALSE)	SetEditPointsFlag(csv.GetBool(1));
			if (csv.IsEmpty(2) == FALSE)	SetShowButtonPurse1TopupFlag(csv.GetBool(2));
			if (csv.IsEmpty(3) == FALSE)	SetShowButtonPointsTopupFlag(csv.GetBool(3));
			if (csv.IsEmpty(4) == FALSE)	SetShowButtonManualBonusFlag(csv.GetBool(4));
			//REMOVED USE GROUPS FLAG
			if (csv.IsEmpty(6) == FALSE)	SetSignOnMethod(csv.GetInt(6));
			if (csv.IsEmpty(7) == FALSE)	SetStartOfWeek(csv.GetInt(7));
			if (csv.IsEmpty(8) == FALSE)	SetEnableBatchUpdatesFlag( csv.GetBool(8) );
			if (csv.IsEmpty(9) == FALSE)	SetEnablePhotoIDListFlag( csv.GetBool(9) );
			if (csv.IsEmpty(10) == FALSE)	SetExportOnExitFlag( csv.GetBool(10) );
			if (csv.IsEmpty(11) == FALSE)	SetExportOnDeleteFlag( csv.GetBool(11) );
			if (csv.IsEmpty(12) == FALSE)	SetEditPurse2Flag(csv.GetBool(12));
			if (csv.IsEmpty(13) == FALSE)	SetBatchUpdatePointsFlag( csv.GetBool(13) );
			if (csv.IsEmpty(14) == FALSE)	SetBatchUpdatePurse1Flag( csv.GetBool(14) );
			if (csv.IsEmpty(15) == FALSE)	SetBatchUpdatePurse2Flag( csv.GetBool(15) );
			if (csv.IsEmpty(16) == FALSE)	SetBatchUpdateSpendTDFlag( csv.GetBool(16) );
			if (csv.IsEmpty(17) == FALSE)	SetEnablePurse2Flag( csv.GetBool(17) );
			if (csv.IsEmpty(18) == FALSE)	SetShowAuditPurchasesFlag( csv.GetBool(18) );
		}
		file.Close();

		Server.Read(Filenames.GetServerDataFilename());

		SetInterfaceType((Server.GetObsoleteCardNoFormat() == 0) ? nINTERFACE_SWIPEv1 : nINTERFACE_SWIPEv2);		// Short or Long
		SetCheckDigitFlag(Server.GetObsoleteCardCheckDigitFlag());
		SetCardNoPrefix( Server.GetObsoleteCardNoPrefix() );

		Write();												// save new system ini file
	}
}

//$$******************************************************************

void CSystemData::RewindAndUpdateBool( CSSIniFile& file, bool& bField, CString strNewLabel, CString strLegacyLabel)
{
	switch (file.GetInt(strNewLabel, 0 ))
	{
	case 1:
		bField = FALSE;
		break;

	case 2:
		bField = TRUE;
		break;

	default:
		bField = file.GetBool(strLegacyLabel,bField);
		break;
	}
}

//$$******************************************************************

bool CSystemData::RewindExternal()
{
	CSSIniFile file(SS_INI_NORMAL);
	bool bReply = file.Read(Filenames.GetSystemFilenameNew());
	RewindInternal(file,TRUE);
	return bReply;
}

//$$******************************************************************

void CSystemData::RewindInternal(CSSIniFile& file, bool bNewFileNo)
{
	//GENERAL TAB
	SetInterfaceType(file.GetInt(SystemDataFields::InterfaceType, m_nInterfaceType));
	SetCardNoPrefix(file.GetString(SystemDataFields::CardNoPrefix, m_strCardNoPrefix));
	SetCheckDigitFlag(file.GetBool(SystemDataFields::CheckDigitFlag, m_bCheckDigit));
	SetShowQRInfoFlag(file.GetBool(SystemDataFields::ShowQRInfoFlag, m_bShowQRInfo));
	SetEnablePurse2Flag(file.GetBool(SystemDataFields::EnablePurse2Flag, m_bEnablePurse2));
	SetPurse2PendingRefreshFlag(file.GetBool(SystemDataFields::Purse2PendingRefreshFlag, m_bPurse2PendingRefresh));
	
	if ( TRUE == bNewFileNo)
	{
		SetSignOnMethod(file.GetInt(SystemDataFields::SignOnMethodNew, m_nSignOnMethod));
	}
	else
	{
		SetSignOnMethod(file.GetInt(SystemDataFields::SignOnMethodOld, m_nSignOnMethod));
	}
	
	SetExternalAccountFlag(file.GetBool(SystemDataFields::ExternalAccountFlag, m_bExternalAccount));
	SetSrvImportUnknownFlag(file.GetBool(SystemDataFields::SrvImportUnknownFlag, m_bSrvImportUnknown));
	SetPaxtonModeFlag(file.GetBool(SystemDataFields::PaxtonModeFlag, m_bPaxtonMode));
	SetQRCodeEmbedFlag(file.GetBool(SystemDataFields::QRCodeEmbedFlag, m_bQRCodeEmbedFlag));
	SetQRCodeEmbedPrefix(file.GetString(SystemDataFields::QRCodeEmbedPrefix, m_strQRCodeEmbedPrefix));

	//EDITING TAB
	SetListRefreshRate(file.GetInt(SystemDataFields::ListRefreshRate, m_nListRefreshRate));
	SetDefaultAccountEditPage(file.GetInt(SystemDataFields::DefaultAccountEditPage, m_nDefaultAccountEditPage));
	RewindAndUpdateBool(file, m_bShowButtonManualBonus, SystemDataFields::ShowButtonManualBonus, "LoyaltyButton");
	RewindAndUpdateBool(file, m_bShowButtonPurse1Topup, SystemDataFields::ShowButtonPurse1Topup, "CashButton");
	RewindAndUpdateBool(file, m_bShowButtonPointsTopup, SystemDataFields::ShowButtonPointsTopup, "PointsButton");
	RewindAndUpdateBool(file, m_bShowButtonPointsRedeem, SystemDataFields::ShowButtonPointsRedeem, "PointsRedeemButton");
	SetShowButtonPurse1SpendFlag(file.GetBool(SystemDataFields::ShowButtonPurse1Spend, m_bShowButtonPurse1Spend));
	SetShowButtonPurse2TopupFlag(file.GetBool(SystemDataFields::ShowButtonPurse2Topup, m_bShowButtonPurse2Topup));
	SetEditPurse1Flag(file.GetBool(SystemDataFields::EditPurse1Flag, m_bEditPurse1));
	SetEditPurse2Flag(file.GetBool(SystemDataFields::EditPurse2Flag, m_bEditPurse2));
	SetEditPointsFlag(file.GetBool(SystemDataFields::EditPointsFlag, m_bEditPoints));
	SetEditCashSpendFlag(file.GetBool(SystemDataFields::EditCashSpendFlag, m_bEditCashSpend));
	SetEnableBatchUpdatesFlag(file.GetBool(SystemDataFields::EnableBatchUpdatesFlag, m_bEnableBatchUpdates));
	SetBatchUpdatePointsFlag(file.GetBool(SystemDataFields::EnableBatchPointsFlag, m_bBatchUpdatePoints));
	SetBatchUpdatePurse1Flag(file.GetBool(SystemDataFields::EnableBatchPurse1Flag, m_bBatchUpdatePurse1));
	SetBatchUpdatePurse2Flag(file.GetBool(SystemDataFields::EnableBatchPurse2Flag, m_bBatchUpdatePurse2));
	SetBatchUpdateSpendTDFlag(file.GetBool(SystemDataFields::EnableBatchSpendTDFlag, m_bBatchUpdateSpendTD));
	SetContextMenuPurse1TopupFlag(file.GetBool(SystemDataFields::ContextMenuPurse1TopupFlag, m_bContextMenuPurse1Topup));
	SetContextMenuPurse2TopupFlag(file.GetBool(SystemDataFields::ContextMenuPurse2TopupFlag, m_bContextMenuPurse2Topup));
	SetAccountFindValidFlag(file.GetBool(SystemDataFields::AccountFindValidFlag, m_bAccountFindValid));
	SetAccountFindExpiredFlag(file.GetBool(SystemDataFields::AccountFindExpiredFlag, m_bAccountFindExpired));
	SetAccountFindHotlistedFlag(file.GetBool(SystemDataFields::AccountFindHotlistedFlag, m_bAccountFindHotlisted));
	SetAccountFindAlertSetFlag(file.GetBool(SystemDataFields::AccountFindAlertSetFlag, m_bAccountFindAlertSet));
	SetAccountFindInactiveFlag(file.GetBool(SystemDataFields::AccountFindInactiveFlag, m_bAccountFindInactive));
	SetAccountFindUsernameFlag(file.GetBool(SystemDataFields::AccountFindUsernameFlag, m_bAccountFindUsername));
	SetAccountFindForenameFlag(file.GetBool(SystemDataFields::AccountFindForenameFlag, m_bAccountFindForename));
	SetAccountFindSurnameFlag(file.GetBool(SystemDataFields::AccountFindSurnameFlag, m_bAccountFindSurname));
	SetAccountFindAddressFlag(file.GetBool(SystemDataFields::AccountFindAddressFlag, m_bAccountFindAddress));
	SetAccountFindPostcodeFlag(file.GetBool(SystemDataFields::AccountFindPostcodeFlag, m_bAccountFindPostcode));
	SetAccountFindPhoneFlag(file.GetBool(SystemDataFields::AccountFindPhoneFlag, m_bAccountFindPhone));
	SetAccountFindInfoFlag(file.GetBool(SystemDataFields::AccountFindInfoFlag, m_bAccountFindInfo));
	SetEnableAccountColourFlag(file.GetBool(SystemDataFields::EnableAccountColourFlag, m_bEnableAccountColour));
	SetAccountPreloadFlag(file.GetBool(SystemDataFields::AccountPreloadFlag, m_bAccountPreload));
	SetAccountCancelPromptFlag(file.GetBool(SystemDataFields::AccountCancelPrompt, m_bAccountCancelPrompt));

	//EXPORT TAB
	SetExportOnExitFlag(file.GetBool(SystemDataFields::ExportOnExitFlag, m_bExportOnExit));
	SetExportOnDeleteFlag(file.GetBool(SystemDataFields::ExportOnDeleteFlag, m_bExportOnDelete));

	//REPORTING TAB
	SetStartOfWeek(file.GetInt(SystemDataFields::StartOfWeek, m_nStartOfWeek));
	SetEnablePhotoIDListFlag( file.GetBool(SystemDataFields::EnablePhotoIDListFlag, m_bEnablePhotoIDList) );
	SetShowAuditPurchasesFlag( file.GetBool(SystemDataFields::ShowAuditPurchasesFlag, m_bShowAuditPurchases) );
	
	FixInternalSettings();
}

//$$******************************************************************

void CSystemData::FixInternalSettings()
{
	if (GetEnablePurse2Flag() == FALSE)
	{
		SetEditPurse2Flag(FALSE);
		SetBatchUpdatePurse2Flag(FALSE);
	}

	m_strLowCardNo = "1";

	switch (m_nInterfaceType)
	{
	case nINTERFACE_MIFAREv1:
		m_nMaxCardNoLength = 10;
		m_strHighCardNo = "9999999999";
		m_strBlankCardNo = "0000000000";
		break;

	case nINTERFACE_GENERALv1:
		m_nMaxCardNoLength = 12;
		m_strHighCardNo = "999999999999";
		m_strBlankCardNo = "000000000000";
		break;

	default:
		m_nMaxCardNoLength = 6;
		m_strHighCardNo = "999999";
		m_strBlankCardNo = "000000";
		break;
	}
}

//$$******************************************************************

bool CSystemData::Write()
{
	CSSIniFile file(SS_INI_NORMAL);

	//GENERAL TAB
	file.Set(SystemDataFields::InterfaceType, GetInterfaceType());
	file.Set(SystemDataFields::CardNoPrefix, GetCardNoPrefix());
	file.Set(SystemDataFields::CheckDigitFlag, GetCheckDigitFlag());
	file.Set(SystemDataFields::ShowQRInfoFlag, GetShowQRInfoFlag());
	file.Set(SystemDataFields::EnablePurse2Flag, GetEnablePurse2Flag());
	file.Set(SystemDataFields::Purse2PendingRefreshFlag, GetPurse2PendingRefreshFlag());
	file.Set(SystemDataFields::SignOnMethodNew, GetSignOnMethod());
	file.Set(SystemDataFields::ExternalAccountFlag, GetExternalAccountFlag());
	file.Set(SystemDataFields::SrvImportUnknownFlag, GetSrvImportUnknownFlag());
	file.Set(SystemDataFields::PaxtonModeFlag, GetPaxtonModeFlag());
	file.Set(SystemDataFields::EnableFamilyFlag, GetEnableFamilyFlag());
	file.Set(SystemDataFields::QRCodeEmbedFlag, GetQRCodeEmbedFlag());
	file.Set(SystemDataFields::QRCodeEmbedPrefix, GetQRCodeEmbedPrefix());
	
	//EDITING TAB
	file.Set(SystemDataFields::ListRefreshRate, GetListRefreshRate());
	file.Set(SystemDataFields::DefaultAccountEditPage, GetDefaultAccountEditPage());
	file.Set(SystemDataFields::ShowButtonManualBonus, GetShowButtonManualBonusFlag() ? 2 : 1);
	file.Set(SystemDataFields::ShowButtonPurse1Topup, GetShowButtonPurse1TopupFlag() ? 2 : 1);
	file.Set(SystemDataFields::ShowButtonPointsTopup, GetShowButtonPointsTopupFlag() ? 2 : 1);
	file.Set(SystemDataFields::ShowButtonPointsRedeem, GetShowButtonPointsRedeemFlag() ? 2 : 1);
	file.Set(SystemDataFields::ShowButtonPurse1Spend, GetShowButtonPurse1SpendFlag());
	file.Set(SystemDataFields::ShowButtonPurse2Topup, GetShowButtonPurse2TopupFlag());
	file.Set(SystemDataFields::EditPurse1Flag, GetEditPurse1Flag());
	file.Set(SystemDataFields::EditPurse2Flag, GetEditPurse2Flag());
	file.Set(SystemDataFields::EditPointsFlag, GetEditPointsFlag());
	file.Set(SystemDataFields::EditCashSpendFlag, GetEditCashSpendFlag());
	file.Set(SystemDataFields::EnableBatchUpdatesFlag, GetEnableBatchUpdatesFlag());
	file.Set(SystemDataFields::EnableBatchPointsFlag, GetBatchUpdatePointsFlag());
	file.Set(SystemDataFields::EnableBatchPurse1Flag, GetBatchUpdatePurse1Flag());
	file.Set(SystemDataFields::EnableBatchPurse2Flag, GetBatchUpdatePurse2Flag());
	file.Set(SystemDataFields::EnableBatchSpendTDFlag, GetBatchUpdateSpendTDFlag());
	file.Set(SystemDataFields::ContextMenuPurse1TopupFlag, GetContextMenuPurse1TopupFlag());
	file.Set(SystemDataFields::ContextMenuPurse2TopupFlag, GetContextMenuPurse2TopupFlag());
	file.Set(SystemDataFields::AccountFindValidFlag, GetAccountFindValidFlag());
	file.Set(SystemDataFields::AccountFindExpiredFlag, GetAccountFindExpiredFlag());
	file.Set(SystemDataFields::AccountFindHotlistedFlag, GetAccountFindHotlistedFlag());
	file.Set(SystemDataFields::AccountFindAlertSetFlag, GetAccountFindAlertSetFlag());
	file.Set(SystemDataFields::AccountFindInactiveFlag, GetAccountFindInactiveFlag());
	file.Set(SystemDataFields::AccountFindUsernameFlag, GetAccountFindUsernameFlag());
	file.Set(SystemDataFields::AccountFindForenameFlag, GetAccountFindForenameFlag());
	file.Set(SystemDataFields::AccountFindSurnameFlag, GetAccountFindSurnameFlag());
	file.Set(SystemDataFields::AccountFindAddressFlag, GetAccountFindAddressFlag());
	file.Set(SystemDataFields::AccountFindPostcodeFlag, GetAccountFindPostcodeFlag());
	file.Set(SystemDataFields::AccountFindPhoneFlag, GetAccountFindPhoneFlag());
	file.Set(SystemDataFields::AccountFindInfoFlag, GetAccountFindInfoFlag());
	file.Set(SystemDataFields::EnableAccountColourFlag, GetEnableAccountColourFlag());
	file.Set(SystemDataFields::AccountPreloadFlag, GetAccountPreloadFlag());
	file.Set(SystemDataFields::AccountCancelPrompt, GetAccountCancelPromptFlag());

	//EXPORT TAB
	file.Set(SystemDataFields::ExportOnExitFlag, GetExportOnExitFlag());
	file.Set(SystemDataFields::ExportOnDeleteFlag, GetExportOnDeleteFlag());

	//REPORTING TAB
	file.Set(SystemDataFields::StartOfWeek, GetStartOfWeek());
	file.Set(SystemDataFields::EnablePhotoIDListFlag, GetEnablePhotoIDListFlag());
	file.Set(SystemDataFields::ShowAuditPurchasesFlag, GetShowAuditPurchasesFlag());
	
	return file.Write(Filenames.GetSystemFilenameNew());
}

//$$******************************************************************

void CSystemData::SetListRefreshRate( int n )
{
	switch( n )
	{
	case 0:
	case 5:
	case 10:
	case 15:
	case 20:
	case 30:
		m_nListRefreshRate = n;
		break;
	}
}

//$$******************************************************************

void CSystemData::SetInterfaceType( int n )
{
	switch( n )
	{
	case nINTERFACE_SWIPEv1:
	case nINTERFACE_SWIPEv2:
	case nINTERFACE_MIFAREv1:
	case nINTERFACE_MIFAREv2:
	case nINTERFACE_MIFAREv3:
	case nINTERFACE_GENERALv1:
		m_nInterfaceType = n;
		break;
	}
}

//$$******************************************************************

void CSystemData::SetSignOnMethod( int n )
{
	if ((n >= 0) && (n <= 2))
	{
		m_nSignOnMethod = n;
	}
}

//$$******************************************************************

void CSystemData::SetStartOfWeek( int n )
{
	if ((n >= 1) && (n <= 7))
	{
		m_nStartOfWeek = n;
	}
}

//$$******************************************************************

void CSystemData::SetQRCodeEmbedPrefix(CString strPrefix)
{
	if (strPrefix.GetLength() <= MAX_QRCODE_PREFIX)
	{
		m_strQRCodeEmbedPrefix = strPrefix;
	}
}

//$$******************************************************************

bool CSystemData::IsSwipeSystem()
{
	switch ( GetInterfaceType() )
	{
	case nINTERFACE_SWIPEv1:
	case nINTERFACE_SWIPEv2:
		return TRUE;

	default:
		return FALSE;
	}
}

//$$******************************************************************

bool CSystemData::IsMiFareSystem()
{
	switch( GetInterfaceType() )
	{
	case nINTERFACE_MIFAREv1:
	case nINTERFACE_MIFAREv2:
	case nINTERFACE_MIFAREv3:
		return TRUE;

	default:
		return FALSE;
	}
}

//$$******************************************************************

bool CSystemData::HaveBarman()
{
	CString strProgName = "sposbarcodemanager.pgm";
	return ::FileExists ( strProgName );
}

//$$******************************************************************

bool CSystemData::IsBackupInProgress ( CWnd* pParent)
{
	return FALSE;
}

//$$******************************************************************

const char* CSystemData::GetCardNoText()
{
	switch( GetInterfaceType() )
	{
	case nINTERFACE_MIFAREv2:
	case nINTERFACE_MIFAREv3:
		return "Account";

	default:
		return "CardNo";
	}
}

//$$******************************************************************
int CSystemData::GetUserIDLength()		{ return m_strHighCardNo.GetLength();	}
//$$******************************************************************
__int64  CSystemData::GetMaxCardNo()	{ return _atoi64 ( m_strHighCardNo );	} 
//$$******************************************************************

CString CSystemData::FormatCardNo ( __int64 nCardNo, bool bPadOut  )
{
	CString strCardNo = "";
	strCardNo.Format ( "%I64d", nCardNo );

	if (bPadOut == TRUE)
	{
		::AddLeading(strCardNo, GetUserIDLength(), '0');
	}

	return strCardNo;
}

//$$******************************************************************

CString CSystemData::FormatCardNo ( CString strCardNo, bool bPadOut )
{
	if (bPadOut == TRUE)
	{
		::AddLeading(strCardNo, GetUserIDLength(), '0');
	}

	return strCardNo;
}

//$$******************************************************************
// given a card number 12 - format it to 000012 or prefix+cardno+checkdigit etc..

CString CSystemData::FormatFullCardNo(CString strCardNo)
{
	CString strCardNoOut = "";

	switch (GetInterfaceType())
	{
	case nINTERFACE_SWIPEv2:
		strCardNoOut.Format("%s%s",
			(const char*)m_strCardNoPrefix,
			(const char*)FormatCardNo(strCardNo));	// prefix + cardno + c
		break;

	default:
		strCardNoOut = FormatCardNo(strCardNo);		// 123 -> 000123
		break;
	}

	if (TRUE == m_bCheckDigit)
	{
		CString strCheckDigit = "";
		strCheckDigit.Format("%d", CalculateCheckSum(strCardNoOut));
		strCardNoOut += strCheckDigit;
	}

	return strCardNoOut;
}

//$$******************************************************************

int CSystemData::GetMaxCardNoDlgLen()
{
	int nMaxLen = 6;

	switch ( System.GetInterfaceType() )
	{
	case nINTERFACE_SWIPEv1:
		nMaxLen = ( m_bCheckDigit ) ? 7 : 6;
		break;

	case nINTERFACE_SWIPEv2:		
		nMaxLen = ( m_bCheckDigit ) ? 7 : 6;
		nMaxLen += m_strCardNoPrefix.GetLength();
		break;

	case nINTERFACE_MIFAREv1:
		nMaxLen = 10;
		break;

	case nINTERFACE_MIFAREv2:
	case nINTERFACE_MIFAREv3:
		nMaxLen = 6;
		break;

	case nINTERFACE_GENERALv1:
		nMaxLen = 12;
		break;
	}

	return nMaxLen;
}

//$$******************************************************************
// Given a cardno get correspong database record card number

CString CSystemData::TranslateCardNo ( CAccountIdentifier& AccountIdentifier, CString& strOutputCardNo, bool bFromCheckBalance )
{
	CString strError = "";
	strOutputCardNo	 = "";								// clear output cardno

	switch ( AccountIdentifier.GetAccountIDType() )
	{
	case nINTERFACE_SWIPEv1:		
		strError = ExtractSwipe1Format  ( AccountIdentifier.GetAccountID(), strOutputCardNo );	
		break;

	case nINTERFACE_SWIPEv2:	
		strError = ExtractSwipe2Format  ( AccountIdentifier.GetAccountID(), strOutputCardNo );
		break;

	case nINTERFACE_MIFAREv1:	
		strError = ExtractMiFare1Format ( AccountIdentifier.GetAccountID(), strOutputCardNo );	
		break;

	case nINTERFACE_MIFAREv2:	
		strError = ExtractMiFare2Format ( AccountIdentifier.GetAccountID(), strOutputCardNo );
		break;

	case nINTERFACE_MIFAREv3:		
		strError = ExtractMiFare3Format ( AccountIdentifier.GetAccountID(), strOutputCardNo, bFromCheckBalance );
		break;

	case nINTERFACE_QRINFO:		
		strError = ExtractQRInfoFormat	( AccountIdentifier.GetAccountID(), strOutputCardNo );	
		break;

	case nINTERFACE_SIMPSINNS:
		strError = ExtractSimpsInnsFormat(AccountIdentifier.GetAccountID(), strOutputCardNo);
		break;

	case nINTERFACE_GENERALv1:	
		strError = ExtractGeneralFormat ( AccountIdentifier.GetAccountID(), strOutputCardNo );
		break;

	default:						
		strError = "Unknown Interface";		
		break;
	}

	return strError;
}

//$$******************************************************************

CString CSystemData::ExtractGeneralFormat(CString strInputCardNo, CString& strOutputCardNo)
{
	strOutputCardNo = strInputCardNo;

	CString strError = "";
	if (strOutputCardNo.GetLength() > m_nMaxCardNoLength)			// stop HID cards ( length = 16 )
	{
		strError = "Invalid card number";
	}

	return strError;
}

//$$******************************************************************

CString CSystemData::ExtractMiFare1Format ( CString strMiFareNo, CString& strOutputCardNo )
{
	strOutputCardNo = strMiFareNo; 

	CString strError = "";
	if (strOutputCardNo.GetLength() > m_nMaxCardNoLength)			// stop HID cards ( length = 16 )
	{
		strError = "Invalid card number";
	}

	return strError;
}

//$$******************************************************************

CString CSystemData::ExtractMiFare2Format ( CString strMiFareNo, CString& strOutputCardNo )
{
	CString strError = "No matching account found";					// assume not found

	CSQLRepositoryAccount repoAccount;
	__int64 nAccountID = repoAccount.LookupMember(Account::ExternalRef.Label, strMiFareNo, NULL).GetSQLResult();
	
	if (nAccountID != 0)		// see if exists
	{
		strOutputCardNo.Format("%I64d", nAccountID);
		strError = "";
	}
	
	return strError;
}

//$$******************************************************************

CString CSystemData::ExtractQRInfoFormat(CString strQRInfo, CString& strOutputCardNo)
{
	CString strError = "No matching account found";					// assume not found

	CSQLRepositoryAccount repoAccount;
	__int64 nAccountID = repoAccount.LookupMember(Account::QRInfo.Label, strQRInfo, NULL).GetSQLResult();
	
	if (nAccountID != 0)		// see if exists
	{
		strOutputCardNo.Format("%I64d", nAccountID);
		strError = "";
	}
	
	return strError;
}

//$$******************************************************************

CString CSystemData::ExtractSimpsInnsFormat(CString strSimpsInns, CString& strOutputCardNo)
{
	CString strError = "No matching account found";					// assume not found

	CSQLRepositoryAccount repoAccount;
	__int64 nAccountID = repoAccount.LookupMember(Account::Info2.Label, strSimpsInns, NULL).GetSQLResult();

	if (nAccountID != 0)		// see if exists
	{
		strOutputCardNo.Format("%I64d", nAccountID);
		strError = "";
	}

	return strError;
}

//$$******************************************************************

CString CSystemData::ExtractMiFare3Format(CString strInputCardNo, CString& strOutputCardNo, bool bFromCheckBalance)
{
	strOutputCardNo = strInputCardNo;

	CString strError = "";
	
	/*** FIDDLE : MAY ENABLED THIS LATER IF CONFIRMED WITH CCM
	if (TRUE == bFromCheckBalance)
	{
		strError = "Account Access by ID number is blocked";
	}
	else 
	***/
	
	if (strOutputCardNo.GetLength() > m_nMaxCardNoLength)			// stop HID cards ( length = 16 )
	{
		strError = "Invalid card number";
	}

	return strError;
}

//$$******************************************************************
// expecting a short swipe card number format card number
// in range from 1 to 999999 ( may have check digit )

CString CSystemData::ExtractSwipe1Format(CString strInputCardNo, CString& strOutputCardNo)
{
	CString strCardNo = strInputCardNo;
	int nLength = strCardNo.GetLength();

	bool bOk = FALSE;
	switch (nLength)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		bOk = TRUE;
		break;

	case 7:
		bOk = m_bCheckDigit;
		break;
	}

	CString strError = "";
	if (bOk == TRUE)										// get final card number 
	{
		if (7 == nLength)
		{
			strCardNo = strCardNo.Left(6);					// lose check digit
		}

		strCardNo.TrimLeft('0');							// must be either 99999 or 999999
		strOutputCardNo = strCardNo;
	}
	else
	{
		strError = "Invalid card number";
	}

	return strError;
}

//$$******************************************************************
// expecting a long swipe card number format card number

CString CSystemData::ExtractSwipe2Format(CString strInputCardNo, CString& strOutputCardNo)
{
	CString strError = "";										// assume no error

	CString strCardNo = strInputCardNo;
	int nLength = strCardNo.GetLength();

	if (nLength > 6)											// treat 6 or less as user type input - no prefix or checkdigit
	{
		int nPrefixLength = m_strCardNoPrefix.GetLength();

		int nMaxLength = nPrefixLength + 6;						// Prefix + 999999 
		if (m_bCheckDigit == TRUE)
		{
			++nMaxLength;										// Prefix + 999999 + c
		}

		if (nLength <= nMaxLength)							// v2.05d - 21/09/2015 - Swipe-2
		{
			if ((m_bCheckDigit == TRUE) && (nLength == nMaxLength))
			{
				strCardNo.Delete(nLength - 1, 1);						// lose check digit on end of number
			}

			if (strCardNo.Left(nPrefixLength) == m_strCardNoPrefix)	// see if prefix match
			{
				strCardNo.Delete(0, nPrefixLength);					// yes - lose prefix from start of number
			}
			else
			{
				strError = "Invalid card number prefix";
			}
		}
		else
		{
			strError = "Invalid card number";
		}
	}

	if (strError == "")
	{
		strCardNo.TrimLeft('0');								// must be either 99999 or 999999
		strOutputCardNo = strCardNo;
	}

	return strError;
}

//$$******************************************************************

int CSystemData::CalculateCheckSum ( CString strCardNo )
{
	int factor = 2;
	int sum = 0;

// Starting from the right and working leftwards is easier since the initial "factor" will always be "2" 

	for (int i = strCardNo.GetLength() - 1; i >= 0; i-- )
	{
		char c = strCardNo.GetAt(i);
		int codePoint = c - 48;

		int addend = factor * codePoint;

	// Alternate the "factor" that each "codePoint" is multiplied by
		factor = (factor == 2) ? 1 : 2;

	// Sum the digits of the "addend" as expressed in base "n"
		addend = (addend / 10) + (addend % 10);
		sum += addend;
	}

// Calculate the number that must be added to the "sum" to make it divisible by "n"
	int remainder = sum % 10;
	int checkCodePoint = 10 - remainder;
	checkCodePoint %= 10;

	return checkCodePoint;
}

//$$******************************************************************

int CSystemData::GetMaxEcrmanDatabases()
{
	if ((Sysset.GetExternalLink() & 0x4000) != 0)
	{
		return 10;
	}
	else
	{
		return 1;
	}
}

//$$******************************************************************

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
