/**********************************************************************/
 
/**********************************************************************/
#include "BarcodePluNoTable.h"
//#include "DataManager.h"
//#include "DealerFlags.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "PriceHelpers.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "LabelListCreator.h"
/**********************************************************************/

CLabelListCreator::CLabelListCreator( CLabelListCSVArray& LabelList ) : m_LabelList ( LabelList )
{
	if ( m_LabelList.IsProductLabelList() == FALSE )
	{
		m_bPrintStockCode = EcrmanOptions.GetLabelsShelfShowStockCodeFlag();
		m_bPrintPluNumber = EcrmanOptions.GetLabelsShelfShowPluFlag();
		m_bPrintSuppRef = EcrmanOptions.GetLabelsShelfShowSuppRefFlag();

		m_bBarcode = FALSE;
		m_nTaxType1 = EcrmanOptions.GetLabelsShelfTaxType1();
		m_nTaxType2 = EcrmanOptions.GetLabelsShelfTaxType2();
		m_bHideZeros = EcrmanOptions.GetLabelsShelfTrimPluZerosFlag();
		m_nPriceLevel1 = EcrmanOptions.GetLabelsShelfPriceLevel1();
		m_nPriceLevel2 = EcrmanOptions.GetLabelsShelfPriceLevel2();
		m_strCurrency1 = EcrmanOptions.GetLabelsShelfPriceSymbol1();
		m_strCurrency2 = EcrmanOptions.GetLabelsShelfPriceSymbol2();
	}
	else
	{
		m_bPrintStockCode = EcrmanOptions.GetLabelsProductShowStockCodeFlag();
		m_bPrintPluNumber = EcrmanOptions.GetLabelsProductShowPluFlag();
		m_bPrintSuppRef = EcrmanOptions.GetLabelsProductShowSuppRefFlag();

		m_bBarcode = TRUE;
		m_nTaxType1 = EcrmanOptions.GetLabelsProductTaxType1();
		m_nTaxType2 = EcrmanOptions.GetLabelsProductTaxType2();
		m_bHideZeros = EcrmanOptions.GetLabelsProductTrimPluZerosFlag();
		m_nPriceLevel1 = EcrmanOptions.GetLabelsProductPriceLevel1();
		m_nPriceLevel2 = EcrmanOptions.GetLabelsProductPriceLevel2();
		m_strCurrency1 = EcrmanOptions.GetLabelsProductPriceSymbol1();
		m_strCurrency2 = EcrmanOptions.GetLabelsProductPriceSymbol2();
	}

#ifndef STOCKMAN_SYSTEM
	m_bPrintPluNumber = TRUE;
	m_bPrintStockCode = FALSE;
#endif
}

/**********************************************************************/

void CLabelListCreator::GetPriceString( CPluCSVRecord& PluRecord, int nMod, CLabelPriceInfo& info, int nMask )
{
	if ( ( nMask < 0 ) || ( nMask > 3 ) )
		nMask = 3;

	if ( 0 == m_nPriceLevel2 )
		nMask &= 1;

	info.m_dPrice1 = 0.0;
	info.m_dPrice2 = 0.0;

	info.m_strPrice1 = "";
	info.m_strPrice2 = "";
	info.m_strPrices = "";

	CTaxRateInfo TaxRateInfo;
	DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetModifierTaxBandIntForReport(nMod), TaxRateInfo );

	if ( ( nMask & 1 ) == 1 )
	{		
		if ( m_nPriceLevel1 >= 1 && m_nPriceLevel1 <= 4 )
			info.m_dPrice1 = PluRecord.GetModifierPrice( nMod, m_nPriceLevel1 - 1 );
		
		info.m_dPrice1 = m_LabelList.CalculateLabelPrice( info.m_dPrice1, TaxRateInfo.m_dTaxRate, m_nTaxType1 );

		info.m_strPrice1.Format ( "%s%.*f", 
			(const char*) m_strCurrency1,
			SysInfo.GetDPValue(), 
			info.m_dPrice1 );
	}

	if ( ( nMask & 2 ) == 2 )
	{		
		if ( m_nPriceLevel2 >= 1 && m_nPriceLevel2 <= 4 )
			info.m_dPrice2 = PluRecord.GetModifierPrice( nMod, m_nPriceLevel2 - 1 );
		
		info.m_dPrice2 = m_LabelList.CalculateLabelPrice( info.m_dPrice2, TaxRateInfo.m_dTaxRate, m_nTaxType2 );

		info.m_strPrice2.Format ( "%s%.*f", 
			(const char*) m_strCurrency2,
			SysInfo.GetDPValue(), 
			info.m_dPrice2 );
	}

	switch( nMask )
	{
	case 1:
		info.m_strPrices = info.m_strPrice1;
		break;

	case 2:
		info.m_strPrices = info.m_strPrice2;
		break;

	case 3:
		info.m_strPrices = info.m_strPrice1;
		info.m_strPrices += "/";
		info.m_strPrices += info.m_strPrice2;
		break;
	}
}

/**********************************************************************/

