//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLDb.h"
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPOSLoyalty4Shared\SQLTable_StaffGiftException\SQLRepositoryStaffGiftException.h"
//**********************************************************************
#include "WorkingDlg.h"
//**********************************************************************
#include "SQLRowSetStaffGiftException.h"
//**********************************************************************

CSQLRowSetStaffGiftException::CSQLRowSetStaffGiftException()
{
}

//**********************************************************************

void CSQLRowSetStaffGiftException::LoadExceptions()
{
	m_arrayRows.RemoveAll();
	
	CSQLRecordSetStaffGiftException RecordSet(NULL, RSParams_StaffGiftException_NormalNoParams{});

	CWorkingDlg WorkingDlg("Loading Staff Gift Exceptions");
	WorkingDlg.Create();
	int nRecordsChecked = 0;

	CSQLRowStaffGiftException RowSC;
	while (RecordSet.StepSelectAll(RowSC) == TRUE)
	{
		nRecordsChecked++;
		if (nRecordsChecked % 10 == 0)
		{
			WorkingDlg.SetCaption2RecordsChecked(nRecordsChecked, FALSE);
		}

		RowSC.SetDisplayUserInfo();
		m_arrayRows.Add(RowSC);
	}
}

//**********************************************************************

void CSQLRowSetStaffGiftException::GetRow(int nRow, CSQLRowStaffGiftException& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		Row = m_arrayRows.GetAt(nRow);
	}
}

//**********************************************************************

void CSQLRowSetStaffGiftException::InsertRow(int n, CSQLRowStaffGiftException& Row)
{
	if ((n>= 0) && (n <= GetSize()))
	{
		m_arrayRows.InsertAt( n, Row);
	}
}

//**********************************************************************

void CSQLRowSetStaffGiftException::UpdateRow(int n, CSQLRowStaffGiftException& Row)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.SetAt(n, Row);
	}
}

//**********************************************************************

void CSQLRowSetStaffGiftException::RemoveRow(int n)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_arrayRows.RemoveAt(n);
	}
}

//**********************************************************************

