//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryStampOfferInfo.h"
//**********************************************************************

CProjectSQLRepositoryStampOfferInfo::CProjectSQLRepositoryStampOfferInfo() : CSQLRepositoryStampOfferInfo()
{
	m_strImportTableName = SQLTableNames::StampOfferInfo;
}

//**********************************************************************

bool CProjectSQLRepositoryStampOfferInfo::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Stamp Offer Info Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Stamp Offer Info Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(StampOfferInfo::StampOfferID.Label);
	arrayHeader.Add(StampOfferInfo::Edition.Label);
	arrayHeader.Add(StampOfferInfo::Enabled.Label);
	arrayHeader.Add(StampOfferInfo::Threshold.Label);
	arrayHeader.Add(StampOfferInfo::Expiry.Label);
	arrayHeader.Add(StampOfferInfo::Description.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		int nOfferID = csvIn.GetInt(0);
		if ((nOfferID < StampOfferInfo::StampOfferID.Min) || (nOfferID > StampOfferInfo::StampOfferID.Max))
		{
			continue;
		}

		CSQLRowStampOfferInfo RowOS;
		RowOS.SetStampOfferID(nOfferID);
		RowOS.SetEdition(csvIn.GetInt(1));
		RowOS.SetEnableFlag(csvIn.GetBool(2));
		RowOS.SetThreshold(csvIn.GetInt(3));
		RowOS.SetExpiry(csvIn.GetInt(4));
		RowOS.SetDescription(csvIn.GetString(5));

		if (InsertRow(RowOS, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
