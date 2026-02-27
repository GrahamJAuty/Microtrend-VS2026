//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetBioRegister.h"
//**********************************************************************

CProjectSQLRecordSetBioRegister::CProjectSQLRecordSetBioRegister() : CSQLRecordSetBioRegister(NULL, RSParams_BioRegister_NormalNoParams	{})
{
}

//**********************************************************************

void CProjectSQLRecordSetBioRegister::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(BioRegister::UserID.Label);
	arrayHeader.Add(BioRegister::Username.Label);
	arrayHeader.Add(BioRegister::RegDate.Label);
	arrayHeader.Add(BioRegister::RegTime.Label);
	arrayHeader.Add(BioRegister::UnRegDate.Label);
	arrayHeader.Add(BioRegister::UnRegTime.Label);
	arrayHeader.Add(BioRegister::UnRegFlag.Label);
	arrayHeader.Add(BioRegister::Counter.Label);
	arrayHeader.Add(BioRegister::FirstDate.Label);
	arrayHeader.Add(BioRegister::FirstTime.Label);
	arrayHeader.Add(BioRegister::LastDate.Label);
	arrayHeader.Add(BioRegister::LastTime.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::BiometricRegister, arrayHeader);

	m_strFilter = "";
	m_strSort = "";

	SQLOut.WriteDelete();

	CSQLRowBioRegister RowBio;
	while (StepSelectAll(RowBio) == TRUE)
	{
		SQLOut.Add(RowBio.GetUserID());
		SQLOut.Add(RowBio.GetUsername());
		SQLOut.Add(RowBio.GetRegDate());
		SQLOut.Add(RowBio.GetRegTime());
		SQLOut.Add(RowBio.GetUnRegDate());
		SQLOut.Add(RowBio.GetUnRegTime());
		SQLOut.Add(RowBio.GetUnRegFlag());
		SQLOut.Add(RowBio.GetBioCounter());
		SQLOut.Add(RowBio.GetFirstDate());
		SQLOut.Add(RowBio.GetFirstTime());
		SQLOut.Add(RowBio.GetLastDate());
		SQLOut.Add(RowBio.GetLastTime());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
