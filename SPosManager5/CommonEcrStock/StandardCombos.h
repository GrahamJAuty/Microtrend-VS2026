#pragma once
//*******************************************************

class CStandardCombos
{
public:
	CStandardCombos(void);

public:
	static void FillPortCombo(CSSComboBox& combo, const char* sz);
	static void FillBaudCombo(CSSComboBox& combo, const char* sz);
	static void FillExtRefCombo(CSSComboBox& combo, int nSel = 0);
	static void FillFormatCombo(CSSComboBox& combo, int nSel = 0);
	
private:
	static void SetComboSel(CSSComboBox& combo, int nSel, int nDefSel = 0);
	static void SelectComboString(CSSComboBox& combo, const char* sz, int nDefSel = -1);
};

//*******************************************************
