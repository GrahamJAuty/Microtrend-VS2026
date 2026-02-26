//$$********************************************************************
#include "SQLNamespaces.h"
//$$********************************************************************
#include "LoyaltyHeaderHelpers.h"
//$$********************************************************************

CLoyaltyHeaderHelpers::CLoyaltyHeaderHelpers()
{
	m_strHeaderFull = "";
	m_strHeaderNoPurse2 = "";
	m_HeaderMap.InitHashTable(103);
	m_HeaderMapMemberMojo.InitHashTable(103);

	AddFieldToHeader(Account::UserID.Label, FALSE);
	AddFieldToHeader(Account::Fullname.Label, FALSE);
	AddFieldToHeader(Account::Forename.Label, FALSE);
	AddFieldToHeader(Account::Surname.Label, FALSE);
	AddFieldToHeader(Account::DOB.Label, FALSE);
	AddFieldToHeader(Account::ExpiryDate.Label, FALSE);
	AddFieldToHeader(Account::Gender.Label, FALSE);
	AddFieldToHeader(Account::GroupNo.Label, FALSE);
	AddFieldToHeader(Account::SchemeNo.Label, FALSE);
	AddFieldToHeader(Account::Info1.Label, FALSE);
	AddFieldToHeader(Account::Info2.Label, FALSE);
	AddFieldToHeader(Account::Info3.Label, FALSE);
	AddFieldToHeader(Account::Info4.Label, FALSE);
	AddFieldToHeader(Account::Address1.Label, FALSE);
	AddFieldToHeader(Account::Address2.Label, FALSE);
	AddFieldToHeader(Account::Address3.Label, FALSE);
	AddFieldToHeader(Account::Address4.Label, FALSE);
	AddFieldToHeader(Account::Address5.Label, FALSE);
	AddFieldToHeader(Account::Phone1.Label, FALSE);
	AddFieldToHeader(Account::Phone2.Label, FALSE);
	AddFieldToHeader(Account::Inactive.Label, FALSE);
	AddFieldToHeader(Account::ExternalRef.Label, FALSE);
	AddFieldToHeader(Account::QRInfo.Label, FALSE);
	AddFieldToHeader(Account::PreviousUserID.Label, FALSE);
	AddFieldToHeader(Account::NextUserID.Label, FALSE);
	AddFieldToHeader(Account::Purse1Balance.Label, FALSE);
	AddFieldToHeader(Account::Purse1LastSpendDate.Label, FALSE);
	AddFieldToHeader(Account::Purse1LastSpendTime.Label, FALSE);
	AddFieldToHeader(Account::Purse1LastSpend.Label, FALSE);
	AddFieldToHeader(Account::Purse1SpendToDate.Label, FALSE);
	AddFieldToHeader(Account::Purse2Balance.Label, TRUE);
	AddFieldToHeader(Account::Purse2LastSpendDate.Label, TRUE);
	AddFieldToHeader(Account::Purse2LastSpendTime.Label, TRUE);
	AddFieldToHeader(Account::Purse2LastSpend.Label, TRUE);
	AddFieldToHeader(Account::Purse2SpendToDate.Label, TRUE);
	AddFieldToHeader(Account::Purse2RefreshedDate.Label, TRUE);
	AddFieldToHeader(Account::Purse2RefreshedTime.Label, TRUE);
	AddFieldToHeader(Account::CashLastSpendDate.Label, FALSE);
	AddFieldToHeader(Account::CashLastSpendTime.Label, FALSE);
	AddFieldToHeader(Account::CashLastSpend.Label, FALSE);
	AddFieldToHeader(Account::CashSpendToDate.Label, FALSE);
	AddFieldToHeader(Account::Points.Label, FALSE);
	AddFieldToHeader(Account::PointsToDate.Label, FALSE);
	AddFieldToHeader(Account::HotlistCode.Label, FALSE);
	AddFieldToHeader(Account::HotlistDate.Label, FALSE);
	AddFieldToHeader(Account::HotlistTime.Label, FALSE);
	AddFieldToHeader(Account::MaxSpend.Label, FALSE);
	AddFieldToHeader(Account::MaxOverdraft.Label, FALSE);
	AddFieldToHeader(Account::AlertCodes.Label, FALSE);
}

