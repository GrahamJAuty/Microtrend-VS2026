//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryGroupTopupBonus.h"
//**********************************************************************

CProjectSQLRepositoryGroupTopupBonus::CProjectSQLRepositoryGroupTopupBonus() : CSQLRepositoryGroupTopupBonus()
{
	m_strImportTableName = SQLTableNames::GroupTopupBonuses;
}

//**********************************************************************

bool CProjectSQLRepositoryGroupTopupBonus::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, bool& bGotTopupBonuses, CWorkingDlg& WorkingDlg)
{
	bGotTopupBonuses = FALSE;

	WorkingDlg.SetCaption1("Preparing Group Topup Bonus Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Group Topup Bonus Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(GroupTopupBonus::GroupNo.Label);
	arrayHeader.Add(GroupTopupBonus::PurseNo.Label);
	arrayHeader.Add(GroupTopupBonus::TopupNo.Label);
	arrayHeader.Add(GroupTopupBonus::BonusFlag.Label);
	arrayHeader.Add(GroupTopupBonus::BonusDisable.Label);
	arrayHeader.Add(GroupTopupBonus::BonusType.Label);
	arrayHeader.Add(GroupTopupBonus::BonusTrip.Label);
	arrayHeader.Add(GroupTopupBonus::BonusPoints.Label);
	arrayHeader.Add(GroupTopupBonus::BonusValue.Label);
	arrayHeader.Add(GroupTopupBonus::BonusPercent.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowGroupTopupBonus RowGroup;
		RowGroup.SetGroupNo(csvIn.GetInt(0));
		RowGroup.SetPurseNo(csvIn.GetInt(1));
		RowGroup.SetTopupNo(csvIn.GetInt(2));
		RowGroup.SetBonusFlag(csvIn.GetBool(3));
		RowGroup.SetBonusDisable(csvIn.GetBool(4));
		RowGroup.SetBonusType(csvIn.GetInt(5));
		RowGroup.SetBonusTrip(csvIn.GetDouble(6));
		RowGroup.SetBonusPoints(csvIn.GetInt(7));
		RowGroup.SetBonusValue(csvIn.GetDouble(8));
		RowGroup.SetBonusPercent(csvIn.GetInt(9));

		if (InsertRow(RowGroup, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}
		else
		{
			bGotTopupBonuses = TRUE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
