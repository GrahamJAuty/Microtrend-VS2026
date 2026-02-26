//*****************************************************************
#ifdef LOYSERVER
//*****************************************************************
#include "..\SPosLoyalty4Server\SPosLoyalty4Server.h"
//*****************************************************************
#else
//*****************************************************************
#include "..\SPosLoyalty4Manager\LoyaltyManager.h"
//*****************************************************************
#endif
//*****************************************************************
#include "SQLTable_EcrmanDatabase\SQLRepositoryEcrmanDatabase.h"
//*****************************************************************
#include "LoyaltyHeaderHelpers.h"
#include "SQLNamespaces.h"
//*****************************************************************
int CServerData::m_nSimulateReaderType = 0;
//*****************************************************************

CServerDataEposPathInfo::CServerDataEposPathInfo()
{
	m_strPathLabel = "";
	m_strReqPath = "";
	m_strLogPath = "";
	m_bIsExternal = FALSE;
}

//*****************************************************************

CServerDataPosDisplayInfo::CServerDataPosDisplayInfo()
{
	m_strField = "";
	m_strText = "";
}
//*****************************************************************

CServerDataTopupInfo::CServerDataTopupInfo()
{
	m_strPluNo = "";
	m_dTrip = 0.0;
	m_nType = 0;
	m_nPoints = 0;
	m_dValue = 0.0;
	m_nPercent = 0;
}

//*****************************************************************

CServerDataPluRenewalInfo::CServerDataPluRenewalInfo()
{
	m_strPluNo = "";
	m_nMultiplier = 0;
	m_nType = 0;
	m_nYearDayNumber = 0;
	m_nFixedYear = 0;
}

//*****************************************************************

CServerDataFolderSetIndex::CServerDataFolderSetIndex()
{
	Reset();
}

//*****************************************************************

void CServerDataFolderSetIndex::Reset()
{
	m_nDbNo = 1;
	m_nSetNo = 1;
}

//*****************************************************************
//*****************************************************************
//*****************************************************************

CServerData::CServerData(void)
{
	//TIME STAMP
	m_strTimeStamp = "***";
	
	//COMMUNICATIONS TAB
	m_bLogAction = FALSE;
	m_bRealTimePlus = FALSE;
	m_bAllowOverSpend = FALSE;
	m_bNewBalanceOnReceipt = TRUE;
	m_nLoyaltyLogSaveType = 0;
	m_CurrentFolderSetIndex.Reset();

	//UNKNOWN CARDS
	m_bAddUnknownCards = TRUE;
	m_bUnknownCardGroupRSP = FALSE;
	m_nUnknownCardGroupNo = 0;
	m_nUnknownCardMinLength = 1;
	m_nUnknownCardMinID = 1;
	m_nUnknownCardMaxID = 999999999999;
	m_strUnknownAccountLine1 = "";
	m_strUnknownAccountLine2 = "";
	m_strUnknownAccountLine3 = "";
	m_strUnknownAccountPendingName = "";
	m_strUnknownAccountCreateName = "";

	//POS DISPLAY
	m_nPhotoIDFolderType = 0;
	m_strLoyaltyUNCSystemPath = "";
	m_strLoyaltyUNCImagePath = "";
	
	//PLU SALES TAB
	for (int p = 0; p <= 2; p++)
	{
		for (int t = 1; t <= REVALUE_PLUNO_PERPURSE_COUNT; t++)
		{
			SetTopupPluNo(p, t, "0");
			SetTopupBonusTrip(p, t, 0.0);
			SetTopupBonusType(p, t, 0);
			SetTopupBonusPoints(p, t, 0);
			SetTopupBonusValue(p, t, 0.0);
			SetTopupBonusPercent(p, t, 0);
		}
	}

	m_nTopupPCBonusTypePurse1 = 0;
	m_nTopupPCBonusTypePurse2 = 0;
	m_strTopupCommentPurse1 = "";
	m_strTopupCommentPurse2 = "";
	m_strStaffGiftPluNo = "";
	m_strStaffGiftRedirectPluNo = "";
	m_strEcrManagerImportFolder = "";
	m_bEcrManagerGlobalDept = FALSE;
	m_bEcrManagerGlobalStampOffer = FALSE;
	m_bEnablePluBonus = FALSE;
	m_bEnableDeptBonus = FALSE;
	m_bNoPurseBonus = FALSE;
	m_bPurchaseHistory = TRUE;
	m_bSpendBonusToPurse2 = FALSE;
	m_bSpendUsePurse1First = FALSE;
	
	//RENEWAL TAB
	for (int n = 1; n <= RENEW_PLUNO_COUNT; n++)
	{
		SetRenewPluNo(n, "0");
		SetRenewMultiplier(n, 1);
		SetRenewType(n, 0);
		SetRenewYearDayNumber(n, 1);
	}
	
	//SPEND BONUS TAB
	m_bRewardPurse1Spend = TRUE;
	m_bRewardPurse2Spend = FALSE;
	m_bRewardCashSpend = FALSE;
	m_bRewardManualSpend = FALSE;
	m_bRewardNoBonusWithPluPoints = FALSE;
	m_dRewardUpperSpendLimit = 0.0;
	m_dRewardTrip = 0.0;
	m_nRewardType = 0;
	m_nRewardPoints = 0;
	m_dRewardValue = 0.0;
	m_strRewardComment = "";
	m_dRewardMultiplierTrigger = 0.0;
	m_nRewardMultiplierMon = 1;
	m_nRewardMultiplierTue = 1;
	m_nRewardMultiplierWed = 1;
	m_nRewardMultiplierThu = 1;
	m_nRewardMultiplierFri = 1;
	m_nRewardMultiplierSat = 1;
	m_nRewardMultiplierSun = 1;
	m_timeRewardMonStart.SetTime("00:00");
	m_timeRewardMonEnd.SetTime("23:59");
	m_timeRewardTueStart.SetTime("00:00");
	m_timeRewardTueEnd.SetTime("23:59");
	m_timeRewardWedStart.SetTime("00:00");
	m_timeRewardWedEnd.SetTime("23:59");
	m_timeRewardThuStart.SetTime("00:00");
	m_timeRewardThuEnd.SetTime("23:59");
	m_timeRewardFriStart.SetTime("00:00");
	m_timeRewardFriEnd.SetTime("23:59");
	m_timeRewardSatStart.SetTime("00:00");
	m_timeRewardSatEnd.SetTime("23:59");
	m_timeRewardSunStart.SetTime("00:00");
	m_timeRewardSunEnd.SetTime("23:59");

	//REDEMPTION TAB
	m_nRedeemPointsTrip = 0;
	m_dRedeemPointsValue = 0.0;
	m_nRedeemPointsType = 0;
	m_strRedeemComment = "";
	m_bRedeemDisableAuto = FALSE;

	//SEARCH TAB
	m_bSearchSnakesAndLadders = FALSE;
	m_bSearchExcludeHotlist = FALSE;
	m_bSearchExcludeInactive = FALSE;
	m_bSearchSurnameMatchFullname = FALSE;
	
	//NON UI
	m_nSearchMax = 3;
	m_nLockRetries = 0;
	m_bThreadDiagnostics = FALSE;
	m_nLockFolderTimeout = 15;
	m_bSPosPointsAward = FALSE;
	m_bSPosPointsRedeem = FALSE;
	m_bSPosPointsNoLog = FALSE;
	m_bCopyLogFiles = FALSE;
	m_bSimpsInnsLookup = FALSE;
	m_bBypassVersionSafetyCheck = FALSE;
	m_bLogCardReads = FALSE;
	m_bDoubleBonusWithHash = FALSE;
	m_strDoubleBonusBeginDate = "20250101";
	m_strDoubleBonusEndDate = "20250101";

	//OBSOLETE
	m_nObsoleteCardNoFormat = 0;
	m_bObsoleteCardCheckDigit = FALSE;
	m_strObsoleteCardNoPrefix = "";
}

//*****************************************************************

CString CServerData::GetIniStringWithFallback(CSSIniFile& file, const CString& strKey, const CString& strFallbackKey, const CString& strDefault)
{
	CString str = file.GetString(strKey, "");
	if (str == "")
	{
		str = file.GetString(strFallbackKey, strDefault);
	}
	return str;
}

//*****************************************************************

bool CServerData::GetIniBoolWithFallback(CSSIniFile& file, const CString& strKey, const CString& strFallbackKey, int nDefault)
{
	int n = file.GetInt(strKey, -1);
	if (-1 == n)
	{
		n = file.GetInt(strFallbackKey, nDefault);
	}
	return (n == 1);
}

//*****************************************************************

void CServerData::SetEcrManagerImportFolder(CString str)
{
	CString strTest = str;
	strTest.MakeLower();

	if ((strTest.Right(6) == "\\db001") || (strTest.Right(6) == "/db001"))
	{
		str = str.Left(str.GetLength() - 6);
	}

	m_strEcrManagerImportFolder = str;	
}

//*****************************************************************

