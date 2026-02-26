/**********************************************************************/
#include "SalesHistoryFields.h"
/**********************************************************************/
#include "EposReportCustomSettings.h"
/**********************************************************************/

CEposReportCustomSettingsHourly::CEposReportCustomSettingsHourly()
{
	Reset();
}

/**********************************************************************/

void CEposReportCustomSettingsHourly::Reset()
{
	m_nTimeSliceType = SH_TIMESLICE_30MIN;
	m_strTimeSliceEntityID = "";
	m_bAllowAdhoc = FALSE;
}

/**********************************************************************/

void CEposReportCustomSettingsHourly::GetSettingsCSVLine( CString& strLine )
{
	CCSV csv;
	csv.Add( 1 );
	csv.Add( m_nTimeSliceType );
	csv.Add( m_strTimeSliceEntityID );
	csv.Add( m_bAllowAdhoc );
	strLine = csv.GetLine();
}

/**********************************************************************/

void CEposReportCustomSettingsHourly::SetSettingsCSVLine( CString& strLine )
{
	Reset();

	CCSV csv( strLine );

	int nVer = csv.GetInt(0);
	switch( nVer )
	{
	case 1:
		SetTimeSliceType( csv.GetInt(1) );
		SetTimeSliceEntityID( csv.GetString(2) );
		SetAllowAdhocFlag( csv.GetBool(3) );
		break;
	}
}

/**********************************************************************/

void CEposReportCustomSettingsHourly::SetTimeSliceType( int n )
{
	switch(n)
	{
	case SH_TIMESLICE_10MIN:
	case SH_TIMESLICE_15MIN:
	case SH_TIMESLICE_30MIN:
	case SH_TIMESLICE_60MIN:
	case SH_CUSTOM:
		m_nTimeSliceType = n;
		break;
	}
}

/**********************************************************************/

