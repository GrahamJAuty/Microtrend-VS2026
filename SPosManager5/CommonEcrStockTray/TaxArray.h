#pragma once
/**********************************************************************/
#define MAX_TAX_BANDS 12
#define MAX_TAX_TEXTLEN 20
/**********************************************************************/

struct CTaxRateInfo
{
public:
	CTaxRateInfo();

public:
	void Reset();

public:
	double m_dTaxRate;
	CString m_strReportText;
};

/**********************************************************************/

class CTaxArray : public CSharedStringArray
{
public:
	CTaxArray();

public:
	int Open( const char* szFilenameNew, int nMode );
	void PrepareForWrite();

public:
	void GetTaxRateInfo( int nRateNo, CTaxRateInfo& info );
	void SetTaxRateInfo( int nRateNo, CTaxRateInfo& info );

public:
	static int GetNumericTaxBand(CString strBand, bool bSingleCharOnly = TRUE, int nOther = 0, int nBlank = 0);
	static CString GetTaxBandFromNumber(int nBand, int nMinBand = -1, const char* szUnknown = "?", const char* szZero = "0" );

private:
	CTaxRateInfo m_TaxRates[ MAX_TAX_BANDS ];
};

/**********************************************************************/
