//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPebbleConfig.h"
//**********************************************************************

CSQLRecordSetMetadataPebbleConfig::CSQLRecordSetMetadataPebbleConfig(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
	if (FALSE == bForExistenceCheck)
	{
		AddIntField(&m_nLineID, PebbleConfig::LineID.Label, 1);
		AddTextField(&m_strInstallationID, PebbleConfig::InstallationID.Label);
		AddTextField(&m_strOrganisationID, PebbleConfig::OrganisationID.Label);
		AddIntField(&m_nMISInfoIndex, PebbleConfig::MISInfoIndex.Label);
		AddIntField(&m_nUPNInfoIndex, PebbleConfig::UPNInfoIndex.Label);
		AddTextField(&m_strLastImportDateTopup, PebbleConfig::LastImportDateTopup.Label);
		AddTextField(&m_strLastImportTimeTopup, PebbleConfig::LastImportTimeTopup.Label);
		AddTextField(&m_strLastImportDateSale, PebbleConfig::LastImportDateSale.Label);
		AddTextField(&m_strLastImportTimeSale, PebbleConfig::LastImportTimeSale.Label);
		AddBOOLField(&m_bFirstImportTopup, PebbleConfig::FirstImportTopup.Label, TRUE);
		AddBOOLField(&m_bFirstImportSale, PebbleConfig::FirstImportSale.Label, TRUE);
		AddIntField(&m_nRecentCacheDays, PebbleConfig::RecentCacheDays.Label);
		AddIntField(&m_nPaymentCacheDays, PebbleConfig::PaymentCacheDays.Label);
		AddDecimalField(&m_VATInfo[0].m_strVATRate, PebbleConfig::VAT_A.Label);
		AddDecimalField(&m_VATInfo[1].m_strVATRate, PebbleConfig::VAT_B.Label);
		AddDecimalField(&m_VATInfo[2].m_strVATRate, PebbleConfig::VAT_C.Label);
		AddDecimalField(&m_VATInfo[3].m_strVATRate, PebbleConfig::VAT_D.Label);
		AddDecimalField(&m_VATInfo[4].m_strVATRate, PebbleConfig::VAT_E.Label);
		AddDecimalField(&m_VATInfo[5].m_strVATRate, PebbleConfig::VAT_F.Label);
		AddDecimalField(&m_VATInfo[6].m_strVATRate, PebbleConfig::VAT_G.Label);
		AddDecimalField(&m_VATInfo[7].m_strVATRate, PebbleConfig::VAT_H.Label);
		AddDecimalField(&m_VATInfo[8].m_strVATRate, PebbleConfig::VAT_I.Label);
		AddDecimalField(&m_VATInfo[9].m_strVATRate, PebbleConfig::VAT_J.Label);
		AddDecimalField(&m_VATInfo[10].m_strVATRate, PebbleConfig::VAT_K.Label);
		AddDecimalField(&m_VATInfo[11].m_strVATRate, PebbleConfig::VAT_L.Label);
		AddIntField(&m_VATInfo[0].m_nVATCode, PebbleConfig::VATCODE_A.Label);
		AddIntField(&m_VATInfo[1].m_nVATCode, PebbleConfig::VATCODE_B.Label);
		AddIntField(&m_VATInfo[2].m_nVATCode, PebbleConfig::VATCODE_C.Label);
		AddIntField(&m_VATInfo[3].m_nVATCode, PebbleConfig::VATCODE_D.Label);
		AddIntField(&m_VATInfo[4].m_nVATCode, PebbleConfig::VATCODE_E.Label);
		AddIntField(&m_VATInfo[5].m_nVATCode, PebbleConfig::VATCODE_F.Label);
		AddIntField(&m_VATInfo[6].m_nVATCode, PebbleConfig::VATCODE_G.Label);
		AddIntField(&m_VATInfo[7].m_nVATCode, PebbleConfig::VATCODE_H.Label);
		AddIntField(&m_VATInfo[8].m_nVATCode, PebbleConfig::VATCODE_I.Label);
		AddIntField(&m_VATInfo[9].m_nVATCode, PebbleConfig::VATCODE_J.Label);
		AddIntField(&m_VATInfo[10].m_nVATCode, PebbleConfig::VATCODE_K.Label);
		AddIntField(&m_VATInfo[11].m_nVATCode, PebbleConfig::VATCODE_L.Label);
		AddInt64Field(&m_nOfficeMealPluNo, PebbleConfig::OfficeMealPluNo.Label, 1);
		AddIntField(&m_nOfficeMealVATBand, PebbleConfig::OfficeMealVATBand.Label, 1);
	}
}

//**********************************************************************

