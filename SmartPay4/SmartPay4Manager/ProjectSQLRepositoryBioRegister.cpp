//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryBioRegister.h"
//**********************************************************************

CProjectSQLRepositoryBioRegister::CProjectSQLRepositoryBioRegister() : CSQLRepositoryBioRegister()
{
	m_strImportTableName = SQLTableNames::BiometricRegister;
}

//**********************************************************************

bool CProjectSQLRepositoryBioRegister::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Biometric Register Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Biometric Register Table");

	fileIn.Seek(0, CFile::begin);

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

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowBioRegister RowBio;
		RowBio.SetUserID(csvIn.GetInt64(0));
		RowBio.SetUsername(csvIn.GetString(1));
		RowBio.SetRegDate(csvIn.GetString(2));
		RowBio.SetRegTime(csvIn.GetString(3));
		RowBio.SetUnRegDate(csvIn.GetString(4));
		RowBio.SetUnRegTime(csvIn.GetString(5));
		RowBio.SetUnRegFlag(csvIn.GetBool(6));
		RowBio.SetBioCounter(csvIn.GetInt(7));
		RowBio.SetFirstDate(csvIn.GetString(8));
		RowBio.SetFirstTime(csvIn.GetString(9));
		RowBio.SetLastDate(csvIn.GetString(10));
		RowBio.SetLastTime(csvIn.GetString(11));

		if (InsertRow(RowBio, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
