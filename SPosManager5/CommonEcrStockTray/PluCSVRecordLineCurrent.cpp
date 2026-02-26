/**********************************************************************/
#include "TaxImportMap.h"
/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/

void CPluCSVRecord::ConvertToCSV ( CCSV& csv )
{
	CreateCSVLine( csv, FALSE );
}

/**********************************************************************/

void CPluCSVRecord::CreateExportLine( CCSV& csv )
{
	CreateCSVLine( csv, TRUE );
}

/**********************************************************************/

void CPluCSVRecord::CreateCSVLine(CCSV& csv, bool bForceFullLine)
{
	int nDPValue = 3;		/* max system dp value */

	//DESCRIPTION AND TYPE
	csv.Add(m_nPluNo);
	csv.Add(PLUDB_VERSION_NO);
	csv.Add(m_strEposText);
	csv.Add(m_strKeyText);
	csv.Add(m_strRepText);
	csv.Add(m_strSortCode);
	csv.Add(m_nAgressoGLCode);
	csv.Add(m_strAgressoCostCentre);
	csv.Add(m_strAgressoProject);
	csv.Add(m_nBaseDeptNo);
	csv.Add(m_nBaseAnalysisCategory);
	csv.Add(m_nMicrotrendPluType);
	csv.Add(m_nExternalProcess);
	csv.Add(m_strBaseTaxBand);

	//PRICES AND POINTS
	for (int i = 0; i < MAX_PLU_PRICES; i++)
	{
		csv.Add(m_dPrice[i], nDPValue);
	}

	csv.Add(m_nPoints);
	csv.Add(m_nStampWeight);

	//COST AND PROFIT
	csv.Add(m_dPurchaseCost, 5);
	csv.Add(m_bReportCostEnable);
	csv.Add(m_dReportCost, 5);
	csv.Add(m_nGPType);
	csv.Add(m_dProfitMargin, 5);

	//ALLERGENS
	for (int i = 0; i < MAX_PLU_ALLERGENS; i++)
	{
		csv.Add(m_nAllergenNo[i]);
	}

	//KEYBOARD LISTS
	csv.Add(m_nFirstMenu);
	csv.Add(m_nFirstQualifier);
	csv.Add(m_nFirstExtra);

	//ORDER PRINTERS
	csv.Add(m_nOrderPrinterGroup);

	for (int i = 0; i < PLU_ORDER_PRINTERS_NOW; i++)
	{
		csv.Add(m_bOrderPrinterFlag[i]);
	}

	csv.Add(m_nPriority);

	//MODIFIERS
	csv.Add(m_bAllowModifier);
	for (int m = 0; m < PLU_MODIFIERS; m++)
	{
		bool bEnable = m_bModifierEnable[m];

		int nEnable;
		if (TRUE == bEnable)
			nEnable = 1;
		else if (EcrmanOptions.GetPluDiscardUnusedModifierFlag() == TRUE)
			nEnable = (bForceFullLine) ? 3 : 2;
		else
			nEnable = 0;

		//0 = disable, append modifier fields
		//1 = enable
		//2 = disable, omit modifier fields
		//3 = disable, append modifier fields as blanks

		if (FALSE == bForceFullLine)
			csv.Add(nEnable);
		else
			csv.Add(1 == nEnable);

		switch (nEnable)
		{
		case 0:
		case 1:
		{
			csv.Add(m_bModifierEposText[m]);
			csv.Add(GetModifierEposText(m + 1));
			csv.Add(m_bModifierKeyText[m]);
			csv.Add(GetModifierKeyText(m + 1));

			csv.Add(GetModifierDeptNoRaw(m + 1));
			csv.Add(GetModifierAnalysisCategoryRaw(m + 1));
			csv.Add(GetModifierMicrotrendPluType(m + 1));
			csv.Add(GetModifierTaxBandStringRaw(m + 1));

			for (int l = 0; l < MAX_PLU_PRICES; l++)
			{
				csv.Add(m_dModifierPrice[m][l], nDPValue);
			}

			csv.Add(GetModifierFirstMenu(m + 1));
			csv.Add(GetModifierFirstQualifier(m + 1));
			csv.Add(GetModifierFirstExtra(m + 1));
			csv.Add(GetModifierOrderPrinterGroup(m + 1));
		}
		break;

		case 3:
		{
			for (int l = 0; l < MAX_PLU_PRICES + 12; l++)
			{
				csv.Add("");
			}
		}
		break;
		}
	}

	//MISC PROGRAMMING
	csv.Add(m_nPriceType);
	csv.Add(m_nAgeVerificationType);
	csv.Add(m_bKPNotAlone);
	csv.Add(m_bKPConsolidateTopItem);
	csv.Add(m_bKPImagePrint);
	csv.Add(m_bKPImageModifier);
	csv.Add(m_bAutoCoversQty);
	csv.Add(m_bEANOnReceipt);
	csv.Add(m_bSalesTracking);
	csv.Add(m_bSoldOut);
	csv.Add(m_bDiscountable);
	csv.Add(m_bStaffDiscount);
	csv.Add(m_bFreeText);
	csv.Add(m_bFreeTextDescription);
	csv.Add(m_bAccountRequired);
	
	//LABELS
	csv.Add(m_bPrintShelf);
	csv.Add(m_bPrintProduct);
	csv.Add(m_strUnitPriceText);
	csv.Add(m_dUnitPriceSize, 5);

	//ECRMAN STOCK
	csv.Add(m_dApparentLevel, 5);
	csv.Add(m_dMinimumLevel, 5);
	csv.Add(m_strSupplierRef);
	csv.Add(m_nSupplierNo);
	csv.Add(m_bNonStock);

	//MISC
	csv.Add(m_bPluInfoModifier);
	csv.Add(m_nLinkType);
	csv.Add(m_bLinkSellTiedItem);
	csv.Add(m_strLinkTable);
	csv.Add(m_strEcrmanRecipeTable);
	csv.Add(m_strImageFilename);
	csv.Add(m_bActive);

	//INTERNAL FIELDS
	csv.Add(m_strModifiedDateTimePlu);
	csv.Add(m_strModifiedDateTimeInfo);
	csv.Add(m_strDownloadDatePlu);
	csv.Add(m_bExport);
}

