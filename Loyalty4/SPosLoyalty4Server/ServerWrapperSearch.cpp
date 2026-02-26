//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\ImageHandler.h"
//**********************************************************************
#include "SPosLoyalty4Server.h"
//**********************************************************************
#include "ServerWrapper.h"
//**********************************************************************

CSPOSSearchInfo::CSPOSSearchInfo()
{
	Reset();
}

//**********************************************************************

void CSPOSSearchInfo::Reset()
{
	m_nMaxToFind = Server.GetSearchMax();
	m_bCheckForename = FALSE;
	m_bCheckSurname = FALSE;
	m_bCheckPostcode = FALSE;
	m_bExcludeHotlist = FALSE;
	m_bExcludeInactive = FALSE;
	m_strForename = "";
	m_strSurname = "";
	m_strPostcode = "";
}

//**********************************************************************

void CSPOSSearchInfo::SetSearchInfo(CString strFindString)
{
	strFindString.MakeUpper();
	CCSV csvFindString(strFindString);

	Reset();

	for (int n = 0; n < csvFindString.GetSize(); n++)
	{
		CString strField = csvFindString.GetString(n);

		if (strField.Left(4) == "MAX=")
		{
			m_nMaxToFind = atoi(strField.Mid(4));
		}
		else if (strField.Left(3) == "N1=")
		{
			m_bCheckSurname = TRUE;
			m_strSurname = strField.Mid(3);
		}
		else if (strField.Left(3) == "N2=")
		{
			m_bCheckForename = TRUE;
			m_strForename = strField.Mid(3);
		}
		else if (strField.Left(3) == "PC=")
		{
			m_bCheckPostcode = TRUE;
			m_strPostcode = strField.Mid(3);
			CServerWrapper::RemoveSpaces(m_strPostcode);
		}
	}

	m_nMaxToFind = max(m_nMaxToFind, 2);
	m_nMaxToFind = min(m_nMaxToFind, 100);
}

//**********************************************************************
//**********************************************************************
//**********************************************************************

void CServerWrapper::CreateSearchReplyAccountLookup(const char* szCsvFindString )
{
	if (SearchRecordsAccountLookup(szCsvFindString) == 0)
	{
		m_LoyReqThreadManager.AddResponseLine(StringNoneExists());
	}
}

//**********************************************************************

int CServerWrapper::SearchRecordsAccountLookup(const char* szCsvFindString)
{
	CSPOSSearchInfo SearchInfo;
	SearchInfo.SetSearchInfo(szCsvFindString);
	SearchInfo.m_bExcludeHotlist = Server.GetSearchExcludeHotlistFlag();
	SearchInfo.m_bExcludeInactive = Server.GetSearchExcludeInactiveFlag();

	CString strWhere = "";
	CSQLBuilderBind BuilderBind{};
	CSQLRepositoryAccount repoAccount;
	repoAccount.PrepareServerSearchWhereAccountLookup(SearchInfo, strWhere, BuilderBind);

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NormalByParams{ &BuilderBind, strWhere });
	
	int nFoundCount = 0;

	CStringArray arrayReply;

	CString strPostcodeToMatch = SearchInfo.m_strPostcode;
	RemoveSpaces(strPostcodeToMatch);
	strPostcodeToMatch.MakeUpper();

	CSQLRowAccountFull RowAccount;
	while ((RecordSet.StepSelectAll(RowAccount) == TRUE) && (nFoundCount <= SearchInfo.m_nMaxToFind))
	{
		/*
		if ((Server.GetSearchExcludeHotlistFlag() == TRUE) && (RowAccount.IsCardHotlisted() == TRUE))
		{
			continue;
		}

		if ((Server.GetSearchExcludeInactiveFlag() == TRUE) && (RowAccount.GetInactive() == TRUE))
		{
			continue;
		}
		*/

		if (strPostcodeToMatch != "")
		{
			CString strAccountPostcode = RowAccount.GetAddress5();
			RemoveSpaces(strAccountPostcode);
			strAccountPostcode.MakeUpper();

			if (strPostcodeToMatch.Find(strAccountPostcode) == -1)
			{
				continue;
			}
		}

		if (++nFoundCount <= SearchInfo.m_nMaxToFind)
		{
			CString strBuf[5];

			strBuf[0].Format("ID=%s", (const char*)RowAccount.GetUserIDString());
			strBuf[1].Format("N=%s", (const char*)RowAccount.GetFullname());
			strBuf[2].Format("N2=%s", (const char*)RowAccount.GetForename());
			strBuf[3].Format("N1=%s", (const char*)RowAccount.GetSurname());
			strBuf[4].Format("PC=%s", (const char*)RowAccount.GetAddress5());

			CCSV csv{};
			csv.Add(strBuf[0]);
			csv.Add(strBuf[1]);
			csv.Add(strBuf[2]);
			csv.Add(strBuf[3]);
			csv.Add(strBuf[4]);

			if (Server.GetPhotoIDFolderType() != 0)
			{
				CImageHandler image(Server.GetLoyaltyUNCSystemPath());
				CString strFilename = image.GetFirst(RowAccount.GetUserIDString());

				if (strFilename != "")
				{
					strBuf[4].Format("image=%s", (const char*)strFilename);
					csv.Add(strBuf[4]);
				}
			}

			if (System.GetInterfaceType() == nINTERFACE_MIFAREv3)
			{
				CString strMiFare = RowAccount.GetExternalRef();
				if (strMiFare == "")
				{
					csv.Add("MifareLink=0");
				}
				else
				{
					csv.Add("MifareLink=1");
				}
			}

			arrayReply.Add(csv.GetLine());
		}
	}

	if (arrayReply.GetSize() > 0)
	{
		CString str = "";
		str.Format("Found=%d", arrayReply.GetSize());
		m_LoyReqThreadManager.AddResponseLine(str);

		for (int n = 0; n < arrayReply.GetCount(); n++)
		{
			m_LoyReqThreadManager.AddResponseLine(arrayReply.GetAt(n));
		}
	}

	if (nFoundCount > SearchInfo.m_nMaxToFind)
	{
		m_LoyReqThreadManager.AddResponseLine(StringMoreExists());		// save indicator that more records exist
	}

	return nFoundCount;
}

