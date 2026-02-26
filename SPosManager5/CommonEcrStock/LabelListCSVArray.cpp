/**********************************************************************/
#include <math.h>
/**********************************************************************/
#include "BarcodeCSVArray.h"
#include "BarcodePluNoTable.h"
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "NodeTypes.h"
#include "PriceHelpers.h"
#include "PriceSetItemManager.h"
#include "ReportHelpers.h"
#include "..\SPosStockManager5\SiteCSVArray.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "LabelListCSVArray.h"
/**********************************************************************/
static const int ALL=0;
static const int CHANGES=1;
/**********************************************************************/

CLabelListCSVRecord::CLabelListCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CLabelListCSVRecord::ClearRecord()
{
	m_nBarcodeNo = 0;
	m_nPluNo = 0;
	m_nModifier = 0;
	m_nQtyToPrint = 0;
	m_strRepText = "";
	m_strLabelText = "";
	m_dFirstPrice = 0.0;
	m_dSecondPrice = 0.0;
	m_strStockCode = "";
	m_dUnitPriceFactor = 0.0;
	m_bDirectPlu = FALSE;
}

/**********************************************************************/

void CLabelListCSVRecord::ConvertToCSV( CCSV& csv )
{
	csv.Add ( m_nBarcodeNo );
	csv.Add ( m_nPluNo );
	csv.Add ( LABELDB_VERSION );
	csv.Add ( m_nModifier );
	csv.Add ( m_nQtyToPrint );
	csv.Add ( m_strRepText );
	csv.Add ( m_dFirstPrice, 3 );
	csv.Add ( m_dSecondPrice, 3 );
	csv.Add ( m_strStockCode );						
	csv.Add ( m_strLabelText );
	csv.Add ( m_dUnitPriceFactor, 5 );
	csv.Add ( m_bDirectPlu );
}

/**********************************************************************/

void CLabelListCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetBarcodeNo ( csv.GetInt64(0) );
	SetPluNo ( csv.GetInt64(1) );

	int nVersion = csv.GetInt ( 2 );

	switch ( nVersion )
	{
	case 1:	V1To2ConvertFromCSV ( csv, 1 );	break;
	case 2:	V1To2ConvertFromCSV ( csv, 2 );	break;
	}
}

/**********************************************************************/

void CLabelListCSVRecord::V1To2ConvertFromCSV ( CCSV& csv, int nVer )
{
	int nOffset = 3;
	if ( nVer >= 2 ) SetModifier( csv.GetInt(nOffset++) );
	SetQtyToPrint ( csv.GetInt(nOffset++) );
	SetRepText ( csv.GetString(nOffset++) );
	SetFirstPrice ( csv.GetDouble(nOffset++) );
	SetSecondPrice ( csv.GetDouble(nOffset++) );
	SetStockCode( csv.GetString(nOffset++) );
	SetLabelText ( csv.GetString(nOffset++) );
	SetUnitPriceFactor ( csv.GetDouble(nOffset++) );
	SetDirectPluFlag( csv.GetBool(nOffset++) );
}

/**********************************************************************/
//	Barcode Number
/**********************************************************************/

void CLabelListCSVRecord::SetBarcodeNo ( __int64 nBarcodeNo )
{
	if ( ( nBarcodeNo >= 1 ) && ( nBarcodeNo <= Super.MaxBarcodeInt() ) )
		m_nBarcodeNo = nBarcodeNo;
}

const char* CLabelListCSVRecord::GetBarcodeNoString()
{
	::FormatBarcodeNo( m_nBarcodeNo, m_strBarcodeNo );
	return m_strBarcodeNo;
}

/**********************************************************************/
//	Plu Number
/**********************************************************************/

void CLabelListCSVRecord::SetPluNo ( __int64 nPluNo )
{
	if ( ( nPluNo >= 1 ) && ( nPluNo <= Super.MaxPluInt() ) )
		m_nPluNo = nPluNo;
}

const char* CLabelListCSVRecord::GetPluNoString()
{
	::FormatPluNo( m_nPluNo, m_strPluNo );	
	return m_strPluNo;
}

/**********************************************************************/

void CLabelListCSVRecord::SetModifier( int nMod )
{
	if ( ( nMod >= 0 ) && ( nMod <= 9 ) )
		m_nModifier = nMod;
}

/**********************************************************************/

CLabelListCSVArray::CLabelListCSVArray( bool bProductLabels ) : CSharedCSVArray( ',', '"', FALSE, TRUE, TRUE )
{
	m_bProductLabels = bProductLabels;
}

/**********************************************************************/

bool CLabelListCSVArray::FindBarcodeByNumber ( __int64 nBarcodeNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;

		__int64 nArrayBarcodeNo = GetBarcodeNoInt( nIndex);
		
		if ( nArrayBarcodeNo < nBarcodeNo )
			nStart = nIndex + 1;

		else if ( nArrayBarcodeNo > nBarcodeNo )
			nEnd = nIndex - 1;

		else
			return TRUE;
	}
		
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

bool CLabelListCSVArray::InsertRecord ( CLabelListCSVRecord& NewRecord, int& nIndex )
{
	__int64 nBarcodeNo = NewRecord.GetBarcodeNoInt();

	if ( FindBarcodeByNumber ( nBarcodeNo, nIndex ) == FALSE )
	{
		InsertAt ( nIndex, NewRecord );
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

const char* CLabelListCSVArray::GetBarcodeNoString ( int nIndex )
{
	::FormatBarcodeNo( GetBarcodeNoInt( nIndex ), m_strBarcodeNo );
	return m_strBarcodeNo;
}

/**********************************************************************/

__int64 CLabelListCSVArray::GetBarcodeNoInt ( int nIndex )
{
	CString strTemp;

	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		return csv.GetInt64 ( 0 );
	}
	return 0;
}

/**********************************************************************/

void CLabelListCSVArray::RemoveAll()
{
	RemoveAt ( 0, GetSize() );
}

/**********************************************************************/

void CLabelListCSVArray::GetPriceInfo( CPluCSVRecord& PluRecord, int nMod, double& dFirstPrice, double& dSecondPrice )
{
	dFirstPrice = 0.0;		
	dSecondPrice = 0.0;
	
	int nPriceLevel1, nPriceLevel2, nTaxType1, nTaxType2;

	if ( TRUE == m_bProductLabels )
	{
		nPriceLevel1 = EcrmanOptions.GetLabelsProductPriceLevel1();
		nPriceLevel2 = EcrmanOptions.GetLabelsProductPriceLevel2();
		nTaxType1 = EcrmanOptions.GetLabelsProductTaxType1();
		nTaxType2 = EcrmanOptions.GetLabelsProductTaxType2();
	}
	else
	{
		nPriceLevel1 = EcrmanOptions.GetLabelsShelfPriceLevel1();
		nPriceLevel2 = EcrmanOptions.GetLabelsShelfPriceLevel2();
		nTaxType1 = EcrmanOptions.GetLabelsShelfTaxType1();
		nTaxType2 = EcrmanOptions.GetLabelsShelfTaxType2();
	}

	CTaxRateInfo TaxRateInfo;
	DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetModifierTaxBandIntForReport(nMod), TaxRateInfo );

	if ( nPriceLevel2 == 0 )
	{		
		if ( nPriceLevel1 >= 1 && nPriceLevel1 <= 4 )
			dFirstPrice = PluRecord.GetModifierPrice( nMod, nPriceLevel1 - 1 );
		
		dFirstPrice = CalculateLabelPrice( dFirstPrice, TaxRateInfo.m_dTaxRate, nTaxType1 );
	}
	else
	{				
		if ( nPriceLevel1 >= 1 && nPriceLevel1 <= 4 )
			dFirstPrice = PluRecord.GetModifierPrice( nMod, nPriceLevel1 - 1 );
		
		if ( nPriceLevel2 >= 1 && nPriceLevel2 <= 4 )
			dSecondPrice = PluRecord.GetModifierPrice( nMod, nPriceLevel2 - 1 );
		
		dFirstPrice = CalculateLabelPrice( dFirstPrice, TaxRateInfo.m_dTaxRate, nTaxType1 );
		dSecondPrice = CalculateLabelPrice( dSecondPrice, TaxRateInfo.m_dTaxRate, nTaxType2 );
	}
}

