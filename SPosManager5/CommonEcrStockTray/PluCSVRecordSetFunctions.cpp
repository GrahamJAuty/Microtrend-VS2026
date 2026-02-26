/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/

void CPluCSVRecord::SetPluNo ( __int64 nPluNo )
{
	Set ( m_nPluNo, nPluNo, Plu::PluNo );
}

/**********************************************************************/

void CPluCSVRecord::SetEposText ( const char* szText )
{
	Set ( m_strEposText, FilterEposText(szText), Plu::EposText );
}

/**********************************************************************/

void CPluCSVRecord::SetKeyText ( const char* szText )
{
	Set ( m_strKeyText, FilterEposText(szText), Plu::KeyText );
}

/**********************************************************************/

void CPluCSVRecord::SetPrice ( int n, double dValue )
{
	if ( n >= 0 && n < MAX_PLU_PRICES )
	{
		Set ( m_dPrice[n], dValue, Plu::Price1 );
	}
}

/**********************************************************************/

void CPluCSVRecord::SetBaseDeptNo ( int nDeptNo )
{
	Set ( m_nBaseDeptNo, nDeptNo, Plu::DeptNo );
}

/**********************************************************************/

void CPluCSVRecord::SetBaseTaxBandString( const char* szTaxBand )
{
	CString strBand = szTaxBand;
	if ( strBand.GetLength() == 1 )
		if ( ( strBand >= "A" ) && ( strBand <= "L" ) )
			m_strBaseTaxBand = strBand;
}

/**********************************************************************/

void CPluCSVRecord::SetBaseTaxBandInt(int nTaxBand)
{
	CString strTaxBand = CTaxArray::GetTaxBandFromNumber(nTaxBand, 1, "", "");

	if (strTaxBand != "")
	{
		SetBaseTaxBandString(strTaxBand);
	}
}

/**********************************************************************/

void CPluCSVRecord::SetMicrotrendPluType ( int nType )
{
	Set ( m_nMicrotrendPluType, nType, Plu::MicrotrendPluType );
}

/**********************************************************************/

void CPluCSVRecord::SetExternalProcess ( int nExternalProcess )
{
	Set ( m_nExternalProcess, nExternalProcess, Plu::ExternalProcess );
}

/**********************************************************************/

void CPluCSVRecord::SetBaseAnalysisCategory ( int nCategory )
{
	Set ( m_nBaseAnalysisCategory, nCategory, Plu::AnalysisCategory );
}

/**********************************************************************/

void CPluCSVRecord::SetAllergenNo ( int n, int nAllergenNo )
{
	if ( ( n >= 0 ) && ( n < MAX_PLU_ALLERGENS ) )
	{
		Set ( m_nAllergenNo[n], nAllergenNo, Plu::AllergenNo1 );
	}
}

/**********************************************************************/

void CPluCSVRecord::SetFirstMenu ( int nMenu )
{
	Set ( m_nFirstMenu, nMenu, Plu::FirstMenu );
}

/**********************************************************************/

void CPluCSVRecord::SetFirstQualifier( int nQualifier )
{
	Set ( m_nFirstQualifier, nQualifier, Plu::FirstQualifier );
}

/**********************************************************************/

void CPluCSVRecord::SetFirstExtra ( int nExtra )
{
	Set ( m_nFirstExtra, nExtra, Plu::FirstExtra );
}

/**********************************************************************/

void CPluCSVRecord::SetOrderPrinterGroup ( int nGroup )
{
	Set ( m_nOrderPrinterGroup, nGroup, Plu::OrderPrinterGroup );
}

/**********************************************************************/

void CPluCSVRecord::SetOrderPrinterFlag ( int nNo, bool bFlag )
{
	if ( ( nNo >= 0 ) && ( nNo < PLU_ORDER_PRINTERS_NOW ) )
	{
		m_bOrderPrinterFlag [ nNo ] = bFlag;
	}
}

