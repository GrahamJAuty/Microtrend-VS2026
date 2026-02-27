//*******************************************************************
#include "ServerData.h"
//*******************************************************************
int CServerData::m_nSimulateReaderType = 0;
//*******************************************************************

CServerData::CServerData(void)
{
	//TIME STAMP
	m_strTimeStamp = "***";

	//COMMUNICATION
	m_bLogAction2 = FALSE;
	m_bAllowOverdraft = FALSE;
	m_bAddUnknowns = FALSE;
	m_strOfflineSpendID = "999999900";
	m_dOfflineSpendLimit = 0.0;
	m_nUnknownGroup = 1;
	m_bNewBalanceOnReceipt = FALSE;
	m_nForenameSearchFieldType = 1;
	m_nCurrentEposPathSet = 1;

	for (int n = 0; n < EPOSREQPATH_COUNT; n++)
	{
		m_strEposPathLabel[n] = "";
		m_strEposRequestPath[n] = "";
		m_strEposLogPath[n] = "";
		m_bExternalRequestPath[n] = FALSE;
	}

	//PHOTO ID
	m_nPhotoIDFolderType = 0;
	m_strSmartPayUNCSystemPath = "";
	m_strSmartPayUNCImagePath = "";

	//PLU SALES
	m_strPluNo = "0";
	m_strPluNo2 = "0";
	m_dFileRevaluationTrip = 0.0;
	m_dFileRevaluationTrip2 = 0.0;
	m_nRevaluationPoints = 0;
	m_dRevaluationValue = 0.0;
	m_nRevaluationType = 0;
	m_strRevaluationComment = "";
	m_bFileEnablePurchaseHistory = FALSE;
	m_bDisablePurchaseHistoryAccountZero = FALSE;
	m_strEcrManagerImportFolder = "";

	//SPEND BONUS
	m_dRewardUpperSpendLimit = 0.0;
	m_dFileRewardTrip = 0.0;
	m_nRewardPoints = 0;
	m_dRewardValue = 0.0;
	m_nRewardMultiplierFactorMon = 1;
	m_nRewardMultiplierFactorTue = 1;
	m_nRewardMultiplierFactorWed = 1;
	m_nRewardMultiplierFactorThu = 1;
	m_nRewardMultiplierFactorFri = 1;
	m_nRewardMultiplierFactorSat = 1;
	m_nRewardMultiplierFactorSun = 1;
	m_dRewardMultiplierTrigger = 0.0;
	m_bRewardIncludePurse1Spend = FALSE;
	m_bRewardIncludePurse2Spend = FALSE;
	m_bRewardIncludePurse3Spend = FALSE;
	m_bRewardIncludeCashSpend = FALSE;
	m_nRewardType = 0;
	m_strRewardComment = "";
	m_timeMonStart.SetTime("00:00");
	m_timeMonEnd.SetTime("23:59");
	m_timeTueStart.SetTime("00:00");
	m_timeTueEnd.SetTime("23:59");
	m_timeWedStart.SetTime("00:00");
	m_timeWedEnd.SetTime("23:59");
	m_timeThuStart.SetTime("00:00");
	m_timeThuEnd.SetTime("23:59");
	m_timeFriStart.SetTime("00:00");
	m_timeFriEnd.SetTime("23:59");
	m_timeSatStart.SetTime("00:00");
	m_timeSatEnd.SetTime("23:59");
	m_timeSunStart.SetTime("00:00");
	m_timeSunEnd.SetTime("23:59");

	//REDEEM POINTS
	m_nFileRedeemPointsTrip = 0;
	m_nRedeemPurse = 1;
	m_dRedeemValue = 0.0;
	m_bRedeemDisableAuto = FALSE;
	m_strRedeemComment = "";
	
	//NON UI
	m_nSearchMax = 3;
	m_bAllowRetries = FALSE;
	m_nAllowPebbleAdjust = 0;
	m_nAllowPINAdmin = 0;
	m_nLockRetries = 5;
	m_nThreadDiagnostics = 0;
	m_bBypassVersionSafetyCheck = FALSE;
	m_bCopyLogFiles = FALSE;
	m_nLockFolderTimeout = 15;
	m_bBromComTestMode = FALSE;
	m_bPebbleDemo = FALSE;
	m_bLockPebbleDemoFlag = FALSE;
	m_nPebbleDiagnosticType = 0;
	m_bLogCardReads = FALSE;	

	//OTHER
    m_strError = "";
	m_strEcrManagerImportPluFilename = "";
	m_strEcrManagerImportOperatorFilename = "";
	m_strEcrManagerImportPurchaseControlFilename = "";
	m_bPebbleDiagnosticMessageBox = TRUE;
}

//*******************************************************************