/**********************************************************************/

double CLabelListCSVArray::CalculateLabelPrice( double dRetailPrice, double dTaxRate, int nTaxType )
{
	double dLabelPrice = 0.0;

	switch ( nTaxType )
	{
	case 1:	
		dLabelPrice = ReportHelpers.CalcNonTax ( dRetailPrice, dTaxRate );
		break;

	case 2:
		dLabelPrice = dRetailPrice - ReportHelpers.CalcNonTax ( dRetailPrice, dTaxRate );
		break;

	case 0:
	default:
		dLabelPrice = dRetailPrice;
		break;
	}	
		
	return dLabelPrice;
}

/**********************************************************************/

void CLabelListCSVArray::CreateSingleItemList( int nPluIdx )
{
	DataManager.Barcode.CreatePluBarcodeList();

	RemoveAll();
	
	CString strPrice;
	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt ( nPluIdx, PluRecord ); 
	__int64 nPluNo = PluRecord.GetPluNoInt();

	//FIRST PASS FOR DIRECT USE OF PLU NUMBER AS BARCODE
	if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == FALSE )
	{
		bool bCanDoLabel = TRUE;
		if ( FALSE == m_bProductLabels )
		{
			if ( PluRecord.GetPrintShelfFlag() == FALSE )
				bCanDoLabel = FALSE;
		}
		else
		{
			if ( PluRecord.GetPrintProductFlag() == FALSE )
				bCanDoLabel = FALSE;
			else if ( CBarcodePluNoTable::IsValidEan( nPluNo ) == FALSE )
				bCanDoLabel = FALSE;
		}
	
		if ( TRUE == bCanDoLabel )
		{
			CLabelListCSVRecord LabelListRecord;
			LabelListRecord.SetBarcodeNo( nPluNo );
			LabelListRecord.SetPluNo ( nPluNo );
			LabelListRecord.SetModifier( 0 );
			LabelListRecord.SetQtyToPrint ( 1 );
			LabelListRecord.SetDirectPluFlag( TRUE );

			int nLabelIdx;
			InsertRecord ( LabelListRecord, nLabelIdx );
		}
	}

	//SECOND PASS FOR LINKS TO BARCODE DATABASE
	CArray<int,int> arrayBarcodeIdx;
	DataManager.Barcode.GetBarcodeIdxArray( nPluNo, arrayBarcodeIdx );

	for ( int nPos = 0; nPos < arrayBarcodeIdx.GetSize(); nPos++ )
	{
		int nBarcodeIdx = arrayBarcodeIdx.GetAt( nPos );
		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );

		__int64 nBarcodeNo = BarcodeRecord.GetBarcodeNoInt();

		bool bCanDoLabel = TRUE;
		if ( FALSE == m_bProductLabels )
		{
			if ( BarcodeRecord.GetPrintShelfFlag() == FALSE )
				bCanDoLabel = FALSE;
		}
		else
		{
			if ( BarcodeRecord.GetPrintProductFlag() == FALSE )
				bCanDoLabel = FALSE;
			else if ( CBarcodePluNoTable::IsValidEan( nBarcodeNo ) == FALSE )
				bCanDoLabel = FALSE;
		}

		if ( TRUE == bCanDoLabel )
		{
			CLabelListCSVRecord LabelListRecord;
			LabelListRecord.SetBarcodeNo( nBarcodeNo );
			LabelListRecord.SetPluNo ( PluRecord.GetPluNoInt() );
			LabelListRecord.SetModifier( BarcodeRecord.GetModifier() );
			LabelListRecord.SetQtyToPrint ( 1 );
			LabelListRecord.SetDirectPluFlag( FALSE );

			int nLabelIdx;
			InsertRecord ( LabelListRecord, nLabelIdx );
		}
	}
}

/**********************************************************************/

void CLabelListCSVArray::CreateAdhocList( CPluFilterArray& FilterArray, __int64 nPluFrom, __int64 nPluTo, __int64 nBarcodeFrom, __int64 nBarcodeTo, int nDepartmentFilter, int nType, const char* szDate, int nItemLabels )
{
	StatusProgress.Lock( TRUE, "Creating label list" );
	CreateAdhocListInternal ( FilterArray, nPluFrom, nPluTo, nBarcodeFrom, nBarcodeTo, nDepartmentFilter, nType, szDate, nItemLabels );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CLabelListCSVArray::CreateAdhocListInternal( CPluFilterArray& FilterArray, __int64 nPluFrom, __int64 nPluTo, __int64 nBarcodeFrom, __int64 nBarcodeTo, int nDepartmentFilter, int nType, const char* szDate, int nItemLabels )
{
	DataManager.Barcode.CreatePluBarcodeList();

	RemoveAll();

	int nStartIdx, nRange;
	FilterArray.GetArrayIdxRange( nPluFrom, nPluTo, nStartIdx, nRange );

	CString strDate = szDate;

	CDepartmentSetCSVRecord DepartmentSet;
	DepartmentSet.CreateDepartmentSet ( nDepartmentFilter );

	//FIRST PASS FOR DIRECT PLU LABELS, SO THEY GET PRIORITY
	for ( int nOffset = 0; nOffset < nRange; nOffset++ )
	{
		StatusProgress.SetPos ( nOffset, nRange ); 

		CString strPrice;
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( FilterArray.GetPluItemIndexAt ( nStartIdx + nOffset ), PluRecord ); 

		__int64 nPluNo = PluRecord.GetPluNoInt();

		if ( ( nPluNo < nPluFrom ) || ( nPluNo > nPluTo ) )
			continue;

		if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == TRUE )
			continue;

		if ( DepartmentSet.GetDepartmentStatus ( PluRecord.GetBaseDeptNo() ) == FALSE )
			continue;

		if ( FALSE == m_bProductLabels )
		{
			if ( PluRecord.GetPrintShelfFlag() == FALSE )
				continue;
		}
		else
		{
			if ( PluRecord.GetPrintProductFlag() == FALSE )
				continue;

			if ( CBarcodePluNoTable::IsValidEan( nPluNo ) == FALSE )
				continue;
		}
	
		CLabelListCSVRecord LabelListRecord;
		LabelListRecord.SetBarcodeNo( nPluNo );
		LabelListRecord.SetPluNo ( nPluNo );
		LabelListRecord.SetModifier( 0 );
		LabelListRecord.SetQtyToPrint ( nItemLabels );
		LabelListRecord.SetDirectPluFlag( TRUE );

		int nLabelIdx;
		InsertRecord ( LabelListRecord, nLabelIdx );
	}

	//SECOND PASS FOR LINKS TO BARCODE DATABASE
	for ( int nOffset = 0; nOffset < nRange; nOffset++ )
	{
		StatusProgress.SetPos ( nOffset, nRange ); 

		CString strPrice;
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( FilterArray.GetPluItemIndexAt ( nStartIdx + nOffset ), PluRecord ); 

		__int64 nPluNo = PluRecord.GetPluNoInt();

		if ( ( nPluNo < nPluFrom ) || ( nPluNo > nPluTo ) )
			continue;

		CArray<int,int> arrayBarcodeIdx;
		DataManager.Barcode.GetBarcodeIdxArray( nPluNo, arrayBarcodeIdx );

		for ( int nPos = 0; nPos < arrayBarcodeIdx.GetSize(); nPos++ )
		{
			int nBarcodeIdx = arrayBarcodeIdx.GetAt( nPos );
			CBarcodeCSVRecord BarcodeRecord;
			DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );

			__int64 nBarcodeNo = BarcodeRecord.GetBarcodeNoInt();
			if ( ( nBarcodeNo < nBarcodeFrom ) || ( nBarcodeNo > nBarcodeTo ) )
				continue;

			if ( FALSE == m_bProductLabels )
			{
				if ( BarcodeRecord.GetPrintShelfFlag() == FALSE )
					continue;
			}
			else
			{
				if ( BarcodeRecord.GetPrintProductFlag() == FALSE )
					continue;

				if ( CBarcodePluNoTable::IsValidEan( nBarcodeNo ) == FALSE )
					continue;
			}

			int nMod = BarcodeRecord.GetModifier();

			if ( DepartmentSet.GetDepartmentStatus ( PluRecord.GetModifierDeptNoForReport(nMod) ) == TRUE )
			{
				CLabelListCSVRecord LabelListRecord;
				LabelListRecord.SetBarcodeNo( nBarcodeNo );
				LabelListRecord.SetPluNo ( PluRecord.GetPluNoInt() );
				LabelListRecord.SetModifier( nMod );
				LabelListRecord.SetQtyToPrint ( nItemLabels );
				LabelListRecord.SetDirectPluFlag( FALSE );

				int nLabelIdx;
				InsertRecord ( LabelListRecord, nLabelIdx );
			}
		}
	}
}

