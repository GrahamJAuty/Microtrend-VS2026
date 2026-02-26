//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetStampOfferInfo.h"
//**********************************************************************

CProjectSQLRecordSetStampOfferInfo::CProjectSQLRecordSetStampOfferInfo() : CSQLRecordSetStampOfferInfo(NULL, RSParams_StampOffer_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetStampOfferInfo::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)			
{
	CStringArray arrayHeader;
	arrayHeader.Add(StampOfferInfo::StampOfferID.Label);
	arrayHeader.Add(StampOfferInfo::Edition.Label);
	arrayHeader.Add(StampOfferInfo::Enabled.Label);
	arrayHeader.Add(StampOfferInfo::Threshold.Label);
	arrayHeader.Add(StampOfferInfo::Expiry.Label);
	arrayHeader.Add(StampOfferInfo::Description.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::StampOfferInfo, arrayHeader);

	SQLOut.WriteDelete();

	int nChecked = 0;
	CSQLRowStampOfferInfo RowOS;
	while (StepSelectAll(RowOS) == TRUE)
	{
		SQLOut.Add(RowOS.GetStampOfferID());
		SQLOut.Add(RowOS.GetEdition());
		SQLOut.Add(RowOS.GetEnableFlag());
		SQLOut.Add(RowOS.GetThreshold());
		SQLOut.Add(RowOS.GetExpiry());
		SQLOut.Add(RowOS.GetDescription());

		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
