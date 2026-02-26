//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryCCNo.h"
//**********************************************************************

CProjectSQLRepositoryCCNo::CProjectSQLRepositoryCCNo() : CSQLRepositoryCCNo()
{
	m_strImportTableName = SQLTableNames::CCNo;
}

//**********************************************************************

bool CProjectSQLRepositoryCCNo::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, int nDbVersion, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing CCNo Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	
	if (nDbVersion >= 5)
	{
		WorkingDlg.SetCaption1("Restoring CCNo Table");

		fileIn.Seek(0, CFile::begin);

		CStringArray arrayHeader;
		arrayHeader.Add(CCNo::Type.Label);
		arrayHeader.Add(CCNo::Number.Label);

		CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

		CString strBuffer = "";
		while (SQLIn.ReadLine(strBuffer))
		{
			CCSV csvIn(strBuffer);

			CSQLRowCCNo RowCC;
			RowCC.SetTypeString(csvIn.GetString(0));
			RowCC.SetNumber(csvIn.GetInt(1));

			if (InsertRow(RowCC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}

			WorkingDlg.IncrementRecordsChecked(10, FALSE);
		}

		return TRUE;
	}
	else
	{		
		WorkingDlg.SetCaption1("Upgrading CCNo Data to SQL");

		if (ImportLegacyCCNo(pDatabase) == FALSE)
		{
			return FALSE;
		}

		return TRUE;
	}
}

//**********************************************************************
