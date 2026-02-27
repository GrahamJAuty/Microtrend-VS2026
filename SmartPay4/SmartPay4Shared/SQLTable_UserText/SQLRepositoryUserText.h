#pragma once
//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowUserText.h"
//**********************************************************************

class CSQLRepositoryUserText
{
public:
	CSQLRepositoryUserText();

public:
	CSQLResultInfo SelectRow(CSQLRowUserText& SQLRowText, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowUserText& SQLRowText, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowUserText& SQLRowText, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowUserText& SQLRowText, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowUserText& SQLRowUserText, CDatabase* pDatabase);
	CSQLResultInfo RowExists(CString strFieldLabel, CDatabase* pDatabase);
	void CreateDefaultTexts(CDatabase* pDatabase);

public:
	CString GetUserText(CString strLabel, CString strDefault = "" );
	CSQLResultInfo SetUserText(CString strLabel, CString strText, CDatabase* pDatabase, bool bUseFlag = TRUE);

public:
	CString GetInfo1Text(CString strDefault = "") { return GetUserText(Account::Info1.Label, strDefault); }
	CString GetInfo2Text(CString strDefault = "") { return GetUserText(Account::Info2.Label, strDefault); }
	CString GetInfo3Text(CString strDefault = "") { return GetUserText(Account::Info3.Label, strDefault); }
	CString GetInfo4Text(CString strDefault = "") { return GetUserText(Account::Info4.Label, strDefault); }
	CString GetInfo5Text(CString strDefault = "") { return GetUserText(Account::Info5.Label, strDefault); }
	CString GetInfo6Text(CString strDefault = "") { return GetUserText(Account::Info6.Label, strDefault); }
	CString GetInfo7Text(CString strDefault = "") { return GetUserText(Account::Info7.Label, strDefault); }
	CString GetInfo8Text(CString strDefault = "") { return GetUserText(Account::Info8.Label, strDefault); }
	CString GetTick1Text(CString strDefault = "");
	CString GetTick2Text(CString strDefault = "");
	CString GetTick3Text(CString strDefault = "");
	CString GetTick4Text(CString strDefault = "");
	CString GetMemberIDText(CString strDefault = "MemberID") { return GetUserText(Account::MemberID.Label, strDefault); }
	CString GetExtRef1Text(CString strDefault = "MiFare") { return GetUserText(Account::ExternalRef1.Label, strDefault); }
	CString GetExtRef2Text(CString strDefault = "MiFare2") { return GetUserText(Account::ExternalRef2.Label, strDefault); }
	CString GetDOBText(CString strDefault = "DoB") { return GetUserText(Account::DOB.Label, strDefault); }
	
public:
	CString GetTickText(int nTickNo);
	CString GetInfoText(int nInfoNo);
	CString GetDisplayInfoText(int nInfoNo);
	CString GetDisplayMemberText();
	CString GetDisplayExtRef1Text();
	CString GetDisplayExtRef2Text();
	CString GetDbDisplayText(CString strLabel);
	static CString GetDbLabel(CString strDisplayText);
	bool IsExtRef2Reqd();

private:
	static CString GetDisplayText(CString strLabel, CString strText);
	
private:
	CString m_strBioConsent;
};

//**********************************************************************