/**********************************************************************/

void CPluCSVRecord::SetDiscountFlagsFromType ( int nType )
{
	m_bDiscountable = ( 1 == nType );
	m_bStaffDiscount = ( 2 == nType );
}

/**********************************************************************/

void CPluCSVRecord::SetDiscountableFlag( bool b )
{
	Set( m_bDiscountable, b );
}

/**********************************************************************/

void CPluCSVRecord::SetStaffDiscountFlag( bool b )
{
	Set( m_bStaffDiscount, b );
}

/**********************************************************************/

void CPluCSVRecord::SetPriority ( int nPriority )
{
	Set ( m_nPriority, nPriority, Plu::Priority );
}

/**********************************************************************/

void CPluCSVRecord::SetPriceType ( int nType )
{
	Set ( m_nPriceType, nType, Plu::PriceType );
}

/**********************************************************************/

void CPluCSVRecord::SetFreeTextFlag ( bool b )
{
	Set ( m_bFreeText, b );
}

/**********************************************************************/

void CPluCSVRecord::SetAccountRequiredFlag ( bool b )
{
	Set ( m_bAccountRequired, b );
}

/**********************************************************************/

void CPluCSVRecord::SetKPImagePrintFlag ( bool b )
{
	Set ( m_bKPImagePrint, b );
}

/**********************************************************************/

void CPluCSVRecord::SetKPImageModifierFlag ( bool b )
{
	Set ( m_bKPImageModifier, b );
}

/**********************************************************************/

void CPluCSVRecord::SetPluInfoModifierFlag ( bool b )
{
	Set ( m_bPluInfoModifier, b );
}

/**********************************************************************/

void CPluCSVRecord::SetModifierEnable( int nMod, bool bFlag )
{
	if ( ( nMod >= 1 ) && ( nMod <= PLU_MODIFIERS ) )
		m_bModifierEnable[nMod - 1] = bFlag;
}

/**********************************************************************/

void CPluCSVRecord::SetModifierEposText( int nMod, bool bFlag, const char* szText )
{
	if ( ( nMod >= 1 ) && ( nMod <= PLU_MODIFIERS ) )
	{
		if ( TRUE == bFlag )
		{
			m_bModifierEposText[ nMod - 1 ] = TRUE;
			Set( m_strModifierEposText[ nMod - 1 ], szText, Plu::EposText );
		}
		else
		{
			m_bModifierEposText[ nMod - 1 ] = FALSE;
			m_strModifierEposText[ nMod - 1 ] = "";
		}
	}
}

/**********************************************************************/

void CPluCSVRecord::SetModifierKeyText( int nMod, bool bFlag, const char* szText )
{
	if ( ( nMod >= 1 ) && ( nMod <= PLU_MODIFIERS ) )
	{
		if ( TRUE == bFlag )
		{
			m_bModifierKeyText[ nMod - 1 ] = TRUE;
			Set( m_strModifierKeyText[ nMod - 1 ], szText, Plu::KeyText );
		}
		else
		{
			m_bModifierKeyText[ nMod - 1 ] = FALSE;
			m_strModifierKeyText[ nMod - 1 ] = "";
		}
	}
}

/**********************************************************************/

void CPluCSVRecord::SetModifierOrderPrinterGroup( int nMod, int n )
{
	if ( nMod >= 1 && nMod <= PLU_MODIFIERS )
		Set ( m_nModifierOrderPrinterGroup[nMod - 1], n, Plu::Modifier1OrderPrinterGroup );
	else if ( nMod == 0 )
		SetOrderPrinterGroup ( n );
}

/**********************************************************************/