bool CServerData::Read(const CString strDatapath)
{
	CSSIniFile file;
	bool bReply = file.Read(strDatapath);

	//TIME STAMP
	CString strTimeStamp = file.GetString(ServerDataFields::TimeStamp, "");

	if (strTimeStamp == m_strTimeStamp)
	{
		return bReply;
	}

	m_strTimeStamp = strTimeStamp;

	//COMMUNICATION
	SetLogAction2Flag(file.GetBool(ServerDataFields::LogAction2, FALSE));
	SetAllowOverdraftFlag(file.GetBool(ServerDataFields::AllowOverdraft, FALSE));
	SetAddUnknownsFlag(file.GetBool(ServerDataFields::AddUnknowns, FALSE));
	SetOfflineSpendID(file.GetString(ServerDataFields::OfflineSpendID, "999999900"));
	SetOfflineSpendLimit(file.GetDouble(ServerDataFields::OfflineSpendLimit, 0.0));
	SetUnknownGroup(file.GetInt(ServerDataFields::UnknownGroup, 1));
	SetNewBalanceOnReceiptFlag(file.GetBool(ServerDataFields::NewBalanceOnReceipt, FALSE));
	SetForenameSearchFieldType(file.GetInt(ServerDataFields::ForenameSearchFieldType, 0));

	if (GetForenameSearchFieldType() == 0)
	{
		bool bLegacyInfo4 = (file.GetBool("ReplaceForenameWithInfo4", FALSE));

		if (TRUE == bLegacyInfo4)
		{
			SetForenameSearchFieldType(3);
		}
		else
		{
			SetForenameSearchFieldType(1);
		}
	}

	{
		CString strPathLabel = ServerDataFields::EposPathLabel;
		CString strLoyReq = ServerDataFields::EposRequestPath;
		CString strLoyLog = ServerDataFields::EposLogPath;
		CString strExtReq = ServerDataFields::ExternalRequestPath;

		CString strSet = "Set ";
		for (int n = 1; n <= EPOSREQPATH_COUNT; n++)
		{
			CString strNum = "";

			if (1 == n)
			{
				SetEposPathLabel(1, file.GetString(strPathLabel, "Standard Folder Set"));
			}
			else
			{
				strNum.Format("%d", n);
				SetEposPathLabel(n, file.GetString(strPathLabel + strNum, strSet + strNum));
			}

			SetEposRequestPath(n, file.GetString(strLoyReq + strNum, ""));
			SetEposLogPath(n, file.GetString(strLoyLog + strNum, ""));
			SetExternalRequestPathFlag(n, file.GetBool(strExtReq + strNum, FALSE));
		}
	}

	//PHOTO ID
	SetPhotoIDFolderType(file.GetInt(ServerDataFields::PhotoIDFolderType, 0));
	SetSmartPayUNCSystemPath(file.GetString(ServerDataFields::SmartPayUNCSystemPath, ""));
	SetSmartPayUNCImagePath(file.GetString(ServerDataFields::SmartPayUNCImagePath, ""));

	//PLU SALES
	SetPluNo(file.GetString(ServerDataFields::PluNo, "0"));
	m_strPluNo.TrimLeft('0');
	SetPluNo2(file.GetString(ServerDataFields::PluNo2, "0"));
	m_strPluNo2.TrimLeft('0');
	SetFileRevaluationTrip(file.GetDouble(ServerDataFields::RevaluationTrip, 0.0));
	SetFileRevaluationTrip2(file.GetDouble(ServerDataFields::RevaluationTrip2, 0.0));
	SetRevaluationPoints(file.GetInt(ServerDataFields::RevaluationPoints, 0));
	SetRevaluationValue(file.GetDouble(ServerDataFields::RevaluationValue, 0.0));
	SetRevaluationType(file.GetInt(ServerDataFields::RevaluationType, 0));
	SetRevaluationComment(file.GetString(ServerDataFields::RevaluationComment, ""));
	SetFileEnablePurchaseHistoryFlag(file.GetBool(ServerDataFields::EnablePurchaseHistory, FALSE));
	SetDisablePurchaseHistoryAccountZeroFlag(file.GetBool(ServerDataFields::DisablePurchaseHistoryAccountZero, FALSE));
	SetEcrManagerImportFolder(file.GetString(ServerDataFields::EcrManagerImportFolder, ""));

	if (m_strEcrManagerImportFolder == "")
	{
		CString strPluUpdateFilename = file.GetString("PluPts", "");

		int nLength = strPluUpdateFilename.GetLength();
		if (nLength > 10)
		{
			strPluUpdateFilename.MakeLower();
			if (strPluUpdateFilename.Right(10) == "plupts.csv")
			{
				m_strEcrManagerImportFolder = strPluUpdateFilename.Left(nLength - 10);
			}
		}
	}

	//SPEND BONUS
	SetRewardUpperSpendLimit(file.GetDouble(ServerDataFields::RewardUpperSpendLimit, 0.0));
	SetFileRewardTrip(file.GetDouble(ServerDataFields::RewardTrip, 0.0));
	SetRewardPoints(file.GetInt(ServerDataFields::RewardPoints, 0));
	SetRewardValue(file.GetDouble(ServerDataFields::RewardValue, 0.0));
	SetRewardMultiplierFactorMon(file.GetInt(ServerDataFields::RewardMultiplierFactorMon, 1));
	SetRewardMultiplierFactorTue(file.GetInt(ServerDataFields::RewardMultiplierFactorTue, 1));
	SetRewardMultiplierFactorWed(file.GetInt(ServerDataFields::RewardMultiplierFactorWed, 1));
	SetRewardMultiplierFactorThu(file.GetInt(ServerDataFields::RewardMultiplierFactorThu, 1));
	SetRewardMultiplierFactorFri(file.GetInt(ServerDataFields::RewardMultiplierFactorFri, 1));
	SetRewardMultiplierFactorSat(file.GetInt(ServerDataFields::RewardMultiplierFactorSat, 1));
	SetRewardMultiplierFactorSun(file.GetInt(ServerDataFields::RewardMultiplierFactorSun, 1));
	SetRewardMultiplierTrigger(file.GetDouble(ServerDataFields::RewardMultiplierTrigger, 0.0));
	SetRewardIncludePurse1SpendFlag(file.GetBool(ServerDataFields::RewardIncludePurse1Spend, FALSE));
	SetRewardIncludePurse2SpendFlag(file.GetBool(ServerDataFields::RewardIncludePurse2Spend, FALSE));
	SetRewardIncludePurse3SpendFlag(file.GetBool(ServerDataFields::RewardIncludePurse3Spend, FALSE));
	SetRewardIncludeCashSpendFlag(file.GetBool(ServerDataFields::RewardIncludeCashSpend, FALSE));
	SetRewardType(file.GetInt(ServerDataFields::RewardType, 0));
	SetRewardComment(file.GetString(ServerDataFields::RewardComment, ""));
	SetMonStart(CSSTime(file.GetString(ServerDataFields::MonStart, "00:00")));
	SetMonEnd(CSSTime(file.GetString(ServerDataFields::MonEnd, "23:59")));
	SetTueStart(CSSTime(file.GetString(ServerDataFields::TueStart, "00:00")));
	SetTueEnd(CSSTime(file.GetString(ServerDataFields::TueEnd, "23:59")));
	SetWedStart(CSSTime(file.GetString(ServerDataFields::WedStart, "00:00")));
	SetWedEnd(CSSTime(file.GetString(ServerDataFields::WedEnd, "23:59")));
	SetThuStart(CSSTime(file.GetString(ServerDataFields::ThuStart, "00:00")));
	SetThuEnd(CSSTime(file.GetString(ServerDataFields::ThuEnd, "23:59")));
	SetFriStart(CSSTime(file.GetString(ServerDataFields::FriStart, "00:00")));
	SetFriEnd(CSSTime(file.GetString(ServerDataFields::FriEnd, "23:59")));
	SetSatStart(CSSTime(file.GetString(ServerDataFields::SatStart, "00:00")));
	SetSatEnd(CSSTime(file.GetString(ServerDataFields::SatEnd, "23:59")));
	SetSunStart(CSSTime(file.GetString(ServerDataFields::SunStart, "00:00")));
	SetSunEnd(CSSTime(file.GetString(ServerDataFields::SunEnd, "23:59")));

	//REDEEM POINTS
	SetFileRedeemPointsTrip(file.GetInt(ServerDataFields::RedeemPointsTrip, 0));
	SetRedeemPurse(file.GetInt(ServerDataFields::RedeemPurse, 1));
	SetRedeemValue(file.GetDouble(ServerDataFields::RedeemValue, 0.0));
	SetRedeemDisableAutoFlag(file.GetBool(ServerDataFields::RedeemDisableAuto, FALSE));
	SetRedeemComment(file.GetString(ServerDataFields::RedeemComment, ""));

	//NON UI
	SetSearchMax(file.GetInt(ServerDataFields::SearchMax, 3));
	SetAllowRetriesFlag(file.GetBool(ServerDataFields::AllowRetries, FALSE));
	SetAllowPebbleAdjustCode(file.GetInt(ServerDataFields::AllowPebbleAdjust, 0));
	SetAllowPINAdminCode(file.GetInt(ServerDataFields::AllowPINAdmin, 0));
	SetLockRetries(file.GetInt(ServerDataFields::LockRetries, 5));
	SetThreadDiagnosticsFlags(file.GetInt(ServerDataFields::ThreadDiagnostics, 0));
	SetCopyLogFilesFlag(file.GetBool(ServerDataFields::CopyLogFiles, FALSE));
	SetLockFolderTimeout(file.GetInt(ServerDataFields::LockFolderTimeout, 15));
	SetBromComTestModeFlag(file.GetBool(ServerDataFields::BromComTestMode, FALSE));
	SetPebbleDemoFlag(file.GetBool(ServerDataFields::PebbleDemo, FALSE));
	SetPebbleDiagnosticType(file.GetInt(ServerDataFields::PebbleDiagnostic, 0));
	SetLogCardReadsFlag(file.GetBool(ServerDataFields::LogCardReads, FALSE));

	{
		CString strTest = file.GetString(ServerDataFields::BypassVersionSafetyCheck, "");
		strTest.MakeUpper();
		SetBypassVersionSafetyCheckFlag(strTest == "<*YES*>");
	}

	SetSimulateReaderType(file.GetInt(ServerDataFields::SimulateReaderType, 0));

	return bReply;
}

