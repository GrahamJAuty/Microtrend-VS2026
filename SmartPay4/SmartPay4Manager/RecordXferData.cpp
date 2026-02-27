//$$******************************************************************
#include "RecordXferData.h"
//$$******************************************************************

CRecordXferData::CRecordXferData(void)
{
	m_bUserName = FALSE;
	m_bForename = FALSE;
	m_bSurname = FALSE;
	m_bGroup = FALSE;
	m_bMemberID = FALSE;
	m_bExpiry = FALSE;
	m_bCopyPurchaseHistory = FALSE;
	/*****/
	m_strUserName = "";
	m_strForename = "";
	m_strSurname = "";
	m_nGroupNo = 0;
	m_strDayGroups = "";
	m_strMemberID = "";
	m_strExpiryDate = "";
	m_strComment = "";
	/*****/
	m_bPoints = FALSE;
	m_bPointsTD = FALSE;
	m_bPurse1Liability = FALSE;
	m_bPurse1Credit = FALSE;
	m_bPurse1SpendTD = FALSE;
	m_bPurse2 = FALSE;
	m_bPurse2SpendTD = FALSE;
	m_bPurse3Liability = FALSE;
	m_bPurse3Credit = FALSE;
	m_bPurse3SpendTD = FALSE;
	m_bPurchaseControl = FALSE;
	/*****/
	m_nPoints = 0;
	m_nPointsTD = 0;;
	m_dPurse1Liability = 0.0;
	m_dPurse1Credit = 0.0;
	m_dPurse1SpendTD = 0.0;
	m_dPurse2 = 0.0;
	m_dPurse2SpendTD = 0.0;
	m_dPurse3Liability = 0.0;
	m_dPurse3Credit = 0.0;
	m_dPurse3SpendTD = 0.0;
	/*****/
	m_bDOB = FALSE;
	m_bEmail = FALSE;
	m_bTransferGender = FALSE;

	for (int n = 0; n <= MAXINFOTICK; n++)
	{
		m_bInfoText[n] = FALSE;
		m_strInfo[n] = "";
	}

	m_bDietary = FALSE;
	m_bAllergyCodes = FALSE;
	m_bAlertCode = FALSE;
	/*****/
	m_strDOB = "";
	m_strEmail = "";
	m_strGender = "";
	m_strDietary = "";
	m_nAllergyCodes = 0;
	m_nAlertCode = 0;
	/*****/

	for (int n = 0; n <= MAXRECORDTICK; n++)
	{
		m_bTick[n] = FALSE;
		m_bRecordTick[n] = FALSE;
	}

	/*****/
	m_bMaxSpend = FALSE;
	/*****/
	m_bOwnMaxSpend = FALSE;
	for ( int nPeriod = 1; nPeriod <= 6; nPeriod++ )
	{
		m_dMaxSpend[nPeriod] = 0.0;
		m_dOverSpend[nPeriod] = 0.0;
		m_dMaxSpendP3[nPeriod] = 0.0;
		m_dOverSpendP3[nPeriod] = 0.0;

		for (int nPurse = 1; nPurse <= 3; nPurse++)
		{
			m_bPurseAllowed[nPeriod][nPurse] = FALSE;
		}
	}
	/*****/
	m_dPurse1Spend = 0.0;
	m_strPurse1DateLastUsed = "";
	m_strPurse1TimeLastUsed = "";
	m_dPurse2Spend = 0.0;
	m_strPurse2DateLastUsed = "";
	m_strPurse2TimeLastUsed = "";
	m_dPurse3Spend = 0.0;
	m_strPurse3DateLastUsed = "";
	m_strPurse3TimeLastUsed = "";
	m_strRefreshedDate = "";
	m_strRefreshedTime = "";
}

//$$******************************************************************
