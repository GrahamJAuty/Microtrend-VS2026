/**********************************************************************/
#include "MaxLengths.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EcrmanOptionsIni.h"
/**********************************************************************/

CEcrmanOptions::CEcrmanOptions()
{
	//PLU TAB
	m_PluTabBarcode.Create			( "PluTabBarcode", TRUE );
	m_PluTabLink.Create				( "PluTabLink", TRUE );
	m_PluTabInfo.Create				( "PluTabInfo", TRUE );
	m_PluPriceSet.Create			( "PluPriceSets", FALSE );
	m_PluDownloadDate.Create		( "PluDownloadByDate", FALSE );
	m_PluTask.Create				( "PluTaskAfterChanges", FALSE );
	m_PluInactiveNoDownload.Create	( "PluInactiveNoDownload", FALSE );
	m_PluInactiveAutoHide.Create	( "PluInactiveAutoHide", FALSE );
	m_PluDefText.Create				( "PluDefaultTextType", 0, 2, 0 );
	m_PluDiscardMod.Create			( "PluDiscardUnusedModifier", TRUE );
	m_PluAutoImport.Create			( "PluCSVAutoImport", TRUE );
	m_PluManImport.Create			( "PluCSVManualImport", TRUE );
	m_PluManExport.Create			( "PluCSVManualExport", TRUE );
	m_PluPurchase.Create			( "PluExportPurchaseHistory", FALSE );
	m_PluProdCSV.Create				( "PluProdCSVImportType", 0, 2, 0 );
	m_PluDelBarcode.Create			( "PluAutoDeleteBarcodeType", 0, 3, 0 );
	m_PluBarcodeMod.Create			( "PluBarcodeModifier", TRUE );
	m_PluAutoModEposText.Create		( "PluAutoModEposTextType", 0, 1, 0 );
	m_PluAutoModKeyText.Create		( "PluAutoModKeyTextType", 0, 1, 0 );

	//GLOBAL TAB
	m_GlobalDept.Create				( "GlobalDepartment", 0, 3, 0 );
	m_GlobalDeptGroupEpos.Create	( "GlobalGroup", FALSE );
	m_GlobalTax.Create				( "GlobalTax", FALSE );
	m_GlobalMod.Create				( "GlobalModifier", FALSE );
	m_GlobalACatLegacy.Create		( "GlobalPluCategory", FALSE );
	m_GlobalACat.Create				( "GlobalAnalysisCategory", 0, 2, 2 );
	m_GlobalPay.Create				( "GlobalPayment", 0, 2, 0 );
	m_GlobalPaidIO.Create			( "GlobalPaidInOut", FALSE );

	//FEATURES
	m_FeaturesPluStock.Create		( "FeaturesPluStock", FALSE );
	m_FeaturesPluAudit.Create		( "FeaturesPluAudit", FALSE );
	m_FeaturesLegacyEposDelivery.Create( "FeaturesPluDeliveryType", 0, 4, 0 );
	m_FeaturesEposDelivery.Create	( "FeaturesEposDeliveryType", -1, 10, -1 );
	m_FeaturesMixMatch.Create		( "FeaturesMixMatch", TRUE );
	m_FeaturesPromotion.Create		( "FeaturesPromotion", TRUE );
	m_FeaturesTimePlan.Create		( "FeaturesTimePlan", TRUE );
	m_FeaturesTimePlanDL.Create		( "FeaturesTimePlanDownload", TRUE );
	m_FeaturesTimePlanTemplate.Create("FeaturesTimePlanTemplate", FALSE);
	m_FeaturesWalletStampRule.Create("FeaturesWalletStampRule", TRUE);
	m_FeaturesGlobalWalletStampRule.Create("FeaturesGlobalWalletStampRule", FALSE);
	m_FeaturesKeyboard.Create		( "FeaturesKeyboard", TRUE );
	m_FeaturesLocKeyboards.Create	( "FeaturesKeyboardPerLocation", 1, MAX_KEYBOARDS_PER_LOCATION, 1 );
	m_FeaturesKBShare.Create		( "FeaturesKeyboardShare", FALSE );
	m_FeaturesKBPriceSet.Create		( "FeaturesKeyboardPriceSet", FALSE );
	m_FeaturesKBTextOrder.Create	( "FeaturesKeyboardTextOrder", 0, 5, 0 );
	m_FeaturesServerType.Create		( "FeaturesServerSettingsType", 0, 3, SERVER_SYS );
	m_FeaturesServerRestrict.Create	( "FeaturesServerRestriction", FALSE );
	m_FeaturesServerPreset.Create	( "FeaturesServerPreset", FALSE );
	m_FeaturesServerLogon.Create	( "FeaturesServerLogon", FALSE);
	m_FeaturesCustomerBal.Create	( "FeaturesCustomerBalance", FALSE );
	m_FeaturesCustomerName.Create	( "FeaturesCustomerNameImportType", 0, 2, 0 );
	m_FeaturesCustomerAccountDelDay.Create	( "FeaturesCustomerAccountDelDay", 0, 99, 0 );
	m_FeaturesCustomerOneOffDelDay.Create	( "FeaturesCustomerOneOffDelDay", 0, 99, 0 );
	m_FeaturesDBaseEdit.Create		( "FeaturesDatabaseEditType", 0, 2, 2 );
	m_FeaturesBackCol.Create		( "FeaturesBackgroundColour", 0x0, 0xFFFFFF, 0x80C080 );
	m_FeaturesContextNow.Create		( "FeaturesContextForceNow", FALSE );
	m_FeaturesOPNameFirst.Create	( "FeaturesOPNameFirst", FALSE);
	m_FeaturesAllergen.Create		( "FeaturesAllergen", FALSE );

	//REPORTS1
	m_ReportsFYearStart.Create			( "ReportsFinancialYearStart", 4, "0101" );
	m_ReportsTaxQStart.Create			( "ReportsVatQuarterStart", 0, 2, 0 );
	m_ReportsWeekStart.Create			( "ReportsWeekStartDay", 0, 6, 0 );
	m_ReportsWeekNumber.Create			( "ReportsWeekNumberingType", 0, 1, 1 );
	m_ReportsDayHour.Create				( "ReportsDayStartHour", 0, 23, 0 );
	m_ReportsDayMinute.Create			( "ReportsDayStartMinute", 0, 59, 0 );
	m_ReportsTimeAttendHour.Create		( "ReportsTimeAttendSignOffHour", 0, 23, 0 );
	m_ReportsTimeAttendMinute.Create	( "ReportsTimeAttendSignOffMinute", 0, 59, 0 );
	m_ReportsLocSort.Create				( "ReportsLocationSortType", 0, 2, 0 );
	m_ReportsSalesTaxType.Create		( "ReportsSalesTaxType", 0, 2, 0 );
	m_ReportsRepPoint.Create			( "ReportsReportpoint", FALSE );
	m_ReportsLocSet.Create				( "ReportsLocationSet", FALSE );
	m_ReportsTrimZero.Create			( "ReportsTrimPluZeros", FALSE );
	m_ReportsRegion.Create				( "ReportsRegionDateRange", TRUE );
	m_ReportsCustTran.Create			( "ReportsCustomerTransaction", FALSE );
	m_ReportsCustState.Create			( "ReportsCustomerStatement", FALSE );
	m_ReportsCustContact.Create			( "ReportsCustomerContact", FALSE );
	m_ReportsContactBeforeTime.Create	( "ReportsContactBeforeTime", 0, 60, 0 );
	m_ReportsContactAfterTime.Create	( "ReportsContactAfterTime", 0, 60, 0 );
	m_ReportsContactVisitTime.Create	( "ReportsContactVisitTime", 0, 999, 60 );
	m_ReportsDateRange.Create			( "ReportsDefaultDateRange", 50, "Today" );
	m_ReportsSingleDayRanges.Create		( "ReportsSingleDayRanges", 0, 3, 0 );
	m_ReportsPreviousDayRanges.Create	( "ReportsPreviousDayRanges", 0, 3, 0 );
	m_ReportsEODSession.Create			( "ReportsEODSession", FALSE );
	m_ReportsEcrLiteLogMode.Create		( "ReportsEcrLiteLogMode", 0, 2, 0 );

	//REPORTS2
	m_ReportsVIPType.Create					( "ReportsVIPReportType", 0, 2, 0 );
	m_ReportsVIPLevel.Create				( "ReportsVIPReportLevel", 0, 10, 0 );
	m_ReportsVIPRetail.Create				( "ReportsVIPRetail", FALSE );
	m_ReportsCustSortType.Create			( "ReportsCustomerSortType", 0, 2, 0 );
	m_ReportsSalesTimeType.Create			( "ReportsSalesTimeType", 0, 1, 0 );
	m_ReportsDiscountDetailSales.Create		( "ReportsShowDiscount", FALSE );
	m_ReportsPremiumDetailSales.Create		( "ReportsShowPremium", FALSE);
	m_ReportsNetOfTaxSales.Create			( "ReportsShowNetOfVat", FALSE );
	m_ReportsDiscountDetailCovers.Create	( "ReportsShowDiscountCovers", 0, 2, 0 );
	m_ReportsPremiumDetailCovers.Create		( "ReportsShowPremiumCovers", FALSE );
	m_ReportsNetOfTaxCovers.Create			( "ReportsShowNetOfVatCovers", FALSE );
	m_ReportsCashRSPItemText.Create			( "ReportsCashRSPItemText", FALSE );
	m_ReportsAuthServer.Create				( "ReportsUseAuthServer", FALSE );
	m_ReportsGroupReportpoint.Create		( "ReportsGroupReportpoint", FALSE );
	m_ReportsEODReasons.Create				( "ReportsEODReasons", FALSE );
	m_ReportsGeneralPage.Create				( "ReportsGeneralPage", FALSE );
	m_ReportsChange.Create					( "ReportsShowCashChange", TRUE );
	m_ReportsMultiCash.Create				( "ReportsShowMultiCashTypes", 0, 3, 0 );
	m_ReportsPayQty.Create					( "ReportsShowPaymentQuantity", FALSE );
	m_ReportsRAPay.Create					( "ReportsShowRAPayments", TRUE );
	m_ReportsSplitRAPay.Create				( "ReportsSeparateRAPayments", FALSE );
	m_ReportsRABal.Create					( "ReportsShowRABalance", FALSE );
	m_ReportsSplitRABal.Create				( "ReportsSeparateRABalance", FALSE );
	m_ReportsCashRSPDiscountPenny.Create	( "ReportsCashRSPDiscountPenny", FALSE );

	//AUTO EXPIRE
	m_ExpireSales.Create			( "AutoExpireTimePluReports", 0, 999, 0 );
	m_ExpireAudit.Create			( "AutoExpireTimePluDeliveryAudit", 0, 999, 0 );
	m_ExpireSummary.Create			( "AutoExpireTimeSummaryReports", 0, 999, 0 );
	m_ExpireDetail.Create			( "AutoExpireTimeDetailReports", 0, 999, 0 );
	//m_ExpireTimeAttend.Create		( "AutoExpireTimeAttendReports", 0, 999, 0);

	//LABELS1
	m_LabelsShelfPlu.Create			( "LabelsShelfShowPlu", TRUE );
	m_LabelsShelfStock.Create		( "LabelsShelfShowStockCode", FALSE );
	m_LabelsShelfSupp.Create		( "LabelsShelfShowSuppRef", FALSE );
	m_LabelsShelfTrimZero.Create	( "LabelsShelfTrimPluZeros", FALSE );
	m_LabelsShelfPrice1.Create		( "LabelsShelfPriceLevel1", 1, 4, 1 );
	m_LabelsShelfPrice2.Create		( "LabelsShelfPriceLevel2", 0, 4, 0 );
	m_LabelsShelfTax1.Create		( "LabelsShelfVatType1", 0, 2, 0 );
	m_LabelsShelfTax2.Create		( "LabelsShelfVatType2", 0, 2, 0 );
	m_LabelsShelfSymbol1.Create		( "LabelsShelfPriceSymbol1", MAX_LENGTH_CURRENCY_SYMBOL, "£" );
	m_LabelsShelfSymbol2.Create		( "LabelsShelfPriceSymbol2", MAX_LENGTH_CURRENCY_SYMBOL, "£" );
	m_LabelsProdPlu.Create			( "LabelsProductShowPlu", TRUE );
	m_LabelsProdStock.Create		( "LabelsProductShowStockCode", FALSE );
	m_LabelsProdSupp.Create			( "LabelsProductShowSuppRef", FALSE );
	m_LabelsProdTrimZero.Create		( "LabelsProductTrimPluZeros", FALSE );
	m_LabelsProdPrice1.Create		( "LabelsProductPriceLevel1", 1, 4, 1 );
	m_LabelsProdPrice2.Create		( "LabelsProductPriceLevel2", 0, 4, 0 );
	m_LabelsProdTax1.Create			( "LabelsProductVatType1", 0, 2, 0 );
	m_LabelsProdTax2.Create			( "LabelsProductVatType2", 0, 2, 0 );
	m_LabelsProdSymbol1.Create		( "LabelsProductPriceSymbol1", MAX_LENGTH_CURRENCY_SYMBOL, "£" );
	m_LabelsProdSymbol2.Create		( "LabelsProductPriceSymbol2", MAX_LENGTH_CURRENCY_SYMBOL, "£" );
	
	//LABELS2
	m_LabelsShelfNameSingle.Create	( "LabelsShelfNameSingle", 100, "" );
	m_LabelsShelfNameBatch.Create	( "LabelsShelfNameBatch", 100, "" );
	m_LabelsShelfPromptBatch.Create	( "LabelsShelfPromptBatch", FALSE );
	m_LabelsShelfAutoBatch.Create	( "LabelsShelfAutoBatch", FALSE );
	m_LabelsShelfPrintSingle.Create	( "LabelsShelfPrintSingle", FALSE );
	m_LabelsShelfPromptSingle.Create( "LabelsShelfPromptSingle", FALSE );
	m_LabelsProdNameSingle.Create	( "LabelsProductNameSingle", 100, "" );
	m_LabelsProdNameBatch.Create	( "LabelsProductNameBatch", 100, "" );
	m_LabelsProdAutoBatch.Create	( "LabelsProductAutoBatch", FALSE );
	m_LabelsProdPrintSingle.Create	( "LabelsProductPrintSingle", FALSE );
	m_LabelsProdPromptSingle.Create	( "LabelsProductPromptSingle", FALSE );

	//CUSTOM MESSAGE BOX
	m_CustomMessageBox.Create			( "CustomMessageBox", TRUE );
	m_CustomMessageBoxFont.Create		( "CustomMessageBoxFont", 8, 14, 8 );
	m_CustomMessageBoxLineGap.Create	( "CustomMessageBoxLineGap", 5, 15, 5 );
	m_CustomMessageBoxColourBack.Create	( "CustomMessageBoxColourBack", 6, "FFFFFF" );
	m_CustomMessageBoxColourText.Create	( "CustomMessageBoxColourText", 6, "000000" );
	
	//STARTUP MESSAGE BOX
	m_StartupMessageBox.Create("StartupMessageBox", TRUE);
	m_StartupMessageBoxFont.Create("StartupMessageBoxFont", 8, 14, 8);
	m_StartupMessageBoxLineGap.Create("StartupMessageBoxLineGap", 5, 15, 5);
	m_StartupMessageBoxColourBack.Create("StartupMessageBoxColourBack", 6, "FFFFFF");
	m_StartupMessageBoxColourText.Create("StartupMessageBoxColourText", 6, "000000");

	//PASSWORDS
	m_PasswordsLog.Create			( "PasswordsLog", FALSE );

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CEcrmanOptions::SetFeaturesDBaseEditType( int n )
{
	switch(n)
	{
	case 0:
	case 2:
		m_FeaturesDBaseEdit.SetValue(n); 
		break;
	}
}

/**********************************************************************/

void CEcrmanOptions::SetCustomMessageBoxFont( int n )
{
	switch(n)
	{
	case 8:
	case 10:
	case 11:
	case 12:
	case 14:
		m_CustomMessageBoxFont.SetValue(n);
		break;

	default:
		m_CustomMessageBoxFont.SetValue(10);
		break;
	}
}

/**********************************************************************/

void CEcrmanOptions::SetStartupMessageBoxFont(int n)
{
	switch (n)
	{
	case 8:
	case 10:
	case 11:
	case 12:
	case 14:
		m_StartupMessageBoxFont.SetValue(n);
		break;

	default:
		m_StartupMessageBoxFont.SetValue(10);
		break;
	}
}

/**********************************************************************/

int CEcrmanOptions::MakeColourInt( CIniFileString& iniString )
{
	int nResult = 0xFFFFFF;

	CString strColour = iniString.GetValue();
	if ( strColour.GetLength() == 6 )
	{
		int n = ::HexToInt( strColour.Right(2) + strColour.Mid(2,2) + strColour.Left(2) );
		if ((n >= 0) && (n <= 0xFFFFFF))
		{
			nResult = n;
		}
	}

	return nResult;
}

/**********************************************************************/

bool CEcrmanOptions::MakeColourString( int nColour, CString& strColour )
{
	bool bResult = FALSE;
	if ( ( nColour >= 0 ) && ( nColour <= 0xFFFFFF ) )
	{
		CString strHex = "";
		strHex.Format( "%6.6X", nColour );
		strColour = strHex.Right(2) + strHex.Mid(2,2) + strHex.Left(2);
		bResult = TRUE;
	}
	return bResult;
}

/**********************************************************************/

void CEcrmanOptions::SetCustomMessageBoxColourTextInt( int n )
{
	CString strColour = "";
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_CustomMessageBoxColourText.SetValue( strColour );
		m_nCustomMessageBoxColourText = MakeColourInt( m_CustomMessageBoxColourText );
	}
}

