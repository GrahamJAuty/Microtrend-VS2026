/**********************************************************************/
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewCustomList.h"
/**********************************************************************/

CStockTreeReportNewCustomList::CStockTreeReportNewCustomList( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_nReportParamsType = STOCK_CUSTOM_LIST;
	m_nTotalisersPerLine = 0;
	m_strCustomName = "";
}

/**********************************************************************/

void CStockTreeReportNewCustomList::PrepareReport()
{
	m_strReportName.Format ( "%s (%s)",
		GetReportTitle(),
		(const char*) m_strCustomName );

	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddReportColumns();
}

/**********************************************************************/

void CStockTreeReportNewCustomList::AddReportColumns()
{
	CCSV csvKey ( m_strKey );
	for (int nPos = 0; nPos < csvKey.GetSize(); nPos++)
	{
		int nField = csvKey.GetInt(nPos);
		
		if (nField == CustomStockList::StockCode.Type)			AddFieldToHeader(CustomStockList::StockCode);
		else if (nField == CustomStockList::Description.Type)		AddFieldToHeader(CustomStockList::Description);
		else if (nField == CustomStockList::ImportSheetCode.Type)	AddFieldToHeader(CustomStockList::ImportSheetCode);
		else if (nField == CustomStockList::CatName.Type)			AddFieldToHeader(CustomStockList::CatName);
		else if (nField == CustomStockList::CustomFieldValue1.Type)	AddFieldToHeader(CustomStockList::CustomFieldValue1, DataManagerNonDb.CustomFieldNames.GetName(1));
		else if (nField == CustomStockList::CustomFieldValue2.Type)	AddFieldToHeader(CustomStockList::CustomFieldValue2, DataManagerNonDb.CustomFieldNames.GetName(2));
		else if (nField == CustomStockList::CustomFieldValue3.Type)	AddFieldToHeader(CustomStockList::CustomFieldValue3, DataManagerNonDb.CustomFieldNames.GetName(3));
		else if (nField == CustomStockList::CustomFieldValue4.Type)	AddFieldToHeader(CustomStockList::CustomFieldValue4, DataManagerNonDb.CustomFieldNames.GetName(4));
		else if (nField == CustomStockList::CustomFieldValue5.Type)	AddFieldToHeader(CustomStockList::CustomFieldValue5, DataManagerNonDb.CustomFieldNames.GetName(5));
		else if (nField == CustomStockList::StockUnitName.Type)	AddFieldToHeader(CustomStockList::StockUnitName);
		else if (nField == CustomStockList::StockmateText.Type)	AddFieldToHeader(CustomStockList::StockmateText);
		else if (nField == CustomStockList::StockUnitDetail.Type)	AddFieldToHeader(CustomStockList::StockUnitDetail);
		else if (nField == CustomStockList::RetailUnitName.Type)	AddFieldToHeader(CustomStockList::RetailUnitName);
		else if (nField == CustomStockList::Wastage.Type)			AddFieldToHeader(CustomStockList::Wastage);
		else if (nField == CustomStockList::LabelsFlag.Type)		AddFieldToHeader(CustomStockList::LabelsFlag);
		else if (nField == CustomStockList::OrderFlag.Type)		AddFieldToHeader(CustomStockList::OrderFlag);
		else if (nField == CustomStockList::PluRangeFlag.Type)	AddFieldToHeader(CustomStockList::PluRangeFlag);
		else if (nField == CustomStockList::Apparent.Type)		AddFieldToHeader(CustomStockList::Apparent);
		else if (nField == CustomStockList::SupplierName1.Type)	AddFieldToHeader(CustomStockList::SupplierName1);
		else if (nField == CustomStockList::SupplierName2.Type)	AddFieldToHeader(CustomStockList::SupplierName2);
		else if (nField == CustomStockList::SupplierName3.Type)	AddFieldToHeader(CustomStockList::SupplierName3);
		else if (nField == CustomStockList::SupplierName4.Type)	AddFieldToHeader(CustomStockList::SupplierName4);
		else if (nField == CustomStockList::SupplierName5.Type)	AddFieldToHeader(CustomStockList::SupplierName5);
		else if (nField == CustomStockList::SupplierRef1.Type)	AddFieldToHeader(CustomStockList::SupplierRef1);
		else if (nField == CustomStockList::SupplierRef2.Type)	AddFieldToHeader(CustomStockList::SupplierRef2);
		else if (nField == CustomStockList::SupplierRef3.Type)	AddFieldToHeader(CustomStockList::SupplierRef3);
		else if (nField == CustomStockList::SupplierRef4.Type)	AddFieldToHeader(CustomStockList::SupplierRef4);
		else if (nField == CustomStockList::SupplierRef5.Type)	AddFieldToHeader(CustomStockList::SupplierRef5);
		else if (nField == CustomStockList::WrapperCode1.Type)	AddFieldToHeader(CustomStockList::WrapperCode1);
		else if (nField == CustomStockList::WrapperCode2.Type)	AddFieldToHeader(CustomStockList::WrapperCode2);
		else if (nField == CustomStockList::WrapperCode3.Type)	AddFieldToHeader(CustomStockList::WrapperCode3);
		else if (nField == CustomStockList::WrapperCode4.Type)	AddFieldToHeader(CustomStockList::WrapperCode4);
		else if (nField == CustomStockList::WrapperCode5.Type)	AddFieldToHeader(CustomStockList::WrapperCode5);
		else if (nField == CustomStockList::DelUnitName1.Type)	AddFieldToHeader(CustomStockList::DelUnitName1);
		else if (nField == CustomStockList::DelUnitName2.Type)	AddFieldToHeader(CustomStockList::DelUnitName2);
		else if (nField == CustomStockList::DelUnitName3.Type)	AddFieldToHeader(CustomStockList::DelUnitName3);
		else if (nField == CustomStockList::DelUnitName4.Type)	AddFieldToHeader(CustomStockList::DelUnitName4);
		else if (nField == CustomStockList::DelUnitName5.Type)	AddFieldToHeader(CustomStockList::DelUnitName5);
		else if (nField == CustomStockList::DelUnitDetail1.Type)	AddFieldToHeader(CustomStockList::DelUnitDetail1);
		else if (nField == CustomStockList::DelUnitDetail2.Type)	AddFieldToHeader(CustomStockList::DelUnitDetail2);
		else if (nField == CustomStockList::DelUnitDetail3.Type)	AddFieldToHeader(CustomStockList::DelUnitDetail3);
		else if (nField == CustomStockList::DelUnitDetail4.Type)	AddFieldToHeader(CustomStockList::DelUnitDetail4);
		else if (nField == CustomStockList::DelUnitDetail5.Type)	AddFieldToHeader(CustomStockList::DelUnitDetail5);
		else if (nField == CustomStockList::DelUnitYield1.Type)	AddFieldToHeader(CustomStockList::DelUnitYield1);
		else if (nField == CustomStockList::DelUnitYield2.Type)	AddFieldToHeader(CustomStockList::DelUnitYield2);
		else if (nField == CustomStockList::DelUnitYield3.Type)	AddFieldToHeader(CustomStockList::DelUnitYield3);
		else if (nField == CustomStockList::DelUnitYield4.Type)	AddFieldToHeader(CustomStockList::DelUnitYield4);
		else if (nField == CustomStockList::DelUnitYield5.Type)	AddFieldToHeader(CustomStockList::DelUnitYield5);
		else if (nField == CustomStockList::DelUnitCost1.Type)	AddFieldToHeader(CustomStockList::DelUnitCost1);
		else if (nField == CustomStockList::DelUnitCost2.Type)	AddFieldToHeader(CustomStockList::DelUnitCost2);
		else if (nField == CustomStockList::DelUnitCost3.Type)	AddFieldToHeader(CustomStockList::DelUnitCost3);
		else if (nField == CustomStockList::DelUnitCost4.Type)	AddFieldToHeader(CustomStockList::DelUnitCost4);
		else if (nField == CustomStockList::DelUnitCost5.Type)	AddFieldToHeader(CustomStockList::DelUnitCost5);
		else if (nField == CustomStockList::StockUnitCost1.Type)	AddFieldToHeader(CustomStockList::StockUnitCost1);
		else if (nField == CustomStockList::StockUnitCost2.Type)	AddFieldToHeader(CustomStockList::StockUnitCost2);
		else if (nField == CustomStockList::StockUnitCost3.Type)	AddFieldToHeader(CustomStockList::StockUnitCost3);
		else if (nField == CustomStockList::StockUnitCost4.Type)	AddFieldToHeader(CustomStockList::StockUnitCost4);
		else if (nField == CustomStockList::StockUnitCost5.Type)	AddFieldToHeader(CustomStockList::StockUnitCost5);
		else if (nField == CustomStockList::DelUnitRetail1.Type)	AddFieldToHeader(CustomStockList::DelUnitRetail1);
		else if (nField == CustomStockList::DelUnitRetail2.Type)	AddFieldToHeader(CustomStockList::DelUnitRetail2);
		else if (nField == CustomStockList::DelUnitRetail3.Type)	AddFieldToHeader(CustomStockList::DelUnitRetail3);
		else if (nField == CustomStockList::DelUnitRetail4.Type)	AddFieldToHeader(CustomStockList::DelUnitRetail4);
		else if (nField == CustomStockList::DelUnitRetail5.Type)	AddFieldToHeader(CustomStockList::DelUnitRetail5);
	}
}

