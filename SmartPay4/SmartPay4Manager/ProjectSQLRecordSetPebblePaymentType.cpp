//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetPebblePaymentType.h"
//**********************************************************************

CProjectSQLRecordSetPebblePaymentType::CProjectSQLRecordSetPebblePaymentType() : CSQLRecordSetPebblePaymentType(NULL, RSParams_PebblePaymentType_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetPebblePaymentType::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)			
{
	CStringArray arrayHeader;
	arrayHeader.Add(PebblePaymentType::SPOSPaymentType.Label);
	arrayHeader.Add(PebblePaymentType::PebblePaymentName.Label);
	arrayHeader.Add(PebblePaymentType::PebbleTranType.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::PebblePaymentTypes, arrayHeader);

	m_strFilter = "";
	m_strSort = "";

	SQLOut.WriteDelete();

	CSQLRowPebblePaymentType RowPB;
	while (StepSelectAll(RowPB) == TRUE)
	{
		SQLOut.Add(RowPB.GetSPOSPaymentType());
		SQLOut.Add(RowPB.GetPebblePaymentName());
		SQLOut.Add(RowPB.GetPebbleTranType());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
