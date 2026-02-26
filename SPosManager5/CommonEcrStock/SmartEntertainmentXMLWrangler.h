#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "ReportConsolidationArray.h"
#include "SmartEntertainmentTransaction.h"
#include "XMLWrangler.h"
/**********************************************************************/

class CSmartEntertainmentXMLWrangler : public CXMLWrangler
{
public:
	CSmartEntertainmentXMLWrangler( int nNetworkIdx, CSSXMLParser& Parser );

public:
	int GetPaymentCount( int nTranIdx );
	void GetTransaction( int nTranIdx, int nPayIdx, CSmartEntertainmentTransaction& Transaction );
	void WriteCashRSPFile( CSmartEntertainmentTransaction& Transaction, CReportConsolidationArray<CSortedStringItem>& arrayTrans, bool& bDuplicate );

private:
	CXMLNodeArray m_arPayment;
	int m_nPaymentCount;
};

/**********************************************************************/
#endif
/**********************************************************************/