/**********************************************************************/

void CStockTreeReportNewCustomList::AddFieldToHeader ( CustomStockField Field, const char* szCustomLabel )
{
	CString strLabel = szCustomLabel;

	if ( strLabel == "" )
		strLabel = Field.ReportLabel;
	
	if ( strLabel == "" ) 
		strLabel = Field.ListLabel;

	AddColumn( strLabel, Field.Orientation, Field.DefSize, FALSE );
}

/**********************************************************************/

bool CStockTreeReportNewCustomList::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );
	double dApparent = StockLevels.GetApparentQty();
	
	CCSV csvKey ( m_strKey );
	for ( int nPos = 0; nPos < csvKey.GetSize(); nPos++ )
	{
		int nField = csvKey.GetInt(nPos);
		
		if ( nField == CustomStockList::StockCode.Type )			
			csv.AppendString( ReportHelpers.GetDisplayStockCode( m_LabelStockRecord.GetStockCode() ) );
		
		else if ( nField == CustomStockList::Description.Type )	
			csv.AppendString( m_LabelStockRecord.GetDescription() );

		else if ( nField == CustomStockList::ImportSheetCode.Type )	
			csv.AppendString( m_LabelStockRecord.GetImportSheetCode() );
		
		else if ( nField == CustomStockList::CatName.Type )		
			csv.AppendString( DataManager.Category.GetText( m_LabelStockRecord.GetCategory() ) );

		else if ( nField == CustomStockList::CustomFieldValue1.Type )
			AppendCustomFieldValueName( csv, 1 );

		else if ( nField == CustomStockList::CustomFieldValue2.Type )
			AppendCustomFieldValueName( csv, 2 );

		else if ( nField == CustomStockList::CustomFieldValue3.Type )
			AppendCustomFieldValueName( csv, 3 );

		else if ( nField == CustomStockList::CustomFieldValue4.Type )
			AppendCustomFieldValueName( csv, 4 );

		else if ( nField == CustomStockList::CustomFieldValue5.Type )
			AppendCustomFieldValueName( csv, 5 );

		else if ( nField == CustomStockList::StockUnitName.Type )
			csv.AppendString( m_LabelStockRecord.GetStockUnitName() );

		else if (nField == CustomStockList::StockmateText.Type)
			csv.AppendString(m_LabelStockRecord.GetStockmateText());

		else if ( nField == CustomStockList::StockUnitDetail.Type )
		{
			if ( nSubUnits > 1 )
				csv.AppendString ( DataManager.Stock.GetStockSubUnitNameForPluEdit ( m_LabelStockRecord ) );
			else  
			{
				double dWeight = m_LabelStockRecord.GetSUWeight(TRUE);

				if ( CPriceHelpers::CompareDoubles ( dWeight, 0.0, 3 ) != 0 )
				{
					CString strText;
					strText.Format ( "%.5f x ", dWeight );

					switch ( m_LabelStockRecord.GetUnitWeightType() )
					{
					case WEIGHTTYPE_GALLON:		strText += "Gallon";		break;
					case WEIGHTTYPE_PINT:		strText += "Pint";			break;
					case WEIGHTTYPE_FLOUNCE:		strText += "Fluid Ounce";	break;
					case WEIGHTTYPE_LITRE:		strText += "Litre";			break;
					case WEIGHTTYPE_CLITRE:		strText += "Centilitre";		break;
					case WEIGHTTYPE_MLITRE:		strText += "Millilitre";		break;
					case WEIGHTTYPE_KILO:		strText += "Kilogram";		break;
					case WEIGHTTYPE_GRAM:		strText += "Gram";			break;
					default:					strText += "Unknown";		break;
					}

					csv.AppendString( strText );
				}
				else
					csv.AppendString( m_LabelStockRecord.GetStockUnitName() );
			}
		}
		
		else if ( nField == CustomStockList::RetailUnitName.Type )
		{
			double dWeight = m_LabelStockRecord.GetSUWeight(TRUE);
			if ( CPriceHelpers::CompareDoubles ( dWeight, 0.0, 3 ) != 0 )
			{
				CString strWeight = "";
				switch ( m_LabelStockRecord.GetSaleWeightType() )
				{
				case WEIGHTTYPE_GALLON:	strWeight = "Gallon";		break;
				case WEIGHTTYPE_PINT:	strWeight = "Pint";			break;
				case WEIGHTTYPE_FLOUNCE:	strWeight = "Fluid Ounce";	break;
				case WEIGHTTYPE_LITRE:	strWeight = "Litre";		break;
				case WEIGHTTYPE_CLITRE:	strWeight = "Centilitre";	break;
				case WEIGHTTYPE_MLITRE:	strWeight = "Millilitre";	break;
				case WEIGHTTYPE_KILO:	strWeight = "Kilogram";		break;
				case WEIGHTTYPE_GRAM:	strWeight = "Gram";			break;
				default:				strWeight = "Unknown";		break;
				}
				
				csv.AppendString( strWeight );
			}
			else
				csv.AppendString( "" );
		}
		
		else if ( nField == CustomStockList::Wastage.Type )		
			csv.AppendVal( m_LabelStockRecord.GetWastePercent(), 2 );
		
		else if ( nField == CustomStockList::LabelsFlag.Type )
			csv.AppendYesOrBlank( m_LabelStockRecord.GetAutoLabelFlag() );

		else if ( nField == CustomStockList::OrderFlag.Type )
			csv.AppendYesOrBlank( m_LabelStockRecord.GetDisableAutoOrderFlag() == FALSE );

		else if ( nField == CustomStockList::PluRangeFlag.Type )
			csv.AppendYesOrBlank( m_LabelStockRecord.GetDisablePluRangeFlag() == FALSE );

		else if ( nField == CustomStockList::SupplierName1.Type )
			AppendSupplierName( csv, 0 );

		else if ( nField == CustomStockList::SupplierName2.Type )
			AppendSupplierName( csv, 1 );

		else if ( nField == CustomStockList::SupplierName3.Type )
			AppendSupplierName( csv, 2 );

		else if ( nField == CustomStockList::SupplierName4.Type )
			AppendSupplierName( csv, 3 );

		else if ( nField == CustomStockList::SupplierName5.Type )
			AppendSupplierName( csv, 4 );

		else if ( nField == CustomStockList::SupplierRef1.Type )
			AppendSupplierRef( csv, 0 );

		else if ( nField == CustomStockList::SupplierRef2.Type )
			AppendSupplierRef( csv, 1 );

		else if ( nField == CustomStockList::SupplierRef3.Type )
			AppendSupplierRef( csv, 2 );

		else if ( nField == CustomStockList::SupplierRef4.Type )
			AppendSupplierRef( csv, 3 );

		else if ( nField == CustomStockList::SupplierRef5.Type )
			AppendSupplierRef( csv, 4 );

		else if ( nField == CustomStockList::WrapperCode1.Type )
			AppendWrapperCode( csv, 0 );

		else if ( nField == CustomStockList::WrapperCode2.Type )
			AppendWrapperCode( csv, 1 );

		else if ( nField == CustomStockList::WrapperCode3.Type )
			AppendWrapperCode( csv, 2 );

		else if ( nField == CustomStockList::WrapperCode4.Type )
			AppendWrapperCode( csv, 3 );

		else if ( nField == CustomStockList::WrapperCode5.Type )
			AppendWrapperCode( csv, 4 );

		else if ( nField == CustomStockList::DelUnitName1.Type )
			AppendDelUnitName( csv, 0 );

		else if ( nField == CustomStockList::DelUnitName2.Type )
			AppendDelUnitName( csv, 1 );

		else if ( nField == CustomStockList::DelUnitName3.Type )
			AppendDelUnitName( csv, 2 );

		else if ( nField == CustomStockList::DelUnitName4.Type )
			AppendDelUnitName( csv, 3 );

		else if ( nField == CustomStockList::DelUnitName5.Type )
			AppendDelUnitName( csv, 4 );

		else if ( nField == CustomStockList::DelUnitDetail1.Type )
			AppendDelUnitDetail( csv, 0 );

		else if ( nField == CustomStockList::DelUnitDetail2.Type )
			AppendDelUnitDetail( csv, 1 );

		else if ( nField == CustomStockList::DelUnitDetail3.Type )
			AppendDelUnitDetail( csv, 2 );

		else if ( nField == CustomStockList::DelUnitDetail4.Type )
			AppendDelUnitDetail( csv, 3 );

		else if ( nField == CustomStockList::DelUnitDetail5.Type )
			AppendDelUnitDetail( csv, 4 );

		else if ( nField == CustomStockList::DelUnitYield1.Type )
			AppendDelUnitYield( csv, 0 );

		else if ( nField == CustomStockList::DelUnitYield2.Type )
			AppendDelUnitYield( csv, 1 );

		else if ( nField == CustomStockList::DelUnitYield3.Type )
			AppendDelUnitYield( csv, 2 );

		else if ( nField == CustomStockList::DelUnitYield4.Type )
			AppendDelUnitYield( csv, 3 );

		else if ( nField == CustomStockList::DelUnitYield5.Type )
			AppendDelUnitYield( csv, 4 );

		else if ( nField == CustomStockList::DelUnitCost1.Type )
			AppendDelUnitCost( csv, 0 );

		else if ( nField == CustomStockList::DelUnitCost2.Type )
			AppendDelUnitCost( csv, 1 );

		else if ( nField == CustomStockList::DelUnitCost3.Type )
			AppendDelUnitCost( csv, 2 );

		else if ( nField == CustomStockList::DelUnitCost4.Type )
			AppendDelUnitCost( csv, 3 );

		else if ( nField == CustomStockList::DelUnitCost5.Type )
			AppendDelUnitCost( csv, 4 );

		else if ( nField == CustomStockList::StockUnitCost1.Type )
			AppendStockUnitCost( csv, 0 );

		else if ( nField == CustomStockList::StockUnitCost2.Type )
			AppendStockUnitCost( csv, 1 );

		else if ( nField == CustomStockList::StockUnitCost3.Type )
			AppendStockUnitCost( csv, 2 );

		else if ( nField == CustomStockList::StockUnitCost4.Type )
			AppendStockUnitCost( csv, 3 );

		else if ( nField == CustomStockList::StockUnitCost5.Type )
			AppendStockUnitCost( csv, 4 );
		
		else if ( nField == CustomStockList::Apparent.Type )		
			csv.AppendStockQty( dApparent, nSubUnits );

		else if ( nField == CustomStockList::DelUnitRetail1.Type )
			AppendDelUnitRetail( csv, 0 );

		else if ( nField == CustomStockList::DelUnitRetail2.Type )
			AppendDelUnitRetail( csv, 1 );

		else if ( nField == CustomStockList::DelUnitRetail3.Type )
			AppendDelUnitRetail( csv, 2 );

		else if ( nField == CustomStockList::DelUnitRetail4.Type )
			AppendDelUnitRetail( csv, 3 );

		else if ( nField == CustomStockList::DelUnitRetail5.Type )
			AppendDelUnitRetail( csv, 4 );
	}

	return TRUE;
}

