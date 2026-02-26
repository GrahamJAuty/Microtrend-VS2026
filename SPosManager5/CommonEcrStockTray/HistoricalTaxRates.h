#pragma once
/**********************************************************************/
#include "PMSBookingCSVArray.h"
#include "TaxArray.h"
/**********************************************************************/

struct CTaxDateInfo
{
public:
	CTaxDateInfo();

public:
	CString m_strDate;
	int m_nPMSDayNumber;
	double m_dRate[ MAX_TAX_BANDS ];
};

/**********************************************************************/

class CHistoricalTaxRates : public CSharedStringArray
{
public:
	CHistoricalTaxRates();

	int Open( const char* szFilename, int nMode );

	void BuildTaxDateInfoArray();
	void PrepareForWrite();

	int GetTaxDateCount(){ return m_arrayTaxDateInfo.GetSize(); }
	
public:
	int GetDateCode( const char* szDate );
	
	double GetTaxRate( int nDateCode, int nTaxBand );
	void SetTaxRate( int nDateCode, int nTaxBand, double dRate );
		
	int SetTaxDateInfo( CTaxDateInfo& info );
	void GetTaxDateInfo( int nDateCode, CTaxDateInfo& info );

	void DeleteDate( int nDateCode );

public:
#ifdef COMPILE_PMS
	void SplitPMSDateRange( int nStartDay, int nNights, CArray<CPMSBookingPartInfo,CPMSBookingPartInfo>& arrayParts );
	int GetPMSDateCode( int nPMSDayNumber );
#endif
	
public:
	const char* GetDisplayLine( int nDateCode, bool bForDelete );
		
private:
	bool m_bGotTaxDateInfoArray;
	CArray<CTaxDateInfo,CTaxDateInfo> m_arrayTaxDateInfo;
	
private:
	CString m_strDisplayLine;
};

/**********************************************************************/
