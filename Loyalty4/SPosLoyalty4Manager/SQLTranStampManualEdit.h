#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRowOfferStamp.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranStampManualEdit : public CSQLTranBase
{
public:
	CSQLTranStampManualEdit(int nAppNo);
	
public:
	void DoWork(CSQLRowAccountFull& RowAccount, CSQLRowOfferStamp& OfferStampOld, CSQLRowOfferStamp& OfferStampNew, CString strOfferName );

public:
	virtual void AfterTransaction();

private:
	int m_nAppNo;
};

/**********************************************************************/