void CSQLRecordSetMetadataPebbleConfig::AddParamsByLineID(const int nLineID)
{
	AddIntParam(PebbleConfig::LineID.Label, nLineID);
}

//**********************************************************************

CSQLRecordSetPebbleConfig::CSQLRecordSetPebbleConfig(CDatabase* pDatabase, RSParams_PebbleConfig_NormalNoParams params) :
	CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetPebbleConfig::CSQLRecordSetPebbleConfig(CDatabase* pDatabase, RSParams_PebbleConfig_NormalByLineId params, bool bForExistenceCheck) :
	CSQLRecordSetConnectionPool(pDatabase),
	m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByLineID(params.nLineId);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetPebbleConfig::CommonInit()
{
	m_nFields = m_MD.GetFieldCount();
	m_nParams = m_MD.GetParamCount();
	SetMetadata(&m_MD);
	SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPebbleConfig::GetDefaultSQL()
{
    return SQLTableNames::PebbleConfig;
}

//**********************************************************************

void CSQLRecordSetPebbleConfig::LoadFromPebbleConfigRow(CSQLRowPebbleConfig& RowPB)
{
    m_MD.m_nLineID = RowPB.GetLineID();
	m_MD.m_strInstallationID = RowPB.GetProductionInstallationID();
	m_MD.m_strOrganisationID = RowPB.GetProductionOrganisationID();
	m_MD.m_nMISInfoIndex = RowPB.GetMISInfoIndex();
	m_MD.m_nUPNInfoIndex = RowPB.GetUPNInfoIndex();
	m_MD.m_strLastImportDateTopup = RowPB.GetLastImportDateTopup();
	m_MD.m_strLastImportTimeTopup = RowPB.GetLastImportTimeTopup();
	m_MD.m_strLastImportDateSale = RowPB.GetLastImportDateSale();
	m_MD.m_strLastImportTimeSale = RowPB.GetLastImportTimeSale();
	m_MD.m_bFirstImportTopup = RowPB.GetFirstImportTopupFlag();
	m_MD.m_bFirstImportSale = RowPB.GetFirstImportSaleFlag();
	m_MD.m_nRecentCacheDays = RowPB.GetRecentCacheDays();
	m_MD.m_nPaymentCacheDays = RowPB.GetPaymentCacheDays();

	for (int i = 0; i < 12; i++)
	{
		CPebbleVATInfo RowInfo = RowPB.GetVATInfo(i);
		m_MD.m_VATInfo[i].m_nVATCode = RowInfo.m_nVATCode;
		SetDoubleString(m_MD.m_VATInfo[i].m_strVATRate, RowInfo.m_dVATRate);
	}

	m_MD.m_nOfficeMealPluNo = RowPB.GetOfficeMealPluNo();
	m_MD.m_nOfficeMealVATBand = RowPB.GetOfficeMealVATBand();
}

//**********************************************************************

void CSQLRecordSetPebbleConfig::SaveToPebbleConfigRow(CSQLRowPebbleConfig& RowPB)
{
    RowPB.SetLineID(m_MD.m_nLineID);
    RowPB.SetProductionInstallationID(m_MD.m_strInstallationID);
    RowPB.SetProductionOrganisationID(m_MD.m_strOrganisationID);
    RowPB.SetMISInfoIndex(m_MD.m_nMISInfoIndex);
    RowPB.SetUPNInfoIndex(m_MD.m_nUPNInfoIndex);
    RowPB.SetLastImportDateTopup(m_MD.m_strLastImportDateTopup);
    RowPB.SetLastImportTimeTopup(m_MD.m_strLastImportTimeTopup);
    RowPB.SetLastImportDateSale(m_MD.m_strLastImportDateSale);
    RowPB.SetLastImportTimeSale(m_MD.m_strLastImportTimeSale);
    RowPB.SetFirstImportTopupFlag(m_MD.m_bFirstImportTopup);
    RowPB.SetFirstImportSaleFlag(m_MD.m_bFirstImportSale);
    RowPB.SetRecentCacheDays(m_MD.m_nRecentCacheDays);
    RowPB.SetPaymentCacheDays(m_MD.m_nPaymentCacheDays);
    
    for (int i = 0; i < 12; i++)
    {
		CPebbleVATInfo RowInfo; 
		RowInfo.m_nVATCode = m_MD.m_VATInfo[i].m_nVATCode;
		RowInfo.m_dVATRate = atof(m_MD.m_VATInfo[i].m_strVATRate);
        RowPB.SetVATInfo(i, RowInfo);
    }
    
    RowPB.SetOfficeMealPluNo(m_MD.m_nOfficeMealPluNo);
    RowPB.SetOfficeMealVATBand(m_MD.m_nOfficeMealVATBand);
}

//**********************************************************************

