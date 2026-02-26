/**********************************************************************/
#include "PluTypeHelpers.h"
/**********************************************************************/

int CPluTypeHelpers::MicrotrendToSPOS(int nType)
{
	int nResult = 0;
	
	switch (nType)
	{
	case 0:		nResult = 0;		break;
	case 1:		nResult = 1;		break;
	case 2:		nResult = 20;		break;
	case 3:		nResult = 40;		break;
	case 4:		nResult = 41;		break;
	case 5:		nResult = 42;		break;
	case 6:		nResult = 90;		break;
	case 7:		nResult = 30;		break;
	case 8:		nResult = 31;		break;
	case 9:		nResult = 32;		break;
	case 10:	nResult = 33;		break;
	case 11:	nResult = 43;		break;
	case 12:	nResult = 34;		break;
	}
	
	return nResult;
}

/**********************************************************************/

int CPluTypeHelpers::SPOSToMicrotrend(int nType)
{
	int nResult = 0;

	switch (nType)
	{
	case 0:		nResult = 0;		break;
	case 1:		nResult = 1;		break;
	case 20:	nResult = 2;		break;
	case 40:	nResult = 3;		break;
	case 41:	nResult = 4;		break;
	case 42:	nResult = 5;		break;
	case 90:	nResult = 6;		break;
	case 30:	nResult = 7;		break;
	case 31:	nResult = 8;		break;
	case 32:	nResult = 9;		break;
	case 33:	nResult = 10;		break;
	case 43:	nResult = 11;		break;
	case 34:	nResult = 12;		break;
	}
	
	return nResult;
}

/**********************************************************************/

int CPluTypeHelpers::ComboToMicrotrend(int nType)
{
	int nResult = 0;

	switch (nType)
	{
	case 0:		nResult = 0;	break;
	case 1:		nResult = 1;	break;
	case 2:		nResult = 2;	break;
	case 3:		nResult = 7;	break;
	case 4:		nResult = 8;	break;
	case 5:		nResult = 9;	break;
	case 6:		nResult = 10;	break;
	case 7:		nResult = 12;	break;
	case 8:		nResult = 3;	break;
	case 9:		nResult = 4;	break;
	case 10:	nResult = 5;	break;
	case 11:	nResult = 11;	break;
	case 12:	nResult = 6;	break;
	}

	return nResult;
}

/**********************************************************************/

CString CPluTypeHelpers::GetText(int nType)
{
	CString strResult = "";

	switch (nType)
	{
	case 0:		strResult = "General";				break;
	case 1:		strResult = "Extra";				break;
	case 2:		strResult = "Condiment";			break;
	case 3:		strResult = "Dummy";				break;
	case 4:		strResult = "Qualifier";			break;
	case 5:		strResult = "Other";				break;
	case 6:		strResult = "System";				break;
	case 7:		strResult = "Deposit";				break;
	case 8:		strResult = "Information";			break;
	case 9:		strResult = "Ext. Process Top-up";	break;
	case 10:	strResult = "Loyalty Top-up";		break;
	case 11:	strResult = "External";				break;
	case 12:	strResult = "Loyalty Staff Gift";	break;
	default:	strResult = "Unknown";				break;
	}

	return strResult;
}

/**********************************************************************/

void CPluTypeHelpers::AddComboStrings(CComboBox& combo)
{
	for (int n = 0; n <= GetMaxPluType(); n++)
	{
		int nType = ComboToMicrotrend(n);
		int nPos = combo.AddString(GetText(nType));
		combo.SetItemData(nPos, nType);
	}
}

/**********************************************************************/
