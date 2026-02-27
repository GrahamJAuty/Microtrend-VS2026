//**********************************************************************
#include "ExternalCommandSigner.h"
//**********************************************************************

int CExternalCommandSigner::CheckSignature(CStringArray& arrayLines)
{
	int nResult = CheckSignatureInternal(arrayLines);

#ifdef _DEBUG
	nResult = 0;
#endif

	return nResult;
}

//**********************************************************************

int CExternalCommandSigner::CheckSignatureInternal(CStringArray& arrayLines)
{
	CString strText = "";
	CString strTimeStamp = "";
	CString strHash = "";

	int nState = 0;

	for (int n = 0; (n < arrayLines.GetSize()) && (nState != 4); n++)
	{
		CString strLine = arrayLines.GetAt(n);

		switch (nState)
		{
		case 0:
			if (strLine == GetDelimiter())
			{
				nState = 1;
			}
			else
			{
				strText += strLine;
				strText += "\n";
			}
			break;

		case 1:
			strTimeStamp = strLine;
			nState = 2;
			break;

		case 2:
			strHash = strLine;
			nState = 3;
			break;

		case 3:
			nState = 4;
			break;
		}
	}

	//ASSUME INVALID SIGNATURE
	int nResult = 1;

	if (3 == nState)
	{
		CString strCalcHash = CSSMD5::CalcMD5(strText + strTimeStamp + GetSecret());
		if (strCalcHash == strHash)
		{
			__int64 nTimeStamp = _atoi64(strTimeStamp);

			time_t timeNow;
			time(&timeNow);
			__int64 nTimeNow = timeNow;

			if ((nTimeNow < nTimeStamp - 600) || (nTimeNow > nTimeStamp + 600))
			{
				nResult = 2;
			}
			else
			{
				nResult = 0;
			}
		}
	}

	return nResult;
}

//**********************************************************************

void CExternalCommandSigner::SignCommand(CStringArray& arrayCommand)
{
	CString strTimeStamp = "";

	{
		time_t timeNow;
		time(&timeNow);
		strTimeStamp = FormatInt64Value(timeNow);
	}

	CString strText = "";
	for (int n = 0; n < arrayCommand.GetSize(); n++)
	{
		strText += arrayCommand.GetAt(n);
		strText += "\n";
	}

	arrayCommand.Add(GetDelimiter());
	arrayCommand.Add(strTimeStamp);
	arrayCommand.Add(CSSMD5::CalcMD5(strText + strTimeStamp + GetSecret()));
}

//**********************************************************************
