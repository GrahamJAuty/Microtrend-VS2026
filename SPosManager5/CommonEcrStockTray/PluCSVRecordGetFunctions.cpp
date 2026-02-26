/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/

const char* CPluCSVRecord::GetPluNoString()
{
	::FormatPluNo( m_nPluNo, m_strPluNoString );
	return m_strPluNoString;
}

/**********************************************************************/

double CPluCSVRecord::GetPrice ( int n )
{
	if ( n >= 0 && n < MAX_PLU_PRICES )
		return m_dPrice[n];
	else
		return 0.0;
}

/**********************************************************************/

int CPluCSVRecord::GetAllergenNo ( int n )
{
	if ( n >= 0 && n < MAX_PLU_ALLERGENS )
		return m_nAllergenNo[n];
	else
		return 0;
}

/**********************************************************************/

bool CPluCSVRecord::GetModifierEnable( int nMod )
{
	if ( ( nMod >= 1 ) && ( nMod <= PLU_MODIFIERS ) )
		return m_bModifierEnable[nMod - 1];
	else
		return FALSE;
}

/**********************************************************************/

bool CPluCSVRecord::GetModifierEposTextFlag( int nMod )
{
	if ( nMod >= 1 && nMod <= PLU_MODIFIERS )
		return m_bModifierEposText[nMod - 1];
	else
		return FALSE;
}

/**********************************************************************/

const char* CPluCSVRecord::GetModifierEposText( int nMod )
{
	if ( ( nMod >= 1 ) && ( nMod <= PLU_MODIFIERS ) )
	{
		if ( GetModifierEposTextFlag( nMod ) == FALSE )
		{
			CString strText = GetEposText();
			if ( EcrmanOptions.GetPluAutoModEposTextType() == 1 )
			{
				CString strTemp = "";
				
				if ( DataManager.Modifier.GetSize() != 0 )
					strTemp = DataManager.Modifier.GetDisplayName( nMod );
				else
					strTemp.Format( "Mod %d :", nMod );

				if ( strTemp != "" )
					strTemp += " ";

				strTemp += strText;
				strText = strTemp;
			}
		
			Set( m_strModifierEposText[ nMod - 1 ], strText, Plu::EposText );
		}

		return m_strModifierEposText[ nMod - 1 ];
	}
	else if ( nMod == 0 )
		return m_strEposText;
	else
		return "";
}

/**********************************************************************/

bool CPluCSVRecord::GetModifierKeyTextFlag( int nMod )
{
	if ( nMod >= 1 && nMod <= PLU_MODIFIERS )
		return m_bModifierKeyText[nMod - 1];
	else
		return FALSE;
}

/**********************************************************************/

const char* CPluCSVRecord::GetModifierKeyText( int nMod )
{
	if ( ( nMod >= 1 ) && ( nMod <= PLU_MODIFIERS ) )
	{
		if ( GetModifierKeyTextFlag( nMod ) == FALSE )
		{
			CString strText = GetKeyText();
			if ( EcrmanOptions.GetPluAutoModKeyTextType() == 1 )
			{
				CString strTemp = "";
				
				if ( DataManager.Modifier.GetSize() != 0 )
					strTemp = DataManager.Modifier.GetDisplayName( nMod );
				else
					strTemp.Format( "Mod %d :", nMod );

				if ( strTemp != "" )
					strTemp += " ";

				strTemp += strText;
				strText = strTemp;
			}
		
			Set( m_strModifierKeyText[ nMod - 1 ], strText, Plu::KeyText );
		}

		return m_strModifierKeyText[ nMod - 1 ];
	}
	else if ( nMod == 0 )
		return m_strKeyText;
	else
		return "";
}

/**********************************************************************/

const char* CPluCSVRecord::GetModifierReportText( int nMod )
{
	if ( nMod >= 1 && nMod <= PLU_MODIFIERS )
		return GetModifierEposText( nMod );
	else if ( nMod == 0 )
		return GetReportText();
	else
		return "";
}

/**********************************************************************/

int CPluCSVRecord::GetModifierOrderPrinterGroup( int nMod )
{
	if ( nMod >= 1 && nMod <= PLU_MODIFIERS )
		return m_nModifierOrderPrinterGroup[nMod - 1];
	else if ( nMod == 0 )
	{
		return m_nOrderPrinterGroup;
	}
	else
		return 0;
}

/**********************************************************************/

const char* CPluCSVRecord::GetModifierTaxBandStringRaw( int nMod )
{
	if ( ( nMod >= 1 ) && ( nMod <= PLU_MODIFIERS ) )
	{
		return m_strModifierTaxBand[nMod - 1];
	}
	else if ( nMod == 0 )
	{
		return m_strBaseTaxBand;
	}
	else
	{
		return "";
	}
}

/**********************************************************************/

int CPluCSVRecord::GetModifierAnalysisCategoryRaw( int nMod )
{
	if ( nMod >= 1 && nMod <= PLU_MODIFIERS )
		return m_nModifierAnalysisCategory[nMod - 1];
	else if ( nMod == 0 )
	{
		return m_nBaseAnalysisCategory;
	}
	else
		return 0;
}

/**********************************************************************/

int CPluCSVRecord::GetModifierAnalysisCategoryForReport( int nMod )
{
	int nCatNo = -1;

	if ( ( nMod >= 1 ) && ( nMod <= 9 ) )
	{
		//TRY TO GET CATEGORY FROM MODIFIER IF RELEVANT
		if ( GetModifierEnable( nMod ) == TRUE )
		{
			nCatNo = GetModifierAnalysisCategoryRaw( nMod );
		}
	}

	//USE BASE CAT NO IF MODIFIER NOT ENABLE OR DEFERS TO BASE
	if ( -1 == nCatNo )
	{
		nCatNo = GetBaseAnalysisCategory();
	}
	
	return nCatNo;
}

