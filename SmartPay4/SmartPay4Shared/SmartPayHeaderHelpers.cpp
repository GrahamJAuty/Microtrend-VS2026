//$$********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//$$********************************************************************
#include "SmartPayHeaderHelpers.h"
//$$********************************************************************

CSmartPayHeaderHelpers::CSmartPayHeaderHelpers()
{
	m_strHeaderFull = "";
	m_strHeaderBiometric = "";
	m_HeaderMap.InitHashTable(103);

	AddFieldToHeader(Account::UserID.Label);
	AddFieldToHeader(Account::Username.Label);
	AddFieldToHeader(Account::Forename.Label);
	AddFieldToHeader(Account::Surname.Label);
	AddFieldToHeader(Account::DOB.Label);
	AddFieldToHeader(Account::ExpiryDate.Label);
	AddFieldToHeader(Account::Gender.Label);
	AddFieldToHeader(Account::GroupNo.Label);
	AddFieldToHeader(Account::GroupShiftStatus.Label);
	AddFieldToHeader(Account::GroupShiftStartDate.Label);
	AddFieldToHeader(Account::GroupShiftEndDate.Label);
	AddFieldToHeader(Account::GroupShiftFutureGroup.Label);
	AddFieldToHeader(Account::DayGroups.Label);
	AddFieldToHeader(Account::Email.Label);
	AddFieldToHeader(Account::Info1.Label);
	AddFieldToHeader(Account::Info2.Label);
	AddFieldToHeader(Account::Info3.Label);
	AddFieldToHeader(Account::Info4.Label);
	AddFieldToHeader(Account::Info5.Label);
	AddFieldToHeader(Account::Info6.Label);
	AddFieldToHeader(Account::Info7.Label);
	AddFieldToHeader(Account::Info8.Label);
	AddFieldToHeader(Account::Tick1.Label);
	AddFieldToHeader(Account::Tick2.Label);
	AddFieldToHeader(Account::Tick3.Label);
	AddFieldToHeader(Account::Tick4.Label);
	AddFieldToHeader(Account::AllergyCodes.Label);
	AddFieldToHeader(Account::Dietary.Label);
	/*****/
	AddFieldToHeader(Account::Inactive.Label);
	AddFieldToHeader(Account::MemberID.Label);
	AddFieldToHeader(Account::ExternalRef1.Label, FALSE);
	AddFieldToHeader(Account::ExternalRef2.Label, FALSE);
	AddFieldToHeader(Account::PreviousUserID.Label, FALSE);
	AddFieldToHeader(Account::NextUserID.Label, FALSE);
	/*****/
	AddFieldToHeader(Account::Purse1Allow.Label);
	AddFieldToHeader(Account::Purse1Liability.Label);
	AddFieldToHeader(Account::Purse1Credit.Label);
	AddFieldToHeader(Account::Purse1LastSpendDate.Label);
	AddFieldToHeader(Account::Purse1LastSpendTime.Label);
	AddFieldToHeader(Account::Purse1LastSpend.Label);
	AddFieldToHeader(Account::Purse1SpendToDate.Label);
	AddFieldToHeader(Account::Purse2Allow.Label);
	/*****/
	AddFieldToHeader(Account::Purse2Balance.Label);
	AddFieldToHeader(Account::Purse2LastSpendDate.Label);
	AddFieldToHeader(Account::Purse2LastSpendTime.Label);
	AddFieldToHeader(Account::Purse2LastSpend.Label);
	AddFieldToHeader(Account::Purse2SpendToDate.Label);
	AddFieldToHeader(Account::Purse2RefreshFlags.Label);
	AddFieldToHeader(Account::Purse2RefreshedDate.Label);
	AddFieldToHeader(Account::Purse2RefreshedTime.Label);
	/*****/
	AddFieldToHeader(Account::Purse3Allow.Label);
	AddFieldToHeader(Account::Purse3Liability.Label);
	AddFieldToHeader(Account::Purse3Credit.Label);
	AddFieldToHeader(Account::Purse3LastSpendDate.Label);
	AddFieldToHeader(Account::Purse3LastSpendTime.Label);
	AddFieldToHeader(Account::Purse3LastSpend.Label);
	AddFieldToHeader(Account::Purse3SpendToDate.Label);
	/*****/
	AddFieldToHeader(Account::CashSpendToDate.Label);
	/*****/
	AddFieldToHeader(Account::Points.Label);
	AddFieldToHeader(Account::PointsToDate.Label);
	/*****/
	AddFieldToHeader(Account::OwnMaxSpend.Label);
	/*****/
	AddFieldToHeader(Account::MaxSpendPurse1Period1.Label);
	AddFieldToHeader(Account::MaxSpendPurse1Period2.Label);
	AddFieldToHeader(Account::MaxSpendPurse1Period3.Label);
	AddFieldToHeader(Account::MaxSpendPurse1Period4.Label);
	AddFieldToHeader(Account::MaxSpendPurse1Period5.Label);
	AddFieldToHeader(Account::MaxSpendPurse1Period6.Label);
	/*****/
	AddFieldToHeader(Account::MaxSpendPurse3Period1.Label);
	AddFieldToHeader(Account::MaxSpendPurse3Period2.Label);
	AddFieldToHeader(Account::MaxSpendPurse3Period3.Label);
	AddFieldToHeader(Account::MaxSpendPurse3Period4.Label);
	AddFieldToHeader(Account::MaxSpendPurse3Period5.Label);
	AddFieldToHeader(Account::MaxSpendPurse3Period6.Label);
	/*****/
	AddFieldToHeader(Account::MaxOverdraftPurse1Period1.Label);
	AddFieldToHeader(Account::MaxOverdraftPurse1Period2.Label);
	AddFieldToHeader(Account::MaxOverdraftPurse1Period3.Label);
	AddFieldToHeader(Account::MaxOverdraftPurse1Period4.Label);
	AddFieldToHeader(Account::MaxOverdraftPurse1Period5.Label);
	AddFieldToHeader(Account::MaxOverdraftPurse1Period6.Label);
	/*****/
	AddFieldToHeader(Account::MaxOverdraftPurse3Period1.Label);
	AddFieldToHeader(Account::MaxOverdraftPurse3Period2.Label);
	AddFieldToHeader(Account::MaxOverdraftPurse3Period3.Label);
	AddFieldToHeader(Account::MaxOverdraftPurse3Period4.Label);
	AddFieldToHeader(Account::MaxOverdraftPurse3Period5.Label);
	AddFieldToHeader(Account::MaxOverdraftPurse3Period6.Label);
	/*****/
	AddFieldToHeader(Account::AlertCode.Label);
	AddFieldToHeader(Account::AlertDate.Label);
	AddFieldToHeader(Account::AlertTime.Label);
}

