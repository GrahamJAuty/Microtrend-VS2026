//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetOfferStamp.h"
//**********************************************************************

CProjectSQLRecordSetOfferStamp::CProjectSQLRecordSetOfferStamp() : CSQLRecordSetOfferStamp(NULL, RSParams_OfferStamp_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetOfferStamp::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)			
{
	CStringArray arrayHeader;
	arrayHeader.Add(OfferStamp::UserID.Label);
	arrayHeader.Add(OfferStamp::StampOfferID.Label);
	arrayHeader.Add(OfferStamp::Edition.Label);
	arrayHeader.Add(OfferStamp::AwardDate.Label);
	arrayHeader.Add(OfferStamp::ExpireDate.Label);
	arrayHeader.Add(OfferStamp::StampCount.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::OfferStamps, arrayHeader);

	SQLOut.WriteDelete();

	int nChecked = 0;
	CSQLRowOfferStamp RowOS;
	while (StepSelectAll(RowOS) == TRUE)
	{
		SQLOut.Add(RowOS.GetUserID());
		SQLOut.Add(RowOS.GetStampOfferID());
		SQLOut.Add(RowOS.GetEdition());
		SQLOut.Add(RowOS.GetAwardDate());
		SQLOut.Add(RowOS.GetExpireDate());
		SQLOut.Add(RowOS.GetStampCount());

		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