/**********************************************************************/

void CLabelListCSVArray::CreateApparentList( CPluFilterArray& FilterArray, __int64 nPluFrom, __int64 nPluTo, __int64 nBarcodeFrom, __int64 nBarcodeTo, int nDepartmentFilter, int nType, const char* szDate, CLocationSelectorEntity& LocSelEntity )
{
	StatusProgress.Lock( TRUE, "Creating label list" );
	CreateApparentListInternal ( FilterArray, nPluFrom, nPluTo, nBarcodeFrom, nBarcodeTo, nDepartmentFilter, nType, szDate, LocSelEntity );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CLabelListCSVArray::CreateApparentListInternal( CPluFilterArray& FilterArray, __int64 nPluFrom, __int64 nPluTo, __int64 nBarcodeFrom, __int64 nBarcodeTo, int nDepartmentFilter, int nType, const char* szDate, CLocationSelectorEntity& LocSelEntity )
{
	int nConIndex = LocSelEntity.GetEntityIdx();

	DataManager.Barcode.CreatePluBarcodeList();

	RemoveAll();

	int nStartIdx, nRange;
	FilterArray.GetArrayIdxRange( nPluFrom, nPluTo, nStartIdx, nRange );

	CString strDate = szDate;

	CDepartmentSetCSVRecord DepartmentSet;
	DepartmentSet.CreateDepartmentSet ( nDepartmentFilter );

	//FIRST PASS FOR DIRECT PLU LABELS, SO THEY GET PRIORITY
	for ( int nOffset = 0; nOffset < nRange; nOffset++ )
	{
		StatusProgress.SetPos ( nOffset, nRange ); 

		CString strPrice;
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( FilterArray.GetPluItemIndexAt ( nStartIdx + nOffset ), PluRecord );

		__int64 nPluNo = PluRecord.GetPluNoInt();

		if ( ( nPluNo < nPluFrom ) || ( nPluNo > nPluTo ) )
			continue;

		if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == TRUE )
			continue;

		if ( DepartmentSet.GetDepartmentStatus ( PluRecord.GetBaseDeptNo() ) == FALSE )
			continue;

		if ( FALSE == m_bProductLabels )
		{
			if ( PluRecord.GetPrintShelfFlag() == FALSE )
				continue;
		}
		else
		{
			if ( PluRecord.GetPrintProductFlag() == FALSE )
				continue;

			if ( CBarcodePluNoTable::IsValidEan( nPluNo ) == FALSE )
				continue;
		}
					
		int nItemLabels = 0;

#ifdef STOCKMAN_SYSTEM

		CRecipeCSVRecord Recipe;
		DataManager.Plu.GetRecipe( -1, PluRecord.GetPluNoInt(), Recipe );

		if ( Recipe.GetSize() == 1 )
		{
			double dStockQty = Recipe.GetStockQty(0);

			if ( CPriceHelpers::CompareDoubles( dStockQty, 1.0, 3 ) == 0 )
			{
				CString strStockCode = Recipe.GetStockCode(0);

				CStockCSVRecord StockRecord;
				StockRecord.SetStockCode( strStockCode );

				CStockLevelsCSVRecord StockLevels;

				switch( LocSelEntity.GetConType() )
				{
				case NODE_STOCKPOINT:
					DataManager.Stock.GetStockpointTotals( -1, StockRecord, dbStockpoint.GetSpNo( nConIndex ), StockLevels );
					break;

				case NODE_SITE:
					DataManager.Stock.GetSiteTotals( -1, StockRecord, dbSite.GetSiteNo( nConIndex ), StockLevels );
					break;

				case NODE_DATABASE:
					DataManager.Stock.GetDatabaseTotals( -1, StockRecord, StockLevels );
					break;
				}

				nItemLabels = int( floor ( StockLevels.GetApparentQty() ) );
			}
		}
		
#else
		nItemLabels = int ( floor ( PluRecord.GetApparentLevel() ) );
#endif

		if ( nItemLabels <= 0 )
			continue;
	
		CLabelListCSVRecord LabelListRecord;
		LabelListRecord.SetBarcodeNo( nPluNo );
		LabelListRecord.SetPluNo ( nPluNo );
		LabelListRecord.SetModifier( 0 );
		LabelListRecord.SetQtyToPrint ( nItemLabels );
		LabelListRecord.SetDirectPluFlag( TRUE );

		int nLabelIdx;
		InsertRecord ( LabelListRecord, nLabelIdx );		
	}

	//SECOND PASS FOR LINKS TO BARCODE DATABASE
	for ( int nOffset = 0; nOffset < nRange; nOffset++ )
	{
		StatusProgress.SetPos ( nOffset, nRange ); 

		CString strPrice;
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( FilterArray.GetPluItemIndexAt ( nStartIdx + nOffset ), PluRecord ); 

		__int64 nPluNo = PluRecord.GetPluNoInt();

		if ( ( nPluNo < nPluFrom ) || ( nPluNo > nPluTo ) )
			continue;

		CArray<int,int> arrayBarcodeIdx;
		DataManager.Barcode.GetBarcodeIdxArray( nPluNo, arrayBarcodeIdx );

		for ( int nPos = 0; nPos < arrayBarcodeIdx.GetSize(); nPos++ )
		{
			int nBarcodeIdx = arrayBarcodeIdx.GetAt( nPos );
			CBarcodeCSVRecord BarcodeRecord;
			DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );

			__int64 nBarcodeNo = BarcodeRecord.GetBarcodeNoInt();
			if ( ( nBarcodeNo < nBarcodeFrom ) || ( nBarcodeNo > nBarcodeTo ) )
				continue;

			if ( FALSE == m_bProductLabels )
			{
				if ( BarcodeRecord.GetPrintShelfFlag() == FALSE )
					continue;
			}
			else
			{
				if ( BarcodeRecord.GetPrintProductFlag() == FALSE )
					continue;

				if ( CBarcodePluNoTable::IsValidEan( BarcodeRecord.GetBarcodeNoInt() ) == FALSE )
					continue;
			}

			int nMod = BarcodeRecord.GetModifier();		
			if ( DepartmentSet.GetDepartmentStatus ( PluRecord.GetModifierDeptNoForReport(nMod) ) == FALSE )
				continue;
		
			int nItemLabels = 0;

#ifdef STOCKMAN_SYSTEM

			CRecipeCSVRecord Recipe;
			DataManager.Plu.GetRecipe( -1, PluRecord.GetPluNoInt(), Recipe );

			if ( Recipe.GetSize() == 1 )
			{
				double dStockQty = Recipe.GetStockQty(0);

				if ( CPriceHelpers::CompareDoubles( dStockQty, 1.0, 3 ) == 0 )
				{
					CString strStockCode = Recipe.GetStockCode(0);

					CStockCSVRecord StockRecord;
					StockRecord.SetStockCode( strStockCode );

					CStockLevelsCSVRecord StockLevels;

					switch( LocSelEntity.GetConType() )
					{
					case NODE_STOCKPOINT:
						DataManager.Stock.GetStockpointTotals( -1, StockRecord, dbStockpoint.GetSpNo( nConIndex ), StockLevels );
						break;

					case NODE_SITE:
						DataManager.Stock.GetSiteTotals( -1, StockRecord, dbSite.GetSiteNo( nConIndex ), StockLevels );
						break;

					case NODE_DATABASE:
						DataManager.Stock.GetDatabaseTotals( -1, StockRecord, StockLevels );
						break;
					}

					nItemLabels = int( floor ( StockLevels.GetApparentQty() ) );
				}
			}
		
#else
			nItemLabels = int ( floor ( PluRecord.GetApparentLevel() ) );
#endif

			if ( nItemLabels > 0 )
			{
				CLabelListCSVRecord LabelListRecord;
				LabelListRecord.SetBarcodeNo( nBarcodeNo );
				LabelListRecord.SetPluNo ( PluRecord.GetPluNoInt() );
				LabelListRecord.SetModifier( nMod );
				LabelListRecord.SetQtyToPrint ( nItemLabels );
				LabelListRecord.SetDirectPluFlag( FALSE );

				int nLabelIdx;
				InsertRecord ( LabelListRecord, nLabelIdx );
			}
		}
	}
}