//*******************************************************************

void CServerData::SetForenameSearchFieldType(int n)
{
	switch (n)
	{
	case 0:	//NOT DEFINED, USE LEGACY FLAG
	case 1: //FORENAME
	case 2: //INFO3
	case 3: //INFO4
		m_nForenameSearchFieldType = n;
		break;
	}
}

//*******************************************************************

void CServerData::SetPebbleDemoFlag(bool b)
{
	if (FALSE == m_bLockPebbleDemoFlag)
	{
		m_bPebbleDemo = b;
		m_bLockPebbleDemoFlag = TRUE;
	}
}

//*******************************************************************

void CServerData::SetSimulateReaderType(int n)
{
	switch (n)
	{
	case 0:
	case 1:
	case 2:
		m_nSimulateReaderType = n;
		break;
	}
}

//*******************************************************************

bool CServerData::Write(const CString strDatapath)
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strTimeStamp;
	strTimeStamp.Format("%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond());

	CSSIniFile file;
	//TIME STAMP
	file.Set(ServerDataFields::TimeStamp, strTimeStamp);
	//COMMUNICATION
	file.Set(ServerDataFields::LogAction2, GetLogAction2Flag());
	file.Set(ServerDataFields::AllowOverdraft, GetAllowOverdraftFlag());
	file.Set(ServerDataFields::AddUnknowns, GetAddUnknownsFlag());
	file.Set(ServerDataFields::OfflineSpendID, GetOfflineSpendID());
	file.Set(ServerDataFields::OfflineSpendLimit, GetOfflineSpendLimit(), 2);
	file.Set(ServerDataFields::UnknownGroup, GetUnknownGroup());
	file.Set(ServerDataFields::NewBalanceOnReceipt, GetNewBalanceOnReceiptFlag());
	file.Set(ServerDataFields::ForenameSearchFieldType, GetForenameSearchFieldType());

	{
		CString strPathLabel = ServerDataFields::EposPathLabel;
		CString strLoyReq = ServerDataFields::EposRequestPath;
		CString strLoyLog = ServerDataFields::EposLogPath;
		CString strExtReq = ServerDataFields::ExternalRequestPath;

		for (int n = 1; n <= EPOSREQPATH_COUNT; n++)
		{
			CString strNum = "";

			if (n != 1)
			{
				strNum.Format("%d", n);
			}

			file.Set(strPathLabel + strNum, GetEposPathLabel(n));
			file.Set(strLoyReq + strNum, GetEposRequestPath(n));
			file.Set(strLoyLog + strNum, GetEposLogPath(n));
			file.Set(strExtReq + strNum, GetExternalRequestPathFlag(n));
		}
	}

	//PHOTO ID
	file.Set(ServerDataFields::PhotoIDFolderType, GetPhotoIDFolderType());
	file.Set(ServerDataFields::SmartPayUNCSystemPath, GetSmartPayUNCSystemPath());
	file.Set(ServerDataFields::SmartPayUNCImagePath, GetSmartPayUNCImagePath());

	//PLU SALES
	m_strPluNo.TrimLeft('0');
	m_strPluNo2.TrimLeft('0');
	file.Set(ServerDataFields::PluNo, GetPluNo());
	file.Set(ServerDataFields::PluNo2, GetPluNo2());
	file.Set(ServerDataFields::RevaluationTrip, GetFileRevaluationTrip(), 2);
	file.Set(ServerDataFields::RevaluationTrip2, GetFileRevaluationTrip2(), 2);
	file.Set(ServerDataFields::RevaluationPoints, GetRevaluationPoints());
	file.Set(ServerDataFields::RevaluationValue, GetRevaluationValue(), 2);
	file.Set(ServerDataFields::RevaluationType, GetRevaluationType());
	file.Set(ServerDataFields::RevaluationComment, GetRevaluationComment());
	file.Set(ServerDataFields::EnablePurchaseHistory, GetFileEnablePurchaseHistoryFlag());
	file.Set(ServerDataFields::DisablePurchaseHistoryAccountZero, GetDisablePurchaseHistoryAccountZeroFlag());
	file.Set(ServerDataFields::EcrManagerImportFolder, GetEcrManagerImportFolder());

	//SPEND BONUS
	file.Set(ServerDataFields::RewardUpperSpendLimit, GetRewardUpperSpendLimit(), 2);
	file.Set(ServerDataFields::RewardTrip, GetFileRewardTrip(), 2);
	file.Set(ServerDataFields::RewardPoints, GetRewardPoints());
	file.Set(ServerDataFields::RewardValue, GetRewardValue(), 2);
	file.Set(ServerDataFields::RewardMultiplierFactorMon, GetRewardMultiplierFactorMon());
	file.Set(ServerDataFields::RewardMultiplierFactorTue, GetRewardMultiplierFactorTue());
	file.Set(ServerDataFields::RewardMultiplierFactorWed, GetRewardMultiplierFactorWed());
	file.Set(ServerDataFields::RewardMultiplierFactorThu, GetRewardMultiplierFactorThu());
	file.Set(ServerDataFields::RewardMultiplierFactorFri, GetRewardMultiplierFactorFri());
	file.Set(ServerDataFields::RewardMultiplierFactorSat, GetRewardMultiplierFactorSat());
	file.Set(ServerDataFields::RewardMultiplierFactorSun, GetRewardMultiplierFactorSun());
	file.Set(ServerDataFields::RewardMultiplierTrigger, GetRewardMultiplierTrigger(), 2);
	file.Set(ServerDataFields::RewardIncludePurse1Spend, GetRewardIncludePurse1SpendFlag());
	file.Set(ServerDataFields::RewardIncludePurse2Spend, GetRewardIncludePurse2SpendFlag());
	file.Set(ServerDataFields::RewardIncludePurse3Spend, GetRewardIncludePurse3SpendFlag());
	file.Set(ServerDataFields::RewardIncludeCashSpend, GetRewardIncludeCashSpendFlag());
	file.Set(ServerDataFields::RewardType, GetRewardType());
	file.Set(ServerDataFields::RewardComment, GetRewardComment());
	file.Set(ServerDataFields::MonStart, GetMonStart().GetTime());
	file.Set(ServerDataFields::MonEnd, GetMonEnd().GetTime());
	file.Set(ServerDataFields::TueStart, GetTueStart().GetTime());
	file.Set(ServerDataFields::TueEnd, GetTueEnd().GetTime());
	file.Set(ServerDataFields::WedStart, GetWedStart().GetTime());
	file.Set(ServerDataFields::WedEnd, GetWedEnd().GetTime());
	file.Set(ServerDataFields::ThuStart, GetThuStart().GetTime());
	file.Set(ServerDataFields::ThuEnd, GetThuEnd().GetTime());
	file.Set(ServerDataFields::FriStart, GetFriStart().GetTime());
	file.Set(ServerDataFields::FriEnd, GetFriEnd().GetTime());
	file.Set(ServerDataFields::SatStart, GetSatStart().GetTime());
	file.Set(ServerDataFields::SatEnd, GetSatEnd().GetTime());
	file.Set(ServerDataFields::SunStart, GetSunStart().GetTime());
	file.Set(ServerDataFields::SunEnd, GetSunEnd().GetTime());

	//REDEEM POINTS
	file.Set(ServerDataFields::RedeemPointsTrip, GetFileRedeemPointsTrip());
	file.Set(ServerDataFields::RedeemPurse, GetRedeemPurse());
	file.Set(ServerDataFields::RedeemValue, GetRedeemValue(), 2);
	file.Set(ServerDataFields::RedeemDisableAuto, GetRedeemDisableAutoFlag());
	file.Set(ServerDataFields::RedeemComment, GetRedeemComment());
	
	//NON UI
	file.Set(ServerDataFields::SearchMax, GetSearchMax());
	file.Set(ServerDataFields::AllowRetries, GetAllowRetriesFlag());
	file.Set(ServerDataFields::AllowPebbleAdjust, GetAllowPebbleAdjustCode());
	file.Set(ServerDataFields::AllowPINAdmin, GetAllowPINAdminCode());
	file.Set(ServerDataFields::LockRetries, GetLockRetries());
	file.Set(ServerDataFields::ThreadDiagnostics, GetThreadDiagnosticsFlags());
	file.Set(ServerDataFields::BypassVersionSafetyCheck, GetBypassVersionSafetyCheckFlag() ? "<*YES*>" : "");
	file.Set(ServerDataFields::LockFolderTimeout, GetLockFolderTimeout());
	file.Set(ServerDataFields::BromComTestMode, GetBromComTestModeFlag());
	file.Set(ServerDataFields::PebbleDemo, GetPebbleDemoFlag());
	file.Set(ServerDataFields::PebbleDiagnostic, GetPebbleDiagnosticType());
	file.Set(ServerDataFields::LogCardReads, GetLogCardReadsFlag());

	switch (m_nSimulateReaderType)
	{
	case 1:
	case 2:
		file.Set(ServerDataFields::SimulateReaderType, m_nSimulateReaderType);
		break;
	}

	return file.Write(strDatapath);
}

