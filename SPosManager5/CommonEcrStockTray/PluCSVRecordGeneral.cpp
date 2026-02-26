/**********************************************************************/
#include "..\CommonEcrStock\CommsExportBase.h"
/**********************************************************************/
#include "AnalysisCategoryCSVArray.h"
#include "DepartmentCSVArray.h"
#include "SPOSVersions.h"
#include "NetworkCSVArray.h"
#include "PluTypeHelpers.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/

CEcrmanRecipeItem::CEcrmanRecipeItem()
{
	Reset();
}

/**********************************************************************/

void CEcrmanRecipeItem::Reset()
{
	m_nPluNo = 0;
	m_dStockQty = 0.0;
}

/**********************************************************************/

void CEcrmanRecipeItem::Add ( CEcrmanRecipeItem& source )
{
	m_dStockQty += source.m_dStockQty;
}

/**********************************************************************/

int CEcrmanRecipeItem::Compare( CEcrmanRecipeItem& source, int nHint )
{
	if (m_nPluNo != source.m_nPluNo)
	{
		return (m_nPluNo > source.m_nPluNo ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPluCSVRecord::CPluCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CPluCSVRecord::ClearRecord()
{
	m_nPluNo = 0;
	m_strEposText = "";
	m_nBaseDeptNo = 1;
	m_strBaseTaxBand = "A";
	m_nMicrotrendPluType = 0;
	m_nExternalProcess = 0;
	m_nBaseAnalysisCategory = 1;
	m_nFirstMenu = 0;
	m_nFirstQualifier = 0;
	m_nFirstExtra = 0;
	m_nOrderPrinterGroup = 0;
	m_bDiscountable = TRUE;
	m_bStaffDiscount = FALSE;
	m_nPriority = 0;
	m_nPriceType = 2;
	m_bFreeText = FALSE;
	m_bFreeTextDescription = TRUE;
	m_bAccountRequired = FALSE;
	m_strModifiedDateTimePlu = "";
	m_strModifiedDateTimeInfo = "";
	m_strDownloadDatePlu = "";
	m_bExport = FALSE;
	m_strRepText = "";
	m_strSortCode = "";
	m_dPurchaseCost = 0.0;
	m_bReportCostEnable = FALSE;
	m_dReportCost = 0.0;
	m_nGPType = PLU_GPTYPE_VARIABLE;
	m_dProfitMargin = 0.0;
	m_dApparentLevel = 0.0;
	m_dMinimumLevel = 0.0;
	m_strUnitPriceText = "";
	m_dUnitPriceSize = 0.0;
	m_strSupplierRef = "";
	m_nSupplierNo = 0;
	m_bNonStock = FALSE;
	m_bKPImagePrint = TRUE;
	m_bKPImageModifier = FALSE;
	m_bSalesTracking = FALSE;
	m_bSoldOut = FALSE;
	m_bEANOnReceipt = TRUE;
	m_bPrintShelf = TRUE;
	m_bPrintProduct = TRUE;
	m_nPoints = 0;
	m_nStampWeight = 1;
	m_bPluInfoModifier = FALSE;

	for (int i = 0; i < MAX_PLU_ALLERGENS; i++)
	{
		m_nAllergenNo[i] = 0;
	}

	for (int i = 0; i < MAX_PLU_PRICES; i++)
	{
		m_dPrice[i] = 0.0;
	}

	for (int i = 0; i < PLU_ORDER_PRINTERS_NOW; i++)
	{
		m_bOrderPrinterFlag[i] = FALSE;
	}

	for (int m = 0; m < PLU_MODIFIERS; m++)
	{
		m_bModifierEnable[m] = FALSE;
		m_bModifierEposText[m] = FALSE;
		m_strModifierEposText[m] = "";
		m_bModifierKeyText[m] = FALSE;
		m_strModifierKeyText[m] = "";
		m_nModifierOrderPrinterGroup[m] = 0;
		m_strModifierTaxBand[m] = "*";
		m_nModifierDeptNo[m] = -1;
		m_nModifierAnalysisCategory[m] = -1;
		m_nModifierMicrotrendPluType[m] = -1;
		m_nModifierFirstMenu[m] = -1;
		m_nModifierFirstQualifier[m] = -1;
		m_nModifierFirstExtra[m] = -1;

		for (int l = 0; l < MAX_PLU_PRICES; l++)
		{
			m_dModifierPrice[m][l] = 0.0;
		}
	}

	{
		int nDeptNo1 = 0;
		int nDeptNo2 = 0;

		int nSize = DataManager.Department.GetSize();

		if (nSize >= 1)
		{
			CDepartmentCSVRecord DeptRecord;
			DataManager.Department.GetAt(0, DeptRecord);
			nDeptNo1 = DeptRecord.GetDeptNo();
		}

		if ((nSize >= 2) && (0 == nDeptNo1))
		{
			CDepartmentCSVRecord DeptRecord;
			DataManager.Department.GetAt(1, DeptRecord);
			nDeptNo2 = DeptRecord.GetDeptNo();
		}

		if (nDeptNo1 != 0)
		{
			m_nBaseDeptNo = nDeptNo1;
		}
		else
		{
			m_nBaseDeptNo = nDeptNo2;
		}
	}

	m_nLinkType = 0;
	m_bLinkSellTiedItem = FALSE;
	m_strLinkTable = "";
	m_strEcrmanRecipeTable = "";
	m_nAgeVerificationType = 0;
	m_bAutoCoversQty = FALSE;
	m_bKPNotAlone = FALSE;
	m_bKPConsolidateTopItem = FALSE;
	m_nAgressoGLCode = 0;
	m_strAgressoCostCentre = "";
	m_strAgressoProject = "";
	m_bActive = TRUE;
	m_bAllowModifier = TRUE;
}

/**********************************************************************/

int CPluCSVRecord::GetChange ( CPluCSVRecord& plu )
{
	if (plu.m_nPluNo != m_nPluNo)
	{
		return EPOS_CHANGE;
	}

	//	dont check "VersionNo"
	
	if (plu.m_strEposText != m_strEposText)
	{
		return EPOS_CHANGE;
	}

	if (plu.m_strKeyText != m_strKeyText)
	{
		return EPOS_CHANGE;
	}

	for (int i = 0; i < MAX_PLU_PRICES; i++)
	{
		if (plu.m_dPrice[i] != m_dPrice[i])
		{
			return EPOS_CHANGE;
		}
	}

	if ( plu.m_nBaseDeptNo != m_nBaseDeptNo )	
		return EPOS_CHANGE;

	if ( plu.m_strBaseTaxBand != m_strBaseTaxBand )	
		return EPOS_CHANGE;

	if ( plu.m_nMicrotrendPluType != m_nMicrotrendPluType )	
		return EPOS_CHANGE;

	if ( plu.m_nExternalProcess != m_nExternalProcess )	
		return EPOS_CHANGE;

	if ( plu.m_nBaseAnalysisCategory != m_nBaseAnalysisCategory )
		return EPOS_CHANGE;

	for (int i = 0; i < MAX_PLU_ALLERGENS; i++)
	{
		if (plu.m_nAllergenNo[i] != m_nAllergenNo[i])
		{
			return EPOS_CHANGE;
		}
	}

	if ( plu.m_nFirstMenu != m_nFirstMenu )				
		return EPOS_CHANGE;

	if ( plu.m_nFirstQualifier != m_nFirstQualifier )	
		return EPOS_CHANGE;

	if ( plu.m_nFirstExtra != m_nFirstExtra )
		return EPOS_CHANGE;

	if ( plu.m_nOrderPrinterGroup != m_nOrderPrinterGroup )
		return EPOS_CHANGE;
	
	for (int i = 0; i < PLU_ORDER_PRINTERS_NOW; i++)
	{
		if (plu.m_bOrderPrinterFlag[i] != m_bOrderPrinterFlag[i])
		{
			return EPOS_CHANGE;
		}
	}

	if ( plu.m_bDiscountable != m_bDiscountable )		
		return EPOS_CHANGE;

	if ( plu.m_bStaffDiscount != m_bStaffDiscount )	
		return EPOS_CHANGE;

	if ( plu.m_nPriority != m_nPriority )
		return EPOS_CHANGE;

	if ( plu.m_nPriceType != m_nPriceType )
		return EPOS_CHANGE;

	if ( plu.m_bFreeText != m_bFreeText )
		return EPOS_CHANGE;

	if ( plu.m_bFreeTextDescription != m_bFreeTextDescription )
		return EPOS_CHANGE;

	if ( plu.m_bAccountRequired != m_bAccountRequired )
		return EPOS_CHANGE;

	if ( plu.m_bKPImagePrint != m_bKPImagePrint )
		return EPOS_CHANGE;

	if ( plu.m_bKPImageModifier != m_bKPImageModifier )	
		return EPOS_CHANGE;

	if ( plu.m_bSalesTracking != m_bSalesTracking )
		return EPOS_CHANGE;

	if ( plu.m_bSoldOut != m_bSoldOut )	
		return EPOS_CHANGE;

	if ( plu.m_bEANOnReceipt != m_bEANOnReceipt )	
		return EPOS_CHANGE;

	if ( plu.m_nLinkType != m_nLinkType )
		return EPOS_CHANGE;
	
	if ( plu.m_bLinkSellTiedItem != m_bLinkSellTiedItem )
		return EPOS_CHANGE;
	
	if ( plu.m_strLinkTable != m_strLinkTable )		
		return EPOS_CHANGE;

	if ( plu.m_nAgeVerificationType != m_nAgeVerificationType )
		return EPOS_CHANGE;

	if ( plu.m_bAutoCoversQty != m_bAutoCoversQty )
		return EPOS_CHANGE;

	if ( plu.m_bKPNotAlone != m_bKPNotAlone )
		return EPOS_CHANGE;

	if ( plu.m_bKPConsolidateTopItem != m_bKPConsolidateTopItem )
		return EPOS_CHANGE;

	if ( plu.m_strImageFilename != m_strImageFilename )	
		return EPOS_CHANGE;

	if ( plu.m_bActive != m_bActive )
		return EPOS_CHANGE;

	if (plu.m_bAllowModifier != m_bAllowModifier)
		return EPOS_CHANGE;

	for ( int m = 0; m < PLU_MODIFIERS; m++ )
	{
		if (plu.m_bModifierEnable[m] != m_bModifierEnable[m])
		{
			return EPOS_CHANGE;
		}

		if (plu.m_bModifierEposText[m] != m_bModifierEposText[m])
		{
			return EPOS_CHANGE;
		}
		
		if (plu.m_strModifierEposText[m] != m_strModifierEposText[m])
		{
			return EPOS_CHANGE;
		}

		if (plu.m_bModifierKeyText[m] != m_bModifierKeyText[m])
		{
			return EPOS_CHANGE;
		}
		
		if (plu.m_strModifierKeyText[m] != m_strModifierKeyText[m])
		{
			return EPOS_CHANGE;
		}

		if (plu.m_nModifierOrderPrinterGroup[m] != m_nModifierOrderPrinterGroup[m])
		{
			return EPOS_CHANGE;
		}

		if (plu.m_strModifierTaxBand[m] != m_strModifierTaxBand[m])
		{
			return EPOS_CHANGE;
		}

		if (plu.m_nModifierAnalysisCategory[m] != m_nModifierAnalysisCategory[m])
		{
			return EPOS_CHANGE;
		}

		if (plu.m_nModifierMicrotrendPluType[m] != m_nModifierMicrotrendPluType[m])
		{
			return EPOS_CHANGE;
		}

		if (plu.m_nModifierDeptNo[m] != m_nModifierDeptNo[m])
		{
			return EPOS_CHANGE;
		}

		if (plu.m_nModifierFirstMenu[m] != m_nModifierFirstMenu[m])
		{
			return EPOS_CHANGE;
		}

		if (plu.m_nModifierFirstQualifier[m] != m_nModifierFirstQualifier[m])
		{
			return EPOS_CHANGE;
		}

		if (plu.m_nModifierFirstExtra[m] != m_nModifierFirstExtra[m])
		{
			return EPOS_CHANGE;
		}

		for (int l = 0; l < MAX_PLU_PRICES; l++)
		{
			if (plu.m_dModifierPrice[m][l] != m_dModifierPrice[m][l])
			{
				return EPOS_CHANGE;
			}
		}
	}

	//	dont check "ModifiedDate"
	//	dont check "Export"

	if ( plu.m_strEcrmanRecipeTable != m_strEcrmanRecipeTable)
		return SUPER_CHANGE;

	if ( plu.m_strDownloadDatePlu != m_strDownloadDatePlu )	
		return SUPER_CHANGE;

	if ( plu.m_bPluInfoModifier != m_bPluInfoModifier )	
		return SUPER_CHANGE;

	if ( plu.m_strRepText != m_strRepText )	
		return SUPER_CHANGE;

	if ( plu.m_strSortCode != m_strSortCode )	
		return SUPER_CHANGE;

	if ( plu.m_dPurchaseCost != m_dPurchaseCost )
		return SUPER_CHANGE;

	if ( plu.m_bReportCostEnable != m_bReportCostEnable )
		return SUPER_CHANGE;

	if ( plu.m_dReportCost != m_dReportCost )
		return SUPER_CHANGE;

	if ( plu.m_nGPType != m_nGPType )	
		return SUPER_CHANGE;

	if ( plu.m_dProfitMargin != m_dProfitMargin )
		return SUPER_CHANGE;

	if ( plu.m_dApparentLevel != m_dApparentLevel )	
		return SUPER_CHANGE;

	if ( plu.m_dMinimumLevel != m_dMinimumLevel )
		return SUPER_CHANGE;

	if ( plu.m_strUnitPriceText != m_strUnitPriceText )	
		return SUPER_CHANGE;

	if ( plu.m_dUnitPriceSize != m_dUnitPriceSize )	
		return SUPER_CHANGE;

	if ( plu.m_strSupplierRef != m_strSupplierRef )	
		return SUPER_CHANGE;

	if ( plu.m_nSupplierNo != m_nSupplierNo )
		return SUPER_CHANGE;

	if ( plu.m_bNonStock != m_bNonStock )
		return SUPER_CHANGE;

	if ( plu.m_bPrintShelf != m_bPrintShelf )
		return SUPER_CHANGE;

	if ( plu.m_bPrintProduct != m_bPrintProduct )
		return SUPER_CHANGE;

	if ( plu.m_nPoints != m_nPoints )
		return SUPER_CHANGE;

	if ( plu.m_nStampWeight != m_nStampWeight)	
		return SUPER_CHANGE;

	if ( plu.m_nAgressoGLCode != m_nAgressoGLCode )	
		return SUPER_CHANGE;

	if ( plu.m_strAgressoCostCentre != m_strAgressoCostCentre )	
		return SUPER_CHANGE;

	if ( plu.m_strAgressoProject != m_strAgressoProject )
		return SUPER_CHANGE;
	
	return NO_CHANGE;
}

/**********************************************************************/

const char* CPluCSVRecord::GetPluTypeText( int nMod )
{
	int nPluType = m_nMicrotrendPluType;

	CString strPrefix = "";
	CString strSuffix = "";
	
	if ( ( nMod >= 1 ) && ( nMod <= 9 ) )
	{
		int nModPluType = GetModifierMicrotrendPluType(nMod);

		if ( nModPluType != -1 )
		{
			nPluType = nModPluType;
		}
		else
		{
			strPrefix = "[";
			strSuffix = "]";
		}
	}

	m_strPluTypeText = "";
	m_strPluTypeText += strPrefix;
	m_strPluTypeText += CPluTypeHelpers::GetText(nPluType);
	m_strPluTypeText += strSuffix;

	return m_strPluTypeText;
}

/**********************************************************************/

const char* CPluCSVRecord::GetExternalProcessText()
{
	switch ( m_nExternalProcess )
	{
	case 0:		
		m_strExternalProcessText = "None";
		break;
	
	case 1:		
	case 2:
	case 3:
	case 4:
		m_strExternalProcessText.Format( "%d", m_nExternalProcess );
		break;

	default:	
		m_strExternalProcessText = "Unknown";	
		break;
	}
	
	return m_strExternalProcessText;
}

/**********************************************************************/

const char* CPluCSVRecord::GetCategoryText( int nMod )
{
	int nCat = m_nBaseAnalysisCategory;

	CString strPrefix = "";
	CString strSuffix = "";
	
	if ( ( nMod >= 1 ) && ( nMod <= 9 ) )
	{
		int nModCat = GetModifierAnalysisCategoryRaw(nMod);

		if ( nModCat != -1 )
		{
			nCat = nModCat;
		}
		else
		{
			strPrefix = "[";
			strSuffix = "]";
		}
	}

	m_strCategoryText = strPrefix;
	m_strCategoryText += DataManager.AnalysisCategory.GetDisplayText( nCat );
	m_strCategoryText += strSuffix;

	return m_strCategoryText;
}

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

const char* CPluCSVRecord::GetAllergenText(int n)
{
	m_strAllergenText = "";

	if ((n >= 0) && (n < MAX_PLU_ALLERGENS))
	{
		m_strAllergenText = DataManagerNonDb.Allergen.GetGroupReportText(m_nAllergenNo[n]);
	}

	return m_strAllergenText;
}

/**********************************************************************/
#endif
/**********************************************************************/

const char* CPluCSVRecord::GetPriceTypeText()
{
	switch ( m_nPriceType )
	{
	case 0:	
		m_strPriceTypeText = "Non-Priced";	
		break;
	
	case 1:	
		m_strPriceTypeText = "Price Check";
		break;

	case 2:	
		m_strPriceTypeText = "Preset";
		break;

	default:	
		m_strPriceTypeText = "Unknown";
		break;
	}

	return m_strPriceTypeText;
}

/**********************************************************************/

const char* CPluCSVRecord::GetAgeVerificationTypeText()
{
	switch ( m_nAgeVerificationType )
	{
	case 0:	
		m_strAgeVerificationTypeText = "None";
		break;

	case 1:	
		m_strAgeVerificationTypeText = "16";
		break;

	case 2:	
		m_strAgeVerificationTypeText = "18";
		break;

	case 3:	
		m_strAgeVerificationTypeText = "21";
		break;

	case 4:		
		m_strAgeVerificationTypeText = "25";
		break;

	default:	
		m_strAgeVerificationTypeText = "Unknown";	
		break;
	}
	return m_strAgeVerificationTypeText;
}

/**********************************************************************/

void CPluCSVRecord::SetDefaultText()
{
	SetRepText( "" );

	switch( EcrmanOptions.GetPluDefaultTextType() )
	{
	case 1:
		{
			CString str1 = "Plu ";
			CString str2 = GetPluNoString();
			::TrimSpaces( str2, TRUE );
			SetEposText( str1 + str2 );
		}
		break;

	case 2:
		{
			CString str = "Plu ";
			str += GetPluNoString();
			SetEposText( str );
		}
		break;

	case 0:
	default:
		SetEposText( "Plu" );
		break;
	}	
}

/**********************************************************************/

void CPluCSVRecord::GetLinkTableArray(CArray<__int64, __int64>& arrayPluNo)
{
	arrayPluNo.RemoveAll();

	CCSV csv(m_strLinkTable);

	for (int x = 0; (x < MAX_LINK_PLU_ITEMS) && (x < csv.GetSize()); x++)
	{
		__int64 nPluNo = csv.GetInt64(x);

		if (0 == nPluNo)
		{
			break;
		}

		arrayPluNo.Add(nPluNo);
	}
}

/**********************************************************************/

void CPluCSVRecord::SetLinkTableArray( CArray<__int64,__int64>& arrayPluNo )
{
	CCSV csv;
	int nSize = 0;

	for (int x = 0; x < arrayPluNo.GetSize(); x++)
	{
		__int64 nPluNo = arrayPluNo[x];
		if ((nPluNo < Plu::PluNo.Min) || (nPluNo > Plu::PluNo.Max))
		{
			continue;
		}

		csv.Add(nPluNo);
		nSize++;

		if (nSize >= MAX_LINK_PLU_ITEMS)
		{
			break;
		}
	}

	m_strLinkTable = csv.GetLine();
}

/**********************************************************************/

void CPluCSVRecord::GetEcrmanRecipeTableArray( CReportConsolidationArray<CEcrmanRecipeItem>& arrayPluNo )
{
	arrayPluNo.RemoveAll();

	CCSV csv( m_strEcrmanRecipeTable );

	for ( int x = 0; x < csv.GetSize() - 1; x += 2 )
	{
		__int64 nSSPluNo = csv.GetInt64(x);
		double dStockQty = csv.GetDouble(x+1);

		if ( ( nSSPluNo >= Plu::PluNo.Min ) && ( nSSPluNo <= Plu::PluNo.Max ) && ( dStockQty > 0.0 ) )
		{
			CEcrmanRecipeItem infoItem;
			infoItem.m_nPluNo = nSSPluNo;
			infoItem.m_dStockQty = dStockQty;

			int nItemIdx;
			if (arrayPluNo.Find(infoItem, nItemIdx) == FALSE)
			{
				arrayPluNo.InsertAt(nItemIdx, infoItem);

				if (arrayPluNo.GetSize() >= MAX_RECIPE_PLU_ITEMS)
				{
					break;
				}
			}
		}
	}
}

/**********************************************************************/

bool CPluCSVRecord::GotEcrmanRecipeTableArray()
{
	bool bResult = FALSE;

	CCSV csv( m_strEcrmanRecipeTable );
	
	for ( int x = 0; x < csv.GetSize() - 1; x += 2 )
	{
		__int64 nSSPluNo = csv.GetInt64(x);
		double dStockQty = csv.GetDouble(x+1);

		if ( ( nSSPluNo >= Plu::PluNo.Min ) && ( nSSPluNo <= Plu::PluNo.Max ) && ( dStockQty > 0.0 ) )
		{
			bResult = TRUE;
			break;
		}
	}

	return bResult;
}

/**********************************************************************/

void CPluCSVRecord::SetEcrmanRecipeTableArray( CReportConsolidationArray<CEcrmanRecipeItem>& arrayPluNo )
{
	CCSV csv;
	int nSize = 0;

	for ( int x = 0; x < arrayPluNo.GetSize(); x++ )
	{
		CEcrmanRecipeItem infoItem;
		arrayPluNo.GetAt( x, infoItem );

		if ((infoItem.m_nPluNo >= Plu::PluNo.Min) && (infoItem.m_nPluNo <= Plu::PluNo.Max) && (infoItem.m_dStockQty > 0.0))
		{
			csv.Add(infoItem.m_nPluNo);
			csv.Add(infoItem.m_dStockQty, 3, TRUE);
			nSize++;

			if (nSize >= MAX_RECIPE_PLU_ITEMS)
			{
				break;
			}
		}
	}

	m_strEcrmanRecipeTable = csv.GetLine();
}

/**********************************************************************/

bool CPluCSVRecord::GetNonStockExternalFlag()
{

#ifdef STOCKMAN_SYSTEM
	
	return m_bNonStock;

#else

	return ( m_bNonStock || GotEcrmanRecipeTableArray() );

#endif
}

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

void CPluCSVRecord::CheckMinimumSPOSVersion( CMinimumSPOSVersion& version, __int64& nSharpPluNo, bool& bModifiable )
{
	CCommsExportBase::CheckPluNo( SPOS_V4, m_nPluNo, nSharpPluNo, bModifiable );

	version.m_nMinVer = SPOS_V4;
	version.m_strReason = "";
	version.m_bInvalidData = FALSE;

	if (GetPriority() > 9)
	{
		version.Merge(SPOS_V4_2_1087, "Order Printer Priority 10 - 16");
	}

	if (GetBaseTaxBandInt() > 6)
	{
		version.Merge(SPOS_V4_2_1081, "Tax Band G - L");
	}

	for ( int n = 0; n < MAX_PLU_ALLERGENS; n++ )
	{
		if ( GetAllergenNo(n) != 0 )
		{
			version.Merge(SPOS_V4_2606, "Plu Allergen link");
			break;
		}
	}

	if ( GetBaseAnalysisCategory() > 9 )
	{
		version.Merge(SPOS_V4_2133, "Analysis Category 10 - 99");
	}

	if ( TRUE == bModifiable )
	{
		for ( int nMod = 1; nMod < 9; nMod++ )
		{
			if ( GetModifierEnable( nMod ) == TRUE )
			{
				if ( GetModifierAnalysisCategoryRaw(nMod) > 9 )
				{
					CString strReason = "";
					strReason.Format( "Modifier %d Analysis Category 10 - 99", nMod );
					version.Merge(SPOS_V4_2133, strReason);
					break;
				}
			}
		}

		for (int nMod = 1; nMod < 9; nMod++)
		{
			if (GetModifierEnable(nMod) == TRUE)
			{
				if (GetModifierTaxBandIntRaw(nMod) > 6)
				{
					CString strReason = "";
					strReason.Format("Modifier %d Tax Band G - L", nMod);
					version.Merge(SPOS_V4_2_1081, strReason);
					break;
				}
			}
		}
	}

	switch( GetMicrotrendPluType() )
	{
	case 12:
		version.Merge(SPOS_V4_2_1456, "Plu Type 'Loyalty Staff Gift'");
		break;

	case 11:
		version.Merge(SPOS_V4_2012, "Plu Type 'External'");
		break;

	case 10:
		version.Merge(SPOS_V4_1519, "Plu Type 'Loyalty Top-up'");
		break;

	case 9:
		version.Merge(SPOS_V4_1300, "Plu Type 'Ext Process Top-up'");
		break;

	case 8:
		version.Merge(SPOS_V4_999, "Plu Type 'Information'");
		break;
	}

	if ( GetAgeVerificationType() > 3 )
	{
		version.Merge(SPOS_V4_753, "Age verification type 25" );
	}

	if ( GetAgeVerificationType() > 0 )
	{
		version.Merge(SPOS_V4_719, "Age verification type 16, 18 or 21");
	}

	if ( GetAccountRequiredFlag() )
	{
		version.Merge(SPOS_V4_3_104, "Account Required Flag");
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
