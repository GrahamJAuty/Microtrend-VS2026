/**********************************************************************/
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "ReportHelpers.h"
/**********************************************************************/

double CReportHelpers::CalcAverage ( double dQty, double dValue )
{
	double dAverage = ( dQty == 0 ) ? 0 : (dValue/dQty);
	return dAverage;
}

/**********************************************************************/

double CReportHelpers::CalcNonTax ( double dSalesValue, double dTaxRate )
{
	return dSalesValue / ( 1 + ( dTaxRate / 100.0 ) );
}

/**********************************************************************/

double CReportHelpers::CalcGP ( double dSalesValue, double dTaxRate, double dCostPrice )
{
	double dNonTaxContent = dSalesValue / ( 1 + ( dTaxRate / 100.0 ) );
	double dTaxContent = dSalesValue - dNonTaxContent;
	return dSalesValue - dTaxContent - dCostPrice;
}

/**********************************************************************/

double CReportHelpers::CalcGPPercent ( double dSalesValue, double dTaxRate, double dCostPrice )
{
	double dNonTaxContent = dSalesValue / ( 1 + ( dTaxRate / 100.0 ) );
	
	if ( dNonTaxContent == 0.0 )
		return 0.0;

	double dResult = ( ( dNonTaxContent - dCostPrice ) * 100  ) / dNonTaxContent;
	return ( dResult >= 0.0 ) ? dResult : 0.0;
}

/**********************************************************************/

const char* CReportHelpers::GetReportText ( CPluCSVRecord& PluRecord )
{
	m_strReportText = PluRecord.GetRepText();
	
	if ( m_strReportText != "" )
		return m_strReportText;

	m_strReportText = PluRecord.GetEposText();

	CString strFilter = "";
	
	for ( int nIndex = 0; nIndex < m_strReportText.GetLength(); nIndex++ )
	{
		char c = m_strReportText.GetAt ( nIndex );

		if ( c != '^' )
			strFilter += c;
	}

	m_strReportText = strFilter;
	return m_strReportText;
}

/**********************************************************************/

const char* CReportHelpers::GetDisplayPluNo ( __int64 nPluNo, bool bAllowZero )
{
	::FormatPluNo( nPluNo, m_strDisplayPluNo, FALSE, bAllowZero );

	if ( EcrmanOptions.GetReportsTrimPluZerosFlag() == TRUE )
		::TrimSpaces ( m_strDisplayPluNo, TRUE );
		
	return m_strDisplayPluNo;
}

/**********************************************************************/

UINT CReportHelpers::HashString(LPCTSTR key)
{
	UINT nHash = 0;
	while (*key)
		nHash = (nHash<<5) + nHash + *key++;
	return nHash;
}

/**********************************************************************/