//*******************************************************************

void CServerData::CopyFrom( CServerData& source )
{
	//TIME STAMP
	m_strTimeStamp = source.m_strTimeStamp;
	
	//COMMUNICATION
	SetLogAction2Flag(source.GetLogAction2Flag());
	SetAllowOverdraftFlag(source.GetAllowOverdraftFlag());
	SetAddUnknownsFlag(source.GetAddUnknownsFlag());
	SetOfflineSpendID(source.GetOfflineSpendID());
	SetOfflineSpendLimit(source.GetOfflineSpendLimit());
	SetUnknownGroup(source.GetUnknownGroup());
	SetNewBalanceOnReceiptFlag(source.GetNewBalanceOnReceiptFlag());
	SetForenameSearchFieldType(source.GetForenameSearchFieldType());

	for (int n = 1; n <= EPOSREQPATH_COUNT; n++)
	{
		SetEposPathLabel(n, source.GetEposPathLabel(n));
		SetEposRequestPath(n, source.GetEposRequestPath(n));
		SetEposLogPath(n, source.GetEposLogPath(n));
		SetExternalRequestPathFlag(n, source.GetExternalRequestPathFlag(n));
	}

	//PHOTO ID
	SetPhotoIDFolderType(source.GetPhotoIDFolderType());
	SetSmartPayUNCSystemPath(source.GetSmartPayUNCSystemPath());
	SetSmartPayUNCImagePath(source.GetSmartPayUNCImagePath());

	//PLU SALES
	SetPluNo(source.GetPluNo());
	SetPluNo2(source.GetPluNo2());
	SetFileRevaluationTrip(source.GetFileRevaluationTrip());
	SetFileRevaluationTrip2(source.GetFileRevaluationTrip2());
	SetRevaluationPoints(source.GetRevaluationPoints());
	SetRevaluationValue(source.GetRevaluationValue());
	SetRevaluationType(source.GetRevaluationType());
	SetRevaluationComment(source.GetRevaluationComment());
	SetFileEnablePurchaseHistoryFlag(source.GetFileEnablePurchaseHistoryFlag());
	SetDisablePurchaseHistoryAccountZeroFlag(source.GetDisablePurchaseHistoryAccountZeroFlag());
	SetEcrManagerImportFolder(source.GetEcrManagerImportFolder());

	//SPEND BONUS
	SetRewardUpperSpendLimit(source.GetRewardUpperSpendLimit());
	SetFileRewardTrip(source.GetFileRewardTrip());
	SetRewardPoints(source.GetRewardPoints());
	SetRewardValue(source.GetRewardValue());
	SetRewardMultiplierFactorMon(source.GetRewardMultiplierFactorMon());
	SetRewardMultiplierFactorTue(source.GetRewardMultiplierFactorTue());
	SetRewardMultiplierFactorWed(source.GetRewardMultiplierFactorWed());
	SetRewardMultiplierFactorThu(source.GetRewardMultiplierFactorThu());
	SetRewardMultiplierFactorFri(source.GetRewardMultiplierFactorFri());
	SetRewardMultiplierFactorSat(source.GetRewardMultiplierFactorSat());
	SetRewardMultiplierFactorSun(source.GetRewardMultiplierFactorSun());
	SetRewardMultiplierTrigger(source.GetRewardMultiplierTrigger());
	SetRewardIncludePurse1SpendFlag(source.GetRewardIncludePurse1SpendFlag());
	SetRewardIncludePurse2SpendFlag(source.GetRewardIncludePurse2SpendFlag());
	SetRewardIncludePurse3SpendFlag(source.GetRewardIncludePurse3SpendFlag());
	SetRewardIncludeCashSpendFlag(source.GetRewardIncludeCashSpendFlag());
	SetRewardType(source.GetRewardType());
	SetRewardComment(source.GetRewardComment());
	SetMonStart(source.GetMonStart());
	SetMonEnd(source.GetMonEnd());
	SetTueStart(source.GetTueStart());
	SetTueEnd(source.GetTueEnd());
	SetWedStart(source.GetWedStart());
	SetWedEnd(source.GetWedEnd());
	SetThuStart(source.GetThuStart());
	SetThuEnd(source.GetThuEnd());
	SetFriStart(source.GetFriStart());
	SetFriEnd(source.GetFriEnd());
	SetSatStart(source.GetSatStart());
	SetSatEnd(source.GetSatEnd());
	SetSunStart(source.GetSunStart());
	SetSunEnd(source.GetSunEnd());

	//REDEEM POINTS
	SetFileRedeemPointsTrip(source.GetFileRedeemPointsTrip());
	SetRedeemPurse(source.GetRedeemPurse());
	SetRedeemValue(source.GetRedeemValue());
	SetRedeemDisableAutoFlag(source.GetRedeemDisableAutoFlag());
	SetRedeemComment(source.GetRedeemComment());

	//NON UI
	SetSearchMax(source.GetSearchMax());
	SetAllowRetriesFlag(source.GetAllowRetriesFlag());
	SetAllowPebbleAdjustCode(source.GetAllowPebbleAdjustCode());
	SetAllowPINAdminCode(source.GetAllowPINAdminCode());
	SetLockRetries(source.GetLockRetries());
	SetThreadDiagnosticsFlags(source.GetThreadDiagnosticsFlags());
	SetBypassVersionSafetyCheckFlag(source.GetBypassVersionSafetyCheckFlag());
	SetCopyLogFilesFlag(source.GetCopyLogFilesFlag());
	SetLockFolderTimeout(source.GetLockFolderTimeout());
	SetBromComTestModeFlag(source.GetBromComTestModeFlag());
	m_bPebbleDemo = source.GetPebbleDemoFlag();
	m_nPebbleDiagnosticType = source.GetPebbleDiagnosticType();
	SetLogCardReadsFlag(source.GetLogCardReadsFlag());
}

