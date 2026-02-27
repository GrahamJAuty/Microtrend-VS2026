//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryWebPaymentOptions.h"
//**********************************************************************

CProjectSQLRepositoryWebPaymentOptions::CProjectSQLRepositoryWebPaymentOptions() : CSQLRepositoryWebPaymentOptions()
{
	m_strImportTableName = SQLTableNames::WebPaymentOptions;
}

//**********************************************************************

bool CProjectSQLRepositoryWebPaymentOptions::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, int nDbVersion, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Web Payment Options Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Web Payment Options Table");

	if (nDbVersion >= 16)
	{
		fileIn.Seek(0, CFile::begin);

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

		CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

		CString strBuffer = "";
		while (SQLIn.ReadLine(strBuffer))
		{
			CCSV csvIn(strBuffer);

			CSQLRepositoryWebPaymentOptions Options;
			Options.m_Row.SetLineID(csvIn.GetInt(0));
			Options.m_Row.SetAccountLinkDays(csvIn.GetInt(1));
			Options.m_Row.SetAccountLinkStart(csvIn.GetString(2));
			Options.m_Row.SetAccountLinkEnd(csvIn.GetString(3));
			Options.m_Row.SetAccountLinkLastDate(csvIn.GetString(4));
			Options.m_Row.SetAccountLinkLastTime(csvIn.GetString(5));
			Options.m_Row.SetPaymentDays(csvIn.GetInt(6));
			Options.m_Row.SetPaymentStart(csvIn.GetString(7));
			Options.m_Row.SetPaymentEnd(csvIn.GetString(8));
			Options.m_Row.SetPaymentLastDate(csvIn.GetString(9));
			Options.m_Row.SetPaymentLastTime(csvIn.GetString(10));
			Options.m_Row.SetBalanceLastDate(csvIn.GetString(11));
			Options.m_Row.SetBalanceLastTime(csvIn.GetString(12));
			Options.m_Row.SetTransactionLastDate(csvIn.GetString(13));
			Options.m_Row.SetTransactionLastTime(csvIn.GetString(14));
			Options.m_Row.SetExclude1Flag(csvIn.GetBool(15));
			Options.m_Row.SetExclude1Start(csvIn.GetString(16));
			Options.m_Row.SetExclude1End(csvIn.GetString(17));
			Options.m_Row.SetExclude2Flag(csvIn.GetBool(18));
			Options.m_Row.SetExclude2Start(csvIn.GetString(19));
			Options.m_Row.SetExclude2End(csvIn.GetString(20));
			Options.m_Row.SetExclude3Flag(csvIn.GetBool(21));
			Options.m_Row.SetExclude3Start(csvIn.GetString(22));
			Options.m_Row.SetExclude3End(csvIn.GetString(23));
			Options.m_Row.SetExclude4Flag(csvIn.GetBool(24));
			Options.m_Row.SetExclude4Start(csvIn.GetString(25));
			Options.m_Row.SetExclude4End(csvIn.GetString(26));
			Options.m_Row.SetExcludeUploadStart(csvIn.GetString(27));
			Options.m_Row.SetExcludeUploadEnd(csvIn.GetString(28));
			Options.m_Row.SetEODAfterPaymentFlag(csvIn.GetBool(29));
			Options.m_Row.SetEODAfterTime(csvIn.GetString(30));
			Options.m_Row.SetPromptBeforeAccessFlag(csvIn.GetBool(31));
			Options.m_Row.SetImportAccountsFlag(csvIn.GetBool(32));

			if (Options.SaveRow(pDatabase) != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}

			WorkingDlg.IncrementRecordsChecked(10, FALSE);
		}

		return TRUE;
	}
	else
	{
		return (ImportLegacyOptions(pDatabase));
	}
}

//**********************************************************************