/********************************************************************/
/*																	*/
/*	convert from csv												*/
/*																	*/
/*	For future compatibility, if count not stored in file:			*/
/*																	*/
/*	1) Cannot use defines											*/
/*	2) Cannot make arrays smaller than previous versions			*/
/*																	*/
/********************************************************************/

void CPluCSVRecord::ConvertFromCSV(CCSV& csv)
{
	ClearRecord();

	switch (csv.GetInt(1))			/* version number */
	{
	case 1:		V1ConvertFromCSV(csv);			break;
	case 2:		V2ConvertFromCSV(csv);			break;
	case 3:		V3ConvertFromCSV(csv);			break;
	case 4:		V4To10ConvertFromCSV(csv, 4);	break;
	case 5:		V4To10ConvertFromCSV(csv, 5);	break;
	case 6:		V4To10ConvertFromCSV(csv, 6);	break;
	case 7:		V4To10ConvertFromCSV(csv, 7);	break;
	case 8:		V4To10ConvertFromCSV(csv, 8);	break;
	case 9:		V4To10ConvertFromCSV(csv, 9);	break;
	case 10:	V4To10ConvertFromCSV(csv, 10);	break;
	case 11:	V11To13ConvertFromCSV(csv, 11);	break;
	case 12:	V11To13ConvertFromCSV(csv, 12);	break;
	case 13:	V11To13ConvertFromCSV(csv, 13);	break;
	case 14:	V14To26ConvertFromCSV(csv, 14);	break;
	case 15:	V14To26ConvertFromCSV(csv, 15);	break;
	case 16:	V14To26ConvertFromCSV(csv, 16);	break;
	case 17:	V14To26ConvertFromCSV(csv, 17);	break;
	case 18:	V14To26ConvertFromCSV(csv, 18);	break;
	case 19:	V14To26ConvertFromCSV(csv, 19);	break;
	case 20:	V14To26ConvertFromCSV(csv, 20);	break;
	case 21:	V14To26ConvertFromCSV(csv, 21);	break;
	case 22:	V14To26ConvertFromCSV(csv, 22);	break;
	case 23:	V14To26ConvertFromCSV(csv, 23);	break;
	case 24:	V14To26ConvertFromCSV(csv, 24);	break;
	case 25:	V14To26ConvertFromCSV(csv, 25);	break;
	case 26:	V14To26ConvertFromCSV(csv, 26);	break;
	case 27:	X500V27To32ConvertFromCSV(csv, 27);	break;
	case 28:	X500V27To32ConvertFromCSV(csv, 28);	break;
	case 29:	X500V27To32ConvertFromCSV(csv, 29);	break;
	case 30:	SPOSV30To36ConvertFromCSV(csv, 30);	break;
	case 31:	SPOSV30To36ConvertFromCSV(csv, 31);	break;
	case 32:	X500V27To32ConvertFromCSV(csv, 32);	break;
	case 33:	SPOSV30To36ConvertFromCSV(csv, 33);	break;
	case 34:	SPOSV30To36ConvertFromCSV(csv, 34);	break;
	case 35:	SPOSV30To36ConvertFromCSV(csv, 35);	break;
	case 36:	SPOSV30To36ConvertFromCSV(csv, 36);	break;
	case 37:	SPOSV37To38ConvertFromCSV(csv, 37);	break;
	case 38:	SPOSV37To38ConvertFromCSV(csv, 38);	break;
	case 39:	SPOSV39To43ConvertFromCSV(csv, 39);	break;
	case 40:	SPOSV39To43ConvertFromCSV(csv, 40);	break;
	case 41:	SPOSV39To43ConvertFromCSV(csv, 41);	break;
	case 42:	SPOSV39To43ConvertFromCSV(csv, 42);	break;
	case 43:	SPOSV39To43ConvertFromCSV(csv, 43);	break;
	}

	if (FALSE == m_bReportCostEnable)
	{
		m_dReportCost = 0.0;
	}
}