//*******************************************************************

bool CServerData::RevaluationPlu2Inuse()
{
	CString strPluNo2 = m_strPluNo2;
	strPluNo2.TrimLeft ( '0' );
	return ( strPluNo2 != "" ) ? TRUE : FALSE;
}

//*******************************************************************

CString CServerData::GetTimeString ( int nHour, int nMinute )
{
	CString strBuf = "";
	strBuf.Format ( "%2.2d:%2.2d", nHour, nMinute );
	return strBuf;
}

//*******************************************************************
// Validate UserID & remove leading 0's

bool CServerData::ValidateUserID ( const char* szInputCardNo, CString& strOutputCardNo )
{
	m_strError = "";
	CString strCardNo = szInputCardNo;

	if ( ::IsStringNumeric ( strCardNo, 1 ) == TRUE )
	{
		strCardNo.TrimLeft('0');

		if (strCardNo == "")
		{
			strOutputCardNo = "0";					// CardNo 0 allowed as its the cash record
		}
		else
		{
			strOutputCardNo = strCardNo;
		}
	}
	else
	{
		m_strError = "Invalid card number";
	}

	return ( m_strError == "" ) ? TRUE : FALSE;
}

//*******************************************************************

void CServerData::SetPhotoIDFolderType( int n )
{
	if ((n >= 0) && (n <= 2))
	{
		m_nPhotoIDFolderType = n;
	}
}

