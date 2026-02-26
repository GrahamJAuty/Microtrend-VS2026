/**********************************************************************/
#include "TaxArray.h"
/**********************************************************************/

CTaxRateInfo::CTaxRateInfo()
{
	Reset();
}

/**********************************************************************/

void CTaxRateInfo::Reset()
{
	m_dTaxRate = 0.0;
	m_strReportText = "";
}

/**********************************************************************/

CTaxArray::CTaxArray()
{
}

/**********************************************************************/

int CTaxArray::Open( const char* szFilename, int nMode )
{
	//CLEAR EXISTING DATA
	RemoveAt( 0, GetSize() );
	for ( int n = 1; n <= MAX_TAX_BANDS; n++ )
		m_TaxRates[ n - 1 ].Reset();
	
	//READ DATA FROM NEW TAX FILE
	int nReply = CSharedStringArray::Open ( szFilename, nMode );
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	//COPY VALUES FROM FILE TO ARRAY IN MEMORY
	for ( int nIndex = 0; ( nIndex < GetSize() ) && ( nIndex < MAX_TAX_BANDS ); nIndex++ )
	{
		CCSV csvTax( GetAt( nIndex ) );

		if ( csvTax.GetSize() == 1 )
			m_TaxRates[nIndex].m_dTaxRate = csvTax.GetDouble(0);
		else
		{
			switch( csvTax.GetInt(0) )
			{
			case 1:
				m_TaxRates[nIndex].m_dTaxRate = csvTax.GetDouble(1);
				m_TaxRates[nIndex].m_strReportText = csvTax.GetString(2);
				break;
			}
		}
	}

	return nReply;
}

/**********************************************************************/

void CTaxArray::PrepareForWrite()
{
	RemoveAt( 0, GetSize() );

	for ( int nTaxRate = 1; nTaxRate <= MAX_TAX_BANDS; nTaxRate++ )
	{
		CTaxRateInfo TaxRateInfo;
		GetTaxRateInfo( nTaxRate, TaxRateInfo );

		CCSV csvTax;
		csvTax.Add(1);
		csvTax.Add( TaxRateInfo.m_dTaxRate, 4 );
		csvTax.Add( TaxRateInfo.m_strReportText );
		Add( csvTax.GetLine() );
	}
}

/**********************************************************************/

void CTaxArray::GetTaxRateInfo ( int nRateNo, CTaxRateInfo& info )
{
	info.Reset();

	if ( 0 == nRateNo )
	{
		info.m_dTaxRate = 0.0;
		info.m_strReportText = "Unknown / Zero";
	}

	if ( ( nRateNo >= 1 ) && ( nRateNo <= MAX_TAX_BANDS ) )
	{
		info = m_TaxRates[ nRateNo - 1 ];

		if ( info.m_strReportText == "" )
		{
			info.m_strReportText = "Band A";
			info.m_strReportText.SetAt( 5, 'A' + nRateNo - 1 );
		}
	}
}

/**********************************************************************/

void CTaxArray::SetTaxRateInfo ( int nRateNo, CTaxRateInfo& info )
{
	if ( ( nRateNo >= 1 ) && ( nRateNo <= MAX_TAX_BANDS ) )
	{
		m_TaxRates[ nRateNo - 1 ].m_dTaxRate = info.m_dTaxRate;
		m_TaxRates[ nRateNo - 1 ].m_strReportText = info.m_strReportText.Left( MAX_TAX_TEXTLEN );
	}	
}

/**********************************************************************/

int CTaxArray::GetNumericTaxBand(CString strBand, bool bSingleCharOnly, int nOther, int nBlank)
{
	if (strBand == "")
	{
		return nBlank;
	}

	if ((TRUE == bSingleCharOnly) && (strBand.GetLength() > 1))
	{
		return nOther;
	}
	
	strBand.MakeUpper();
	
	{
		char c = strBand[0];
		if ((c >= 'A') && (c <= 'L'))
		{
			return (c - 'A') + 1;
		}

		if (c == '0')
		{
			return 0;
		}
	}

	return nOther;
}

/**********************************************************************/

CString CTaxArray::GetTaxBandFromNumber(int nBand, int nMinBand, const char* szUnknown, const char* szZero)
{
	CString strBand = szUnknown;

	if (nBand >= nMinBand)
	{
		switch (nBand)
		{
		case -1:	strBand = "-";		break;
		case 0:		szZero;				break;
		case 1:		strBand = "A";		break;
		case 2:		strBand = "B";		break;
		case 3:		strBand = "C";		break;
		case 4:		strBand = "D";		break;
		case 5:		strBand = "E";		break;
		case 6:		strBand = "F";		break;
		case 7:		strBand = "G";		break;
		case 8:		strBand = "H";		break;
		case 9:		strBand = "I";		break;
		case 10:	strBand = "J";		break;
		case 11:	strBand = "K";		break;
		case 12:	strBand = "L";		break;
		}
	}

	return strBand;
}

/**********************************************************************/