/**********************************************************************/

void CLabelListCSVArray::CreateShelfChangesList() 
{
	StatusProgress.Lock( TRUE, "Creating label list" );
	CreateShelfChangesListInternal();
	StatusProgress.Unlock();
}

/**********************************************************************/

void CLabelListCSVArray::CreateShelfChangesListInternal()
{
	//FLAG EXISTING PRINT REQUESTS AS OLD
	CByteArray FlagArray;
	FlagArray.SetSize ( GetSize() );

	for ( int nIndex = 0; nIndex < GetSize(); nIndex++ )
		FlagArray.SetAt ( nIndex, 0 );

	//FIRST PASS FOR PLU RECORDS SO THEY GET PRIORITY
	int nCount = DataManager.Plu.GetSize();
	for ( int nPluIdx = 0; nPluIdx < nCount; nPluIdx++ )
	{
		StatusProgress.SetPos( nPluIdx, nCount );

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		if ( PluRecord.GetPrintShelfFlag() == FALSE )
			continue;
				
		__int64 nPluNo = PluRecord.GetPluNoInt();
		if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == TRUE )
			continue;

		int nLabelIdx;
		if ( FindBarcodeByNumber ( nPluNo, nLabelIdx ) == FALSE ) 
		{
			CString strStockCode = "";
			DataManager.Plu.GetPluStockCode( nPluNo, strStockCode, FALSE );

			CLabelListCSVRecord LabelListRecord;
			LabelListRecord.SetBarcodeNo ( nPluNo );
			LabelListRecord.SetPluNo ( nPluNo );
			LabelListRecord.SetModifier(0);
			LabelListRecord.SetStockCode( strStockCode );
			LabelListRecord.SetQtyToPrint ( 1 );
			LabelListRecord.SetDirectPluFlag( TRUE );
			LabelListRecord.SetRepText ( PluRecord.GetReportText() );
			
			double dFirstPrice, dSecondPrice;
			GetPriceInfo ( PluRecord, 0, dFirstPrice, dSecondPrice );
			LabelListRecord.SetFirstPrice( dFirstPrice );
			LabelListRecord.SetSecondPrice( dSecondPrice );

			LabelListRecord.SetLabelText ( PluRecord.GetUnitPriceText() );
			LabelListRecord.SetUnitPriceFactor ( CPriceHelpers::GetUnitPriceRatio ( PluRecord ) );

			if ( InsertRecord ( LabelListRecord, nLabelIdx ) == TRUE )
				FlagArray.InsertAt ( nLabelIdx, 1 );
		}
		else
		{
			CLabelListCSVRecord LabelListRecord;
			GetAt ( nLabelIdx, LabelListRecord );
			
			if ( LabelListRecord.GetQtyToPrint() > 1 )
				FlagArray.SetAt ( nLabelIdx, 1 );
			else 
			{
				CString strSavedRepText = LabelListRecord.GetRepText();
				double dSavedFirstPrice = LabelListRecord.GetFirstPrice();
				double dSavedSecondPrice = LabelListRecord.GetSecondPrice();
				CString strSavedLabelText = LabelListRecord.GetLabelText();
				double dSavedUnitPriceFactor = LabelListRecord.GetUnitPriceFactor();
				CString strSavedStockCode = LabelListRecord.GetStockCode();

				CString strCurrentRepText = PluRecord.GetReportText();
				double dCurrentFirstPrice, dCurrentSecondPrice;
				GetPriceInfo ( PluRecord, 0, dCurrentFirstPrice, dCurrentSecondPrice );
				CString strCurrentLabelText = PluRecord.GetUnitPriceText();
				double dCurrentUnitPriceFactor = CPriceHelpers::GetUnitPriceRatio ( PluRecord );
				CString strCurrentStockCode;
				DataManager.Plu.GetPluStockCode( PluRecord.GetPluNoInt(), strCurrentStockCode, FALSE );

				if ( ( CPriceHelpers::CompareDoubles ( dSavedFirstPrice, dCurrentFirstPrice, SysInfo.GetDPValue() ) != 0 ) ||
					( CPriceHelpers::CompareDoubles ( dSavedSecondPrice, dCurrentSecondPrice, SysInfo.GetDPValue() ) != 0 ) ||
					( CPriceHelpers::CompareDoubles( dSavedUnitPriceFactor, dCurrentUnitPriceFactor, SysInfo.GetDPValue() ) != 0 ) ||
					strSavedRepText != strCurrentRepText ||
					strSavedLabelText != strCurrentLabelText ||
					strSavedStockCode != strCurrentStockCode )
				{
					LabelListRecord.SetStockCode ( strCurrentStockCode );
					LabelListRecord.SetFirstPrice ( dCurrentFirstPrice );
					LabelListRecord.SetSecondPrice ( dCurrentSecondPrice );
					LabelListRecord.SetRepText ( strCurrentRepText );
					LabelListRecord.SetLabelText ( strCurrentLabelText );
					LabelListRecord.SetUnitPriceFactor ( dCurrentUnitPriceFactor );
					LabelListRecord.SetDirectPluFlag( TRUE );
					SetAt ( nLabelIdx, LabelListRecord );
					FlagArray.SetAt ( nLabelIdx, 1 );
				}
			}	
		}
	}

	//SECOND PASS FOR BARCODE RECORDS
	nCount = DataManager.Barcode.GetSize();
	for ( int nBarcodeIdx = 0; nBarcodeIdx < nCount; nBarcodeIdx++ )
	{
		StatusProgress.SetPos( nBarcodeIdx, nCount );

		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt ( nBarcodeIdx, BarcodeRecord );

		if ( BarcodeRecord.GetPrintShelfFlag() == FALSE )
			continue;
				
		int nLabelIdx;
		__int64 nBarcodeNo = BarcodeRecord.GetBarcodeNoInt();
		if ( FindBarcodeByNumber ( nBarcodeNo, nLabelIdx ) == FALSE ) 
		{
			int nPluIdx;
			__int64 nPluNo = BarcodeRecord.GetPluNoInt();
			
			//IF THE BARCODE IS NOT ALREADY LISTED FOR PRINTING THEN LIST IT
			//NOW ON CONDITION THAT THE LINKED PLU RECORD EXISTS
			if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt ( nPluIdx, PluRecord );

				CString strStockCode = "";
				DataManager.Plu.GetPluStockCode( nPluNo, strStockCode, FALSE );

				CLabelListCSVRecord LabelListRecord;
				LabelListRecord.SetBarcodeNo ( nBarcodeNo );
				LabelListRecord.SetPluNo ( nPluNo );
				LabelListRecord.SetModifier( BarcodeRecord.GetModifier() );
				LabelListRecord.SetStockCode( strStockCode );
				LabelListRecord.SetQtyToPrint ( 1 );
				LabelListRecord.SetRepText ( PluRecord.GetReportText() );
				LabelListRecord.SetDirectPluFlag( FALSE );
			
				double dFirstPrice, dSecondPrice;
				GetPriceInfo ( PluRecord, BarcodeRecord.GetModifier(), dFirstPrice, dSecondPrice );
				LabelListRecord.SetFirstPrice( dFirstPrice );
				LabelListRecord.SetSecondPrice( dSecondPrice );

				LabelListRecord.SetLabelText ( PluRecord.GetUnitPriceText() );
				LabelListRecord.SetUnitPriceFactor ( CPriceHelpers::GetUnitPriceRatio ( PluRecord ) );

				if ( InsertRecord ( LabelListRecord, nLabelIdx ) == TRUE )
					FlagArray.InsertAt ( nLabelIdx, 1 );
			}
		}
		else
		{
			int nPluIdx;
			__int64 nPluNo = BarcodeRecord.GetPluNoInt();
			
			if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) )
			{
				CLabelListCSVRecord LabelListRecord;
				GetAt ( nLabelIdx, LabelListRecord );

				if ( LabelListRecord.GetDirectPluFlag() == FALSE )
				{
					if ( LabelListRecord.GetQtyToPrint() > 1 )
						FlagArray.SetAt ( nLabelIdx, 1 );
					else 
					{
						CPluCSVRecord PluRecord;
						DataManager.Plu.GetAt( nPluIdx, PluRecord );

						__int64 nSavedPluNo = LabelListRecord.GetPluNoInt();
						CString strSavedRepText = LabelListRecord.GetRepText();
						double dSavedFirstPrice = LabelListRecord.GetFirstPrice();
						double dSavedSecondPrice = LabelListRecord.GetSecondPrice();
						CString strSavedLabelText = LabelListRecord.GetLabelText();
						double dSavedUnitPriceFactor = LabelListRecord.GetUnitPriceFactor();
						CString strSavedStockCode = LabelListRecord.GetStockCode();

						__int64 nCurrentPluNo = nPluNo;
						CString strCurrentRepText = PluRecord.GetReportText();
						double dCurrentFirstPrice, dCurrentSecondPrice;
						GetPriceInfo ( PluRecord, BarcodeRecord.GetModifier(), dCurrentFirstPrice, dCurrentSecondPrice );
						CString strCurrentLabelText = PluRecord.GetUnitPriceText();
						double dCurrentUnitPriceFactor = CPriceHelpers::GetUnitPriceRatio ( PluRecord );
						CString strCurrentStockCode;
						DataManager.Plu.GetPluStockCode( PluRecord.GetPluNoInt(), strCurrentStockCode, FALSE );

						if ( ( CPriceHelpers::CompareDoubles ( dSavedFirstPrice, dCurrentFirstPrice, SysInfo.GetDPValue() ) != 0 ) ||
							( CPriceHelpers::CompareDoubles ( dSavedSecondPrice, dCurrentSecondPrice, SysInfo.GetDPValue() ) != 0 ) ||
							( CPriceHelpers::CompareDoubles( dSavedUnitPriceFactor, dCurrentUnitPriceFactor, SysInfo.GetDPValue() ) != 0 ) ||
							nSavedPluNo != nCurrentPluNo ||
							strSavedRepText != strCurrentRepText ||
							strSavedLabelText != strCurrentLabelText ||
							strSavedStockCode != strCurrentStockCode )
						{
							LabelListRecord.SetPluNo( nCurrentPluNo );
							LabelListRecord.SetStockCode ( strCurrentStockCode );
							LabelListRecord.SetFirstPrice ( dCurrentFirstPrice );
							LabelListRecord.SetSecondPrice ( dCurrentSecondPrice );
							LabelListRecord.SetRepText ( strCurrentRepText );
							LabelListRecord.SetLabelText ( strCurrentLabelText );
							LabelListRecord.SetUnitPriceFactor ( dCurrentUnitPriceFactor );
							SetAt ( nLabelIdx, LabelListRecord );
							FlagArray.SetAt ( nLabelIdx, 1 );
						}
					}
				}
			}	
		}
	}

	for ( int nIndex = GetSize() - 1; nIndex >= 0; nIndex-- )
	{
		if ( FlagArray.GetAt ( nIndex ) == 0 )
			RemoveAt ( nIndex );
	}
}