/********************************************************************/

void CPluCSVRecord::SPOSV39To43ConvertFromCSV(CCSV& csv, int nVer)
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

	if (nVer >= 40)
	{
		SetStampWeight(csv.GetInt(n++));
	}

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

	for (int i = 0; i < PLU_ORDER_PRINTERS_NOW; i++)
	{
		SetOrderPrinterFlag(i, csv.GetBool(n++));
	}

	SetPriority(csv.GetInt(n++));

	//MODIFIERS

	if (nVer >= 42)
	{
		SetAllowModifierFlag(csv.GetBool(n++));
	}

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
	SetAgeVerificationType(csv.GetInt(n++));
	SetKPNotAloneFlag(csv.GetBool(n++));
	SetKPConsolidateTopItemFlag(csv.GetBool(n++));
	SetKPImagePrintFlag(csv.GetBool(n++));
	SetKPImageModifierFlag(csv.GetBool(n++));
	SetAutoCoversQtyFlag(csv.GetBool(n++));
	SetEANOnReceiptFlag(csv.GetBool(n++));
	SetSalesTrackingFlag(csv.GetBool(n++));
	SetSoldOutFlag(csv.GetBool(n++));
	SetDiscountableFlag(csv.GetBool(n++));
	SetStaffDiscountFlag(csv.GetBool(n++));
	SetFreeTextFlag(csv.GetBool(n++));
	SetFreeTextDescriptionFlag(csv.GetBool(n++));

	if (nVer >= 43)
	{
		SetAccountRequiredFlag(csv.GetBool(n++));
	}

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

	if (nVer >= 41)
	{
		SetLinkSellTiedItemFlag(csv.GetBool(n++));
	}

	SetLinkTableString( csv.GetString(n++) );
	SetEcrmanRecipeTableString( csv.GetString(n++) );
	SetImageFilename( csv.GetString(n++) );
	SetActiveItemFlag( csv.GetBool(n++) );

	//INTERNAL FIELDS
	SetModifiedDateTimePlu( csv.GetString(n++) );
	SetModifiedDateTimeInfo( csv.GetString(n++) );
	SetDownloadDatePlu( csv.GetString(n++) );
	SetExport ( csv.GetBool(n++) );
}

