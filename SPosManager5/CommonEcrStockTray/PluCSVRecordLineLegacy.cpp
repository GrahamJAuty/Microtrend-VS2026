/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/

/**********************************************************************/
/*	Version 1												*/
/**********************************************************************/

void CPluCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	int n = 0;

	SetPluNo ( csv.GetInt64(n++) );
	n++;
	SetEposText ( csv.GetString(n++) );

	for ( int i = 0 ; i < 4 ; i++ )
		SetPrice ( i, csv.GetDouble(n++) );			
	
	SetBaseDeptNo ( csv.GetInt(n++) );
	SetBaseTaxBandString ( csv.GetString(n++) );
	SetMicrotrendPluType ( csv.GetInt(n++) );
	SetBaseAnalysisCategory ( csv.GetInt(n++) );
	SetFirstMenu ( csv.GetInt(n++) );
	SetFirstQualifier ( csv.GetInt(n++) );
	SetFirstExtra ( csv.GetInt(n++) );
	
	for ( int i = 0; i < PLU_ORDER_PRINTERS_PRE_V39; i++ )
		SetOrderPrinterFlag ( i, csv.GetBool(n++) );
	
	SetDiscountFlagsFromType ( csv.GetInt(n++) );
	SetPriority ( csv.GetInt(n++) );
	SetPriceType( csv.GetInt(n++) );
	n++; //LEGACY OFFER
	n++; //LEGACY BUFFER
	SetFreeTextFlag( csv.GetBool(n++) );
	m_bKPImagePrint = TRUE;
	m_bKPImageModifier = FALSE;
	m_bPluInfoModifier = FALSE;

	for ( int m = 1; m <= PLU_MODIFIERS; m++ )
	{
		SetModifierEnable ( m, csv.GetBool(n++) );
		SetModifierEposText ( m, TRUE, csv.GetString(n++) );
		SetModifierKeyText( m, TRUE, "" );
		
		for ( int l = 0; l < 4; l++ )
			SetModifierPrice( m, l, csv.GetDouble(n++) );
	}

	SetModifiedDateTimePlu ( csv.GetString(n++) );
	SetExport ( csv.GetBool(n++) );
	SetRepText ( csv.GetString(n++) );
	SetPurchaseCost ( csv.GetDouble(n++) );
	SetGPType ( csv.GetInt(n++) );
	SetProfitMargin ( csv.GetDouble(n++) );
	SetApparentLevel ( csv.GetDouble(n++) );
	SetMinimumLevel ( csv.GetDouble(n++) );
}

/**********************************************************************/
/*	Version 2												*/
/**********************************************************************/

void CPluCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	int n = 0;

	SetPluNo ( csv.GetInt64(n++) );
	n++;
	SetEposText ( csv.GetString(n++) );

	for ( int i = 0 ; i < 4 ; i++ )
		SetPrice ( i, csv.GetDouble(n++) );			
	
	SetBaseDeptNo ( csv.GetInt(n++) );
	SetBaseTaxBandString ( csv.GetString(n++) );
	SetMicrotrendPluType ( csv.GetInt(n++) );
	SetBaseAnalysisCategory ( csv.GetInt(n++) );
	SetFirstMenu ( csv.GetInt(n++) );
	SetFirstQualifier ( csv.GetInt(n++) );
	SetFirstExtra ( csv.GetInt(n++) );
	
	for ( int i = 0; i < PLU_ORDER_PRINTERS_PRE_V39; i++ )
		SetOrderPrinterFlag ( i, csv.GetBool(n++) );
	
	SetDiscountFlagsFromType ( csv.GetInt(n++) );
	SetPriority ( csv.GetInt(n++) );
	SetPriceType( csv.GetInt(n++) );
	n++; //LEGACY OFFER
	n++; //LEGACY BUFFER
	SetFreeTextFlag( csv.GetBool(n++) );
	m_bKPImagePrint = TRUE;
	m_bKPImageModifier = FALSE;

	for ( int m = 1; m <= PLU_MODIFIERS; m++ )
	{
		SetModifierEnable ( m, csv.GetBool(n++) );
		SetModifierEposText ( m, TRUE, csv.GetString(n++) );
		SetModifierKeyText( m, TRUE, "" );
		
		for ( int l = 0; l < 4; l++ )
			SetModifierPrice( m, l, csv.GetDouble(n++) );
	}

	SetModifiedDateTimePlu ( csv.GetString(n++) );
	SetExport ( csv.GetBool(n++) );
	SetRepText ( csv.GetString(n++) );
	SetPurchaseCost ( csv.GetDouble(n++) );
	SetGPType ( csv.GetInt(n++) );
	SetProfitMargin ( csv.GetDouble(n++) );
	SetApparentLevel ( csv.GetDouble(n++) );
	SetMinimumLevel ( csv.GetDouble(n++) );
	SetUnitPriceText ( csv.GetString(n++) );
	SetUnitPriceSize ( csv.GetDouble(n++) );
}