//*******************************************************************

void CServerData::SetLockFolderTimeout(int n)
{
	if ((n >= 3) && (n <= 15))
	{
		m_nLockFolderTimeout = n;
	}
}

//*******************************************************************

void CServerData::SetEposRequestPath(int n, const char* sz)
{
	if ((n >= 1) && (n <= EPOSREQPATH_COUNT))
	{
		m_strEposRequestPath[n - 1] = sz;
	}
}

//*******************************************************************

const char* CServerData::GetEposRequestPath(int n)
{
	if ((n >= 1) && (n <= EPOSREQPATH_COUNT))
	{
		return m_strEposRequestPath[n - 1];
	}
	else
	{
		return m_strEposRequestPath[0];
	}
}

//*******************************************************************

void CServerData::SetEposLogPath(int n, const char* sz)
{
	if ((n >= 1) && (n <= EPOSREQPATH_COUNT))
	{
		m_strEposLogPath[n - 1] = sz;
	}
}

//*******************************************************************

const char* CServerData::GetEposLogPath(int n)
{
	if ((n >= 1) && (n <= EPOSREQPATH_COUNT))
	{
		return m_strEposLogPath[n - 1];
	}
	else
	{
		return m_strEposLogPath[0];
	}
}

//*******************************************************************

void CServerData::SetExternalRequestPathFlag(int n, bool b)
{
	if ((n >= 1) && (n <= EPOSREQPATH_COUNT))
	{
		m_bExternalRequestPath[n - 1] = b;
	}
}

