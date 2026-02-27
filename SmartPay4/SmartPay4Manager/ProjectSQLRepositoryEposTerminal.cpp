//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryEposTerminal.h"
//**********************************************************************

CProjectSQLRepositoryEposTerminal::CProjectSQLRepositoryEposTerminal() : CSQLRepositoryEposTerminal()
{
	m_strImportTableName = SQLTableNames::EposTerminals;
}

//**********************************************************************

bool CProjectSQLRepositoryEposTerminal::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, int nDbVersion, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Epos Terminal Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}

	if (nDbVersion >= 12)
	{	
		WorkingDlg.SetCaption1("Restoring Epos Terminal Table");

		fileIn.Seek(0, CFile::begin);

		CStringArray arrayHeader;
		arrayHeader.Add(EposTerminal::TerminalNo.Label);
		arrayHeader.Add(EposTerminal::TerminalType.Label);
		arrayHeader.Add(EposTerminal::TerminalName.Label);
		arrayHeader.Add(EposTerminal::LastUsedDate.Label);
		arrayHeader.Add(EposTerminal::LastUsedTime.Label);
		arrayHeader.Add(EposTerminal::CCNo.Label);
		arrayHeader.Add(EposTerminal::UserID.Label);
		arrayHeader.Add(EposTerminal::CardValue.Label);
		arrayHeader.Add(EposTerminal::TotalCounter.Label);
		arrayHeader.Add(EposTerminal::TotalInDrawer.Label);
		arrayHeader.Add(EposTerminal::Disabled.Label);
		arrayHeader.Add(EposTerminal::TerminalOn.Label);
		arrayHeader.Add(EposTerminal::InService.Label);
		arrayHeader.Add(EposTerminal::DoorOpen.Label);

		CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

		CString strBuffer = "";
		while (SQLIn.ReadLine(strBuffer))
		{
			CCSV csvIn(strBuffer);

			CSQLRowEposTerminal RowEP;
			RowEP.SetTerminalNo(csvIn.GetInt(0));
			RowEP.SetTerminalType(csvIn.GetInt(1));
			RowEP.SetTerminalName(csvIn.GetString(2));
			RowEP.SetLastUsedDate(csvIn.GetString(3));
			RowEP.SetLastUsedTime(csvIn.GetString(4));
			RowEP.SetCCNo(csvIn.GetInt(5));
			RowEP.SetUserID(csvIn.GetInt64(6));
			RowEP.SetCardValue(csvIn.GetDouble(7));
			RowEP.SetTotalCounter(csvIn.GetInt(8));
			RowEP.SetTotalInDrawer(csvIn.GetDouble(9));
			RowEP.SetDisableFlag(csvIn.GetBool(10));
			RowEP.SetTerminalOnFlag(csvIn.GetBool(11));
			RowEP.SetInServiceFlag(csvIn.GetBool(12));
			RowEP.SetDoorOpenFlag(csvIn.GetBool(13));

			if (InsertRow(RowEP, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}

			WorkingDlg.IncrementRecordsChecked(10, FALSE);
		}

		return TRUE;
	}
	else
	{
		WorkingDlg.SetCaption1("Upgrading Epos Terminal Data to SQL");
		return (UpgradeLegacyData(pDatabase, TRUE));
	}
}

//**********************************************************************
