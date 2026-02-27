#pragma once

//*******************************************************************

//#define	szEMAIL_FINISHED		"Database scan complete"
//#define	szEMAIL_CANCELLED		"Database scan terminated!"

//*******************************************************************

class CEmailAlertData
{
public:
	CEmailAlertData();
	void Read();
	void Save();
	CString CheckIndicatorConflict( int nPage );
	bool AlertsRequired();

public:
	int m_nEmailContactNo;									// email field 0=Email or Info1 - 8
	CString m_strSignatureText1;
	bool m_bEnableLowBalanceAlerts;
	bool m_bPromptLowBalanceAlerts;
	CString m_strLowBalanceEmailText;
	int m_nLowBalanceTickNo;								// 0=not used else 1 - 4
	bool m_bLowBalancePurse1Reqd;
	bool m_bLowBalancePurse2Reqd;
	bool m_bLowBalancePurse3Reqd;
	bool m_bLowBalanceShowPurse1;
	bool m_bLowBalanceShowPurse2;
	bool m_bLowBalanceShowPurse3;
	bool m_bLowBalanceShowPoints;
	bool m_bLowBalancePurse1Symbol;
	bool m_bLowBalancePurse2Symbol;
	bool m_bLowBalancePurse3Symbol;
	CString m_strLowBalancePurse1Text;
	CString m_strLowBalancePurse2Text;
	CString m_strLowBalancePurse3Text;

	bool m_bEnablePurchaseAlerts;
	CString m_strPurchaseEmailText;
	bool m_bPromptPurchaseAlerts;
	int m_nPurchaseTickNo;									// 0=not defined else 1 - 4
	int m_nPurchaseDay;
	CString m_strPurchaseStartFrom;
	bool m_bPurchaseExcludeZeroPriced;

	bool m_bEnablePointsAchievement;
	bool m_bPromptPointsAchievement;
	CString m_strPointsAchievementEmailText;
	int m_nPointsAchievementTickNo;							// 0=not defined else 1 - 4

	bool m_bEnableWeeklyPoints; 
	bool m_bPromptWeeklyPoints;
	CString m_strWeeklyPointsEmailText; 
	int m_nWeeklyPointsTickNo;								// 0=not defined else 1 - 4
	int m_nWeeklyPointsDay;									// 0=sunday, 1-monday .. 5-Friday
	bool m_bWeeklyPointsExcludeNoChange;

public:
	bool PointsBalanceReqd();
	bool SendLowBalancesReqd();
	bool SendPurchasesReqd();
	bool SendPointsAchievementReqd();
	bool SendWeeklyPointsReqd();

private:
	CString m_strFilename;
};

//*******************************************************************