//$$********************************************************************

CLoyaltyHeaderHelpers::~CLoyaltyHeaderHelpers()
{
	m_HeaderMap.RemoveAll();
	m_HeaderMapMemberMojo.RemoveAll();
}

//$$********************************************************************

void CLoyaltyHeaderHelpers::AddFieldToHeader(CString strHeader, bool bIsPurse2)
{
	if (m_strHeaderFull != "")
	{
		m_strHeaderFull += ",";
	}

	m_strHeaderFull += strHeader;

	if (FALSE == bIsPurse2)
	{
		if (m_strHeaderNoPurse2 != "")
		{
			m_strHeaderNoPurse2 += ",";
		}

		m_strHeaderNoPurse2 += strHeader;
	}
}

//$$********************************************************************

void CLoyaltyHeaderHelpers::AddFieldToHeaderMap(CString strField, int nFieldNo)
{
	strField.MakeUpper();
	m_HeaderMap.SetAt(strField, nFieldNo);
}

//$$********************************************************************

void CLoyaltyHeaderHelpers::AddFieldToHeaderMapMemberMojo(CString strField, int nFieldNo)
{
	strField.MakeUpper();
	m_HeaderMapMemberMojo.SetAt(strField, nFieldNo);
}

//$$********************************************************************

int CLoyaltyHeaderHelpers::GetNumericField(CString strField)
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

void CLoyaltyHeaderHelpers::CreateNumericHeader(CString strHeader, CArray<int, int>& arrayHeader)
{
	CCSV csvHeader(strHeader);
	CreateNumericHeader(csvHeader, arrayHeader);
}

//$$********************************************************************

