//$$******************************************************************
#pragma once
//$$******************************************************************

class CGroupComboHelpers
{
public:
	static void FillGroupCombo(CSSTabbedComboBox& combo, int nGroupNo, bool bAllGroups, int nTabPos = 50);
	static void FillGroupCombo(CSSTabbedComboBox& combo, CArray<int,int>& arrayGroups, int nGroupNo, int nTabPos = 50);
	static int GetGroupNoFromCombo(CSSTabbedComboBox& combo, int nDefault);
};

//$$******************************************************************