/********************************************************************/
/*																	*/
/*	Import															*/
/*																	*/
/*	Import must match current database csv record					*/
/*																	*/
/********************************************************************/

void CPluCSVRecord::ImportLine(CSSImportFile & import, CCSV & csv, bool bWithStock)
{
	int n = 0;

	//DESCRIPTION AND TYPE
	ImportInt64(csv, import, n, m_nPluNo, Plu::PluNo);

	if (TRUE == bWithStock)
	{
		n++;
	}

	n++;		// protected "VersionNo"
	ImportString(csv, import, n, m_strEposText, Plu::EposText, TRUE);
	ImportString(csv, import, n, m_strKeyText, Plu::KeyText, TRUE);
	ImportString(csv, import, n, m_strRepText, Plu::RepText);
	ImportString(csv, import, n, m_strSortCode, Plu::SortCode);
	ImportInt(csv, import, n, m_nAgressoGLCode, Plu::AgressoGLCode);
	ImportString(csv, import, n, m_strAgressoCostCentre, Plu::AgressoCostCentre);
	ImportString(csv, import, n, m_strAgressoProject, Plu::AgressoProject);

	ImportInt(csv, import, n, m_nBaseDeptNo, Plu::DeptNo);
	ImportInt(csv, import, n, m_nBaseAnalysisCategory, Plu::AnalysisCategory);
	ImportInt(csv, import, n, m_nMicrotrendPluType, Plu::MicrotrendPluType);
	ImportInt(csv, import, n, m_nExternalProcess, Plu::ExternalProcess);

	{
		CString strTaxBand = "XXX";
		ImportString(csv, import, n, strTaxBand, Plu::TaxBand);

		if ((strTaxBand.GetLength() == 1) && (strTaxBand[0] >= '0') && (strTaxBand[0] <= '6'))
		{
			switch (strTaxBand[0] - '0')
			{
			case 0: strTaxBand = TaxImportMap.GetTaxMap0(); break;
			case 1: strTaxBand = TaxImportMap.GetTaxMap1(); break;
			case 2: strTaxBand = TaxImportMap.GetTaxMap2(); break;
			case 3: strTaxBand = TaxImportMap.GetTaxMap3(); break;
			case 4: strTaxBand = TaxImportMap.GetTaxMap4(); break;
			case 5: strTaxBand = TaxImportMap.GetTaxMap5(); break;
			case 6: strTaxBand = TaxImportMap.GetTaxMap6(); break;
			}
		}

		if (strTaxBand != "XXX")
		{
			SetBaseTaxBandString(strTaxBand);
		}
	}

	//PRICES AND POINTS
	for (int i = 0; i < MAX_PLU_PRICES; i++)
	{
		ImportDouble(csv, import, n, m_dPrice[i], Plu::Price1);
	}

	ImportInt(csv, import, n, m_nPoints, Plu::Points);
	ImportInt(csv, import, n, m_nStampWeight, Plu::StampWeight);

	//COST AND PROFIT
	ImportDouble(csv, import, n, m_dPurchaseCost, Plu::PurchaseCost);
	ImportBool(csv, import, n, m_bReportCostEnable);
	ImportDouble(csv, import, n, m_dReportCost, Plu::ReportCost);
	ImportInt(csv, import, n, m_nGPType, Plu::GPType);
	ImportDouble(csv, import, n, m_dProfitMargin, Plu::ProfitMargin);

	//ALLERGENS
	for (int i = 0; i < MAX_PLU_ALLERGENS; i++)
	{
		ImportInt(csv, import, n, m_nAllergenNo[i], Plu::AllergenNo1);
	}

	//KEYBOARD LISTS
	ImportInt(csv, import, n, m_nFirstMenu, Plu::FirstMenu);
	ImportInt(csv, import, n, m_nFirstQualifier, Plu::FirstQualifier);
	ImportInt(csv, import, n, m_nFirstExtra, Plu::FirstExtra);

	//ORDER PRINTERS
	ImportInt(csv, import, n, m_nOrderPrinterGroup, Plu::OrderPrinterGroup);

	for (int i = 0; i < PLU_ORDER_PRINTERS_NOW; i++)
	{
		ImportBool(csv, import, n, m_bOrderPrinterFlag[i]);
	}

	ImportInt(csv, import, n, m_nPriority, Plu::Priority);

	//MODIFIERS
	ImportBool(csv, import, n, m_bAllowModifier);
	for (int m = 1; m <= PLU_MODIFIERS; m++)
	{
		ImportBool(csv, import, n, m_bModifierEnable[m - 1]);

		{
			bool bNewFlag = GetModifierEposTextFlag(m);
			CString strNewText = GetModifierEposText(m);

			ImportBool(csv, import, n, bNewFlag);
			ImportString(csv, import, n, strNewText, Plu::Modifier1EposText, TRUE);

			SetModifierEposText(m, bNewFlag, strNewText);
		}

		{
			bool bNewFlag = GetModifierKeyTextFlag(m);
			CString strNewText = GetModifierKeyText(m);

			ImportBool(csv, import, n, bNewFlag);
			ImportString(csv, import, n, strNewText, Plu::Modifier1KeyText, TRUE);

			SetModifierKeyText(m, bNewFlag, strNewText);
		}

		ImportInt(csv, import, n, m_nModifierDeptNo[m - 1], Plu::Modifier1DeptNo);
		ImportInt(csv, import, n, m_nModifierAnalysisCategory[m - 1], Plu::Modifier1AnalysisCategory);
		ImportInt(csv, import, n, m_nModifierMicrotrendPluType[m - 1], Plu::Modifier1MicrotrendPluType);

		{
			CString strTaxBand = "XXX";
			ImportString(csv, import, n, strTaxBand, Plu::Modifier1TaxBand);

			if ((strTaxBand.GetLength() == 1) && (strTaxBand[0] >= '0') && (strTaxBand[0] <= '6'))
			{
				switch (strTaxBand[0] - '0')
				{
				case 0: strTaxBand = TaxImportMap.GetTaxMap0(); break;
				case 1: strTaxBand = TaxImportMap.GetTaxMap1(); break;
				case 2: strTaxBand = TaxImportMap.GetTaxMap2(); break;
				case 3: strTaxBand = TaxImportMap.GetTaxMap3(); break;
				case 4: strTaxBand = TaxImportMap.GetTaxMap4(); break;
				case 5: strTaxBand = TaxImportMap.GetTaxMap5(); break;
				case 6: strTaxBand = TaxImportMap.GetTaxMap6(); break;
				}
			}

			if (strTaxBand != "XXX")
			{
				SetModifierTaxBandStringRaw(m, strTaxBand);
			}
		}

		for (int l = 1; l <= MAX_PLU_PRICES; l++)
		{
			ImportDouble(csv, import, n, m_dModifierPrice[m - 1][l - 1], Plu::Modifier1Price1);
		}

		ImportInt(csv, import, n, m_nModifierFirstMenu[m - 1], Plu::Modifier1FirstMenu);
		ImportInt(csv, import, n, m_nModifierFirstQualifier[m - 1], Plu::Modifier1FirstQualifier);
		ImportInt(csv, import, n, m_nModifierFirstExtra[m - 1], Plu::Modifier1FirstExtra);
		ImportInt(csv, import, n, m_nModifierOrderPrinterGroup[m - 1], Plu::Modifier1OrderPrinterGroup);
	}

	//MISC PROGRAMMING
	ImportInt(csv, import, n, m_nPriceType, Plu::PriceType);
	ImportInt(csv, import, n, m_nAgeVerificationType, Plu::AgeVerificationType);
	ImportBool(csv, import, n, m_bKPNotAlone);
	ImportBool(csv, import, n, m_bKPConsolidateTopItem);
	ImportBool(csv, import, n, m_bKPImagePrint);
	ImportBool(csv, import, n, m_bKPImageModifier);
	ImportBool(csv, import, n, m_bAutoCoversQty);
	ImportBool(csv, import, n, m_bEANOnReceipt);
	ImportBool(csv, import, n, m_bSalesTracking);
	ImportBool(csv, import, n, m_bSoldOut);
	ImportBool(csv, import, n, m_bDiscountable);
	ImportBool(csv, import, n, m_bStaffDiscount);
	ImportBool(csv, import, n, m_bFreeText);
	ImportBool(csv, import, n, m_bFreeTextDescription);
	ImportBool(csv, import, n, m_bAccountRequired);
	
	//LABELS
	ImportBool(csv, import, n, m_bPrintShelf);
	ImportBool(csv, import, n, m_bPrintProduct);
	ImportString(csv, import, n, m_strUnitPriceText, Plu::UnitPriceText);
	ImportDouble(csv, import, n, m_dUnitPriceSize, Plu::UnitPriceSize);

	//ECRMAN STOCK
	ImportDouble(csv, import, n, m_dApparentLevel, Plu::ApparentLevel);
	ImportDouble(csv, import, n, m_dMinimumLevel, Plu::MinimumLevel);
	ImportString(csv, import, n, m_strSupplierRef, Plu::SupplierRef);
	ImportInt(csv, import, n, m_nSupplierNo, Plu::SupplierNo);
	ImportBool(csv, import, n, m_bNonStock);

	//MISC
	n++;		// protected Plu Info Modifier Flag
	ImportInt(csv, import, n, m_nLinkType, Plu::LinkType);
	ImportBool(csv, import, n, m_bLinkSellTiedItem);
	ImportString(csv, import, n, m_strLinkTable, Plu::LinkTable);
	ImportString(csv, import, n, m_strEcrmanRecipeTable, Plu::EcrmanRecipeTable);
	ImportString(csv, import, n, m_strImageFilename, Plu::ImageFilename);
	ImportBool(csv, import, n, m_bActive);

	//INTERNAL
	n++;		// protected "ModifiedDate Plu"
	n++;		// protected "ModifiedDate Info"
	n++;		// protected "DownloadDate Plu"
	n++;		// protected "Export"

	//NON DATABASE
	{
		int nDiscountType = -1;
		ImportInt(csv, import, n, nDiscountType, Plu::LegacyDiscountType);

		if (nDiscountType != -1)
		{
			SetDiscountableFlag(1 == nDiscountType);
			SetStaffDiscountFlag(2 == nDiscountType);
		}
	}

	if (m_nAgressoGLCode < 1000)
	{
		m_nAgressoGLCode = 0;
	}
}