//$$********************************************************************

void CSmartPayHeaderHelpers::AddFieldToHeader(CString strHeader, bool bAllowWithBiometric)
{
	if (m_strHeaderFull != "")
	{
		m_strHeaderFull += ",";
	}

	m_strHeaderFull += strHeader;

	if (TRUE == bAllowWithBiometric)
	{
		if (m_strHeaderBiometric != "")
		{
			m_strHeaderBiometric += ",";
		}

		m_strHeaderBiometric += strHeader;
	}
}

//$$********************************************************************

void CSmartPayHeaderHelpers::AddFieldToHeaderMap(CString strField, int nFieldNo)
{
	strField.MakeUpper();
	m_HeaderMap.SetAt(strField, nFieldNo);
}

//$$********************************************************************

int CSmartPayHeaderHelpers::GetNumericField(CString strField)
{
	CArray<int, int> arrayHeader;
	CreateNumericHeader(strField, arrayHeader);

	if (arrayHeader.GetSize() > 0)
	{
		return arrayHeader.GetAt(0);
	}
	else
	{
		return -1;
	}
}

//$$********************************************************************

void CSmartPayHeaderHelpers::CreateNumericHeader(CString strHeader, CArray<int, int>& arrayHeader)
{
	CCSV csvHeader(strHeader);
	CreateNumericHeader(csvHeader, arrayHeader);
}

//$$********************************************************************