/**********************************************************************/
/*	Version 3												*/
/**********************************************************************/

void CPluCSVRecord::V3ConvertFromCSV ( CCSV& csv )
{
	int n = 0;

	SetPluNo ( csv.GetInt64(n++) );
	n++;
	SetEposText ( csv.GetString(n++) );

	for ( int i = 0 ; i < 4 ; i++ )
		SetPrice ( i, csv.GetDouble(n++) );			
	
	SetBaseDeptNo ( csv.GetInt(n++) );
	SetBaseTaxBandString ( csv.GetString(n++) );
	SetMicrotrendPluType ( csv.GetInt(n++) );
	SetBaseAnalysisCategory ( csv.GetInt(n++) );
	SetFirstMenu ( csv.GetInt(n++) );
	SetFirstQualifier ( csv.GetInt(n++) );
	SetFirstExtra ( csv.GetInt(n++) );
	
	for ( int i = 0; i < PLU_ORDER_PRINTERS_PRE_V39; i++ )
		SetOrderPrinterFlag ( i, csv.GetBool(n++) );
	
	SetDiscountFlagsFromType ( csv.GetInt(n++) );
	SetPriority ( csv.GetInt(n++) );
	SetPriceType( csv.GetInt(n++) );
	n++; //LEGACY OFFER
	n++; //LEGACY BUFFER
	SetFreeTextFlag( csv.GetBool(n++) );
	m_bKPImagePrint = TRUE;
	m_bKPImageModifier = FALSE;

	for ( int m = 1; m <= PLU_MODIFIERS; m++ )
	{
		SetModifierEnable ( m, csv.GetBool(n++) );
		SetModifierEposText ( m, TRUE, csv.GetString(n++) );
		SetModifierKeyText ( m, TRUE, "" );
		
		for ( int l = 0; l < 4; l++ )
			SetModifierPrice( m, l, csv.GetDouble(n++) );
	}

	SetModifiedDateTimePlu ( csv.GetString(n++) );
	SetExport ( csv.GetBool(n++) );
	SetRepText ( csv.GetString(n++) );
	SetPurchaseCost ( csv.GetDouble(n++) );
	SetGPType ( csv.GetInt(n++) );
	SetProfitMargin ( csv.GetDouble(n++) );
	SetApparentLevel ( csv.GetDouble(n++) );
	SetMinimumLevel ( csv.GetDouble(n++) );
	SetUnitPriceText ( csv.GetString(n++) );
	SetUnitPriceSize ( csv.GetDouble(n++) );
	SetSupplierRef( csv.GetString(n++) );
	SetSupplierNo( csv.GetInt(n++) );
	SetNonStockInternalFlag( csv.GetBool(n++) );
}

/**********************************************************************/
/*	Version 4 to 10	    											  */
/**********************************************************************/

