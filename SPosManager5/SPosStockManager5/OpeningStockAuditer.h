#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class COpeningStockAuditer  
{
public:
	COpeningStockAuditer();
};

/**********************************************************************/

class CCSVOpeningStockTemp : public CCSV
{
public:
	CCSVOpeningStockTemp ( const char* szCSVLine ) : CCSV(szCSVLine) {}
	int GetSpNo() { return GetInt(0); }
	const char* GetStockCode() { return GetString(1); }
	const char* GetDescription() { return GetString(2); }
	const char* GetCategory() { return GetString(3); }
	int GetSubUnits() { return GetInt(4); }
	double GetOpeningQty() { return GetDouble(5); }
};

/**********************************************************************/
#endif
/**********************************************************************/