/**********************************************************************/

void CEcrmanOptions::SetCustomMessageBoxColourBackInt( int n )
{
	CString strColour = "";
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_CustomMessageBoxColourBack.SetValue( strColour );
		m_nCustomMessageBoxColourBack = MakeColourInt( m_CustomMessageBoxColourBack );
	}
}

/**********************************************************************/

void CEcrmanOptions::SetStartupMessageBoxColourTextInt(int n)
{
	CString strColour = "";
	if (MakeColourString(n, strColour) == TRUE)
	{
		m_StartupMessageBoxColourText.SetValue(strColour);
		m_nStartupMessageBoxColourText = MakeColourInt(m_StartupMessageBoxColourText);
	}
}

/**********************************************************************/

void CEcrmanOptions::SetStartupMessageBoxColourBackInt(int n)
{
	CString strColour = "";
	if (MakeColourString(n, strColour) == TRUE)
	{
		m_StartupMessageBoxColourBack.SetValue(strColour);
		m_nStartupMessageBoxColourBack = MakeColourInt(m_StartupMessageBoxColourBack);
	}
}

/**********************************************************************/

void CEcrmanOptions::CopyFrom( CEcrmanOptions& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CEcrmanOptions::ReloadFromIniFile(CEnhancedIniFile& iniFile)
{
	//PLU TAB
	iniFile.ReadBool(m_PluTabBarcode);
	iniFile.ReadBool(m_PluTabLink);
	iniFile.ReadBool(m_PluTabInfo);
	iniFile.ReadBool(m_PluPriceSet);
	iniFile.ReadBool(m_PluDownloadDate);
	iniFile.ReadBool(m_PluInactiveNoDownload);
	iniFile.ReadBool(m_PluInactiveAutoHide);
	iniFile.ReadBool(m_PluTask);
	iniFile.ReadInt(m_PluDefText);
	iniFile.ReadBool(m_PluDiscardMod);
	iniFile.ReadBool(m_PluAutoImport);
	iniFile.ReadBool(m_PluManImport);
	iniFile.ReadBool(m_PluManExport);
	iniFile.ReadBool(m_PluPurchase);
	iniFile.ReadInt(m_PluProdCSV);
	iniFile.ReadInt(m_PluDelBarcode);
	iniFile.ReadBool(m_PluBarcodeMod);
	iniFile.ReadInt(m_PluAutoModEposText);
	iniFile.ReadInt(m_PluAutoModKeyText);

	//GLOBAL TAB
	iniFile.ReadInt(m_GlobalDept);
	iniFile.ReadBool(m_GlobalDeptGroupEpos);
	iniFile.ReadBool(m_GlobalTax);
	iniFile.ReadBool(m_GlobalMod);
	iniFile.ReadBool(m_GlobalACatLegacy);
	iniFile.ReadInt(m_GlobalACat);
	iniFile.ReadInt(m_GlobalPay);
	iniFile.ReadBool(m_GlobalPaidIO);

	if (m_GlobalACat.GetValue() == 2)
		m_GlobalACat.SetValue(m_GlobalACatLegacy.GetValue() ? 1 : 0);

	//FOR LEGACY REASONS, OTHER GLOBAL FLAGS ARE NEVER BLOCKED
	if (SysInfo.GetMaxDatabases() < 2)
	{
		m_GlobalDept.SetValue(0);
		m_GlobalDeptGroupEpos.SetValue(FALSE);
		m_GlobalTax.SetValue(FALSE);
		m_GlobalPaidIO.SetValue(FALSE);
	}

	//FEATURES TAB
	iniFile.ReadBool(m_FeaturesPluStock);
	iniFile.ReadBool(m_FeaturesPluAudit);
	iniFile.ReadInt(m_FeaturesLegacyEposDelivery);
	iniFile.ReadInt(m_FeaturesEposDelivery);
	iniFile.ReadBool(m_FeaturesMixMatch);
	iniFile.ReadBool(m_FeaturesPromotion);
	iniFile.ReadBool(m_FeaturesTimePlan);
	iniFile.ReadBool(m_FeaturesTimePlanDL);
	iniFile.ReadBool(m_FeaturesTimePlanTemplate);
	iniFile.ReadBool(m_FeaturesWalletStampRule);
	iniFile.ReadBool(m_FeaturesGlobalWalletStampRule);
	iniFile.ReadBool(m_FeaturesKeyboard);
	iniFile.ReadInt(m_FeaturesLocKeyboards);
	iniFile.ReadBool(m_FeaturesKBShare);
	iniFile.ReadBool(m_FeaturesKBPriceSet);
	iniFile.ReadInt(m_FeaturesKBTextOrder);
	iniFile.ReadInt(m_FeaturesServerType);
	iniFile.ReadBool(m_FeaturesServerRestrict);
	iniFile.ReadBool(m_FeaturesServerPreset);
	iniFile.ReadBool(m_FeaturesServerLogon);
	iniFile.ReadBool(m_FeaturesCustomerBal);
	iniFile.ReadInt(m_FeaturesCustomerName);
	iniFile.ReadInt(m_FeaturesCustomerAccountDelDay);
	iniFile.ReadInt(m_FeaturesCustomerOneOffDelDay);
	iniFile.ReadInt(m_FeaturesDBaseEdit);
	iniFile.ReadInt(m_FeaturesBackCol);
	iniFile.ReadBool(m_FeaturesContextNow);
	iniFile.ReadBool(m_FeaturesOPNameFirst);
	iniFile.ReadBool(m_FeaturesAllergen);

	if (m_FeaturesEposDelivery.GetValue() == -1)
	{
		int nValue = 0;
		switch (m_FeaturesLegacyEposDelivery.GetValue())
		{
		case 1:		nValue = 2;	break;	//PRICE2
		case 2:		nValue = 3;	break;	//PRICE3
		case 3:		nValue = 4;	break;	//PRICE4
		case 4:		nValue = 1;	break;	//WASTAGE
		}
		m_FeaturesEposDelivery.SetValue(nValue);
	}

	if (m_FeaturesDBaseEdit.GetValue() == 1)
	{
		m_FeaturesDBaseEdit.SetValue(0);
	}

	//REPORTS1 TAB
	iniFile.ReadString(m_ReportsFYearStart);
	iniFile.ReadInt(m_ReportsTaxQStart);
	iniFile.ReadInt(m_ReportsWeekStart);
	iniFile.ReadInt(m_ReportsWeekNumber);
	iniFile.ReadInt(m_ReportsDayHour);
	iniFile.ReadInt(m_ReportsDayMinute);
	iniFile.ReadInt(m_ReportsTimeAttendHour);
	iniFile.ReadInt(m_ReportsTimeAttendMinute);
	iniFile.ReadInt(m_ReportsLocSort);
	iniFile.ReadInt(m_ReportsSalesTaxType);
	iniFile.ReadBool(m_ReportsRepPoint);
	iniFile.ReadBool(m_ReportsLocSet);
	iniFile.ReadBool(m_ReportsTrimZero);
	iniFile.ReadBool(m_ReportsRegion);
	iniFile.ReadBool(m_ReportsCustTran);
	iniFile.ReadBool(m_ReportsCustState);
	iniFile.ReadBool(m_ReportsCustContact);
	iniFile.ReadInt(m_ReportsContactBeforeTime);
	iniFile.ReadInt(m_ReportsContactAfterTime);
	iniFile.ReadInt(m_ReportsContactVisitTime);
	iniFile.ReadString(m_ReportsDateRange);
	iniFile.ReadInt(m_ReportsSingleDayRanges);
	iniFile.ReadInt(m_ReportsPreviousDayRanges);
	iniFile.ReadBool(m_ReportsEODSession);
	iniFile.ReadInt(m_ReportsEcrLiteLogMode);

	//REPORTS2 TAB
	iniFile.ReadInt(m_ReportsVIPType);
	iniFile.ReadInt(m_ReportsVIPLevel);
	iniFile.ReadBool(m_ReportsVIPRetail);
	iniFile.ReadInt(m_ReportsCustSortType);
	iniFile.ReadInt(m_ReportsSalesTimeType);
	iniFile.ReadBool(m_ReportsDiscountDetailSales);
	iniFile.ReadBool(m_ReportsPremiumDetailSales);
	iniFile.ReadBool(m_ReportsNetOfTaxSales);
	iniFile.ReadInt(m_ReportsDiscountDetailCovers);
	iniFile.ReadBool(m_ReportsPremiumDetailCovers);
	iniFile.ReadBool(m_ReportsNetOfTaxCovers);
	iniFile.ReadBool(m_ReportsCashRSPItemText);
	iniFile.ReadBool(m_ReportsAuthServer);
	iniFile.ReadBool(m_ReportsGroupReportpoint);
	iniFile.ReadBool(m_ReportsEODReasons);
	iniFile.ReadBool(m_ReportsGeneralPage);
	iniFile.ReadBool(m_ReportsChange);
	iniFile.ReadInt(m_ReportsMultiCash);
	iniFile.ReadBool(m_ReportsPayQty);
	iniFile.ReadBool(m_ReportsRAPay);
	iniFile.ReadBool(m_ReportsSplitRAPay);
	iniFile.ReadBool(m_ReportsRABal);
	iniFile.ReadBool(m_ReportsSplitRABal);
	iniFile.ReadBool(m_ReportsCashRSPDiscountPenny);

	if (m_ReportsVIPLevel.GetValue() == -1)
		m_ReportsVIPLevel.SetValue(4);

	if (m_ReportsDiscountDetailCovers.GetValue() == 0)
		m_ReportsDiscountDetailCovers.SetValue(m_ReportsDiscountDetailSales.GetValue() ? 2 : 1);

	//AUTO EXPIRE TAB
	iniFile.ReadInt(m_ExpireSales);
	iniFile.ReadInt(m_ExpireAudit);
	iniFile.ReadInt(m_ExpireSummary);
	iniFile.ReadInt(m_ExpireDetail);
	//iniFile.ReadInt(m_ExpireTimeAttend);

	//LABELS1 TAB
	iniFile.ReadBool(m_LabelsShelfPlu);
	iniFile.ReadBool(m_LabelsShelfStock);
	iniFile.ReadBool(m_LabelsShelfSupp);
	iniFile.ReadBool(m_LabelsShelfTrimZero);
	iniFile.ReadInt(m_LabelsShelfPrice1);
	iniFile.ReadInt(m_LabelsShelfPrice2);
	iniFile.ReadInt(m_LabelsShelfTax1);
	iniFile.ReadInt(m_LabelsShelfTax2);
	iniFile.ReadString(m_LabelsShelfSymbol1);
	iniFile.ReadString(m_LabelsShelfSymbol2);
	iniFile.ReadBool(m_LabelsProdPlu);
	iniFile.ReadBool(m_LabelsProdStock);
	iniFile.ReadBool(m_LabelsProdSupp);
	iniFile.ReadBool(m_LabelsProdTrimZero);
	iniFile.ReadInt(m_LabelsProdPrice1);
	iniFile.ReadInt(m_LabelsProdPrice2);
	iniFile.ReadInt(m_LabelsProdTax1);
	iniFile.ReadInt(m_LabelsProdTax2);
	iniFile.ReadString(m_LabelsProdSymbol1);
	iniFile.ReadString(m_LabelsProdSymbol2);

	//LABELS2 TAB
	iniFile.ReadString(m_LabelsShelfNameSingle);
	iniFile.ReadString(m_LabelsShelfNameBatch);
	iniFile.ReadBool(m_LabelsShelfAutoBatch);
	iniFile.ReadBool(m_LabelsShelfPromptBatch);
	iniFile.ReadBool(m_LabelsShelfPrintSingle);
	iniFile.ReadBool(m_LabelsShelfPromptSingle);
	iniFile.ReadString(m_LabelsProdNameSingle);
	iniFile.ReadString(m_LabelsProdNameBatch);
	iniFile.ReadBool(m_LabelsProdAutoBatch);
	iniFile.ReadBool(m_LabelsProdPrintSingle);
	iniFile.ReadBool(m_LabelsProdPromptSingle);

	//CUSTOM MESSAGE BOX
	iniFile.ReadBool(m_CustomMessageBox);
	iniFile.ReadInt(m_CustomMessageBoxFont);
	iniFile.ReadInt(m_CustomMessageBoxLineGap);
	SetCustomMessageBoxFont(GetCustomMessageBoxFont());
	iniFile.ReadString(m_CustomMessageBoxColourBack);
	iniFile.ReadString(m_CustomMessageBoxColourText);

	//STARTUP MESSAGE BOX
	iniFile.ReadBool(m_StartupMessageBox);
	iniFile.ReadInt(m_StartupMessageBoxFont);
	iniFile.ReadInt(m_StartupMessageBoxLineGap);
	SetStartupMessageBoxFont(GetStartupMessageBoxFont());
	iniFile.ReadString(m_StartupMessageBoxColourBack);
	iniFile.ReadString(m_StartupMessageBoxColourText);

	//PASSWORDS
	iniFile.ReadBool(m_PasswordsLog);

	MakeIntSettings();
}

/**********************************************************************/

void CEcrmanOptions::PrepareIniFile(CEnhancedIniFile& iniFile)
{
	//PLU TAB 
	iniFile.WriteBool(m_PluTabBarcode);
	iniFile.WriteBool(m_PluTabLink);
	iniFile.WriteBool(m_PluTabInfo);
	iniFile.WriteBool(m_PluPriceSet);
	iniFile.WriteBool(m_PluDownloadDate);
	iniFile.WriteBool(m_PluInactiveNoDownload);
	iniFile.WriteBool(m_PluInactiveAutoHide);
	iniFile.WriteBool(m_PluTask);
	iniFile.WriteInt(m_PluDefText);
	iniFile.WriteBool(m_PluDiscardMod);
	iniFile.WriteBool(m_PluAutoImport);
	iniFile.WriteBool(m_PluManImport);
	iniFile.WriteBool(m_PluManExport);
	iniFile.WriteBool(m_PluPurchase);
	iniFile.WriteInt(m_PluProdCSV);
	iniFile.WriteInt(m_PluDelBarcode);
	iniFile.WriteBool(m_PluBarcodeMod);
	iniFile.WriteInt(m_PluAutoModEposText);
	iniFile.WriteInt(m_PluAutoModKeyText);

	//GLOBAL TAB
	iniFile.WriteInt(m_GlobalDept);
	iniFile.WriteBool(m_GlobalDeptGroupEpos);
	iniFile.WriteBool(m_GlobalTax);
	iniFile.WriteBool(m_GlobalMod);
	iniFile.WriteInt(m_GlobalACat);
	iniFile.WriteInt(m_GlobalPay);
	iniFile.WriteBool(m_GlobalPaidIO);

	//FEATURES TAB
	iniFile.WriteBool(m_FeaturesPluStock);
	iniFile.WriteBool(m_FeaturesPluAudit);
	iniFile.WriteInt(m_FeaturesEposDelivery);
	iniFile.WriteBool(m_FeaturesMixMatch);
	iniFile.WriteBool(m_FeaturesPromotion);
	iniFile.WriteBool(m_FeaturesTimePlan);
	iniFile.WriteBool(m_FeaturesTimePlanDL);
	iniFile.WriteBool(m_FeaturesTimePlanTemplate);
	iniFile.WriteBool(m_FeaturesWalletStampRule);
	iniFile.WriteBool(m_FeaturesGlobalWalletStampRule);
	iniFile.WriteBool(m_FeaturesKeyboard);
	iniFile.WriteInt(m_FeaturesLocKeyboards);
	iniFile.WriteBool(m_FeaturesKBShare);
	iniFile.WriteBool(m_FeaturesKBPriceSet);
	iniFile.WriteInt(m_FeaturesKBTextOrder);
	iniFile.WriteInt(m_FeaturesServerType);
	iniFile.WriteBool(m_FeaturesServerRestrict);
	iniFile.WriteBool(m_FeaturesServerPreset);
	iniFile.WriteBool(m_FeaturesServerLogon);
	iniFile.WriteBool(m_FeaturesCustomerBal);
	iniFile.WriteInt(m_FeaturesCustomerName);
	iniFile.WriteInt(m_FeaturesCustomerAccountDelDay);
	iniFile.WriteInt(m_FeaturesCustomerOneOffDelDay);
	iniFile.WriteInt(m_FeaturesDBaseEdit);
	iniFile.WriteInt(m_FeaturesBackCol);
	iniFile.WriteBool(m_FeaturesContextNow);
	iniFile.WriteBool(m_FeaturesAllergen);

	//REPORTS1 TAB
	iniFile.WriteString(m_ReportsFYearStart);
	iniFile.WriteInt(m_ReportsTaxQStart);
	iniFile.WriteInt(m_ReportsWeekStart);
	iniFile.WriteInt(m_ReportsWeekNumber);
	iniFile.WriteInt(m_ReportsDayHour);
	iniFile.WriteInt(m_ReportsDayMinute);
	iniFile.WriteInt(m_ReportsTimeAttendHour);
	iniFile.WriteInt(m_ReportsTimeAttendMinute);
	iniFile.WriteInt(m_ReportsLocSort);
	iniFile.WriteInt(m_ReportsSalesTaxType);
	iniFile.WriteBool(m_ReportsRepPoint);
	iniFile.WriteBool(m_ReportsLocSet);
	iniFile.WriteBool(m_ReportsTrimZero);
	iniFile.WriteBool(m_ReportsRegion);
	iniFile.WriteBool(m_ReportsCustTran);
	iniFile.WriteBool(m_ReportsCustState);
	iniFile.WriteBool(m_ReportsCustContact);
	iniFile.WriteInt(m_ReportsContactBeforeTime);
	iniFile.WriteInt(m_ReportsContactAfterTime);
	iniFile.WriteInt(m_ReportsContactVisitTime);
	iniFile.WriteString(m_ReportsDateRange);
	iniFile.WriteInt(m_ReportsSingleDayRanges);
	iniFile.WriteInt(m_ReportsPreviousDayRanges);
	iniFile.WriteBool(m_ReportsEODSession);
	iniFile.WriteInt(m_ReportsEcrLiteLogMode);

	//REPORTS2 TAB
	iniFile.WriteInt(m_ReportsVIPType);
	iniFile.WriteInt(m_ReportsVIPLevel);
	iniFile.WriteBool(m_ReportsVIPRetail);
	iniFile.WriteInt(m_ReportsCustSortType);
	iniFile.WriteInt(m_ReportsSalesTimeType);
	iniFile.WriteBool(m_ReportsDiscountDetailSales);
	iniFile.WriteBool(m_ReportsPremiumDetailSales);
	iniFile.WriteBool(m_ReportsNetOfTaxSales);
	iniFile.WriteInt(m_ReportsDiscountDetailCovers);
	iniFile.WriteBool(m_ReportsPremiumDetailCovers);
	iniFile.WriteBool(m_ReportsNetOfTaxCovers);
	iniFile.WriteBool(m_ReportsCashRSPItemText);
	iniFile.WriteBool(m_ReportsAuthServer);
	iniFile.WriteBool(m_ReportsGroupReportpoint);
	iniFile.WriteBool(m_ReportsEODReasons);
	iniFile.WriteBool(m_ReportsGeneralPage);
	iniFile.WriteBool(m_ReportsChange);
	iniFile.WriteInt(m_ReportsMultiCash);
	iniFile.WriteBool(m_ReportsPayQty);
	iniFile.WriteBool(m_ReportsRAPay);
	iniFile.WriteBool(m_ReportsSplitRAPay);
	iniFile.WriteBool(m_ReportsRABal);
	iniFile.WriteBool(m_ReportsSplitRABal);
	iniFile.WriteBool(m_ReportsCashRSPDiscountPenny);

	//AUTO EXPIRE TAB
	iniFile.WriteInt(m_ExpireSales);
	iniFile.WriteInt(m_ExpireAudit);
	iniFile.WriteInt(m_ExpireSummary);
	iniFile.WriteInt(m_ExpireDetail);
	//iniFile.WriteInt(m_ExpireTimeAttend);

	//LABELS1 TAB
	iniFile.WriteBool(m_LabelsShelfPlu);
	iniFile.WriteBool(m_LabelsShelfStock);
	iniFile.WriteBool(m_LabelsShelfSupp);
	iniFile.WriteBool(m_LabelsShelfTrimZero);
	iniFile.WriteInt(m_LabelsShelfPrice1);
	iniFile.WriteInt(m_LabelsShelfPrice2);
	iniFile.WriteInt(m_LabelsShelfTax1);
	iniFile.WriteInt(m_LabelsShelfTax2);
	iniFile.WriteString(m_LabelsShelfSymbol1);
	iniFile.WriteString(m_LabelsShelfSymbol2);
	iniFile.WriteBool(m_LabelsProdPlu);
	iniFile.WriteBool(m_LabelsProdStock);
	iniFile.WriteBool(m_LabelsProdSupp);
	iniFile.WriteBool(m_LabelsProdTrimZero);
	iniFile.WriteInt(m_LabelsProdPrice1);
	iniFile.WriteInt(m_LabelsProdPrice2);
	iniFile.WriteInt(m_LabelsProdTax1);
	iniFile.WriteInt(m_LabelsProdTax2);
	iniFile.WriteString(m_LabelsProdSymbol1);
	iniFile.WriteString(m_LabelsProdSymbol2);

	//LABELS2 TAB
	iniFile.WriteString(m_LabelsShelfNameSingle);
	iniFile.WriteString(m_LabelsShelfNameBatch);
	iniFile.WriteBool(m_LabelsShelfAutoBatch);
	iniFile.WriteBool(m_LabelsShelfPromptBatch);
	iniFile.WriteBool(m_LabelsShelfPrintSingle);
	iniFile.WriteBool(m_LabelsShelfPromptSingle);
	iniFile.WriteString(m_LabelsProdNameSingle);
	iniFile.WriteString(m_LabelsProdNameBatch);
	iniFile.WriteBool(m_LabelsProdAutoBatch);
	iniFile.WriteBool(m_LabelsProdPrintSingle);
	iniFile.WriteBool(m_LabelsProdPromptSingle);

	//CUSTOM MESSAGE BOX
	iniFile.WriteBool(m_CustomMessageBox);
	iniFile.WriteInt(m_CustomMessageBoxFont);
	iniFile.WriteInt(m_CustomMessageBoxLineGap);
	iniFile.WriteString(m_CustomMessageBoxColourBack);
	iniFile.WriteString(m_CustomMessageBoxColourText);

	//STARTUP MESSAGE BOX
	iniFile.WriteBool(m_StartupMessageBox);
	iniFile.WriteInt(m_StartupMessageBoxFont);
	iniFile.WriteInt(m_StartupMessageBoxLineGap);
	iniFile.WriteString(m_StartupMessageBoxColourBack);
	iniFile.WriteString(m_StartupMessageBoxColourText);

	//PASSWORDS
	iniFile.WriteBool(m_PasswordsLog);
}

/**********************************************************************/

void CEcrmanOptions::Reset()
{
	//PLU TAB
	m_PluTabBarcode.Reset();
	m_PluTabLink.Reset();
	m_PluTabInfo.Reset();
	m_PluPriceSet.Reset();
	m_PluDownloadDate.Reset();
	m_PluInactiveNoDownload.Reset();
	m_PluInactiveAutoHide.Reset();
	m_PluTask.Reset();
	m_PluDefText.Reset();
	m_PluDiscardMod.Reset();
	m_PluAutoImport.Reset();
	m_PluManImport.Reset();
	m_PluManExport.Reset();
	m_PluPurchase.Reset();
	m_PluProdCSV.Reset();
	m_PluDelBarcode.Reset();
	m_PluBarcodeMod.Reset();
	m_PluAutoModEposText.Reset();
	m_PluAutoModKeyText.Reset();

	//GLOBAL TAB
	m_GlobalDept.Reset();
	m_GlobalDeptGroupEpos.Reset();
	m_GlobalTax.Reset();
	m_GlobalMod.Reset();
	m_GlobalACat.Reset();
	m_GlobalACatLegacy.Reset();
	m_GlobalPay.Reset();
	m_GlobalPaidIO.Reset();

	//FEATURES TAB
	m_FeaturesPluStock.Reset();
	m_FeaturesPluAudit.Reset();
	m_FeaturesLegacyEposDelivery.Reset();
	m_FeaturesEposDelivery.Reset();
	m_FeaturesMixMatch.Reset();
	m_FeaturesPromotion.Reset();
	m_FeaturesTimePlan.Reset();
	m_FeaturesTimePlanDL.Reset();
	m_FeaturesTimePlanTemplate.Reset();
	m_FeaturesWalletStampRule.Reset();
	m_FeaturesGlobalWalletStampRule.Reset();
	m_FeaturesKeyboard.Reset();
	m_FeaturesLocKeyboards.Reset();
	m_FeaturesKBShare.Reset();
	m_FeaturesKBPriceSet.Reset();
	m_FeaturesKBTextOrder.Reset();
	m_FeaturesServerType.Reset( SERVER_SYS );
	m_FeaturesServerRestrict.Reset();
	m_FeaturesServerPreset.Reset();
	m_FeaturesServerLogon.Reset();
	m_FeaturesCustomerBal.Reset();
	m_FeaturesCustomerName.Reset();
	m_FeaturesDBaseEdit.Reset();
	m_FeaturesBackCol.Reset();
	m_FeaturesContextNow.Reset();
	m_FeaturesOPNameFirst.Reset();
	m_FeaturesAllergen.Reset();
	
	//REPORTS1 TAB
	m_ReportsFYearStart.Reset();
	m_ReportsTaxQStart.Reset();
	m_ReportsWeekStart.Reset();
	m_ReportsWeekNumber.Reset();
	m_ReportsDayHour.Reset();
	m_ReportsDayMinute.Reset();
	m_ReportsTimeAttendHour.Reset();
	m_ReportsTimeAttendMinute.Reset();
	m_ReportsLocSort.Reset();
	m_ReportsSalesTaxType.Reset();
	m_ReportsRepPoint.Reset();
	m_ReportsLocSet.Reset();
	m_ReportsTrimZero.Reset();
	m_ReportsRegion.Reset();
	m_ReportsCustTran.Reset();
	m_ReportsCustState.Reset();
	m_ReportsCustContact.Reset();
	m_ReportsContactBeforeTime.Reset();
	m_ReportsContactAfterTime.Reset();
	m_ReportsContactVisitTime.Reset();
	m_ReportsDateRange.Reset();
	m_ReportsSingleDayRanges.Reset();
	m_ReportsPreviousDayRanges.Reset();
	m_ReportsEODSession.Reset();
	m_ReportsEcrLiteLogMode.Reset();

	//REPORTS2 TAB
	m_ReportsVIPType.Reset();
	m_ReportsVIPLevel.Reset();
	m_ReportsVIPRetail.Reset();
	m_ReportsCustSortType.Reset();
	m_ReportsSalesTimeType.Reset();
	m_ReportsDiscountDetailSales.Reset();
	m_ReportsPremiumDetailSales.Reset();
	m_ReportsNetOfTaxSales.Reset();
	m_ReportsDiscountDetailCovers.Reset();
	m_ReportsPremiumDetailCovers.Reset();
	m_ReportsNetOfTaxCovers.Reset();
	m_ReportsAuthServer.Reset();
	m_ReportsGroupReportpoint.Reset();
	m_ReportsEODReasons.Reset();
	m_ReportsGeneralPage.Reset();
	m_ReportsChange.Reset();
	m_ReportsMultiCash.Reset();
	m_ReportsPayQty.Reset();
	m_ReportsRAPay.Reset();
	m_ReportsSplitRAPay.Reset();
	m_ReportsRABal.Reset();
	m_ReportsSplitRABal.Reset();
	m_ReportsCashRSPDiscountPenny.Reset();

	//AUTO EXPIRE TAB
	m_ExpireSales.Reset();
	m_ExpireAudit.Reset();
	m_ExpireSummary.Reset();
	m_ExpireDetail.Reset();
	//m_ExpireTimeAttend.Reset();
	
	//LABELS1 TAB
	m_LabelsShelfPlu.Reset();
	m_LabelsShelfStock.Reset();
	m_LabelsShelfSupp.Reset();
	m_LabelsShelfTrimZero.Reset();
	m_LabelsShelfPrice1.Reset();
	m_LabelsShelfPrice2.Reset();
	m_LabelsShelfTax1.Reset();
	m_LabelsShelfTax2.Reset();
	m_LabelsShelfSymbol1.Reset();
	m_LabelsShelfSymbol2.Reset();
	m_LabelsProdPlu.Reset();
	m_LabelsProdStock.Reset();
	m_LabelsProdSupp.Reset();
	m_LabelsProdTrimZero.Reset();
	m_LabelsProdPrice1.Reset();
	m_LabelsProdPrice2.Reset();
	m_LabelsProdTax1.Reset();
	m_LabelsProdTax2.Reset();
	m_LabelsProdSymbol1.Reset();
	m_LabelsProdSymbol2.Reset();
	
	//LABELS2 TAB
	m_LabelsShelfNameSingle.Reset();
	m_LabelsShelfNameBatch.Reset();
	m_LabelsShelfAutoBatch.Reset();
	m_LabelsShelfPromptBatch.Reset();
	m_LabelsShelfPrintSingle.Reset();
	m_LabelsShelfPromptSingle.Reset();
	m_LabelsProdNameSingle.Reset();
	m_LabelsProdNameBatch.Reset();
	m_LabelsProdAutoBatch.Reset();
	m_LabelsProdPrintSingle.Reset();
	m_LabelsProdPromptSingle.Reset();

	//CUSTOM MESSAGE BOX TAB
	m_CustomMessageBox.Reset();
	m_CustomMessageBoxFont.Reset();
	m_CustomMessageBoxLineGap.Reset();
	m_CustomMessageBoxColourBack.Reset();
	m_CustomMessageBoxColourText.Reset();

	//STARTUP MESSAGE BOX TAB
	m_StartupMessageBox.Reset();
	m_StartupMessageBoxFont.Reset();
	m_StartupMessageBoxLineGap.Reset();
	m_StartupMessageBoxColourBack.Reset();
	m_StartupMessageBoxColourText.Reset();
	
	//PASSWORDS
	m_PasswordsLog.Reset();

	MakeIntSettings();
}

/**********************************************************************/

void CEcrmanOptions::SetDefaults()
{
	Reset();

	if ( SysInfo.GetMaxDatabases() >= 2 )
	{
		m_GlobalDept.SetValue( 1 );
		m_GlobalDeptGroupEpos.SetValue( TRUE );
		m_GlobalTax.SetValue( TRUE );		
	}

	if ( SysInfo.IsRetailStockSystem() == TRUE )
	{
		m_LabelsShelfAutoBatch.SetValue( TRUE );
		m_LabelsProdAutoBatch.SetValue( TRUE );
	}
}

/**********************************************************************/

bool CEcrmanOptions::Read()
{
	bool bResult = ReadInternal();

	if (SysInfo.IsEcrLiteSystem() == TRUE)
	{
		m_PluPriceSet.SetValue(FALSE);
		m_FeaturesKBPriceSet.SetValue(FALSE);
		m_FeaturesMixMatch.SetValue(FALSE);
		m_FeaturesPromotion.SetValue(FALSE);
		m_FeaturesTimePlan.SetValue(FALSE);
		m_FeaturesTimePlanDL.SetValue(FALSE);
		m_FeaturesTimePlanTemplate.SetValue(FALSE);
		m_FeaturesCustomerBal.SetValue(FALSE);
		m_FeaturesCustomerName.SetValue(0);
		m_ReportsRegion.SetValue(FALSE);
		m_ReportsCustTran.SetValue(FALSE);
		m_ReportsCustState.SetValue(FALSE);
		m_ReportsCustContact.SetValue(FALSE);
	}

	return bResult;
}

/**********************************************************************/

bool CEcrmanOptions::ReadInternal()
{
	CFilenameUpdater FnUpOld(SysFiles::LegacyEcrmanOptions);
	CString strFilenameOld = FnUpOld.GetFilenameToUse();

	CFilenameUpdater FnUpNew(SysFiles::EcrmanOptions);
	CString strFilenameNew = FnUpNew.GetFilenameToUse();

	if (::FileExists(strFilenameNew) == TRUE)
	{
		CEnhancedIniFile iniFile;
		if (iniFile.Read(strFilenameNew) == FALSE)
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile(iniFile);
		return TRUE;
	}
	else
	{
		CSSFile fileOptions;
		if (fileOptions.Open(strFilenameOld, "rb") == FALSE)
		{
			if (GetLastError() != ERROR_FILE_NOT_FOUND)
			{
				m_bFatalReadError = TRUE;
			}
			return FALSE;
		}

		CString strBuffer;
		if (fileOptions.ReadString(strBuffer) == FALSE)
		{
			return TRUE;
		}

		LegacyReloadFromString(strBuffer);
		return TRUE;
	}
}

/**********************************************************************/

bool CEcrmanOptions::Write()
{
	bool bResult = FALSE;

	if ( DataManager.LockAllUsers() == TRUE )
	{
		CEnhancedIniFile iniFile;
		PrepareIniFile( iniFile );
		
		CFilenameUpdater FnUp( SysFiles::EcrmanOptions );
		bResult = iniFile.Write( FnUp.GetFilenameToUse() );

		DataManager.UnlockAllUsers();
	}
		
	return bResult;
}

/**********************************************************************/

void CEcrmanOptions::GetFYearStart ( int& nDay, int& nMonth )
{
	CString strFYear = "";
	strFYear.Format ( "%s1999", m_ReportsFYearStart.GetValue() );
	bool bValid = FALSE;

	if ( strFYear.GetLength() == 8 )
	{
		CSSDate date;

		if ( date.SetDate ( strFYear ) == TRUE )
		{
			nDay = date.GetDay();
			nMonth = date.GetMonth();
			bValid = TRUE;
		}
	}

	if ( bValid == FALSE )
	{
		nDay = 1;
		nMonth = 1;
	}
}

/**********************************************************************/

bool CEcrmanOptions::GetGlobalDepartmentFlag()
{
	int nType = m_GlobalDept.GetValue();
	return ( 1 == nType );
}

/**********************************************************************/

bool CEcrmanOptions::GetGlobalDeptGroupReportFlag()
{
	int nType = m_GlobalDept.GetValue();
	return ( ( 1 == nType ) || ( 2 == nType ) );
}

/**********************************************************************/

bool CEcrmanOptions::GetGlobalDeptGroupConsolFlag()
{
	int nType = m_GlobalDept.GetValue();
	return ( ( nType >= 1 ) && ( nType <= 3 ) );
}

/**********************************************************************/

bool CEcrmanOptions::GetGlobalPaymentFlag()
{
	int nType = m_GlobalPay.GetValue();
	return ( 1 == nType );
}

/**********************************************************************/

bool CEcrmanOptions::GetGlobalPaymentGroupFlag()
{
	int nType = m_GlobalPay.GetValue();
	return ( ( 1 == nType ) || ( 2 == nType ) );
}

/**********************************************************************/

bool CEcrmanOptions::GetFeaturesCustomerAccountAutoDeleteFlag()
{
	return ( m_FeaturesCustomerAccountDelDay.GetValue() > 0 );
}

/**********************************************************************/

bool CEcrmanOptions::GetFeaturesCustomerOneOffAutoDeleteFlag()
{
	return ( m_FeaturesCustomerOneOffDelDay.GetValue() > 0 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
//LEGACY FUNCTION FOR READING OLD OPTIONS FILE FORMAT
/**********************************************************************/

void CEcrmanOptions::LegacyReloadFromString( CString& strBuffer )
{
	CCSV csv ( strBuffer );

	//PLU TAB
	m_PluPriceSet.Reset( csv.GetBool(0) );
	m_PluTask.Reset( csv.GetBool(70) );
	m_PluDefText.Reset( csv.GetInt(76) );
	m_PluDiscardMod.Reset( csv.GetBool(99) );
	m_PluAutoImport.Reset( csv.GetBool(1) == FALSE );
	m_PluManImport.Reset( csv.GetBool(2) == FALSE );
	m_PluManExport.Reset( csv.GetBool(3) == FALSE );
	m_PluPurchase.Reset( csv.GetBool(60) );
	m_PluProdCSV.Reset( csv.GetInt(84) );
	m_PluDelBarcode.Reset( csv.GetInt(69) );

	//GLOBAL TAB
	m_GlobalDept.Reset( csv.GetInt(6) );
	m_GlobalDeptGroupEpos.Reset( csv.GetBool(7) );
	m_GlobalTax.Reset( csv.GetBool(8) );	
	m_GlobalMod.Reset( csv.GetBool(57) == FALSE ); 
	m_GlobalACat.Reset( ( csv.GetBool(58) == FALSE ) ? 1 : 0 ); 
	m_GlobalPay.Reset( csv.GetBool(59) == FALSE ); 
	m_GlobalPaidIO.Reset( csv.GetInt(85) != 2 ); 

	//FOR LEGACY REASONS, OTHER GLOBAL FLAGS ARE NEVER BLOCKED
	if ( SysInfo.GetMaxDatabases() < 2 )
	{
		m_GlobalDept.SetValue( 0 );
		m_GlobalDeptGroupEpos.SetValue( FALSE );
		m_GlobalTax.SetValue( FALSE );
		m_GlobalPaidIO.SetValue( FALSE );
	}

	//FEATURES TAB
	m_FeaturesPluStock.Reset( csv.GetBool(38) );
	m_FeaturesPluAudit.Reset( csv.GetBool(39) );
	m_FeaturesLegacyEposDelivery.Reset( csv.GetInt(82) );
	m_FeaturesMixMatch.Reset( csv.GetBool(54) );
	m_FeaturesPromotion.Reset( csv.GetBool(55) );
	m_FeaturesTimePlan.Reset( csv.GetBool(61) );
	m_FeaturesTimePlanDL.Reset( csv.GetBool(63) );
	m_FeaturesKeyboard.Reset( csv.GetBool(15) );
	m_FeaturesLocKeyboards.Reset( csv.GetInt(16) );
	m_FeaturesKBPriceSet.Reset( csv.GetBool(66) );
	m_FeaturesKBShare.Reset( csv.GetBool(81) );
	m_FeaturesKBTextOrder.Reset( csv.GetInt(101) );
	m_FeaturesServerType.Reset( csv.GetInt(46) );
	m_FeaturesCustomerBal.Reset( csv.GetBool(90) );
	m_FeaturesCustomerName.Reset( csv.GetInt(100) );

	//REPORTS TAB
	m_ReportsFYearStart.Reset( csv.GetString(13) );
	m_ReportsTaxQStart.Reset( csv.GetInt(12) );
	m_ReportsWeekStart.Reset( csv.GetInt(11) );
	m_ReportsWeekNumber.Reset( csv.GetInt(43) );
	m_ReportsDayHour.Reset( csv.GetInt(9) );
	m_ReportsDayMinute.Reset( csv.GetInt(10) );
	m_ReportsLocSort.Reset( csv.GetInt(56) );
	m_ReportsRepPoint.Reset( csv.GetBool(5) );
	m_ReportsLocSet.Reset( csv.GetBool(44) );
	m_ReportsTrimZero.Reset( csv.GetBool(4) );
	m_ReportsRegion.Reset( csv.GetBool(64) == FALSE );
	m_ReportsCustTran.Reset( csv.GetBool(102) );
	m_ReportsCustState.Reset( csv.GetBool(103) );

	//REPORTS2 TAB
	m_ReportsVIPType.Reset( csv.GetInt(80) );
	m_ReportsDiscountDetailSales.Reset( csv.GetBool(65) );
	m_ReportsEODReasons.Reset( csv.GetBool(79) );
	m_ReportsChange.Reset( csv.GetBool(71) );
	m_ReportsPayQty.Reset( csv.GetBool(97) );
	m_ReportsRAPay.Reset( csv.GetBool(83) );
	m_ReportsSplitRAPay.Reset( csv.GetBool(86) );
	m_ReportsRABal.Reset( csv.GetBool(87) );
	m_ReportsSplitRABal.Reset( csv.GetBool(88) );
	
	//AUTO EXPIRE TAB
	m_ExpireSales.Reset( csv.GetInt(41) );
	m_ExpireAudit.Reset( csv.GetInt(42) );
	m_ExpireSummary.Reset( csv.GetInt(77) );
	m_ExpireDetail.Reset( csv.GetInt(78) );

	//LABELS1 TAB
	m_LabelsShelfPlu.Reset( csv.GetBool(48) );
	m_LabelsShelfStock.Reset( csv.GetBool(49) );
	m_LabelsShelfSupp.Reset( csv.GetBool(50) );
	m_LabelsShelfTrimZero.Reset( csv.GetBool(33) );
	m_LabelsShelfPrice1.Reset( csv.GetInt(28) );
	m_LabelsShelfPrice2.Reset( csv.GetInt(29) );
	m_LabelsShelfTax1.Reset( csv.GetInt(30) );
	m_LabelsShelfTax2.Reset( csv.GetInt(31) );
	m_LabelsShelfSymbol1.Reset( csv.GetString(34) );
	m_LabelsShelfSymbol2.Reset( csv.GetString(35) );
	m_LabelsProdPlu.Reset( csv.GetBool(51) );
	m_LabelsProdStock.Reset( csv.GetBool(52) );
	m_LabelsProdSupp.Reset( csv.GetBool(53) );
	m_LabelsProdTrimZero.Reset( csv.GetBool(23) );
	m_LabelsProdPrice1.Reset( csv.GetInt(18) );
	m_LabelsProdPrice2.Reset( csv.GetInt(19) );		
	m_LabelsProdTax1.Reset( csv.GetInt(20) );
	m_LabelsProdTax2.Reset( csv.GetInt(21) );
	m_LabelsProdSymbol1.Reset( csv.GetString(24) );
	m_LabelsProdSymbol2.Reset( csv.GetString(25) );

	//LABELS2 TAB
	m_LabelsShelfNameSingle.Reset( csv.GetString(95) );
	m_LabelsShelfNameBatch.Reset( csv.GetString(36) );
	m_LabelsShelfAutoBatch.Reset( csv.GetBool(37) );
	m_LabelsShelfPromptBatch.Reset( csv.GetBool(40) );
	m_LabelsShelfPrintSingle.Reset( csv.GetBool(94) );
	m_LabelsShelfPromptSingle.Reset( csv.GetBool(96) );
	m_LabelsProdNameSingle.Reset( csv.GetString(92) );
	m_LabelsProdNameBatch.Reset( csv.GetString(26) );
	m_LabelsProdAutoBatch.Reset( csv.GetBool(27) );
	m_LabelsProdPrintSingle.Reset( csv.GetBool(91) );
	m_LabelsProdPromptSingle.Reset( csv.GetBool(93) );

	//PASSWORDS
	m_PasswordsLog.Reset( csv.GetBool(14) );
}

/**********************************************************************/

void CEcrmanOptions::MakeIntSettings()
{
	m_nCustomMessageBoxColourBack = MakeColourInt( m_CustomMessageBoxColourBack );
	m_nCustomMessageBoxColourText = MakeColourInt( m_CustomMessageBoxColourText );
	m_nStartupMessageBoxColourBack = MakeColourInt(m_StartupMessageBoxColourBack);
	m_nStartupMessageBoxColourText = MakeColourInt(m_StartupMessageBoxColourText);
}

/**********************************************************************/

bool CEcrmanOptions::GetReportsDiscountDetailCoversFlag()
{
	switch( m_ReportsDiscountDetailCovers.GetValue() )
	{
	case 1:		return FALSE;
	case 2:		return TRUE;
	default:	return m_ReportsDiscountDetailSales.GetValue();
	}
}

/**********************************************************************/

bool CEcrmanOptions::AllowBarcodeModifierLinks()
{
	return ( ( SysInfo.GetMaxBasePluLen() != 0 ) && ( m_PluBarcodeMod.GetValue() == TRUE ) );
}

/**********************************************************************/

const char* CEcrmanOptions::GetSalesTaxName()
{
	m_strSalesTaxName = "VAT";
	switch( EcrmanOptions.GetReportsSalesTaxType() )
	{
	case 0:	m_strSalesTaxName = "VAT";	break;
	case 1:	m_strSalesTaxName = "GST";	break;
	case 2:	m_strSalesTaxName = "Tax";	break;
	}
	return m_strSalesTaxName;
}

/**********************************************************************/

const char* CEcrmanOptions::GetEstTaxString()
{
	return GetTaxString( "Est. %T" );
}

/**********************************************************************/

const char* CEcrmanOptions::GetExTaxString()
{
	return GetTaxString( "Ex %T" );
}

/**********************************************************************/

const char* CEcrmanOptions::GetTaxContentString()
{
	return GetTaxString( "%T Content" );
}

/**********************************************************************/

const char* CEcrmanOptions::GetTaxBandString()
{
	return GetTaxString( "%T Band" );
}

/**********************************************************************/

const char* CEcrmanOptions::GetTaxString( const char* szSource )
{
	m_strSalesTaxLabel = szSource;
	m_strSalesTaxLabel.Replace( "%T", GetSalesTaxName() );
	return m_strSalesTaxLabel;
}

/**********************************************************************/

bool CEcrmanOptions::HasIdleTimeCustomerTask()
{
	if ( GetFeaturesCustomerNameImportType() != 0 )
		return TRUE;

	if ( GetFeaturesCustomerAccountAutoDeleteFlag() == TRUE )
		return TRUE;

	if ( GetFeaturesCustomerOneOffAutoDeleteFlag() == TRUE )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

bool CEcrmanOptions::GetFeaturesWalletStampRuleFlag()
{
	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_LOY_V4:
	case LOYALTY_TYPE_SMP_V4:
		return m_FeaturesWalletStampRule.GetValue();
		break;

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CEcrmanOptions::GetFeaturesGlobalWalletStampRuleFlag()
{
	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_LOY_V4:
	case LOYALTY_TYPE_SMP_V4:
		return m_FeaturesGlobalWalletStampRule.GetValue();
		break;
	default:
		return FALSE;
	}
}

/**********************************************************************/