void CPluCSVRecord::SetModifierTaxBandStringRaw( int nMod, const char* szTaxBand )
{
	if ( ( nMod >= 1 ) && ( nMod <= PLU_MODIFIERS ) )
	{
		CString strBand = szTaxBand;
		if ( strBand.GetLength() == 1 )
		{
			CString strAllow = "ABCDEFGHIJKL*";
			if ( strAllow.Find( strBand ) != - 1 )
			{
				m_strModifierTaxBand[ nMod - 1 ] = strBand;
			}
		}
	}
	else if ( 0 == nMod )
	{
		SetBaseTaxBandString( szTaxBand );
	}
}

/**********************************************************************/

void CPluCSVRecord::SetModifierTaxBandIntRaw(int nMod, int nTaxBand)
{
	CString strTaxBand = CTaxArray::GetTaxBandFromNumber(nTaxBand, 0, "", "*");

	if (strTaxBand != "")
	{
		SetModifierTaxBandStringRaw(nMod, strTaxBand);
	}
}

/**********************************************************************/

void CPluCSVRecord::SetModifierAnalysisCategoryRaw( int nMod, int nCat )
{
	if ( ( nMod >= 1 ) && ( nMod <= PLU_MODIFIERS ) )
	{
		Set ( m_nModifierAnalysisCategory[nMod - 1], nCat, Plu::Modifier1AnalysisCategory );
	}
	else if ( nMod == 0 )
	{
		SetBaseAnalysisCategory ( nCat );
	}
}

/**********************************************************************/

void CPluCSVRecord::SetModifierMicrotrendPluType( int nMod, int nType )
{
	if ( nMod >= 1 && nMod <= PLU_MODIFIERS )
	{
		switch( nType )
		{
		case -1:	//AS BASE PLU
		case 0:		//GENERAL
		case 4:		//QUALIFIER
			Set ( m_nModifierMicrotrendPluType[nMod - 1], nType, Plu::Modifier1MicrotrendPluType );
			break;
		}
	}
	else if ( nMod == 0 )
	{
		SetMicrotrendPluType ( nType );
	}
}

/**********************************************************************/

void CPluCSVRecord::SetModifierDeptNoRaw( int nMod, int nDeptNo )
{
	if ( nMod >= 1 && nMod <= PLU_MODIFIERS )
		Set ( m_nModifierDeptNo[nMod - 1], nDeptNo, Plu::Modifier1DeptNo );
	else if ( nMod == 0 )
		SetBaseDeptNo ( nDeptNo );
}

/**********************************************************************/

void CPluCSVRecord::SetModifierFirstMenu(int nMod, int nMenu)
{
	if (nMod >= 1 && nMod <= PLU_MODIFIERS)
	{
		Set(m_nModifierFirstMenu[nMod - 1], nMenu, Plu::Modifier1FirstMenu);
	}
	else if (nMod == 0)
	{
		SetFirstMenu(nMenu);
	}
}

/**********************************************************************/

void CPluCSVRecord::SetModifierFirstQualifier(int nMod, int nQual)
{
	if (nMod >= 1 && nMod <= PLU_MODIFIERS)
	{
		Set(m_nModifierFirstQualifier[nMod - 1], nQual, Plu::Modifier1FirstQualifier);
	}
	else if (nMod == 0)
	{
		SetFirstQualifier(nQual);
	}
}

/**********************************************************************/

void CPluCSVRecord::SetModifierFirstExtra( int nMod, int nExtra )
{
	if (nMod >= 1 && nMod <= PLU_MODIFIERS)
	{
		Set(m_nModifierFirstExtra[nMod - 1], nExtra, Plu::Modifier1FirstExtra);
	}
	else if (nMod == 0)
	{
		SetFirstExtra(nExtra);
	}
}

/**********************************************************************/

void CPluCSVRecord::SetModifierPrice(int nMod, int nLevel, double dPrice)
{
	if (nMod >= 1 && nMod <= PLU_MODIFIERS)
	{
		if (nLevel >= 0 && nLevel < MAX_PLU_PRICES)
		{
			Set(m_dModifierPrice[nMod - 1][nLevel], dPrice, Plu::Modifier1Price1);
		}
	}
	else if (nMod == 0)
	{
		SetPrice(nLevel, dPrice);
	}
}

