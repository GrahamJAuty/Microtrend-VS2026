#pragma once
//*******************************************************

class CStandardCombos
{
public:
	CStandardCombos(void);

public:
	static void FillGenderCombo( CSSComboBox& combo, int nSel = 0, bool bIgnore = FALSE );
	static void FillGenderCombo( CSSComboBox& combo, const char* sz );
	static void FillIgnoreSelectCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillNoYesCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillAndOrCombo( CSSComboBox& combo, const char* sz );
	static void FillConditionCombo( CSSComboBox& combo, const char* sz );
	static void FillAutoRefreshCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillMemberTypeCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillDailySpendCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillPCBonusCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillFieldNamesCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillStartOfWeekCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillEditPageCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillExtRefCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillFormatCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillRenewTypeCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillBaudCombo( CSSComboBox& combo, const char* sz );
	static void FillPortCombo( CSSComboBox& combo, const char* sz );
	static void FillDatabaseCombo(CSSTabbedComboBox& combo, bool bHasAll, int nDbNo);
	static void FillFolderSetCombo(CSSTabbedComboBox& combo, int nDbNo, int nTargetSetNo);

public:
	static void SelectComboStringWithDefaultPos(CSSComboBox& combo, const char* sz, int nPos);

private:
	static void AddGenderTexts( CSSComboBox& combo, bool bIgnore = FALSE ); 

private:
	static void SetComboSel( CSSComboBox& combo, int nSel, int nDefSel = 0 );
	static void SelectComboString( CSSComboBox& combo, const char* sz );
};

//*******************************************************