void CPluCSVRecord::V4To10ConvertFromCSV ( CCSV& csv, int nVer )
{
	int n = 0;

	SetPluNo ( csv.GetInt64(n++) );
	n++;
	SetEposText ( csv.GetString(n++) );

	for ( int i = 0 ; i < 4 ; i++ )
		SetPrice ( i, csv.GetDouble(n++) );			
	
	SetBaseDeptNo ( csv.GetInt(n++) );
	SetBaseTaxBandString ( csv.GetString(n++) );
	SetMicrotrendPluType ( csv.GetInt(n++) );
	SetBaseAnalysisCategory ( csv.GetInt(n++) );
	SetFirstMenu ( csv.GetInt(n++) );
	SetFirstQualifier ( csv.GetInt(n++) );
	SetFirstExtra ( csv.GetInt(n++) );
	
	for ( int i = 0; i < PLU_ORDER_PRINTERS_PRE_V39; i++ )
		SetOrderPrinterFlag ( i, csv.GetBool(n++) );
	
	SetDiscountFlagsFromType ( csv.GetInt(n++) );
	SetPriority ( csv.GetInt(n++) );
	SetPriceType( csv.GetInt(n++) );
	n++; //LEGACY OFFER
	n++; //LEGACY BUFFER
	SetFreeTextFlag( csv.GetBool(n++) );
	SetKPImagePrintFlag( csv.GetBool(n++) );
	SetKPImageModifierFlag( csv.GetBool(n++) );

	for ( int m = 1; m <= PLU_MODIFIERS; m++ )
	{
		SetModifierEnable ( m, csv.GetBool(n++) );
		SetModifierEposText ( m, TRUE, csv.GetString(n++) );
		SetModifierKeyText( m, TRUE, "" );
		
		for ( int l = 0; l < 4; l++ )
			SetModifierPrice( m, l, csv.GetDouble(n++) );
	}

	SetModifiedDateTimePlu ( csv.GetString(n++) );
	SetExport ( csv.GetBool(n++) );
	SetRepText ( csv.GetString(n++) );
	SetPurchaseCost ( csv.GetDouble(n++) );
	SetGPType ( csv.GetInt(n++) );
	SetProfitMargin ( csv.GetDouble(n++) );
	SetApparentLevel ( csv.GetDouble(n++) );
	SetMinimumLevel ( csv.GetDouble(n++) );
	SetUnitPriceText ( csv.GetString(n++) );
	SetUnitPriceSize ( csv.GetDouble(n++) );
	SetSupplierRef( csv.GetString(n++) );
	SetSupplierNo( csv.GetInt(n++) );
	SetNonStockInternalFlag( csv.GetBool(n++) );

	if ( nVer >= 5 )
	{
		SetPrintShelfFlag( csv.GetBool(n++ ) );
		SetPrintProductFlag( csv.GetBool(n++ ) );
	}
	else
	{
		SetPrintShelfFlag( TRUE );
		SetPrintProductFlag( TRUE );
	}

	if ( nVer >= 6 )
		SetPoints( csv.GetInt(n++) );

	if ( nVer >= 7 )
	{
		SetSalesTrackingFlag( csv.GetBool(n++) );
		SetEANOnReceiptFlag( csv.GetBool(n++) );
	}

	if ( nVer >= 8 )
		SetFreeTextDescriptionFlag( csv.GetBool(n++) );

	if ( nVer >= 9 )
	{
		SetLinkType( csv.GetInt(n++) );
		SetLinkTableString( csv.GetString(n++) );
	}

	if ( nVer >= 10 )
		SetAgeVerificationType( csv.GetInt(n++) );
}

/**********************************************************************/
/*	Version 11	  													  */
/**********************************************************************/

void CPluCSVRecord::V11To13ConvertFromCSV ( CCSV& csv, int nVer )
{
	int n = 0;

	SetPluNo ( csv.GetInt64(n++) );
	n++;
	SetEposText ( csv.GetString(n++) );

	if ( nVer >= 12 )
		SetKeyText ( csv.GetString(n++) );

	for ( int i = 0 ; i < 10 ; i++ )
		SetPrice ( i, csv.GetDouble(n++) );			
	
	SetBaseDeptNo ( csv.GetInt(n++) );
	SetBaseTaxBandString ( csv.GetString(n++) );
	SetMicrotrendPluType ( csv.GetInt(n++) );

	if ( nVer >= 13 )
		SetExternalProcess( csv.GetInt(n++) );

	SetBaseAnalysisCategory ( csv.GetInt(n++) );
	SetFirstMenu ( csv.GetInt(n++) );
	SetFirstQualifier ( csv.GetInt(n++) );
	SetFirstExtra ( csv.GetInt(n++) );
	
	for ( int i = 0; i < PLU_ORDER_PRINTERS_PRE_V39; i++ )
		SetOrderPrinterFlag ( i, csv.GetBool(n++) );
	
	SetDiscountFlagsFromType ( csv.GetInt(n++) );
	SetPriority ( csv.GetInt(n++) );
	SetPriceType( csv.GetInt(n++) );
	n++; //LEGACY OFFER
	n++; //LEGACY BUFFER
	SetFreeTextFlag( csv.GetBool(n++) );
	SetKPImagePrintFlag( csv.GetBool(n++) );
	SetKPImageModifierFlag( csv.GetBool(n++) );

	for ( int m = 1; m <= PLU_MODIFIERS; m++ )
	{
		int nEnable = csv.GetInt(n++);
		SetModifierEnable( m, ( nEnable == 1 ) );

		if ( nEnable != 2 )
		{
			SetModifierEposText ( m, TRUE, csv.GetString(n++) );

			if ( nVer >= 12 )
				SetModifierKeyText ( m, TRUE, csv.GetString(n++) );
		
			for ( int l = 0; l < 10; l++ )
				SetModifierPrice( m, l, csv.GetDouble(n++) );
		}
	}

	SetModifiedDateTimePlu ( csv.GetString(n++) );
	SetExport ( csv.GetBool(n++) );
	SetRepText ( csv.GetString(n++) );
	SetPurchaseCost ( csv.GetDouble(n++) );
	SetGPType ( csv.GetInt(n++) );
	SetProfitMargin ( csv.GetDouble(n++) );
	SetApparentLevel ( csv.GetDouble(n++) );
	SetMinimumLevel ( csv.GetDouble(n++) );
	SetUnitPriceText ( csv.GetString(n++) );
	SetUnitPriceSize ( csv.GetDouble(n++) );
	SetSupplierRef( csv.GetString(n++) );
	SetSupplierNo( csv.GetInt(n++) );
	SetNonStockInternalFlag( csv.GetBool(n++) );
	SetPrintShelfFlag( csv.GetBool(n++ ) );
	SetPrintProductFlag( csv.GetBool(n++ ) );
	SetPoints( csv.GetInt(n++) );
	SetSalesTrackingFlag( csv.GetBool(n++) );
	SetEANOnReceiptFlag( csv.GetBool(n++) );
	SetFreeTextDescriptionFlag( csv.GetBool(n++) );
	SetLinkType( csv.GetInt(n++) );
	SetLinkTableString( csv.GetString(n++) );
	SetAgeVerificationType( csv.GetInt(n++) );
}

