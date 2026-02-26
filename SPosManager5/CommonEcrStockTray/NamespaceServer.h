/**********************************************************************/
#pragma once
/**********************************************************************/

namespace Server
{
	const recInt		ServerNo =		{ "ServerNo", 1, 9999 };
	const recInt		VersionNo =		{ "VersionNo", 1, 6 };
	const recString		TimeStamp =		{ "TimeStamp", 0, 20 };
	const recString		ReceiptName =	{ "ReceiptName", 1, 40 };
	const recString		FullName =		{ "FullName", 1, 80 };
	const recInt		Authority =		{ "Authority", 0, 9 };
	const recInt		Password =		{ "Password", 0, 2147483647 };
	const recInt		Drawer =		{ "Drawer", 0, 2 };
	const recBool		FlipView =		{ "FlipView" };
	const recBool		TrainingMode =	{ "TrainingMode" };
	const recInt		LogonMacro	=	{ "LogonMacro", 0, 9999 };
	const recInt		OpenTableMode =	{ "OpenTableMode", 1, 2 };
	const recInt		HomeNwkLocNo =	{ "HomeNwkLocNo", 0, 999999 };
	const recDouble		HourlyRate =	{ "HourlyRate", 0.0, 999.99 };
	const recInt		DailyHours =	{ "DailyHours", 1, 24 };
	const recBool		MidnightShift = { "MidnightShift" };
	const recInt		TradingColourScheme = { "TradingColourScheme", 0, 10 };
};

/**********************************************************************/