/********************************************************************/

void CPluCSVRecord::ImportString( CCSV& csv, CSSImportFile& import, int& n, CString& str, const recString& rec, bool bFilter )
{
	if ( import.IsRequired(n) == TRUE )
	{
		CString strIn;
		if ( FALSE == bFilter )
		{
			strIn = csv.GetString(n);
		}
		else
		{
			strIn = FilterEposText( csv.GetString(n) );
		}

		Import ( strIn, str, rec );	
	}
	
	n++;
}

/**********************************************************************/

void CPluCSVRecord::ImportDouble( CCSV& csv, CSSImportFile& import, int& n, double& d, const recDouble& rec )
{
	if ( import.IsRequired(n) == TRUE )
	{
		Import ( csv.GetString(n), d, rec );	
	}

	n++;
}

/**********************************************************************/

void CPluCSVRecord::ImportInt( CCSV& csv, CSSImportFile& import, int& n, int& i, const recInt& rec )
{
	if ( import.IsRequired(n) == TRUE )
	{
		Import ( csv.GetString(n), i, rec );
	}
	
	n++;
}

/**********************************************************************/

void CPluCSVRecord::ImportInt64( CCSV& csv, CSSImportFile& import, int& n, __int64& i, const recInt64& rec )
{
	if ( import.IsRequired(n) == TRUE )
	{
		Import ( csv.GetString(n), i, rec );	
	}

	n++;
}

/**********************************************************************/

void CPluCSVRecord::ImportBool( CCSV& csv, CSSImportFile& import, int& n, bool& b )
{
	if ( import.IsRequired(n) == TRUE )
	{
		Import ( csv.GetString(n), b );	
	}

	n++;
}

/**********************************************************************/
