#pragma once
/**********************************************************************/

namespace Dept
{
	const recInt		DeptNo				= { "DeptNo", 0, 999 };
	const recInt		VersionNo			= { "VersionNo", 1, 5 };
	const recBool		ReportEnable		= { "ReportEnable" };
	const recInt		EposGroup			= { "EposGroup", 0, 99 };
	const recInt		ReportGroup			= { "ReportGroup", 0, 99 };
	const recString		RepText				= { "RepText", 0, 32 };
	const recString		EposText			= { "EposText", 0, 32 };
	const recInt		TicketsMode			= { "TicketsMode", 0, 2 };
	const recInt		Tickets				= { "Tickets", 0, 99 };
	const recBool		DPQty				= { "DPQty" };
	const recBool		Negative			= { "Negative" };
	const recBool		NonAdd				= { "NonAdd" };
	const recBool		IDraughtExport		= { "IDraughtExport" };
	const recInt64		SpecimenLearnPlu	= { "SpecimenLearnPlu", 0, 99999999999999 };
	const recBool		StockTemplate		= { "StockTemplate" };
	const recBool		StockDescription	= { "StockDescription" };
	const recBool		StockCategory		= { "StockCategory" };
	const recBool		StockQty			= { "StockQty" };
	const recBool		SportsBooker		= { "SportsBooker" };
	const recString		ImageFilename		= { "ImageFilename", 0, 40 };
	const recInt		LoyaltyBonusType	= { "LoyaltyBonusType", 0, 2 };
	const recDouble		LoyaltyBonusTrip	= { "LoyaltyBonusTrip", 0, 999999.99 };
	const recInt		LoyaltyBonusPoints = { "LoyaltyBonusPoints", 0, 999999 };
	const recDouble		LoyaltyBonusValue	= { "LoyaltyBonusValue", 0, 999999.99 };
	const recInt		LoyaltyBonusPercent	= { "LoyaltyBonusPercent", 0, 100 };
};

/**********************************************************************/