/**********************************************************************/

void CPluCSVRecord::SetModifiedDateTimePlu()
{
	CTime tm = CTime::GetCurrentTime();
	m_strModifiedDateTimePlu = tm.Format ( "%Y%m%d%H%M%S" );
}

void CPluCSVRecord::SetModifiedDateTimePlu ( const char* szDateTime )
{
	m_strModifiedDateTimePlu = szDateTime;
}

/**********************************************************************/

void CPluCSVRecord::SetModifiedDateTimeInfo()
{
	CTime tm = CTime::GetCurrentTime();
	m_strModifiedDateTimeInfo = tm.Format ( "%Y%m%d%H%M%S" );
}

void CPluCSVRecord::SetModifiedDateTimeInfo ( const char* szDateTime )
{
	m_strModifiedDateTimeInfo = szDateTime;
}

/**********************************************************************/

void CPluCSVRecord::SetDownloadDatePlu()
{
	COleDateTime tm = COleDateTime::GetCurrentTime();
	m_strDownloadDatePlu = tm.Format ( "%Y%m%d" );
}

void CPluCSVRecord::SetDownloadDatePlu ( const char* szDate )
{
	m_strDownloadDatePlu = szDate;
}

/**********************************************************************/

void CPluCSVRecord::SetExport ( bool b )
{
	Set ( m_bExport, b );
}

/**********************************************************************/

void CPluCSVRecord::SetRepText ( const char* szRepText )
{
	Set ( m_strRepText, szRepText, Plu::RepText );
}

/**********************************************************************/

void CPluCSVRecord::SetSortCode ( const char* szSortCode )
{
	Set ( m_strSortCode, szSortCode, Plu::SortCode );
}

/**********************************************************************/

void CPluCSVRecord::SetPurchaseCost ( double dCost )
{
	Set ( m_dPurchaseCost, dCost, Plu::PurchaseCost );
}

/**********************************************************************/

void CPluCSVRecord::SetReportCostEnable( bool b )
{
	Set ( m_bReportCostEnable, b );
}

/**********************************************************************/

void CPluCSVRecord::SetReportCost ( double dCost )
{
	Set ( m_dReportCost, dCost, Plu::ReportCost );
}

/**********************************************************************/

void CPluCSVRecord::SetGPType ( int nGPType )
{
	Set ( m_nGPType, nGPType, Plu::GPType );
}

/**********************************************************************/

void CPluCSVRecord::SetProfitMargin ( double dMargin )
{
	Set ( m_dProfitMargin, dMargin, Plu::ProfitMargin );
}

/**********************************************************************/

void CPluCSVRecord::SetApparentLevel ( double dLevel )
{
	Set ( m_dApparentLevel, dLevel, Plu::ApparentLevel );
}

/**********************************************************************/

void CPluCSVRecord::SetMinimumLevel ( double dLevel )
{
	Set ( m_dMinimumLevel, dLevel, Plu::MinimumLevel );
}

/**********************************************************************/

void CPluCSVRecord::SetUnitPriceText ( const char* szText )
{
	Set ( m_strUnitPriceText, szText, Plu::UnitPriceText );
}

/**********************************************************************/

void CPluCSVRecord::SetUnitPriceSize ( double dSize )
{
	Set ( m_dUnitPriceSize, dSize, Plu::UnitPriceSize );
}

/**********************************************************************/

void CPluCSVRecord::SetSupplierRef( const char* szRef )
{
	Set ( m_strSupplierRef, szRef, Plu::SupplierRef );
}

/**********************************************************************/

void CPluCSVRecord::SetSupplierNo( int nSuppNo )
{
	Set ( m_nSupplierNo, nSuppNo, Plu::SupplierNo );
}

/**********************************************************************/

void CPluCSVRecord::SetNonStockInternalFlag( bool b )
{
	Set( m_bNonStock, b );
}