/**********************************************************************/
/*	Version 14	  													  */
/**********************************************************************/

void CPluCSVRecord::V14To26ConvertFromCSV ( CCSV& csv, int nVer )
{
	int n = 0;

	SetPluNo ( csv.GetInt64(n++) );
	n++;
	SetEposText ( csv.GetString(n++) );
	SetKeyText ( csv.GetString(n++) );

	for ( int i = 0 ; i < 10 ; i++ )
		SetPrice ( i, csv.GetDouble(n++) );			
	
	SetBaseDeptNo ( csv.GetInt(n++) );
	SetBaseTaxBandString ( csv.GetString(n++) );
	SetMicrotrendPluType ( csv.GetInt(n++) );
	SetExternalProcess( csv.GetInt(n++) );

	SetBaseAnalysisCategory ( csv.GetInt(n++) );

	if ( nVer == 20 )
	{
		for ( int i = 0 ; i < 3 ; i++ )
			SetAllergenNo ( i, csv.GetInt(n++) );
	}

	if ( nVer >= 21 )
	{
		for ( int i = 0 ; i < 6 ; i++ )
			SetAllergenNo ( i, csv.GetInt(n++) );
	}

	SetFirstMenu ( csv.GetInt(n++) );
	SetFirstQualifier ( csv.GetInt(n++) );
	SetFirstExtra ( csv.GetInt(n++) );

	if ( nVer >= 25 )
		SetOrderPrinterGroup( csv.GetInt(n++) );
	
	for ( int i = 0; i < PLU_ORDER_PRINTERS_PRE_V39; i++ )
		SetOrderPrinterFlag ( i, csv.GetBool(n++) );
	
	SetDiscountFlagsFromType ( csv.GetInt(n++) );
	SetPriority ( csv.GetInt(n++) );
	SetPriceType( csv.GetInt(n++) );
	n++; //LEGACY OFFER
	n++; //LEGACY BUFFER
	SetFreeTextFlag( csv.GetBool(n++) );
	SetKPImagePrintFlag( csv.GetBool(n++) );
	SetKPImageModifierFlag( csv.GetBool(n++) );

	if ( nVer >= 17 )
		SetPluInfoModifierFlag( csv.GetBool(n++) );

	for ( int m = 1; m <= PLU_MODIFIERS; m++ )
	{
		int nEnable = csv.GetInt(n++);
		SetModifierEnable( m, ( nEnable == 1 ) );

		if ( nEnable != 2 )
		{
			{
				bool b = csv.GetBool(n++);
				CString str = csv.GetString(n++);
				SetModifierEposText( m, b, str );
			}

			{
				bool b = csv.GetBool(n++);
				CString str = csv.GetString(n++);
				SetModifierKeyText( m, b, str );
			}

			for ( int l = 0; l < 10; l++ )
				SetModifierPrice( m, l, csv.GetDouble(n++) );
		}
	}

	SetModifiedDateTimePlu( csv.GetString(n++) );

	if ( nVer >= 18 )
		SetModifiedDateTimeInfo( csv.GetString(n++) );

	if ( nVer >= 19 )
		SetDownloadDatePlu( csv.GetString(n++) );

	SetExport ( csv.GetBool(n++) );
	SetRepText ( csv.GetString(n++) );

	if ( nVer >= 22 )
		SetSortCode( csv.GetString(n++) );

	SetPurchaseCost ( csv.GetDouble(n++) );

	if ( nVer >= 23 )
	{
		SetReportCostEnable( csv.GetBool(n++) );
		SetReportCost ( csv.GetDouble(n++) );
	}

	SetGPType ( csv.GetInt(n++) );
	SetProfitMargin ( csv.GetDouble(n++) );
	SetApparentLevel ( csv.GetDouble(n++) );
	SetMinimumLevel ( csv.GetDouble(n++) );
	SetUnitPriceText ( csv.GetString(n++) );
	SetUnitPriceSize ( csv.GetDouble(n++) );
	SetSupplierRef( csv.GetString(n++) );
	SetSupplierNo( csv.GetInt(n++) );
	SetNonStockInternalFlag( csv.GetBool(n++) );
	SetPrintShelfFlag( csv.GetBool(n++ ) );
	SetPrintProductFlag( csv.GetBool(n++ ) );
	SetPoints( csv.GetInt(n++) );
	SetSalesTrackingFlag( csv.GetBool(n++) );
	SetEANOnReceiptFlag( csv.GetBool(n++) );
	SetFreeTextDescriptionFlag( csv.GetBool(n++) );
	SetLinkType( csv.GetInt(n++) );
	SetLinkTableString( csv.GetString(n++) );

	if ( nVer >= 24 )
		SetEcrmanRecipeTableString( csv.GetString(n++) );

	SetAgeVerificationType( csv.GetInt(n++) );

	if ( 15 == nVer )
	{
		SetAutoCoversQtyFlag( csv.GetBool(n++) );
		SetKPConsolidateTopItemFlag( csv.GetBool(n++) );
	}

	if ( nVer >= 16 )
	{
		SetKPConsolidateTopItemFlag( csv.GetBool(n++) );
		SetAutoCoversQtyFlag( csv.GetBool(n++) );
	}

	if ( nVer >= 26 )
	{
		SetAgressoGLCode( csv.GetInt(n++) );
		SetAgressoCostCentre( csv.GetString(n++) );
		SetAgressoProject( csv.GetString(n++) );
	}
}