int CLabelListCreator::CreateLabelList()
{
	CString strDataPath = "Labels.dat";
	GetDataProgramPath( strDataPath );
	
	CSSFile fileLabels;
	if ( fileLabels.Open ( strDataPath, "wb" ) == FALSE )
		return 1;

	int nCount = m_LabelList.GetSize();
	bool bGotRecord = FALSE;

	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, nCount ); 

		CLabelListCSVRecord LabelListRecord;
		m_LabelList.GetAt ( nIndex, LabelListRecord );
		
		//WE CAN ONLY PRINT LABELS FOR EXISTING PLU RECORDS
		int nPluIdx;
		__int64 nPluNo = LabelListRecord.GetPluNoInt();
		if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == FALSE )
			continue;

		//FIND OUT IF THE LABEL IS A VALID BARCODE
		CString strBarcodeNo = LabelListRecord.GetBarcodeNoString();
		
		::TrimSpaces ( strBarcodeNo, TRUE );
		if ( strBarcodeNo.GetLength() > 8 && strBarcodeNo.GetLength() < 13 )
			::AddLeading ( strBarcodeNo, 13, '0' );
		if ( strBarcodeNo.GetLength() < 8 )
			::AddLeading ( strBarcodeNo, 8, '0' );

		//BARCODE LABELS MUST HAVE A VALID BARCODE
		bool bValidCheckDigit = CBarcodePluNoTable::IsValidEan ( strBarcodeNo );
		if ( ( bValidCheckDigit == FALSE ) && ( m_bBarcode == TRUE ) )
			continue;
						
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord ); 

		int nMod = 0;
		if ( LabelListRecord.GetDirectPluFlag() == FALSE )
			nMod = LabelListRecord.GetModifier();

		if ( EcrmanOptions.AllowBarcodeModifierLinks() == FALSE )
			nMod = 0;

		//LABEL TEXT
		CString strCSVLabelText, strBaseLabelText;
		if ( DealerFlags.GetEcrTextOnLabelsFlag() == FALSE )
		{
			strCSVLabelText = PluRecord.GetModifierReportText(nMod);
			strBaseLabelText = PluRecord.GetReportText();
		}
		else
		{
			strCSVLabelText = PluRecord.GetModifierEposText(nMod);
			strBaseLabelText = PluRecord.GetEposText();
		}
		
		//PRICES
		CLabelPriceInfo info1, info2;
		GetPriceString ( PluRecord, nMod, info1, 1 );
		GetPriceString ( PluRecord, nMod, info2, 2 );
		CString strCSVPrice1 = info1.m_strPrice1;
		CString strCSVPrice2 = info2.m_strPrice2;
		double dFirstPrice = info1.m_dPrice1;

		//PLU NUMBER
		CString strCSVPluNo = "";
		if ( m_bPrintPluNumber )
		{
			strCSVPluNo = PluRecord.GetPluNoString();
			if ( m_bHideZeros == TRUE ) ::TrimSpaces( strCSVPluNo, TRUE );
		}
		
		//BARCODE NO
		CString strCSVBarcodeNo = "";
		if ( bValidCheckDigit == TRUE )
			strCSVBarcodeNo = strBarcodeNo;

		//PLU UNIT
		CString strCSVPluUnit = "";
		double dRatio = CPriceHelpers::GetUnitPriceRatio ( PluRecord );
		double dUnitPrice = ( dRatio == 0.0 ) ? 0.0 : dFirstPrice / dRatio;
		CString strPluUnit = PluRecord.GetUnitPriceText();
		strCSVPluUnit = strPluUnit;

		bool bShowInPence = TRUE;

		if ( dUnitPrice >= 1.00 || m_strCurrency1 == "" || m_strCurrency1.GetAt(0) != '£' )
			bShowInPence = FALSE;

		if ( bShowInPence == TRUE )
		{
			if ( dRatio != 0.0 )
				strCSVPluUnit.Format ( "%.1fp %s", 
					dUnitPrice * 100.0, 
					(const char*) strPluUnit );
		}
		else
		{
			if ( dRatio != 0.0 )
				strCSVPluUnit.Format ( "%s%.2f %s", 
					(const char*) m_strCurrency1,
					dUnitPrice, 
					(const char*) strPluUnit );
		}
				
		//STOCK CODE AND SUPPLIER REF
		CString strCSVStockCode = "";
		
		if ( m_bPrintStockCode == TRUE || m_bPrintSuppRef == TRUE )
		{
			CString strStockCode = "";
			DataManager.Plu.GetPluStockCode( PluRecord.GetPluNoInt(), strStockCode, FALSE );
			
			CString strSuppRef = "";
			
			if ( m_bPrintSuppRef == TRUE )
			{
#ifdef STOCKMAN_SYSTEM
				{
					if ( strStockCode != "" )
					{
						int nStockIdx;
						if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == TRUE )
						{
							CStockCSVRecord StockRecord;
							DataManager.Stock.GetAt( nStockIdx, StockRecord );

							if ( StockRecord.GetSupplierCount() > 0 )
								strSuppRef = StockRecord.GetSuppRefExternal(0, DealerFlags.GetSuppRefFlag() );
						}
					}
				}
#else
				strSuppRef = PluRecord.GetSupplierRef();
				::TrimSpaces( strSuppRef, FALSE );
#endif
			}

			if ( m_bPrintStockCode == FALSE )
				strStockCode = "";
			else if ( m_bHideZeros == TRUE )
				::TrimSpaces( strStockCode, TRUE );

			if ( strStockCode != "" && strSuppRef != "" )
				strStockCode += " ";

			strCSVStockCode = strStockCode + strSuppRef;
		}	

		CCSV csv;
		csv.Add( strCSVLabelText );
		csv.Add( strCSVPrice1 );
		csv.Add( strCSVPluNo );
		csv.Add( strCSVBarcodeNo );
		csv.Add( strCSVPluUnit );
		csv.Add( "" );	//SKIP IMAGE FILENAME
		csv.Add( strCSVStockCode );
		csv.Add( strCSVPrice2 );
		
		for ( int nIdx = 1; nIdx <= LabelListRecord.GetQtyToPrint(); nIdx++ )
			fileLabels.WriteLine ( csv.GetLine() );

		bGotRecord = TRUE;
	}
	fileLabels.Close();	
	return ( bGotRecord == TRUE ) ? 0 : 2;
}

/**********************************************************************/

