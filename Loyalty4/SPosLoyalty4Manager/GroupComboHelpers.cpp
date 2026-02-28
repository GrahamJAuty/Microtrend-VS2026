//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRecordSetGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_Scheme\SQLRepositoryScheme.h"
//*******************************************************************
#include "GroupComboHelpers.h"
//*******************************************************************

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

	CSQLRecordSetGroup RecordSet(NULL, RSParams_Group_NormalNoParams{});

	CSQLRowGroup RowGroup;
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

void CGroupComboHelpers::FillSchemeCombo(CSSTabbedComboBox& combo, int nGroupNo, int nSchemeNo, bool bShowZero, bool bUseGroup, int nTabPos)
{
	combo.ResetContent();

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(nTabPos);
	combo.SetTabStops(arrayStops);

	int nSchemeCurSel = -1;

	CReportConsolidationArray<CSortedStringByInt> arrayNames;
	CSQLRepositoryScheme PrepStatScheme;
	PrepStatScheme.GetSchemeNameList(arrayNames, nSchemeNo, TRUE, NULL);

	if (TRUE == bShowZero)
	{
		if (FALSE == bUseGroup)
		{
			CSortedStringByInt item;
			item.m_nItem = 0;
			item.m_strItem = "From Group";
			arrayNames.InsertAt(0, item);
		}
		else
		{
			int nSchemeNo = nGroupNo;

			CSQLRowGroup RowGroup;
			RowGroup.SetGroupNo(nGroupNo);
			CSQLRepositoryGroup repoGroup;
			if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				if (RowGroup.GetSchemeNo() > 0)
				{
					nSchemeNo = RowGroup.GetSchemeNo();
				}
			}

			CSortedStringByInt item;
			item.m_nItem = nSchemeNo;

			int nPos = 0;
			if (arrayNames.Find(item, nPos) == TRUE)
			{
				arrayNames.GetAt(nPos, item);
			}

			SolutionGlobalFunctions::TrimSpacesFromString(item.m_strItem, FALSE);

			if (item.m_strItem == "")
			{
				item.m_strItem.Format("Scheme %d", nSchemeNo);
			}

			item.m_nItem = 0;
			arrayNames.InsertAt(0, item);
		}
	}

	for (int n = 0; n < arrayNames.GetSize(); n++)
	{
		CSortedStringByInt item;
		arrayNames.GetAt(n, item);

		CString strText = "";

		if (item.m_nItem != 0)
		{
			strText.Format("S%3.3d,%s",
				item.m_nItem,
				(const char*)item.m_strItem);
		}
		else
		{
			strText.Format("GRP,%s",
				(const char*)item.m_strItem);
		}

		combo.AddItem(strText);
		
		int nPos = combo.GetCount() - 1;
		combo.SetItemData(nPos, item.m_nItem);
		
		if (nSchemeNo == item.m_nItem)
		{
			nSchemeCurSel = nPos;
		}
	}

	if (nSchemeCurSel >= 0)
	{
		combo.SetCurSel(nSchemeCurSel);
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

//*******************************************************************
