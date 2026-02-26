//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryDeptInfo.h"
//**********************************************************************

CProjectSQLRepositoryDeptInfo::CProjectSQLRepositoryDeptInfo() : CSQLRepositoryDeptInfo()
{
	m_strImportTableName = SQLTableNames::DeptInfo;
}

//**********************************************************************

bool CProjectSQLRepositoryDeptInfo::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Department Info Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Department Info Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(DeptInfo::DbNo.Label);
	arrayHeader.Add(DeptInfo::DeptNo.Label);
	arrayHeader.Add(DeptInfo::Name.Label);
	arrayHeader.Add(DeptInfo::LoyaltyBonusTrip.Label);
	arrayHeader.Add(DeptInfo::LoyaltyBonusType.Label);
	arrayHeader.Add(DeptInfo::LoyaltyBonusPoints.Label);
	arrayHeader.Add(DeptInfo::LoyaltyBonusValue.Label);
	arrayHeader.Add(DeptInfo::LoyaltyBonusPercent.Label);
	arrayHeader.Add(DeptInfo::GeneralSpendBonusFlag.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowDeptInfo RowDeptInfo;
		RowDeptInfo.SetDbNo(csvIn.GetInt(0));
		RowDeptInfo.SetDeptNo(csvIn.GetInt(1));
		RowDeptInfo.SetName(csvIn.GetString(2));
		RowDeptInfo.SetLoyaltyBonusTrip(csvIn.GetDouble(3));
		RowDeptInfo.SetLoyaltyBonusType(csvIn.GetInt(4));
		RowDeptInfo.SetLoyaltyBonusPoints(csvIn.GetInt(5));
		RowDeptInfo.SetLoyaltyBonusValue(csvIn.GetDouble(6));
		RowDeptInfo.SetLoyaltyBonusPercent(csvIn.GetInt(7));
		RowDeptInfo.SetGeneralSpendBonusFlag(csvIn.GetBool(8));

		if (InsertRow(RowDeptInfo, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
