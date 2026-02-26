#pragma once
/**********************************************************************/
#include "CustomerSelectionListAccount.h"
#include "CustomerSelectionListOneOff.h"
/**********************************************************************/

class CCustomerNameTable
{
public:
	CCustomerNameTable(void){}

public:
	void BuildList();
	void ClearList();
	void BuildSortOrder();
	void GetCustomerStatus( CCustomerSelectionListAccountInfo& info, int& nType, int& nPos );
	
public:
	CCustomerSelectionListAccount Account;
	CCustomerSelectionListOneOff OneOff;
};

/**********************************************************************/
