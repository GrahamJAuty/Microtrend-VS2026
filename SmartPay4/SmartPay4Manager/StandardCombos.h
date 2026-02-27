#pragma once
//*******************************************************

class CStandardCombos
{
public:
	CStandardCombos(void);

public:
	static void FillGenderCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillAutoRefreshCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillGroupMaxSpendCombo( CSSComboBox& combo, int nMaxSpendType );
	static void FillGroupMaxSpendOverrideCombo( CSSComboBox& combo, int nMaxSpendType );
	static void FillFieldNamesCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillWeekdayCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillFormatCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillBioRegisterCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillServiceProviderCombo( CSSTabbedComboBox& combo, int nSel = 0 );
	static void FillDatabaseImportCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillInterfaceCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillBackgroundOptionsCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillExtRefCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillDefaultExtRefCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillOutputTypeCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillSortOnCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillPaymentOrderCombo( CSSComboBox& combo, int nSel = 0 );
	static void FillRefreshPurseCombo( CSSComboBox& combo, int nSel );
	static void FillFolderSetCombo(CSSTabbedComboBox& combo, int nTargetSetNo);

public:
	static void FillGenderCombo( CSSComboBox& combo, const char* sz );
	static void FillGenderConditionCombo( CSSComboBox& combo, const char* sz );
	static void FillAndOrCombo( CSSComboBox& combo, const char* sz );
	static void FillConditionCombo( CSSComboBox& combo, const char* sz );
	static void FillExportFieldCombo(CSSComboBox& combo, const char* sz);
	static void FillBaudCombo( CSSComboBox& combo, const char* sz );
	static void FillSoapCombo( CSSComboBox& combo, const char* sz );
	static void FillPortCombo( CSSComboBox& combo, const char* sz );

public:
	static void SelectComboStringWithDefaultPos(CSSComboBox& combo, const char* sz, int nPos);
	static int GetItemDataOfSelection(CSSTabbedComboBox& combo, int nDefault);

private:
	static void AddGenderTexts( CSSComboBox& combo ); 

private:
	static void SetComboSel( CSSComboBox& combo, int nSel, int nDefSel = 0 );
	static void SetComboSel( CSSTabbedComboBox& combo, int nSel, int nDefSel = 0);
	static void SelectComboString( CSSComboBox& combo, const char* sz, int nDefSel = -1 );
};

//*******************************************************
