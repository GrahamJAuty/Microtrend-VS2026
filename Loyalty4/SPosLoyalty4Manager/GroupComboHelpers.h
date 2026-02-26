//*******************************************************************
#pragma once
//**********************************************************************

class CGroupComboHelpers
{
public:
	static void FillGroupCombo(CSSTabbedComboBox& combo, int nGroupNo, bool bAllGroups, int nTabPos = 50 );
	static void FillSchemeCombo(CSSTabbedComboBox& combo, int nGroupNo, int nSchemeNo, bool bShowZero, bool bUseGroup, int nTabPos = 50);
	static int GetGroupNoFromCombo(CSSTabbedComboBox& combo, int nDefault);
};

//**********************************************************************