void CSmartPayHeaderHelpers::CreateNumericHeader(CCSV& csvHeader, CArray<int, int>& arrayHeader)
{
	arrayHeader.RemoveAll();

	if (m_HeaderMap.GetCount() == 0)
	{
		AddFieldToHeaderMap(Account::UserID.Label, SMARTPAYFIELD_USERID);
		AddFieldToHeaderMap("CardNo", SMARTPAYFIELD_USERID);
		AddFieldToHeaderMap(Account::Username.Label, SMARTPAYFIELD_USERNAME);
		AddFieldToHeaderMap(Account::Forename.Label, SMARTPAYFIELD_FORENAME);
		AddFieldToHeaderMap(Account::Surname.Label, SMARTPAYFIELD_SURNAME);
		AddFieldToHeaderMap(Account::DOB.Label, SMARTPAYFIELD_DOB);
		AddFieldToHeaderMap(Account::ExpiryDate.Label, SMARTPAYFIELD_EXPIRYDATE);
		AddFieldToHeaderMap("Expiry", SMARTPAYFIELD_EXPIRYDATE);
		AddFieldToHeaderMap(Account::Gender.Label, SMARTPAYFIELD_GENDER);
		AddFieldToHeaderMap(Account::GroupNo.Label, SMARTPAYFIELD_GROUPNO);
		AddFieldToHeaderMap(Account::GroupShiftStatus.Label, SMARTPAYFIELD_GROUPSHIFT_STATUS);
		AddFieldToHeaderMap(Account::GroupShiftStartDate.Label, SMARTPAYFIELD_GROUPSHIFT_STARTDATE);
		AddFieldToHeaderMap(Account::GroupShiftEndDate.Label, SMARTPAYFIELD_GROUPSHIFT_ENDDATE);
		AddFieldToHeaderMap(Account::GroupShiftFutureGroup.Label, SMARTPAYFIELD_GROUPSHIFT_FUTUREGROUP);
		AddFieldToHeaderMap(Account::DayGroups.Label, SMARTPAYFIELD_DAYGROUPS);
		AddFieldToHeaderMap(Account::Email.Label, SMARTPAYFIELD_EMAIL);
		AddFieldToHeaderMap(Account::Info1.Label, SMARTPAYFIELD_INFO1);
		AddFieldToHeaderMap(Account::Info2.Label, SMARTPAYFIELD_INFO2);
		AddFieldToHeaderMap(Account::Info3.Label, SMARTPAYFIELD_INFO3);
		AddFieldToHeaderMap(Account::Info4.Label, SMARTPAYFIELD_INFO4);
		AddFieldToHeaderMap(Account::Info5.Label, SMARTPAYFIELD_INFO5);
		AddFieldToHeaderMap(Account::Info6.Label, SMARTPAYFIELD_INFO6);
		AddFieldToHeaderMap(Account::Info7.Label, SMARTPAYFIELD_INFO7);
		AddFieldToHeaderMap(Account::Info8.Label, SMARTPAYFIELD_INFO8);
		AddFieldToHeaderMap(Account::Tick1.Label, SMARTPAYFIELD_TICK1);
		AddFieldToHeaderMap(Account::Tick2.Label, SMARTPAYFIELD_TICK2);
		AddFieldToHeaderMap(Account::Tick3.Label, SMARTPAYFIELD_TICK3);
		AddFieldToHeaderMap(Account::Tick4.Label, SMARTPAYFIELD_TICK4);
		AddFieldToHeaderMap(Account::AllergyCodes.Label, SMARTPAYFIELD_ALLERGYCODES);
		AddFieldToHeaderMap("Allergy", SMARTPAYFIELD_ALLERGYCODES);
		AddFieldToHeaderMap(Account::Dietary.Label, SMARTPAYFIELD_DIETARY);
		AddFieldToHeaderMap(Account::Inactive.Label, SMARTPAYFIELD_INACTIVE);
		AddFieldToHeaderMap(Account::MemberID.Label, SMARTPAYFIELD_MEMBERID);
		AddFieldToHeaderMap(Account::ExternalRef1.Label, SMARTPAYFIELD_EXTERNALREF1);
		AddFieldToHeaderMap("ExtRef", SMARTPAYFIELD_EXTERNALREF1);
		AddFieldToHeaderMap(Account::ExternalRef2.Label, SMARTPAYFIELD_EXTERNALREF2);
		AddFieldToHeaderMap("ExtRef2", SMARTPAYFIELD_EXTERNALREF2);
		AddFieldToHeaderMap(Account::PreviousUserID.Label, SMARTPAYFIELD_PREVIOUS_USERID);
		AddFieldToHeaderMap("PreviousNo", SMARTPAYFIELD_PREVIOUS_USERID);
		AddFieldToHeaderMap(Account::NextUserID.Label, SMARTPAYFIELD_NEXT_USERID);
		AddFieldToHeaderMap("NextNo", SMARTPAYFIELD_NEXT_USERID);
		AddFieldToHeaderMap(Account::Purse1Allow.Label, SMARTPAYFIELD_PURSE1_ALLOW);
		AddFieldToHeaderMap("P1Allow", SMARTPAYFIELD_PURSE1_ALLOW);
		AddFieldToHeaderMap(Account::Purse1Liability.Label, SMARTPAYFIELD_PURSE1_LIABILITY);
		AddFieldToHeaderMap("Purse1", SMARTPAYFIELD_PURSE1_LIABILITY);
		AddFieldToHeaderMap(Account::Purse1Credit.Label, SMARTPAYFIELD_PURSE1_CREDIT);
		AddFieldToHeaderMap(Account::Purse1LastSpendDate.Label, SMARTPAYFIELD_PURSE1_LASTSPENDDATE);
		AddFieldToHeaderMap("Purse1Date", SMARTPAYFIELD_PURSE1_LASTSPENDDATE);
		AddFieldToHeaderMap(Account::Purse1LastSpendTime.Label, SMARTPAYFIELD_PURSE1_LASTSPENDTIME);
		AddFieldToHeaderMap("Purse1Time", SMARTPAYFIELD_PURSE1_LASTSPENDTIME);
		AddFieldToHeaderMap(Account::Purse1LastSpend.Label, SMARTPAYFIELD_PURSE1_LASTSPEND);
		AddFieldToHeaderMap("Purse1Spend", SMARTPAYFIELD_PURSE1_LASTSPEND);
		AddFieldToHeaderMap(Account::Purse1SpendToDate.Label, SMARTPAYFIELD_PURSE1_SPENDTODATE);
		AddFieldToHeaderMap("Purse1SpendTD", SMARTPAYFIELD_PURSE1_SPENDTODATE);
		AddFieldToHeaderMap(Account::Purse2Allow.Label, SMARTPAYFIELD_PURSE2_ALLOW);
		AddFieldToHeaderMap("P2Allow", SMARTPAYFIELD_PURSE2_ALLOW);
		AddFieldToHeaderMap(Account::Purse2Balance.Label, SMARTPAYFIELD_PURSE2_BALANCE);
		AddFieldToHeaderMap("Purse2", SMARTPAYFIELD_PURSE2_BALANCE);
		AddFieldToHeaderMap(Account::Purse2LastSpendDate.Label, SMARTPAYFIELD_PURSE2_LASTSPENDDATE);
		AddFieldToHeaderMap("Purse2Date", SMARTPAYFIELD_PURSE2_LASTSPENDDATE);
		AddFieldToHeaderMap(Account::Purse2LastSpendTime.Label, SMARTPAYFIELD_PURSE2_LASTSPENDTIME);
		AddFieldToHeaderMap("Purse2Time", SMARTPAYFIELD_PURSE2_LASTSPENDTIME);
		AddFieldToHeaderMap(Account::Purse2LastSpend.Label, SMARTPAYFIELD_PURSE2_LASTSPEND);
		AddFieldToHeaderMap("Purse2Spend", SMARTPAYFIELD_PURSE2_LASTSPEND);
		AddFieldToHeaderMap(Account::Purse2SpendToDate.Label, SMARTPAYFIELD_PURSE2_SPENDTODATE);
		AddFieldToHeaderMap("Purse2SpendTD", SMARTPAYFIELD_PURSE2_SPENDTODATE);
		AddFieldToHeaderMap(Account::Purse2RefreshFlags.Label, SMARTPAYFIELD_PURSE2_REFRESHFLAGS);
		AddFieldToHeaderMap("RefreshRef", SMARTPAYFIELD_PURSE2_REFRESHFLAGS);
		AddFieldToHeaderMap(Account::Purse2RefreshedDate.Label, SMARTPAYFIELD_PURSE2_REFRESHED_DATE);
		AddFieldToHeaderMap("RefreshDate", SMARTPAYFIELD_PURSE2_REFRESHED_DATE);
		AddFieldToHeaderMap(Account::Purse2RefreshedTime.Label, SMARTPAYFIELD_PURSE2_REFRESHED_TIME);
		AddFieldToHeaderMap("RefreshTime", SMARTPAYFIELD_PURSE2_REFRESHED_TIME);
		AddFieldToHeaderMap(Account::Purse3Allow.Label, SMARTPAYFIELD_PURSE3_ALLOW);
		AddFieldToHeaderMap("P3Allow", SMARTPAYFIELD_PURSE3_ALLOW);
		AddFieldToHeaderMap(Account::Purse3Liability.Label, SMARTPAYFIELD_PURSE3_LIABILITY);
		AddFieldToHeaderMap("Purse3", SMARTPAYFIELD_PURSE3_LIABILITY);
		AddFieldToHeaderMap(Account::Purse3Credit.Label, SMARTPAYFIELD_PURSE3_CREDIT);
		AddFieldToHeaderMap(Account::Purse3LastSpendDate.Label, SMARTPAYFIELD_PURSE3_LASTSPENDDATE);
		AddFieldToHeaderMap("Purse3Date", SMARTPAYFIELD_PURSE3_LASTSPENDDATE);
		AddFieldToHeaderMap(Account::Purse3LastSpendTime.Label, SMARTPAYFIELD_PURSE3_LASTSPENDTIME);
		AddFieldToHeaderMap("Purse3Time", SMARTPAYFIELD_PURSE3_LASTSPENDTIME);
		AddFieldToHeaderMap(Account::Purse3LastSpend.Label, SMARTPAYFIELD_PURSE3_LASTSPEND);
		AddFieldToHeaderMap("Purse3Spend", SMARTPAYFIELD_PURSE3_LASTSPEND);
		AddFieldToHeaderMap(Account::Purse3SpendToDate.Label, SMARTPAYFIELD_PURSE3_SPENDTODATE);
		AddFieldToHeaderMap("Purse3SpendTD", SMARTPAYFIELD_PURSE3_SPENDTODATE);
		AddFieldToHeaderMap(Account::CashSpendToDate.Label, SMARTPAYFIELD_CASH_SPENDTODATE);
		AddFieldToHeaderMap("CashSpendTD", SMARTPAYFIELD_CASH_SPENDTODATE);
		AddFieldToHeaderMap(Account::Points.Label, SMARTPAYFIELD_POINTS);
		AddFieldToHeaderMap(Account::PointsToDate.Label, SMARTPAYFIELD_POINTS_TODATE);
		AddFieldToHeaderMap("PointsTD", SMARTPAYFIELD_POINTS_TODATE);
		AddFieldToHeaderMap(Account::OwnMaxSpend.Label, SMARTPAYFIELD_OWNMAXSPEND);
		AddFieldToHeaderMap(Account::MaxSpendPurse1Period1.Label, SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD1);
		AddFieldToHeaderMap("MaxSpend", SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD1);
		AddFieldToHeaderMap(Account::MaxSpendPurse1Period2.Label, SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD2);
		AddFieldToHeaderMap("MSV2", SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD2);
		AddFieldToHeaderMap(Account::MaxSpendPurse1Period3.Label, SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD3);
		AddFieldToHeaderMap("MSV3", SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD3);
		AddFieldToHeaderMap(Account::MaxSpendPurse1Period4.Label, SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD4);
		AddFieldToHeaderMap("MSV4", SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD4);
		AddFieldToHeaderMap(Account::MaxSpendPurse1Period5.Label, SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD5);
		AddFieldToHeaderMap("MSV5", SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD5);
		AddFieldToHeaderMap(Account::MaxSpendPurse1Period6.Label, SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD6);
		AddFieldToHeaderMap("MSV6", SMARTPAYFIELD_MAXSPEND_PURSE1_PERIOD6);
		AddFieldToHeaderMap(Account::MaxSpendPurse3Period1.Label, SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD1);
		AddFieldToHeaderMap("MaxSpendP3", SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD1);
		AddFieldToHeaderMap(Account::MaxSpendPurse3Period2.Label, SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD2);
		AddFieldToHeaderMap("MSV32", SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD2);
		AddFieldToHeaderMap(Account::MaxSpendPurse3Period3.Label, SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD3);
		AddFieldToHeaderMap("MSV33", SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD3);
		AddFieldToHeaderMap(Account::MaxSpendPurse3Period4.Label, SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD4);
		AddFieldToHeaderMap("MSV34", SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD4);
		AddFieldToHeaderMap(Account::MaxSpendPurse3Period5.Label, SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD5);
		AddFieldToHeaderMap("MSV35", SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD5);
		AddFieldToHeaderMap(Account::MaxSpendPurse3Period6.Label, SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD6);
		AddFieldToHeaderMap("MSV36", SMARTPAYFIELD_MAXSPEND_PURSE3_PERIOD6);
		AddFieldToHeaderMap(Account::MaxOverdraftPurse1Period1.Label, SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD1);
		AddFieldToHeaderMap("OverSpend", SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD1);
		AddFieldToHeaderMap(Account::MaxOverdraftPurse1Period2.Label, SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD2);
		AddFieldToHeaderMap("OSV2", SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD2);
		AddFieldToHeaderMap(Account::MaxOverdraftPurse1Period3.Label, SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD3);
		AddFieldToHeaderMap("OSV3", SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD3);
		AddFieldToHeaderMap(Account::MaxOverdraftPurse1Period4.Label, SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD4);
		AddFieldToHeaderMap("OSv4", SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD4);
		AddFieldToHeaderMap(Account::MaxOverdraftPurse1Period5.Label, SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD5);
		AddFieldToHeaderMap("OSV5", SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD5);
		AddFieldToHeaderMap(Account::MaxOverdraftPurse1Period6.Label, SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD6);
		AddFieldToHeaderMap("OSV6", SMARTPAYFIELD_MAXOVERDRAFT_PURSE1_PERIOD6);
		AddFieldToHeaderMap(Account::MaxOverdraftPurse3Period1.Label, SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD1);
		AddFieldToHeaderMap("OverSpendP3", SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD1);
		AddFieldToHeaderMap(Account::MaxOverdraftPurse3Period2.Label, SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD2);
		AddFieldToHeaderMap("OSV32", SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD2);
		AddFieldToHeaderMap(Account::MaxOverdraftPurse3Period3.Label, SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD3);
		AddFieldToHeaderMap("OSV33", SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD3);
		AddFieldToHeaderMap(Account::MaxOverdraftPurse3Period4.Label, SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD4);
		AddFieldToHeaderMap("OSV34", SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD4);
		AddFieldToHeaderMap(Account::MaxOverdraftPurse3Period5.Label, SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD5);
		AddFieldToHeaderMap("OSV35", SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD5);
		AddFieldToHeaderMap(Account::MaxOverdraftPurse3Period6.Label, SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD6);
		AddFieldToHeaderMap("OSV36", SMARTPAYFIELD_MAXOVERDRAFT_PURSE3_PERIOD6);
		AddFieldToHeaderMap(Account::AlertCode.Label,SMARTPAYFIELD_ALERTCODE);
		AddFieldToHeaderMap("Alert", SMARTPAYFIELD_ALERTCODE);
		AddFieldToHeaderMap(Account::AlertDate.Label, SMARTPAYFIELD_ALERTDATE);
		AddFieldToHeaderMap(Account::AlertTime.Label, SMARTPAYFIELD_ALERTTIME);
		/****/
		AddFieldToHeaderMap(Account::FullUserID.Label, SMARTPAYFIELD_FULLUSERID);
		AddFieldToHeaderMap(Account::ForenameSurname.Label, SMARTPAYFIELD_FORENAME_SURNAME);
		AddFieldToHeaderMap(Account::GroupType.Label, SMARTPAYFIELD_GROUP_TYPE);
		AddFieldToHeaderMap(Account::GroupUserID.Label, SMARTPAYFIELD_GROUP_USERID);
		AddFieldToHeaderMap(Account::GroupTypeUserID.Label, SMARTPAYFIELD_GROUP_TYPE_USERID);
		AddFieldToHeaderMap(Account::GroupFullUserID.Label, SMARTPAYFIELD_GROUP_FULLUSERID);
		AddFieldToHeaderMap(Account::GroupTypeFullUserID.Label, SMARTPAYFIELD_GROUP_TYPE_FULLUSERID);
		AddFieldToHeaderMap(Account::Text.Label, SMARTPAYFIELD_TEXT);
	}

	for (int n = 0; n < csvHeader.GetSize(); n++)
	{
		CString strField = csvHeader.GetString(n);
		strField.MakeUpper();

		int nIndex = 0;
		if (m_HeaderMap.Lookup(strField, nIndex) == 0)
		{
			arrayHeader.Add(-1);
		}
		else
		{
			arrayHeader.Add(nIndex);
		}
	}
}

//$$********************************************************************
