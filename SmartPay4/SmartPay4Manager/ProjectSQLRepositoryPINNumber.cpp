//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryPINNumber.h"
//**********************************************************************

CProjectSQLRepositoryPINNumber::CProjectSQLRepositoryPINNumber() : CSQLRepositoryPINNumber()
{
	m_strImportTableName = SQLTableNames::PINNumber;
}

//**********************************************************************

bool CProjectSQLRepositoryPINNumber::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing PIN Number Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring PIN Number Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(PINNumber::PINNumber.Label);
	arrayHeader.Add(PINNumber::UserID.Label);
	arrayHeader.Add(PINNumber::Username.Label);
	arrayHeader.Add(PINNumber::FirstDate.Label);
	arrayHeader.Add(PINNumber::FirstTime.Label);
	arrayHeader.Add(PINNumber::LastDate.Label);
	arrayHeader.Add(PINNumber::LastTime.Label);
	arrayHeader.Add(PINNumber::UseCounter.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowPINNumber RowPN;
		RowPN.SetPINNumber(csvIn.GetString(0));
		RowPN.SetUserID(csvIn.GetString(1));
		RowPN.SetUsername(csvIn.GetString(2));
		RowPN.SetFirstDate(csvIn.GetString(3));
		RowPN.SetFirstTime(csvIn.GetString(4));
		RowPN.SetLastDate(csvIn.GetString(5));
		RowPN.SetLastTime(csvIn.GetString(6));
		RowPN.SetUseCounter(csvIn.GetInt(7));

		if (InsertRow(RowPN, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}

	return TRUE;
}

//**********************************************************************
