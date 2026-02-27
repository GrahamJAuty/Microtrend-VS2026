//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_PebbleConfig\SQLRepositoryPebbleConfig.h"
//**********************************************************************
#include "ProjectSQLRecordSetPebbleConfig.h"
//**********************************************************************

CProjectSQLRecordSetPebbleConfig::CProjectSQLRecordSetPebbleConfig() : CSQLRecordSetPebbleConfig(NULL, RSParams_PebbleConfig_NormalNoParams		{})
{
}

//**********************************************************************

void CProjectSQLRecordSetPebbleConfig::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(PebbleConfig::LineID.Label);
	arrayHeader.Add(PebbleConfig::InstallationID.Label);
	arrayHeader.Add(PebbleConfig::OrganisationID.Label);
	arrayHeader.Add(PebbleConfig::MISInfoIndex.Label);
	arrayHeader.Add(PebbleConfig::UPNInfoIndex.Label);
	arrayHeader.Add(PebbleConfig::LastImportDateTopup.Label);
	arrayHeader.Add(PebbleConfig::LastImportTimeTopup.Label);
	arrayHeader.Add(PebbleConfig::LastImportDateSale.Label);
	arrayHeader.Add(PebbleConfig::LastImportTimeSale.Label);
	arrayHeader.Add(PebbleConfig::FirstImportTopup.Label);
	arrayHeader.Add(PebbleConfig::FirstImportSale.Label);
	arrayHeader.Add(PebbleConfig::RecentCacheDays.Label);
	arrayHeader.Add(PebbleConfig::PaymentCacheDays.Label);
	arrayHeader.Add(PebbleConfig::VAT_A.Label);
	arrayHeader.Add(PebbleConfig::VAT_B.Label);
	arrayHeader.Add(PebbleConfig::VAT_C.Label);
	arrayHeader.Add(PebbleConfig::VAT_D.Label);
	arrayHeader.Add(PebbleConfig::VAT_E.Label);
	arrayHeader.Add(PebbleConfig::VAT_F.Label);
	arrayHeader.Add(PebbleConfig::VAT_G.Label);
	arrayHeader.Add(PebbleConfig::VAT_H.Label);
	arrayHeader.Add(PebbleConfig::VAT_I.Label);
	arrayHeader.Add(PebbleConfig::VAT_J.Label);
	arrayHeader.Add(PebbleConfig::VAT_K.Label);
	arrayHeader.Add(PebbleConfig::VAT_L.Label);
	arrayHeader.Add(PebbleConfig::VATCODE_A.Label);
	arrayHeader.Add(PebbleConfig::VATCODE_B.Label);
	arrayHeader.Add(PebbleConfig::VATCODE_C.Label);
	arrayHeader.Add(PebbleConfig::VATCODE_D.Label);
	arrayHeader.Add(PebbleConfig::VATCODE_E.Label);
	arrayHeader.Add(PebbleConfig::VATCODE_F.Label);
	arrayHeader.Add(PebbleConfig::VATCODE_G.Label);
	arrayHeader.Add(PebbleConfig::VATCODE_H.Label);
	arrayHeader.Add(PebbleConfig::VATCODE_I.Label);
	arrayHeader.Add(PebbleConfig::VATCODE_J.Label);
	arrayHeader.Add(PebbleConfig::VATCODE_K.Label);
	arrayHeader.Add(PebbleConfig::VATCODE_L.Label);
	arrayHeader.Add(PebbleConfig::OfficeMealPluNo.Label);
	arrayHeader.Add(PebbleConfig::OfficeMealVATBand.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::PebbleConfig, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRepositoryPebbleConfig PebbleConfig;
	PebbleConfig.LoadRow(NULL);

	SQLOut.Add(PebbleConfig.m_Row.GetLineID());
	SQLOut.Add(PebbleConfig.m_Row.GetProductionInstallationID());
	SQLOut.Add(PebbleConfig.m_Row.GetProductionOrganisationID());
	SQLOut.Add(PebbleConfig.m_Row.GetMISInfoIndex());
	SQLOut.Add(PebbleConfig.m_Row.GetUPNInfoIndex());
	SQLOut.Add(PebbleConfig.m_Row.GetLastImportDateTopup());
	SQLOut.Add(PebbleConfig.m_Row.GetLastImportTimeTopup());
	SQLOut.Add(PebbleConfig.m_Row.GetLastImportDateSale());
	SQLOut.Add(PebbleConfig.m_Row.GetLastImportTimeSale());
	SQLOut.Add(PebbleConfig.m_Row.GetFirstImportTopupFlag());
	SQLOut.Add(PebbleConfig.m_Row.GetFirstImportSaleFlag());
	SQLOut.Add(PebbleConfig.m_Row.GetRecentCacheDays());
	SQLOut.Add(PebbleConfig.m_Row.GetPaymentCacheDays());
	SQLOut.Add(PebbleConfig.m_Row.GetVAT_A(), 4);
	SQLOut.Add(PebbleConfig.m_Row.GetVAT_B(), 4);
	SQLOut.Add(PebbleConfig.m_Row.GetVAT_C(), 4);
	SQLOut.Add(PebbleConfig.m_Row.GetVAT_D(), 4);
	SQLOut.Add(PebbleConfig.m_Row.GetVAT_E(), 4);
	SQLOut.Add(PebbleConfig.m_Row.GetVAT_F(), 4);
	SQLOut.Add(PebbleConfig.m_Row.GetVAT_G(), 4);
	SQLOut.Add(PebbleConfig.m_Row.GetVAT_H(), 4);
	SQLOut.Add(PebbleConfig.m_Row.GetVAT_I(), 4);
	SQLOut.Add(PebbleConfig.m_Row.GetVAT_J(), 4);
	SQLOut.Add(PebbleConfig.m_Row.GetVAT_K(), 4);
	SQLOut.Add(PebbleConfig.m_Row.GetVAT_L(), 4);
	SQLOut.Add(PebbleConfig.m_Row.GetVATCode_A());
	SQLOut.Add(PebbleConfig.m_Row.GetVATCode_B());
	SQLOut.Add(PebbleConfig.m_Row.GetVATCode_C());
	SQLOut.Add(PebbleConfig.m_Row.GetVATCode_D());
	SQLOut.Add(PebbleConfig.m_Row.GetVATCode_E());
	SQLOut.Add(PebbleConfig.m_Row.GetVATCode_F());
	SQLOut.Add(PebbleConfig.m_Row.GetVATCode_G());
	SQLOut.Add(PebbleConfig.m_Row.GetVATCode_H());
	SQLOut.Add(PebbleConfig.m_Row.GetVATCode_I());
	SQLOut.Add(PebbleConfig.m_Row.GetVATCode_J());
	SQLOut.Add(PebbleConfig.m_Row.GetVATCode_K());
	SQLOut.Add(PebbleConfig.m_Row.GetVATCode_L());
	SQLOut.Add(PebbleConfig.m_Row.GetOfficeMealPluNo());
	SQLOut.Add(PebbleConfig.m_Row.GetOfficeMealVATBand());

	SQLOut.WriteInsert();
}

//**********************************************************************
