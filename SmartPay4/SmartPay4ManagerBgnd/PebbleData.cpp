//**********************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "PebbleData.h"
//**********************************************************************

CPebbleData::CPebbleData()
{
	m_strInstallationID = "01J3316HJ7H5YCMMN6MMVBEGWM";
	m_strOrganisationID = "01J3314FGKSDG9DR17WJQNAY5Q";
	SetUPNInfoIndex(7);
	SetMISInfoIndex(8);

	CString strPath = System.GetWebPaymentPath("", nWEBPAYMENT_PEBBLE);	// creates folder if not found
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	m_strFilename = System.GetWebPaymentPath("Pebble.dat", nWEBPAYMENT_PEBBLE);
}

//**********************************************************************

void CPebbleData::Read()
{
	CSSIniFile file(SS_INI_AES);
	if (file.Read(m_strFilename) == TRUE)
	{
		m_strInstallationID = file.GetString(PebbleDataFields::InstallationID, m_strInstallationID);
		m_strOrganisationID = file.GetString(PebbleDataFields::OrganisationID, m_strOrganisationID);
		SetMISInfoIndex( file.GetInt(PebbleDataFields::MISInfoIndex, m_nMISInfoIndex));
		SetUPNInfoIndex( file.GetInt(PebbleDataFields::UPNInfoIndex, m_nUPNInfoIndex));
	}
}

//**********************************************************************

void CPebbleData::Save()
{
	CSSIniFile file(SS_INI_AES);

	file.Set(PebbleDataFields::InstallationID, m_strInstallationID);
	file.Set(PebbleDataFields::OrganisationID, m_strOrganisationID);
	file.Set(PebbleDataFields::MISInfoIndex, m_nMISInfoIndex);
	file.Set(PebbleDataFields::UPNInfoIndex, m_nUPNInfoIndex);

	file.Write(m_strFilename);
}

//**********************************************************************

void CPebbleData::SetMISInfoIndex(int n)
{
	if ((n >= 0) && (n <= nMAX_INFOTEXTS))
	{
		m_nMISInfoIndex = n;
	}
}

//**********************************************************************

void CPebbleData::SetUPNInfoIndex(int n)
{
	if ((n >= 0) && (n <= nMAX_INFOTEXTS))
	{
		m_nUPNInfoIndex = n;
	}
}

//**********************************************************************

bool CPebbleData::IsSet()
{
	if (m_strInstallationID == "")
	{
		return FALSE;
	}

	if (m_strOrganisationID == "")
	{
		return FALSE;
	}

	return TRUE;
}

//**********************************************************************

CString CPebbleData::GetLocateByLabelMisID()
{
	return GetTextLabel(m_nMISInfoIndex);
}

//*******************************************************************

CString CPebbleData::GetLocateByLabelUPN()
{
	return GetTextLabel(m_nUPNInfoIndex);
}

//*******************************************************************

CString CPebbleData::GetTextLabel(int n)
{
	CString strResult = "";

	switch (n)
	{
	case 1:
		strResult = Account::Info1.Label;
		break;

	case 2:
		strResult = Account::Info2.Label;
		break;

	case 3:
		strResult = Account::Info3.Label;
		break;

	case 4:
		strResult = Account::Info4.Label;
		break;

	case 5:
		strResult = Account::Info5.Label;
		break;

	case 6:
		strResult = Account::Info6.Label;
		break;

	case 7:
		strResult = Account::Info7.Label;
		break;

	case 8:
		strResult = Account::Info8.Label;
		break;
	}

	return strResult;
}

//*******************************************************************

CString CPebbleData::GetLocateByText(int n)
{
	CString strResult = "";

	switch (n)
	{
	case nPEBBLE_LOCATEBY_UPN:
		strResult = "UPN";
		break;

	case nPEBBLE_LOCATEBY_MIS:
		strResult = "MIS ID";
		break;
	}

	return strResult;
}

//*******************************************************************