bool CServerData::Read(CString strDatapath)
{
	CSSIniFile file;
	bool bReply = file.Read(strDatapath);

	CString strTimeStamp = file.GetString(ServerDataFields::TimeStamp, "");

	if (strTimeStamp == m_strTimeStamp)
	{
		return bReply;
	}

	//COMMUNICATIONS TAB
	SetLogActionFlag(file.GetBool(ServerDataFields::LogAction, FALSE));
	SetRealTimePlusFlag(file.GetBool(ServerDataFields::RealTimePlus, FALSE));
	SetAllowOverSpendFlag(file.GetBool(ServerDataFields::OverSpend, FALSE));
	SetNewBalanceOnReceiptFlag(file.GetBool(ServerDataFields::NewBalanceOnReceipt, TRUE));
	SetLoyaltyLogSaveType(file.GetInt(ServerDataFields::LoyaltyLogSaveType, 0));

	{
		CString strPathLabel = ServerDataFields::EposPathLabel;
		CString strLoyReq = ServerDataFields::EposRequestPath;
		CString strLoyLog = ServerDataFields::EposLogPath;
		CString strExtReq = ServerDataFields::ExternalRequestPath;
		
		CServerDataFolderSetIndex FolderSetIndex;

		for (int nDbNo = 1; nDbNo <= System.GetMaxEcrmanDatabases(); nDbNo++)
		{
			CString strDbNo = "";
			strDbNo.Format("DB%d_", nDbNo);
			FolderSetIndex.m_nDbNo = nDbNo;
			FolderSetIndex.m_nSetNo = 1;

			if (1 == nDbNo)
			{
				SetEposPathLabel(FolderSetIndex, GetIniStringWithFallback(file, strDbNo + strPathLabel, strPathLabel, "Set 1"));
				SetEposRequestPath(FolderSetIndex, GetIniStringWithFallback(file, strDbNo + strLoyReq, strLoyReq, ""));
				SetEposLogPath(FolderSetIndex, GetIniStringWithFallback(file, strDbNo + strLoyLog, strLoyLog, ""));
				SetExternalRequestPathFlag(FolderSetIndex, GetIniBoolWithFallback(file, strDbNo + strExtReq, strExtReq, 0));
			}
			else
			{
				SetEposPathLabel(FolderSetIndex, file.GetString(strDbNo + strPathLabel, "Set 1"));
				SetEposRequestPath(FolderSetIndex, file.GetString(strDbNo + strLoyReq, ""));
				SetEposLogPath(FolderSetIndex, file.GetString(strDbNo + strLoyLog, ""));
				SetExternalRequestPathFlag(FolderSetIndex, file.GetInt(strDbNo + strExtReq, FALSE));
			}

			CString strSet = "Set ";
			for (int nSetNo = 2; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
			{
				CString strSetNo = "";
				strSetNo.Format("%d", nSetNo);
				FolderSetIndex.m_nSetNo = nSetNo;
				
				if (1 == nDbNo)
				{
					SetEposPathLabel(FolderSetIndex, GetIniStringWithFallback(file, strDbNo + strPathLabel + strSetNo, strPathLabel + strSetNo, strSet + strSetNo));
					SetEposRequestPath(FolderSetIndex, GetIniStringWithFallback(file, strDbNo + strLoyReq + strSetNo, strLoyReq + strSetNo, ""));
					SetEposLogPath(FolderSetIndex, GetIniStringWithFallback(file, strDbNo + strLoyLog + strSetNo, strLoyLog + strSetNo, ""));
					SetExternalRequestPathFlag(FolderSetIndex, GetIniBoolWithFallback(file, strDbNo + strExtReq + strSetNo, strExtReq + strSetNo, FALSE));
				}
				else
				{
					SetEposPathLabel(FolderSetIndex, file.GetString(strDbNo + strPathLabel + strSetNo, strSet + strSetNo));
					SetEposRequestPath(FolderSetIndex, file.GetString(strDbNo + strLoyReq + strSetNo, ""));
					SetEposLogPath(FolderSetIndex, file.GetString(strDbNo + strLoyLog + strSetNo, ""));
					SetExternalRequestPathFlag(FolderSetIndex, file.GetBool(strDbNo + strExtReq + strSetNo, FALSE));
				}
			}
		}
	}

	for (int nDbNo = System.GetMaxEcrmanDatabases() + 1; nDbNo <= PLU_DBNO_COUNT; nDbNo++)
	{
		CServerDataFolderSetIndex FolderSetIndex;
		FolderSetIndex.m_nDbNo = nDbNo;

		for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
		{
			CString strSetNo = "";
			strSetNo.Format("Set %d", nSetNo);

			FolderSetIndex.m_nSetNo = nSetNo;
			SetEposPathLabel(FolderSetIndex, strSetNo);
			SetEposRequestPath(FolderSetIndex, "");
			SetEposLogPath(FolderSetIndex, "");
			SetExternalRequestPathFlag(FolderSetIndex, FALSE);
		}
	}	

	//UNKNOWN CARDS
	SetAddUnknownCardsFlag(file.GetBool(ServerDataFields::AddUnknowns, TRUE));
	SetUnknownCardGroupRSPFlag(file.GetBool(ServerDataFields::UnknownCardGroupRSP, FALSE));
	SetUnknownCardGroupNo(file.GetInt(ServerDataFields::UnknownCardGroupNo, 0));
	SetUnknownCardMinLength(file.GetInt(ServerDataFields::UnknownCardMinLength, 1));
	SetUnknownCardMinID(file.GetInt64(ServerDataFields::UnknownCardMinID, 1));
	SetUnknownCardMaxID(file.GetInt64(ServerDataFields::UnknownCardMaxID, 999999999999));

	if (0 == GetUnknownCardGroupNo())
	{
		SetUnknownCardGroupNo(file.GetInt("UnknownGroup", 1));
	}

	SetUnknownAccountLine1(file.GetString(ServerDataFields::UnknownAccountLine1, ""));
	SetUnknownAccountLine2(file.GetString(ServerDataFields::UnknownAccountLine2, ""));
	SetUnknownAccountLine3(file.GetString(ServerDataFields::UnknownAccountLine3, ""));
	SetUnknownAccountPendingName(file.GetString(ServerDataFields::UnknownAccountPendingName, ""));
	SetUnknownAccountCreateName(file.GetString(ServerDataFields::UnknownAccountCreateName, ""));

	//POS DISPLAY TAB
	SetPosDisplayField(1, file.GetString(ServerDataFields::Field1, Account::Fullname.Label));
	SetPosDisplayField(2, file.GetString(ServerDataFields::Field2, "<blank>"));
	SetPosDisplayField(3, file.GetString(ServerDataFields::Field3, "<blank>"));
	SetPosDisplayField(4, file.GetString(ServerDataFields::Field4, "<blank>"));
	SetPosDisplayField(5, file.GetString(ServerDataFields::Field5, "<blank>"));
	SetPosDisplayField(6, file.GetString(ServerDataFields::Field6, "<blank>"));
	SetPosDisplayField(7, file.GetString(ServerDataFields::Field7, "<blank>"));
	SetPosDisplayField(8, file.GetString(ServerDataFields::Field8, "<blank>"));
	SetPosDisplayText(1, file.GetString(ServerDataFields::Text1, ""));
	SetPosDisplayText(2, file.GetString(ServerDataFields::Text2, ""));
	SetPosDisplayText(3, file.GetString(ServerDataFields::Text3, ""));
	SetPosDisplayText(4, file.GetString(ServerDataFields::Text4, ""));
	SetPosDisplayText(5, file.GetString(ServerDataFields::Text5, ""));
	SetPosDisplayText(6, file.GetString(ServerDataFields::Text6, ""));
	SetPosDisplayText(7, file.GetString(ServerDataFields::Text7, ""));
	SetPosDisplayText(8, file.GetString(ServerDataFields::Text8, ""));
	SetPhotoIDFolderType(file.GetInt(ServerDataFields::PhotoIDFolderType, 0));
	SetLoyaltyUNCSystemPath(file.GetString(ServerDataFields::LoyaltyUNCSystemPath, ""));
	SetLoyaltyUNCImagePath(file.GetString(ServerDataFields::LoyaltyUNCImagePath, ""));
	
	//TOPUPS
	for (int p = 1; p <= 2; p++)
	{
		CString strPurse = "";
		strPurse.Format("Purse%d_", p);

		if (1 == p)
		{
			SetTopupPCBonusTypePurse1(file.GetInt(strPurse + ServerDataFields::TopupPCBonusType, 0));
			SetTopupCommentPurse1(file.GetString(strPurse + ServerDataFields::TopupComment, ""));
		}
		else
		{
			SetTopupPCBonusTypePurse2(file.GetInt(strPurse + ServerDataFields::TopupPCBonusType, 0));
			SetTopupCommentPurse2(file.GetString(strPurse + ServerDataFields::TopupComment, ""));
		}
		
		for (int t = 1; t <= REVALUE_PLUNO_PERPURSE_COUNT; t++)
		{
			CString strTopup = "";
			strTopup.Format("Topup%d_", t);

			SetTopupPluNo(p, t, file.GetString(strPurse + strTopup + ServerDataFields::TopupPlu, "0"));
			SetTopupBonusTrip(p, t, file.GetDouble(strPurse + strTopup + ServerDataFields::TopupBonusTrip, 0.0));
			SetTopupBonusType(p, t, file.GetInt(strPurse + strTopup + ServerDataFields::TopupBonusType, 0));	
			SetTopupBonusPoints(p, t, file.GetInt(strPurse + strTopup + ServerDataFields::TopupBonusPoints, 0));
			SetTopupBonusValue(p, t, file.GetDouble(strPurse + strTopup + ServerDataFields::TopupBonusValue, 0.0));
			SetTopupBonusPercent(p, t, file.GetInt(strPurse + strTopup + ServerDataFields::TopupBonusPercent, 0));
		}
	}

	//LEGACY TOPUPS
	if (file.GetBool(ServerDataFields::NoLegacyTopups, FALSE) == FALSE)
	{
		CheckLegacyTopups(file);
	}

	SetStaffGiftPluNo(file.GetString(ServerDataFields::StaffGiftPluNo, "0"));
	SetStaffGiftRedirectPluNo(file.GetString(ServerDataFields::StaffGiftRedirectPluNo, "0"));
	SetEcrManagerImportFolder(file.GetString(ServerDataFields::EcrManagerImportFolder, ""));
	SetEcrManagerGlobalDeptFlag(file.GetBool(ServerDataFields::EcrManagerGlobalDept, FALSE));
	SetEcrManagerGlobalStampOfferFlag(file.GetBool(ServerDataFields::EcrManagerGlobalStampOffer, FALSE));
	SetEnablePluBonusFlag(file.GetBool(ServerDataFields::EnablePluBonus, FALSE));
	SetEnableDeptBonusFlag(file.GetBool(ServerDataFields::EnableDeptBonus, FALSE));
	SetNoPurseBonusFlag(file.GetBool(ServerDataFields::NoPurseBonus, FALSE));
	SetPurchaseHistoryFlag(file.GetBool(ServerDataFields::PurchaseHistory, TRUE));
	SetSpendBonusToPurse2Flag(file.GetBool(ServerDataFields::SpendBonusToPurse2, FALSE));
	SetSpendUsePurse1FirstFlag(file.GetBool(ServerDataFields::SpendUsePurse1First, FALSE));

	if (m_strEcrManagerImportFolder == "")
	{
		CString strPluUpdateFilename = file.GetString("PluUpdate", "");

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

	m_strStaffGiftPluNo.TrimLeft('0');
	m_strStaffGiftRedirectPluNo.TrimLeft('0');
	
	for (int p = 1; p <= 2; p++)
	{
		for (int t = 1; t <= REVALUE_PLUNO_PERPURSE_COUNT; t++)
		{
			m_TopupInfo[p][t].m_strPluNo.TrimLeft('0');
		}
	}

	//RENEWAL TAB
	for (int n = 1; n <= RENEW_PLUNO_COUNT; n++)
	{
		CString strLabel;
		strLabel.Format("%s%d", ServerDataFields::RenewPluNo, n);
		SetRenewPluNo(n, file.GetString(strLabel, "0"));

		strLabel.Format("%s%d", ServerDataFields::RenewMultiplier, n);
		SetRenewMultiplier(n, file.GetInt(strLabel, 1));

		strLabel.Format("%s%d", ServerDataFields::RenewType, n);
		SetRenewType(n, file.GetInt(strLabel, 0));

		{
			strLabel.Format("%s%d", ServerDataFields::RenewYearDayNumber, n);
			int nRenewDay = file.GetInt(strLabel, -1);

			if (-1 == nRenewDay)
			{
				strLabel.Format("RenewYearDay%d", n);
				nRenewDay = file.GetInt(strLabel, 1);

				//ADJUST LEGACY DAY NUMBER TO ALLOW FOR LEAP YEARS
				if (nRenewDay >= 60)
				{
					nRenewDay++;
				}
			}

			SetRenewYearDayNumber(n, nRenewDay);
		}

		strLabel.Format("%s%d", ServerDataFields::RenewFixedYear, n);
		SetRenewFixedYear(n, file.GetInt(strLabel, 1));
	}

	for (int n = 1; n <= RENEW_PLUNO_COUNT; n++)
	{
		m_PluRenewalInfo[n].m_strPluNo.TrimLeft('0');
	}

	//SPEND BONUS TAB
	SetRewardPurse1SpendFlag(file.GetBool(ServerDataFields::RewardPurse1Spend, TRUE));
	SetRewardPurse2SpendFlag(file.GetBool(ServerDataFields::RewardPurse2Spend, FALSE));
	SetRewardCashSpendFlag(file.GetBool(ServerDataFields::RewardCashSpend, FALSE));
	SetRewardManualSpendFlag(file.GetBool(ServerDataFields::RewardManualSpend, FALSE));
	SetRewardUpperSpendLimit(file.GetDouble(ServerDataFields::RewardUpperSpendLimit, 0.0));
	SetRewardNoBonusWithPluPointsFlag(file.GetBool(ServerDataFields::RewardNoBonusWithPluPoints, FALSE));
	SetRewardTrip(file.GetDouble(ServerDataFields::RewardTrip, 0.0));
	SetRewardType(file.GetInt(ServerDataFields::RewardType, 0));
	SetRewardPoints(file.GetInt(ServerDataFields::RewardPoints, 0));
	SetRewardValue(file.GetDouble(ServerDataFields::RewardValue, 0.0));
	SetRewardComment(file.GetString(ServerDataFields::RewardComment, ""));
	SetRewardMultiplierTrigger(file.GetDouble(ServerDataFields::RewardMultiplierTrigger, 0.0));
	SetRewardMultiplierMon(file.GetInt(ServerDataFields::RewardMultiplierMon, 1));
	SetRewardMultiplierTue(file.GetInt(ServerDataFields::RewardMultiplierTue, 1));
	SetRewardMultiplierWed(file.GetInt(ServerDataFields::RewardMultiplierWed, 1));
	SetRewardMultiplierThu(file.GetInt(ServerDataFields::RewardMultiplierThu, 1));
	SetRewardMultiplierFri(file.GetInt(ServerDataFields::RewardMultiplierFri, 1));
	SetRewardMultiplierSat(file.GetInt(ServerDataFields::RewardMultiplierSat, 1));
	SetRewardMultiplierSun(file.GetInt(ServerDataFields::RewardMultiplierSun, 1));
	SetRewardMonStart(CSSTime(file.GetString(ServerDataFields::RewardMonStart, "00:00")));
	SetRewardMonEnd(CSSTime(file.GetString(ServerDataFields::RewardMonEnd, "23:59")));
	SetRewardTueStart(CSSTime(file.GetString(ServerDataFields::RewardTueStart, "00:00")));
	SetRewardTueEnd(CSSTime(file.GetString(ServerDataFields::RewardTueEnd, "23:59")));
	SetRewardWedStart(CSSTime(file.GetString(ServerDataFields::RewardWedStart, "00:00")));
	SetRewardWedEnd(CSSTime(file.GetString(ServerDataFields::RewardWedEnd, "23:59")));
	SetRewardThuStart(CSSTime(file.GetString(ServerDataFields::RewardThuStart, "00:00")));
	SetRewardThuEnd(CSSTime(file.GetString(ServerDataFields::RewardThuEnd, "23:59")));
	SetRewardFriStart(CSSTime(file.GetString(ServerDataFields::RewardFriStart, "00:00")));
	SetRewardFriEnd(CSSTime(file.GetString(ServerDataFields::RewardFriEnd, "23:59")));
	SetRewardSatStart(CSSTime(file.GetString(ServerDataFields::RewardSatStart, "00:00")));
	SetRewardSatEnd(CSSTime(file.GetString(ServerDataFields::RewardSatEnd, "23:59")));
	SetRewardSunStart(CSSTime(file.GetString(ServerDataFields::RewardSunStart, "00:00")));
	SetRewardSunEnd(CSSTime(file.GetString(ServerDataFields::RewardSunEnd, "23:59")));

	//REDEMPION TAB
	SetRedeemPointsTrip(file.GetInt(ServerDataFields::RedeemPointsTrip, 0));
	SetRedeemPointsValue(file.GetDouble(ServerDataFields::RedeemPointsValue, 0.0));
	SetRedeemComment(file.GetString(ServerDataFields::RedeemComment, ""));
	SetRedeemDisableAutoFlag(file.GetBool(ServerDataFields::RedeemDisableAuto, FALSE));
	
	//SEARCH TAB
	SetSearchSnakesAndLaddersFlag(file.GetBool(ServerDataFields::SearchSnakesAndLadders, FALSE));
	SetSearchExcludeHotlistFlag(file.GetBool(ServerDataFields::SearchExcludeHotlist, FALSE));
	SetSearchExcludeInactiveFlag(file.GetBool(ServerDataFields::SearchExcludeInactive, FALSE));
	SetSearchSurnameMatchFullnameFlag(file.GetBool(ServerDataFields::SearchSurnameMatchFullname, FALSE));

	//NON UI
	SetSearchMax(file.GetInt(ServerDataFields::SearchMax, 3));
	SetLockRetries(file.GetInt(ServerDataFields::LockRetries, 0));
	SetThreadDiagnosticsFlag(file.GetBool(ServerDataFields::ThreadDiagnostics, FALSE));
	SetLockFolderTimeout(file.GetInt(ServerDataFields::LockFolderTimeout, 15));
	SetSPosPointsAwardFlag(file.GetBool(ServerDataFields::SPOSPointsAward, FALSE));
	SetSPosPointsRedeemFlag(file.GetBool(ServerDataFields::SPOSPointsRedeem, FALSE));
	SetSPosPointsNoLogFlag(file.GetBool(ServerDataFields::SPOSPointsNoLog, FALSE));
	SetSimulateReaderType(file.GetInt(ServerDataFields::SimulateReaderType, 0));
	SetCopyLogFilesFlag(file.GetBool(ServerDataFields::CopyLogFiles, FALSE));
	SetSimpsInnsLookupFlag(file.GetBool(ServerDataFields::SimpsInnsLookup, FALSE));
	SetLogCardReadsFlag(file.GetBool(ServerDataFields::LogCardReads, FALSE));	
	SetDoubleBonusWithHashFlag(file.GetBool(ServerDataFields::DoubleBonusWithHash, FALSE));
	SetDoubleBonusBeginDate(file.GetString(ServerDataFields::DoubleBonusBeginDate, "20250101"));
	SetDoubleBonusEndDate(file.GetString(ServerDataFields::DoubleBonusEndDate, "20250101"));

	{
		CString strTest = file.GetString(ServerDataFields::BypassVersionSafetyCheck, "");
		strTest.MakeUpper();
		SetBypassVersionSafetyCheckFlag(strTest == "<*YES*>");
	}

	//OBSOLETE
	SetObsoleteCardCheckDigitFlag(file.GetBool(ServerDataFields::ObsoleteCardCheckDigit, FALSE));
	SetObsoleteCardNoFormat(file.GetInt(ServerDataFields::ObsoleteCardNoFormat, 0));
	SetObsoleteCardNoPrefix(file.GetString(ServerDataFields::ObsoleteCardNoPrefix, ""));
	
	return bReply;
}

//*******************************************************************

void CServerData::CheckLegacyTopups(CSSIniFile& file)
{
	{
		CString strComment = file.GetString(ServerDataFields::LegacayRevalueComment, "");
		if (strComment != "")
		{
			SetTopupCommentPurse1(strComment);
		}
	}

	{
		int nPCBonusType = file.GetInt(ServerDataFields::LegacyRevaluePCBonusType, -1);

		if (nPCBonusType != -1)
		{
			SetTopupPCBonusTypePurse1(nPCBonusType);
		}
	}

	for (int n = 1; n <= 4; n++)
	{
		CString strLabel = "";

		{
			strLabel.Format("%s%d", ServerDataFields::LegacyRevaluePlu, n);
			CString strPluNo = file.GetString(strLabel, "");

			if (strPluNo == "")
			{
				strLabel.Format("RevaluePlu%d", n);
				strPluNo = file.GetString(strLabel, "");
			}

			if (strPluNo != "")
			{
				SetTopupPluNo(1, n, strPluNo);
			}
		}

		{
			strLabel.Format("%s%d", ServerDataFields::LegacyRevalueTrip, n);
			double dTrip = file.GetDouble(strLabel, 0.0);

			if (CompareDoubles(dTrip, 0.0, 2) == 1)
			{
				SetTopupBonusTrip(1, n, dTrip);
			}
		}

		{
			strLabel.Format("%s%d", ServerDataFields::LegacyRevalueType, n);
			int nType = file.GetInt(strLabel, -1);

			if (nType != -1)
			{
				SetTopupBonusType(1, n, nType);
			}
		}

		{
			strLabel.Format("%s%d", ServerDataFields::LegacyRevaluePoints, n);
			int nPoints = file.GetInt(strLabel, -1);

			if (nPoints != -1)
			{
				SetTopupBonusPoints(1, n, nPoints);
			}
		}

		{
			strLabel.Format("%s%d", ServerDataFields::LegacyRevalueValue, n);
			double dValue = file.GetDouble(strLabel, 0.0);

			if (CompareDoubles(dValue, 0.0, 2) == 1)
			{
				SetTopupBonusValue(1, n, dValue);
			}
		}

		{
			strLabel.Format("%s%d", ServerDataFields::LegacyRevaluePercent, n);
			int nPercent = file.GetInt(strLabel, -1);

			if (nPercent != -1)
			{
				SetTopupBonusPercent(1, n, nPercent);
			}
		}
	}

	int nTestLegacy = file.GetInt(ServerDataFields::LegacyRevalueType, -1);
	if (nTestLegacy != -1)
	{
		SetTopupBonusType(1, 1, nTestLegacy);
		SetTopupBonusType(1, 2, nTestLegacy);

		int nPoints = file.GetInt(ServerDataFields::LegacyRevaluePoints, 0);
		SetTopupBonusPoints(1, 1, nPoints);
		SetTopupBonusPoints(1, 2, nPoints);

		double dValue = file.GetDouble(ServerDataFields::LegacyRevalueValue, 0.0);
		SetTopupBonusValue(1, 1, dValue);
		SetTopupBonusValue(1, 2, dValue);

		int nPercent = file.GetInt(ServerDataFields::LegacyRevaluePercent, 0);
		SetTopupBonusPercent(1, 1, nPercent);
		SetTopupBonusPercent(1, 2, nPercent);
	}

	if (m_TopupInfo[1][1].m_strPluNo == "0")
	{
		SetTopupPluNo(1, 1, file.GetString("PluNo", "0"));
	}

	if (m_TopupInfo[1][2].m_strPluNo == "0")
	{
		SetTopupPluNo(1, 2, file.GetString("PluNo2", "0"));
	}

	if (m_TopupInfo[1][1].m_dTrip == 0.0)
	{
		SetTopupBonusTrip(1, 1, file.GetDouble("RevalueTrip", 0.0));
	}
}

//*******************************************************************

bool CServerData::Write(CString strDatapath)
{
	for (int p = 1; p <= 2; p++)
	{
		for (int t = 1; t <= REVALUE_PLUNO_PERPURSE_COUNT; t++)
		{
			m_TopupInfo[p][t].m_strPluNo.TrimLeft('0');
		}
	}

	for (int n = 1; n < RENEW_PLUNO_COUNT; n++)
	{
		m_PluRenewalInfo[n].m_strPluNo.TrimLeft('0');
	}

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
	
	//EPOS VALIDATION
	file.Set(ServerDataFields::LogAction, GetLogActionFlag());
	file.Set(ServerDataFields::RealTimePlus, GetRealTimePlusFlag());
	file.Set(ServerDataFields::OverSpend, GetAllowOverSpendFlag());
	file.Set(ServerDataFields::NewBalanceOnReceipt, GetNewBalanceOnReceiptFlag());
	file.Set(ServerDataFields::LoyaltyLogSaveType, GetLoyaltyLogSaveType());
	
	{
		CString strPathLabel = ServerDataFields::EposPathLabel;
		CString strLoyReq = ServerDataFields::EposRequestPath;
		CString strLoyLog = ServerDataFields::EposLogPath;
		CString strExtReq = ServerDataFields::ExternalRequestPath;
		
		CServerDataFolderSetIndex FolderSetIndex;

		for (int nDbNo = 1; nDbNo <= PLU_DBNO_COUNT; nDbNo++)
		{
			CString strDbNo = "";
			strDbNo.Format("DB%d_", nDbNo);
			FolderSetIndex.m_nDbNo = nDbNo;

			for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
			{
				FolderSetIndex.m_nSetNo = nSetNo;

				CString strSetNo = "";

				if (nSetNo != 1)
				{
					strSetNo.Format("%d", nSetNo);
				}

				file.Set(strDbNo + strPathLabel + strSetNo, GetEposPathLabel(FolderSetIndex));
				file.Set(strDbNo + strLoyReq + strSetNo, GetEposRequestPath(FolderSetIndex));
				file.Set(strDbNo + strLoyLog + strSetNo, GetEposLogPath(FolderSetIndex));
				file.Set(strDbNo + strExtReq + strSetNo, GetExternalRequestPathFlag(FolderSetIndex));
			}
		}
	}

	//UNKNOWN CARDS
	file.Set(ServerDataFields::AddUnknowns, GetAddUnknownCardsFlag());
	file.Set(ServerDataFields::UnknownCardGroupRSP, GetUnknownCardGroupRSPFlag());
	file.Set(ServerDataFields::UnknownCardGroupNo, GetUnknownCardGroupNo());
	file.Set(ServerDataFields::UnknownCardMinLength, GetUnknownCardMinLength());
	file.Set(ServerDataFields::UnknownCardMinID, GetUnknownCardMinID());
	file.Set(ServerDataFields::UnknownCardMaxID, GetUnknownCardMaxID());
	file.Set(ServerDataFields::UnknownAccountLine1, GetUnknownAccountLine1());
	file.Set(ServerDataFields::UnknownAccountLine2, GetUnknownAccountLine2());
	file.Set(ServerDataFields::UnknownAccountLine3, GetUnknownAccountLine3());
	file.Set(ServerDataFields::UnknownAccountPendingName, GetUnknownAccountPendingName());
	file.Set(ServerDataFields::UnknownAccountCreateName, GetUnknownAccountCreateName());

	//POS DISPLAY
	file.Set(ServerDataFields::Field1, GetPosDisplayField(1));
	file.Set(ServerDataFields::Field2, GetPosDisplayField(2));
	file.Set(ServerDataFields::Field3, GetPosDisplayField(3));
	file.Set(ServerDataFields::Field4, GetPosDisplayField(4));
	file.Set(ServerDataFields::Field5, GetPosDisplayField(5));
	file.Set(ServerDataFields::Field6, GetPosDisplayField(6));
	file.Set(ServerDataFields::Field7, GetPosDisplayField(7));
	file.Set(ServerDataFields::Field8, GetPosDisplayField(8));
	file.Set(ServerDataFields::Text1, GetPosDisplayText(1));
	file.Set(ServerDataFields::Text2, GetPosDisplayText(2));
	file.Set(ServerDataFields::Text3, GetPosDisplayText(3));
	file.Set(ServerDataFields::Text4, GetPosDisplayText(4));
	file.Set(ServerDataFields::Text5, GetPosDisplayText(5));
	file.Set(ServerDataFields::Text6, GetPosDisplayText(6));
	file.Set(ServerDataFields::Text7, GetPosDisplayText(7));
	file.Set(ServerDataFields::Text8, GetPosDisplayText(8));
	file.Set(ServerDataFields::PhotoIDFolderType, GetPhotoIDFolderType());
	file.Set(ServerDataFields::LoyaltyUNCSystemPath, GetLoyaltyUNCSystemPath());
	file.Set(ServerDataFields::LoyaltyUNCImagePath, GetLoyaltyUNCImagePath());
	
	//PLU SALES TAB
	file.Set(ServerDataFields::NoLegacyTopups, TRUE);

	for (int p = 1; p <= 2; p++)
	{
		CString strPurse = "";
		strPurse.Format("Purse%d_", p);

		if (1 == p)
		{
			file.Set(strPurse + ServerDataFields::TopupPCBonusType, GetTopupPCBonusTypePurse1());
			file.Set(strPurse + ServerDataFields::TopupComment, GetTopupCommentPurse1());
		}
		else
		{
			file.Set(strPurse + ServerDataFields::TopupPCBonusType, GetTopupPCBonusTypePurse2());
			file.Set(strPurse + ServerDataFields::TopupComment, GetTopupCommentPurse2());
		}

		for (int t = 1; t <= REVALUE_PLUNO_PERPURSE_COUNT; t++)
		{
			CString strTopup = "";
			strTopup.Format("Topup%d_", t);

			CString strPrefix = strPurse + strTopup;

			file.Set(strPrefix + ServerDataFields::TopupPlu, GetTopupPluNo(p, t));
			file.Set(strPrefix + ServerDataFields::TopupBonusTrip, GetTopupBonusTrip(p, t), 2);
			file.Set(strPrefix + ServerDataFields::TopupBonusType, GetTopupBonusType(p, t));
			file.Set(strPrefix + ServerDataFields::TopupBonusPoints, GetTopupBonusPoints(p, t));
			file.Set(strPrefix + ServerDataFields::TopupBonusValue, GetTopupBonusValue(p, t), 2);
			file.Set(strPrefix + ServerDataFields::TopupBonusPercent, GetTopupBonusPercent(p, t));
		}
	}

	file.Set(ServerDataFields::StaffGiftPluNo, GetStaffGiftPluNo());
	file.Set(ServerDataFields::StaffGiftRedirectPluNo, GetStaffGiftRedirectPluNo());
	file.Set(ServerDataFields::EcrManagerImportFolder, GetEcrManagerImportFolder());
	file.Set(ServerDataFields::EcrManagerGlobalDept, GetEcrManagerGlobalDeptFlag());
	file.Set(ServerDataFields::EcrManagerGlobalStampOffer, GetEcrManagerGlobalStampOfferFlag());
	file.Set(ServerDataFields::EnablePluBonus, GetEnablePluBonusFlag());
	file.Set(ServerDataFields::EnableDeptBonus, GetEnableDeptBonusFlag());
	file.Set(ServerDataFields::NoPurseBonus, GetNoPurseBonusFlag());
	file.Set(ServerDataFields::PurchaseHistory, GetPurchaseHistoryFlag());
	file.Set(ServerDataFields::SpendBonusToPurse2, GetSpendBonusToPurse2Flag());
	file.Set(ServerDataFields::SpendUsePurse1First, GetSpendUsePurse1FirstFlag());
	
	//RENWAL TAB
	for (int n = 1; n <= RENEW_PLUNO_COUNT; n++)
	{
		CString strLabel;
		strLabel.Format("%s%d", ServerDataFields::RenewPluNo, n);
		file.Set(strLabel, GetRenewPluNo(n));

		strLabel.Format("%s%d", ServerDataFields::RenewMultiplier, n);
		file.Set(strLabel, GetRenewMultiplier(n));

		strLabel.Format("%s%d", ServerDataFields::RenewType, n);
		file.Set(strLabel, GetRenewType(n));

		strLabel.Format("%s%d", ServerDataFields::RenewYearDayNumber, n);
		file.Set(strLabel, GetRenewYearDayNumber(n));

		strLabel.Format("%s%d", ServerDataFields::RenewFixedYear, n);
		file.Set(strLabel, GetRenewFixedYear(n));
	}
	
	//SPEND BONUS TAB
	file.Set(ServerDataFields::RewardPurse1Spend, GetRewardPurse1SpendFlag());
	file.Set(ServerDataFields::RewardPurse2Spend, GetRewardPurse2SpendFlag());
	file.Set(ServerDataFields::RewardCashSpend, GetRewardCashSpendFlag());
	file.Set(ServerDataFields::RewardManualSpend, GetRewardManualSpendFlag());
	file.Set(ServerDataFields::RewardUpperSpendLimit, GetRewardUpperSpendLimit(), 2);
	file.Set(ServerDataFields::RewardNoBonusWithPluPoints, GetRewardNoBonusWithPluPointsFlag(), 2);
	file.Set(ServerDataFields::RewardTrip, GetRewardTrip(), 2);
	file.Set(ServerDataFields::RewardType, GetRewardType());
	file.Set(ServerDataFields::RewardPoints, GetRewardPoints());
	file.Set(ServerDataFields::RewardValue, GetRewardValue(), 2);
	file.Set(ServerDataFields::RewardComment, GetRewardComment());
	file.Set(ServerDataFields::RewardMultiplierTrigger, GetRewardMultiplierTrigger(), 2);
	file.Set(ServerDataFields::RewardMultiplierMon, GetRewardMultiplierMon());
	file.Set(ServerDataFields::RewardMultiplierTue, GetRewardMultiplierTue());
	file.Set(ServerDataFields::RewardMultiplierWed, GetRewardMultiplierWed());
	file.Set(ServerDataFields::RewardMultiplierThu, GetRewardMultiplierThu());
	file.Set(ServerDataFields::RewardMultiplierFri, GetRewardMultiplierFri());
	file.Set(ServerDataFields::RewardMultiplierSat, GetRewardMultiplierSat());
	file.Set(ServerDataFields::RewardMultiplierSun, GetRewardMultiplierSun());
	file.Set(ServerDataFields::RewardMonStart, GetRewardMonStart().GetTime());
	file.Set(ServerDataFields::RewardMonEnd, GetRewardMonEnd().GetTime());
	file.Set(ServerDataFields::RewardTueStart, GetRewardTueStart().GetTime());
	file.Set(ServerDataFields::RewardTueEnd, GetRewardTueEnd().GetTime());
	file.Set(ServerDataFields::RewardWedStart, GetRewardWedStart().GetTime());
	file.Set(ServerDataFields::RewardWedEnd, GetRewardWedEnd().GetTime());
	file.Set(ServerDataFields::RewardThuStart, GetRewardThuStart().GetTime());
	file.Set(ServerDataFields::RewardThuEnd, GetRewardThuEnd().GetTime());
	file.Set(ServerDataFields::RewardFriStart, GetRewardFriStart().GetTime());
	file.Set(ServerDataFields::RewardFriEnd, GetRewardFriEnd().GetTime());
	file.Set(ServerDataFields::RewardSatStart, GetRewardSatStart().GetTime());
	file.Set(ServerDataFields::RewardSatEnd, GetRewardSatEnd().GetTime());
	file.Set(ServerDataFields::RewardSunStart, GetRewardSunStart().GetTime());
	file.Set(ServerDataFields::RewardSunEnd, GetRewardSunEnd().GetTime());

	//REDEMPION TAB
	file.Set(ServerDataFields::RedeemPointsTrip, GetRedeemPointsTrip());
	file.Set(ServerDataFields::RedeemPointsValue, GetRedeemPointsValue(), 2);
	file.Set(ServerDataFields::RedeemComment, GetRedeemComment());
	file.Set(ServerDataFields::RedeemDisableAuto, GetRedeemDisableAutoFlag());

	//SEARCH TAB
	file.Set(ServerDataFields::SearchSnakesAndLadders, GetSearchSnakesAndLaddersFlag());
	file.Set(ServerDataFields::SearchExcludeHotlist, GetSearchExcludeHotlistFlag());
	file.Set(ServerDataFields::SearchExcludeInactive, GetSearchExcludeInactiveFlag());
	file.Set(ServerDataFields::SearchSurnameMatchFullname, GetSearchSurnameMatchFullnameFlag());

	//NON UI
	file.Set(ServerDataFields::SearchMax, m_nSearchMax);
	file.Set(ServerDataFields::LockRetries, m_nLockRetries);
	file.Set(ServerDataFields::ThreadDiagnostics, m_bThreadDiagnostics);
	file.Set(ServerDataFields::LockFolderTimeout, m_nLockFolderTimeout);
	file.Set(ServerDataFields::SPOSPointsAward, m_bSPosPointsAward);
	file.Set(ServerDataFields::SPOSPointsRedeem, m_bSPosPointsRedeem);
	file.Set(ServerDataFields::SPOSPointsNoLog, m_bSPosPointsNoLog);
	file.Set(ServerDataFields::CopyLogFiles, m_bCopyLogFiles);
	file.Set(ServerDataFields::SimpsInnsLookup, m_bSimpsInnsLookup);
	file.Set(ServerDataFields::BypassVersionSafetyCheck, m_bBypassVersionSafetyCheck ? "<*YES*>" : "");
	file.Set(ServerDataFields::LogCardReads, m_bLogCardReads);
	file.Set(ServerDataFields::DoubleBonusWithHash, m_bDoubleBonusWithHash);
	file.Set(ServerDataFields::DoubleBonusBeginDate, m_strDoubleBonusBeginDate);
	file.Set(ServerDataFields::DoubleBonusEndDate, m_strDoubleBonusEndDate);

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
	m_strTimeStamp = source.m_strTimeStamp;
	
	//EPOS VALIDATION
	SetLogActionFlag(source.GetLogActionFlag());
	SetRealTimePlusFlag(source.GetRealTimePlusFlag());
	SetAllowOverSpendFlag(source.GetAllowOverSpendFlag());
	SetNewBalanceOnReceiptFlag(source.GetNewBalanceOnReceiptFlag());
	SetLoyaltyLogSaveType(source.GetLoyaltyLogSaveType());

	for (int nDbNo = 1; nDbNo <= PLU_DBNO_COUNT; nDbNo++)
	{
		for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
		{
			m_EposPathInfo[nDbNo-1][nSetNo-1] = source.m_EposPathInfo[nDbNo-1][nSetNo-1];
		}
	}

	//UNKNOWN CARDS
	SetAddUnknownCardsFlag(source.GetAddUnknownCardsFlag());
	SetUnknownCardGroupRSPFlag(source.GetUnknownCardGroupRSPFlag());
	SetUnknownCardGroupNo(source.GetUnknownCardGroupNo());
	SetUnknownCardMinLength(source.GetUnknownCardMinLength());
	SetUnknownCardMinID(source.GetUnknownCardMinID());
	SetUnknownCardMaxID(source.GetUnknownCardMaxID());
	SetUnknownAccountLine1(source.GetUnknownAccountLine1());
	SetUnknownAccountLine2(source.GetUnknownAccountLine2());
	SetUnknownAccountLine3(source.GetUnknownAccountLine3());
	SetUnknownAccountPendingName(source.GetUnknownAccountPendingName());
	SetUnknownAccountCreateName(source.GetUnknownAccountCreateName());

	//POS DISPLAY
	for (int n = 1; n <= 8; n++)
	{
		m_PosDisplayInfo[n] = source.m_PosDisplayInfo[n];
	}
	SetPhotoIDFolderType(source.GetPhotoIDFolderType());
	SetLoyaltyUNCSystemPath(source.GetLoyaltyUNCSystemPath());
	SetLoyaltyUNCImagePath(source.GetLoyaltyUNCImagePath());
	
	//PLU SALES TAB
	for (int p = 1; p <= 2; p++)
	{
		SetTopupPCBonusTypePurse1(source.GetTopupPCBonusTypePurse1());
		SetTopupCommentPurse1(source.GetTopupCommentPurse1());
		SetTopupPCBonusTypePurse2(source.GetTopupPCBonusTypePurse2());
		SetTopupCommentPurse2(source.GetTopupCommentPurse2());

		for (int t = 1; t <= REVALUE_PLUNO_PERPURSE_COUNT; t++)
		{
			m_TopupInfo[p][t] = source.m_TopupInfo[p][t];
		}
	}
	
	SetStaffGiftPluNo(source.GetStaffGiftPluNo());
	SetStaffGiftRedirectPluNo(source.GetStaffGiftRedirectPluNo());
	SetEcrManagerImportFolder(source.GetEcrManagerImportFolder());
	SetEcrManagerGlobalDeptFlag(source.GetEcrManagerGlobalDeptFlag());
	SetEcrManagerGlobalStampOfferFlag(source.GetEcrManagerGlobalStampOfferFlag());
	SetEnablePluBonusFlag(source.GetEnablePluBonusFlag());
	SetEnableDeptBonusFlag(source.GetEnableDeptBonusFlag());
	SetNoPurseBonusFlag(source.GetNoPurseBonusFlag());
	SetPurchaseHistoryFlag(source.GetPurchaseHistoryFlag());
	
	//RENEWAL TAB
	for (int n = 1; n <= RENEW_PLUNO_COUNT; n++)
	{
		m_PluRenewalInfo[n] = source.m_PluRenewalInfo[n];
	}
	
	//SPEND BONUS TAB
	SetRewardPurse1SpendFlag(source.GetRewardPurse1SpendFlag());
	SetRewardPurse2SpendFlag(source.GetRewardPurse2SpendFlag());
	SetRewardCashSpendFlag(source.GetRewardCashSpendFlag());
	SetRewardManualSpendFlag(source.GetRewardManualSpendFlag());
	SetRewardUpperSpendLimit(source.GetRewardUpperSpendLimit());
	SetRewardTrip(source.GetRewardTrip());
	SetRewardType(source.GetRewardType());
	SetRewardPoints(source.GetRewardPoints());
	SetRewardValue(source.GetRewardValue());
	SetRewardComment(source.GetRewardComment());
	SetRewardMultiplierTrigger(source.GetRewardMultiplierTrigger());
	SetRewardMultiplierMon(source.GetRewardMultiplierMon());
	SetRewardMultiplierTue(source.GetRewardMultiplierTue());
	SetRewardMultiplierWed(source.GetRewardMultiplierWed());
	SetRewardMultiplierThu(source.GetRewardMultiplierThu());
	SetRewardMultiplierFri(source.GetRewardMultiplierFri());
	SetRewardMultiplierSat(source.GetRewardMultiplierSat());
	SetRewardMultiplierSun(source.GetRewardMultiplierSun());
	SetRewardMonStart(source.GetRewardMonStart());
	SetRewardMonEnd(source.GetRewardMonEnd());
	SetRewardTueStart(source.GetRewardTueStart());
	SetRewardTueEnd(source.GetRewardTueEnd());
	SetRewardWedStart(source.GetRewardWedStart());
	SetRewardWedEnd(source.GetRewardWedEnd());
	SetRewardThuStart(source.GetRewardThuStart());
	SetRewardThuEnd(source.GetRewardThuEnd());
	SetRewardFriStart(source.GetRewardFriStart());
	SetRewardFriEnd(source.GetRewardFriEnd());
	SetRewardSatStart(source.GetRewardSatStart());
	SetRewardSatEnd(source.GetRewardSatEnd());
	SetRewardSunStart(source.GetRewardSunStart());
	SetRewardSunEnd(source.GetRewardSunEnd());

	//REDEMPTION TAB
	SetRedeemPointsTrip(source.GetRedeemPointsTrip());
	SetRedeemPointsValue(source.GetRedeemPointsValue());
	SetRedeemComment(source.GetRedeemComment());
	SetRedeemDisableAutoFlag(source.GetRedeemDisableAutoFlag());
		
	//SEARCH TAB
	SetSearchSnakesAndLaddersFlag(source.GetSearchSnakesAndLaddersFlag());
	SetSearchExcludeHotlistFlag(source.GetSearchExcludeHotlistFlag());
	SetSearchExcludeInactiveFlag(source.GetSearchExcludeInactiveFlag());
	SetSearchSurnameMatchFullnameFlag(source.GetSearchSurnameMatchFullnameFlag());

	//NON UI
	SetSearchMax(source.GetSearchMax());
	SetLockRetries(source.GetLockRetries());
	SetThreadDiagnosticsFlag(source.GetThreadDiagnosticsFlag());
	SetLockFolderTimeout(source.GetLockFolderTimeout());
	SetSPosPointsAwardFlag(source.GetSPosPointsAwardFlag());
	SetSPosPointsRedeemFlag(source.GetSPosPointsRedeemFlag());
	SetSPosPointsNoLogFlag(source.GetSPosPointsNoLogFlag());
	SetCopyLogFilesFlag(source.GetCopyLogFilesFlag());
	SetSimpsInnsLookupFlag(source.GetSimpsInnsLookupFlag());
	SetBypassVersionSafetyCheckFlag(source.GetBypassVersionSafetyCheckFlag());
	SetLogCardReadsFlag(source.GetLogCardReadsFlag());
	SetDoubleBonusWithHashFlag(source.GetDoubleBonusWithHashFlag());
	SetDoubleBonusBeginDate(source.GetDoubleBonusBeginDate());
	SetDoubleBonusEndDate(source.GetDoubleBonusEndDate());

	//OBSOLETE
	SetObsoleteCardCheckDigitFlag(source.GetObsoleteCardCheckDigitFlag());
	SetObsoleteCardNoFormat(source.GetObsoleteCardNoFormat());
	SetObsoleteCardNoPrefix(source.GetObsoleteCardNoPrefix());
}

//*******************************************************************

bool CServerData::RevaluationBonusEnabled()
{
	for (int p = 1; p <= 2; p++)
	{
		for (int t = 1; t <= REVALUE_PLUNO_PERPURSE_COUNT; t++)
		{
			if (m_TopupInfo[p][t].m_dTrip != 0.0)
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

//*******************************************************************

CString CServerData::GetPosDisplayField( int n )
{
	if ((n >= 1) && (n <= POSDISPLAY_LINE_COUNT))
	{
		m_PosDisplayInfo[0].m_strField = m_PosDisplayInfo[n].m_strField;
	}
	else
	{
		m_PosDisplayInfo[0].m_strField = "";
	}

	LoyaltyHeaderHelpers.UpdateLegacyField(m_PosDisplayInfo[0].m_strField);
	
	return m_PosDisplayInfo[0].m_strField;
}

//*******************************************************************

void CServerData::SetPosDisplayField( int n, CString str )
{
	if ((n >= 1) && (n <= POSDISPLAY_LINE_COUNT))
	{
		m_PosDisplayInfo[n].m_strField = str;
	}
}

//*******************************************************************

CString CServerData::GetPosDisplayText( int n )
{
	if ((n >= 1) && (n <= POSDISPLAY_LINE_COUNT))
	{
		m_PosDisplayInfo[0].m_strText = m_PosDisplayInfo[n].m_strText;
	}
	else
	{
		m_PosDisplayInfo[0].m_strText = "";
	}

	return m_PosDisplayInfo[0].m_strText;
}

//*******************************************************************

void CServerData::SetPosDisplayText( int n, CString str )
{
	if ((n >= 1) && (n <= POSDISPLAY_LINE_COUNT))
	{
		m_PosDisplayInfo[n].m_strText = str;
	}
}

//*******************************************************************

CString CServerData::GetRenewPluNo( int n )
{
	if ((n >= 1) && (n <= RENEW_PLUNO_COUNT))
	{
		m_PluRenewalInfo[0].m_strPluNo = m_PluRenewalInfo[n].m_strPluNo;
	}
	else
	{
		m_PluRenewalInfo[0].m_strPluNo = "";
	}

	return m_PluRenewalInfo[0].m_strPluNo;
}

//*******************************************************************

void CServerData::SetRenewPluNo( int n, CString str )
{
	if ((n >= 1) && (n <= RENEW_PLUNO_COUNT))
	{
		m_PluRenewalInfo[n].m_strPluNo = str;
	}
}

//*******************************************************************

int CServerData::GetRenewMultiplier( int n )
{
	if ((n >= 1) && (n <= RENEW_PLUNO_COUNT))
	{
		return m_PluRenewalInfo[n].m_nMultiplier;
	}
	else
	{
		return 1;
	}
}

//*******************************************************************

void CServerData::SetRenewMultiplier( int n, int x )
{
	if ((n >= 1) && (n <= RENEW_PLUNO_COUNT))
	{
		if ((x >= 1) && (x <= 99))
		{
			m_PluRenewalInfo[n].m_nMultiplier = x;
		}
	}
}

//*******************************************************************

int CServerData::GetRenewType( int n )
{
	if ((n >= 1) && (n <= RENEW_PLUNO_COUNT))
	{
		return m_PluRenewalInfo[n].m_nType;
	}
	else
	{
		return 0;
	}
}

//*******************************************************************

void CServerData::SetRenewType(int n, int x)
{
	if ((n >= 1) && (n <= RENEW_PLUNO_COUNT))
	{
		if ((x >= 0) && (x <= 3))
		{
			m_PluRenewalInfo[n].m_nType = x;
		}
	}
}

//*******************************************************************

int CServerData::GetRenewYearDayNumber( int n )
{
	if ((n >= 1) && (n <= RENEW_PLUNO_COUNT))
	{
		return m_PluRenewalInfo[n].m_nYearDayNumber;
	}
	else
	{
		return 0;
	}
}

//*******************************************************************

void CServerData::SetRenewYearDayNumber(int n, int x)
{
	if ((n >= 1) && (n <= RENEW_PLUNO_COUNT))
	{
		if ((x >= 1) && (x <= 366))
		{
			m_PluRenewalInfo[n].m_nYearDayNumber = x;
		}
	}
}

//*******************************************************************

int CServerData::GetRenewFixedYear(int n)
{
	if ((n >= 1) && (n <= RENEW_PLUNO_COUNT))
	{
		return m_PluRenewalInfo[n].m_nFixedYear;
	}
	else
	{
		return 0;
	}
}

//*******************************************************************

void CServerData::SetRenewFixedYear(int n, int x)
{
	if ((n >= 1) && (n <= RENEW_PLUNO_COUNT))
	{
		if ((0 == x) || ((x >= 2020) && (x <= 3000)))
		{
			m_PluRenewalInfo[n].m_nFixedYear = x;
		}
	}
}

//*******************************************************************

CString CServerData::GetTopupPluNo(int p, int t)
{
	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			m_TopupInfo[p][0].m_strPluNo = m_TopupInfo[p][t].m_strPluNo;
		}
		else
		{
			m_TopupInfo[p][0].m_strPluNo = "";
		}
	}
	else
	{
		p = 0;
		m_TopupInfo[p][0].m_strPluNo = "";
	}

	return m_TopupInfo[p][0].m_strPluNo;
}


//*******************************************************************

void CServerData::SetTopupPluNo( int p, int t, CString str )
{
	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			m_TopupInfo[p][t].m_strPluNo = str;
		}
	}
}

//*******************************************************************

double CServerData::GetTopupBonusTrip(int p, int t)
{
	double dResult = 0.0;	

	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			dResult = m_TopupInfo[p][t].m_dTrip;
		}
	}

	return dResult;
}

