//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryPluInfo.h"
//**********************************************************************

CProjectSQLRepositoryPluInfo::CProjectSQLRepositoryPluInfo() : CSQLRepositoryPluInfo()
{
	m_strImportTableName = SQLTableNames::PluInfo;
}

//**********************************************************************

bool CProjectSQLRepositoryPluInfo::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing PLU Info Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring PLU Info Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(PluInfo::DbNo.Label);
	arrayHeader.Add(PluInfo::PluNo.Label);
	arrayHeader.Add(PluInfo::Description.Label);
	arrayHeader.Add(PluInfo::DeptNo.Label);
	arrayHeader.Add(PluInfo::TaxCode.Label);
	arrayHeader.Add(PluInfo::Points.Label);
	arrayHeader.Add(PluInfo::ModType.Label);
	arrayHeader.Add(PluInfo::StampOfferID.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowPluInfo RowPluInfo;
		RowPluInfo.SetDbNo(csvIn.GetInt(0));
		RowPluInfo.SetPluNo(csvIn.GetInt64(1));
		RowPluInfo.SetDescription(csvIn.GetString(2));
		RowPluInfo.SetDeptNo(csvIn.GetInt(3));
		RowPluInfo.SetTaxCode(csvIn.GetString(4));
		RowPluInfo.SetPoints(csvIn.GetInt(5));
		RowPluInfo.SetModType(csvIn.GetInt(6));
		RowPluInfo.SetStampOfferID(csvIn.GetInt(7));

		if (InsertRow(RowPluInfo, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