/**********************************************************************/

void CLabelListCSVArray::UpdateShelfChangesList()
{
	StatusProgress.Lock( TRUE, "Updating label list" );
	UpdateShelfChangesListInternal();
	StatusProgress.Unlock();
}

/**********************************************************************/

void CLabelListCSVArray::UpdateShelfChangesListInternal() 
{
	//LOG PLU NUMBERS THAT HAVE BEEN DEALT WITH 
	CBarcodeCSVArray arrayPluAsBarcode;

	//BUILD NEW ARRAY OF LABELS THAT DON'T NEED PRINTING
	//OR THAT NEED PRINTING TWO OR MORE TIMES
	CLabelListCSVArray arrayUpToDateLabels( FALSE );

	//LOOP THROUGH PLU RECORDS
	int nCount = DataManager.Plu.GetSize();
	for ( int nPluIdx = 0; nPluIdx < nCount; nPluIdx++ )
	{
		StatusProgress.SetPos ( nPluIdx, nCount );
		
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );

		__int64 nPluNo = PluRecord.GetPluNoInt();
		if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == TRUE )
			continue;

		if ( PluRecord.GetPrintShelfFlag() == FALSE )
			continue;

		CBarcodeCSVRecord recordPluAsBarcode;
		recordPluAsBarcode.SetBarcodeNo( nPluNo );
		recordPluAsBarcode.SetPluNo( nPluNo );

		int nIdx;
		if ( arrayPluAsBarcode.FindBarcodeByNumber( nPluNo, nIdx ) == FALSE )
			arrayPluAsBarcode.InsertRecord( nIdx, recordPluAsBarcode );

		int nLabelIdx;
		if ( FindBarcodeByNumber ( nPluNo, nLabelIdx ) == FALSE )
		{	
			CString strStockCode;
			DataManager.Plu.GetPluStockCode( nPluNo, strStockCode, FALSE );

			CLabelListCSVRecord LabelListRecord;
			LabelListRecord.SetBarcodeNo( nPluNo );
			LabelListRecord.SetPluNo( nPluNo );
			LabelListRecord.SetModifier(0);
			LabelListRecord.SetStockCode( strStockCode );
			LabelListRecord.SetQtyToPrint ( 1 );
			LabelListRecord.SetRepText ( PluRecord.GetReportText() );
			LabelListRecord.SetDirectPluFlag( TRUE );
			
			double dFirstPluPrice, dSecondPluPrice;
			GetPriceInfo ( PluRecord, 0, dFirstPluPrice, dSecondPluPrice );
			LabelListRecord.SetFirstPrice( dFirstPluPrice );
			LabelListRecord.SetSecondPrice( dSecondPluPrice );
			LabelListRecord.SetLabelText ( PluRecord.GetUnitPriceText() );
			LabelListRecord.SetUnitPriceFactor ( CPriceHelpers::GetUnitPriceRatio ( PluRecord ) );
		
			int nLabelIdx;
			arrayUpToDateLabels.InsertRecord ( LabelListRecord, nLabelIdx );
		}
		else
		{
			CLabelListCSVRecord LabelListRecord;
			GetAt ( nLabelIdx, LabelListRecord );

			if ( LabelListRecord.GetQtyToPrint() > 1 )
			{
				LabelListRecord.SetPluNo( 0 );
				LabelListRecord.SetStockCode ( "" );
				LabelListRecord.SetLabelText ( "" );
				LabelListRecord.SetRepText ( "" );
				LabelListRecord.SetUnitPriceFactor ( 0.0 );
				LabelListRecord.SetFirstPrice ( 0.0 );
				LabelListRecord.SetSecondPrice ( 0.0 );
				LabelListRecord.SetDirectPluFlag( TRUE );

				int nLabelIdx;
				arrayUpToDateLabels.InsertRecord ( LabelListRecord, nLabelIdx );
			}
		}
	}

	//LOOP THROUGH BARCODE RECORDS
	nCount = DataManager.Barcode.GetSize();
	for ( int nBarcodeIdx = 0; nBarcodeIdx < nCount; nBarcodeIdx++ )
	{
		StatusProgress.SetPos ( nBarcodeIdx, nCount );
		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt ( nBarcodeIdx, BarcodeRecord );

		int nIdx;
		__int64 nBarcodeNo = BarcodeRecord.GetBarcodeNoInt();
		if ( arrayPluAsBarcode.FindBarcodeByNumber( nBarcodeNo, nIdx ) == TRUE )
			continue;

		if ( BarcodeRecord.GetPrintShelfFlag() == FALSE )
			continue;

		int nLabelIdx;
		if ( FindBarcodeByNumber ( BarcodeRecord.GetBarcodeNoInt(), nLabelIdx ) == FALSE )
		{	
			int nPluIdx;
			__int64 nPluNo = BarcodeRecord.GetPluNoInt();
			if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt ( nPluIdx, PluRecord );
				
				CString strStockCode;
				DataManager.Plu.GetPluStockCode( PluRecord.GetPluNoInt(), strStockCode, FALSE );

				CLabelListCSVRecord LabelListRecord;
				LabelListRecord.SetBarcodeNo( BarcodeRecord.GetBarcodeNoInt() );
				LabelListRecord.SetPluNo( BarcodeRecord.GetPluNoInt() );
				LabelListRecord.SetModifier( BarcodeRecord.GetModifier() );
				LabelListRecord.SetStockCode( strStockCode );
				LabelListRecord.SetQtyToPrint ( 1 );
				LabelListRecord.SetRepText ( PluRecord.GetReportText() );
				LabelListRecord.SetDirectPluFlag( FALSE );
			
				double dFirstPluPrice, dSecondPluPrice;
				GetPriceInfo ( PluRecord, BarcodeRecord.GetModifier(), dFirstPluPrice, dSecondPluPrice );
				LabelListRecord.SetFirstPrice( dFirstPluPrice );
				LabelListRecord.SetSecondPrice( dSecondPluPrice );
				LabelListRecord.SetLabelText ( PluRecord.GetUnitPriceText() );
				LabelListRecord.SetUnitPriceFactor ( CPriceHelpers::GetUnitPriceRatio ( PluRecord ) );
				
				int nLabelIdx;
				arrayUpToDateLabels.InsertRecord ( LabelListRecord, nLabelIdx );
			}
		}
		else
		{
			CLabelListCSVRecord LabelListRecord;
			GetAt ( nLabelIdx, LabelListRecord );

			if ( LabelListRecord.GetQtyToPrint() > 1 )
			{
				LabelListRecord.SetPluNo( 0 );
				LabelListRecord.SetStockCode ( "" );
				LabelListRecord.SetLabelText ( "" );
				LabelListRecord.SetRepText ( "" );
				LabelListRecord.SetUnitPriceFactor ( 0.0 );
				LabelListRecord.SetFirstPrice ( 0.0 );
				LabelListRecord.SetSecondPrice ( 0.0 );
				
				int nLabelIdx;
				arrayUpToDateLabels.InsertRecord ( LabelListRecord, nLabelIdx );
			}
		}
	}

	RemoveAll();
	
	nCount = arrayUpToDateLabels.GetSize();
	for ( int n = 0; n < nCount; n++ )
	{
		StatusProgress.SetPos( n, nCount );

		CLabelListCSVRecord record;
		arrayUpToDateLabels.GetAt( n, record );
		Add( record );
	}
}

