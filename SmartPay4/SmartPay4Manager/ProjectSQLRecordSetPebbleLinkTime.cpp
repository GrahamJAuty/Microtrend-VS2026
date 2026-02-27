//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetPebbleLinkTime.h"
//**********************************************************************

CProjectSQLRecordSetPebbleLinkTime::CProjectSQLRecordSetPebbleLinkTime() : CSQLRecordSetPebbleLinkTime(NULL, RSParams_PebbleLinkTime_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetPebbleLinkTime::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)			
{
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

	CSQLOut SQLOut(fileOut, SQLTableNames::PebbleLinkTimes, arrayHeader);

	m_strFilter = "";
	m_strSort = "";

	SQLOut.WriteDelete();

	CSQLRowPebbleLinkTime RowPB;
	while (StepSelectAll(RowPB) == TRUE)
	{
		SQLOut.Add(RowPB.GetLineID());
		SQLOut.Add(RowPB.GetDate());
		SQLOut.Add(RowPB.GetTime());
		SQLOut.Add(RowPB.GetUserID());
		SQLOut.Add(RowPB.GetUsername());
		SQLOut.Add(RowPB.GetForename());
		SQLOut.Add(RowPB.GetSurname());
		SQLOut.Add(RowPB.GetGroupNo());
		SQLOut.Add(RowPB.GetPebbleFirstName());
		SQLOut.Add(RowPB.GetPebbleLastName());
		SQLOut.Add(RowPB.GetProcessType());
		SQLOut.Add(RowPB.GetMatchType());
		SQLOut.Add(RowPB.GetMIS());
		SQLOut.Add(RowPB.GetUPN());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
