//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetWebPaymentOptions.h"
//**********************************************************************

CSQLRecordSetMetadataWebPaymentOptions::CSQLRecordSetMetadataWebPaymentOptions(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
	m_bForExistenceCheck = bForExistenceCheck;

	if (FALSE == bForExistenceCheck)
	{
		AddIntField(&m_nLineID, WebPaymentOption::LineID.Label, 1);
		AddIntField(&m_nAccountLinkDays, WebPaymentOption::AccountLinkDays.Label, 0x3E);
		AddTextField(&m_strAccountLinkStart, WebPaymentOption::AccountLinkStart.Label, "00:00");
		AddTextField(&m_strAccountLinkEnd, WebPaymentOption::AccountLinkEnd.Label, "23:59");
		AddTextField(&m_strAccountLinkLastDate, WebPaymentOption::AccountLinkLastDate.Label, "");
		AddTextField(&m_strAccountLinkLastTime, WebPaymentOption::AccountLinkLastTime.Label, "");
		AddIntField(&m_nPaymentDays, WebPaymentOption::PaymentDays.Label, 0x3E);
		AddTextField(&m_strPaymentStart, WebPaymentOption::PaymentStart.Label, "00:00");
		AddTextField(&m_strPaymentEnd, WebPaymentOption::PaymentEnd.Label, "23:59");
		AddTextField(&m_strPaymentLastDate, WebPaymentOption::PaymentLastDate.Label, "");
		AddTextField(&m_strPaymentLastTime, WebPaymentOption::PaymentLastTime.Label, "");
		AddTextField(&m_strBalanceLastDate, WebPaymentOption::BalanceLastDate.Label, "");
		AddTextField(&m_strBalanceLastTime, WebPaymentOption::BalanceLastTime.Label, "");
		AddTextField(&m_strTransactionLastDate, WebPaymentOption::TransactionLastDate.Label, "");
		AddTextField(&m_strTransactionLastTime, WebPaymentOption::TransactionLastTime.Label, "");
		AddBOOLField(&m_bExclude1Flag, WebPaymentOption::Exclude1Flag.Label, FALSE);
		AddTextField(&m_strExclude1Start, WebPaymentOption::Exclude1Start.Label, "00:00");
		AddTextField(&m_strExclude1End, WebPaymentOption::Exclude1End.Label, "00:00");
		AddBOOLField(&m_bExclude2Flag, WebPaymentOption::Exclude2Flag.Label, FALSE);
		AddTextField(&m_strExclude2Start, WebPaymentOption::Exclude2Start.Label, "00:00");
		AddTextField(&m_strExclude2End, WebPaymentOption::Exclude2End.Label, "00:00");
		AddBOOLField(&m_bExclude3Flag, WebPaymentOption::Exclude3Flag.Label, FALSE);
		AddTextField(&m_strExclude3Start, WebPaymentOption::Exclude3Start.Label, "00:00");
		AddTextField(&m_strExclude3End, WebPaymentOption::Exclude3End.Label, "00:00");
		AddBOOLField(&m_bExclude4Flag, WebPaymentOption::Exclude4Flag.Label, FALSE);
		AddTextField(&m_strExclude4Start, WebPaymentOption::Exclude4Start.Label, "00:00");
		AddTextField(&m_strExclude4End, WebPaymentOption::Exclude4End.Label, "00:00");
		AddTextField(&m_strExcludeUploadStart, WebPaymentOption::ExcludeUploadStart.Label, "00:00");
		AddTextField(&m_strExcludeUploadEnd, WebPaymentOption::ExcludeUploadEnd.Label, "00:00");
		AddBOOLField(&m_bEODAfterPaymentFlag, WebPaymentOption::EODAfterPaymentFlag.Label, TRUE);
		AddTextField(&m_strEODAfterTime, WebPaymentOption::EODAfterTime.Label, "16:00");
		AddBOOLField(&m_bPromptBeforeAccessFlag, WebPaymentOption::PromptBeforeAccessFlag.Label, TRUE);
		AddBOOLField(&m_bImportAccountsFlag, WebPaymentOption::ImportAccountsFlag.Label, TRUE);
	}
}