//*******************************************************************

void CServerData::SetTopupBonusTrip(int p, int t, double x)
{
	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			m_TopupInfo[p][t].m_dTrip = x;
		}
	}
}

//*******************************************************************

int CServerData::GetTopupBonusType(int p, int t)
{
	int nResult = 0;

	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			nResult = m_TopupInfo[p][t].m_nType;
		}
	}

	return nResult;
}

//*******************************************************************

void CServerData::SetTopupBonusType(int p, int t, int x)
{
	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			m_TopupInfo[p][t].m_nType = x;
		}
	}
}

//*******************************************************************

int CServerData::GetTopupBonusPoints(int p, int t)
{
	int nResult = 0;

	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			nResult = m_TopupInfo[p][t].m_nPoints;
		}
	}

	return nResult;
}

//*******************************************************************

void CServerData::SetTopupBonusPoints(int p, int t, int x)
{
	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			if (x >= 0)
			{
				m_TopupInfo[p][t].m_nPoints = x;
			}
		}
	}
}

//*******************************************************************

double CServerData::GetTopupBonusValue(int p, int t)
{
	double dResult = 0.0;

	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			dResult = m_TopupInfo[p][t].m_dValue;
		}
	}

	return dResult;
}

//*******************************************************************

void CServerData::SetTopupBonusValue(int p, int t, double x)
{
	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			if (::CompareDoubles(x, 0.0, 2) >= 0)
			{
				m_TopupInfo[p][t].m_dValue = x;
			}
		}
	}
}