/**********************************************************************/

void CStockTreeReportNewCustomList::AppendSupplierName( CStockReportLine& csv, int nStockSuppIdx )
{
	if ( nStockSuppIdx >= m_LabelStockRecord.GetSupplierCount() )
		csv.AppendString( "" );
	else
	{
		int nSuppIdx;
		int nSuppNo = m_LabelStockRecord.GetSuppNo( nStockSuppIdx );

		if ( DataManager.Supplier.FindSupplierByNumber( nSuppNo, nSuppIdx ) == TRUE )
			csv.AppendString( DataManager.Supplier.GetName( nSuppIdx ) );
		else
			csv.AppendString( "Unknown" );
	}
}

/**********************************************************************/

void CStockTreeReportNewCustomList::AppendSupplierRef( CStockReportLine& csv, int nStockSuppIdx )
{
	if ( nStockSuppIdx >= m_LabelStockRecord.GetSupplierCount() )
		csv.AppendString( "" );
	else
		csv.AppendString( m_LabelStockRecord.GetSuppRefExternal( nStockSuppIdx, DealerFlags.GetSuppRefFlag() ) );
}

/**********************************************************************/

void CStockTreeReportNewCustomList::AppendWrapperCode( CStockReportLine& csv, int nStockSuppIdx )
{
	if ( nStockSuppIdx >= m_LabelStockRecord.GetSupplierCount() )
		csv.AppendString( "" );
	else
		csv.AppendString( m_LabelStockRecord.GetWrapper( nStockSuppIdx ) );
}

