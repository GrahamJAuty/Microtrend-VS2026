#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SportsBookerTransaction.h"
#include "XMLWrangler.h"
/**********************************************************************/

class CSportsBookerXMLWrangler : public CXMLWrangler
{
public:
	CSportsBookerXMLWrangler( int nNetworkIdx, CSSXMLParser& Parser );

public:
	void GetTransaction( int nTranIdx, CSportsBookerTransaction& Transaction );
	void WriteCashRSPFile( CSportsBookerTransaction& Transaction );
	
private:
	void GetDeptAndGroupNo( const char* szXMLDeptNo, int& nDeptNo, int& nGroupNo );
};

/**********************************************************************/
#endif
/**********************************************************************/