//*******************************************************************

bool CServerData::GetExternalRequestPathFlag(int n)
{
	if ((n >= 1) && (n <= EPOSREQPATH_COUNT))
	{
		return m_bExternalRequestPath[n - 1];
	}
	else
	{
		return m_bExternalRequestPath[0];
	}
}

//*******************************************************************

const char* CServerData::GetEposPathLabel(int n)
{
	if ((n >= 1) && (n <= EPOSREQPATH_COUNT))
	{
		return m_strEposPathLabel[n - 1];
	}
	else
	{
		return m_strEposPathLabel[0];
	}
}

//*******************************************************************

CString CServerData::GetDefaultEposPathLabel(int n)
{
	CString strResult = "Standard Folder Set";

	if ((n >= 2) && (n <= 10))
	{
		strResult.Format("Set %d", n);
	}

	return strResult;
}

//*******************************************************************

void CServerData::SetCurrentEposPathSet(int n)
{
	if ((n >= 1) && (n <= EPOSREQPATH_COUNT))
	{
		m_nCurrentEposPathSet = n;
	}
}

//*******************************************************************

const char* CServerData::GetCurrentEposRequestPath()
{
	return GetEposRequestPath(m_nCurrentEposPathSet);
}

//*******************************************************************

const char* CServerData::GetCurrentEposLogPath()
{
	return GetEposLogPath(m_nCurrentEposPathSet);
}