/**********************************************************************/

void CPluCSVRecord::SetPrintShelfFlag( bool b )
{
	Set( m_bPrintShelf, b );
}

/**********************************************************************/

void CPluCSVRecord::SetPrintProductFlag( bool b )
{
	Set( m_bPrintProduct, b );
}

/**********************************************************************/

void CPluCSVRecord::SetPoints ( int nPoints )
{
	Set ( m_nPoints, nPoints, Plu::Points );
}

/**********************************************************************/

void CPluCSVRecord::SetStampWeight(int nSW)
{
	Set(m_nStampWeight, nSW, Plu::StampWeight);
}

/**********************************************************************/

void CPluCSVRecord::SetSalesTrackingFlag( bool b )
{
	Set( m_bSalesTracking, b );
}

/**********************************************************************/

void CPluCSVRecord::SetEANOnReceiptFlag( bool b )
{
	Set( m_bEANOnReceipt, b );
}

/**********************************************************************/

void CPluCSVRecord::SetFreeTextDescriptionFlag ( bool b )
{
	Set ( m_bFreeTextDescription, b );
}

/**********************************************************************/

void CPluCSVRecord::SetAllowModifierFlag(bool b)
{
	Set(m_bAllowModifier, b);
}

/**********************************************************************/

void CPluCSVRecord::SetLinkType ( int n )
{
	Set ( m_nLinkType, n, Plu::LinkType );
}

/**********************************************************************/

void CPluCSVRecord::SetLinkSellTiedItemFlag(bool b)
{
	Set(m_bLinkSellTiedItem, b);
}

/**********************************************************************/

void CPluCSVRecord::SetLinkTableString ( const char* sz )
{
	Set ( m_strLinkTable, sz, Plu::LinkTable );
}

/**********************************************************************/

void CPluCSVRecord::SetEcrmanRecipeTableString ( const char* sz )
{
	Set ( m_strEcrmanRecipeTable, sz, Plu::EcrmanRecipeTable );
}

/**********************************************************************/

void CPluCSVRecord::SetAgeVerificationType( int n )
{
	Set ( m_nAgeVerificationType, n, Plu::AgeVerificationType );
}

/**********************************************************************/

void CPluCSVRecord::SetKPNotAloneFlag( bool b )
{
	Set ( m_bKPNotAlone, b );
}

/**********************************************************************/

void CPluCSVRecord::SetKPConsolidateTopItemFlag( bool b )
{
	Set ( m_bKPConsolidateTopItem, b );
}

/**********************************************************************/

void CPluCSVRecord::SetAutoCoversQtyFlag( bool b )
{
	Set ( m_bAutoCoversQty, b );
}

/**********************************************************************/

void CPluCSVRecord::SetAgressoGLCode(int n)
{
	Set(m_nAgressoGLCode, n, Plu::AgressoGLCode);

	if (m_nAgressoGLCode < 1000)
	{
		m_nAgressoGLCode = 0;
	}
}

/**********************************************************************/

void CPluCSVRecord::SetAgressoCostCentre( const char* sz )
{
	Set ( m_strAgressoCostCentre, sz, Plu::AgressoCostCentre );
}

/**********************************************************************/

void CPluCSVRecord::SetAgressoProject( const char* sz )
{
	Set ( m_strAgressoProject, sz, Plu::AgressoProject );
}

/**********************************************************************/

void CPluCSVRecord::SetSoldOutFlag( bool b )
{
	Set ( m_bSoldOut, b );
}

/**********************************************************************/

void CPluCSVRecord::SetImageFilename( const char* sz )
{
	CString str = sz;
	::TrimSpaces ( str, FALSE );
	Set ( m_strImageFilename, str, Plu::ImageFilename );
}

/**********************************************************************/

void CPluCSVRecord::SetActiveItemFlag( bool b )
{
	Set ( m_bActive, b );
}

/**********************************************************************/
