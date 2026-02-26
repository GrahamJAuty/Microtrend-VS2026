//$$******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//$$******************************************************************
#include "UserTexts.h"
//$$******************************************************************

void CUserTexts::GetFieldInfo(int nField, CString& strField, CString& strFallback)
{
	strField = "";
	strFallback = "";

	switch (nField)
	{
	case LOYALTYFIELD_FULLNAME:
		strField = Account::Fullname.Label;
		strFallback = "User name";
		break;

	case LOYALTYFIELD_SURNAME:
		strField = Account::Surname.Label;
		strFallback = "Surname";
		break;

	case LOYALTYFIELD_FORENAME:
		strField = Account::Forename.Label;
		strFallback = "Forename";
		break;

	case LOYALTYFIELD_ADDRESS1:
		strField = Account::Address1.Label;
		strFallback = "Address1";
		break;

	case LOYALTYFIELD_ADDRESS2:
		strField = Account::Address2.Label;
		break;

	case LOYALTYFIELD_ADDRESS3:
		strField = Account::Address3.Label;
		break;

	case LOYALTYFIELD_ADDRESS4:
		strField = Account::Address4.Label;
		break;

	case LOYALTYFIELD_ADDRESS5:
		strField = Account::Address5.Label;
		strFallback = "Postcode";
		break;

	case LOYALTYFIELD_PHONE1:
		strField = Account::Phone1.Label;
		strFallback = "Telephone";
		break;

	case LOYALTYFIELD_PHONE2:
		strField = Account::Phone2.Label;
		break;

	case LOYALTYFIELD_INFO1:
		strField = Account::Info1.Label;
		strFallback = "Info";
		break;

	case LOYALTYFIELD_INFO2:
		strField = Account::Info2.Label;
		break;

	case LOYALTYFIELD_INFO3:
		strField = Account::Info3.Label;
		break;

	case LOYALTYFIELD_INFO4:
		strField = Account::Info4.Label;
		break;

	case LOYALTYFIELD_DOB:
		strField = Account::DOB.Label;
		strFallback = "Anniversary";
		break;

	case LOYALTYFIELD_EXTERNALREF:
		strField = Account::ExternalRef.Label;
		strFallback = "External Reference";
		break;
	}
}

//$$******************************************************************

void CUserTexts::GetFieldData( int nField, CString& strField, CString& strData, CString strDefault)
{
	strField = "";
	strData = "";

	CString strFallback = "";
	GetFieldInfo(nField, strField, strFallback);

	if (strField != "")
	{
		if (strDefault == "")
		{
			strDefault = strFallback;
		}

		strData = m_file.GetString(strField, strDefault);
	}
}

//$$******************************************************************

CString CUserTexts::GetText(int nField, CString strDefault)
{
	CString strField = "";
	CString strData = "";
	GetFieldData(nField, strField, strData, strDefault);

	if (strData != "")
	{
		CCSV csv(strData);
		strData = csv.GetString(0);

		if (strData == "")
		{
			strData = strDefault;
		}
	}

	return strData;
}

//$$******************************************************************

void CUserTexts::SetText(int nField, CString strText)
{
	CString strField = "";
	CString strData = "";
	GetFieldData(nField, strField, strData, "");
	
	if (strField != "")
	{
		CCSV csv(strData);
		csv.SetAt(0, strText);
		m_file.Set(strField, csv.GetLine());
	}
}

//$$******************************************************************

bool CUserTexts::GetCompulsoryFlag(int nField)
{
	CString strField = "";
	CString strData = "";
	GetFieldData(nField, strField, strData, "");
	CCSV csv(strData);
	return csv.GetBool(1);
}

//$$******************************************************************

void CUserTexts::SetCompulsoryFlag(int nField, bool b)
{
	CString strField = "";
	CString strData = "";
	GetFieldData(nField, strField, strData, "");
	
	if (strField != "")
	{
		CCSV csv(strData);
		csv.SetAt(1, b);
		m_file.Set(strField, csv.GetLine());
	}
}

//$$******************************************************************

bool CUserTexts::GetDateFlag(int nField)
{
	CString strField = "";
	CString strData = "";
	GetFieldData(nField, strField, strData, "");
	CCSV csv(strData);
	return csv.GetBool(2);
}

//$$******************************************************************

void CUserTexts::SetDateFlag(int nField, bool b)
{
	CString strField = "";
	CString strData = "";
	GetFieldData(nField, strField, strData, "");

	if (strField != "")
	{
		CCSV csv(strData);
		csv.SetAt(2, b);
		m_file.Set(strField, csv.GetLine());
	}
}

//$$******************************************************************

