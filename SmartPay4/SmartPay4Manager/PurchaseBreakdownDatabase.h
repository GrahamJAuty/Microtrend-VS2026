#pragma once

//**********************************************************************

//#include "SystemData.h"

namespace DBPurchaseBreakdown
{
const recString		Orderby		= { "X1", 0, 0 };	// 
const recInt		GroupNo		= { "X2", 0, 0 };	// 
const recString		PluNo		= { "X3", 0, 0 };	// Plu\Dept text
const recString		Text		= { "X4", 0, 0 };	// Plu\Dept text
const recInt		Count		= { "X5", 0, 0 };	//	Transaction count
const recDouble		RefundQty	= { "X6", 0, 0 };	//	Refunded qty
const recDouble		Qty			= { "X7", 0, 0 };	//	sold qty
const recDouble		Value		= { "X8", 0, 0 };	//	sold value
const recInt		Points		= { "X9", 0, 0 };	//  sold qty points
};

//**********************************************************************

class CPurchaseBreakdownDatabase : public CSSStringDatabase
{
public:
	CPurchaseBreakdownDatabase() {}
	int Open ( const char* szFilename, int nMode );
	CString MakeKey ( const char* szOrderBy, int nGroupNo, const char* szText );

	void SetOrderBy ( const char* szText )	{ CSSDatabase::Set ( DBPurchaseBreakdown::Orderby.Label, szText ); }
	const char* GetOrderBy()				{ return CSSDatabase::GetString ( DBPurchaseBreakdown::Orderby.Label ); }

	void SetPluNo ( const char* szText )	{ CSSDatabase::Set ( DBPurchaseBreakdown::PluNo.Label, szText ); }
	const char* GetPluNo()					{ return CSSDatabase::GetString ( DBPurchaseBreakdown::PluNo.Label ); }

	void SetText ( const char* szText )		{ CSSDatabase::Set ( DBPurchaseBreakdown::Text.Label, szText ); }
	const char* GetText()					{ return CSSDatabase::GetString ( DBPurchaseBreakdown::Text.Label ); }

	void SetGroupNo ( int n )				{ CSSDatabase::Set ( DBPurchaseBreakdown::GroupNo.Label, n ); }
	int GetGroupNo()						{ return CSSDatabase::GetInt ( DBPurchaseBreakdown::GroupNo.Label ); }

	void AddRefundQty ( double dValue );
	double GetRefundQty()					{ return CSSDatabase::GetDouble ( DBPurchaseBreakdown::RefundQty.Label ); }

	void AddQty ( double dValue );
	double GetQty()							{ return CSSDatabase::GetDouble ( DBPurchaseBreakdown::Qty.Label ); }

	void AddValue ( double dValue );
	double GetValue()						{ return CSSDatabase::GetDouble ( DBPurchaseBreakdown::Value.Label ); }

	void AddPoints ( int nValue );
	int GetPoints()							{ return CSSDatabase::GetInt ( DBPurchaseBreakdown::Points.Label ); }

	void IncCount();
	int GetCount()							{ return CSSDatabase::GetInt ( DBPurchaseBreakdown::Count.Label ); }
};
 