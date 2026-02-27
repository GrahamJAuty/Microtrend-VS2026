//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryPurchaseControlRule.h"
//**********************************************************************

CProjectSQLRepositoryPurchaseControlRule::CProjectSQLRepositoryPurchaseControlRule() : CSQLRepositoryPurchaseControlRule()
{
	m_strImportTableName = SQLTableNames::PurchaseControlRules;
}

//**********************************************************************

bool CProjectSQLRepositoryPurchaseControlRule::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Purchase Control Rule Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Purchase Control Rule Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(PurchaseControlRule::RuleID.Label);
	arrayHeader.Add(PurchaseControlRule::Enabled.Label);
	arrayHeader.Add(PurchaseControlRule::Threshold.Label);
	arrayHeader.Add(PurchaseControlRule::Expiry.Label);
	arrayHeader.Add(PurchaseControlRule::Description.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		int nRuleID = csvIn.GetInt(0);
		if ((nRuleID < PurchaseControlRule::RuleID.Min) || (nRuleID > PurchaseControlRule::RuleID.Max))
		{
			continue;
		}

		CSQLRowPurchaseControlRule RowRule;
		RowRule.SetRuleID(nRuleID);
		RowRule.SetEnableFlag(csvIn.GetBool(1));
		RowRule.SetThreshold(csvIn.GetInt(2));
		RowRule.SetExpiryDays(csvIn.GetInt(3));
		RowRule.SetDescription(csvIn.GetString(4));

		if (InsertRow(RowRule, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