/**********************************************************************/

void CLabelListCSVArray::RestoreSavedList()
{
	StatusProgress.Lock( TRUE, "Restoring label list" );
	RestoreSavedListInternal();
	StatusProgress.Unlock();
}

/**********************************************************************/

void CLabelListCSVArray::RestoreSavedListInternal()
{
	RemoveAll();
	
	CString strFilename; 
	if (TRUE == m_bProductLabels )
		strFilename = PriceSetItemManager.GetSavedProductLabelsFilename();
	else
		strFilename = PriceSetItemManager.GetSavedShelfEdgeLabelsFilename();

	CSSFile fileLabelList;
	if ( fileLabelList.Open ( strFilename, "rb" ) == FALSE )
		return;

	CString strBuffer;
	while ( fileLabelList.ReadString ( strBuffer ) == TRUE )
	{
		StatusProgress.SetPos ( fileLabelList );
		
		CCSV csv ( strBuffer );
		__int64 nBarcodeOrPluNo = csv.GetInt64(0);
		int nItemLabels = csv.GetInt(1);
		bool bDirectPlu = csv.GetBool(2);
	
		if ( TRUE == m_bProductLabels ) 
			if ( BarcodePluNoTable.IsValidEan( nBarcodeOrPluNo ) == FALSE )
				continue;

		__int64 nBarcodeNo = 0;
		__int64 nPluNo = 0;
		int nModifier = 0;

		CPluCSVRecord PluRecord;

		if ( TRUE == bDirectPlu )
		{
			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber( nBarcodeOrPluNo, nPluIdx ) == TRUE )
			{
				nBarcodeNo = nBarcodeOrPluNo;
				nPluNo = nBarcodeOrPluNo;
				DataManager.Plu.GetAt( nPluIdx, PluRecord );
			}
		}
		else 
		{
			int nBarcodeIdx;
			if ( DataManager.Barcode.FindBarcodeByNumber( nBarcodeOrPluNo, nBarcodeIdx ) == TRUE )
			{
				CBarcodeCSVRecord BarcodeRecord;
				DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );

				int nPluIdx;
				nPluNo = BarcodeRecord.GetPluNoInt();
				if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
				{
					nBarcodeNo = nBarcodeOrPluNo;
					DataManager.Plu.GetAt( nPluIdx, PluRecord );
					nModifier = BarcodeRecord.GetModifier();
				}
			}
		}

		if ( 0 == nBarcodeNo )
			continue;

		CLabelListCSVRecord LabelListRecord;
		LabelListRecord.SetBarcodeNo( nBarcodeNo );
		LabelListRecord.SetPluNo( nPluNo );
		LabelListRecord.SetModifier( nModifier );
		LabelListRecord.SetQtyToPrint ( nItemLabels );
		LabelListRecord.SetDirectPluFlag( bDirectPlu );

		if ( FALSE == m_bProductLabels )
		{
			LabelListRecord.SetRepText ( PluRecord.GetReportText() );
				
			double dFirstPluPrice, dSecondPluPrice;
			GetPriceInfo( PluRecord, nModifier, dFirstPluPrice, dSecondPluPrice );
			LabelListRecord.SetFirstPrice( dFirstPluPrice );
			LabelListRecord.SetSecondPrice( dSecondPluPrice );
			LabelListRecord.SetLabelText ( PluRecord.GetUnitPriceText() );
			LabelListRecord.SetUnitPriceFactor ( CPriceHelpers::GetUnitPriceRatio ( PluRecord ) );
		}
		else
		{
			LabelListRecord.SetRepText ( "" );
			LabelListRecord.SetFirstPrice( 0.0 );
			LabelListRecord.SetSecondPrice( 0.0 );
			LabelListRecord.SetLabelText ( "" );
			LabelListRecord.SetUnitPriceFactor ( 0.0 );
		}
				
		int nLabelIdx;
		InsertRecord ( LabelListRecord, nLabelIdx );	
	}	
}

