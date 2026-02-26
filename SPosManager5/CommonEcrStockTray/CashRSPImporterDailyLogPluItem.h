#pragma once
/**********************************************************************/

struct CCashRSPImporterDailyLogPluItem
{
public:
	CCashRSPImporterDailyLogPluItem();

public:
	CString m_strLineSale;
	CString m_strLineItemPay;
	CString m_strActionDate;
	CString m_strTaxBand;
	__int64 m_nPluNo;
	int m_nMixMatchType;
	int m_nMixMatchOfferNo;
};

/**********************************************************************/