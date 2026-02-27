//$$******************************************************************
#include "UserTypeText.h"
//$$******************************************************************

int CUserTypeText::Open(const char* szFilename, int nMode)
{
	int nReply = 0;
	if ((nReply = CSharedStringArray::Open(szFilename, nMode)) != DB_ERR_SHARE)
	{
		int nCount = GetSize();
		if (nCount == 0)
		{
			Add("Student");										// default values
			Add("Student Free");									// default values
			Add("Staff");										// default values
			Add("Staff Free");									// default values
			Add("Visitor");										// default values
			nCount = 5;
		}

		for (int i = nCount; i < USERTYPE_MAX; i++)				// make up shortfall
		{
			Add("");												// with default values
		}

		if (nMode == DB_READONLY)	// if file does not exist use default values
		{
			nReply = DB_ERR_NONE;
		}
	}

	return nReply;
}

//$$******************************************************************

CString CUserTypeText::GetText(int nTypeNo)
{
	CString strText = "";

	int nIndex = nTypeNo - 1;
	if ((nIndex >= 0) && (nIndex < GetSize()))
	{
		strText = GetAt(nIndex);
	}

	return strText;
}

//$$******************************************************************

void CUserTypeText::SetText ( int nTypeNo, const char* szText )
{
	int nIndex = nTypeNo - 1;

	if ((nIndex >= 0) && (nIndex < GetSize()))
	{
		SetAt(nIndex, szText);
	}
}

//$$******************************************************************

int CUserTypeText::GetCount()
{
	int nCount = 0;
	for (int nIndex = 0; nIndex < USERTYPE_MAX; nIndex++)
	{
		if (GetText(nIndex) != "")
		{
			++nCount;
		}
	}
	return nCount;
}

//$$******************************************************************