/**********************************************************************/

bool CLabelListCSVArray::SaveList()
{
	CString strFilename; 
	if ( TRUE == m_bProductLabels )
		strFilename = PriceSetItemManager.GetSavedProductLabelsFilename();
	else
		strFilename = PriceSetItemManager.GetSavedShelfEdgeLabelsFilename();

	CSSFile fileLabelList;
	if ( fileLabelList.Open ( strFilename, "wb" ) == FALSE )
		return FALSE;

	StatusProgress.Lock( TRUE, "Saving label list" );
	for ( int nIndex = 0; nIndex < GetSize(); nIndex++ )
	{
		StatusProgress.SetPos ( nIndex, GetSize() );
		CLabelListCSVRecord LabelListRecord;
		GetAt ( nIndex, LabelListRecord );
		
		CCSV csvOut;
		csvOut.Add ( LabelListRecord.GetBarcodeNoInt() );
		csvOut.Add ( LabelListRecord.GetQtyToPrint() );
		csvOut.Add ( LabelListRecord.GetDirectPluFlag() );
		csvOut.Add ( LabelListRecord.GetModifier() );
		fileLabelList.WriteLine ( csvOut.GetLine() );
	}
	StatusProgress.Unlock();
	return TRUE;
}

/**********************************************************************/

int CLabelListCSVArray::GetTotalLabelCount()
{
	int nCount = 0;

	StatusProgress.Lock( TRUE, "Counting labels" );
	for ( int n = 0; n < GetSize(); n++ )
	{
		StatusProgress.SetPos( n, GetSize() );

		CLabelListCSVRecord Label;
		GetAt( n, Label );
		nCount += Label.GetQtyToPrint();
	}
	StatusProgress.Unlock();

	return nCount;
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CLabelListCSVArray::AutoBarcode( CStockCSVRecord& StockRecord, int nQty )
{
	if ( Sysset.IsBarmanSystem() == FALSE )
		return;

	if ( EcrmanOptions.GetLabelsProductAutoBatchFlag() == FALSE )
		return;

	if ( StockRecord.GetAutoLabelFlag() == FALSE )
		return;
	
	int nStockPluIdx;
	CStockPluRecord StockPluRecord;

	//GIVE UP IF NOT LINKED TO ANY STOCK PLU RECORDS
	if ( DataManager.StockPlu.FindStockCode ( StockRecord.GetStockCode(), nStockPluIdx ) == FALSE )
		return;

	//GIVE UP IF NOT LINKED TO ANY PLU RECORDS
	DataManager.StockPlu.GetAt ( nStockPluIdx, StockPluRecord );
	if ( StockPluRecord.GetPluNoCount() == 0 )
		return;

	//GIVE UP IF THE PLU DOES NOT EXIST
	int nPluIdx;
	__int64 nPluNo = StockPluRecord.GetPluNo(0);
	if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == FALSE )
		return;

	//GIVE UP IF THE PLU DOES NOT SELL EXACTLY ONE STOCK ITEM
	double dStockQtyPerPlu;
	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt( nPluIdx, PluRecord );
	DataManager.Plu.GetPluStockQty ( nPluNo, dStockQtyPerPlu );
	if ( CPriceHelpers::CompareDoubles ( dStockQtyPerPlu, 1.0, 3 ) != 0 )
		return;

	if ( AutoPluBarcode( PluRecord, nQty ) == TRUE )
		return;
		
	AutoExternalBarcode( PluRecord, nQty );
}

/**********************************************************************/

