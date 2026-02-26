//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetPluInfo.h"
//**********************************************************************

CProjectSQLRecordSetPluInfo::CProjectSQLRecordSetPluInfo() : CSQLRecordSetPluInfo(NULL, RSParams_PluInfo_NormalNoParams	{})
{
}

//**********************************************************************

void CProjectSQLRecordSetPluInfo::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)			
{
	CStringArray arrayHeader;
	arrayHeader.Add(PluInfo::DbNo.Label);
	arrayHeader.Add(PluInfo::PluNo.Label);
	arrayHeader.Add(PluInfo::Description.Label);
	arrayHeader.Add(PluInfo::DeptNo.Label);
	arrayHeader.Add(PluInfo::TaxCode.Label);
	arrayHeader.Add(PluInfo::Points.Label);
	arrayHeader.Add(PluInfo::ModType.Label);
	arrayHeader.Add(PluInfo::StampOfferID.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::PluInfo, arrayHeader);

	SQLOut.WriteDelete();

	int nChecked = 0;
	CSQLRowPluInfo RowPluInfo;
	while (StepSelectAll(RowPluInfo) == TRUE)
	{
		SQLOut.Add(RowPluInfo.GetDbNo());
		SQLOut.Add(RowPluInfo.GetPluNo());
		SQLOut.Add(RowPluInfo.GetDescription());
		SQLOut.Add(RowPluInfo.GetDeptNo());
		SQLOut.Add(RowPluInfo.GetTaxCode());
		SQLOut.Add(RowPluInfo.GetPoints());
		SQLOut.Add(RowPluInfo.GetModType());
		SQLOut.Add(RowPluInfo.GetStampOfferID());

		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
