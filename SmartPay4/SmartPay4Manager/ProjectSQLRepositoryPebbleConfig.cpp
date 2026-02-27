//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryPebbleConfig.h"
//**********************************************************************

CProjectSQLRepositoryPebbleConfig::CProjectSQLRepositoryPebbleConfig() : CSQLRepositoryPebbleConfig()
{
	m_strImportTableName = SQLTableNames::PebbleConfig;
}

//**********************************************************************

bool CProjectSQLRepositoryPebbleConfig::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Pebble Config Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Pebble Config Table");

	fileIn.Seek(0, CFile::begin);

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

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		m_Row.SetLineID(csvIn.GetInt(0));
		m_Row.SetProductionInstallationID(csvIn.GetString(1));
		m_Row.SetProductionOrganisationID(csvIn.GetString(2));
		m_Row.SetMISInfoIndex(csvIn.GetInt(3));
		m_Row.SetUPNInfoIndex(csvIn.GetInt(4));
		m_Row.SetLastImportDateTopup(csvIn.GetString(5));
		m_Row.SetLastImportTimeTopup(csvIn.GetString(6));
		m_Row.SetLastImportDateSale(csvIn.GetString(7));
		m_Row.SetLastImportTimeSale(csvIn.GetString(8));
		m_Row.SetFirstImportTopupFlag(csvIn.GetBool(9));
		m_Row.SetFirstImportSaleFlag(csvIn.GetBool(10));
		m_Row.SetRecentCacheDays(csvIn.GetBool(11));
		m_Row.SetPaymentCacheDays(csvIn.GetBool(12));
		m_Row.SetVAT_A(csvIn.GetBool(13));
		m_Row.SetVAT_B(csvIn.GetBool(14));
		m_Row.SetVAT_C(csvIn.GetBool(15));
		m_Row.SetVAT_D(csvIn.GetBool(16));
		m_Row.SetVAT_E(csvIn.GetBool(17));
		m_Row.SetVAT_F(csvIn.GetBool(18));
		m_Row.SetVAT_G(csvIn.GetBool(19));
		m_Row.SetVAT_H(csvIn.GetBool(20));
		m_Row.SetVAT_I(csvIn.GetBool(21));
		m_Row.SetVAT_J(csvIn.GetBool(22));
		m_Row.SetVAT_K(csvIn.GetBool(23));
		m_Row.SetVAT_L(csvIn.GetBool(24));
		m_Row.SetVATCode_A(csvIn.GetBool(25));
		m_Row.SetVATCode_B(csvIn.GetBool(26));
		m_Row.SetVATCode_C(csvIn.GetBool(27));
		m_Row.SetVATCode_D(csvIn.GetBool(28));
		m_Row.SetVATCode_E(csvIn.GetBool(29));
		m_Row.SetVATCode_F(csvIn.GetBool(30));
		m_Row.SetVATCode_G(csvIn.GetBool(31));
		m_Row.SetVATCode_H(csvIn.GetBool(32));
		m_Row.SetVATCode_I(csvIn.GetBool(33));
		m_Row.SetVATCode_J(csvIn.GetBool(34));
		m_Row.SetVATCode_K(csvIn.GetBool(35));
		m_Row.SetVATCode_L(csvIn.GetBool(36));
		m_Row.SetOfficeMealPluNo(csvIn.GetBool(37));
		m_Row.SetOfficeMealVATBand(csvIn.GetBool(38));

		if (SaveRow(pDatabase) != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
