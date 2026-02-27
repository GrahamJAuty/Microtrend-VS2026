//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryPurchaseControlStamp.h"
//**********************************************************************

CProjectSQLRepositoryPurchaseControlStamp::CProjectSQLRepositoryPurchaseControlStamp() : CSQLRepositoryPurchaseControlStamp()
{
	m_strImportTableName = SQLTableNames::PurchaseControlStamps;
}

//**********************************************************************

bool CProjectSQLRepositoryPurchaseControlStamp::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Purchase Control Stamp Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Purchase Control Stamp Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(PurchaseControlStamp::UserID.Label);
	arrayHeader.Add(PurchaseControlStamp::RuleID.Label);
	arrayHeader.Add(PurchaseControlStamp::AwardDate.Label);
	arrayHeader.Add(PurchaseControlStamp::ExpireDate.Label);
	arrayHeader.Add(PurchaseControlStamp::StampCount.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		int nRuleID = csvIn.GetInt(1);
		if ((nRuleID < PurchaseControlStamp::RuleID.Min) || (nRuleID > PurchaseControlStamp::RuleID.Max))
		{
			continue;
		}

		CSQLRowPurchaseControlStamp RowStamp;
		RowStamp.SetUserID(csvIn.GetInt64(0));
		RowStamp.SetRuleID(nRuleID);
		RowStamp.SetAwardDate(csvIn.GetString(2));
		RowStamp.SetExpireDate(csvIn.GetString(3));
		RowStamp.SetStampCount(csvIn.GetInt(4));

		if (InsertRow(RowStamp, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
