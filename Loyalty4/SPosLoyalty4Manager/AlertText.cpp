//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "AlertText.h"
//$$******************************************************************

CString CAlertText::CreateAlertStatusString(CSQLRowAccountFull& RowAccount)
{
	CString strResult = "";
	
	AddTrailing(strResult, ALERT_MAX, '0');

	CCSV csv(RowAccount.GetAlertCodes(),':');

	for (int n = 0; (n < ALERT_MAX) && (n < csv.GetSize()); n++)
	{
		int a = csv.GetInt(n);

		if ((a >= 1) && (a <= ALERT_MAX))
		{
			strResult.SetAt(a - 1, '1');
		}
	}

	return strResult;
}

//$$******************************************************************

bool CAlertText::ValidateRecord(CSQLRowAccountFull& RowAccount, CString strAlertFilter)
{
	CArray<int, int> arrayAlertFilter;
	ConvertFilterStringToArray(strAlertFilter, arrayAlertFilter);

	CString strAccountAlerts = "";

	bool bOK = TRUE;
	for (int n = 1; (n <= arrayAlertFilter.GetSize()) && (TRUE == bOK); n++)
	{
		int nOption = arrayAlertFilter[n - 1];

		switch (nOption)
		{
		case 1:
		case 2:

			if (strAccountAlerts == "")
			{
				strAccountAlerts = CreateAlertStatusString(RowAccount);
			}

			if (1 == nOption)
			{
				bOK = strAccountAlerts[n - 1] == '1';
			}
			else
			{
				bOK = strAccountAlerts[n - 1] == '0';
			}

			break;
		}
	}

	return bOK;
}

//$$******************************************************************

void CAlertText::ConvertFilterStringToArray(CString str, CArray<int, int>& array)
{
	array.RemoveAll();
	for (int n = 0; n < ALERT_MAX; n++)
	{
		array.Add(0);
	}

	for (int n = 0; (n < ALERT_MAX) && (n < str.GetLength()); n++)
	{
		char c = str[n];

		if (c == '1')
		{
			array.SetAt(n, 1);
		}
		else if (c == '2')
		{
			array.SetAt(n, 2);
		}
	}
}

//$$******************************************************************