/**********************************************************************/
/*	Version 27 to 29												  */
/**********************************************************************/

void CPluCSVRecord::X500V27To32ConvertFromCSV ( CCSV& csv, int nVer )
{
	int n = 0;

	SetPluNo ( csv.GetInt64(n++) );
	n++;
	SetEposText ( csv.GetString(n++) );
	SetKeyText ( csv.GetString(n++) );

	for ( int i = 0 ; i < 10 ; i++ )
		SetPrice ( i, csv.GetDouble(n++) );			
	
	SetBaseDeptNo ( csv.GetInt(n++) );
	SetBaseTaxBandString ( csv.GetString(n++) );
	SetMicrotrendPluType ( csv.GetInt(n++) );
	SetExternalProcess( csv.GetInt(n++) );

	SetBaseAnalysisCategory ( csv.GetInt(n++) );

	for ( int i = 0 ; i < 6 ; i++ )
		SetAllergenNo ( i, csv.GetInt(n++) );
	
	SetFirstMenu ( csv.GetInt(n++) );
	SetFirstQualifier ( csv.GetInt(n++) );
	SetFirstExtra ( csv.GetInt(n++) );
	SetOrderPrinterGroup( csv.GetInt(n++) );
	
	for ( int i = 0; i < PLU_ORDER_PRINTERS_PRE_V39; i++ )
		SetOrderPrinterFlag ( i, csv.GetBool(n++) );
	
	SetDiscountFlagsFromType ( csv.GetInt(n++) );
	SetPriority ( csv.GetInt(n++) );
	SetPriceType( csv.GetInt(n++) );
	n++; //LEGACY OFFER
	n++; //LEGACY BUFFER
	SetFreeTextFlag( csv.GetBool(n++) );
	SetKPImagePrintFlag( csv.GetBool(n++) );
	SetKPImageModifierFlag( csv.GetBool(n++) );
	SetPluInfoModifierFlag( csv.GetBool(n++) );

	for ( int m = 1; m <= PLU_MODIFIERS; m++ )
	{
		int nEnable = csv.GetInt(n++);
		SetModifierEnable( m, ( nEnable == 1 ) );

		if ( nEnable != 2 )
		{
			{
				bool b = csv.GetBool(n++);
				CString str = csv.GetString(n++);
				SetModifierEposText( m, b, str );
			}

			{
				bool b = csv.GetBool(n++);
				CString str = csv.GetString(n++);
				SetModifierKeyText( m, b, str );
			}

			SetModifierOrderPrinterGroup( m, csv.GetInt(n++) );

			for ( int l = 0; l < 10; l++ )
				SetModifierPrice( m, l, csv.GetDouble(n++) );
		}
	}

	SetModifiedDateTimePlu( csv.GetString(n++) );
	SetModifiedDateTimeInfo( csv.GetString(n++) );
	SetDownloadDatePlu( csv.GetString(n++) );
	SetExport ( csv.GetBool(n++) );
	SetRepText ( csv.GetString(n++) );
	SetSortCode( csv.GetString(n++) );
	SetPurchaseCost ( csv.GetDouble(n++) );
	SetReportCostEnable( csv.GetBool(n++) );
	SetReportCost ( csv.GetDouble(n++) );
	SetGPType ( csv.GetInt(n++) );
	SetProfitMargin ( csv.GetDouble(n++) );
	SetApparentLevel ( csv.GetDouble(n++) );
	SetMinimumLevel ( csv.GetDouble(n++) );
	SetUnitPriceText ( csv.GetString(n++) );
	SetUnitPriceSize ( csv.GetDouble(n++) );
	SetSupplierRef( csv.GetString(n++) );
	SetSupplierNo( csv.GetInt(n++) );
	SetNonStockInternalFlag( csv.GetBool(n++) );
	SetPrintShelfFlag( csv.GetBool(n++ ) );
	SetPrintProductFlag( csv.GetBool(n++ ) );
	SetPoints( csv.GetInt(n++) );
	SetSalesTrackingFlag( csv.GetBool(n++) );
	SetEANOnReceiptFlag( csv.GetBool(n++) );
	SetFreeTextDescriptionFlag( csv.GetBool(n++) );
	SetLinkType( csv.GetInt(n++) );
	SetLinkTableString( csv.GetString(n++) );
	SetEcrmanRecipeTableString( csv.GetString(n++) );
	SetAgeVerificationType( csv.GetInt(n++) );

	if ( nVer >= 32 )
		SetKPNotAloneFlag( csv.GetBool(n++) );

	SetKPConsolidateTopItemFlag( csv.GetBool(n++) );
	SetAutoCoversQtyFlag( csv.GetBool(n++) );	
	SetAgressoGLCode( csv.GetInt(n++) );
	SetAgressoCostCentre( csv.GetString(n++) );
	SetAgressoProject( csv.GetString(n++) );

	if ( nVer >= 28 )
		SetSoldOutFlag( csv.GetBool(n++) );

	if ( nVer >= 29 )
		SetImageFilename( csv.GetString(n++) );
}