//**********************************************************************

void CServerWrapper::CreateSearchReplySnakesAndLadders(const char* szCsvFindString)
{
	CString strCardNo = "";
	if (SearchRecordsSnakesAndLadders(szCsvFindString, strCardNo) == 1)
	{
		CAccountIdentifier AccountIdentifier(strCardNo, nINTERFACE_SWIPEv1);
		CreateBalanceReply1(AccountIdentifier, TRUE);
	}
}

//**********************************************************************

int CServerWrapper::SearchRecordsSnakesAndLadders(const char* szCsvFindString, CString& strCardNo)
{
	CSPOSSearchInfo SearchInfo;
	SearchInfo.SetSearchInfo(szCsvFindString);
	SearchInfo.m_bCheckForename = FALSE;

	strCardNo = "";

	CStringArray arrayReply;

	CString strWhere = "";
	CSQLBuilderBind BuilderBind{};
	CSQLRepositoryAccount repoAccount;
	repoAccount.PrepareServerSearchWhereSnakesAndLadders(SearchInfo, strWhere, BuilderBind);

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NormalByParams{ &BuilderBind, strWhere });
	
	CString strPostcodeToMatch = SearchInfo.m_strPostcode;
	RemoveSpaces(strPostcodeToMatch);
	strPostcodeToMatch.MakeUpper();

	CSQLRowAccountFull RowAccount;
	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (strPostcodeToMatch != "")
		{
			CString strAccountPostcode = RowAccount.GetAddress5();
			RemoveSpaces(strAccountPostcode);
			strAccountPostcode.MakeUpper();

			if (strPostcodeToMatch.Find(strAccountPostcode) == -1)
			{
				continue;
			}
		}

		CCSV csvLine;
		csvLine.Add(RowAccount.GetUserID());
		csvLine.Add(RowAccount.GetFullname());
		csvLine.Add(RowAccount.GetForename());
		csvLine.Add(RowAccount.GetSurname());
		csvLine.Add(RowAccount.GetAddress5());							// postcode

		if (Server.GetPhotoIDFolderType() != 0)
		{
			CImageHandler image(Server.GetLoyaltyUNCSystemPath());
			CString strFilename = image.GetFirst(RowAccount.GetUserIDString());

			if (strFilename != "")
			{
				CString strText = "image=" + strFilename;
				csvLine.Add(strText);
			}
		}

		arrayReply.Add(csvLine.GetLine());

		if (arrayReply.GetSize() == 1)
		{
			strCardNo = RowAccount.GetUserIDString();
		}
		else
		{
			strCardNo = "";
		}
	}

	int nResult = arrayReply.GetSize();

	if (nResult != 1)
	{
		CString str = "";
		str.Format("Found=%d", arrayReply.GetSize());
		m_LoyReqThreadManager.AddResponseLine(str);

		for (int n = 0; n < arrayReply.GetCount(); n++)
		{
			m_LoyReqThreadManager.AddResponseLine(arrayReply.GetAt(n));
		}
	}

	return nResult;
}

//**********************************************************************

CString CServerWrapper::RemoveSpaces(const char* szText)
{
	CString strIn = szText;
	CString strOut = "";

	unsigned char uc;
	int nLength = strlen(strIn);
	for (int i = 0; i < nLength; i++)
	{
		uc = strIn.GetAt(i);

		if (uc != ' ')
		{
			strOut += uc;
		}
	}
	return strOut;
}

//***************************************************************
