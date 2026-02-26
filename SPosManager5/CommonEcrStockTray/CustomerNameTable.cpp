/**********************************************************************/
 
/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "CustomerNameTable.h"
/**********************************************************************/

void CCustomerNameTable::BuildList()
{
	Account.BuildList();
	OneOff.BuildList();
}

/**********************************************************************/

void CCustomerNameTable::ClearList()
{
	Account.ClearList();
	OneOff.ClearList();
}

/**********************************************************************/

void CCustomerNameTable::BuildSortOrder()
{
	Account.BuildSortOrder();
	OneOff.BuildSortOrder();
}

/**********************************************************************/

void CCustomerNameTable::GetCustomerStatus( CCustomerSelectionListAccountInfo& info, int& nType, int& nPos )
{
	if ( Account.FindCustomer( info, nPos ) == TRUE )
	{
		Account.GetCustomerStatus( nPos, info );
		nType = NODE_CUSTOMER_ACCOUNT;
	}
	else if ( OneOff.FindCustomer( info, nPos ) == TRUE )
	{
		OneOff.GetCustomerStatus( nPos, info );
		nType = NODE_CUSTOMER_ONEOFF;
	}
	else
	{
		nPos = 0;
		Account.GetCustomerStatus( info );
		nType = NODE_PLACEHOLDER;
	}
}

/**********************************************************************/