/********************************************************/

void CPluCSVRecord::SPOSV30To36ConvertFromCSV ( CCSV& csv, int nVer )
{
	int n = 0;

	SetPluNo ( csv.GetInt64(n++) );
	n++;
	SetEposText ( csv.GetString(n++) );
	SetKeyText ( csv.GetString(n++) );

	for ( int i = 0 ; i < 10 ; i++ )
		SetPrice ( i, csv.GetDouble(n++) );			
	
	SetBaseDeptNo ( csv.GetInt(n++) );
	SetBaseTaxBandString ( csv.GetString(n++) );
	SetMicrotrendPluType ( csv.GetInt(n++) );
	SetExternalProcess( csv.GetInt(n++) );

	SetBaseAnalysisCategory ( csv.GetInt(n++) );

	for ( int i = 0 ; i < 6 ; i++ )
		SetAllergenNo ( i, csv.GetInt(n++) );
	
	SetFirstMenu ( csv.GetInt(n++) );
	SetFirstQualifier ( csv.GetInt(n++) );
	SetFirstExtra ( csv.GetInt(n++) );
	SetOrderPrinterGroup( csv.GetInt(n++) );
	
	for ( int i = 0; i < PLU_ORDER_PRINTERS_PRE_V39; i++ )
		SetOrderPrinterFlag ( i, csv.GetBool(n++) );
	
	SetDiscountFlagsFromType ( csv.GetInt(n++) );
	SetPriority ( csv.GetInt(n++) );
	SetPriceType( csv.GetInt(n++) );
	n++; //LEGACY OFFER
	n++; //LEGACY BUFFER
	SetFreeTextFlag( csv.GetBool(n++) );
	SetKPImagePrintFlag( csv.GetBool(n++) );
	SetKPImageModifierFlag( csv.GetBool(n++) );
	SetPluInfoModifierFlag( csv.GetBool(n++) );

	for ( int m = 1; m <= PLU_MODIFIERS; m++ )
	{
		int nEnable = csv.GetInt(n++);
		SetModifierEnable( m, ( nEnable == 1 ) );

		if ( nEnable != 2 )
		{
			{
				bool b = csv.GetBool(n++);
				CString str = csv.GetString(n++);
				SetModifierEposText( m, b, str );
			}

			{
				bool b = csv.GetBool(n++);
				CString str = csv.GetString(n++);
				SetModifierKeyText( m, b, str );
			}

			SetModifierOrderPrinterGroup( m, csv.GetInt(n++) );
			SetModifierTaxBandStringRaw( m, csv.GetString(n++) );

			if ( nVer >= 34 )
			{
				SetModifierAnalysisCategoryRaw( m, csv.GetInt(n++) );
			}

			if ( nVer >= 36 )
			{
				SetModifierMicrotrendPluType( m, csv.GetInt(n++) );
			}

			if ( nVer >= 35 )
			{
				SetModifierFirstMenu( m, csv.GetInt(n++) );
				SetModifierFirstQualifier( m, csv.GetInt(n++) );
				SetModifierFirstExtra( m, csv.GetInt(n++) );
			}

			for ( int l = 0; l < 10; l++ )
				SetModifierPrice( m, l, csv.GetDouble(n++) );
		}
	}

	SetModifiedDateTimePlu( csv.GetString(n++) );
	SetModifiedDateTimeInfo( csv.GetString(n++) );
	SetDownloadDatePlu( csv.GetString(n++) );
	SetExport ( csv.GetBool(n++) );
	SetRepText ( csv.GetString(n++) );
	SetSortCode( csv.GetString(n++) );
	SetPurchaseCost ( csv.GetDouble(n++) );
	SetReportCostEnable( csv.GetBool(n++) );
	SetReportCost ( csv.GetDouble(n++) );
	SetGPType ( csv.GetInt(n++) );
	SetProfitMargin ( csv.GetDouble(n++) );
	SetApparentLevel ( csv.GetDouble(n++) );
	SetMinimumLevel ( csv.GetDouble(n++) );
	SetUnitPriceText ( csv.GetString(n++) );
	SetUnitPriceSize ( csv.GetDouble(n++) );
	SetSupplierRef( csv.GetString(n++) );
	SetSupplierNo( csv.GetInt(n++) );
	SetNonStockInternalFlag( csv.GetBool(n++) );
	SetPrintShelfFlag( csv.GetBool(n++ ) );
	SetPrintProductFlag( csv.GetBool(n++ ) );
	SetPoints( csv.GetInt(n++) );
	SetSalesTrackingFlag( csv.GetBool(n++) );
	SetEANOnReceiptFlag( csv.GetBool(n++) );
	SetFreeTextDescriptionFlag( csv.GetBool(n++) );
	SetLinkType( csv.GetInt(n++) );
	SetLinkTableString( csv.GetString(n++) );
	SetEcrmanRecipeTableString( csv.GetString(n++) );
	SetAgeVerificationType( csv.GetInt(n++) );

	if ( nVer >= 33 )
		SetKPNotAloneFlag( csv.GetBool(n++) );

	SetKPConsolidateTopItemFlag( csv.GetBool(n++) );
	SetAutoCoversQtyFlag( csv.GetBool(n++) );	
	SetAgressoGLCode( csv.GetInt(n++) );
	SetAgressoCostCentre( csv.GetString(n++) );
	SetAgressoProject( csv.GetString(n++) );
	SetSoldOutFlag( csv.GetBool(n++) );
	SetImageFilename( csv.GetString(n++) );

	if ( nVer >= 31 )
		SetActiveItemFlag( csv.GetBool(n++) );
}

