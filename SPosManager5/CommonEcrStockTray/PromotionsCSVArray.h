#pragma once
/**********************************************************************/
#include "MinimumSPOSVersion.h"
/**********************************************************************/
#define PROMOTIONS_VERSION 11
/**********************************************************************/
#define PROMOTION_TYPE_DISCOUNT 0
#define PROMOTION_TYPE_SURCHARGE 1
#define PROMOTION_TYPE_MARKDOWN 2
/**********************************************************************/
#define PROMO_REPORTASPLU_ALWAYS 0
#define PROMO_REPORTASPLU_NONVATABLE 1
#define PROMO_REPORTASPLU_NEVER 2
/**********************************************************************/

namespace Promotions
{
	const recInt PromoNo				= { "PromoNo", 1, 99 };
	const recString Description			= { "Description", 0 ,20 };
	const recInt PromoType				= { "PromoType", 0, 2 };
	const recInt FlexType				= { "FlexType", 0, 2 };
	const recDouble Percent				= { "Percent", -999.99, 999.99 };
	const recDouble Markdown			= { "Markdown", 0.0, 999999.99 };
	const recDouble Trip				= { "Trip", 0.0, 999999.99 };
	const recBool Gratuity				= { "Gratuity" };
	const recBool SPOSApplyTax			= { "SPOSApplyTax" };
	const recInt Usage					= { "Usage" };
	const recInt DeptNoEpos				= { "DeptNoEpos", 0, 999 };
	const recBool MyPromoOnPromo		= { "MyPromoOnPromo" };
	const recBool PromoOnMyPromo		= { "PromoOnMyPromo" };
	const recInt TimeplanMode			= { "TimeplanMode", 0, 2 };
	const recInt UseGate				= { "UseGate", 0, 200 };
	const recInt ReportAsPluType		= { "ReportAsPluType", 0, 2 };
	const recInt DeptNoReport			= { "DeptNoReport", 0, 999 };
	const recInt FilterType				= { "FilterType", 0, 2 };
	const recInt FilterOfferGroup		= { "FilterOfferGroup", 0, 999 };
	const recString FilterAnalysis		= { "FilterAnalysis", 0, 300 };
	const recBool ReportAsZeroQty		= { "ReportAsZeroQty" };
	const recInt SageNominalCode		= { "SageNominalCode", 0, 99999999 };
	const recBool Reasons				= { "Reasons" };
	const recBool ServiceChargeVATFix	= { "ServiceChangeVATFix" };
	const recBool DiscountAlwaysVATable = { "DiscountAlwaysVATable" };
	const recInt FlexBoundCovers		= { "FlexBoundCovers", 0, 9999 };
	const recDouble FlexBoundValue		= { "FlexBoundValue", 0, 9999999.99 };
	const recBool UpTo					= { "UpTo" };
	const recString TimeplanList		= { "TimeplanList", 0, 200 };	
};

/**********************************************************************/

class CPromotionsCSVRecord : public CCSVRecord
{
public:
	CPromotionsCSVRecord(); 
	virtual ~CPromotionsCSVRecord() {}

public:
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	int GetPromoNo() { return m_nPromoNo; }
	const char* GetDescription() { return m_strDescription; }
	int GetPromoType() { return m_nPromoType; }
	int GetFlexType() { return m_nFlexType; }
	double GetPercent() { return m_dPercent; }
	double GetMarkdown() { return m_dMarkdown; }
	double GetTrip() { return m_dTrip; }
	bool GetGratuityFlag() { return m_bGratuity; }
	bool GetSPOSApplyTaxFlag() { return m_bSPOSApplyTax; }
	int GetUsage() { return m_nUsage; }
	int GetDeptNoEpos() { return m_nDeptNoEpos; }
	bool GetMyPromoOnPromoFlag(){ return m_bMyPromoOnPromo; }
	bool GetPromoOnMyPromoFlag(){ return m_bPromoOnMyPromo; }
	int GetTimeplanMode(){ return m_nTimeplanMode; }
	int GetUseGate(){ return m_nUseGate; }
	int GetFilterType(){ return m_nFilterType; }
	int GetFilterOfferGroup(){ return m_nFilterOfferGroup; }
	const char* GetFilterAnalysis(){ return m_strFilterAnalysis; }
	bool GetLegacyAnalysis( int& nFlags );
	bool GetServiceChargeVATFixFlag() { return m_bServiceChargeVATFix; }
	bool GetDiscountAlwaysVATableFlag() { return m_bDiscountAlwaysVATable; }
	bool GetUpToFlag() { return m_bUpTo; }
	CString GetTimeplanList() { return m_strTimeplanList; }	

	void GetTimeplanMap(CReportConsolidationArray<CSortedIntByInt>& TimeplanMap);

	int GetFlexBoundCovers( int nBand );
	double GetFlexBoundValue( int nBand );
	double GetFlexPercent( int nBand );

