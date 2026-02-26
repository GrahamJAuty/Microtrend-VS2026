/**********************************************************************/
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ReorderImportHelper.h"
/**********************************************************************/

CReorderImportHelper::CReorderImportHelper(void)
{
	m_bAllowReorder = ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES );
	
	m_bWrapperMode = FALSE;
	m_nFieldIdxMin = -1;
	m_nFieldIdxReorder = -1;
	m_nFieldIdxMax = -1;
	m_nHeaderError = 0;
}

/**********************************************************************/

bool CReorderImportHelper::CheckHeader( const char* szHeader )
{
	bool bAllowPlu = StockOptions.GetImportReorderPluFlag();
	bool bAllowWrp = StockOptions.GetImportReorderWrapperFlag();

	m_bWrapperMode = FALSE;
	m_nFieldIdxMin = -1;
	m_nFieldIdxReorder = -1;
	m_nFieldIdxMax = -1;
	m_nHeaderError = 0;

	CCSV csv( szHeader );
	CString strType = csv.GetString(0);
	strType.MakeUpper();

	bool bAccept = FALSE;

	if ( TRUE == bAllowPlu )
	{
		if ( DealerFlags.GetImportBarcodeFlag() == FALSE ) 
		{
			if ( strType == "PLUNO" )
			{
				bAccept = TRUE;
				m_bWrapperMode = FALSE;
			}
		}
		else
		{
			if ( strType == "BARCODENO" )
			{
				bAccept = TRUE;
				m_bWrapperMode = FALSE;
			}
		}
	}

	if ( ( FALSE == bAccept ) && ( TRUE == bAllowWrp ) && ( strType == "WRAPPER" ) )
	{
		bAccept = TRUE;
		m_bWrapperMode = TRUE;
	}
	
	if ( FALSE == bAccept )
	{
		m_nHeaderError = 1;
	}
	if ( TRUE == bAccept )
	{
		int nFieldCount = 0;
		int nFieldTarget = ( m_bAllowReorder ) ? 3 : 2;

		for ( int n = 1; n < csv.GetSize(); n++ )
		{
			CString strField = csv.GetString(n);
			strField.MakeUpper();

			if ( ( -1 == m_nFieldIdxMin ) && ( strField == "MINIMUM" ) )
			{
				m_nFieldIdxMin = n;
				nFieldCount++;
			}
			else if ( ( -1 == m_nFieldIdxMax ) && ( strField == "MAXIMUM" ) )
			{
				m_nFieldIdxMax = n;
				nFieldCount++;
			}
			else if ( ( TRUE == m_bAllowReorder ) && ( -1 == m_nFieldIdxReorder ) && ( strField == "REORDER" ) )
			{
				m_nFieldIdxReorder = n;
				nFieldCount++;
			}

			if ( nFieldCount == nFieldTarget )
				break;
		}

		if ( 0 == nFieldCount )
		{
			bAccept = FALSE;
			m_nHeaderError = 2;
		}
	}

	return bAccept;
}

/**********************************************************************/

void CReorderImportHelper::ShowHeaderError()
{
	if ( m_nHeaderError != 0 )
	{
		CString strError = "";
		strError.Format( "Unknown import file format (code %d)", m_nHeaderError );
		Prompter.Error( strError );
	}
}

/**********************************************************************/

double CReorderImportHelper::GetMinQty( CCSV& csv )
{
	if ( GotMinField() == TRUE )
		return csv.GetDouble( m_nFieldIdxMin );
	else
		return 0.0;
}

/**********************************************************************/

double CReorderImportHelper::GetReorderQty( CCSV& csv )
{
	if ( GotReorderField() == TRUE )
		return csv.GetDouble( m_nFieldIdxReorder );
	else
		return 0.0;
}

/**********************************************************************/

double CReorderImportHelper::GetMaxQty( CCSV& csv )
{
	if ( GotMaxField() == TRUE )
		return csv.GetDouble( m_nFieldIdxMax );
	else
		return 0.0;
}

/**********************************************************************/