void CLoyaltyHeaderHelpers::CreateNumericHeader(CCSV& csvHeader, CArray<int, int>& arrayHeader)
{
	arrayHeader.RemoveAll();

	if (m_HeaderMap.GetCount() == 0)
	{
		AddFieldToHeaderMap(Account::UserID.Label, LOYALTYFIELD_USERID);
		AddFieldToHeaderMap("CardNo", LOYALTYFIELD_USERID);
		AddFieldToHeaderMap(Account::Fullname.Label, LOYALTYFIELD_FULLNAME);
		AddFieldToHeaderMap(Account::Forename.Label, LOYALTYFIELD_FORENAME);
		AddFieldToHeaderMap(Account::Surname.Label, LOYALTYFIELD_SURNAME);
		AddFieldToHeaderMap(Account::DOB.Label, LOYALTYFIELD_DOB);
		AddFieldToHeaderMap(Account::ExpiryDate.Label, LOYALTYFIELD_EXPIRYDATE);
		AddFieldToHeaderMap(Account::Gender.Label, LOYALTYFIELD_GENDER);
		AddFieldToHeaderMap(Account::GroupNo.Label, LOYALTYFIELD_GROUPNO);
		AddFieldToHeaderMap(Account::SchemeNo.Label, LOYALTYFIELD_SCHEME);
		AddFieldToHeaderMap(Account::Info1.Label, LOYALTYFIELD_INFO1);
		AddFieldToHeaderMap(Account::Info2.Label, LOYALTYFIELD_INFO2);
		AddFieldToHeaderMap(Account::Info3.Label, LOYALTYFIELD_INFO3);
		AddFieldToHeaderMap(Account::Info4.Label, LOYALTYFIELD_INFO4);
		AddFieldToHeaderMap(Account::Address1.Label, LOYALTYFIELD_ADDRESS1);
		AddFieldToHeaderMap(Account::Address2.Label, LOYALTYFIELD_ADDRESS2);
		AddFieldToHeaderMap(Account::Address3.Label, LOYALTYFIELD_ADDRESS3);
		AddFieldToHeaderMap(Account::Address4.Label, LOYALTYFIELD_ADDRESS4);
		AddFieldToHeaderMap(Account::Address5.Label, LOYALTYFIELD_ADDRESS5);
		AddFieldToHeaderMap(Account::Phone1.Label, LOYALTYFIELD_PHONE1);
		AddFieldToHeaderMap("Telephone1", LOYALTYFIELD_PHONE1);
		AddFieldToHeaderMap(Account::Phone2.Label, LOYALTYFIELD_PHONE2);
		AddFieldToHeaderMap("Telephone2", LOYALTYFIELD_PHONE2);
		AddFieldToHeaderMap(Account::Inactive.Label, LOYALTYFIELD_INACTIVE);
		AddFieldToHeaderMap(Account::ExternalRef.Label, LOYALTYFIELD_EXTERNALREF);
		AddFieldToHeaderMap("ExtRef", LOYALTYFIELD_EXTERNALREF);
		AddFieldToHeaderMap(Account::QRInfo.Label, LOYALTYFIELD_QRINFO);
		AddFieldToHeaderMap(Account::PreviousUserID.Label, LOYALTYFIELD_PREVUSERID);
		AddFieldToHeaderMap("PreviousNo", LOYALTYFIELD_PREVUSERID);
		AddFieldToHeaderMap(Account::NextUserID.Label, LOYALTYFIELD_NEXTUSERID);
		AddFieldToHeaderMap("NextNo", LOYALTYFIELD_NEXTUSERID);
		AddFieldToHeaderMap(Account::Purse1Balance.Label, LOYALTYFIELD_PURSE1_BALANCE);
		AddFieldToHeaderMap("Purse", LOYALTYFIELD_PURSE1_BALANCE);
		AddFieldToHeaderMap("Purse1", LOYALTYFIELD_PURSE1_BALANCE);
		AddFieldToHeaderMap(Account::Purse1LastSpendDate.Label, LOYALTYFIELD_PURSE1_LASTSPENDDATE);
		AddFieldToHeaderMap("LastUsedDate", LOYALTYFIELD_PURSE1_LASTSPENDDATE);
		AddFieldToHeaderMap(Account::Purse1LastSpendTime.Label, LOYALTYFIELD_PURSE1_LASTSPENDTIME);
		AddFieldToHeaderMap("LastUsedTime", LOYALTYFIELD_PURSE1_LASTSPENDTIME);
		AddFieldToHeaderMap(Account::Purse1LastSpend.Label, LOYALTYFIELD_PURSE1_LASTSPEND);
		AddFieldToHeaderMap("CardSpend", LOYALTYFIELD_PURSE1_LASTSPEND);
		AddFieldToHeaderMap(Account::Purse1SpendToDate.Label, LOYALTYFIELD_PURSE1_SPENDTODATE);
		AddFieldToHeaderMap("CardSpendTD", LOYALTYFIELD_PURSE1_SPENDTODATE);
		AddFieldToHeaderMap("Purse1SpendTD", LOYALTYFIELD_PURSE1_SPENDTODATE);
		AddFieldToHeaderMap(Account::Purse2Balance.Label, LOYALTYFIELD_PURSE2_BALANCE);
		AddFieldToHeaderMap("Purse2", LOYALTYFIELD_PURSE2_BALANCE);
		AddFieldToHeaderMap(Account::Purse2LastSpendDate.Label, LOYALTYFIELD_PURSE2_LASTSPENDDATE);
		AddFieldToHeaderMap("Purse2Date", LOYALTYFIELD_PURSE2_LASTSPENDDATE);
		AddFieldToHeaderMap(Account::Purse2LastSpendTime.Label, LOYALTYFIELD_PURSE2_LASTSPENDTIME);
		AddFieldToHeaderMap("Purse2Time", LOYALTYFIELD_PURSE2_LASTSPENDTIME);
		AddFieldToHeaderMap(Account::Purse2LastSpend.Label, LOYALTYFIELD_PURSE2_LASTSPEND);
		AddFieldToHeaderMap("Purse2Spend", LOYALTYFIELD_PURSE2_LASTSPEND);
		AddFieldToHeaderMap(Account::Purse2SpendToDate.Label, LOYALTYFIELD_PURSE2_SPENDTODATE);
		AddFieldToHeaderMap("Purse2SpendTD", LOYALTYFIELD_PURSE2_SPENDTODATE);
		AddFieldToHeaderMap(Account::Purse2RefreshedDate.Label, LOYALTYFIELD_PURSE2_REFRESHEDDATE);
		AddFieldToHeaderMap("RefreshDate", LOYALTYFIELD_PURSE2_REFRESHEDDATE);
		AddFieldToHeaderMap(Account::Purse2RefreshedTime.Label, LOYALTYFIELD_PURSE2_REFRESHEDTIME);
		AddFieldToHeaderMap("RefreshTime", LOYALTYFIELD_PURSE2_REFRESHEDTIME);
		AddFieldToHeaderMap(Account::CashLastSpendDate.Label, LOYALTYFIELD_CASH_LASTSPENDDATE);
		AddFieldToHeaderMap(Account::CashLastSpendTime.Label, LOYALTYFIELD_CASH_LASTSPENDTIME);
		AddFieldToHeaderMap(Account::CashLastSpend.Label, LOYALTYFIELD_CASH_LASTSPEND);
		AddFieldToHeaderMap(Account::CashSpendToDate.Label, LOYALTYFIELD_CASH_SPENDTODATE);
		AddFieldToHeaderMap("CashSpendTD", LOYALTYFIELD_CASH_SPENDTODATE);
		AddFieldToHeaderMap(Account::Points.Label, LOYALTYFIELD_POINTS);
		AddFieldToHeaderMap(Account::PointsToDate.Label, LOYALTYFIELD_POINTSTODATE);
		AddFieldToHeaderMap("PointsTD", LOYALTYFIELD_POINTSTODATE);
		AddFieldToHeaderMap(Account::HotlistCode.Label, LOYALTYFIELD_HOTLISTCODE);
		AddFieldToHeaderMap(Account::HotlistDate.Label, LOYALTYFIELD_HOTLISTDATE);
		AddFieldToHeaderMap(Account::HotlistTime.Label, LOYALTYFIELD_HOTLISTTIME);		
		AddFieldToHeaderMap(Account::MaxSpend.Label, LOYALTYFIELD_MAXSPEND);
		AddFieldToHeaderMap(Account::MaxOverdraft.Label, LOYALTYFIELD_MAXOVERDRAFT);
		AddFieldToHeaderMap("Overspend", LOYALTYFIELD_MAXOVERDRAFT);
		AddFieldToHeaderMap(Account::AlertCodes.Label, LOYALTYFIELD_ALERTCODES);
		AddFieldToHeaderMap("Alert", LOYALTYFIELD_ALERTCODES);
		/*****/
		AddFieldToHeaderMap(Account::Blank.Label, LOYALTYFIELD_BLANK);
		AddFieldToHeaderMap(Account::TotalSpendToDate.Label, LOYALTYFIELD_TOTAL_SPENDTODATE);
		AddFieldToHeaderMap(Account::DOBNextAge.Label, LOYALTYFIELD_DOB_NEXTAGE);
		AddFieldToHeaderMap(Account::DOBDay.Label, LOYALTYFIELD_DOB_DAY);
		AddFieldToHeaderMap(Account::DOBCheck.Label, LOYALTYFIELD_DOB_CHECK);
		AddFieldToHeaderMap(Account::DeletedDate.Label, LOYALTYFIELD_DELDATE);
		AddFieldToHeaderMap(Account::DeletedTime.Label, LOYALTYFIELD_DELTIME);
		AddFieldToHeaderMap(Account::Text.Label, LOYALTYFIELD_TEXT);
		AddFieldToHeaderMap(Account::CardNameWithBalance.Label, LOYALTYFIELD_NAME_BALANCE);
		AddFieldToHeaderMap(Account::Balance.Label, LOYALTYFIELD_BALANCE);
		AddFieldToHeaderMap(Account::BonusCheck.Label, LOYALTYFIELD_BONUS_CHECK);
		AddFieldToHeaderMap(Account::SkipCheck.Label, LOYALTYFIELD_SKIP_CHECK);
		AddFieldToHeaderMap(Account::AlertCheck.Label, LOYALTYFIELD_ALERT_CHECK);
		AddFieldToHeaderMap(Account::FullUserID.Label, LOYALTYFIELD_FULLCARDNO);
		AddFieldToHeaderMap(Account::GroupUserID.Label, LOYALTYFIELD_GROUPCARDNO);
		AddFieldToHeaderMap(Account::GroupFullUserID.Label, LOYALTYFIELD_GROUPFULLCARDNO);
		AddFieldToHeaderMap("Purse1TopUp", LOYALTYFIELD_PURSE1_TOPUP);
		AddFieldToHeaderMap("Purse2TopUp", LOYALTYFIELD_PURSE2_TOPUP);
		AddFieldToHeaderMap(Account::StampOffers.Label, LOYALTYFIELD_STAMPOFFER);
		AddFieldToHeaderMap(Account::AvailableSpend.Label, LOYALTYFIELD_AVAILABLESPEND);
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

void CLoyaltyHeaderHelpers::CreateNumericHeaderMemberMojo(CString strHeader, CArray<int, int>& arrayHeader)
{
	CCSV csvHeader(strHeader);
	CreateNumericHeaderMemberMojo(csvHeader, arrayHeader);
}

//$$********************************************************************

void CLoyaltyHeaderHelpers::CreateNumericHeaderMemberMojo(CCSV& csvHeader, CArray<int, int>& arrayHeader)
{
	arrayHeader.RemoveAll();

	if (m_HeaderMapMemberMojo.GetCount() == 0)
	{
		AddFieldToHeaderMapMemberMojo(Account::UserID.Label, LOYALTYFIELD_USERID);
		AddFieldToHeaderMapMemberMojo("First name", LOYALTYFIELD_FORENAME);
		AddFieldToHeaderMapMemberMojo("Last name", LOYALTYFIELD_SURNAME);
		AddFieldToHeaderMapMemberMojo("Expires on", LOYALTYFIELD_EXPIRYDATE);
		AddFieldToHeaderMapMemberMojo(Account::GroupNo.Label, LOYALTYFIELD_GROUPNO);
		AddFieldToHeaderMapMemberMojo(Account::Address1.Label, LOYALTYFIELD_ADDRESS1);
		AddFieldToHeaderMapMemberMojo("Address line 1", LOYALTYFIELD_ADDRESS1);
		AddFieldToHeaderMapMemberMojo(Account::Address2.Label, LOYALTYFIELD_ADDRESS2);
		AddFieldToHeaderMapMemberMojo("Address line 2", LOYALTYFIELD_ADDRESS2);
		AddFieldToHeaderMapMemberMojo(Account::Address3.Label, LOYALTYFIELD_ADDRESS3);
		AddFieldToHeaderMapMemberMojo("Address line 3", LOYALTYFIELD_ADDRESS3);
		AddFieldToHeaderMapMemberMojo(Account::Address4.Label, LOYALTYFIELD_ADDRESS4);
		AddFieldToHeaderMapMemberMojo("Address line 4", LOYALTYFIELD_ADDRESS4);
		AddFieldToHeaderMapMemberMojo(Account::Address5.Label, LOYALTYFIELD_ADDRESS5);
		AddFieldToHeaderMapMemberMojo("Address line 5", LOYALTYFIELD_ADDRESS5);
		AddFieldToHeaderMapMemberMojo("Postcode", LOYALTYFIELD_ADDRESS5);
		AddFieldToHeaderMapMemberMojo(Account::Phone1.Label, LOYALTYFIELD_PHONE1);
		AddFieldToHeaderMapMemberMojo("Contact number", LOYALTYFIELD_PHONE1);
		AddFieldToHeaderMapMemberMojo(Account::Phone2.Label, LOYALTYFIELD_PHONE2);
		AddFieldToHeaderMapMemberMojo("Mobile number", LOYALTYFIELD_PHONE2);
		AddFieldToHeaderMapMemberMojo("Short URL", LOYALTYFIELD_QRINFO);
	}

	for (int n = 0; n < csvHeader.GetSize(); n++)
	{
		CString strField = csvHeader.GetString(n);
		strField.MakeUpper();

		int nIndex = 0;
		if (m_HeaderMapMemberMojo.Lookup(strField, nIndex) == 0)
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

void CLoyaltyHeaderHelpers::UpdateLegacyField(CString& strField)
{
	switch (GetNumericField(strField))
	{
	case LOYALTYFIELD_USERID:
		strField = Account::UserID.Label;
		break;

	case LOYALTYFIELD_PHONE1:
		strField = Account::Phone1.Label;
		break;

	case LOYALTYFIELD_PHONE2:
		strField = Account::Phone2.Label;
		break;

	case LOYALTYFIELD_EXTERNALREF:
		strField = Account::ExternalRef.Label;
		break;

	case LOYALTYFIELD_PREVUSERID:
		strField = Account::PreviousUserID.Label;
		break;

	case LOYALTYFIELD_NEXTUSERID:
		strField = Account::NextUserID.Label;
		break;

	case LOYALTYFIELD_PURSE1_BALANCE:
		strField = Account::Purse1Balance.Label;
		break;

	case LOYALTYFIELD_PURSE1_LASTSPENDDATE:
		strField = Account::Purse1LastSpendDate.Label;
		break;

	case LOYALTYFIELD_PURSE1_LASTSPENDTIME:
		strField = Account::Purse1LastSpendTime.Label;
		break;

	case LOYALTYFIELD_PURSE1_LASTSPEND:
		strField = Account::Purse1LastSpend.Label;
		break;

	case LOYALTYFIELD_PURSE1_SPENDTODATE:
		strField = Account::Purse1SpendToDate.Label;
		break;

	case LOYALTYFIELD_PURSE2_BALANCE:
		strField = Account::Purse2Balance.Label;
		break;

	case LOYALTYFIELD_PURSE2_LASTSPENDDATE:
		strField = Account::Purse2LastSpendDate.Label;
		break;

	case LOYALTYFIELD_PURSE2_LASTSPENDTIME:
		strField = Account::Purse2LastSpendTime.Label;
		break;

	case LOYALTYFIELD_PURSE2_LASTSPEND:
		strField = Account::Purse2LastSpend.Label;
		break;

	case LOYALTYFIELD_PURSE2_SPENDTODATE:
		strField = Account::Purse2SpendToDate.Label;
		break;

	case LOYALTYFIELD_PURSE2_REFRESHEDDATE:
		strField = Account::Purse2RefreshedDate.Label;
		break;

	case LOYALTYFIELD_PURSE2_REFRESHEDTIME:
		strField = Account::Purse2RefreshedTime.Label;
		break;

	case LOYALTYFIELD_CASH_SPENDTODATE:
		strField = Account::CashSpendToDate.Label;
		break;

	case LOYALTYFIELD_POINTSTODATE:
		strField = Account::PointsToDate.Label;
		break;

	case LOYALTYFIELD_MAXOVERDRAFT:
		strField = Account::MaxOverdraft.Label;
		break;

	case LOYALTYFIELD_ALERTCODES:
		strField = Account::AlertCodes.Label;
		break;
	}
}

//$$********************************************************************
