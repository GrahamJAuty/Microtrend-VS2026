#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "MinimumSPOSVersion.h"
/**********************************************************************/
#define LOYALTYSCHEME_VERSION 1
/**********************************************************************/
#define LOYALTYSCHEME_ACTION_NONE 0
#define LOYALTYSCHEME_ACTION_PROMO_ITEM 1
#define LOYALTYSCHEME_ACTION_PROMO_SUBTOTAL 2
#define LOYALTYSCHEME_ACTION_PRICEBAND 3
#define LOYALTYSCHEME_ACTION_PROMO_MULTIPLE 4
/**********************************************************************/

namespace LoyaltyScheme
{
	const recInt		SchemeNo	=			{ "SchemeNo", 1, 999 };
	const recBool		SchemeActive =			{ "SchemeActive" };
	const recString		SchemeName =			{ "SchemeName", 0, 40 };
	const recString		Format =				{ "Format", 0, 40 };
	const recString		NumberStartLower =		{ "NumberStartLower", 0, 19 };
	const recString		NumberStartUpper =		{ "NumberStartUpper", 0, 19 };
	const recBool		CheckDigit =			{ "CheckDigit" };
	const recBool		StaffCard =				{ "StaffCard" };
	const recBool		StaffCredit =			{ "StaffCredit" };
	const recInt		StaffLimitDay	=		{ "StaffLimitDay", 1, 99999 };
	const recInt		StaffLimitWeek	=		{ "StaffLimitWeek", 1, 99999 };
	const recInt		StaffLimitMonth	=		{ "StaffLimitMonth", 1, 99999 };	//array
	const recInt		PencePerPoint =			{ "PencePerPoint", 0, 999999 };
	const recInt		MacroNo =				{ "MacroNo", 1, 9999 };
	const recString		DeniedPayments =		{ "DeniedPayments", 1, 4000 };
};

/**********************************************************************/

class CLoyaltySchemeCSVRecord : public CCSVRecord
{
public:
	CLoyaltySchemeCSVRecord(); 
	virtual ~CLoyaltySchemeCSVRecord(){}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	void CheckMinimumSPOSVersion( CMinimumSPOSVersion& version );
	
public:
	int GetSchemeNo()					{ return m_nSchemeNo; }
	bool GetActiveFlag()				{ return m_bActive; }
	const char* GetSchemeName()			{ return m_strSchemeName; }
	const char* GetFormat()				{ return m_strFormat; }
	const char* GetNumberStartLower()	{ return m_strNumberStartLower; }
	const char* GetNumberStartUpper()	{ return m_strNumberStartUpper; }
	bool GetCheckDigitFlag()			{ return m_bCheckDigit; }
	bool GetStaffCardFlag()				{ return m_bStaffCard; }
	bool GetStaffCreditFlag()			{ return m_bStaffCredit; }
	int GetStaffLimitDay()				{ return m_nStaffLimitDay; }
	int GetStaffLimitWeek()				{ return m_nStaffLimitWeek; }
	int GetStaffLimitMonth( int nMth );
	int GetActionType()					{ return m_nActionType; }
	int GetSelection()					{ return m_nSelection; }
	int GetPromoNo( int nIdx );
	int GetPencePerPoint()				{ return m_nPencePerPoint; }
	int GetMacroNo()					{ return m_nMacroNo; }
	const char* GetDeniedPayments()		{ return m_strDeniedPayments; }
	
	const char* GetDisplayName();
	void TidyPromos();
	
	void SetSchemeNo( int n );
	void SetActiveFlag( bool b ){ m_bActive = b; }
	void SetSchemeName( const char* sz );
	void SetFormat( const char* sz );
	void SetNumberStartUpper( const char* sz );
	void SetNumberStartLower( const char* sz );
	void SetCheckDigitFlag( bool b ){ m_bCheckDigit = b; }
	void SetStaffCardFlag( bool b ){ m_bStaffCard = b; }
	void SetStaffCreditFlag( bool b ){ m_bStaffCredit = b; }
	void SetStaffLimitDay( int n );
	void SetStaffLimitWeek( int n );
	void SetStaffLimitMonth( int nMth, int n );
	void SetActionTypeAndSelection( int nType, int nSelection );
	void SetPromoNo( int nIdx, int n );
	void SetPencePerPoint( int n );
	void SetMacroNo( int n );
	void SetDeniedPayments( const char* sz );
	
private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );

public:
	const char* GetDeniedPaymentsForVPM();
	void SetDeniedPaymentsFromVPM( const char* sz );
	void CopyDeniedPaymentsToArray( CReportConsolidationArray<CSortedIntWithFlag>& arrayAllow, CReportConsolidationArray<CSortedIntWithFlag>& arrayDeny );
	void CopyDeniedPaymentsFromArray( CReportConsolidationArray<CSortedIntWithFlag>& arrayDeny, bool bIdxArray );

private:
	void GetSafeDeniedPaymentRanges( CArray<CSortedIntAndInt>& arrayRanges );
	void AddPaymentRange( CCSV& csv, int nSeqStart, int nSeqEnd );
	
private:
	int m_nSchemeNo;
	bool m_bActive;
	CString m_strSchemeName;
	CString m_strDisplayName;
	CString m_strFormat;
	CString m_strNumberStartLower;
	CString m_strNumberStartUpper;
	bool m_bCheckDigit;
	bool m_bStaffCard;
	bool m_bStaffCredit;
	int m_nStaffLimitDay;
	int m_nStaffLimitWeek;
	int m_nStaffLimitMonth[12];
	int m_nActionType;
	int m_nSelection;
	int m_nPromoNo[5];
	int m_nPencePerPoint;
	int m_nMacroNo;
	CString m_strDeniedPayments;

private:
	CString m_strPaymentDenyListVPM;
};

/**********************************************************************/

class CLoyaltySchemeCSVArray : public CSharedCSVArray
{
public:
	CLoyaltySchemeCSVArray();
	~CLoyaltySchemeCSVArray();
	int Open( const char* szFilename, int nMode );
	
public:
	bool FindSchemeByNumber( int nSchemeNo, int& nSchemeIdx );
	int FindFirstFreeNumber();

public:
	const char* GetDisplayName( int nSchemeNo );
	
private:
	CString m_strDisplayName;
};

/**********************************************************************/
#endif
/**********************************************************************/