/**********************************************************************/

void CStockTreeReportNewCustomList::AppendDelUnitName( CStockReportLine& csv, int nStockSuppIdx )
{
	if ( nStockSuppIdx >= m_LabelStockRecord.GetSupplierCount() )
		csv.AppendString( "" );
	else
		csv.AppendString( m_LabelStockRecord.GetCase( nStockSuppIdx ) );
}

/**********************************************************************/

void CStockTreeReportNewCustomList::AppendDelUnitDetail( CStockReportLine& csv, int nStockSuppIdx )
{
	if ( nStockSuppIdx >= m_LabelStockRecord.GetSupplierCount() )
		csv.AppendString( "" );
	else
	{
		CString strDetail;
		strDetail.Format( "%d x %.3f",
			m_LabelStockRecord.GetDUItems( nStockSuppIdx ),
			m_LabelStockRecord.GetDUItemSize( nStockSuppIdx ) );

		csv.AppendString( strDetail );
	}
}

/**********************************************************************/

void CStockTreeReportNewCustomList::AppendDelUnitYield( CStockReportLine& csv, int nStockSuppIdx )
{
	if ( nStockSuppIdx >= m_LabelStockRecord.GetSupplierCount() )
		csv.AppendString( "" );
	else
		csv.AppendVal( m_LabelStockRecord.GetYield( nStockSuppIdx ), 3 );
}

