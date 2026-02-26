#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpers.h"
/**********************************************************************/

class CDataAccessHelpersCustomer : public CDataAccessHelpers
{
public:
	CDataAccessHelpersCustomer();
	
public:
	void EditCustomerAccount( int nLocIdx );
	void EditCustomerOneOff( int nLocIdx );
	void CustomerTrackTrace( int nLocIdx );

private:
	void EditCustomerInternal( int nLocIdx, int nActionType );
};

/**********************************************************************/
#endif
/**********************************************************************/
