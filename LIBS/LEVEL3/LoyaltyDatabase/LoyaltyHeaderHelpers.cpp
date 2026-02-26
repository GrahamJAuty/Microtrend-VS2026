//$$********************************************************************
#include "LoyaltyDatabase.hpp"
//$$********************************************************************

CLoyaltyHeaderHelpers::CLoyaltyHeaderHelpers()
{
	m_strHeaderFull = "";
	m_strHeaderNoPurse2 = "";
	m_HeaderMap.InitHashTable(103);

	CString strLegacy = "";
	AddLegacyField("Purse", Loyalty::Purse1.Label, strLegacy);
	AddLegacyField("CardSpend", Loyalty::Purse1LastSpend.Label, strLegacy);
	AddLegacyField("LastUsedDate", Loyalty::Purse1LastSpendDate.Label, strLegacy);
	AddLegacyField("LastUsedTime", Loyalty::Purse1LastSpendTime.Label, strLegacy);
	AddLegacyField("CardSpendTD", Loyalty::Purse1SpendTD.Label, strLegacy);
	AddLegacyField("Purse2Spend", Loyalty::Purse2LastSpend.Label, strLegacy);
	AddLegacyField("Purse2Date", Loyalty::Purse2LastSpendDate.Label, strLegacy);
	AddLegacyField("Purse2Time", Loyalty::Purse2LastSpendTime.Label, strLegacy);
	AddLegacyField("RefreshDate", Loyalty::Purse2RefreshedDate.Label, strLegacy);
	AddLegacyField("RefreshTime", Loyalty::Purse2RefreshedTime.Label, strLegacy);
	SetLegacyHeaders(strLegacy);

	AddFieldToHeader(Loyalty::ExCardNo.Label, FALSE);
	AddFieldToHeader(Loyalty::GroupNo.Label, FALSE);
	AddFieldToHeader(Loyalty::FullName.Label, FALSE);
	AddFieldToHeader(Loyalty::Forename.Label, FALSE);
	AddFieldToHeader(Loyalty::Surname.Label, FALSE);
	AddFieldToHeader(Loyalty::Dob.Label, FALSE);
	AddFieldToHeader(Loyalty::Gender.Label, FALSE);
	AddFieldToHeader(Loyalty::Points.Label, FALSE);
	AddFieldToHeader(Loyalty::PointsTD.Label, FALSE);
	AddFieldToHeader(Loyalty::Purse1.Label, FALSE);
	AddFieldToHeader(Loyalty::Purse1LastSpendDate.Label, FALSE);
	AddFieldToHeader(Loyalty::Purse1LastSpendTime.Label, FALSE);
	AddFieldToHeader(Loyalty::Purse1LastSpend.Label, FALSE);
	AddFieldToHeader(Loyalty::Purse1SpendTD.Label, FALSE);
	AddFieldToHeader(Loyalty::Purse2.Label, TRUE);
	AddFieldToHeader(Loyalty::Purse2LastSpendDate.Label, TRUE);
	AddFieldToHeader(Loyalty::Purse2LastSpendTime.Label, TRUE);
	AddFieldToHeader(Loyalty::Purse2LastSpend.Label, TRUE);
	AddFieldToHeader(Loyalty::Purse2SpendTD.Label, TRUE);
	AddFieldToHeader(Loyalty::Purse2RefreshedDate.Label, TRUE);
	AddFieldToHeader(Loyalty::Purse2RefreshedTime.Label, TRUE);
	AddFieldToHeader(Loyalty::CashLastSpendDate.Label, FALSE);
	AddFieldToHeader(Loyalty::CashLastSpendTime.Label, FALSE);
	AddFieldToHeader(Loyalty::CashLastSpend.Label, FALSE);
	AddFieldToHeader(Loyalty::CashSpendTD.Label, FALSE);
	AddFieldToHeader(Loyalty::MaxSpend.Label, FALSE);
	AddFieldToHeader(Loyalty::OverSpend.Label, FALSE);
	AddFieldToHeader(Loyalty::ExpiryDate.Label, FALSE);
	AddFieldToHeader(Loyalty::AlertCodes.Label, FALSE);
	AddFieldToHeader(Loyalty::Inactive.Label, FALSE);
	AddFieldToHeader(Loyalty::HotlistCode.Label, FALSE);
	AddFieldToHeader(Loyalty::HotlistDate.Label, FALSE);
	AddFieldToHeader(Loyalty::HotlistTime.Label, FALSE);
	AddFieldToHeader(Loyalty::ExPreviousCardNo.Label, FALSE);
	AddFieldToHeader(Loyalty::ExNextCardNo.Label, FALSE);
	AddFieldToHeader(Loyalty::Add1.Label, FALSE);
	AddFieldToHeader(Loyalty::Add2.Label, FALSE);
	AddFieldToHeader(Loyalty::Add3.Label, FALSE);
	AddFieldToHeader(Loyalty::Add4.Label, FALSE);
	AddFieldToHeader(Loyalty::Add5.Label, FALSE);
	AddFieldToHeader(Loyalty::Tel1.Label, FALSE);
	AddFieldToHeader(Loyalty::Tel2.Label, FALSE);
	AddFieldToHeader(Loyalty::ExtRef.Label, FALSE);
	AddFieldToHeader(Loyalty::Info1.Label, FALSE);
	AddFieldToHeader(Loyalty::Info2.Label, FALSE);
	AddFieldToHeader(Loyalty::Info3.Label, FALSE);
	AddFieldToHeader(Loyalty::Info4.Label, FALSE);
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

void CLoyaltyHeaderHelpers::AddLegacyField(CString strLegacyField, CString strNewField, CString& strCSV)
{
	CLoyaltyLegacyField field;
	field.m_strLegacyField = strLegacyField;
	field.m_strNewField = strNewField;
	m_arrayLegacyFields.Add(field);

	if (strCSV != "")
	{
		strCSV += ",";
	}

	strCSV += strLegacyField;
	strCSV += ",";
	strCSV += strNewField;
}

//$$********************************************************************

void CLoyaltyHeaderHelpers::UpdateLegacyField(CString& strField)
{
	if (m_arrayLegacyFields.GetSize() == 0)
	{
		CCSV csv(GetLegacyHeaders());

		for (int n = 0; n < csv.GetSize() - 1; n += 2)
		{
			CLoyaltyLegacyField field;
			field.m_strLegacyField = csv.GetString(n);
			field.m_strNewField = csv.GetString(n + 1);
			m_arrayLegacyFields.Add(field);
		}
	}

	for (int n = 0; n < m_arrayLegacyFields.GetSize(); n++)
	{
		if (strField.CompareNoCase(m_arrayLegacyFields[n].m_strLegacyField) == 0)
		{
			strField = m_arrayLegacyFields[n].m_strNewField;
			break;
		}
	}
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
		AddFieldToHeaderMap(Loyalty::ExCardNo.Label, LOYALTYFIELD_CARDNO);
		AddFieldToHeaderMap(Loyalty::GroupNo.Label, LOYALTYFIELD_GROUPNO);
		AddFieldToHeaderMap(Loyalty::FullName.Label, LOYALTYFIELD_FULLNAME);
		AddFieldToHeaderMap(Loyalty::Forename.Label, LOYALTYFIELD_FORENAME);
		AddFieldToHeaderMap(Loyalty::Surname.Label, LOYALTYFIELD_SURNAME);
		AddFieldToHeaderMap(Loyalty::Dob.Label, LOYALTYFIELD_DOB);
		AddFieldToHeaderMap(Loyalty::Gender.Label, LOYALTYFIELD_GENDER);
		AddFieldToHeaderMap(Loyalty::Points.Label, LOYALTYFIELD_POINTS);
		AddFieldToHeaderMap(Loyalty::PointsTD.Label, LOYALTYFIELD_POINTSTD);
		AddFieldToHeaderMap(Loyalty::Purse1.Label, LOYALTYFIELD_PURSE1);
		AddFieldToHeaderMap(Loyalty::Purse1LastSpendDate.Label, LOYALTYFIELD_PURSE1_LSDATE);
		AddFieldToHeaderMap(Loyalty::Purse1LastSpendTime.Label, LOYALTYFIELD_PURSE1_LSTIME);
		AddFieldToHeaderMap(Loyalty::Purse1LastSpend.Label, LOYALTYFIELD_PURSE1_LS);
		AddFieldToHeaderMap(Loyalty::Purse1SpendTD.Label, LOYALTYFIELD_PURSE1_STD);
		AddFieldToHeaderMap(Loyalty::Purse2.Label, LOYALTYFIELD_PURSE2);
		AddFieldToHeaderMap(Loyalty::Purse2LastSpendDate.Label, LOYALTYFIELD_PURSE2_LSDATE);
		AddFieldToHeaderMap(Loyalty::Purse2LastSpendTime.Label, LOYALTYFIELD_PURSE2_LSTIME);
		AddFieldToHeaderMap(Loyalty::Purse2LastSpend.Label, LOYALTYFIELD_PURSE2_LS);
		AddFieldToHeaderMap(Loyalty::Purse2SpendTD.Label, LOYALTYFIELD_PURSE2_STD);
		AddFieldToHeaderMap(Loyalty::Purse2RefreshedDate.Label, LOYALTYFIELD_PURSE2_RFDATE);
		AddFieldToHeaderMap(Loyalty::Purse2RefreshedTime.Label, LOYALTYFIELD_PURSE2_RFTIME);
		AddFieldToHeaderMap(Loyalty::CashLastSpendDate.Label, LOYALTYFIELD_CASH_LSDATE);
		AddFieldToHeaderMap(Loyalty::CashLastSpendTime.Label, LOYALTYFIELD_CASH_LSTIME);
		AddFieldToHeaderMap(Loyalty::CashLastSpend.Label, LOYALTYFIELD_CASH_LS);
		AddFieldToHeaderMap(Loyalty::CashSpendTD.Label, LOYALTYFIELD_CASH_STD);
		AddFieldToHeaderMap(Loyalty::MaxSpend.Label, LOYALTYFIELD_MAXSPEND);
		AddFieldToHeaderMap(Loyalty::OverSpend.Label, LOYALTYFIELD_OVERSPEND);
		AddFieldToHeaderMap(Loyalty::ExpiryDate.Label, LOYALTYFIELD_EXPIRYDATE);
		AddFieldToHeaderMap(Loyalty::AlertCodes.Label, LOYALTYFIELD_ALERT);
		AddFieldToHeaderMap(Loyalty::Inactive.Label, LOYALTYFIELD_INACTIVE);
		AddFieldToHeaderMap(Loyalty::HotlistCode.Label, LOYALTYFIELD_HOTCODE);
		AddFieldToHeaderMap(Loyalty::HotlistDate.Label, LOYALTYFIELD_HOTDATE);
		AddFieldToHeaderMap(Loyalty::HotlistTime.Label, LOYALTYFIELD_HOTTIME);
		AddFieldToHeaderMap(Loyalty::ExPreviousCardNo.Label, LOYALTYFIELD_PREVCARD);
		AddFieldToHeaderMap(Loyalty::ExNextCardNo.Label, LOYALTYFIELD_NEXTCARD);
		AddFieldToHeaderMap(Loyalty::Add1.Label, LOYALTYFIELD_ADDRESS1);
		AddFieldToHeaderMap(Loyalty::Add2.Label, LOYALTYFIELD_ADDRESS2);
		AddFieldToHeaderMap(Loyalty::Add3.Label, LOYALTYFIELD_ADDRESS3);
		AddFieldToHeaderMap(Loyalty::Add4.Label, LOYALTYFIELD_ADDRESS4);
		AddFieldToHeaderMap(Loyalty::Add5.Label, LOYALTYFIELD_ADDRESS5);
		AddFieldToHeaderMap(Loyalty::Tel1.Label, LOYALTYFIELD_TEL1);
		AddFieldToHeaderMap(Loyalty::Tel2.Label, LOYALTYFIELD_TEL2);
		AddFieldToHeaderMap(Loyalty::ExtRef.Label, LOYALTYFIELD_EXTREF);
		AddFieldToHeaderMap(Loyalty::Info1.Label, LOYALTYFIELD_INFO1);
		AddFieldToHeaderMap(Loyalty::Info2.Label, LOYALTYFIELD_INFO2);
		AddFieldToHeaderMap(Loyalty::Info3.Label, LOYALTYFIELD_INFO3);
		AddFieldToHeaderMap(Loyalty::Info4.Label, LOYALTYFIELD_INFO4);
		/*****/
		AddFieldToHeaderMap(Loyalty::Blank.Label, LOYALTYFIELD_BLANK);
		AddFieldToHeaderMap(Loyalty::TotalSpendTD.Label, LOYALTYFIELD_TOTAL_STD);
		AddFieldToHeaderMap(Loyalty::DobNextAge.Label, LOYALTYFIELD_DOB_NEXTAGE);
		AddFieldToHeaderMap(Loyalty::DobDay.Label, LOYALTYFIELD_DOB_DAY);
		AddFieldToHeaderMap(Loyalty::DobCheck.Label, LOYALTYFIELD_DOB_CHECK);
		AddFieldToHeaderMap(Loyalty::DeletedDate.Label, LOYALTYFIELD_DELDATE);
		AddFieldToHeaderMap(Loyalty::DeletedTime.Label, LOYALTYFIELD_DELTIME);
		AddFieldToHeaderMap(Loyalty::Text.Label, LOYALTYFIELD_TEXT);
		AddFieldToHeaderMap(Loyalty::CardNameWithBalance.Label, LOYALTYFIELD_NAME_BALANCE);
		AddFieldToHeaderMap(Loyalty::Balance.Label, LOYALTYFIELD_BALANCE);
		AddFieldToHeaderMap(Loyalty::BonusCheck.Label, LOYALTYFIELD_BONUS_CHECK);
		AddFieldToHeaderMap(Loyalty::SkipCheck.Label, LOYALTYFIELD_SKIP_CHECK);
		AddFieldToHeaderMap(Loyalty::AlertCheck.Label, LOYALTYFIELD_ALERT_CHECK);
		AddFieldToHeaderMap(Loyalty::FullCardNo.Label, LOYALTYFIELD_FULLCARDNO);
		AddFieldToHeaderMap(Loyalty::GroupCardNo.Label, LOYALTYFIELD_GROUPCARDNO);
		AddFieldToHeaderMap(Loyalty::GroupFullCardNo.Label, LOYALTYFIELD_GROUPFULLCARDNO);
		
		CString str = Loyalty::Purse1.Label;
		str += "TopUp";
		AddFieldToHeaderMap(str, LOYALTYFIELD_PURSE1_TOPUP);
	}

	for (int n = 0; n < csvHeader.GetSize(); n++)
	{
		CString strField = csvHeader.GetString(n);
		UpdateLegacyField(strField);
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