//*******************************************************************

int CServerData::GetTopupBonusPercent(int p, int t)
{
	int nResult = 0;

	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			nResult = m_TopupInfo[p][t].m_nPercent;
		}
	}

	return nResult;
}


//*******************************************************************

void CServerData::SetTopupBonusPercent(int p, int t, int x)
{
	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			if ((x >= 0) && (x <= 100))
			{
				m_TopupInfo[p][t].m_nPercent = x;
			}
		}
	}
}

//*******************************************************************

CString CServerData::GetTimeString ( int nHour, int nMinute )
{
	CString strBuf;
	strBuf.Format ( "%2.2d:%2.2d", nHour, nMinute );
	return strBuf;
}

//*******************************************************************

void CServerData::SetSearchMax( int nMax )
{
	if ((nMax >= 2) && (nMax <= 100))
	{
		m_nSearchMax = nMax;
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

bool CServerData::HaveRevaluationPluCode(int p, int t)
{
	bool bResult = FALSE;

	if ((1 == p) || (2 == p))
	{
		if ((t >= 1) && (t <= REVALUE_PLUNO_PERPURSE_COUNT))
		{
			CString strPluNo = m_TopupInfo[p][t].m_strPluNo;
			strPluNo.TrimLeft('0');
			bResult = (strPluNo != "");
		}
	}

	return bResult;
}

//*******************************************************************

bool CServerData::HaveStaffGiftCode()
{
	CString strPluNo = m_strStaffGiftPluNo;
	strPluNo.TrimLeft('0');
	return (strPluNo != "");
}

//*******************************************************************

int CServerData::CalculateRevaluationBonusPoints ( double dValueAdded, double dTripValue, int nRevaluationPoints )
{
	int nPoints = 0;

	int nFactor = (int)( dValueAdded / dTripValue ); 
	nPoints = nRevaluationPoints * nFactor;

	return nPoints;
}

//*******************************************************************

double CServerData::CalculateRevaluationBonusValue ( double dValueAdded, double dTripValue, double dRevaluationValue )
{
	double dValue = 0.0;

	int nFactor = (int)( dValueAdded / dTripValue );		// round it
	dValue = dRevaluationValue * (double)nFactor;

	return dValue;
}

//*******************************************************************

double CServerData::CalculateRevaluationBonusPercent ( double dValueAdded, double dTripValue, int nRevaluationPercent )
{
	double dValue = 0.0;

	if ( ( dValueAdded >= dTripValue ) && ( nRevaluationPercent > 0 ) )
	{
		dValue = dValueAdded * (double) nRevaluationPercent;
		dValue /= 100.0;

		CString strValue;
		strValue.Format( "%.2f", dValue );
		dValue = atof( strValue );
	}

	return dValue;
}

//*******************************************************************

bool CServerData::CanAddUnknownCard(CString strCardNo)
{
	bool bResult = FALSE;

	switch (System.GetInterfaceType())
	{
	case nINTERFACE_MIFAREv2:
	case nINTERFACE_MIFAREv3:
		break;

	case nINTERFACE_GENERALv1:
		bResult = m_bAddUnknownCards;
		if ((TRUE == bResult) && (TRUE == m_bSimpsInnsLookup) && ( strCardNo != "" ) )
		{
			if (strCardNo.GetLength() != 12)
			{
				bResult = FALSE;
			}
			else if (::TestNumeric(strCardNo) == FALSE)
			{
				bResult = FALSE;
			}
			else if (strCardNo.Left(1) == "0")
			{
				bResult = FALSE;
			}
		}
		break;

	default:
		bResult = m_bAddUnknownCards;
		break;
	}

	return bResult;
}

//*******************************************************************

void CServerData::SetUnknownCardMinID(__int64 n)
{
	if ((n >= 1) && (n <= 999999999999))
	{
		m_nUnknownCardMinID = n;
	}
}

//*******************************************************************

void CServerData::SetUnknownCardMaxID(__int64 n)
{
	if ((n >= 1) && (n <= 999999999999))
	{
		m_nUnknownCardMaxID = n;
	}
}

//*******************************************************************

void CServerData::SetPhotoIDFolderType( int n )
{
	switch(n)
	{
	case 0:
	case 1:
	case 2:
		m_nPhotoIDFolderType = n;
		break;
	}
}

//*******************************************************************

void CServerData::SetEposRequestPath(CServerDataFolderSetIndex& FolderSetIndex, CString str)
{
	int nDbNo = FolderSetIndex.m_nDbNo;
	int nSetNo = FolderSetIndex.m_nSetNo;

	if ((nDbNo >= 1) && (nDbNo <= PLU_DBNO_COUNT) && (nSetNo >= 1) && (nSetNo <= EPOSREQPATH_COUNT))
	{
		m_EposPathInfo[nDbNo - 1][nSetNo - 1].m_strReqPath = str;
	}
}

//*******************************************************************

void CServerData::SetEposPathLabel(CServerDataFolderSetIndex& FolderSetIndex, CString str)
{
	int nDbNo = FolderSetIndex.m_nDbNo;
	int nSetNo = FolderSetIndex.m_nSetNo;

	if ((nDbNo >= 1) && (nDbNo <= PLU_DBNO_COUNT) && (nSetNo >= 1) && (nSetNo <= EPOSREQPATH_COUNT))
	{
		m_EposPathInfo[nDbNo - 1][nSetNo - 1].m_strPathLabel = str;
	}
}

//*******************************************************************

CString CServerData::GetEposRequestPath(CServerDataFolderSetIndex& FolderSetIndex)
{
	int nDbNo = FolderSetIndex.m_nDbNo;
	int nSetNo = FolderSetIndex.m_nSetNo;

	if ((nDbNo < 1) || (nDbNo > PLU_DBNO_COUNT))
	{
		nDbNo = 1;
	}

	if ((nSetNo < 1) || (nSetNo > EPOSREQPATH_COUNT))
	{
		nSetNo = 1;
	}

	return m_EposPathInfo[nDbNo-1][nSetNo-1].m_strReqPath;
}

//*******************************************************************

CString CServerData::GetEposPathLabel(CServerDataFolderSetIndex& FolderSetIndex)
{
	int nDbNo = FolderSetIndex.m_nDbNo;
	int nSetNo = FolderSetIndex.m_nSetNo;

	if ((nDbNo < 1) || (nDbNo > PLU_DBNO_COUNT))
	{
		nDbNo = 1;
	}
	if ((nSetNo < 1) || (nSetNo > EPOSREQPATH_COUNT))
	{
		nSetNo = 1;
	}

	return m_EposPathInfo[nDbNo - 1][nSetNo - 1].m_strPathLabel;
}

//*******************************************************************

void CServerData::SetEposLogPath(CServerDataFolderSetIndex& FolderSetIndex, CString str)
{
	int nDbNo = FolderSetIndex.m_nDbNo;
	int nSetNo = FolderSetIndex.m_nSetNo;

	if ((nDbNo >= 1) && (nDbNo <= PLU_DBNO_COUNT) && (nSetNo >= 1) && (nSetNo <= EPOSREQPATH_COUNT))
	{
		m_EposPathInfo[nDbNo - 1][nSetNo - 1].m_strLogPath = str;
	}
}

//*******************************************************************

CString CServerData::GetEposLogPath(CServerDataFolderSetIndex& FolderSetIndex)
{
	int nDbNo = FolderSetIndex.m_nDbNo;
	int nSetNo = FolderSetIndex.m_nSetNo;

	if ((nDbNo < 1) || (nDbNo > PLU_DBNO_COUNT))
	{
		nDbNo = 1;
	}
	if ((nSetNo < 1) || (nSetNo > EPOSREQPATH_COUNT))
	{
		nSetNo = 1;
	}
	return m_EposPathInfo[nDbNo - 1][nSetNo - 1].m_strLogPath;
}

//*******************************************************************

void CServerData::SetExternalRequestPathFlag(CServerDataFolderSetIndex& FolderSetIndex, bool b)
{
	int nDbNo = FolderSetIndex.m_nDbNo;
	int nSetNo = FolderSetIndex.m_nSetNo;

	if ((nDbNo >= 1) && (nDbNo <= PLU_DBNO_COUNT) && (nSetNo >= 1) && (nSetNo <= EPOSREQPATH_COUNT))
	{
		m_EposPathInfo[nDbNo - 1][nSetNo - 1].m_bIsExternal = b;
	}
}

//*******************************************************************

bool CServerData::GetExternalRequestPathFlag(CServerDataFolderSetIndex& FolderSetIndex)
{
	int nDbNo = FolderSetIndex.m_nDbNo;
	int nSetNo = FolderSetIndex.m_nSetNo;

	if ((nDbNo < 1) || (nDbNo > PLU_DBNO_COUNT))
	{
		nDbNo = 1;
	}
	if ((nSetNo < 1) || (nSetNo > EPOSREQPATH_COUNT))
	{
		nSetNo = 1;
	}

	return m_EposPathInfo[nDbNo - 1][nSetNo - 1].m_bIsExternal;
}

//*******************************************************************

void CServerData::SetCurrentFolderSetIndex(CServerDataFolderSetIndex& index)
{
	if ((index.m_nDbNo >= 1) && (index.m_nDbNo <= PLU_DBNO_COUNT) && (index.m_nSetNo >= 1) && (index.m_nSetNo <= EPOSREQPATH_COUNT))
	{
		m_CurrentFolderSetIndex = index;
	}
}

//*******************************************************************

CString CServerData::GetCurrentEposRequestPath()
{
	return GetEposRequestPath(m_CurrentFolderSetIndex);
}

//*******************************************************************

CString CServerData::GetCurrentEposLogPath()
{
	return GetEposLogPath(m_CurrentFolderSetIndex);
}

//*******************************************************************

bool CServerData::GetCurrentExternalRequestPathFlag()
{
	return GetExternalRequestPathFlag(m_CurrentFolderSetIndex);
}

//*******************************************************************

void CServerData::SetUnknownCardMinLength(int n)
{
	if ((n >= 1) && (n <= 12))
	{
		m_nUnknownCardMinLength = n;
	}	
}

//*******************************************************************

void CServerData::SetUnknownAccountLine1(CString str)
{
	if (str.GetLength() <= MAX_UNKNOWN_ACCOUNT_PROMPTLEN)
	{
		m_strUnknownAccountLine1 = str;
	}
}

//*******************************************************************

void CServerData::SetUnknownAccountLine2(CString str)
{
	if (str.GetLength() <= MAX_UNKNOWN_ACCOUNT_PROMPTLEN)
	{
		m_strUnknownAccountLine2 = str;
	}
}

//*******************************************************************

void CServerData::SetUnknownAccountLine3(CString str)
{
	if (str.GetLength() <= MAX_UNKNOWN_ACCOUNT_PROMPTLEN)
	{
		m_strUnknownAccountLine3 = str;
	}
}

//*******************************************************************

void CServerData::SetUnknownAccountPendingName(CString str)
{
	if (str.GetLength() <= MAX_UNKNOWN_ACCOUNT_NAMELEN)
	{
		m_strUnknownAccountPendingName = str;
	}
}

//*******************************************************************

void CServerData::SetUnknownAccountCreateName(CString str)
{
	if (str.GetLength() <= MAX_UNKNOWN_ACCOUNT_NAMELEN)
	{
		m_strUnknownAccountCreateName = str;
	}
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

void CServerData::SetDoubleBonusBeginDate(CString strDate)
{
	if ( Validate8DigitDate(strDate) == TRUE)
	{
		m_strDoubleBonusBeginDate = strDate;
	}
}

//*******************************************************************

void CServerData::SetDoubleBonusEndDate(CString strDate)
{
	if (Validate8DigitDate(strDate) == TRUE)
	{
		m_strDoubleBonusEndDate = strDate;
	}
}

//*******************************************************************

bool CServerData::Validate8DigitDate(CString strDate)
{
	bool bResult = FALSE;

	if (strDate.GetLength() == 8 )
	{
		if (::TestNumeric(strDate) == TRUE)
		{
			COleDateTime timeTest = COleDateTime(
				atoi(strDate.Mid(0, 4)),
				atoi(strDate.Mid(4, 2)),
				atoi(strDate.Mid(6, 2)),
				0, 0, 0);

			if (timeTest.GetStatus() == COleDateTime::valid)
			{
				bResult = TRUE;
			}
		}
	}

	return bResult;
}

//*******************************************************************

bool CServerData::CanDoDoubleBonusNow()
{
	bool bResult = FALSE;

	if (TRUE == m_bDoubleBonusWithHash)
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		CString strDateNow = "";
		strDateNow.Format("%4.4d%2.2d%2.2d",
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay());

		CString strBeginDate = m_strDoubleBonusBeginDate;
		CString strEndDate = m_strDoubleBonusEndDate;


		if ((Validate8DigitDate(strBeginDate) == TRUE) && (Validate8DigitDate(strEndDate) == TRUE))
		{
			if ((strDateNow >= strBeginDate) && (strDateNow <= strEndDate))
			{
				bResult = TRUE;
			}
		}
	}

	return bResult;
}

//*******************************************************************

CString CServerData::GetDefaultEposPathLabel(int nSet)
{
	CString strResult = "Standard Folder Set";

	if ((nSet >= 2) && (nSet <= 10))
	{
		strResult.Format("Set %d", nSet);
	}

	return strResult;
}

//*******************************************************************

CString CServerData::GetEcrManagerImportDbInfoFilename()
{
	m_strEcrManagerImportDbInfoFilename = m_strEcrManagerImportFolder;

	if (m_strEcrManagerImportDbInfoFilename != "")
	{
		m_strEcrManagerImportDbInfoFilename += "\\sysfiles\\loydbinfo.csv";
	}

	return m_strEcrManagerImportDbInfoFilename;
}

//*******************************************************************

CString CServerData::GetEcrManagerImportPluFilename(int nDbNo)
{
	m_strEcrManagerImportPluFilename = m_strEcrManagerImportFolder;

	if (m_strEcrManagerImportPluFilename != "")
	{
		CString strAppend = "";
		strAppend.Format("\\db%3.3d\\plupts.csv", 
			nDbNo);

		m_strEcrManagerImportPluFilename += strAppend;
	}

	return m_strEcrManagerImportPluFilename;
}

//*******************************************************************

CString CServerData::GetEcrManagerImportDeptFilename(int nDbNo)
{
	m_strEcrManagerImportDeptFilename = m_strEcrManagerImportFolder;

	if (m_strEcrManagerImportDeptFilename != "")
	{
		CString strAppend = "";

		if (FALSE == m_bEcrManagerGlobalDept)
		{
			strAppend.Format("\\db%3.3d\\loydept.csv",
				nDbNo);
		}
		else
		{
			strAppend = "\\sysfiles\\loydept.csv";
		}
		
		m_strEcrManagerImportDeptFilename += strAppend;
	}

	return m_strEcrManagerImportDeptFilename;
}

//*******************************************************************

CString CServerData::GetEcrManagerImportStampOfferFilename()
{
	m_strEcrManagerImportStampOfferFilename = m_strEcrManagerImportFolder;

	if (m_strEcrManagerImportStampOfferFilename != "")
	{
		if (FALSE == m_bEcrManagerGlobalStampOffer)
		{
			m_strEcrManagerImportStampOfferFilename += "\\db001\\stampoffer.csv";
		}
		else
		{
			m_strEcrManagerImportStampOfferFilename += "\\sysfiles\\stampoffer.csv";
		}
	}

	return m_strEcrManagerImportStampOfferFilename;
}

//*******************************************************************

CString CServerData::GetEcrManagerImportLoyaltySchemeFilename()
{
	m_strEcrManagerImportLoyaltySchemeFilename = m_strEcrManagerImportFolder;

	if (m_strEcrManagerImportLoyaltySchemeFilename != "")
	{
		m_strEcrManagerImportLoyaltySchemeFilename += "\\db001\\loyscheme.csv";
	}

	return m_strEcrManagerImportLoyaltySchemeFilename;
}

//*******************************************************************

void CServerData::UpdateEcrmanDbInfo()
{
	CString strImportFile = GetEcrManagerImportDbInfoFilename();

	if (strImportFile != "")
	{
		if (::FileExists(strImportFile) == TRUE)	// see if an update exists
		{
			CSQLRepositoryEcrmanDatabase repoDb;
			repoDb.ImportEcrmanDatabase(strImportFile);
			::remove(strImportFile);				// lose original
		}
	}
}

//*******************************************************************