/********************************************************/

void CPluCSVRecord::SPOSV37To38ConvertFromCSV(CCSV& csv, int nVer)
{
	int n = 0;

	//DESCRIPTION AND TYPE
	SetPluNo(csv.GetInt64(n++));
	n++;
	SetEposText(csv.GetString(n++));
	SetKeyText(csv.GetString(n++));
	SetRepText(csv.GetString(n++));
	SetSortCode(csv.GetString(n++));
	SetAgressoGLCode(csv.GetInt(n++));
	SetAgressoCostCentre(csv.GetString(n++));
	SetAgressoProject(csv.GetString(n++));
	SetBaseDeptNo(csv.GetInt(n++));
	SetBaseAnalysisCategory(csv.GetInt(n++));
	SetMicrotrendPluType(csv.GetInt(n++));
	SetExternalProcess(csv.GetInt(n++));
	SetBaseTaxBandString(csv.GetString(n++));

	//PRICES AND POINTS
	for (int i = 0; i < 10; i++)
	{
		SetPrice(i, csv.GetDouble(n++));
	}

	SetPoints(csv.GetInt(n++));

	//COST AND PROFIT
	SetPurchaseCost(csv.GetDouble(n++));
	SetReportCostEnable(csv.GetBool(n++));
	SetReportCost(csv.GetDouble(n++));
	SetGPType(csv.GetInt(n++));
	SetProfitMargin(csv.GetDouble(n++));

	//ALLERGENS
	for (int i = 0; i < 6; i++)
	{
		SetAllergenNo(i, csv.GetInt(n++));
	}

	//KEYBOARD LISTS
	SetFirstMenu(csv.GetInt(n++));
	SetFirstQualifier(csv.GetInt(n++));
	SetFirstExtra(csv.GetInt(n++));

	//ORDER PRINTER GROUPS
	SetOrderPrinterGroup(csv.GetInt(n++));

	for (int i = 0; i < PLU_ORDER_PRINTERS_PRE_V39; i++)
	{
		SetOrderPrinterFlag(i, csv.GetBool(n++));
	}

	SetPriority(csv.GetInt(n++));

	//MODIFIERS
	for (int m = 1; m <= PLU_MODIFIERS; m++)
	{
		int nEnable = csv.GetInt(n++);
		SetModifierEnable(m, (nEnable == 1));

		if (nEnable != 2)
		{
			{
				bool b = csv.GetBool(n++);
				CString str = csv.GetString(n++);
				SetModifierEposText(m, b, str);
			}

			{
				bool b = csv.GetBool(n++);
				CString str = csv.GetString(n++);
				SetModifierKeyText(m, b, str);
			}

			SetModifierDeptNoRaw(m, csv.GetInt(n++));
			SetModifierAnalysisCategoryRaw(m, csv.GetInt(n++));
			SetModifierMicrotrendPluType(m, csv.GetInt(n++));
			SetModifierTaxBandStringRaw(m, csv.GetString(n++));

			for (int l = 0; l < 10; l++)
			{
				SetModifierPrice(m, l, csv.GetDouble(n++));
			}

			SetModifierFirstMenu(m, csv.GetInt(n++));
			SetModifierFirstQualifier(m, csv.GetInt(n++));
			SetModifierFirstExtra(m, csv.GetInt(n++));
			SetModifierOrderPrinterGroup(m, csv.GetInt(n++));
		}
	}

	//MISC PROGRAMMING
	SetPriceType(csv.GetInt(n++));

	if (37 == nVer)
	{
		SetDiscountFlagsFromType(csv.GetInt(n++));
	}

	SetAgeVerificationType(csv.GetInt(n++));
	SetKPNotAloneFlag(csv.GetBool(n++));
	SetKPConsolidateTopItemFlag(csv.GetBool(n++));
	SetKPImagePrintFlag(csv.GetBool(n++));
	SetKPImageModifierFlag(csv.GetBool(n++));
	SetAutoCoversQtyFlag(csv.GetBool(n++));
	SetEANOnReceiptFlag(csv.GetBool(n++));
	SetSalesTrackingFlag(csv.GetBool(n++));
	SetSoldOutFlag(csv.GetBool(n++));

	if (nVer >= 38)
	{
		SetDiscountableFlag(csv.GetBool(n++));
		SetStaffDiscountFlag(csv.GetBool(n++));
	}

	SetFreeTextFlag(csv.GetBool(n++));
	SetFreeTextDescriptionFlag(csv.GetBool(n++));

	//LABELS
	SetPrintShelfFlag(csv.GetBool(n++));
	SetPrintProductFlag(csv.GetBool(n++));
	SetUnitPriceText(csv.GetString(n++));
	SetUnitPriceSize(csv.GetDouble(n++));

	//ECRMAN STOCK
	SetApparentLevel(csv.GetDouble(n++));
	SetMinimumLevel(csv.GetDouble(n++));
	SetSupplierRef(csv.GetString(n++));
	SetSupplierNo(csv.GetInt(n++));
	SetNonStockInternalFlag(csv.GetBool(n++));

	//MISC
	SetPluInfoModifierFlag(csv.GetBool(n++));
	SetLinkType(csv.GetInt(n++));
	SetLinkTableString(csv.GetString(n++));
	SetEcrmanRecipeTableString(csv.GetString(n++));
	SetImageFilename(csv.GetString(n++));
	SetActiveItemFlag(csv.GetBool(n++));

	//LEGACY
	if (37 == nVer)
	{
		n++;	//SKIP LEGACY OFFER
		n++;	//SKIP LEGACY BUCKET
	}

	//INTERNAL FIELDS
	SetModifiedDateTimePlu(csv.GetString(n++));
	SetModifiedDateTimeInfo(csv.GetString(n++));
	SetDownloadDatePlu(csv.GetString(n++));
	SetExport(csv.GetBool(n++));
}

/********************************************************/
