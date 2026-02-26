/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "PriceHelpers.h"
#include "..\CommonEcrStock\PriceSetItemManager.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "..\SPosEcrManager5\PluMonitorEcrman.h"
/**********************************************************************/

CPluMonitorEcrman::CPluMonitorEcrman()
{
}

/**********************************************************************/

CPluMonitorEcrman::~CPluMonitorEcrman()
{
}

/**********************************************************************/

int CPluMonitorEcrman::InsertRecord ( CPluCSVRecord& NewRecord, bool bExportDate )
{
	int nPluIdx;
	if ( FindPluByNumber ( NewRecord.GetPluNoInt(), nPluIdx ) )
		return nPluIdx;

	if ( bExportDate == TRUE )
		NewRecord.SetExport( TRUE );

	m_PluDatabase.InsertAt ( nPluIdx, NewRecord );
	return nPluIdx;
}

/**********************************************************************/

void CPluMonitorEcrman::RemoveAt ( int nPluIdx )
{
	if ( nPluIdx < 0 || nPluIdx >= m_PluDatabase.GetSize() )
		return;

	m_PluDatabase.RemoveAt ( nPluIdx );
}

/**********************************************************************/

void CPluMonitorEcrman::RemoveAll()
{
	m_PluDatabase.RemoveAt( 0, m_PluDatabase.GetSize() );
}

/**********************************************************************/

bool CPluMonitorEcrman::IsModified()
{
	return ( m_PluDatabase.IsModified() );
}

/**********************************************************************/

int CPluMonitorEcrman::ImportLine ( int nImportMethod, int& nPos, CSSImportFile& import, const char* szImportLine )
{
	return m_PluDatabase.ImportLine( nImportMethod, nPos, import, szImportLine );
}

/**********************************************************************/

int CPluMonitorEcrman::ImportLinePriceSet ( int& nPos, CSSImportFile& import, const char* szImportLine, CString& strOldRecord )
{
	return m_PluDatabase.ImportLinePriceSet(nPos, import, szImportLine, strOldRecord );
}

/**********************************************************************/

int CPluMonitorEcrman::SavePluRecord ( int nPluIdx, CPluCSVRecord& PluRecord, bool bResourceChangeInfo, bool bResourceChangePlu )
{
	int nResult = 0;
	if ( ( nPluIdx >= 0 ) && ( nPluIdx < GetSize() ) )
	{
		CPluCSVRecord OldRecord;
		GetAt ( nPluIdx, OldRecord );
		nResult = OldRecord.GetChange( PluRecord );

		bool bSetInfoDate = bResourceChangeInfo || ( ( OldRecord.GetPluInfoModifierFlag() == FALSE ) && ( PluRecord.GetPluInfoModifierFlag() == TRUE ) );
		bool bSetPluDate = bResourceChangePlu;
		bool bSetExportFlag = FALSE;
		
		switch ( nResult )
		{
		case 1:
			bSetPluDate = TRUE;
			bSetExportFlag = TRUE;
			break;

		case 2:
			bSetExportFlag = TRUE;
			break;

		default:
			break;
		}

		if ( TRUE == bSetInfoDate )
			PluRecord.SetModifiedDateTimeInfo();

		if ( TRUE == bSetPluDate )
			PluRecord.SetModifiedDateTimePlu();

		if ( TRUE == bSetExportFlag )
			PluRecord.SetExport( TRUE );

		if ( TRUE == ( bSetInfoDate | bSetPluDate | bSetExportFlag ) )
			SetAt ( nPluIdx, PluRecord );
	}

	return nResult;
}

/**********************************************************************/

void CPluMonitorEcrman::UpdatePrices()
{
	int nSize = GetSize();

	StatusProgress.Lock( TRUE, "Updating PLU prices" );
	for ( int nPluIdx = 0; nPluIdx < nSize; nPluIdx++ )
	{
		StatusProgress.SetPos ( nPluIdx, nSize ); 
		CPluCSVRecord PluRecord;
		GetAt ( nPluIdx, PluRecord );
		UpdatePluPrice ( PluRecord, 0.0, TRUE );
		SavePluRecord ( nPluIdx, PluRecord );
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

void CPluMonitorEcrman::UpdatePluPrice( CPluCSVRecord& Record, double dCost, bool bGetCost )
{
	switch ( Record.GetGPType() )
	{
	//skip plus with fixed prices
	case PLU_GPTYPE_VARIABLE:
		break;

	case PLU_GPTYPE_FIXED:
		{
			CTaxRateInfo TaxRateInfo;
			DataManager.TaxRates.GetTaxRateInfo( Record.GetBaseTaxBandInt(), TaxRateInfo );
			if ( bGetCost ) dCost = GetPluPurchaseCost( Record );
			double dProfit = Record.GetProfitMargin();
			double dOldPrice1 = Record.GetPrice(0);
			double dNewPrice1 = CPriceHelpers::GetPrice1GPFixed ( TaxRateInfo.m_dTaxRate, dCost, dProfit );
			Record.SetPrice ( 0, dNewPrice1 );
		}
		break;

	case PLU_GPTYPE_PERCENT:
		{
			CTaxRateInfo TaxRateInfo;
			DataManager.TaxRates.GetTaxRateInfo( Record.GetBaseTaxBandInt(), TaxRateInfo );
			if ( bGetCost ) dCost = GetPluPurchaseCost( Record );
			double dProfit = Record.GetProfitMargin();
			double dOldPrice1 = Record.GetPrice(0);
			double dNewPrice1 = CPriceHelpers::GetPrice1GPPercent ( TaxRateInfo.m_dTaxRate, dCost, dProfit );
			Record.SetPrice ( 0, dNewPrice1 );
		}
		break;
	}
}

/**********************************************************************/

void CPluMonitorEcrman::PurgeForDownload( int nLocIdx, CPluCSVArray& PluArray, bool bChanges )
{
	CString strDateNow;
	COleDateTime dateNow = COleDateTime::GetCurrentTime();
	strDateNow.Format( "%4.4d%2.2d%2.2d",
		dateNow.GetYear(),
		dateNow.GetMonth(),
		dateNow.GetDay() );

	for ( int nPluIdx = 0; nPluIdx < GetSize(); nPluIdx++ )
	{
		bool bIncludeThisRecord = TRUE;

		if ( PriceSetItemManager.IsPluMaster() == FALSE )
		{
			if ( PriceSetItemManager.GetPluStatus( nPluIdx ) == LOCALPLU_EXCLUDE )
				bIncludeThisRecord = FALSE;
		}

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );
			
		if ( bIncludeThisRecord == TRUE )
			PluArray.Add( PluRecord );
	}
}

/**********************************************************************/
