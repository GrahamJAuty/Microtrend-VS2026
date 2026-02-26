#pragma once
/**********************************************************************/

class CPluTypeHelpers
{
public:
	static int MicrotrendToSPOS(int nType);
	static int SPOSToMicrotrend(int nType);
	static int ComboToMicrotrend(int nType);
	static CString GetText(int nType);
	static void AddComboStrings(CComboBox& combo);
	static int GetMaxPluType() { return 12; }
};

/**********************************************************************/
extern CPluTypeHelpers PluTypeHelpers;
/**********************************************************************/