bool CLabelListCSVArray::AutoPluBarcode( CPluCSVRecord& PluRecord, int nQty )
{
	__int64 nPluNo = PluRecord.GetPluNoInt();

	if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == TRUE )
		return FALSE;

	if ( BarcodePluNoTable.IsValidEan( nPluNo ) == FALSE )
		return FALSE;

	if ( PluRecord.GetPrintProductFlag() == FALSE )
		return FALSE;

	int nBarcodeLogIdx;				
	if ( FindBarcodeByNumber ( nPluNo, nBarcodeLogIdx ) == TRUE )				
	{
		CLabelListCSVRecord BarcodeLogRecord;
		GetAt ( nBarcodeLogIdx, BarcodeLogRecord );
					
		BarcodeLogRecord.SetBarcodeNo ( nPluNo );
		BarcodeLogRecord.SetPluNo ( nPluNo );
						
		if ( BarcodeLogRecord.GetDirectPluFlag() == TRUE )
			BarcodeLogRecord.SetQtyToPrint ( BarcodeLogRecord.GetQtyToPrint() + nQty );
		else	
			BarcodeLogRecord.SetQtyToPrint ( nQty );
						
		BarcodeLogRecord.SetDirectPluFlag( TRUE );

		SetAt ( nBarcodeLogIdx, BarcodeLogRecord );
	}
	else
	{
		CLabelListCSVRecord BarcodeLogRecord;
		BarcodeLogRecord.SetBarcodeNo ( nPluNo );
		BarcodeLogRecord.SetPluNo ( nPluNo );
		BarcodeLogRecord.SetQtyToPrint ( nQty );
		BarcodeLogRecord.SetDirectPluFlag( TRUE );
		InsertAt ( nBarcodeLogIdx, BarcodeLogRecord );
	}

	return TRUE;
}

/**********************************************************************/

bool CLabelListCSVArray::AutoExternalBarcode( CPluCSVRecord& PluRecord, int nQty )
{
	__int64 nPluNo = PluRecord.GetPluNoInt();

	DataManager.Barcode.CreatePluBarcodeList();
	
	CArray<int,int> BarcodeIdxArray;
	DataManager.Barcode.GetBarcodeIdxArray( nPluNo, BarcodeIdxArray );

	bool bFound = FALSE;
	int nBarcodeIdx = 0;
	CBarcodeCSVRecord BarcodeRecord;
	for ( int nPos = 0; nPos < BarcodeIdxArray.GetSize(); nPos++ )
	{
		nBarcodeIdx = BarcodeIdxArray.GetAt( nPos );
		DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );
		if ( BarcodeRecord.GetPrintProductFlag() == TRUE )
		{
			bFound = TRUE;
			break;
		}
	}

	//GIVE UP IF THE PLU IS NOT LINKED TO A BARCODE
	if ( bFound == FALSE )
		return FALSE;

	//GIVE UP IF NOT A VALID BAR CODE
	__int64 nBarcodeNo = BarcodeRecord.GetBarcodeNoInt();
	if ( CBarcodePluNoTable::IsValidEan ( nBarcodeNo ) == FALSE )
		return FALSE;
	
	int nBarcodeLogIdx;				
	if ( FindBarcodeByNumber ( nBarcodeNo, nBarcodeLogIdx ) == TRUE )				
	{
		CLabelListCSVRecord BarcodeLogRecord;
		GetAt ( nBarcodeLogIdx, BarcodeLogRecord );
					
		BarcodeLogRecord.SetBarcodeNo ( nBarcodeNo );
		BarcodeLogRecord.SetPluNo ( nPluNo );
						
		if ( BarcodeLogRecord.GetDirectPluFlag() == FALSE )
			BarcodeLogRecord.SetQtyToPrint ( BarcodeLogRecord.GetQtyToPrint() + nQty );
		else	
			BarcodeLogRecord.SetQtyToPrint ( nQty );
						
		BarcodeLogRecord.SetDirectPluFlag( FALSE );

		SetAt ( nBarcodeLogIdx, BarcodeLogRecord );
	}
	else
	{
		CLabelListCSVRecord BarcodeLogRecord;
		BarcodeLogRecord.SetBarcodeNo ( nBarcodeNo );
		BarcodeLogRecord.SetPluNo ( nPluNo );
		BarcodeLogRecord.SetQtyToPrint ( nQty );
		BarcodeLogRecord.SetDirectPluFlag( FALSE );
		InsertAt ( nBarcodeLogIdx, BarcodeLogRecord );
	}

	return TRUE;
}

/**********************************************************************/

void CLabelListCSVArray::MergePendingFiles()
{
	m_arrayPendingFiles.RemoveAll();

	int nDbIdx = DataManager.GetActiveDbIdx();
	if ( ( nDbIdx < 0 ) || ( nDbIdx >= dbDatabase.GetSize() ) )
		return;

	CString strPath = DataManager.GetFilePathProductLabels( nDbIdx );
	strPath = strPath.Left( strPath.GetLength() - 4 );
	strPath += "_";

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strPath + "*.*" );

	bool bResult = FALSE;
	int nFileCount = 0;

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDots() == TRUE )
			continue;

		if ( finder.IsDirectory() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();

		CString strTest = strFilename.Right(19);
		strTest.MakeUpper();
		
		if ( strTest.Right(4) != ".DAT" )
			continue;

		if ( ::TestNumeric( strTest.Left(8) ) == FALSE )
			continue;

		if ( strTest.Mid(8,1) != "_" )
			continue;

		if ( ::TestNumeric( strTest.Mid(9,6) ) == FALSE )
			continue;

		m_arrayPendingFiles.Add( strFilename );
		CLabelListCSVArray PendingLabels( TRUE );
		PendingLabels.Read( finder.GetFilePath() );
		MergeLabels( PendingLabels );
	}
}

/**********************************************************************/

void CLabelListCSVArray::DeletePendingFiles()
{
	int nDbIdx = DataManager.GetActiveDbIdx();
	if ( ( nDbIdx < 0 ) || ( nDbIdx >= dbDatabase.GetSize() ) )
		return;

	CString strPath = dbDatabase.GetFolderPathData( nDbIdx );
	strPath += "\\";

	for ( int n = 0; n < m_arrayPendingFiles.GetSize(); n++ )
		CFileRemover FileRemover( strPath + m_arrayPendingFiles.GetAt(n) );

	m_arrayPendingFiles.RemoveAll();
}

/**********************************************************************/

void CLabelListCSVArray::ClearPendingFileList()
{
	m_arrayPendingFiles.RemoveAll();
}

/**********************************************************************/

void CLabelListCSVArray::MergeLabels( CLabelListCSVArray& arraySource )
{
	for ( int nPendingLabelIdx = 0; nPendingLabelIdx < arraySource.GetSize(); nPendingLabelIdx++ )
	{
		CLabelListCSVRecord PendingLabel;
		arraySource.GetAt( nPendingLabelIdx, PendingLabel );

		int nMainLabelIdx;
		if ( FindBarcodeByNumber( PendingLabel.GetBarcodeNoInt(), nMainLabelIdx ) == FALSE )
			InsertAt( nMainLabelIdx, PendingLabel );
		else
		{
			CLabelListCSVRecord ExistingLabel;
			GetAt( nMainLabelIdx, ExistingLabel );
			ExistingLabel.SetQtyToPrint( ExistingLabel.GetQtyToPrint() + PendingLabel.GetQtyToPrint() );
			SetAt( nMainLabelIdx, ExistingLabel );
		}
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