//**********************************************************************

void CSQLRecordSetMetadataWebPaymentOptions::AddParamsByLineID(const int nLineID)
{
	AddIntParam(WebPaymentOption::LineID.Label, nLineID);
}

//**********************************************************************

CSQLRecordSetWebPaymentOptions::CSQLRecordSetWebPaymentOptions(CDatabase* pDatabase, RSParams_WebPaymentOptions_NormalNoParams params) :
	CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetWebPaymentOptions::CSQLRecordSetWebPaymentOptions(CDatabase* pDatabase, RSParams_WebPaymentOptions_NormalByLineId params, bool bForExistenceCheck) :
	CSQLRecordSetConnectionPool(pDatabase),
	m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByLineID(params.nLineId);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetWebPaymentOptions::CommonInit()
{
	m_bForExistenceCheck = m_MD.m_bForExistenceCheck;
	m_nFields = m_MD.GetFieldCount();
	m_nParams = m_MD.GetParamCount();

	if (FALSE == m_bForExistenceCheck)
	{
		m_nFields++;
	}

	SetMetadata(&m_MD);
	SetInitialFilterString();
}

//**********************************************************************

void CSQLRecordSetWebPaymentOptions::DoFieldExchange(CFieldExchange* pFX)
{
	ExchangeFields(pFX);

	if (FALSE == m_bForExistenceCheck)
	{
		RFX_Binary(pFX, Account::RowVersion.Label, m_arrayRowVersion);
	}

	ExchangeParams(pFX);
}

//**********************************************************************

CString CSQLRecordSetWebPaymentOptions::GetDefaultSQL()
{
    return SQLTableNames::WebPaymentOptions;
}

//**********************************************************************

void CSQLRecordSetWebPaymentOptions::LoadFromWebPaymentOptionsRow(CSQLRowWebPaymentOptions& RowPB)
{
    m_MD.m_nLineID = RowPB.GetLineID();
	m_MD.m_nAccountLinkDays = RowPB.GetAccountLinkDays();
	m_MD.m_strAccountLinkStart = RowPB.GetAccountLinkStart();
	m_MD.m_strAccountLinkEnd = RowPB.GetAccountLinkEnd();
	m_MD.m_strAccountLinkLastDate = RowPB.GetAccountLinkLastDate();
	m_MD.m_strAccountLinkLastTime = RowPB.GetAccountLinkLastTime();
	m_MD.m_nPaymentDays = RowPB.GetPaymentDays();
	m_MD.m_strPaymentStart = RowPB.GetPaymentStart();
	m_MD.m_strPaymentEnd = RowPB.GetPaymentEnd();
	m_MD.m_strPaymentLastDate = RowPB.GetPaymentLastDate();
	m_MD.m_strPaymentLastTime = RowPB.GetPaymentLastTime();
	m_MD.m_strBalanceLastDate = RowPB.GetBalanceLastDate();
	m_MD.m_strBalanceLastTime = RowPB.GetBalanceLastTime();
	m_MD.m_strTransactionLastDate = RowPB.GetTransactionLastDate();
	m_MD.m_strTransactionLastTime = RowPB.GetTransactionLastTime();
	m_MD.m_bExclude1Flag = RowPB.GetExclude1Flag();
	m_MD.m_strExclude1Start = RowPB.GetExclude1Start();
	m_MD.m_strExclude1End = RowPB.GetExclude1End();
	m_MD.m_bExclude2Flag = RowPB.GetExclude2Flag();
	m_MD.m_strExclude2Start = RowPB.GetExclude2Start();
	m_MD.m_strExclude2End = RowPB.GetExclude2End();
	m_MD.m_bExclude3Flag = RowPB.GetExclude3Flag();
	m_MD.m_strExclude3Start = RowPB.GetExclude3Start();
	m_MD.m_strExclude3End = RowPB.GetExclude3End();
	m_MD.m_bExclude4Flag = RowPB.GetExclude4Flag();
	m_MD.m_strExclude4Start = RowPB.GetExclude4Start();
	m_MD.m_strExclude4End = RowPB.GetExclude4End();
	m_MD.m_strExcludeUploadStart = RowPB.GetExcludeUploadStart();
	m_MD.m_strExcludeUploadEnd = RowPB.GetExcludeUploadEnd();
	m_MD.m_bEODAfterPaymentFlag = RowPB.GetEODAfterPaymentFlag();
	m_MD.m_strEODAfterTime = RowPB.GetEODAfterTime();
	m_MD.m_bPromptBeforeAccessFlag = RowPB.GetPromptBeforeAccessFlag();
	m_MD.m_bImportAccountsFlag = RowPB.GetImportAccountsFlag();
}

