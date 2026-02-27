//$$******************************************************************
#include "TaxCodes.h"
//$$******************************************************************

CTaxCodes::CTaxCodes(void)
{
	for (int i = 0; i < nMAX_TAXCODES; i++)
	{
		m_tax[i].dValue = 0;
		m_tax[i].strText = "";
	}
}

//$$******************************************************************

bool CTaxCodes::Read()
{
	bool bReply;

	CSSFile file;
	if ( ( bReply = file.Open ( System.GetEposTaxRateFilename(), "rb" ) ) == TRUE )
	{
		for ( int i = 0 ; i < nMAX_TAXCODES ; i++ )
		{
			CCSV csv;
			if ( file.Read ( csv ) == TRUE )
			{
				if ( csv.GetSize() == 1 )						// value
				{
					m_tax[i].dValue = csv.GetDouble(0);
					m_tax[i].strText.Format ( "Tax %c", i + 'A' );
				}
				else
				{
					int nVersion = csv.GetInt(0);
					
					if ( nVersion == 1 )						// n, value, text
					{
						m_tax[i].dValue = csv.GetDouble(1);
						m_tax[i].strText = csv.GetString(2);
					}
				}
			}
		}

		file.Close();
	}

	return bReply;
}

//$$******************************************************************

double CTaxCodes::CalcVatContent ( double dValue, int nIndex )
{
	double dVatContent = 0;

	double dTaxRate = GetTaxRate ( nIndex );

	if ( dTaxRate != 0 )
	{
		double dExVat = dValue / ( 100 + dTaxRate );
		dExVat *= 100;

		dVatContent = ::RoundDoubleValue ( dValue - dExVat );				// lose rounding errors
	}

	return dVatContent;
}

//$$******************************************************************

double CTaxCodes::GetTaxRate(int nIndex)
{
	double dTaxRate = 0;

	if ((nIndex >= 0) && (nIndex < nMAX_TAXCODES))
	{
		dTaxRate = m_tax[nIndex].dValue;
	}

	return dTaxRate;
}

//$$******************************************************************

CString CTaxCodes::GetTaxText ( int nIndex )
{
	CString strText	= "";

	if ((nIndex >= 0) && (nIndex < nMAX_TAXCODES))
	{
		strText = m_tax[nIndex].strText;
	}

	return strText;
}

//$$******************************************************************