void CEposReportCustomSettingsHourly::SetTimeSliceEntityID( const char* sz )
{
	m_strTimeSliceEntityID = sz;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCustomSettingsPluPriceBand::CEposReportCustomSettingsPluPriceBand()
{
	Reset();
}

/**********************************************************************/

void CEposReportCustomSettingsPluPriceBand::Reset()
{
	m_bAllowAdhoc = FALSE;
	m_nPriceBandFlags = 1023;
	m_bColumnNames = FALSE;
	m_bSortByServer = FALSE;
	m_nReportSubType = EPOSPRICEBANDREPORT_PLUSALES;
}

/**********************************************************************/

void CEposReportCustomSettingsPluPriceBand::GetSettingsCSVLine(CString& strLine)
{
	CCSV csv;
	csv.Add(1);
	csv.Add(m_bAllowAdhoc);
	csv.Add(m_nPriceBandFlags);
	csv.Add(m_bColumnNames);
	csv.Add(m_nReportSubType);
	csv.Add(m_bSortByServer);
	strLine = csv.GetLine();
}

/**********************************************************************/

void CEposReportCustomSettingsPluPriceBand::SetSettingsCSVLine(CString& strLine)
{
	Reset();

	CCSV csv(strLine);

	int nVer = csv.GetInt(0);
	switch (nVer)
	{
	case 1:
		SetAllowAdhocFlag(csv.GetBool(1));
		SetPriceBandFlags(csv.GetInt(2));
		SetColumnNamesFlag(csv.GetBool(3));
		SetReportSubType(csv.GetInt(4));
		SetSortByServerFlag(csv.GetBool(5));
		break;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCustomSettingsGeneral::CEposReportCustomSettingsGeneral()
{
	Reset();
}

/**********************************************************************/

void CEposReportCustomSettingsGeneral::Reset()
{
	m_bWeekday = FALSE;
	m_bQuantity = FALSE;
	m_bTaxBand = FALSE;
	m_bTaxAmount = FALSE;
	m_bPromoDetail = FALSE;
	m_bPromoSummary = FALSE;
	m_bPromoPositive = FALSE;
	m_bReconciliation = TRUE;
	m_bVoid = FALSE;
	m_nVoidQtyType = 0;
	m_bNoSale = FALSE;
	m_bConsolDept = TRUE;
	m_bConsolGroupReport = FALSE;
	m_bConsolGroupConsol = FALSE;
	m_bConsolGroupEpos = FALSE;
	m_bSalesPromo = TRUE;
	m_bSeparateChange = TRUE;
	m_bCCDVariance = TRUE;
	m_bAllowAdhoc = FALSE;
}

/**********************************************************************/

void CEposReportCustomSettingsGeneral::ValidateConsolSettings()
{
	if ( ( TRUE == m_bConsolGroupReport ) || ( TRUE == m_bConsolGroupConsol ) )
		m_bConsolGroupEpos = FALSE;
	else if ( ( FALSE == m_bConsolDept ) && ( FALSE == m_bConsolGroupEpos ) )
		m_bConsolDept = TRUE;
}

/**********************************************************************/

void CEposReportCustomSettingsGeneral::SetVoidQtyType( int n )
{
	if ( ( n >= 0 ) && ( n <= 2 ) )
		m_nVoidQtyType = n;
}

/**********************************************************************/

void CEposReportCustomSettingsGeneral::GetSettingsCSVLine( CString& strLine )
{
	CCSV csv;
	csv.Add( 10 );
	csv.Add( m_bWeekday );
	csv.Add( m_bQuantity );
	csv.Add( m_bTaxBand );
	csv.Add( m_bPromoDetail );
	csv.Add( m_bPromoSummary );
	csv.Add( m_bPromoPositive );
	csv.Add( m_bReconciliation );
	csv.Add( m_bVoid );
	csv.Add( m_nVoidQtyType );
	csv.Add( m_bNoSale );
	csv.Add( m_bConsolDept );
	csv.Add( m_bConsolGroupReport );
	csv.Add( m_bConsolGroupConsol );
	csv.Add( m_bConsolGroupEpos );
	csv.Add( m_bSalesPromo );
	csv.Add( m_bTaxAmount );
	csv.Add( m_bSeparateChange );
	csv.Add( m_bCCDVariance );
	csv.Add( m_bAllowAdhoc );
	strLine = csv.GetLine();
}

/**********************************************************************/

void CEposReportCustomSettingsGeneral::SetSettingsCSVLine( CString& strLine )
{
	Reset();

	CCSV csv( strLine );

	int nVersion = csv.GetInt(0);
	
	switch( nVersion )
	{
	case 1:
		{
			int nReportType = csv.GetInt(2);
			SetWeekdayFlag( csv.GetBool(1) );
			SetTaxBandFlag( FALSE );
			SetPromoDetailFlag( FALSE );
			SetPromoSummaryFlag( nReportType != 0 );
			SetReconciliationFlag( nReportType != 1 );
			SetVoidFlag( nReportType != 0 );
			SetNoSaleFlag( nReportType != 0 );
			SetConsolDeptFlag( csv.GetBool(3) );
			SetConsolGroupReportFlag( csv.GetBool(4) );
			SetConsolGroupConsolFlag( csv.GetBool(5) );
			SetConsolGroupEposFlag( csv.GetBool(6) );
			SetTaxAmountFlag( FALSE );
			SetSeparateChangeFlag( TRUE );
			SetCCDVarianceFlag( TRUE );
			SetAllowAdhocFlag( FALSE );
			ValidateConsolSettings();
		}
		break;

	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		{
			int nOffset = 1;
			SetWeekdayFlag( csv.GetBool(nOffset++) );
			if ( nVersion >= 5 ) SetQuantityFlag(csv.GetBool(nOffset++) );
			if ( nVersion >= 3 ) SetTaxBandFlag(csv.GetBool(nOffset++) );
			SetPromoDetailFlag( csv.GetBool(nOffset++) );
			SetPromoSummaryFlag( csv.GetBool(nOffset++) );
			if ( nVersion >= 7 ) SetPromoPositiveFlag( csv.GetBool(nOffset++) );
			SetReconciliationFlag( csv.GetBool(nOffset++) );
			SetVoidFlag( csv.GetBool(nOffset++) );
			if ( nVersion >= 6 ) SetVoidQtyType( csv.GetInt(nOffset++) );
			SetNoSaleFlag( csv.GetBool(nOffset++) );
			SetConsolDeptFlag( csv.GetBool(nOffset++) );
			SetConsolGroupReportFlag( csv.GetBool(nOffset++) );
			SetConsolGroupConsolFlag( csv.GetBool(nOffset++) );
			SetConsolGroupEposFlag( csv.GetBool(nOffset++) );
			if ( nVersion >= 8 ) SetSalesPromoFlag(csv.GetBool(nOffset++) );
			SetAllowAdhocFlag( csv.GetBool(nOffset++) );
			if ( nVersion >= 4 ) SetTaxAmountFlag(csv.GetBool(nOffset++) );
			SetSeparateChangeFlag( FALSE );
			SetCCDVarianceFlag( TRUE );
			ValidateConsolSettings();
		}
		break;

	case 9:
	case 10:
		{
			int nOffset = 1;
			SetWeekdayFlag( csv.GetBool(nOffset++) );
			SetQuantityFlag(csv.GetBool(nOffset++) );
			SetTaxBandFlag(csv.GetBool(nOffset++) );
			SetPromoDetailFlag( csv.GetBool(nOffset++) );
			SetPromoSummaryFlag( csv.GetBool(nOffset++) );
			SetPromoPositiveFlag( csv.GetBool(nOffset++) );
			SetReconciliationFlag( csv.GetBool(nOffset++) );
			SetVoidFlag( csv.GetBool(nOffset++) );
			SetVoidQtyType( csv.GetInt(nOffset++) );
			SetNoSaleFlag( csv.GetBool(nOffset++) );
			SetConsolDeptFlag( csv.GetBool(nOffset++) );
			SetConsolGroupReportFlag( csv.GetBool(nOffset++) );
			SetConsolGroupConsolFlag( csv.GetBool(nOffset++) );
			SetConsolGroupEposFlag( csv.GetBool(nOffset++) );
			SetSalesPromoFlag(csv.GetBool(nOffset++) );
			SetTaxAmountFlag(csv.GetBool(nOffset++) );
			SetSeparateChangeFlag(csv.GetBool(nOffset++) );
			if ( nVersion >= 10 ) SetCCDVarianceFlag( csv.GetBool(nOffset++) );
			SetAllowAdhocFlag( csv.GetBool(nOffset++) );
			ValidateConsolSettings();
		}
		break;
	}

	switch( nVersion )
	{
	case 1:
	case 2:
	case 3:
	case 4:
		SetQuantityFlag( m_bPromoDetail | m_bPromoSummary | m_bVoid | m_bNoSale );
		break;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCustomSettingsTransaction::CEposReportCustomSettingsTransaction()
{
	Reset();
}

/**********************************************************************/

void CEposReportCustomSettingsTransaction::Reset()
{
	m_bDiscrepancy = FALSE;
	m_bCashRSPVersion = TRUE;
	m_nSortType = 0;
	m_nTimeType = 0;
	m_bItemSaleDate = FALSE;
	m_nTranServerFrom = 0;
	m_nTranServerTo = 9999;
	m_nItemServerFrom = 0;
	m_nItemServerTo = 9999;
	m_bShowItemServer = FALSE;
	m_bModifierText = FALSE;
	m_nSeqNoFrom = 0;
	m_nSeqNoTo = 999999;
	m_strFilterText = "";
	m_nFilterTextType = 0;
	m_bAllowAdhoc = FALSE;
}

/**********************************************************************/

void CEposReportCustomSettingsTransaction::FixRange( int& nStart, int& nEnd )
{
	if ( nEnd < nStart )
	{
		int nTemp = nStart;
		nStart = nEnd;
		nEnd = nTemp;
	}
}

/**********************************************************************/

void CEposReportCustomSettingsTransaction::SetSortType( int n )
{
	if ( ( n >= 0 ) && ( n <= 2 ) )
		m_nSortType = n;
}

/**********************************************************************/

void CEposReportCustomSettingsTransaction::SetTimeType( int n )
{
	if ( ( n >= 0 ) && ( n <= 2 ) )
		m_nTimeType = n;
}

/**********************************************************************/

void CEposReportCustomSettingsTransaction::SetSeqNoFrom( int n )
{
	if ( ( n >= 0 ) && ( n <= 999999 ) )
		m_nSeqNoFrom = n;
}

/**********************************************************************/

void CEposReportCustomSettingsTransaction::SetSeqNoTo( int n )
{
	if ( ( n >= 0 ) && ( n <= 999999 ) )
		m_nSeqNoTo = n;
}

/**********************************************************************/

void CEposReportCustomSettingsTransaction::SetTranServerFrom( int n )
{
	if ( ( n >= 0 ) && ( n <= 9999 ) )
		m_nTranServerFrom = n;
}

/**********************************************************************/

void CEposReportCustomSettingsTransaction::SetTranServerTo( int n )
{
	if ( ( n >= 0 ) && ( n <= 9999 ) )
		m_nTranServerTo = n;
}

/**********************************************************************/

void CEposReportCustomSettingsTransaction::SetItemServerFrom( int n )
{
	if ( ( n >= 0 ) && ( n <= 9999 ) )
		m_nItemServerFrom = n;
}

/**********************************************************************/

void CEposReportCustomSettingsTransaction::SetItemServerTo( int n )
{
	if ( ( n >= 0 ) && ( n <= 9999 ) )
		m_nItemServerTo = n;
}

/**********************************************************************/

void CEposReportCustomSettingsTransaction::SetFilterTextType( int n )
{
	if ( ( n >= 0 ) && ( n <= 2 ) )
		m_nFilterTextType = n;
}

/**********************************************************************/

void CEposReportCustomSettingsTransaction::GetSettingsCSVLine( CString& strLine )
{
	FixRange( m_nSeqNoFrom, m_nSeqNoTo );
	FixRange( m_nTranServerFrom, m_nTranServerTo );
	FixRange( m_nItemServerFrom, m_nItemServerTo );

	CCSV csv;
	csv.Add( 9 );
	csv.Add( m_bDiscrepancy );
	csv.Add( m_bCashRSPVersion );
	csv.Add( m_nSortType );
	csv.Add( m_nTimeType );
	csv.Add( m_bItemSaleDate );
	csv.Add( m_nSeqNoFrom );
	csv.Add( m_nSeqNoTo );
	csv.Add( m_nTranServerFrom );
	csv.Add( m_nTranServerTo );
	csv.Add( m_nItemServerFrom );
	csv.Add( m_nItemServerTo );
	csv.Add( m_bShowItemServer );
	csv.Add( m_bModifierText );
	csv.Add( m_strFilterText );
	csv.Add( m_nFilterTextType );
	csv.Add( m_bAllowAdhoc );
	strLine = csv.GetLine();
}

/**********************************************************************/

void CEposReportCustomSettingsTransaction::SetSettingsCSVLine( CString& strLine )
{
	Reset();

	CCSV csv( strLine );
	
	int nVer = csv.GetInt(0);
	switch( nVer )
	{
	case 1:
		SetDiscrepancyFlag( csv.GetBool(1) );
		SetTimeType( csv.GetBool(2) ? 1 : 0 );
		SetSortType( csv.GetInt(3) );
		SetAllowAdhocFlag( csv.GetBool(4) );
		break;

	case 2:
		SetDiscrepancyFlag( csv.GetBool(1) );
		SetTimeType( csv.GetBool(2) ? 1 : 0 );
		SetSortType( csv.GetInt(3) );
		SetFilterText( csv.GetString(4) );
		SetAllowAdhocFlag( csv.GetBool(5) );
		break;

	case 3:
		SetDiscrepancyFlag( csv.GetBool(1) );
		SetCashRSPVersionFlag( csv.GetBool(2) );
		SetSortType( csv.GetInt(3) );
		SetTimeType( csv.GetInt(4) );
		SetFilterText( csv.GetString(5) );
		SetAllowAdhocFlag( csv.GetBool(6) );
		break;
	
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		{
			int nPos = 1;
			SetDiscrepancyFlag( csv.GetBool(nPos++) );
			SetCashRSPVersionFlag( csv.GetBool(nPos++) );
			SetSortType( csv.GetInt(nPos++) );
			SetTimeType( csv.GetInt(nPos++) );

			if ( nVer >= 8 )
				SetItemSaleDateFlag( csv.GetBool(nPos++) );

			SetSeqNoFrom( csv.GetInt(nPos++) );
			SetSeqNoTo( csv.GetInt(nPos++) );
			SetTranServerFrom( csv.GetInt(nPos++) );
			SetTranServerTo( csv.GetInt(nPos++) );

			if ( nVer >= 5 )
			{
				SetItemServerFrom( csv.GetInt(nPos++) );
				SetItemServerTo( csv.GetInt(nPos++) );
			}

			if ( nVer >= 6 )
				SetShowItemServerFlag( csv.GetBool(nPos++) );

			if ( nVer >= 7 )
				SetModifierTextFlag( csv.GetBool(nPos++) );
			
			SetFilterText( csv.GetString(nPos++) );

			if ( nVer >= 9 )
				SetFilterTextType( csv.GetInt(nPos++) );

			SetAllowAdhocFlag( csv.GetBool(nPos++) );
		}
		break;
	}

	FixRange( m_nSeqNoFrom, m_nSeqNoTo );
	FixRange( m_nTranServerFrom, m_nTranServerTo );
	FixRange( m_nItemServerFrom, m_nItemServerTo );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCustomSettingsMixMatch::CEposReportCustomSettingsMixMatch()
{
	Reset();
}

/**********************************************************************/

void CEposReportCustomSettingsMixMatch::Reset()
{
	m_nReportType = 0;
	m_nOfferFilter = 0x1FF;
	m_bCovers = FALSE;
	m_bAllowAdhoc = FALSE;
}

/**********************************************************************/

void CEposReportCustomSettingsMixMatch::GetSettingsCSVLine( CString& strLine )
{
	CCSV csv;
	csv.Add( 2 );
	csv.Add( m_nReportType );
	csv.Add( m_nOfferFilter );
	csv.Add( m_bCovers );
	csv.Add( m_bAllowAdhoc );
	strLine = csv.GetLine();
}

/**********************************************************************/

void CEposReportCustomSettingsMixMatch::SetSettingsCSVLine( CString& strLine )
{
	Reset();

	CCSV csv( strLine );

	int nVer = csv.GetInt(0);
	switch( nVer )
	{
	case 1:
		SetReportType( csv.GetInt(1) );
		SetOfferFilter( csv.GetInt(2) );
		SetAllowAdhocFlag( csv.GetBool(3) );
		break;

	case 2:
		SetReportType( csv.GetInt(1) );
		SetOfferFilter( csv.GetInt(2) );
		SetCoversFlag( csv.GetBool(3) );
		SetAllowAdhocFlag( csv.GetBool(4) );
		break;
	}
}

/**********************************************************************/

void CEposReportCustomSettingsMixMatch::SetReportType( int n )
{
	if ( ( n >= 0 ) && ( n <= 2 ) )
		m_nReportType = n;
}

/**********************************************************************/

void CEposReportCustomSettingsMixMatch::SetOfferFilter( int n )
{
	if ( ( n >= 0 ) && ( n <= 511 ) )
		m_nOfferFilter = n;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCustomSettingsDiscount::CEposReportCustomSettingsDiscount()
{
	Reset();
}

/**********************************************************************/

void CEposReportCustomSettingsDiscount::Reset()
{
	m_nOfferFilter = 0x1FF;
	m_bByServer = TRUE;
	m_bSubtotalItems = FALSE;
	m_bAllowAdhoc = FALSE;
}

/**********************************************************************/

void CEposReportCustomSettingsDiscount::GetSettingsCSVLine( CString& strLine )
{
	CCSV csv;
	csv.Add( 1 );
	csv.Add( m_nOfferFilter );
	csv.Add( m_bByServer );
	csv.Add( m_bSubtotalItems );
	csv.Add( m_bAllowAdhoc );
	strLine = csv.GetLine();
}

/**********************************************************************/

void CEposReportCustomSettingsDiscount::SetSettingsCSVLine( CString& strLine )
{
	Reset();

	CCSV csv( strLine );

	int nVer = csv.GetInt(0);
	switch( nVer )
	{
	case 1:
		SetOfferFilter( csv.GetInt(1) );
		SetByServerFlag( csv.GetBool(2) );
		SetSubtotalItemsFlag( csv.GetBool(3) );
		SetAllowAdhocFlag( csv.GetBool(4) );
		break;
	}
}

/**********************************************************************/

void CEposReportCustomSettingsDiscount::SetOfferFilter( int n )
{
	if ( ( n >= 0 ) && ( n <= 511 ) )
		m_nOfferFilter = n;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCustomSettingsDeposit::CEposReportCustomSettingsDeposit()
{
	Reset();
}

/**********************************************************************/

void CEposReportCustomSettingsDeposit::Reset()
{
	m_nReportType = 0;
	m_bShowRefund = TRUE;
	m_bShowRedeem = TRUE;
	m_bShowPayment = TRUE;
	m_bEndDateCutOff = FALSE;
	m_bAllowAdhoc = FALSE;
}

/**********************************************************************/

void CEposReportCustomSettingsDeposit::GetSettingsCSVLine( CString& strLine )
{
	CCSV csv;
	csv.Add( 2 );
	csv.Add( m_nReportType );
	csv.Add( m_bShowRefund );
	csv.Add( m_bShowRedeem );
	csv.Add( m_bShowPayment );
	csv.Add( m_bEndDateCutOff );
	csv.Add( m_bAllowAdhoc );
	strLine = csv.GetLine();
}

/**********************************************************************/

void CEposReportCustomSettingsDeposit::SetSettingsCSVLine( CString& strLine )
{
	Reset();

	CCSV csv( strLine );

	int nVer = csv.GetInt(0);
	switch( nVer )
	{
	case 1:
	case 2:
		{
			int nPos = 1;
			
			SetReportType( csv.GetInt(nPos++) );
			m_bShowRefund = csv.GetBool(nPos++);
			m_bShowRedeem = csv.GetBool(nPos++);
			m_bShowPayment = csv.GetBool(nPos++);
		
			if ( 2 == nVer )
				m_bEndDateCutOff = csv.GetBool(nPos++);

			m_bAllowAdhoc = csv.GetBool(nPos++);
		}
		break;
	}
}

/**********************************************************************/

void CEposReportCustomSettingsDeposit::SetReportType( int n )
{
	if ( ( n >= 0 ) && ( n <= 3 ) )
		m_nReportType = n;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCustomSettingsLoyaltyRecon::CEposReportCustomSettingsLoyaltyRecon()
{
	Reset();
}

/**********************************************************************/

void CEposReportCustomSettingsLoyaltyRecon::Reset()
{
	m_strLoyaltyRequestPath = "";
	m_bAllowAdhoc = FALSE;
	m_bUnreconOnly = FALSE;
}

/**********************************************************************/

void CEposReportCustomSettingsLoyaltyRecon::GetSettingsCSVLine(CString& strLine)
{
	CCSV csv;
	csv.Add(1);
	csv.Add(m_strLoyaltyRequestPath);
	csv.Add(m_bAllowAdhoc);
	csv.Add(m_bUnreconOnly);
	strLine = csv.GetLine();
}

/**********************************************************************/

void CEposReportCustomSettingsLoyaltyRecon::SetSettingsCSVLine(CString& strLine)
{
	Reset();

	CCSV csv(strLine);

	int nVer = csv.GetInt(0);
	switch (nVer)
	{
	case 1:
		SetLoyaltyRequestPath(csv.GetString(1));
		SetAllowAdhocFlag(csv.GetBool(2));
		SetUnreconOnlyFlag(csv.GetBool(3));
		break;
	}
}

/**********************************************************************/

void CEposReportCustomSettingsLoyaltyRecon::SetLoyaltyRequestPath(const char* sz)
{
	m_strLoyaltyRequestPath = sz;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCustomSettingsPaymentDetail::CEposReportCustomSettingsPaymentDetail()
{
	Reset();
}

/**********************************************************************/

void CEposReportCustomSettingsPaymentDetail::Reset()
{
	m_bShowTableNo = FALSE;
	m_bShowTableName = FALSE;
	m_bTableFilter = FALSE;
	m_nTableNoFrom = 0;
	m_nTableNoTo = 999;
	m_bShowSEQNo = FALSE;
	m_bShowServer = FALSE;
	m_bPayNumFilter = FALSE;
	m_strPayNumFilter = "";
	m_nPaymentTypeFilter = 0xFFFFFFFF;
	m_bAllowAdhoc = FALSE;
}

/**********************************************************************/

void CEposReportCustomSettingsPaymentDetail::SetTableNoFrom(int n)
{
	if ((n >= 0) && (n <= 999))
	{
		m_nTableNoFrom = n;
	}
}

/**********************************************************************/

void CEposReportCustomSettingsPaymentDetail::SetTableNoTo(int n)
{
	if ((n >= 0) && (n <= 999))
	{
		m_nTableNoTo = n;
	}
}

/**********************************************************************/

void CEposReportCustomSettingsPaymentDetail::SetPayNumFilter(CString str)
{
	if (str.GetLength() <= 200)
	{
		m_strPayNumFilter = str;
	}
}

/**********************************************************************/

void CEposReportCustomSettingsPaymentDetail::SetPaymentTypeFilter(int n)
{
	if ((n >= 0) && (n <= 0xFFFFFFFF))
	{
		m_nPaymentTypeFilter = n;
	}
}

/**********************************************************************/

void CEposReportCustomSettingsPaymentDetail::GetSettingsCSVLine(CString& strLine)
{
	CCSV csv;
	csv.Add(1);
	csv.Add(m_bShowTableNo);
	csv.Add(m_bShowTableName);
	csv.Add(m_bTableFilter);
	csv.Add(m_nTableNoFrom);
	csv.Add(m_nTableNoTo);
	csv.Add(m_bPayNumFilter);
	csv.Add(m_strPayNumFilter);
	csv.Add(m_nPaymentTypeFilter);
	csv.Add(m_bAllowAdhoc);
	csv.Add(m_bShowSEQNo);
	csv.Add(m_bShowServer);
	strLine = csv.GetLine();
}

/**********************************************************************/

void CEposReportCustomSettingsPaymentDetail::SetSettingsCSVLine(CString& strLine)
{
	Reset();

	CCSV csv(strLine);

	int nVer = csv.GetInt(0);
	switch (nVer)
	{
	case 1:
		SetShowTableNoFlag(csv.GetBool(1));
		SetShowTableNameFlag(csv.GetBool(2));
		SetTableFilterFlag(csv.GetBool(3));
		SetTableNoFrom(csv.GetInt(4));
		SetTableNoTo(csv.GetInt(5));
		SetPayNumFilterFlag(csv.GetBool(6));
		SetPayNumFilter(csv.GetString(7));
		SetPaymentTypeFilter(csv.GetInt(8));
		SetAllowAdhocFlag(csv.GetBool(9));
		SetShowSEQNoFlag(csv.GetBool(10));
		SetShowServerFlag(csv.GetBool(11));
		break;
	}
}

/**********************************************************************/

bool CEposReportCustomSettingsPaymentDetail::IsPaymentTypeEnabled(int nType)
{
	int nShiftLeft = 0;

	switch (nType)
	{
	case PAYMENT_TYPE_CASH:
		nShiftLeft = 0;
		break;

	case PAYMENT_TYPE_CHEQUE:
		nShiftLeft = 1;
		break;

	case PAYMENT_TYPE_EFT:
		nShiftLeft = 2;
		break;

	case PAYMENT_TYPE_EFT_CNP:
		nShiftLeft = 3;
		break;

	case PAYMENT_TYPE_ROOM_AUTO:
		nShiftLeft = 4;
		break;

	case PAYMENT_TYPE_ROOM_MANUAL:
		nShiftLeft = 5;
		break;

	case PAYMENT_TYPE_LOYALTY:
		nShiftLeft = 6;
		break;

	case PAYMENT_TYPE_CURRENCY:
		nShiftLeft = 7;
		break;

	case PAYMENT_TYPE_DEPOSIT:
		nShiftLeft = 8;
		break;

	case PAYMENT_TYPE_EFT_SHARE:
		nShiftLeft = 9;
		break;

	case PAYMENT_TYPE_ACCOUNT:
		nShiftLeft = 10;
		break;

	case PAYMENT_TYPE_CREDITNOTE:
		nShiftLeft = 11;
		break;

	case PAYMENT_TYPE_GIFTCARD:
		nShiftLeft = 12;
		break;

	case PAYMENT_TYPE_EXTERNAL_APP:
		nShiftLeft = 13;
		break;

	case PAYMENT_TYPE_EXTERNAL_PAY:
		nShiftLeft = 14;
		break;

	case PAYMENT_TYPE_OTHER:
		nShiftLeft = 15;
		break;

	case PAYMENT_TYPE_VOUCHERCART:
		nShiftLeft = 17;
		break;

	default:
		nShiftLeft = 16;
		break;
	}

	int nMask = 1 << nShiftLeft;
	return ((m_nPaymentTypeFilter & nMask) != 0);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
