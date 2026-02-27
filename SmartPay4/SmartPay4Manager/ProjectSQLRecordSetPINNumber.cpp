//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetPINNumber.h"
//**********************************************************************

CProjectSQLRecordSetPINNumber::CProjectSQLRecordSetPINNumber() : CSQLRecordSetPINNumber(NULL, RSParams_PINNumber_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetPINNumber::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)				
{
	CStringArray arrayHeader;
	arrayHeader.Add(PINNumber::PINNumber.Label);
	arrayHeader.Add(PINNumber::UserID.Label);
	arrayHeader.Add(PINNumber::Username.Label);
	arrayHeader.Add(PINNumber::FirstDate.Label);
	arrayHeader.Add(PINNumber::FirstTime.Label);
	arrayHeader.Add(PINNumber::LastDate.Label);
	arrayHeader.Add(PINNumber::LastTime.Label);
	arrayHeader.Add(PINNumber::UseCounter.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::PINNumber, arrayHeader);

	SQLOut.WriteDelete();
	
	CSQLRowPINNumber RowPN;
	while (StepSelectAll(RowPN) == TRUE)
	{
		SQLOut.Add(RowPN.GetPINNumber());
		SQLOut.Add(RowPN.GetUserID());
		SQLOut.Add(RowPN.GetUsername());
		SQLOut.Add(RowPN.GetFirstDate());
		SQLOut.Add(RowPN.GetFirstTime());
		SQLOut.Add(RowPN.GetLastDate());
		SQLOut.Add(RowPN.GetLastTime());
		SQLOut.Add(RowPN.GetUseCounter());
		
		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
