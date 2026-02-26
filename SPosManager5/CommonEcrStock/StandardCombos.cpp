//*******************************************************
#include "StandardCombos.h"
//*******************************************************

CStandardCombos::CStandardCombos(void)
{
}

//*******************************************************

void CStandardCombos::FillPortCombo(CSSComboBox& combo, const char* sz)
{
	combo.ResetContent();

	for (int n = 1; n <= 16; n++)
	{
		CString str;
		str.Format("COM%d", n);
		combo.AddString(str);
	}

	SelectComboString(combo, sz);
}

//*******************************************************

void CStandardCombos::FillBaudCombo(CSSComboBox& combo, const char* sz)
{
	combo.ResetContent();
	combo.AddString("9600");
	combo.AddString("19200");
	combo.AddString("38400");
	combo.AddString("57600");
	SelectComboString(combo, sz);
}

//*******************************************************

void CStandardCombos::FillExtRefCombo(CSSComboBox& combo, int nSel)
{
	combo.ResetContent();
	combo.AddString("MiFare");
	combo.AddString("Paxton");
	combo.AddString("Innerrange");
	SetComboSel(combo, nSel);
}

//*******************************************************

void CStandardCombos::FillFormatCombo(CSSComboBox& combo, int nSel)
{
	combo.ResetContent();
	combo.AddString("Format1");
	combo.AddString("Format2");
	SetComboSel(combo, nSel);
}

//*******************************************************

void CStandardCombos::SelectComboString(CSSComboBox& combo, const char* sz, int nDefSel)
{
	if (combo.SelectString(-1, sz) == CB_ERR)
	{
		if ((nDefSel >= 0) && (nDefSel < combo.GetCount()))
		{
			combo.SetCurSel(nDefSel);
		}
		else
		{
			combo.SetWindowText(sz);
		}
	}
}

//*******************************************************

void CStandardCombos::SetComboSel(CSSComboBox& combo, int nSel, int nDefSel)
{
	if ((nSel >= 0) && (nSel < combo.GetCount()))
	{
		combo.SetCurSel(nSel);
	}
	else if ((nDefSel >= 0) && (nDefSel < combo.GetCount()))
	{
		combo.SetCurSel(nDefSel);
	}
}

//*******************************************************
