#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLDefines.h"
/**********************************************************************/

struct EODSale
{
	int nCardCount;
	double dPurse1;
	double dPurse2;
	double dPurse3;
	double dOther;
};

/**********************************************************************/

class CEODGroupSales
{
public:
	CEODGroupSales ( const char* szAuditFile );
	int Extract ( const char* szDateFrom = "", const char* szTimeFrom = "", const char* szDateTo = "", const char* szTimeTo = "" );

public:
	EODSale Sales[nMAXPERIODS][100];

private:
	void Reset();
	void SaveLine ( CSQLAuditRecord* pAtcRecord );

private:
	CString m_strAuditFilename;
};

/**********************************************************************/
