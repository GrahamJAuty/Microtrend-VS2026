#pragma once
/**********************************************************************/

struct ComboTextInfo
{
	const char* szComboItems;
	const char* szDisplayItems;
};

/**********************************************************************/

namespace ComboTexts
{
	const ComboTextInfo	DeptType =		{ ",Normal,Hash,Gift,Gift Reload", "" };
	const ComboTextInfo	Scale =			{ ",Compulsory,Enable,Inhibit", "" };
	const ComboTextInfo	YesNoBlank =		{ ",No,Yes", "" };
	const ComboTextInfo	YesNoOnly =		{ "No,Yes", "" };
	const ComboTextInfo Sign =			{ ",Normal,Minus", "" };
	const ComboTextInfo StageGroup =		{ ",Appetizer,Main Dish,Dessert,Other", "" };
	const ComboTextInfo SplitPricing =		{ "Split Pricing,Stock Count", "" };
	const ComboTextInfo MenuType =		{ "Normal,Set PLU,Link", "" };
	const ComboTextInfo CondimentEntry =	{ "Non compulsory,Compulsory", "" };
	const ComboTextInfo PriceChange =		{ "Disable,Enable", "" };
	const ComboTextInfo SifSics =			{ ",Normal,SIF,SICS", "" };
	const ComboTextInfo PriceEntry =		{ "Inhibited,Preset Only,Open & Preset,Open", "" };
	const ComboTextInfo ZeroPrice =		{ ",Inhibit,Allow", "" };
	const ComboTextInfo ReceiptType =		{ ",Single,Double", "" };
	const ComboTextInfo ItemValidation =	{ ",Non Compulsory,Compulsory", "" };
	const ComboTextInfo KPType =			{ ",Addition,Single/Double,Addition + Single,Double Addition,Double Add + Single", "" };
	const ComboTextInfo KPColour =		{ ",Black,Red", "" };	
};

/**********************************************************************/

