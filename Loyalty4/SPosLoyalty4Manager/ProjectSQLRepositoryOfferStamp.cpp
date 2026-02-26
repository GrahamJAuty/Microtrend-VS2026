//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryOfferStamp.h"
//**********************************************************************

CProjectSQLRepositoryOfferStamp::CProjectSQLRepositoryOfferStamp() : CSQLRepositoryOfferStamp()
{
	m_strImportTableName = SQLTableNames::OfferStamps;
}

//**********************************************************************

bool CProjectSQLRepositoryOfferStamp::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Offer Stamp Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Offer Stamp Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(OfferStamp::UserID.Label);
	arrayHeader.Add(OfferStamp::StampOfferID.Label);
	arrayHeader.Add(OfferStamp::Edition.Label);
	arrayHeader.Add(OfferStamp::AwardDate.Label);
	arrayHeader.Add(OfferStamp::ExpireDate.Label);
	arrayHeader.Add(OfferStamp::StampCount.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		int nOfferID = csvIn.GetInt(1);
		if ((nOfferID < OfferStamp::StampOfferID.Min) || (nOfferID > OfferStamp::StampOfferID.Max))
		{
			continue;
		}

		CSQLRowOfferStamp RowOS;
		RowOS.SetUserID(csvIn.GetInt64(0));
		RowOS.SetStampOfferID(nOfferID);
		RowOS.SetEdition(csvIn.GetInt(2));
		RowOS.SetAwardDate(csvIn.GetString(3));
		RowOS.SetExpireDate(csvIn.GetString(4));
		RowOS.SetStampCount(csvIn.GetInt(5));

		if (InsertRow(RowOS, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
