//$$****************************************************************
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLDb.h"
//$$******************************************************************
#include "GroupComboHelpers.h"
//$$******************************************************************

void CGroupComboHelpers::FillGroupCombo(CSSTabbedComboBox& combo, int nGroupNo, bool bAllGroups, int nTabPos )
{
	combo.ResetContent();

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(nTabPos);
	combo.SetTabStops(arrayStops);

	int nGroupCurSel = -1;

	if (TRUE == bAllGroups)
	{
		combo.AddItem(",All groups");
		combo.SetItemData(0, 0);
	}

	CSQLRecordSetGroupFull RecordSet(NULL, RSParams_Group_NormalNoParams{});

	CSQLRowGroupFull RowGroup;
	while (RecordSet.StepSelectAll(RowGroup) == TRUE)
	{
		int nItemGroupNo = RowGroup.GetGroupNo();

		// format text to 'nnn : group name...'
		CString strText;
		strText.Format("G%3.3d,%s",
			nItemGroupNo,
			(const char*) RowGroup.GetGroupName());

		combo.AddItem(strText);

		int nPos = combo.GetCount() - 1;

		combo.SetItemData(nPos, nItemGroupNo);

		if (nGroupNo == nItemGroupNo)
		{
			nGroupCurSel = nPos;
		}
	}

	if (nGroupCurSel >= 0 )
	{
		combo.SetCurSel(nGroupCurSel);
	}
	else if (combo.GetCount() > 0)
	{
		combo.SetCurSel(0);
	}

	combo.EnableWindow(combo.GetCount() > 1);
}

//*******************************************************************

void CGroupComboHelpers::FillGroupCombo(CSSTabbedComboBox& combo, CArray<int,int>& arrayGroups, int nGroupNo, int nTabPos)
{
	combo.ResetContent();

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(nTabPos);
	combo.SetTabStops(arrayStops);

	int nGroupCurSel = -1;

	for (int n = 0; n < arrayGroups.GetSize(); n++)
	{
		int nThisGroupNo = arrayGroups.GetAt(n);

		CSQLRowGroupFull RowGroup;
		RowGroup.SetGroupNo(nThisGroupNo);

		CString strText = "";

		CSQLRepositoryGroup RepoGroup;
		if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			strText.Format("G%3.3d,%s",
				nThisGroupNo,
				(const char*) RowGroup.GetGroupName());
		}
		else
		{
			strText.Format("G%3.3d,Group %d",
				nThisGroupNo,
				nThisGroupNo);
		}

		combo.AddItem(strText);

		int nPos = combo.GetCount() - 1;

		combo.SetItemData(nPos, nThisGroupNo);

		if (nGroupNo == nThisGroupNo)
		{
			nGroupCurSel = nPos;
		}
	}

	if (nGroupCurSel >= 0)
	{
		combo.SetCurSel(nGroupCurSel);
	}
	else if (combo.GetCount() > 0)
	{
		combo.SetCurSel(0);
	}

	combo.EnableWindow(combo.GetCount() > 1);
}

//*******************************************************************

int CGroupComboHelpers::GetGroupNoFromCombo(CSSTabbedComboBox& combo, int nDefault)
{
	int nResult = nDefault;

	int nSel = combo.GetCurSel();
	if ((nSel >= 0) && (nSel < combo.GetCount()))
	{
		nResult = combo.GetItemData(nSel);
	}

	return nResult;
}

//$$******************************************************************