//**********************************************************************

void CSQLRecordSetWebPaymentOptions::SaveToWebPaymentOptionsRow(CSQLRowWebPaymentOptions& RowPB)
{
    RowPB.SetLineID(m_MD.m_nLineID);
	RowPB.SetAccountLinkDays(m_MD.m_nAccountLinkDays);
	RowPB.SetAccountLinkStart(m_MD.m_strAccountLinkStart);
	RowPB.SetAccountLinkEnd(m_MD.m_strAccountLinkEnd);
	RowPB.SetAccountLinkLastDate(m_MD.m_strAccountLinkLastDate);
	RowPB.SetAccountLinkLastTime(m_MD.m_strAccountLinkLastTime);
	RowPB.SetPaymentDays(m_MD.m_nPaymentDays);
	RowPB.SetPaymentStart(m_MD.m_strPaymentStart);
	RowPB.SetPaymentEnd(m_MD.m_strPaymentEnd);
	RowPB.SetPaymentLastDate(m_MD.m_strPaymentLastDate);
	RowPB.SetPaymentLastTime(m_MD.m_strPaymentLastTime);
	RowPB.SetBalanceLastDate(m_MD.m_strBalanceLastDate);
	RowPB.SetBalanceLastTime(m_MD.m_strBalanceLastTime);
	RowPB.SetTransactionLastDate(m_MD.m_strTransactionLastDate);
	RowPB.SetTransactionLastTime(m_MD.m_strTransactionLastTime);
	RowPB.SetExclude1Flag(m_MD.m_bExclude1Flag);
	RowPB.SetExclude1Start(m_MD.m_strExclude1Start);
	RowPB.SetExclude1End(m_MD.m_strExclude1End);
	RowPB.SetExclude2Flag(m_MD.m_bExclude2Flag);
	RowPB.SetExclude2Start(m_MD.m_strExclude2Start);
	RowPB.SetExclude2End(m_MD.m_strExclude2End);
	RowPB.SetExclude3Flag(m_MD.m_bExclude3Flag);
	RowPB.SetExclude3Start(m_MD.m_strExclude3Start);
	RowPB.SetExclude3End(m_MD.m_strExclude3End);
	RowPB.SetExclude4Flag(m_MD.m_bExclude4Flag);
	RowPB.SetExclude4Start(m_MD.m_strExclude4Start);
	RowPB.SetExclude4End(m_MD.m_strExclude4End);
	RowPB.SetExcludeUploadStart(m_MD.m_strExcludeUploadStart);
	RowPB.SetExcludeUploadEnd(m_MD.m_strExcludeUploadEnd);
	RowPB.SetEODAfterPaymentFlag(m_MD.m_bEODAfterPaymentFlag);
	RowPB.SetEODAfterTime(m_MD.m_strEODAfterTime);
	RowPB.SetPromptBeforeAccessFlag(m_MD.m_bPromptBeforeAccessFlag);
	RowPB.SetImportAccountsFlag(m_MD.m_bImportAccountsFlag);	
	/*****/
	RowPB.SetRowVersion(m_arrayRowVersion);
}

//**********************************************************************