/**********************************************************************/

void CStockTreeReportNewCustomList::AppendDelUnitCost( CStockReportLine& csv, int nStockSuppIdx )
{
	if ( nStockSuppIdx >= m_LabelStockRecord.GetSupplierCount() )
		csv.AppendString( "" );
	else
		csv.AppendVal( m_LabelStockRecord.GetCost( nStockSuppIdx ) );
}

/**********************************************************************/

void CStockTreeReportNewCustomList::AppendDelUnitRetail( CStockReportLine& csv, int nStockSuppIdx )
{
	if ( nStockSuppIdx >= m_LabelStockRecord.GetSupplierCount() )
		csv.AppendString( "" );
	else
		csv.AppendVal( m_LabelStockRecord.GetRetail( nStockSuppIdx ) );
}

/**********************************************************************/

void CStockTreeReportNewCustomList::AppendStockUnitCost( CStockReportLine& csv, int nStockSuppIdx )
{
	if ( nStockSuppIdx >= m_LabelStockRecord.GetSupplierCount() )
		csv.AppendString( "" );
	else
		csv.AppendVal( m_LabelStockRecord.GetUnitCost( nStockSuppIdx ) );
}

/**********************************************************************/

void CStockTreeReportNewCustomList::AppendCustomFieldValueName( CStockReportLine& csv, int nFieldNo )
{
	if ( ( nFieldNo < 1 ) || ( nFieldNo > MAX_ITEM_CUSTOM_FIELDS ) )
		csv.AppendString( "" );
	else
	{
		int nValueIdx;
		int nValueNo = m_LabelStockRecord.GetCustomFieldValueNo( nFieldNo );

		CString strName = "N/A";

		if ( 0 != nValueNo )
			if ( DataManager.CustomFieldValue[nFieldNo].FindValueByNumber( nValueNo, nValueIdx ) == TRUE )
				strName = DataManager.CustomFieldValue[nFieldNo].GetName( nValueIdx );
		
		csv.AppendString( strName );
	}
}

/**********************************************************************/

void CStockTreeReportNewCustomList::GetCSVReportName( int n, CString& strName )
{
	strName = "";

	if ( n >= 1 && n <= 5 )
	{	
		CSSFile file;
		CFilenameUpdater FnUp( SysFiles::StockCustomDef, 0, n );
		if ( file.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
		{
			CString strBuffer;
			file.ReadString( strBuffer );
			file.Close();
		
			CCSV csv( strBuffer );
			for ( int nPos = 0; nPos < csv.GetSize(); nPos++ )
			{
				if ( csv.GetInt(nPos) == 9999 )
				{
					strName = csv.GetString( nPos + 1 );
					break;
				}
			}
		}
	}

	::TrimSpaces( strName, FALSE );

	if ( strName == "" )
		strName.Format( "Custom List %d", n );
}

/**********************************************************************/