/**********************************************************************/

int CPluCSVRecord::GetModifierMicrotrendPluType( int nMod )
{
	if ((nMod >= 1) && (nMod <= PLU_MODIFIERS))
	{
		return m_nModifierMicrotrendPluType[nMod - 1];
	}
	else if ( nMod == 0 )
	{
		return m_nMicrotrendPluType;
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

int CPluCSVRecord::GetModifierDeptNoRaw( int nMod )
{
	if ( nMod >= 1 && nMod <= PLU_MODIFIERS )
	{
		return m_nModifierDeptNo[nMod - 1];
	}
	else if ( nMod == 0 )
	{
		return m_nBaseDeptNo;
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

int CPluCSVRecord::GetModifierDeptNoForReport( int nMod )
{
	int nDeptNo = -1;

	if ( ( nMod >= 1 ) && ( nMod <= 9 ) )
	{
		//TRY TO GET DEPT NO FROM MODIFIER IF RELEVANT
		if ( GetModifierEnable( nMod ) == TRUE )
		{
			nDeptNo = GetModifierDeptNoRaw( nMod );
		}
	}

	//USE BASE DEPT NO IF MODIFIER NOT ENABLE OR DEFERS TO BASE
	if ( -1 == nDeptNo )
	{
		nDeptNo = m_nBaseDeptNo;
	}
	
	return nDeptNo;
}

/**********************************************************************/

int CPluCSVRecord::GetModifierFirstMenu( int nMod )
{
	if ( nMod >= 1 && nMod <= PLU_MODIFIERS )
		return m_nModifierFirstMenu[nMod - 1];
	else if ( nMod == 0 )
	{
		return m_nFirstMenu;
	}
	else
		return 0;
}

/**********************************************************************/

int CPluCSVRecord::GetModifierFirstQualifier( int nMod )
{
	if ( nMod >= 1 && nMod <= PLU_MODIFIERS )
		return m_nModifierFirstQualifier[nMod - 1];
	else if ( nMod == 0 )
	{
		return m_nFirstQualifier;
	}
	else
		return 0;
}

/**********************************************************************/

int CPluCSVRecord::GetModifierFirstExtra( int nMod )
{
	if ( nMod >= 1 && nMod <= PLU_MODIFIERS )
		return m_nModifierFirstExtra[nMod - 1];
	else if ( nMod == 0 )
	{
		return m_nFirstExtra;
	}
	else
		return 0;
}

/**********************************************************************/

double CPluCSVRecord::GetModifierPrice( int nMod, int nLevel )
{
	if ( nMod >= 1 && nMod <= PLU_MODIFIERS )
	{
		if ( nLevel >= 0 && nLevel < MAX_PLU_PRICES )
			return m_dModifierPrice[nMod - 1][nLevel];
		else
			return 0.0;
	}
	else if ( nMod == 0 )
	{
		if ( nLevel >= 0 && nLevel < MAX_PLU_PRICES )
			return m_dPrice[nLevel];
		else
			return 0.0;
	}
	else
		return 0.0;
}

/**********************************************************************/

const char* CPluCSVRecord::GetReportText()
{
	m_strReportText = m_strRepText;
	::TrimSpaces ( m_strReportText, FALSE );
	
	if ( m_strReportText == "" )
		m_strReportText = m_strEposText;

	return m_strReportText;
}

/**********************************************************************/

int CPluCSVRecord::GetBaseTaxBandInt()
{
	return CTaxArray::GetNumericTaxBand(m_strBaseTaxBand);
}

/**********************************************************************/

int CPluCSVRecord::GetModifierTaxBandIntRaw(int nMod)
{
	if ((nMod >= 1) && (nMod <= PLU_MODIFIERS))
	{
		return CTaxArray::GetNumericTaxBand(m_strModifierTaxBand[nMod - 1]);
	}
	else if (0 == nMod)
	{
		return GetBaseTaxBandInt();
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

int CPluCSVRecord::GetModifierTaxBandIntForReport( int nMod )
{
	int nTaxBand = 0;
	
	if ( ( nMod >= 0 ) && ( nMod <= 9 ) )
	{
		//TRY TO GET TAX BAND FROM MODIFIER IF RELEVANT
		if ( nMod != 0 )
		{
			if ( GetModifierEnable( nMod ) == TRUE )
			{
				nTaxBand = GetModifierTaxBandIntRaw( nMod );
			}
		}

		//USE BASE TAX BAND IF MODIFIER NOT ENABLE OR DEFERS TO BASE
		if ( 0 == nTaxBand )
		{
			nTaxBand = GetBaseTaxBandInt();
		}
		
		if ( ( nTaxBand < 0 ) || ( nTaxBand > MAX_TAX_BANDS ) )
		{
			nTaxBand = 0;
		}
	}

	return nTaxBand;
}

/**********************************************************************/

CString CPluCSVRecord::GetModifierTaxBandStringForReport(int nMod)
{
	int nBand = GetModifierTaxBandIntForReport(nMod);
	return CTaxArray::GetTaxBandFromNumber(nBand);
}

/**********************************************************************/

bool CPluCSVRecord::GetOrderPrinterFlag( int nNo )
{
	if ( nNo >= 0 && nNo < PLU_ORDER_PRINTERS_NOW )
		return m_bOrderPrinterFlag[ nNo ];
	else
		return FALSE;
}

/**********************************************************************/