//*******************************************************************

bool CServerData::GetCurrentExternalRequestPathFlag()
{
	return GetExternalRequestPathFlag(m_nCurrentEposPathSet);
}

//*******************************************************************

void CServerData::SetEposPathLabel(int n, const char* sz)
{
	if ((n >= 1) && (n <= EPOSREQPATH_COUNT))
	{
		m_strEposPathLabel[n - 1] = sz;
	}
}

//*******************************************************************

CString CServerData::GetEcrManagerImportPluFilename()
{
	m_strEcrManagerImportPluFilename = m_strEcrManagerImportFolder;

	if (m_strEcrManagerImportPluFilename != "")
	{
		m_strEcrManagerImportPluFilename += "\\plupts.csv";
	}

	return m_strEcrManagerImportPluFilename;
}

//*******************************************************************

CString CServerData::GetEcrManagerImportOperatorFilename()
{
	m_strEcrManagerImportOperatorFilename = m_strEcrManagerImportFolder;

	if (m_strEcrManagerImportOperatorFilename != "")
	{
		m_strEcrManagerImportOperatorFilename += "\\opnames.csv";
	}

	return m_strEcrManagerImportOperatorFilename;
}

//*******************************************************************

CString CServerData::GetEcrManagerImportEposTerminalFilename()
{
	m_strEcrManagerImportEposTerminalFilename = m_strEcrManagerImportFolder;

	if (m_strEcrManagerImportEposTerminalFilename != "")
	{
		m_strEcrManagerImportEposTerminalFilename += "\\termnames.csv";
	}

	return m_strEcrManagerImportEposTerminalFilename;
}

//*******************************************************************

CString CServerData::GetEcrManagerImportPurchaseControlFilename()
{
	m_strEcrManagerImportPurchaseControlFilename = m_strEcrManagerImportFolder;

	if (m_strEcrManagerImportPurchaseControlFilename != "")
	{
		m_strEcrManagerImportPurchaseControlFilename += "\\purchasectrl.csv";
	}

	return m_strEcrManagerImportPurchaseControlFilename;
}

//*******************************************************************

void CServerData::SetPebbleDiagnosticType(int n)
{
	if ((n >= 0) && (n <= 2))
	{
		m_nPebbleDiagnosticType = n;
	}
}

//*******************************************************************

double CServerData::GetActiveRevaluationTrip()
{
	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		return 0.0;
	}
	else
	{
		return m_dFileRevaluationTrip;
	}
}

//*******************************************************************

double CServerData::GetActiveRevaluationTrip2()
{
	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		return 0.0;
	}
	else
	{
		return m_dFileRevaluationTrip2;
	}
}

//*******************************************************************

bool CServerData::RevaluationBonusEnabled()
{
	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		return FALSE;
	}

	if (::CompareDoubles(m_dFileRevaluationTrip, 0.0, 3) == 1)
	{
		return TRUE;
	}

	if (::CompareDoubles(m_dFileRevaluationTrip2, 0.0, 3) == 1)
	{
		return TRUE;
	}

	return FALSE;
}

//*******************************************************************

int CServerData::GetActiveRedeemPointsTrip()
{
	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		return 0;
	}
	else
	{
		return m_nFileRedeemPointsTrip;
	}
}

//*******************************************************************

double CServerData::GetActiveRewardTrip()
{
	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		return 0;
	}
	else
	{
		return m_dFileRewardTrip;
	}
}

//*******************************************************************

bool CServerData::SpendBonusEnabled()
{
	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		return FALSE;
	}

	if (CompareDoubles(m_dFileRewardTrip, 0.0, 3) == 1)
	{
		return TRUE;
	}

	return FALSE;
}

//*******************************************************************

bool CServerData::GetActiveEnablePurchaseHistoryFlag()
{
	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		return TRUE;
	}

	return m_bFileEnablePurchaseHistory;
}

//*******************************************************************

void CServerData::SetLockRetries(int n)
{
	if ((n >= 0) && (n <= 10))
	{
		m_nLockRetries = n;
	}
}

//*******************************************************************
