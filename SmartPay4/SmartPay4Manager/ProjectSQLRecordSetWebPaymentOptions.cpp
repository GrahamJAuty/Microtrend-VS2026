//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_WebPaymentOptions\SQLRepositoryWebPaymentOptions.h"
//**********************************************************************
#include "ProjectSQLRecordSetWebPaymentOptions.h"
//**********************************************************************

CProjectSQLRecordSetWebPaymentOptions::CProjectSQLRecordSetWebPaymentOptions() : CSQLRecordSetWebPaymentOptions(NULL, RSParams_WebPaymentOptions_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetWebPaymentOptions::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)				
{
	CStringArray arrayHeader;
	arrayHeader.Add(WebPaymentOption::LineID.Label);
	arrayHeader.Add(WebPaymentOption::AccountLinkDays.Label);
	arrayHeader.Add(WebPaymentOption::AccountLinkStart.Label);
	arrayHeader.Add(WebPaymentOption::AccountLinkEnd.Label);
	arrayHeader.Add(WebPaymentOption::AccountLinkLastDate.Label);
	arrayHeader.Add(WebPaymentOption::AccountLinkLastTime.Label);
	arrayHeader.Add(WebPaymentOption::PaymentDays.Label);
	arrayHeader.Add(WebPaymentOption::PaymentStart.Label);
	arrayHeader.Add(WebPaymentOption::PaymentEnd.Label);
	arrayHeader.Add(WebPaymentOption::PaymentLastDate.Label);
	arrayHeader.Add(WebPaymentOption::PaymentLastTime.Label);
	arrayHeader.Add(WebPaymentOption::BalanceLastDate.Label);
	arrayHeader.Add(WebPaymentOption::BalanceLastTime.Label);
	arrayHeader.Add(WebPaymentOption::TransactionLastDate.Label);
	arrayHeader.Add(WebPaymentOption::TransactionLastTime.Label);
	arrayHeader.Add(WebPaymentOption::Exclude1Flag.Label);
	arrayHeader.Add(WebPaymentOption::Exclude1Start.Label);
	arrayHeader.Add(WebPaymentOption::Exclude1End.Label);
	arrayHeader.Add(WebPaymentOption::Exclude2Flag.Label);
	arrayHeader.Add(WebPaymentOption::Exclude2Start.Label);
	arrayHeader.Add(WebPaymentOption::Exclude2End.Label);
	arrayHeader.Add(WebPaymentOption::Exclude3Flag.Label);
	arrayHeader.Add(WebPaymentOption::Exclude3Start.Label);
	arrayHeader.Add(WebPaymentOption::Exclude3End.Label);
	arrayHeader.Add(WebPaymentOption::Exclude4Flag.Label);
	arrayHeader.Add(WebPaymentOption::Exclude4Start.Label);
	arrayHeader.Add(WebPaymentOption::Exclude4End.Label);
	arrayHeader.Add(WebPaymentOption::ExcludeUploadStart.Label);
	arrayHeader.Add(WebPaymentOption::ExcludeUploadEnd.Label);
	arrayHeader.Add(WebPaymentOption::EODAfterPaymentFlag.Label);
	arrayHeader.Add(WebPaymentOption::EODAfterTime.Label);
	arrayHeader.Add(WebPaymentOption::PromptBeforeAccessFlag.Label);
	arrayHeader.Add(WebPaymentOption::ImportAccountsFlag.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::WebPaymentOptions, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRepositoryWebPaymentOptions Options;
	Options.LoadRow(NULL);

	SQLOut.Add(Options.m_Row.GetLineID());
	SQLOut.Add(Options.m_Row.GetAccountLinkDays());
	SQLOut.Add(Options.m_Row.GetAccountLinkStart());
	SQLOut.Add(Options.m_Row.GetAccountLinkEnd());
	SQLOut.Add(Options.m_Row.GetAccountLinkLastDate());
	SQLOut.Add(Options.m_Row.GetAccountLinkLastTime());
	SQLOut.Add(Options.m_Row.GetPaymentDays());
	SQLOut.Add(Options.m_Row.GetPaymentStart());
	SQLOut.Add(Options.m_Row.GetPaymentEnd());
	SQLOut.Add(Options.m_Row.GetPaymentLastDate());
	SQLOut.Add(Options.m_Row.GetPaymentLastTime());
	SQLOut.Add(Options.m_Row.GetBalanceLastDate());
	SQLOut.Add(Options.m_Row.GetBalanceLastTime());
	SQLOut.Add(Options.m_Row.GetTransactionLastDate());
	SQLOut.Add(Options.m_Row.GetTransactionLastTime());
	SQLOut.Add(Options.m_Row.GetExclude1Flag());
	SQLOut.Add(Options.m_Row.GetExclude1Start());
	SQLOut.Add(Options.m_Row.GetExclude1End());
	SQLOut.Add(Options.m_Row.GetExclude2Flag());
	SQLOut.Add(Options.m_Row.GetExclude2Start());
	SQLOut.Add(Options.m_Row.GetExclude2End());
	SQLOut.Add(Options.m_Row.GetExclude3Flag());
	SQLOut.Add(Options.m_Row.GetExclude3Start());
	SQLOut.Add(Options.m_Row.GetExclude3End());
	SQLOut.Add(Options.m_Row.GetExclude4Flag());
	SQLOut.Add(Options.m_Row.GetExclude4Start());
	SQLOut.Add(Options.m_Row.GetExclude4End());
	SQLOut.Add(Options.m_Row.GetExcludeUploadStart());
	SQLOut.Add(Options.m_Row.GetExcludeUploadEnd());
	SQLOut.Add(Options.m_Row.GetEODAfterPaymentFlag());
	SQLOut.Add(Options.m_Row.GetEODAfterTime());
	SQLOut.Add(Options.m_Row.GetPromptBeforeAccessFlag());
	SQLOut.Add(Options.m_Row.GetImportAccountsFlag());

	SQLOut.WriteInsert();
}

//**********************************************************************