void CReportHelpers::CreateTabRuler( CReportFileDoc& ReportFileDoc, const char* szParamsFilename, const char* szKey, CTabRuler& TabRuler )
{
	CString strKey = szKey;
	if ( strKey != "" )
	{
		UINT nKey = HashString ( strKey );
		strKey.Format ( "%u", nKey );
	}

	TabRuler = ReportFileDoc.GetRuler();			// copy ruler

	CSSFile file;
	if ( file.Open ( szParamsFilename, "rb" ) == TRUE )
	{
		CCSV csv;

		while ( file.Read ( csv ) == TRUE )
		{
			if ( csv.GetInt(0) == DMORIENT_PORTRAIT || csv.GetInt(0) == DMORIENT_LANDSCAPE )	// pre version 1.12 (value = 1 or 2)
			{
				int nColumnCount = csv.GetInt(1);

				if ( TabRuler.GetSize() == nColumnCount )
				{
					if ( strKey == csv.GetString(nColumnCount+2) )
					{
						/*
						if ( csv.GetInt(0) == DMORIENT_LANDSCAPE )
							m_nOrientation = DMORIENT_LANDSCAPE;
						else
							m_nOrientation = DMORIENT_PORTRAIT;
						*/

						if ( csv.GetInt(nColumnCount+3) == nColumnCount && csv.GetSize() == (2*(nColumnCount+1)+2) )	// we have an order array
						{
							for ( int i = 0 ; i < TabRuler.GetSize() ; i++ )
							{
								TabRuler.SetWidth ( i, csv.GetInt(i+2)/3 );
								TabRuler.SetOrder ( i, csv.GetInt(i+nColumnCount+4) );
							}
						}
						else
						{
							for ( int i = 0 ; i < TabRuler.GetSize() ; i++ )
							{
								TabRuler.SetWidth ( i, csv.GetInt(i+2)/3 );
							}
						}
						return;
					}
				}
			}
			else if ( csv.GetInt(0) == 3 )	// version number (v1.12 and above)
			{
				if ( strKey == csv.GetString(1) )
				{
					int nColumnCount = csv.GetInt(3);

					if ( TabRuler.GetSize() == nColumnCount )
					{
						/*
						if ( csv.GetInt(2) == DMORIENT_LANDSCAPE )
							m_nOrientation = DMORIENT_LANDSCAPE;
						else
							m_nOrientation = DMORIENT_PORTRAIT;
						*/

						for ( int i = 0 ; i < nColumnCount ; i++ )
						{
							int nWidth = csv.GetInt(i*3+4)/3;
							int nAlign = csv.GetInt(i*3+5);
							int nOrder = csv.GetInt(i*3+6);

							TabRuler.SetAlign ( i, nAlign );
							TabRuler.SetWidth ( i, nWidth );
							TabRuler.SetOrder ( i, nOrder );
						}
						return;
					}
				}
			}
		}
	}

	TabRuler = ReportFileDoc.GetRuler();
}

/**********************************************************************/

bool CReportHelpers::CanUseSystemPluFilter( int nConType, int nPluFilterNo )
{
	if ( NODE_SYSTEM == nConType )
		return ( DataManagerNonDb.SystemPluFilterItems.IsFilterEmpty( nPluFilterNo ) == FALSE );
	else
		return FALSE;
}

/**********************************************************************/

bool CReportHelpers::CheckFilterPluNo( bool bSystem, int nFilterNo, CSQLPluNoInfo& infoPlu )
{
	if ( FALSE == bSystem )
		return DataManager.DbPluFilterItems.CheckPluNo( nFilterNo, infoPlu );
	else
		return DataManagerNonDb.SystemPluFilterItems.CheckPluNo( nFilterNo, infoPlu );
}

/**********************************************************************/

int CReportHelpers::GetNumericTaxBandFromString( CString strTaxBand )
{
	return CTaxArray::GetNumericTaxBand(strTaxBand, TRUE, REPORT_TAXBAND_UNKNOWN_TAXBAND, REPORT_TAXBAND_UNKNOWN_TAXBAND);
}

/**********************************************************************/

CString CReportHelpers::GetTaxBandString(int nTaxBand)
{
	CString strResult = "";

	if ((nTaxBand >= 1) && (nTaxBand <= MAX_TAX_BANDS))
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo(nTaxBand, TaxRateInfo);
		strResult = TaxRateInfo.m_strReportText;
	}
	else
	{
		switch (nTaxBand)
		{
		case 0:
			strResult = EcrmanOptions.GetTaxString("Non %T items");
			break;

		case REPORT_TAXBAND_NONTAX_DISCOUNT:
			strResult = EcrmanOptions.GetTaxString("Non %T discounts");
			break;

		case REPORT_TAXBAND_UNKNOWN_PLU:
			strResult = "Unknown PLU items";
			break;

		case REPORT_TAXBAND_UNKNOWN_TAXBAND:
		default:
			strResult = "Unknown";
			break;
		}
	}

	return strResult;
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

const char* CReportHelpers::GetDisplayStockCode ( const char* szCode )
{
	m_strDisplayStockCode = szCode;

	if ( StockOptions.GetReportsTrimStockZerosFlag() == TRUE )
		::TrimSpaces ( m_strDisplayStockCode, TRUE );
		
	return m_strDisplayStockCode;
}

/**********************************************************************/
#endif
/**********************************************************************/
