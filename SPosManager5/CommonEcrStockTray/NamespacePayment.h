/**********************************************************************/
#pragma once
/**********************************************************************/

namespace Payment
{
	const recInt		PaymentNo	=				{ "PaymentNo", 1, 50 };
	const recInt		VersionNo =					{ "VersionNo", 1, 12 };
	const recString		PaymentEPOSName =			{ "PaymentEPOSName", 1, 21 };
	const recString		PaymentReportName =			{ "PaymentReportName", 1, 21 };
	const recInt		PaymentType =				{ "PaymentType", 0, 99 };
	const recInt		ReportGroup	=				{ "ReportGroup", 1, 99 };
	const recDouble		Limit =						{ "Limit", 0.0, 999999.9999 };
	const recBool		Tender =					{ "Tender" };
	const recBool		Overtender =				{ "Overtender" };
	const recBool		Change =					{ "Change" };
	const recBool		Gratuity =					{ "Gratuity" };
	const recBool		Drawer =					{ "Drawer" };
	const recBool		EFTCashback =				{ "EFTCashback" };
	const recBool		NonDrawer =					{ "NonDrawer" };
	const recBool		AccountAllow =				{ "AccountAllow" };
	const recBool		CashDeclaration =			{ "CashDeclaration" };
	const recBool		ChangeAsCashback =			{ "ChangeAsCashback" };
	const recBool		Handheld =					{ "Handheld" };
	const recInt		FunctionNo =				{ "FunctionNo", 18, 33 };
	const recInt		ForexNo =					{ "ForexNo", 0, 999 };
	const recBool		CashLimit =					{ "CashLimit" };
	const recInt		DepositChangeType =			{ "DepositChangeType", 0, 2 };
	const recInt		DepositChangeVoucherMin =	{ "DepositChangeVoucherMin", 0, 999 };
	const recString		ExternalParams =			{ "ExternalParams", 0, 40 };
	const recInt		PaymentTextCount =			{ "PaymentTextCount", 0, 20 };
	const recString		PaymentText1 =				{ "PaymentText1", 1, 200 };
	const recString		PaymentText2 =				{ "PaymentText2", 1, 200 };
	const recString		PaymentText3 =				{ "PaymentText3", 1, 200 };
	const recString		PaymentText4 =				{ "PaymentText4", 1, 200 };
	const recString		PaymentText5 =				{ "PaymentText5", 1, 200 };
	const recString		PaymentText6 =				{ "PaymentText6", 1, 200 };
	const recString		PaymentText7 =				{ "PaymentText7", 1, 200 };
	const recString		PaymentText8 =				{ "PaymentText8", 1, 200 };
	const recString		PaymentText9 =				{ "PaymentText9", 1, 200 };
	const recString		PaymentText10 =				{ "PaymentText10", 1, 200 };
	const recString		PaymentText11 =				{ "PaymentText11", 1, 200 };
	const recString		PaymentText12 =				{ "PaymentText12", 1, 200 };
	const recString		PaymentText13 =				{ "PaymentText13", 1, 200 };
	const recString		PaymentText14 =				{ "PaymentText14", 1, 200 };
	const recString		PaymentText15 =				{ "PaymentText15", 1, 200 };
	const recString		PaymentText16 =				{ "PaymentText16", 1, 200 };
	const recString		PaymentText17 =				{ "PaymentText17", 1, 200 };
	const recString		PaymentText18 =				{ "PaymentText18", 1, 200 };
	const recString		PaymentText19 =				{ "PaymentText19", 1, 200 };
	const recString		PaymentText20 =				{ "PaymentText20", 1, 200 };
};

/**********************************************************************/
