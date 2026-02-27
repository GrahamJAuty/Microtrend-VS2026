//**********************************************************************
#include "..\\ServerData.h"
//**********************************************************************
#include "SQLRowPebbleConfig.h"
//**********************************************************************

CSQLRowPebbleConfig::CSQLRowPebbleConfig()
{
	Reset();
}

//**********************************************************************

void CSQLRowPebbleConfig::Reset()
{
	m_nLineID = 1;
	m_strProductionInstallationID = "";
	m_strProductionOrganisationID = "";
	m_nMISInfoIndex = 8;
	m_nUPNInfoIndex = 7;
	m_strLastImportDateTopup = "20240101";
	m_strLastImportTimeTopup = "000000";
	m_strLastImportDateSale = "20240101";
	m_strLastImportTimeSale = "000000";
	m_bFirstImportTopup = TRUE;
	m_bFirstImportSale = TRUE;
	m_nRecentCacheDays = 0;
	m_nPaymentCacheDays = 0;

	for (int i = 0; i < 12; i++)
	{
		m_VATInfo[i].m_nVATCode = 0;
		m_VATInfo[i].m_dVATRate = 0.0;
	}

	m_nOfficeMealPluNo = 9999;
	m_nOfficeMealVATBand = 1;
}

//**********************************************************************

CString CSQLRowPebbleConfig::GetActiveInstallationID()
{
	if (Server.GetPebbleDemoFlag() == TRUE)
	{
		return "01J8HW0ETA6NGYNKZAF4C9P1DZ";
	}
	else
	{
		return m_strProductionInstallationID;
	}
}

//**********************************************************************

CString CSQLRowPebbleConfig::GetActiveOrganisationID()
{
	if (Server.GetPebbleDemoFlag() == TRUE)
	{
		return "01J8HXJM0PGHCFHK0S1BS47D8A";
	}
	else
	{
		return m_strProductionOrganisationID;
	}
}

//**********************************************************************

