//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryClosingBalance.h"
//**********************************************************************

CProjectSQLRepositoryClosingBalance::CProjectSQLRepositoryClosingBalance() : CSQLRepositoryClosingBalance()
{
	m_strImportTableName = SQLTableNames::ClosingBalances;
}

//**********************************************************************

bool CProjectSQLRepositoryClosingBalance::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Closing Balances Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Closing Balances Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(ClosingBalance::KeyDate.Label);
	arrayHeader.Add(ClosingBalance::Date.Label);
	arrayHeader.Add(ClosingBalance::Time.Label);
	arrayHeader.Add(ClosingBalance::ArchiveDate.Label);
	arrayHeader.Add(ClosingBalance::ArchiveTime.Label);
	arrayHeader.Add(ClosingBalance::Purse1LiabilityAudit.Label);
	arrayHeader.Add(ClosingBalance::Purse2BalanceAudit.Label);
	arrayHeader.Add(ClosingBalance::Purse3LiabilityAudit.Label);
	arrayHeader.Add(ClosingBalance::Purse1CreditAudit.Label);
	arrayHeader.Add(ClosingBalance::Purse3CreditAudit.Label);
	arrayHeader.Add(ClosingBalance::HaveAdjust.Label);
	arrayHeader.Add(ClosingBalance::Purse1LiabilityAdjust.Label);
	arrayHeader.Add(ClosingBalance::Purse3LiabilityAdjust.Label);
	arrayHeader.Add(ClosingBalance::Purse1CreditAdjust.Label);
	arrayHeader.Add(ClosingBalance::Purse3CreditAdjust.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowClosingBalance RowClosing;
		RowClosing.SetKeyDate(csvIn.GetString(0));
		RowClosing.SetDate(csvIn.GetString(1));
		RowClosing.SetTime(csvIn.GetString(2));
		RowClosing.SetArchiveDate(csvIn.GetString(3));
		RowClosing.SetArchiveTime(csvIn.GetString(4));
		RowClosing.SetPurse1LiabilityAudit(csvIn.GetDouble(5));
		RowClosing.SetPurse2BalanceAudit(csvIn.GetDouble(6));
		RowClosing.SetPurse3LiabilityAudit(csvIn.GetDouble(7));
		RowClosing.SetPurse1CreditAudit(csvIn.GetDouble(8));
		RowClosing.SetPurse3CreditAudit(csvIn.GetDouble(9));
		RowClosing.SetHaveAdjustFlag(csvIn.GetDouble(10));
		RowClosing.SetPurse1LiabilityAdjust(csvIn.GetDouble(11));
		RowClosing.SetPurse3LiabilityAdjust(csvIn.GetDouble(12));
		RowClosing.SetPurse1CreditAdjust(csvIn.GetDouble(13));
		RowClosing.SetPurse3CreditAdjust(csvIn.GetDouble(14));

		if (InsertRow(RowClosing, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
