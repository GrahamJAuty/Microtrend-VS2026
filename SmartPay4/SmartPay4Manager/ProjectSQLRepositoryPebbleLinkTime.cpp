//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryPebbleLinkTime.h"
//**********************************************************************

CProjectSQLRepositoryPebbleLinkTime::CProjectSQLRepositoryPebbleLinkTime() : CSQLRepositoryPebbleLinkTime()
{
	m_strImportTableName = SQLTableNames::PebbleLinkTimes;
}

//**********************************************************************

bool CProjectSQLRepositoryPebbleLinkTime::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Pebble Link Time Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Pebble Link Time Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(PebbleLinkTime::LineID.Label);
	arrayHeader.Add(PebbleLinkTime::Date.Label);
	arrayHeader.Add(PebbleLinkTime::Time.Label);
	arrayHeader.Add(PebbleLinkTime::UserID.Label);
	arrayHeader.Add(PebbleLinkTime::Username.Label);
	arrayHeader.Add(PebbleLinkTime::Forename.Label);
	arrayHeader.Add(PebbleLinkTime::Surname.Label);
	arrayHeader.Add(PebbleLinkTime::GroupNo.Label);
	arrayHeader.Add(PebbleLinkTime::PebbleFirstName.Label);
	arrayHeader.Add(PebbleLinkTime::PebbleLastName.Label);
	arrayHeader.Add(PebbleLinkTime::ProcessType.Label);
	arrayHeader.Add(PebbleLinkTime::MatchType.Label);
	arrayHeader.Add(PebbleLinkTime::MIS.Label);
	arrayHeader.Add(PebbleLinkTime::UPN.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowPebbleLinkTime RowPB;
		RowPB.SetLineID(csvIn.GetInt(0));
		RowPB.SetDate(csvIn.GetString(1));
		RowPB.SetTime(csvIn.GetString(2));
		RowPB.SetUserID(csvIn.GetInt(3));
		RowPB.SetUsername(csvIn.GetString(4));
		RowPB.SetForename(csvIn.GetString(5));
		RowPB.SetSurname(csvIn.GetString(6));
		RowPB.SetGroupNo(csvIn.GetInt(7));
		RowPB.SetPebbleFirstName(csvIn.GetString(8));
		RowPB.SetPebbleLastName(csvIn.GetString(9));
		RowPB.SetProcessType(csvIn.GetInt(10));
		RowPB.SetMatchType(csvIn.GetInt(11));
		RowPB.SetMIS(csvIn.GetString(12));
		RowPB.SetUPN(csvIn.GetString(13));

		if (InsertRow(RowPB, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
