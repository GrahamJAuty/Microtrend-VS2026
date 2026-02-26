//$$******************************************************************
//$$*** exact same file used by LoyaltyManager / LoyaltyServer *******
//$$******************************************************************
#include "AlertText_base.h"
//$$******************************************************************

CAlertTextInfo::CAlertTextInfo()
{
	Reset();
}

//$$******************************************************************

void CAlertTextInfo::Reset()
{
	m_strText = "";
	m_bEPOS = FALSE;
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

int CAlertText_base::Open(const char* szFilename, int nMode)
{
	int nReply;
	if ((nReply = CSharedStringArray::Open(szFilename, nMode)) != DB_ERR_SHARE)
	{
		for (int i = GetSize(); i < ALERT_MAX; i++)	// make up shortfall
		{
			Add("");										// with default values
		}

		if (nMode == DB_READONLY)	// if file does not exist use default values
		{
			nReply = DB_ERR_NONE;
		}
	}

	return nReply;
}

//$$******************************************************************

void CAlertText_base::GetInfo(int nAlertCode, CAlertTextInfo& info )
{
	info.Reset();

	int nIndex = nAlertCode - 1;
	if ((nIndex >= 0) && (nIndex < GetSize()))
	{
		//FOR BACKWARDS COMPATIBILITY, ZERO IS EPOS FLAG ON
		CCSV csv(GetAt(nIndex));
		info.m_strText = csv.GetString(0);
		info.m_bEPOS = (csv.GetBool(1) == FALSE);
	}
}

//$$******************************************************************

void CAlertText_base::SetInfo(int nAlertCode, CAlertTextInfo& info)
{
	int nIndex = nAlertCode - 1;
	if ((nIndex >= 0) && (nIndex < GetSize()))
	{
		//FOR BACKWARDS COMPATIBILITY, ZERO IS EPOS FLAG ON
		CCSV csv;
		csv.Add(info.m_strText);
		csv.Add( FALSE == info.m_bEPOS);
		SetAt(nIndex, csv.GetLine());
	}
}

//$$******************************************************************

CString CAlertText_base::GetDisplayText(CString strAlertCodes)
{
	CString strText = "";

	CCSV csv(strAlertCodes, ':');

	int nCount = 0;
	for (int n = 0; (n < csv.GetSize()) && (n < ALERT_MAX); n++)
	{
		int a = csv.GetInt(n);

		if ((a >= 1) && (a <= GetSize()))
		{
			if (strText == "")
			{
				CAlertTextInfo info;
				GetInfo(a, info);
				strText = info.m_strText;
			}
			nCount++;
		}
	}

	if (nCount > 1)
	{
		CString strExtra = "";
		strExtra.Format(" + %d", nCount - 1);
		strText += strExtra;
	}

	if (0 == nCount)
	{
		strText = GetClearText();
	}

	return strText;
}

//$$******************************************************************

CString CAlertText_base::GetClearText()
{
	CString strText = "{ Clear }";
	return strText;
}

//$$******************************************************************

int CAlertText_base::GetAlertCount()
{
	int nCount = 0;
	for (int nIndex = 0; nIndex < ALERT_MAX; nIndex++)
	{
		CAlertTextInfo info;
		GetInfo(nIndex + 1, info);

		if (info.m_strText != "")
		{
			++nCount;
		}
	}
	return nCount;
}

//$$******************************************************************

