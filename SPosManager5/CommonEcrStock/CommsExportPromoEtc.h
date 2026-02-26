#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsExportBase.h"
#include "SQLOut.h"
/**********************************************************************/
#define DNTYPE_PROMO 0
#define DNTYPE_MIXMATCH 1
#define DNTYPE_WALLETSTAMPRULE 2
#define DNTYPE_ALLOFFER 3
/**********************************************************************/
#define DN_SUBJOB_PROMO 0
#define DN_SUBJOB_MIXMATCH 1
#define DN_SUBJOB_OFFERGROUP 2
#define DN_SUBJOB_OFFERENTRY 3
#define DN_SUBJOB_WALLETSTAMPRULE 4
/**********************************************************************/

class CCommsExportPromoEtc : public CCommsExportBase
{
public:
	CCommsExportPromoEtc( CJobListCSVRecord& JobInfo, bool bFromMemory, int nDownloadType );
	int PrepareFile();
	
private:
	int PrepareSPOSFile( int nSPOSVersion );
	
private:
	void CreateMixMatchHeader(int nSPOSVersion);
	void CreatePromoHeader( int nSPOSVersion, bool bLegacyAnalysis);
	void CreateOfferGroupHeader();
	void CreateOfferGroupEntryHeader();
	void CreateWalletStampRuleHeader(int nSPOSVersion);

private:
	void AppendMixMatch(int nSPOSVersion, CSQLOut& SQLOut);
	void AppendPromos( int nSPOSVersion, bool bLegacyAnalysis, CSQLOut& SQLOut );
	void AppendOfferGroup(CSQLOut& SQLOut);
	void AppendOfferGroupEntry(int nSPOSVersion, CSQLOut& SQLOut);
	void AppendWalletStampRule(int nSPOSVersion, CSQLOut& SQLOut);

private:
	int m_nDownloadType;
	CString m_strMixMatchHeader;
	CString m_strPromoHeader;
	CString m_strOfferGroupHeader;
	CString m_strOfferGroupEntryHeader;
	CString m_strWalletStampRuleHeader;
};

/**********************************************************************/
#endif
/**********************************************************************/

