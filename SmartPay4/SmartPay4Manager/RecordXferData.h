#pragma once
//$$******************************************************************
#define MAXINFOTICK 8
#define MAXRECORDTICK 4
//$$******************************************************************

class CRecordXferData
{
public:
	CRecordXferData();

public:
	bool m_bUserName;
	bool m_bForename;
	bool m_bSurname;
	bool m_bGroup;
	bool m_bMemberID;
	bool m_bExpiry;
	bool m_bCopyPurchaseHistory;

public:
	bool m_bPoints;
	bool m_bPointsTD;
	bool m_bPurse1Liability;
	bool m_bPurse1Credit;
	bool m_bPurse1SpendTD;
	bool m_bPurse2;
	bool m_bPurse2SpendTD;
	bool m_bPurse3Liability;
	bool m_bPurse3Credit;
	bool m_bPurse3SpendTD;
	bool m_bPurchaseControl;
	
public:
	bool m_bDOB;
	bool m_bEmail;
	bool m_bTransferGender;
	bool m_bInfoText[MAXINFOTICK + 1];
	bool m_bDietary;
	bool m_bAllergyCodes;
	bool m_bAlertCode;

public:
	bool m_bTick[MAXRECORDTICK + 1];
	
public:
	CString m_strUserName;
	CString m_strForename;
	CString m_strSurname;
	int m_nGroupNo;
	CString m_strDayGroups;
	CString m_strMemberID;
	CString m_strExpiryDate;
	CString m_strComment;

public:
	int m_nPoints;
	int m_nPointsTD;
	double m_dPurse1Liability;
	double m_dPurse1Credit;
	double m_dPurse1SpendTD;
	double m_dPurse2;
	double m_dPurse2SpendTD;
	double m_dPurse3Liability;
	double m_dPurse3Credit;
	double m_dPurse3SpendTD;
	
public:
	CString m_strDOB;
	CString m_strEmail;
	CString m_strGender;
	CString m_strInfo[MAXINFOTICK + 1];
	CString m_strDietary;
	int m_nAllergyCodes;
	int m_nAlertCode;

public:
	bool m_bRecordTick[MAXRECORDTICK + 1];

public:
	double m_dPurse1Spend;
	CString m_strPurse1DateLastUsed;
	CString m_strPurse1TimeLastUsed;
	double m_dPurse2Spend;
	CString m_strPurse2DateLastUsed;
	CString m_strPurse2TimeLastUsed;
	double m_dPurse3Spend;
	CString m_strPurse3DateLastUsed;
	CString m_strPurse3TimeLastUsed;
	CString m_strRefreshedDate;
	CString m_strRefreshedTime;

public:
	bool m_bMaxSpend;

public:
	bool m_bOwnMaxSpend;
	double m_dMaxSpend[7];
	double m_dOverSpend[7];
	double m_dMaxSpendP3[7];
	double m_dOverSpendP3[7];
	bool m_bPurseAllowed[7][4];
};

//$$******************************************************************