	int GetReportAsPluType(){ return m_nReportAsPluType; }
	int GetDeptNoReport() { return m_nDeptNoReport; }
	bool GetReportAsZeroQtyFlag(){ return m_bReportAsZeroQty; }
	int GetSageNominalCode(){ return m_nSageNominalCode; }
	bool GetReasonsFlag(){ return m_bReasons; }

	const char* GetDisplayName();
	const char* GetPromoTypeName();
	const char* GetFlexTypeName();

	const char* GetAnalysisList( bool bForDownload );
	void ValidateFlexSettings();
	void ValidateUpToFlag();

public:
	void CheckMinimumSPOSVersion( CMinimumSPOSVersion& Version, int& nLegacyCats );
	
public:
	void SetPromoNo( int n );
	void SetDescription( const char* sz );
	void SetPromoType( int n );
	void SetFlexType( int n );
	void SetPercent( double d );
	void SetMarkdown( double d );
	void SetTrip( double d );
	void SetGratuityFlag( bool b );
	void SetSPOSApplyTaxFlag( bool b );
	void SetUsage( int n );
	void SetDeptNoEpos( int n );
	void SetMyPromoOnPromoFlag( bool b );
	void SetPromoOnMyPromoFlag( bool b );
	void SetTimeplanMode( int n );
	void SetUseGate( int n );
	void SetFilterType( int n );
	void SetFilterOfferGroup( int n );
	void SetFilterAnalysis( const char* sz );
	void SetLegacyAnalysis( int n );
	void SetServiceChargeVATFixFlag(bool b) { m_bServiceChargeVATFix = b; }
	void SetDiscountAlwaysVATableFlag(bool b) { m_bDiscountAlwaysVATable = b; }
	void SetUpToFlag(bool b) { m_bUpTo = b; }
	void SetTimeplanList(const char* sz);
	
	void SetFlexBoundCovers	( int nBand, int n );
	void SetFlexBoundValue	( int nBand, double d  );
	void SetFlexPercent		( int nBand, double d );

	void SetReportAsPluType( int n );
	void SetDeptNoReport( int n );
	void SetReportAsZeroQtyFlag( bool b );
	void SetSageNominalCode( int n );
	void SetReasonsFlag( bool b );

	void ClrServiceChargeVATFixBeforeEditing() { m_bServiceChargeVATFixBeforeEditing = FALSE; }
	void TagServiceChargeVATFixBeforeEditing() { m_bServiceChargeVATFixBeforeEditing = m_bServiceChargeVATFix; }
	bool GetServiceChargeVATFixBeforeEditing() { return m_bServiceChargeVATFixBeforeEditing; }

	void SetServiceChargeVATFixFields();

private:
	void ClearRecord();
	void V1to3ConvertFromCSV( CCSV& csv, int nVer );
	void V4to6ConvertFromCSV( CCSV& csv, int nVer );
	void V7to11ConvertFromCSV( CCSV& csv, int nVer );
	
private:
	int m_nPromoNo;
	CString m_strDescription;
	int m_nPromoType;
	int m_nFlexType;
	double m_dPercent;
	double m_dMarkdown;
	double m_dTrip;
	bool m_bGratuity;
	bool m_bSPOSApplyTax;
	int m_nUsage;
	int m_nDeptNoEpos;
	bool m_bMyPromoOnPromo;
	bool m_bPromoOnMyPromo;
	int m_nTimeplanMode;
	int m_nUseGate;
	int m_nFilterType;
	int m_nFilterOfferGroup;
	CString m_strFilterAnalysis;
	bool m_bServiceChargeVATFix;
	bool m_bServiceChargeVATFixBeforeEditing;
	bool m_bDiscountAlwaysVATable;
	bool m_bUpTo;
	CString m_strTimeplanList;

	int m_nReportAsPluType;
	int m_nDeptNoReport;
	bool m_bReportAsZeroQty;
	int m_nSageNominalCode;
	bool m_bReasons;

	int m_nFlexBoundCovers[4];
	double m_dFlexBoundValue[4];
	double m_dFlexPercent[5];

	CString m_strDisplayName;
	CString m_strPromoTypeName;
	CString m_strFlexTypeName;
	CString m_strAnalysisList;
};

/**********************************************************************/

class CPromotionsCSVArray : public CSharedCSVArray
{
public:
	CPromotionsCSVArray();
	~CPromotionsCSVArray();
	
public:
	int Open( const char* szFilename, int nMode );

public:
	int GetPromoNoByIndex( int nPromoIdx );
	bool FindTableByNumber( int nTableNo, int& nTableIdx );
	int FindFirstFreeNumber();

public:
	const char* GetTabbedComboText( int nPromoNo );
	const char* GetSimpleListText( int nPromoNo );

private:
	void GetComboTexts( int nPromoNo, CString& strNum, CString& strName );

private:
	CString m_strDisplayName;
	CString m_strTabbedComboText;
	CString m_strSimpleListText;
};

/**********************************************************************/
