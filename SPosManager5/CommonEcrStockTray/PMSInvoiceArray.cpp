/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include <math.h>
/**********************************************************************/
#include "FileRemover.h"
#include "NetworkCSVArray.h"
#include "PMSBookingCSVArray.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
#include "PMSTermLineEdit.h"
#include "PriceHelpers.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PMSInvoiceArray.h"
/**********************************************************************/

CConsolPMSInvoiceGroup::CConsolPMSInvoiceGroup()
{
	Reset();
}

/**********************************************************************/

void CConsolPMSInvoiceGroup::Reset()
{
	m_nGroupNo = 0;
	m_dQty = 0.0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CConsolPMSInvoiceGroup::Add ( CConsolPMSInvoiceGroup& source )
{
	m_dQty += source.m_dQty;
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CConsolPMSInvoiceGroup::Compare( CConsolPMSInvoiceGroup& source, int nHint )
{
	if ( m_nGroupNo != source.m_nGroupNo )
		return ( m_nGroupNo > source.m_nGroupNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPMSInvoiceArray::CPMSInvoiceArray()
{
	Reset();
}

/**********************************************************************/

void CPMSInvoiceArray::Reset()
{
	m_dInvoiceTaxContent = 0.0;
	m_dInvoiceDiscountBuffer = 0.0;

	for (int n = 0; n <= MAX_TAX_BANDS; n++)
	{
		m_dInvoiceTaxableBuffer[n] = 0.0;
	}

	m_arrayInvoiceItems.RemoveAll();
	m_arrayPaymentTotals.RemoveAll();
	m_arrayAllowanceTotals.RemoveAll();
}

/**********************************************************************/

void CPMSInvoiceArray::CopyInvoiceItems( CPMSInvoiceArray& source )
{
	Reset();

	for ( int n = 0; n < source.GetInvoiceItemCount(); n++ )
	{
		CPMSInvoiceLineInfo info;
		source.GetInvoiceItem( n, info );
		m_arrayInvoiceItems.Add( info );
	}
}

/**********************************************************************/

int CPMSInvoiceArray::GetInvoiceItemCount()
{
	return m_arrayInvoiceItems.GetSize();
}

/**********************************************************************/

void CPMSInvoiceArray::GetInvoiceItem( int n, CPMSInvoiceLineInfo& info )
{
	if ( ( n >= 0 ) && ( n < GetInvoiceItemCount() ) )
		info = m_arrayInvoiceItems.GetAt(n);
}

/**********************************************************************/

void CPMSInvoiceArray::SetInvoiceItem( int n, CPMSInvoiceLineInfo& info )
{
	if ( ( n >= 0 ) && ( n < GetInvoiceItemCount() ) )
		m_arrayInvoiceItems.SetAt(n, info);
}

/**********************************************************************/

void CPMSInvoiceArray::RemoveInvoiceItem( int n )
{
	if ( ( n >= 0 ) && ( n < GetInvoiceItemCount() ) )
		m_arrayInvoiceItems.RemoveAt(n);
}

/**********************************************************************/

int CPMSInvoiceArray::AddInvoiceItem( CPMSInvoiceLineInfo& item )
{
	int nPos;
	if ( FindInvoiceItem( item, nPos ) == FALSE )
		m_arrayInvoiceItems.InsertAt( nPos, item );
	else if ( m_arrayInvoiceItems[ nPos ].IsBlankItemLine() == TRUE )
		m_arrayInvoiceItems.SetAt( nPos, item );
	else
	{
		m_arrayInvoiceItems[ nPos ].m_dBasePluQty += item.m_dBasePluQty;
		m_arrayInvoiceItems[ nPos ].m_dEposPluQty += item.m_dEposPluQty;
		m_arrayInvoiceItems[ nPos ].m_dValue += item.m_dValue;
		m_arrayInvoiceItems[ nPos ].m_dDiscount += item.m_dDiscount;
		m_arrayInvoiceItems[ nPos ].m_dAllowance += item.m_dAllowance;
	}

	return nPos;
}

/**********************************************************************/

bool CPMSInvoiceArray::FindInvoiceItem( CPMSInvoiceLineInfo& item, int& nPos )
{
	int nStart = 0;
	int nEnd = GetInvoiceItemCount() - 1;

	while( nStart <= nEnd )
	{
		nPos = ( nStart + nEnd ) / 2;

		if ( m_arrayInvoiceItems[ nPos ].m_nSection > item.m_nSection )
			nEnd = nPos - 1;
		else if ( m_arrayInvoiceItems[ nPos ].m_nSection < item.m_nSection )
			nStart = nPos + 1;
		else if ( m_arrayInvoiceItems[ nPos ].m_dateSale > item.m_dateSale )
			nEnd = nPos - 1;
		else if ( m_arrayInvoiceItems[ nPos ].m_dateSale < item.m_dateSale )
			nStart = nPos + 1;
		else if ( m_arrayInvoiceItems[ nPos ].m_nTransactionNo > item.m_nTransactionNo )
			nEnd = nPos - 1;
		else if ( m_arrayInvoiceItems[ nPos ].m_nTransactionNo < item.m_nTransactionNo )
			nStart = nPos + 1;
		else if ( m_arrayInvoiceItems[ nPos ].m_nLineType > item.m_nLineType )
			nEnd = nPos - 1;
		else if ( m_arrayInvoiceItems[ nPos ].m_nLineType < item.m_nLineType )
			nStart = nPos + 1;
		else if ( m_arrayInvoiceItems[ nPos ].m_nLineType == PMS_LINE_TRAN )
			return TRUE;
		else if ( m_arrayInvoiceItems[ nPos ].m_nLineType == PMS_LINE_SDISC )
			return TRUE;
		else if ( m_arrayInvoiceItems[ nPos ].m_nLineType == PMS_LINE_LDISC )
			return TRUE;
		else if ( m_arrayInvoiceItems[ nPos ].m_nBasePluNo > item.m_nBasePluNo )
			nEnd = nPos - 1;
		else if ( m_arrayInvoiceItems[ nPos ].m_nBasePluNo < item.m_nBasePluNo )
			nStart = nPos + 1;
		else if ( m_arrayInvoiceItems[ nPos ].m_nModifier > item.m_nModifier )
			nEnd = nPos - 1;
		else if ( m_arrayInvoiceItems[ nPos ].m_nModifier < item.m_nModifier )
			nStart = nPos + 1;
		else if ( m_arrayInvoiceItems[ nPos ].m_nLevel > item.m_nLevel )
			nEnd = nPos - 1;
		else if ( m_arrayInvoiceItems[ nPos ].m_nLevel < item.m_nLevel )
			nStart = nPos + 1;
		else
			return TRUE;
	}

	nPos = nStart;
	return FALSE;
}

/**********************************************************************/
//PAYMENT TOTALS
/**********************************************************************/

int CPMSInvoiceArray::GetPaymentTotalCount()
{
	return m_arrayPaymentTotals.GetSize();
}

/**********************************************************************/

void CPMSInvoiceArray::GetPaymentTotal( int n, CPMSInvoiceLineInfo& info )
{
	if ( ( n >= 0 ) && ( n < GetPaymentTotalCount() ) )
		info = m_arrayPaymentTotals.GetAt(n);
}

/**********************************************************************/

void CPMSInvoiceArray::RemovePaymentTotal( int n )
{
	if ( ( n >= 0 ) && ( n < GetPaymentTotalCount() ) )
		m_arrayPaymentTotals.RemoveAt(n);
}

/**********************************************************************/

int CPMSInvoiceArray::AddPaymentTotal( CPMSInvoiceLineInfo& item )
{
	int nPos;
	if ( FindPaymentTotal( item, nPos ) == FALSE )
		m_arrayPaymentTotals.InsertAt( nPos, item );
	else
		m_arrayPaymentTotals[ nPos ].m_dValue += item.m_dValue;
		
	return nPos;
}

/**********************************************************************/

bool CPMSInvoiceArray::FindPaymentTotal( CPMSInvoiceLineInfo& item, int& nPos )
{
	int nStart = 0;
	int nEnd = GetPaymentTotalCount() - 1;

	while( nStart <= nEnd )
	{
		nPos = ( nStart + nEnd ) / 2;

		if ( m_arrayPaymentTotals[ nPos ].m_nSection > item.m_nSection )
			nEnd = nPos - 1;
		else if ( m_arrayPaymentTotals[ nPos ].m_nSection < item.m_nSection )
			nStart = nPos + 1;
		else if ( m_arrayPaymentTotals[ nPos ].m_nBasePluNo > item.m_nBasePluNo )
			nEnd = nPos - 1;
		else if ( m_arrayPaymentTotals[ nPos ].m_nBasePluNo < item.m_nBasePluNo )
			nStart = nPos + 1;
		else
			return TRUE;
	}

	nPos = nStart;
	return FALSE;
}

/**********************************************************************/
//ALLOWANCE TOTALS
/**********************************************************************/

int CPMSInvoiceArray::GetAllowanceTotalCount()
{
	return m_arrayAllowanceTotals.GetSize();
}

/**********************************************************************/

void CPMSInvoiceArray::GetAllowanceTotal( int n, CPMSInvoiceLineInfo& info )
{
	if ( ( n >= 0 ) && ( n < GetAllowanceTotalCount() ) )
		info = m_arrayAllowanceTotals.GetAt(n);
}

/**********************************************************************/

void CPMSInvoiceArray::RemoveAllowanceTotal( int n )
{
	if ( ( n >= 0 ) && ( n < GetAllowanceTotalCount() ) )
		m_arrayAllowanceTotals.RemoveAt(n);
}

/**********************************************************************/

int CPMSInvoiceArray::AddAllowanceTotal( CPMSInvoiceLineInfo& item )
{
	int nPos;
	if ( FindAllowanceTotal( item, nPos ) == FALSE )
		m_arrayAllowanceTotals.InsertAt( nPos, item );
	else
		m_arrayAllowanceTotals[ nPos ].m_dAllowance += item.m_dAllowance;
		
	return nPos;
}

/**********************************************************************/

bool CPMSInvoiceArray::FindAllowanceTotal( CPMSInvoiceLineInfo& item, int& nPos )
{
	int nStart = 0;
	int nEnd = GetAllowanceTotalCount() - 1;

	while( nStart <= nEnd )
	{
		nPos = ( nStart + nEnd ) / 2;

		if ( m_arrayAllowanceTotals[ nPos ].m_nBasePluNo > item.m_nBasePluNo )
			nEnd = nPos - 1;
		else if ( m_arrayAllowanceTotals[ nPos ].m_nBasePluNo < item.m_nBasePluNo )
			nStart = nPos + 1;
		else
			return TRUE;
	}

	nPos = nStart;
	return FALSE;
}

/**********************************************************************/

void CPMSInvoiceArray::PurgeBlankItemLines()
{
	for ( int n = GetInvoiceItemCount() - 1; n >= 0; n-- )
	{
		CPMSInvoiceLineInfo info;
		GetInvoiceItem(n, info );

		if ( info.IsBlankItemLine() == TRUE )
			RemoveInvoiceItem(n);
	}
}

/**********************************************************************/

void CPMSInvoiceArray::PurgeBlankPaymentLines()
{
	for ( int n = GetPaymentTotalCount() - 1; n >= 0; n-- )
	{
		CPMSInvoiceLineInfo info;
		GetPaymentTotal(n, info );

		if ( info.IsBlankItemLine() == TRUE )
			RemovePaymentTotal(n);
	}
}

/**********************************************************************/

void CPMSInvoiceArray::PurgeBlankAllowanceLines()
{
	for ( int n = GetAllowanceTotalCount() - 1; n >= 0; n-- )
	{
		CPMSInvoiceLineInfo info;
		GetAllowanceTotal(n, info );

		if ( info.IsBlankItemLine() == TRUE )
			RemoveAllowanceTotal(n);
	}
}

/**********************************************************************/

void CPMSInvoiceArray::PurgeEmptyTransactions()
{
	for ( int n = GetInvoiceItemCount() - 2; n >= 0; n-- )
	{
		CPMSInvoiceLineInfo info1 ,info2;
		GetInvoiceItem( n, info1 );
		GetInvoiceItem( n + 1, info2 );

		if ( ( info1.m_nLineType == PMS_LINE_TRAN ) && ( info2.m_nLineType == PMS_LINE_TRAN ) )
			RemoveInvoiceItem(n);
	}
}

/**********************************************************************/

void CPMSInvoiceArray::BuildInvoiceArray( int nBookingId, int nAccountNo, int nInvoiceType )
{
	Reset();

	CPMSBookingCSVRecord BookingRecord;
	BookingRecord.SetBookingId( nBookingId );

	if ( 1 == nAccountNo )
	{
		switch( nInvoiceType )
		{
		case PMS_INVOICETYPE_ITEM:
		case PMS_INVOICETYPE_GROUP:
		case PMS_INVOICETYPE_TRANSACTION:
		case PMS_INVOICETYPE_EPOS_REPORT:
			{
				int nBookingIdx;
				if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == TRUE )
					DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
			}
			break;
		}
	}

	BuildInvoiceArray( BookingRecord, nAccountNo, nInvoiceType );
}

/**********************************************************************/

void CPMSInvoiceArray::BuildInvoiceArray( CPMSBookingCSVRecord& BookingRecord, int nAccountNo, int nInvoiceType, bool bDBBCredit )
{
	if ( 1 == nAccountNo )
	{
		switch( nInvoiceType )
		{
		case PMS_INVOICETYPE_EPOS_REPORT:
			{
				double dRoomSpend;
				double dExtraBedSpend;
				int nStartDay, nStayLength;
				BookingRecord.GetAccomodationSpend( dRoomSpend, dExtraBedSpend, nStartDay, nStayLength );

				double dRate = BookingRecord.GetRoomRate();

				if ( PMS_BOOKING_RATE_FREE != BookingRecord.GetRoomRateType() )
				{
					for ( int n = 0; n < nStayLength; n++ )
					{
						CPMSInvoiceLineInfo infoTrans, infoAccom;
						BookingRecord.GetAccomodationInvoiceLine( nStartDay + n, 1, infoTrans, infoAccom ); 
						
						CPMSInvoiceLineInfo infoBreakfast;
						bool bBreakfast = BookingRecord.SplitAccommodationLine( infoAccom, infoBreakfast );
					
						AddInvoiceItem( infoTrans );
						AddInvoiceItem( infoAccom );

						switch ( BookingRecord.GetTariffType() )
						{
						case PMS_TARIFFTYPE_EXTRABED:
						case PMS_TARIFFTYPE_EXTRACOT:
							{
								CPMSInvoiceLineInfo infoExtraBed;
								BookingRecord.GetExtraBedInvoiceLine( nStartDay + n, 1, infoTrans, infoExtraBed );
								AddInvoiceItem( infoExtraBed );
							}
							break;
						}

						if ( TRUE == bBreakfast )
							AddInvoiceItem( infoBreakfast );		
					}
				}
			}
			break;

		case PMS_INVOICETYPE_ITEM:
		case PMS_INVOICETYPE_GROUP:
		case PMS_INVOICETYPE_TRANSACTION:
			{
				if ( PMS_BOOKING_RATE_FREE != BookingRecord.GetRoomRateType() )
				{		
					CArray<CPMSBookingPartInfo,CPMSBookingPartInfo> arrayParts;
					BookingRecord.GetAccomodationTaxPeriods( arrayParts );

					for ( int n = 0; n < arrayParts.GetSize(); n++ )
					{
						int nStartDay = arrayParts[n].GetStartDay();
						int nNights = arrayParts[n].GetNights();

						CPMSInvoiceLineInfo infoTrans, infoAccom;
						BookingRecord.GetAccomodationInvoiceLine( nStartDay, nNights, infoTrans, infoAccom );
						AddInvoiceItem( infoTrans );
						AddInvoiceItem( infoAccom );

						switch( BookingRecord.GetTariffType() )
						{
						case PMS_TARIFFTYPE_EXTRABED:
						case PMS_TARIFFTYPE_EXTRACOT:
							{
								CPMSInvoiceLineInfo infoExtraBed;
								BookingRecord.GetExtraBedInvoiceLine( nStartDay, nNights, infoTrans, infoExtraBed );
								AddInvoiceItem( infoExtraBed );
							}
							break;
						}
					}
				}
			}
			break;
		}

		switch( nInvoiceType )
		{
		case PMS_INVOICETYPE_ITEM:
		case PMS_INVOICETYPE_GROUP:
		case PMS_INVOICETYPE_TRANSACTION:
		case PMS_INVOICETYPE_EPOS_REPORT:
			{
				CArray<CPMSInvoiceLineInfo,CPMSInvoiceLineInfo> arrayDeposit;
				BookingRecord.GetDepositInvoiceArray( arrayDeposit );

				for ( int n = 0; n < arrayDeposit.GetSize(); n++ )
				{
					CPMSInvoiceLineInfo infoDeposit = arrayDeposit[n];
					AddInvoiceItem( infoDeposit );

					if ( PMS_LINE_DEPOSIT == infoDeposit.m_nLineType )
						AddPaymentTotal( infoDeposit );
				}
			}
			break;
		}
	}
	
	CString strTransFile;
	strTransFile.Format( "%s\\%d.%d",
		PMSOptions.GetPMSFolderSales(),
		BookingRecord.GetBookingId(),
		nAccountNo );

	{
		CSSFile fileTrans;
		if ( fileTrans.Open( strTransFile, "rb" ) == TRUE )
			BuildInvoiceArrayItem( fileTrans, nInvoiceType, BookingRecord.GetBookingId() );
	}

	int nDinnerTaxBand = 0;

	if ( TRUE == bDBBCredit )
		if ( BookingRecord.CheckDinnerSettings( nDinnerTaxBand ) == FALSE )
			bDBBCredit = FALSE;

	if ( TRUE == bDBBCredit )
	{
		switch( nInvoiceType )
		{
		case PMS_INVOICETYPE_GROUP:
		case PMS_INVOICETYPE_TRANSACTION:
			PrepareDBBCreditsGroup( BookingRecord, nDinnerTaxBand );
			break;

		case PMS_INVOICETYPE_ITEM:
		case PMS_INVOICETYPE_EPOS_REPORT:
		default:
			{
				CReportConsolidationArray<CConsolidatedDoubleByString> arraySales;
				double dDummy1, dDummy2, dDummy3;
				PrepareDBBCreditsItem( BookingRecord, nDinnerTaxBand, dDummy1, dDummy2, dDummy3, arraySales, TRUE );
			}
			break;
		}
	}

	ProcessInvoiceTax();

	switch( nInvoiceType )
	{
	case PMS_INVOICETYPE_ITEM:
	case PMS_INVOICETYPE_EPOS_REPORT:
		break;

	case PMS_INVOICETYPE_GROUP:
		FinaliseInvoiceArrayGroup();
		break;

	case PMS_INVOICETYPE_TRANSACTION:
		FinaliseInvoiceArrayTransaction();
		break;
	}

	PurgeBlankItemLines();
	PurgeBlankPaymentLines();
	PurgeBlankAllowanceLines();

	switch( nInvoiceType )
	{
	case PMS_INVOICETYPE_EDIT_SALES:
	case PMS_INVOICETYPE_EDIT_PAYMENTS:
		CPMSInvoiceLineInfo infoNewSale;
		infoNewSale.m_nLineType = PMS_LINE_TRAN;
		infoNewSale.m_dateSale = COleDateTime::GetCurrentTime();
		infoNewSale.m_dEposPluQty = 0.0;
		infoNewSale.m_dBasePluQty = 0.0;
		infoNewSale.m_dValue = 0.0;
		infoNewSale.m_dDiscount = 0.0;
		infoNewSale.m_dAllowance = 0.0;
		AddInvoiceItem( infoNewSale );
		break;
	}

	if ( PMS_INVOICETYPE_TRANSACTION != nInvoiceType )
		PurgeEmptyTransactions();
}

/**********************************************************************/

void CPMSInvoiceArray::BuildInvoiceArrayItem( CSSFile& fileTrans, int nInvoiceType, int nBookingId )
{
	bool bInclude = FALSE;
	
	CPMSInvoiceLineInfo infoTransaction;
	CPMSInvoiceLineInfo infoSale;

	CString strBuffer;
	while( fileTrans.ReadString( strBuffer ) == TRUE )
	{
		infoSale.m_dBasePluQty = 0.0;
		infoSale.m_dEposPluQty = 0.0;
		infoSale.m_dValue = 0.0;
		infoSale.m_dDiscount = 0.0;
		infoSale.m_dAllowance = 0.0;

		CString strLineType = strBuffer.SpanExcluding( "," );

		bool bCheckedLine = FALSE;

		if ( strLineType == "$TRAN" )
		{
			bInclude = ProcessTransactionLine( strBuffer, infoSale );

			if ( TRUE == bInclude )
			{
				infoTransaction = infoSale;
				AddInvoiceItem( infoSale );
			}

			bCheckedLine = TRUE;
		}
		else if ( strLineType == "$TRAN_EDIT" )
		{
			bInclude = ProcessTransactionEditLine( strBuffer, infoSale );

			if ( TRUE == bInclude )
			{
				infoTransaction = infoSale;
				AddInvoiceItem( infoSale );
			}

			bCheckedLine = TRUE;
		}

		if ( ( TRUE == bCheckedLine ) || ( FALSE == bInclude ) )
			continue;

		if ( nInvoiceType != PMS_INVOICETYPE_EDIT_PAYMENTS )
		{
			if  ( strLineType == "$ITEM" )
			{
				if ( TRUE == ProcessItemLine( strBuffer, infoSale ) )
					AddInvoiceItem( infoSale );
			}
			else if  ( strLineType == "$IDISC" )
			{
				if ( TRUE == ProcessItemDiscountLine( strBuffer, infoSale ) )
					AddInvoiceItem( infoSale );
			}
			else if  ( strLineType == "$SDISC" )
			{
				if ( TRUE == ProcessSubtotalDiscountLine( strBuffer, infoSale, PMS_LINE_SDISC ) )
					AddInvoiceItem( infoSale );
			}
			else if ( strLineType == "$LDISC" )
			{
				if ( TRUE == ProcessSubtotalDiscountLine( strBuffer, infoSale, PMS_LINE_LDISC ) )
					AddInvoiceItem( infoSale );
			}
			else if ( strLineType == "$ITEM_EDIT" )
			{
				if ( TRUE == ProcessItemEditLine( strBuffer, infoSale, PMS_LINE_SALE ) )
					AddInvoiceItem( infoSale );
			}
			else if ( strLineType == "$IDISC_EDIT" )
			{
				if ( TRUE == ProcessItemDiscountEditLine( strBuffer, infoSale ) )
					AddInvoiceItem( infoSale );
			}
			else if ( strLineType == "$SDISC_EDIT" )
			{
				if ( TRUE == ProcessSubtotalDiscountEditLine( strBuffer, infoSale, PMS_LINE_SDISC ) )
					AddInvoiceItem( infoSale );
			}
			else if ( strLineType == "$LDISC_EDIT" )
			{
				if ( TRUE == ProcessSubtotalDiscountEditLine( strBuffer, infoSale, PMS_LINE_LDISC ) )
					AddInvoiceItem( infoSale );
			}
			else if ( strLineType == "$REFUND_EDIT" )
			{
				if ( TRUE == ProcessItemEditLine( strBuffer, infoSale, PMS_LINE_REFUND ) )
					AddInvoiceItem( infoSale );
			}
			else if ( strLineType == "$ALLOWANCE_EDIT" )
			{
				if ( TRUE == ProcessItemEditLine( strBuffer, infoSale, PMS_LINE_ALLOWANCE ) )
				{
					AddInvoiceItem( infoSale );
					AddAllowanceTotal( infoSale );
				}
			}
		}

		if ( nInvoiceType != PMS_INVOICETYPE_EDIT_SALES )
		{
			if ( strLineType == "$PAYM_EDIT" )
			{
				if ( TRUE == ProcessPaymentEditLine( strBuffer, infoSale ) )
				{
					AddInvoiceItem( infoSale );
					AddPaymentTotal( infoSale );
				}
			}
		}
	}
}

/**********************************************************************/

void CPMSInvoiceArray::FinaliseInvoiceArrayGroup()
{
	CPMSInvoiceArray arrayTemp;
	CPMSInvoiceLineInfo infoTransaction;
	CReportConsolidationArray<CConsolPMSInvoiceGroup> arraySales;

	for ( int n = 0; n < GetInvoiceItemCount(); n++ )
	{
		CPMSInvoiceLineInfo infoLine;
		GetInvoiceItem( n, infoLine );

		switch( infoLine.m_nLineType )
		{
		case PMS_LINE_TRAN:
			ProcessGroupTransaction( arrayTemp, infoTransaction, arraySales );
			infoTransaction = infoLine;
			arraySales.RemoveAll();
			break;

		case PMS_LINE_SALE:
		case PMS_LINE_REFUND:
			{
				CString strGroupNo;
				strGroupNo.Format( "%3.3d", infoLine.m_nGroupNo );
			
				if ( PMS_LINE_REFUND != infoLine.m_nLineType )
				{
					if ( CPriceHelpers::CompareDoubles( infoLine.m_dValue, 0.0, 3 ) != 0 )
					{
						CConsolPMSInvoiceGroup infoGroup;
						infoGroup.m_nGroupNo = infoLine.m_nGroupNo;
						infoGroup.m_dQty = infoLine.m_dBasePluQty;
						infoGroup.m_dVal = infoLine.m_dValue;
						arraySales.Consolidate( infoGroup );
					}

					if ( CPriceHelpers::CompareDoubles( infoLine.m_dDiscount, 0.0, 3 ) != 0 )
					{
						CConsolPMSInvoiceGroup infoGroup;
						infoGroup.m_nGroupNo = infoLine.m_nGroupNo;
						infoGroup.m_dQty = 0.0;
						infoGroup.m_dVal = infoLine.m_dDiscount;
						arraySales.Consolidate( infoGroup );
					}
				}
				else
				{
					if ( CPriceHelpers::CompareDoubles( infoLine.m_dValue, 0.0, 3 ) != 0 )
					{
						CConsolPMSInvoiceGroup infoGroup;
						infoGroup.m_nGroupNo = infoLine.m_nGroupNo;
						infoGroup.m_dQty = -infoLine.m_dBasePluQty;
						infoGroup.m_dVal = -infoLine.m_dValue;
						arraySales.Consolidate( infoGroup );
					}
				}
			}
			break;	

		//INCLUDE THESE LINES AS IS, DO NOT CONSOLIDATE BY GROUP
		case PMS_LINE_ACCOM:		//ACCOMMODATION COST
		case PMS_LINE_EXTRABED:		//EXTRA BED SUPPLEMENT
		case PMS_LINE_EXTRACOT:		//EXTRA COT SUPPLEMENT
		case PMS_LINE_BREAKFAST:	//BREAKFAST VALUE
		case PMS_LINE_DINNER:		//DINNER SUPPLEMENT
		case PMS_LINE_SDISC:		//SUBTOTAL DISCOUNT
		case PMS_LINE_LDISC:		//LOYALTY DISCOUNT
			arrayTemp.AddInvoiceItem( infoLine );
			break;

		//DO NOT INCLUDE THESE LINES IN THE MAIN BODY OF THE INVOICE 
		case PMS_LINE_ALLOWANCE:	//MANUAL ALLOWANCE
		case PMS_LINE_PAYMENT:		//PAYMENT
		case PMS_LINE_DEPOSIT:		//DEPOSIT
			break;
		}
	}

	ProcessGroupTransaction( arrayTemp, infoTransaction, arraySales );

	m_arrayInvoiceItems.RemoveAll();

	for ( int n = 0; n < arrayTemp.GetInvoiceItemCount(); n++ )
	{
		CPMSInvoiceLineInfo info;
		arrayTemp.GetInvoiceItem(n, info);
		m_arrayInvoiceItems.Add( info );
	}
}

/**********************************************************************/

void CPMSInvoiceArray::ProcessGroupTransaction( CPMSInvoiceArray& arrayInvoice, CPMSInvoiceLineInfo& infoTransaction, CReportConsolidationArray<CConsolPMSInvoiceGroup>& arraySales )
{
	//THE TRANSACTION HEADER MUST BE RETAINED, EVEN IF THERE ARE NO GROUP SALES,
	//AS IT MAY RELATE TO SUBTOTAL DISCOUNTS, DEPOSITS OR ACCOMMODATION COSTS
	arrayInvoice.AddInvoiceItem( infoTransaction );

	for ( int g = 0; g < arraySales.GetSize(); g++ )
	{
		CConsolPMSInvoiceGroup infoGroup;
		arraySales.GetAt( g, infoGroup );

		infoTransaction.m_nLineType = PMS_LINE_SALE;
		infoTransaction.m_nBasePluNo = infoGroup.m_nGroupNo;
		infoTransaction.m_dBasePluQty = infoGroup.m_dQty;
		infoTransaction.m_dValue = infoGroup.m_dVal;

		arrayInvoice.AddInvoiceItem( infoTransaction );
	}
}

/**********************************************************************/

void CPMSInvoiceArray::FinaliseInvoiceArrayTransaction()
{
	CPMSInvoiceArray arrayTemp;
	
	CPMSInvoiceLineInfo infoTransaction;
	bool bGotTransaction = FALSE;

	for ( int n = 0; n < GetInvoiceItemCount(); n++ )
	{
		CPMSInvoiceLineInfo infoLine;
		GetInvoiceItem( n, infoLine );

		switch( infoLine.m_nLineType )
		{
		case PMS_LINE_TRAN:
			{
				if ( TRUE == bGotTransaction )
					arrayTemp.AddInvoiceItem( infoTransaction );
			
				infoTransaction = infoLine;
				infoTransaction.m_dValue = 0.0;
				
				bGotTransaction = TRUE;
			}
			break;

		case PMS_LINE_SALE:
			infoTransaction.m_dValue += infoLine.m_dValue;
			infoTransaction.m_dValue -= infoLine.m_dDiscount;
			break;

		case PMS_LINE_REFUND:
			infoTransaction.m_dValue -= infoLine.m_dValue;
			break;

		case PMS_LINE_SDISC:
		case PMS_LINE_LDISC:
			infoTransaction.m_dValue -= infoLine.m_dDiscount;
			break;

		//INCLUDE THESE LINES AS IS, DO NOT CONSOLIDATE BY TRANSACTION
		case PMS_LINE_ACCOM:		//ACCOMODATION COST
		case PMS_LINE_EXTRABED:		//EXTRA BED SUPPLEMENT
		case PMS_LINE_EXTRACOT:		//EXTRA COT SUPPLEMENT
		case PMS_LINE_BREAKFAST:	//BREAKFAST VALUE
		case PMS_LINE_DINNER:		//DINNER SUPPLEMENT
			arrayTemp.AddInvoiceItem( infoLine );
			break;

		//DO NOT INCLUDE THESE LINES IN THE MAIN BODY OF THE INVOICE 
		case PMS_LINE_ALLOWANCE:	//MANUAL ALLOWANCE
		case PMS_LINE_PAYMENT:		//PAYMENT
		case PMS_LINE_DEPOSIT:		//DEPOSIT
			break;
		}
	}

	if ( TRUE == bGotTransaction )
		arrayTemp.AddInvoiceItem( infoTransaction );

	m_arrayInvoiceItems.RemoveAll();

	for ( int n = 0; n < arrayTemp.GetInvoiceItemCount(); n++ )
	{
		CPMSInvoiceLineInfo info;
		arrayTemp.GetInvoiceItem(n, info);
		m_arrayInvoiceItems.Add( info );
	}
}

/**********************************************************************/

void CPMSInvoiceArray::ProcessInvoiceTax()
{
	CPMSInvoiceLineInfo infoTransaction;
	bool bGotTransaction = FALSE;

	for (int n = 0; n <= MAX_TAX_BANDS; n++)
	{
		m_dInvoiceTaxableBuffer[n] = 0.0;
	}

	m_dInvoiceDiscountBuffer = 0.0;
	m_dInvoiceTaxContent = 0.0;

	for ( int n = 0; n < GetInvoiceItemCount(); n++ )
	{
		CPMSInvoiceLineInfo infoLine;
		GetInvoiceItem(n, infoLine);

		int nTaxBand = 1;

		switch( infoLine.m_nLineType )
		{
		case PMS_LINE_SALE:
		case PMS_LINE_REFUND:
		case PMS_LINE_ALLOWANCE:
		case PMS_LINE_ACCOM:
		case PMS_LINE_EXTRABED:
		case PMS_LINE_EXTRACOT:
		case PMS_LINE_DINNER:
		case PMS_LINE_BREAKFAST:
			nTaxBand = infoLine.m_nTaxBand;
			break;
		}

		if ( ( nTaxBand < 0 ) || ( nTaxBand > MAX_TAX_BANDS ) )
			nTaxBand = 0;

		switch( infoLine.m_nLineType )
		{
		case PMS_LINE_TRAN:
			{
				if ( TRUE == bGotTransaction )
					ProcessTransactionTax( infoTransaction );
		
				GetInvoiceItem( n, infoTransaction );
				bGotTransaction = TRUE;
			}
			break;

		case PMS_LINE_SALE:
		case PMS_LINE_ACCOM:
			m_dInvoiceTaxableBuffer[ nTaxBand ] += infoLine.m_dValue;
			m_dInvoiceTaxableBuffer[ nTaxBand ] -= infoLine.m_dDiscount;
			break;

		case PMS_LINE_DINNER:
		case PMS_LINE_BREAKFAST:
		case PMS_LINE_EXTRABED:
		case PMS_LINE_EXTRACOT:
			m_dInvoiceTaxableBuffer[ nTaxBand ] += infoLine.m_dValue;
			break;

		case PMS_LINE_REFUND:
			m_dInvoiceTaxableBuffer[ nTaxBand ] -= infoLine.m_dValue;
			break;

		case PMS_LINE_ALLOWANCE:
			m_dInvoiceTaxableBuffer[ nTaxBand ] -= infoLine.m_dAllowance;
			break;
		
		case PMS_LINE_SDISC:
		case PMS_LINE_LDISC:
			m_dInvoiceDiscountBuffer += infoLine.m_dDiscount;
			break;
		}
	}

	if ( TRUE == bGotTransaction )
		ProcessTransactionTax( infoTransaction );
}

/**********************************************************************/

void CPMSInvoiceArray::ProcessTransactionTax( CPMSInvoiceLineInfo& infoTransaction )
{
	if ( CPriceHelpers::CompareDoubles( m_dInvoiceDiscountBuffer, 0.0, 3 ) != 0 )
	{
		double dTotal = 0.0;
		for (int n = 0; n <= MAX_TAX_BANDS; n++)
		{
			dTotal += m_dInvoiceTaxableBuffer[n];
		}

		if (CPriceHelpers::CompareDoubles(dTotal, 0.0, 3) == 0)
		{
			m_dInvoiceTaxableBuffer[1] -= m_dInvoiceDiscountBuffer;
		}
		else
		{
			for ( int n = 1; n <= MAX_TAX_BANDS; n++ )
			{
				double dShare = ( m_dInvoiceTaxableBuffer[n] * m_dInvoiceDiscountBuffer ) / dTotal;
				m_dInvoiceTaxableBuffer[n] -= dShare;
			}
		}
	}

	CString strDate;
	strDate.Format( "%4.4d%2.2d%2.2d",
		infoTransaction.m_dateSale.GetYear(),
		infoTransaction.m_dateSale.GetMonth(),
		infoTransaction.m_dateSale.GetDay() );

	for ( int n = 1; n <= MAX_TAX_BANDS; n++ )
	{
		double dVal = m_dInvoiceTaxableBuffer[n];
		
		int nDateCode = DataManager.HistoricalTaxRates.GetDateCode( strDate );
		double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( nDateCode, n );
		
		CReportHelpers ReportHelpers;
		m_dInvoiceTaxContent += dVal - ( ReportHelpers.CalcNonTax ( dVal, dTaxRate ) );
	}

	m_dInvoiceDiscountBuffer = 0.0;

	for (int n = 0; n <= MAX_TAX_BANDS; n++)
	{
		m_dInvoiceTaxableBuffer[n] = 0.0;
	}
}

/**********************************************************************/

bool CPMSInvoiceArray::ProcessTransactionLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale )
{			
	CCSVPMSTermLine csv( strBuffer );

	CString strDate = csv.GetTransactionDate();
	if ( strDate.GetLength() != 8 )
		return FALSE;

	CString strTime = csv.GetTransactionTime();
	if ( strTime.GetLength() != 8 )
		return FALSE;

	infoSale.m_dateSale = COleDateTime(
		2000 + atoi ( strDate.Right(2) ),
		atoi( strDate.Mid(3,2) ),
		atoi( strDate.Left(2) ),
		atoi( strTime.Left(2) ),
		atoi( strTime.Mid(3,2) ),
		atoi( strTime.Right(2) ) );

	if ( infoSale.m_dateSale.m_status != COleDateTime::valid )
		return FALSE;

	infoSale.m_nLineType = PMS_LINE_TRAN;
	infoSale.m_nTransactionNo = csv.GetTransactionNumber();
	infoSale.m_dValue = ( double ) csv.GetTransactionValue() / 100.0;
	infoSale.m_dAllowance = 0.0;
	return TRUE;
}

/**********************************************************************/

bool CPMSInvoiceArray::ProcessItemLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale )
{			
	CCSVPMSTermLine csv( strBuffer );

	CSQLPluNoInfo infoPluNo;
	infoPluNo.m_nEposPluNo = csv.GetItemPluNo();
	::ProcessPluNo( infoPluNo );
				
	if ( FALSE == infoPluNo.m_bValid )
		return FALSE;

	CString strDescription = "**Unknown";
	
	int nPluIdx = 0;
	int nPluDeptNo = 0;
	int nTaxBand = 0;

	if ( DataManager.Plu.FindPluByNumber( infoPluNo.m_nBasePluNo, nPluIdx ) == TRUE )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		strDescription = PluRecord.GetModifierReportText( infoPluNo.m_nModifier );
		nTaxBand = PluRecord.GetModifierTaxBandIntForReport( infoPluNo.m_nModifier );
		nPluDeptNo = PluRecord.GetModifierDeptNoForReport( infoPluNo.m_nModifier );
	}

	if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
		nTaxBand = csv.GetItemNumericTaxBand();
	
	double dQty = csv.GetItemQty();
	int nGroupNo = 0;

	CString strGroupNo = csv.GetItemGroupNoString();
	if ( strGroupNo != "" )
		nGroupNo = csv.GetItemGroupNoInt();
	else
	{
		int nDeptIdx;
		if ( DataManager.Department.FindDeptByNumber( nPluDeptNo, nDeptIdx ) == TRUE )
		{
			CDepartmentCSVRecord Dept;
			DataManager.Department.GetAt( nDeptIdx, Dept );
			nGroupNo = Dept.GetEposGroup();
		}
	}

	infoSale.m_nBasePluNo = infoPluNo.m_nBasePluNo;
	infoSale.m_nModifier = infoPluNo.m_nModifier;
	infoSale.m_nLevel = csv.GetItemPriceBand();
	infoSale.m_nDeptNo = csv.GetItemDeptNo();
	infoSale.m_nGroupNo = nGroupNo;
	infoSale.m_nTaxBand = nTaxBand;
	infoSale.m_strDescription = strDescription;
	infoSale.m_dEposPluQty = dQty;
	infoSale.m_dBasePluQty = dQty * DataManager.Modifier.GetMultiplierForQty( infoSale.m_nModifier );
	infoSale.m_dValue = csv.GetItemLineValueReport();

	CString strRefund = csv.GetItemRefundFlag();

	if ( strRefund == "R" )
	{
		infoSale.m_nLineType = PMS_LINE_REFUND;
		infoSale.m_dValue *= -1;
	}
	else if ( strRefund == "A" )
	{
		infoSale.m_nLineType = PMS_LINE_ALLOWANCE;
		infoSale.m_dAllowance = infoSale.m_dValue;
		infoSale.m_dAllowance *= -1;
		infoSale.m_dValue = 0.0;
	}
	else
		infoSale.m_nLineType = PMS_LINE_SALE;
	
	return TRUE;
}

/**********************************************************************/

bool CPMSInvoiceArray::ProcessItemDiscountLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale )
{			
	CCSVPMSTermLine csv( strBuffer );

	CSQLPluNoInfo infoPluNo;
	infoPluNo.m_nEposPluNo = csv.GetItemDiscountPluNoInt();
	::ProcessPluNo( infoPluNo );
				
	if ( FALSE == infoPluNo.m_bValid )
		return FALSE;

	CString strDescription = "**Unknown";
	
	int nPluIdx = 0;
	int nTaxBand = 0;
	if ( DataManager.Plu.FindPluByNumber( infoPluNo.m_nBasePluNo, nPluIdx ) == TRUE )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		strDescription = PluRecord.GetModifierReportText( infoPluNo.m_nModifier );
		nTaxBand = PluRecord.GetModifierTaxBandIntForReport( infoPluNo.m_nModifier );
	}

	infoSale.m_nBasePluNo = infoPluNo.m_nBasePluNo;
	infoSale.m_nModifier = infoPluNo.m_nModifier;
	infoSale.m_nLevel = 0;
	infoSale.m_strDescription = strDescription;
	infoSale.m_nTaxBand = nTaxBand;
	infoSale.m_dEposPluQty = 0.0;
	infoSale.m_dBasePluQty = 0.0;
	infoSale.m_dValue = 0.0;
	infoSale.m_dDiscount = ( ( double) csv.GetItemDiscountAmount() / 100.0 ) * -1;

	infoSale.m_nLineType = PMS_LINE_SALE;
	
	return TRUE;
}

/**********************************************************************/

bool CPMSInvoiceArray::ProcessSubtotalDiscountLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale, int nLineType )
{			
	CCSVPMSTermLine csv( strBuffer );

	infoSale.m_nBasePluNo = 0;
	infoSale.m_nModifier = 0;
	infoSale.m_nLevel = 0;
	infoSale.m_strDescription = "";
	infoSale.m_dEposPluQty = 0.0;
	infoSale.m_dBasePluQty = 0.0;
	infoSale.m_dValue = 0.0;
	infoSale.m_dDiscount = ( ( double) csv.GetSubtotalDiscountAmount() / 100.0 ) * -1;
		
	infoSale.m_nLineType = nLineType;
	
	return TRUE;
}

/**********************************************************************/

bool CPMSInvoiceArray::ProcessTransactionEditLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale )
{			
	CPMSTermLineEdit csv( strBuffer );

	CString strDate = csv.GetTransactionDate();
	if ( strDate.GetLength() != 8 )
		return FALSE;

	CString strTime = csv.GetTransactionTime();
	if ( strTime.GetLength() != 8 )
		return FALSE;

	infoSale.m_dateSale = COleDateTime(
		2000 + atoi ( strDate.Right(2) ),
		atoi( strDate.Mid(3,2) ),
		atoi( strDate.Left(2) ),
		atoi( strTime.Left(2) ),
		atoi( strTime.Mid(3,2) ),
		atoi( strTime.Right(2) ) );

	if ( infoSale.m_dateSale.m_status != COleDateTime::valid )
		return FALSE;

	infoSale.m_nLineType = PMS_LINE_TRAN;
	infoSale.m_nTransactionNo = csv.GetTransactionNumber();
	infoSale.m_dValue = ( ( double )csv.GetTransactionValue() ) / 100.0;
	return TRUE;
}

/**********************************************************************/

bool CPMSInvoiceArray::ProcessItemEditLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale, int nLineType )
{			
	CPMSTermLineEdit csv( strBuffer );

	CSQLPluNoInfo infoPluNo;
	infoPluNo.m_nEposPluNo = csv.GetItemPluNo();
	::ProcessPluNo( infoPluNo );
				
	if ( FALSE == infoPluNo.m_bValid )
		return FALSE;

	CString strDescription = "**Unknown";
	
	int nPluIdx = 0;
	int nTaxBand = 0;
	if ( DataManager.Plu.FindPluByNumber( infoPluNo.m_nBasePluNo, nPluIdx ) == TRUE )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		strDescription = PluRecord.GetModifierReportText( infoPluNo.m_nModifier );
		nTaxBand = PluRecord.GetModifierTaxBandIntForReport( infoPluNo.m_nModifier );
	}

	if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
		nTaxBand = csv.GetItemNumericTaxBand();

	infoSale.m_nBasePluNo = infoPluNo.m_nBasePluNo;
	infoSale.m_nModifier = infoPluNo.m_nModifier;
	infoSale.m_nLevel = csv.GetItemPriceBand();
	infoSale.m_nDeptNo = csv.GetItemDeptNo();
	infoSale.m_nGroupNo = csv.GetItemGroupNo();
	infoSale.m_nTaxBand = nTaxBand;
	infoSale.m_strDescription = strDescription;
	infoSale.m_dEposPluQty = csv.GetItemQty();
	infoSale.m_dBasePluQty = csv.GetItemQty() * DataManager.Modifier.GetMultiplierForQty( infoSale.m_nModifier );
	infoSale.m_dValue = ( (double) csv.GetItemLinePrice() ) / 100.0;
	
	infoSale.m_nLineType = nLineType;

	switch( nLineType )
	{
	case PMS_LINE_REFUND:
		infoSale.m_dValue *= -1;
		break;

	case PMS_LINE_ALLOWANCE:
		infoSale.m_dAllowance = infoSale.m_dValue;
		infoSale.m_dAllowance *= -1;
		infoSale.m_dValue = 0.0;
		break;
	}

	infoSale.m_dDiscount = 0.0;

	return TRUE;
}

/**********************************************************************/

bool CPMSInvoiceArray::ProcessItemDiscountEditLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale )
{			
	CPMSTermLineEdit csv( strBuffer );

	CSQLPluNoInfo infoPluNo;
	infoPluNo.m_nEposPluNo = csv.GetItemDiscountPluNo();
	::ProcessPluNo( infoPluNo );
				
	if ( FALSE == infoPluNo.m_bValid )
		return FALSE;

	CString strDescription = "**Unknown";
	
	int nPluIdx = 0;
	int nTaxBand = 0;
	if ( DataManager.Plu.FindPluByNumber( infoPluNo.m_nBasePluNo, nPluIdx ) == TRUE )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		strDescription = PluRecord.GetModifierReportText( infoPluNo.m_nModifier );
		nTaxBand = PluRecord.GetModifierTaxBandIntForReport( infoPluNo.m_nModifier );
	}

	infoSale.m_nBasePluNo = infoPluNo.m_nBasePluNo;
	infoSale.m_nModifier = infoPluNo.m_nModifier;
	infoSale.m_nLevel = 0;
	infoSale.m_strDescription = strDescription;
	infoSale.m_nTaxBand = nTaxBand;
	infoSale.m_dEposPluQty = 0.0;
	infoSale.m_dBasePluQty = 0.0;
	infoSale.m_dValue = 0.0;
	infoSale.m_dDiscount = ( ( (double) csv.GetItemDiscountAmount() ) / 100.0 ) * -1;

	infoSale.m_nLineType = PMS_LINE_SALE;

	return TRUE;
}

/**********************************************************************/

bool CPMSInvoiceArray::ProcessSubtotalDiscountEditLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale, int nLineType )
{			
	CPMSTermLineEdit csv( strBuffer );

	infoSale.m_nBasePluNo = 0;
	infoSale.m_nModifier = 0;
	infoSale.m_nLevel = 0;
	infoSale.m_strDescription = "";
	infoSale.m_dEposPluQty = 0.0;
	infoSale.m_dBasePluQty = 0.0;
	infoSale.m_dValue = 0.0;
	infoSale.m_dDiscount = ( ( (double) csv.GetSubtotalDiscountAmount() ) / 100.0 ) * -1;

	infoSale.m_nLineType = nLineType;

	return TRUE;
}

/**********************************************************************/

bool CPMSInvoiceArray::ProcessPaymentEditLine( CString& strBuffer, CPMSInvoiceLineInfo& infoSale )
{			
	CPMSTermLineEdit csv( strBuffer );

	infoSale.m_nBasePluNo = csv.GetPaymentNo();
	infoSale.m_nModifier = 0;
	infoSale.m_nLevel = 0;
	infoSale.m_strDescription = csv.GetPaymentText();
	infoSale.m_dEposPluQty = 0.0;
	infoSale.m_dBasePluQty = 0.0;
	infoSale.m_dValue = ( ( (double) csv.GetPaymentAmount() ) / 100.0 );
	infoSale.m_dDiscount = 0.0;
	infoSale.m_nLineType = PMS_LINE_PAYMENT;

	return TRUE;
}

/**********************************************************************/

void CPMSInvoiceArray::GetInvoiceChanges( CPMSInvoiceArray& arrayInput1, CPMSInvoiceArray& arrayInput2 )
{
	Reset();

	for ( int n = 0; n < arrayInput1.GetInvoiceItemCount(); n++ )
	{
		CPMSInvoiceLineInfo info;
		arrayInput1.GetInvoiceItem( n, info );
		
		switch( info.m_nLineType )
		{
		case PMS_LINE_SALE:
		case PMS_LINE_REFUND:
		case PMS_LINE_ALLOWANCE:
		case PMS_LINE_SDISC:
		case PMS_LINE_LDISC:
		case PMS_LINE_PAYMENT:
			AddInvoiceItem( info );
			break;
		}
	}

	for ( int n = 0; n < arrayInput2.GetInvoiceItemCount(); n++ )
	{
		CPMSInvoiceLineInfo info;
		arrayInput2.GetInvoiceItem( n, info );
		
		switch( info.m_nLineType )
		{
		case PMS_LINE_SALE:
		case PMS_LINE_REFUND:
		case PMS_LINE_ALLOWANCE:
		case PMS_LINE_SDISC:
		case PMS_LINE_LDISC:
		case PMS_LINE_PAYMENT:
			info.m_dEposPluQty *= -1;
			info.m_dBasePluQty *= -1;
			info.m_dValue *= -1;
			info.m_dDiscount *= -1;
			info.m_dAllowance *= -1;
			AddInvoiceItem( info );
			break;
		}
	}
	
	PurgeBlankItemLines();
	
	CPMSInvoiceArray arrayHeaders;

	for ( int n = 0; n < GetInvoiceItemCount(); n++ )
	{
		CPMSInvoiceLineInfo info;
		GetInvoiceItem( n, info );

		switch( info.m_nLineType )
		{
		case PMS_LINE_REFUND:
			info.m_dValue *= -1;
			break;

		case PMS_LINE_ALLOWANCE:
			info.m_dAllowance = info.m_dValue;
			info.m_dAllowance *= -1;
			info.m_dValue = 0;
			break;
		}

		info.m_nLineType = PMS_LINE_TRAN;
		arrayHeaders.AddInvoiceItem( info );
	}
	
	for ( int n = 0; n < arrayHeaders.GetInvoiceItemCount(); n++ )
	{
		CPMSInvoiceLineInfo info ;
		arrayHeaders.GetInvoiceItem( n, info );
		AddInvoiceItem( info );
	}
}

/**********************************************************************/

void CPMSInvoiceArray::WriteInvoice( int nBookingId, int nAccountNo, bool bStock )
{
	CString strTransFile;
	strTransFile.Format( "%s\\%d.%d",
		PMSOptions.GetPMSFolderSales(),
		nBookingId,
		nAccountNo );

	CSSFile filePMS;
	if ( filePMS.Open( strTransFile, "ab" ) == FALSE )
		return;

	CSSFile fileStock;
	if ( TRUE == bStock )
	{
		::CreateSubdirectory( dbNetwork.GetFolderPathSysset( -1 ) );
		::CreateSubdirectory( dbNetwork.GetFolderPathSyssetStock( -1 ) );

		CString strStockFile = dbNetwork.GetFolderPathSyssetStock( -1 );
		strStockFile += "\\stock.001";

		if ( fileStock.Open( strStockFile, "ab" ) == FALSE )
			bStock = FALSE;
	}

	COleDateTime timeStock = COleDateTime::GetCurrentTime();
	
	CString strStockDate;
	strStockDate.Format( "%2.2d%2.2d%4.4d",
		timeStock.GetDay(),
		timeStock.GetMonth(),
		timeStock.GetYear() );

	CString strStockTime;
	strStockTime.Format( "%2.2d%2.2d%2.2d",
		timeStock.GetHour(),
		timeStock.GetMinute(),
		timeStock.GetSecond() );

	for ( int n = 0; n < GetInvoiceItemCount(); n++ )
	{
		CPMSInvoiceLineInfo infoSale;
		GetInvoiceItem( n, infoSale );

		switch( infoSale.m_nLineType )
		{
		case PMS_LINE_TRAN:
			{
				strStockDate.Format( "%2.2d%2.2d%4.4d",
					infoSale.m_dateSale.GetDay(),
					infoSale.m_dateSale.GetMonth(),
					infoSale.m_dateSale.GetYear() );

				strStockTime.Format( "%2.2d%2.2d%2.2d",
					infoSale.m_dateSale.GetHour(),
					infoSale.m_dateSale.GetMinute(),
					infoSale.m_dateSale.GetSecond() );

				CString strTranNo;
				strTranNo.Format( "%9.9d", infoSale.m_nTransactionNo );

				CString strDate;
				strDate.Format( "%2.2d/%2.2d/%2.2d",
					infoSale.m_dateSale.GetDay(),
					infoSale.m_dateSale.GetMonth(),
					infoSale.m_dateSale.GetYear() % 100 );

				CString strTime;
				strTime.Format( "%2.2d:%2.2d:%2.2d",
					infoSale.m_dateSale.GetHour(),
					infoSale.m_dateSale.GetMinute(),
					infoSale.m_dateSale.GetSecond() );

				double dTempVal = infoSale.m_dValue;
				dTempVal *= 100.0;
				dTempVal += 0.5;
				dTempVal = floor( dTempVal );

				int nValue = int ( dTempVal );

				dTempVal = infoSale.m_dDiscount;
				dTempVal *= 100.0;
				dTempVal += 0.5;
				dTempVal = floor( dTempVal );

				int nDiscount = int ( dTempVal );

				CString strDateComment = "";

				{
					COleDateTime timeNow = COleDateTime::GetCurrentTime();

					CString strDateNow;
					strDateNow.Format( "%4.4d%2.2d%2.2d",
						timeNow.GetYear(),
						timeNow.GetMonth(),
						timeNow.GetDay() );

					CString strTimeNow;
					strTimeNow.Format( "%2.2d%2.2d%2.2d",
						timeNow.GetHour(),
						timeNow.GetMinute(),
						timeNow.GetSecond() );
					
					CCSV csv;
					csv.Add( "$COMMENT" );
					csv.Add( 1 );
					csv.Add( 1 );
					csv.Add( strDateNow );
					csv.Add( strTimeNow );
					strDateComment = csv.GetLine();
				}

				{
					CCSV csv;
					csv.Add( "$TRAN_EDIT" );
					csv.Add( 1 );
					csv.Add( strTranNo );
					csv.Add( strDate );	
					csv.Add( strTime );
					csv.Add( nValue - nDiscount );
					filePMS.WriteLine( csv.GetLine() );
					filePMS.WriteLine( strDateComment );
				}
			}
			break;

		case PMS_LINE_SALE:
			{
				CStringArray arraySales;
				if ( GetPMSSalesLine( infoSale, arraySales, 0 ) == TRUE )
				{
					CString strSale = arraySales.GetAt(0);
					if ( strSale != "" )
						filePMS.WriteLine( strSale );

					CString strDiscount = arraySales.GetAt(1);
					if ( strDiscount != "" )
						filePMS.WriteLine( strDiscount );

					if ( TRUE == bStock )
					{
						CString strStockLine = "";
						strStockLine += strStockDate + ",";
						strStockLine += strStockTime + ",";
						strStockLine += arraySales.GetAt(2);
						fileStock.WriteLine( strStockLine );
					}
				}
			}
			break;

		case PMS_LINE_REFUND:
		case PMS_LINE_ALLOWANCE:
			{
				CStringArray arraySales;
				if ( GetPMSSalesLine( infoSale, arraySales, 0 ) == TRUE )
				{
					CString strSale = arraySales.GetAt(0);
					if ( strSale != "" )
						filePMS.WriteLine( strSale );

					if ( TRUE == bStock )
					{
						CString strStockLine = "";
						strStockLine += strStockDate + ",";
						strStockLine += strStockTime + ",";
						strStockLine += arraySales.GetAt(2);
						fileStock.WriteLine( strStockLine );
					}
				}
			}
			break;

		case PMS_LINE_SDISC:
		case PMS_LINE_LDISC:
			{
				CString strLine;
				if ( GetPMSDiscountLine( infoSale, strLine ) == TRUE )
					filePMS.WriteLine( strLine );
			}
			break;

		case PMS_LINE_PAYMENT:
			{
				CString strLine;
				if ( GetPMSPaymentLine( infoSale, strLine ) == TRUE )
					filePMS.WriteLine( strLine );
			}
			break;
		}	
	}
}

/**********************************************************************/

bool CPMSInvoiceArray::GetPMSSalesLine( CPMSInvoiceLineInfo& infoSale, CStringArray& arraySales, int nCovers )
{
	return GetTermSalesLine( infoSale, arraySales, TRUE, nCovers );
}

/**********************************************************************/

bool CPMSInvoiceArray::GetCASHRSPSalesLine( CPMSInvoiceLineInfo& infoSale, CStringArray& arraySales, int nCovers )
{
	return GetTermSalesLine( infoSale, arraySales, FALSE, nCovers );
}

/**********************************************************************/

bool CPMSInvoiceArray::GetTermSalesLine( CPMSInvoiceLineInfo& infoSale, CStringArray& arraySales, bool bPMS, int nCovers )
{
	//CStringArray in PMS mode
	//0 = PMS Sales line
	//1 = PMS Discount line
	//2 = Pending stock line
	//
	//CStringArray in CASHRSP mode
	//0 = CASHRSP Sales line
	//1 = PLU Sales History line
	//2 = CASHRSP Discount line
	//3 = PLU Sales History discount line

	arraySales.RemoveAll();

	arraySales.Add( "" );
	arraySales.Add( "" );
	arraySales.Add( "" );
		
	if ( FALSE == bPMS )
		arraySales.Add( "" );
	
	__int64 nEPOSPluNo = 0;
	if ( SysInfo.IsModifiableSSPluNo( infoSale.m_nBasePluNo ) == TRUE )
		nEPOSPluNo = ( infoSale.m_nBasePluNo * 10 ) + infoSale.m_nModifier;
	else
		nEPOSPluNo = infoSale.m_nBasePluNo;

	CString strLabelPMS;
	CString strRefundFlag;
	int nDiscount = 0;

	double dHistoryQty = infoSale.m_dEposPluQty;
	double dTempVal = 0.0;
	double dHistoryVal = 0.0;

	switch( infoSale.m_nLineType )
	{
	case PMS_LINE_SALE:
	case PMS_LINE_REFUND:
	case PMS_LINE_ACCOM:
	case PMS_LINE_EXTRABED:
	case PMS_LINE_EXTRACOT:
	case PMS_LINE_DINNER:
	case PMS_LINE_BREAKFAST:
		dTempVal = infoSale.m_dValue;
		dHistoryVal = infoSale.m_dValue;
		break;

	case PMS_LINE_ALLOWANCE:
		dTempVal = infoSale.m_dAllowance;
		dHistoryVal = infoSale.m_dAllowance;
		break;
	}
	
	dTempVal *= 100.0;
	dTempVal += 0.5;
	dTempVal = floor( dTempVal );
				
	int nTermValue = int( dTempVal );
	
	bool bGotSale = ( 0 != nTermValue ) || ( CPriceHelpers::CompareDoubles( infoSale.m_dEposPluQty, 0.0, 3 ) != 0 );

	if ( PMS_LINE_REFUND == infoSale.m_nLineType )
	{
		strLabelPMS = "$REFUND_EDIT";
		strRefundFlag = "R";
		nTermValue *= -1;
		dHistoryQty *= -1;
		dHistoryVal *= -1;
	}
	else if ( PMS_LINE_ALLOWANCE == infoSale.m_nLineType )
	{
		strLabelPMS = "$ALLOWANCE_EDIT";
		strRefundFlag = "A";
		nTermValue *= -1;
		dHistoryVal *= -1;
	}
	else
	{
		strLabelPMS = "$ITEM_EDIT";
		strRefundFlag = "";

		double dTempVal = infoSale.m_dDiscount;
		dTempVal *= 100.0;
		dTempVal += 0.5;
		dTempVal = floor( dTempVal );
				
		nDiscount = int( dTempVal );
		nDiscount *= -1;
	}

	CString strTaxBand = CTaxArray::GetTaxBandFromNumber(infoSale.m_nTaxBand, 1, "", "");
	
	if ( TRUE == bPMS )
	{
		if ( TRUE == bGotSale )
		{
			CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
			csv.Add( strLabelPMS );
			csv.Add( 1 );
			csv.Add( nEPOSPluNo );
			csv.Add( infoSale.m_dEposPluQty, 2 );
			csv.Add( infoSale.m_nLevel );
			csv.Add( nTermValue );
			csv.Add( infoSale.m_nDeptNo );
			csv.Add( infoSale.m_nGroupNo );
			csv.Add( strTaxBand );
			arraySales.SetAt( 0, csv.GetLine() );
		}

		if ( nDiscount != 0 )
		{
			CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
			csv.Add( "$IDISC_EDIT" );
			csv.Add( 1 );
			csv.Add( nEPOSPluNo );
			csv.Add( nDiscount );
			arraySales.SetAt( 1, csv.GetLine() );
		}

		if ( TRUE == bGotSale )
		{
			double dValue = ( (double) nTermValue ) / 100.0;
			//DATE AND TIME WILL BE ADDED BY CALLING FUNCTION
			CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
			csv.Add( nEPOSPluNo );
			csv.Add( infoSale.m_nLevel );
			csv.Add( infoSale.m_dEposPluQty, 2 );
			csv.Add( dValue, 2 );
			csv.Add( 0 );	//SALE, NOT WASTE
			arraySales.SetAt( 2, csv.GetLine() );
		}
	}
	else
	{
		if ( TRUE == bGotSale )
		{
			{
				CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
				csv.Add( "$ITEM" );
				csv.Add( nEPOSPluNo );					//PLU NO
				csv.Add( infoSale.m_nDeptNo );			//DEPT NO
				csv.Add( infoSale.m_dEposPluQty, 5 );	//PLU QTY
				csv.Add( infoSale.m_nLevel );			//PRICE BAND
				csv.Add( "" );							//UNIT PRICE
				csv.Add( strTaxBand );					//TAX BAND
				csv.Add( strRefundFlag );				//REFUND FLAG
				csv.Add( "" );							//VOID FLAG
				csv.Add( infoSale.m_nGroupNo );			//GROUP NUMBER
				csv.Add( "" );							//PRICE OVERRIDE
				csv.Add( "" );							//DATABASE PRICE
				csv.Add( nTermValue );					//LINE PRICE
				csv.Add( "" );							//BARCODE
				csv.Add( "" );							//DEPOSIT ID
				csv.Add( "" );							//DISCOUNT AMOUNT
				arraySales.SetAt( 0, csv.GetLine() );
			}

			{
				CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
				csv.Add( "" );		//TIME
				csv.Add( 0 );		//SERVER NO
				csv.Add( nEPOSPluNo );
				csv.Add( infoSale.m_nLevel );
				csv.Add( dHistoryQty, 5 );
				csv.Add( dHistoryVal, 2 );
				csv.Add( infoSale.m_nDeptNo );
				csv.Add( FALSE );	//TRAINING FLAG
				csv.Add( FALSE );	//DISCOUNT FLAG
				csv.Add( 0 );		//PAYMENT NUMBER
				csv.Add( 0 );		//WASTAGE FLAG
				csv.Add( 0 );		//PMS DAY
				csv.Add( infoSale.m_nGroupNo + 1 );
				csv.Add( nCovers );
				csv.Add( strTaxBand );
				arraySales.SetAt( 1, csv.GetLine() );
			}
		}

		if ( nDiscount != 0 )
		{
			{
				CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
				csv.Add( "$IDISC" );
				csv.Add( nEPOSPluNo );		//PLU NUMBER
				csv.Add( nDiscount );		//AMOUNT
				csv.Add( "M" );				//TYPE
				csv.Add( "" );				//RATE
				arraySales.SetAt( 2, csv.GetLine() );
			}

			{
				CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
				csv.Add( "" );				//TIME
				csv.Add( 0 );				//SERVER NO
				csv.Add( nEPOSPluNo );
				csv.Add( infoSale.m_nLevel );
				csv.Add( 0 );
				csv.Add( infoSale.m_dDiscount, 2 );
				csv.Add( infoSale.m_nDeptNo );
				csv.Add( FALSE );			//TRAINING FLAG
				csv.Add( TRUE );			//DISCOUNT FLAG
				csv.Add( 0 );				//PAYMENT NUMBER
				csv.Add( 0 );				//WASTAGE FLAG
				csv.Add( 0 );				//PMS DAY
				csv.Add( infoSale.m_nGroupNo + 1 );
				csv.Add( "" );				//COVERS
				csv.Add( strTaxBand );
				arraySales.SetAt( 3, csv.GetLine() );
			}
		}
	}

	return TRUE;
}

/**********************************************************************/

bool CPMSInvoiceArray::GetPMSDiscountLine( CPMSInvoiceLineInfo& infoSale, CString& strLine )
{
	return GetTermDiscountLine( infoSale, strLine, TRUE );
}

/**********************************************************************/

bool CPMSInvoiceArray::GetCASHRSPDiscountLine( CPMSInvoiceLineInfo& infoSale, CString& strLine )
{
	return GetTermDiscountLine( infoSale, strLine, FALSE );
}

/**********************************************************************/

bool CPMSInvoiceArray::GetTermDiscountLine( CPMSInvoiceLineInfo& infoSale, CString& strLine, bool bPMS )
{
	double dTempVal = infoSale.m_dDiscount;
	dTempVal *= 100.0;
	dTempVal += 0.5;
	dTempVal = floor( dTempVal );

	int nValue = int ( dTempVal );

	if ( 0 == nValue )
		return FALSE;

	CString strPMSLabel;
	CString strCASHRSPLabel;

	if ( PMS_LINE_LDISC == infoSale.m_nLineType )
	{
		strPMSLabel = "$LDISC_EDIT";
		strCASHRSPLabel = "$LDISC";
	}
	else
	{
		strPMSLabel = "$SDISC_EDIT";
		strCASHRSPLabel = "$SDISC";
	}

	if ( TRUE == bPMS )
	{
		CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
		csv.Add( strPMSLabel );
		csv.Add( 1 );
		csv.Add( -nValue );
		strLine = csv.GetLine();
	}
	else
	{
		CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
		csv.Add( strCASHRSPLabel );
		csv.Add( -nValue );			//AMOUNT
		csv.Add( "M" );				//TYPE
		csv.Add( "" );				//RATE
		csv.Add( "" );				//PROMO
		strLine = csv.GetLine();
	}

	return TRUE;
}

/**********************************************************************/

bool CPMSInvoiceArray::GetPMSPaymentLine( CPMSInvoiceLineInfo& infoSale, CString& strLine )
{
	return GetTermPaymentLine( infoSale, strLine, TRUE );
}

/**********************************************************************/

bool CPMSInvoiceArray::GetCASHRSPPaymentLine( CPMSInvoiceLineInfo& infoSale, CString& strLine )
{
	return GetTermPaymentLine( infoSale, strLine, FALSE );
}

/**********************************************************************/

bool CPMSInvoiceArray::GetTermPaymentLine( CPMSInvoiceLineInfo& infoSale, CString& strLine, bool bPMS )
{
	double dTempVal = infoSale.m_dValue;
	dTempVal *= 100.0;
	dTempVal += 0.5;
	dTempVal = floor( dTempVal );

	int nValue = int ( dTempVal );

	if ( 0 == nValue )
		return FALSE;

	CString strPMSLabel;
	CString strCASHRSPLabel;

	if ( TRUE == bPMS )
	{
		CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
		csv.Add( "$PAYM_EDIT" );
		csv.Add( 1 );
		csv.Add( infoSale.m_nBasePluNo );
		csv.Add( infoSale.m_strDescription );
		csv.Add( nValue );
		strLine = csv.GetLine();
	}
	else
	{
		CCSV csv ( ',', '"', FALSE, FALSE, TRUE );

		if ( PMS_LINE_DEPOSIT == infoSale.m_nLineType )
			csv.Add( "#PMS_DEPO" );
		else
			csv.Add( "$PAYM" );

		csv.Add( infoSale.m_nBasePluNo );			
		csv.Add( infoSale.m_strDescription );
		csv.Add( nValue );
		csv.Add( nValue );
		csv.Add( "" );
		csv.Add( "" );
		csv.Add( "" );
		strLine = csv.GetLine();
	}

	return TRUE;
}

/**********************************************************************/

void CPMSInvoiceArray::DeleteEPOSReportFile( int nRoomGridRoomIdx, int nRoomGridAccountIdx )
{
	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( nRoomGridRoomIdx, infoRoom );

	int nAccountNo = nRoomGridAccountIdx + 1;

	CString strReportFile;
	strReportFile.Format( "%s\\%d.%d",
		PMSOptions.GetPMSFolderReports(),
		infoRoom.GetBookingId(),
		nAccountNo );

	CFileRemover FileRemover( strReportFile );
}

/**********************************************************************/

void CPMSInvoiceArray::CreateEPOSReportFile( int nRoomGridRoomIdx, int nRoomGridAccountIdx, bool bForReport )
{
	CStringArray arrayReportTran;
	CStringArray arrayReportSale;
	CStringArray arrayReportTerm;

	int nDepoMinDay, nDepoMaxDay, nTranMinDay, nTranMaxDay;
	CreateEPOSReportData( nRoomGridRoomIdx, nRoomGridAccountIdx, arrayReportTran, arrayReportSale, arrayReportTerm, nDepoMinDay, nDepoMaxDay, nTranMinDay, nTranMaxDay );

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( nRoomGridRoomIdx, infoRoom );

	CPMSRoomAccountInfo infoAccount;
	PMSRoomStatusTable.GetAccountInfo( nRoomGridRoomIdx, nRoomGridAccountIdx, infoAccount );

	int nAccountNo = nRoomGridAccountIdx + 1;

	CString strReportFile;
	
	if ( FALSE == bForReport )
	{
		strReportFile.Format( "%s\\%d.%d",
			PMSOptions.GetPMSFolderReports(),
			infoRoom.GetBookingId(),
			nAccountNo );
	}
	else
	{
		CString strFolder = "PMS_REPORT";
		GetTempPath( strFolder );

		strReportFile.Format( "%s\\%d.%d",
			(const char*) strFolder,
			infoRoom.GetBookingId(),
			nAccountNo );
	}

	for ( int n = arrayReportTerm.GetSize() - 1; n >= 0; n-- )
	{
		CString strCheck = arrayReportTerm.GetAt(n);
		if ( strCheck.Left(9) == "#PMS_TRAN" )
			arrayReportTerm.RemoveAt(n);
		else
			break;
	}

	for ( int n = arrayReportTran.GetSize() - 1; n >= 0; n-- )
	{
		CString strCheck = arrayReportTran.GetAt(n);
		if ( strCheck.Left(9) == "#PMS_TIME" )
			arrayReportTran.RemoveAt(n);
		else
			break;
	}

	for ( int n = arrayReportSale.GetSize() - 1; n >= 0; n-- )
	{
		CString strCheck = arrayReportSale.GetAt(n);
		if ( strCheck.Left(9) == "#PMS_TIME" )
			arrayReportSale.RemoveAt(n);
		else
			break;
	}

	CSSFile fileReports;
	if ( fileReports.Open( strReportFile, "wb" ) == TRUE )
	{
		int nTranSize = arrayReportTran.GetSize();
		int nSaleSize = arrayReportSale.GetSize();
		int nTermSize = arrayReportTerm.GetSize();

		CString strHeader;
		strHeader.Format( "#DATE,%d,%d,%d,%d",
			nDepoMinDay,
			nDepoMaxDay,
			nTranMinDay,
			nTranMaxDay );
		fileReports.WriteLine( strHeader );

		CCSV csvTran ( ',', '"', FALSE, FALSE, TRUE );;
		csvTran.Add ( "#TRAN" );
		csvTran.Add ( nTranSize );
		fileReports.WriteLine( csvTran.GetLine() );

		for ( int n = 0; n < nTranSize; n++ )
			fileReports.WriteLine( arrayReportTran.GetAt(n) );

		CCSV csvSale ( ',', '"', FALSE, FALSE, TRUE );;
		csvSale.Add ( "#PLU" );
		csvSale.Add ( nSaleSize );
		fileReports.WriteLine( csvSale.GetLine() );

		for ( int n = 0; n < nSaleSize; n++ )
			fileReports.WriteLine( arrayReportSale.GetAt(n) );

		CCSV csvTerm ( ',', '"', FALSE, FALSE, TRUE );;
		csvTerm.Add ( "#TERM" );
		csvTerm.Add ( nTermSize );
		fileReports.WriteLine( csvTerm.GetLine() );

		for ( int n = 0; n < nTermSize; n++ )
			fileReports.WriteLine( arrayReportTerm.GetAt(n) );
	}
}

/**********************************************************************/

void CPMSInvoiceArray::CreateEPOSReportData( int nRoomGridRoomIdx, int nRoomGridAccountIdx, CStringArray& arrayReportTran, CStringArray& arrayReportSale, CStringArray& arrayReportTerm, int& nDepoMinDay, int& nDepoMaxDay, int& nTranMinDay, int& nTranMaxDay )
{
	arrayReportTran.RemoveAll();
	arrayReportSale.RemoveAll();
	arrayReportTerm.RemoveAll();

	nDepoMinDay = -1;
	nDepoMaxDay = -1;
	nTranMinDay = -1;
	nTranMaxDay = -1;

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( nRoomGridRoomIdx, infoRoom );

	CPMSRoomAccountInfo infoAccount;
	PMSRoomStatusTable.GetAccountInfo( nRoomGridRoomIdx, nRoomGridAccountIdx, infoAccount );

	int nAccountNo = nRoomGridAccountIdx + 1;
	int nGuestCount = infoAccount.GetGuestCount();

	COleDateTime date;
	int nDate = infoRoom.GetStartDay() + infoRoom.GetNights();
	GetOleDateSince2011( nDate, date );
	
	CString strTranDate;
	strTranDate.Format( "%2.2d/%2.2d/%2.2d",
		date.GetDay(),
		date.GetMonth(),
		date.GetYear() % 100 );

	CReportConsolidationArray<CCashRSPImporterDiscountHelperSales> arraySales;
	CReportConsolidationArray<CCashRSPImporterDiscountHelperPendingItem> arrayDiscounts;
	double dTransactionSubtotalDiscount = 0.0;
	double dTransactionTotalSales = 0.0;

	int nTransactionItemLines = 0;
	int nTotalAccountItemLines = 0;
	double dTransactionQty = 0.0;
	double dTotalAccountQty = 0.0;
	
	CString strTranNo;
	strTranNo.Format( "000%5.5d%1.1d",
		infoRoom.GetBookingId() % 100000,
		nAccountNo );
 
	double dTranVal = infoAccount.GetTotalSpend() - infoAccount.GetTotalAllowance();
	dTranVal *= 100.0;
	dTranVal += 0.5;
	dTranVal = floor( dTranVal );

	double dTranPay = infoAccount.GetTotalPaid();
	dTranPay *= 100.0;
	dTranPay += 0.5;
	dTranPay = floor( dTranPay );

	CCSV csvTimeHeader ( ',', '"', FALSE, FALSE, TRUE );
	CCSV csvTimeHeaderTerm ( ',', '"', FALSE, FALSE, TRUE );
	
	int nTermHeaderPos = 0;
	int nTranHeaderPos = 0;
	int nSaleHeaderPos = 0;

	int nPMSDayNumber = -1;
	bool bIsDepositTransaction = FALSE;

	for ( int nInvoiceItemPos = 0; nInvoiceItemPos < GetInvoiceItemCount(); nInvoiceItemPos++ )
	{
		CPMSInvoiceLineInfo item;
		GetInvoiceItem(nInvoiceItemPos, item );
		
		switch( item.m_nLineType )
		{
		case PMS_LINE_TRAN:
	
			//ADD TRANSACTION SUMMARY LINE FOR PREVIOUS TRANSACTION
			if ( nTermHeaderPos < arrayReportTerm.GetSize() )
			{
				ProcessEPOSDiscounts( arrayReportSale, arraySales, arrayDiscounts, dTransactionTotalSales, dTransactionSubtotalDiscount, nPMSDayNumber );
				
				double dSubTranVal = dTransactionTotalSales - dTransactionSubtotalDiscount;
				csvTimeHeader.Add( dSubTranVal, 2 );

				double dSubTranValTerm = dSubTranVal;
				dSubTranValTerm *= 100.0;
				dSubTranValTerm += 0.5;
				dSubTranValTerm = floor( dSubTranValTerm );

				csvTimeHeaderTerm.SetAt( 3, dTransactionQty, 5 );
				csvTimeHeaderTerm.SetAt( 5, nTransactionItemLines );
				csvTimeHeaderTerm.SetAt( 6, int ( dSubTranValTerm ) );

				CString strTimeHeader = csvTimeHeader.GetLine();
				arrayReportTerm.InsertAt( nTermHeaderPos, csvTimeHeaderTerm.GetLine() );
				arrayReportTran.InsertAt( nTranHeaderPos, strTimeHeader );
				arrayReportSale.InsertAt( nSaleHeaderPos, strTimeHeader );

				CCSV csvOut ( ',', '"', FALSE, FALSE, TRUE );
				csvOut.Add( "" );	//TIME
				csvOut.Add( 0 );					
				csvOut.Add( dTransactionQty, 5 );
				csvOut.Add( dSubTranVal, 2 );
				csvOut.Add( infoAccount.GetGuestCount() );
				csvOut.Add( 0 );	//TRAINING FLAG
				csvOut.Add( 0 );	//PAYMENT TYPE
				arrayReportTran.Add( csvOut.GetLine() );
				
				arraySales.RemoveAll();
				arrayDiscounts.RemoveAll();
				dTransactionTotalSales = 0.0;
				dTransactionSubtotalDiscount = 0.0;
				nTransactionItemLines = 0;
				dTransactionQty = 0.0;

				nTermHeaderPos = arrayReportTerm.GetSize();
				nTranHeaderPos = arrayReportTran.GetSize();
				nSaleHeaderPos = arrayReportSale.GetSize();

				if ( FALSE == bIsDepositTransaction )
				{
					if ( ( nPMSDayNumber < nTranMinDay ) || ( -1 == nTranMinDay ) )
						nTranMinDay = nPMSDayNumber;

					if ( ( nPMSDayNumber > nTranMaxDay ) || ( -1 == nTranMaxDay ) )
						nTranMaxDay = nPMSDayNumber;
				}
				else
				{
					if ( ( nPMSDayNumber < nDepoMinDay ) || ( -1 == nDepoMinDay ) )
						nDepoMinDay = nPMSDayNumber;

					if ( ( nPMSDayNumber > nDepoMaxDay ) || ( -1 == nDepoMaxDay ) )
						nDepoMaxDay = nPMSDayNumber;
				}
			}

			//CREATE HEADER FOR REPORTING PMS SALES BY ACTUAL DATE AND TIME
			{
				nPMSDayNumber = GetDayNumberSince2011( item.m_dateSale );
				bIsDepositTransaction = FALSE;

				csvTimeHeader.RemoveAll();
				csvTimeHeader.Add( "#PMS_TIME" );
				csvTimeHeader.Add( nPMSDayNumber );
				csvTimeHeader.Add( item.m_dateSale.GetHour() );
				csvTimeHeader.Add( item.m_dateSale.GetMinute() );

				CString strTranTime;
				strTranTime.Format( "%2.2d:%2.2d:00",
					item.m_dateSale.GetHour(),
					item.m_dateSale.GetMinute() );

				CString strTranDate;
				strTranDate.Format( "%2.2d/%2.2d/%2.2d",
					item.m_dateSale.GetDay(),
					item.m_dateSale.GetMonth(),
					item.m_dateSale.GetYear() % 100 );

				csvTimeHeaderTerm.RemoveAll();
				csvTimeHeaderTerm.Add( "#PMS_TRAN" );
				csvTimeHeaderTerm.Add( strTranNo );
				csvTimeHeaderTerm.Add( 0 );					//NOT USED (TABLE NO)
				csvTimeHeaderTerm.Add( 0 );					//FILLED IN LATER (QTY)				
				csvTimeHeaderTerm.Add( nGuestCount );	
				csvTimeHeaderTerm.Add( 0 );					//FILLED IN LATER (LINE COUNT) 
				csvTimeHeaderTerm.Add( 0 );					//FILLED IN LATER (VALUE)
				csvTimeHeaderTerm.Add( strTranDate );		 
				csvTimeHeaderTerm.Add( strTranTime );		
				csvTimeHeaderTerm.Add( 0 );					//NOT USED (SERVER NO)
			}
			
			break;

		case PMS_LINE_SALE:
		case PMS_LINE_REFUND:
		case PMS_LINE_ALLOWANCE:
		case PMS_LINE_ACCOM:
		case PMS_LINE_EXTRABED:
		case PMS_LINE_EXTRACOT:
		case PMS_LINE_DINNER:
		case PMS_LINE_BREAKFAST:
			{
				CStringArray arrayCASHRSP;
				if ( GetCASHRSPSalesLine( item, arrayCASHRSP, nGuestCount ) == TRUE )
				{
					nTransactionItemLines++;
					nTotalAccountItemLines++;
					dTransactionQty += item.m_dEposPluQty;
					dTotalAccountQty += item.m_dEposPluQty;

					__int64 nEPOSPluNo = 0;
					if ( SysInfo.IsModifiableSSPluNo( item.m_nBasePluNo ) == TRUE )
						nEPOSPluNo = ( item.m_nBasePluNo * 10 ) + item.m_nModifier;
					else
						nEPOSPluNo = item.m_nBasePluNo;

					CString strLine = "";
					
					//CASHRSP SALES LINE
					strLine = arrayCASHRSP.GetAt(0);
					if ( strLine != "" )
						arrayReportTerm.Add( strLine );

					//HISTORY SALES LINE
					strLine = arrayCASHRSP.GetAt(1);
					if ( strLine != "" )
					{
						CCSV csv( strLine );
						csv.SetAt( 11, nPMSDayNumber );
						arrayReportSale.Add( csv.GetLine() );
					}

					//CASHRSP DISCOUNT LINE
					strLine = arrayCASHRSP.GetAt(2);
					if ( strLine != "" )
						arrayReportTerm.Add( strLine );

					//HISTORY DISCOUNT LINE
					//strLine = arrayCASHRSP.GetAt(3);
					//if ( strLine != "" )
					//{
					//	CCSV csv( strLine );
					//	csv.SetAt( 11, nPMSDayNumber );
					//	arrayReportSale.Add( csv.GetLine() );
					//}

					//ADD SALES TO TRANSACTION BUFFER
					strLine = arrayCASHRSP.GetAt(0);
					if ( strLine != "" )
					{
						double dQty = item.m_dEposPluQty;
						double dValue = item.m_dValue;

						switch( item.m_nLineType )
						{
						case PMS_LINE_REFUND:
							dQty *= -1;
							dValue *= -1;
							break;

						case PMS_LINE_ALLOWANCE:
							dValue *= -1;
							break;
						}

						switch( item.m_nLineType )
						{
						case PMS_LINE_REFUND:
						case PMS_LINE_SALE:
						case PMS_LINE_ACCOM:
						case PMS_LINE_EXTRABED:
						case PMS_LINE_EXTRACOT:
						case PMS_LINE_DINNER:
						case PMS_LINE_BREAKFAST:
							{
								dTransactionTotalSales += dValue;

								//COPY ITEM POS TO ANALYSIS CATEGORY FIELD TO TIE SALE TO DISCOUNT
								CCashRSPImporterDiscountHelperSales info;
								info.m_nPluNo = nEPOSPluNo;
								info.m_nAnalysisCategory = nInvoiceItemPos;
								info.m_nItemServerNo = 0;
								info.m_nPriceBand = item.m_nLevel;
								info.m_nDeptNo = item.m_nDeptNo;
								info.m_nGroupNo = item.m_nGroupNo;
								info.m_strTaxBand.Format( "%d", item.m_nTaxBand );
								info.m_dQty = dQty;
								info.m_dVal = dValue;

								arraySales.Consolidate( info );
							}
							break;
						}
					}

					//ADD DISCOUNTS TO TRANSACTION BUFFER
					strLine = arrayCASHRSP.GetAt(2);
					if ( strLine != "" )
					{
						//COPY ITEM POS TO ANALYSIS CATEGORY FIELD TO TIE SALE TO DISCOUNT
						CCashRSPImporterDiscountHelperPendingItem info;
						info.m_nPluNo = nEPOSPluNo;
						info.m_nAnalysisCategory = nInvoiceItemPos;
						info.m_dAmountAsDouble = item.m_dDiscount;
						
						arrayDiscounts.Consolidate( info );

						dTransactionTotalSales -= item.m_dDiscount;
						nTransactionItemLines++;
						nTotalAccountItemLines++;
						dTransactionQty += 1.0;
						dTotalAccountQty += 1.0;
					}
				}
			}
			break;

		case PMS_LINE_SDISC:
		case PMS_LINE_LDISC:
			{
				CString strCASHRSP;
				if ( GetCASHRSPDiscountLine( item, strCASHRSP ) == TRUE )
				{
					nTransactionItemLines++;
					nTotalAccountItemLines++;
					dTransactionQty += 1.0;
					dTotalAccountQty += 1.0;
					
					arrayReportTerm.Add( strCASHRSP );			
					dTransactionSubtotalDiscount += item.m_dDiscount;
				}
			}
			break;

		case PMS_LINE_PAYMENT:
			{
				CString strCASHRSP;
				if ( GetCASHRSPPaymentLine( item, strCASHRSP ) == TRUE )
					arrayReportTerm.Add( strCASHRSP );
			}
			break;

		case PMS_LINE_DEPOSIT:
			{
				CString strCASHRSP;
				if ( GetCASHRSPPaymentLine( item, strCASHRSP ) == TRUE )
				{
					arrayReportTerm.Add( strCASHRSP );
					bIsDepositTransaction = TRUE;
				}
			}
			break;
		}
	}

	if ( nTermHeaderPos < arrayReportTerm.GetSize() )		
	{
		ProcessEPOSDiscounts( arrayReportSale, arraySales, arrayDiscounts, dTransactionTotalSales, dTransactionSubtotalDiscount, nPMSDayNumber );

		double dSubTranVal = dTransactionTotalSales - dTransactionSubtotalDiscount;
		csvTimeHeader.Add( dSubTranVal, 2 );

		double dSubTranValTerm = dSubTranVal;
		dSubTranValTerm *= 100.0;
		dSubTranValTerm += 0.5;
		dSubTranValTerm = floor( dSubTranValTerm );

		csvTimeHeaderTerm.SetAt( 3, dTransactionQty, 5 );
		csvTimeHeaderTerm.SetAt( 5, nTransactionItemLines );
		csvTimeHeaderTerm.SetAt( 6, int ( dSubTranValTerm ) );

		CString strTimeHeader = csvTimeHeader.GetLine();
		arrayReportTerm.InsertAt( nTermHeaderPos, csvTimeHeaderTerm.GetLine() );
		arrayReportTran.InsertAt( nTranHeaderPos, strTimeHeader );
		arrayReportSale.InsertAt( nSaleHeaderPos, strTimeHeader );

		CCSV csvOut ( ',', '"', FALSE, FALSE, TRUE );
		csvOut.Add( "" );	//TIME
		csvOut.Add( 0 );					
		csvOut.Add( dTransactionQty, 5 );
		csvOut.Add( dTransactionTotalSales - dTransactionSubtotalDiscount, 2 );
		csvOut.Add( infoAccount.GetGuestCount() );
		csvOut.Add( 0 );	//TRAINING FLAG
		csvOut.Add( 0 );	//PAYMENT TYPE
		arrayReportTran.Add( csvOut.GetLine() );

		if ( FALSE == bIsDepositTransaction )
		{
			if ( ( nPMSDayNumber < nTranMinDay ) || ( -1 == nTranMinDay ) )
				nTranMinDay = nPMSDayNumber;

			if ( ( nPMSDayNumber > nTranMaxDay ) || ( -1 == nTranMaxDay ) )
				nTranMaxDay = nPMSDayNumber;
		}
		else
		{
			if ( ( nPMSDayNumber < nDepoMinDay ) || ( -1 == nDepoMinDay ) )
				nDepoMinDay = nPMSDayNumber;

			if ( ( nPMSDayNumber > nDepoMaxDay ) || ( -1 == nDepoMaxDay ) )
				nDepoMaxDay = nPMSDayNumber;
		}
	}

	CCSV csvOut ( ',', '"', FALSE, FALSE, TRUE );
	csvOut.Add( "$TRAN" );
	csvOut.Add( strTranNo );
	csvOut.Add( 0 );							//TABLE NO
	csvOut.Add( dTotalAccountQty, 5 );				
	csvOut.Add( infoAccount.GetGuestCount() );	
	csvOut.Add( nTotalAccountItemLines );
	csvOut.Add( int ( dTranVal ) );
	csvOut.Add( strTranDate );
	csvOut.Add( "00:00:00" );					//TRAN TIME
	csvOut.Add( 0 );							//SERVER NO
	arrayReportTerm.InsertAt( 0, csvOut.GetLine() );
	
	int nHeaderPos = 1;

	{
		CString str;
		str.Format( "$PMS_HEADER,Room %d", infoRoom.GetRoomNo() );
		arrayReportTerm.InsertAt( nHeaderPos++, str );
	}

	{
		CString strName = infoAccount.GetGuestName();
		::TrimSpaces( strName, FALSE );

		if ( strName != "" )
		{
			CString str;
			str.Format( "$PMS_HEADER,%s", strName );
			arrayReportTerm.InsertAt( nHeaderPos++, str );
		}
	}


	{
		CString str;
		str.Format( "$PMS_HEADER,\"Booking Ref : %8.8d, Account %d\"", infoRoom.GetBookingId(), nAccountNo );
		arrayReportTerm.InsertAt( nHeaderPos++, str );
	}
	
	{
		COleDateTime date1;
		int nDate1 = infoRoom.GetStartDay();
		GetOleDateSince2011( nDate1, date1 );
	
		COleDateTime date2;
		int nDate2 = infoRoom.GetStartDay() + infoRoom.GetNights();
		GetOleDateSince2011( nDate2, date2 );
	
		CString str;
		str.Format( "$PMS_HEADER,%2.2d/%2.2d/%4.4d - %2.2d/%2.2d/%4.4d", 
			 date1.GetDay(),
			 date1.GetMonth(),
			 date1.GetYear(),
			 date2.GetDay(),
			 date2.GetMonth(),
			 date2.GetYear() );

		arrayReportTerm.InsertAt( nHeaderPos++, str );
	}

	{
		int nBookingIdx;
		if ( DataManagerNonDb.PMSBooking.FindBookingById( infoRoom.GetBookingId(), nBookingIdx ) == TRUE )
		{
			CPMSBookingCSVRecord BookingRecord;
			DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

			CString str = "$PMS_HEADER,";

			switch( BookingRecord.GetBookingStatus() )
			{
			case PMS_BOOKING_STATUS_PENDING:
				arrayReportTerm.InsertAt( nHeaderPos++, str + "Pending booking" );
				break;

			case PMS_BOOKING_STATUS_ACTIVE:
				arrayReportTerm.InsertAt( nHeaderPos++, str + "Active booking" );
				break;

			case PMS_BOOKING_STATUS_COMPLETE:
				arrayReportTerm.InsertAt( nHeaderPos++, str + "Completed booking" );
				break;

			case PMS_BOOKING_STATUS_CANCELLED:
				arrayReportTerm.InsertAt( nHeaderPos++, str + "Cancelled booking" );
				break;
			}
		}
	}

	if ( CPriceHelpers::CompareDoubles( dTranVal, dTranPay, 0 ) != 0.0 )
	{
		CCSV csvOut ( ',', '"', FALSE, FALSE, FALSE );
		
		csvOut.Add( "$PMS_COMMENT" );

		arrayReportTerm.Add( csvOut.GetLine() );

		if ( dTranVal > dTranPay )
		{
			csvOut.Add ( "** Under paid" );
			csvOut.Add ( ( dTranVal - dTranPay  ) / 100.0, 2 );
		}
		else
		{
			csvOut.Add ( "** Over paid" );
			csvOut.Add ( ( dTranPay - dTranVal ) / 100.0, 2 );
		}

		arrayReportTerm.Add( csvOut.GetLine() );
	}
}

/**********************************************************************/

void CPMSInvoiceArray::ProcessEPOSDiscounts(CStringArray& arrayReportSale, CReportConsolidationArray<CCashRSPImporterDiscountHelperSales>& arraySales, CReportConsolidationArray<CCashRSPImporterDiscountHelperPendingItem>& arrayDiscounts, double dSales, double dDiscount, int nPMSDayNumber)
{
	if ((dSales != 0.0) && (dDiscount != 0.0))
	{
		//Allow for item discounts before distributing subtotal discount
		for (int n = 0; n < arrayDiscounts.GetSize(); n++)
		{
			CCashRSPImporterDiscountHelperPendingItem infoDiscount;
			arrayDiscounts.GetAt(n, infoDiscount);

			CCashRSPImporterDiscountHelperSales infoSales;
			infoSales.m_nPluNo = infoDiscount.m_nPluNo;
			infoSales.m_nItemServerNo = 0;
			infoSales.m_dVal = -infoDiscount.m_dAmountAsDouble;
			arraySales.Consolidate(infoSales);
		}

		//Distribute subtotal discounts across items
		for (int n = 0; n < arraySales.GetSize(); n++)
		{
			CCashRSPImporterDiscountHelperSales infoSales;
			arraySales.GetAt(n, infoSales);

			if (infoSales.m_dVal != 0.0)
			{
				double dSubtotalShare = infoSales.m_dVal;
				dSubtotalShare *= dDiscount;
				dSubtotalShare /= dSales;

				CCashRSPImporterDiscountHelperPendingItem infoDiscount;
				infoDiscount.m_nPluNo = infoSales.m_nPluNo;
				//infoDiscount.m_nItemServerNo = 0;
				infoDiscount.m_dAmountAsDouble = dSubtotalShare;
				arrayDiscounts.Consolidate(infoDiscount);
			}
		}
	}

	//write discounts to sales history
	for (int n = 0; n < arrayDiscounts.GetSize(); n++)
	{
		CCashRSPImporterDiscountHelperPendingItem infoDiscount;
		arrayDiscounts.GetAt(n, infoDiscount);

		if (infoDiscount.m_dAmountAsDouble != 0.0)
		{
			//ANALYSIS CATEGORY IS BEING USED AS A LINE NUMBER TO TIE DISCOUNT TO SALE
			CCashRSPImporterDiscountHelperSales infoSale;
			infoSale.m_nPluNo = infoDiscount.m_nPluNo;
			infoSale.m_nAnalysisCategory = infoDiscount.m_nAnalysisCategory;
			infoSale.m_nItemServerNo = -1;

			{
				int nPos = 0;
				arraySales.Find(infoSale, nPos);
				if ((nPos >= 0) && (nPos < arraySales.GetSize()))
				{
					CCashRSPImporterDiscountHelperSales infoSaleCheck;
					arraySales.GetAt(nPos, infoSaleCheck);

					if ((infoSale.m_nPluNo = infoSaleCheck.m_nPluNo) && (infoSale.m_nAnalysisCategory == infoSaleCheck.m_nAnalysisCategory))
					{
						infoSale = infoSaleCheck;
					}
				}
			}

			CString strTaxBand = CTaxArray::GetTaxBandFromNumber(atoi(infoSale.m_strTaxBand), 1, "", "");

			CCSV csvOut(',', '"', FALSE, FALSE, TRUE);
			csvOut.Add("");		//TIME
			csvOut.Add(0);		//SERVER NO
			csvOut.Add(infoDiscount.m_nPluNo);
			csvOut.Add(infoSale.m_nPriceBand);
			csvOut.Add(0);		//QTY
			csvOut.Add(infoDiscount.m_dAmountAsDouble, 4);
			csvOut.Add(infoSale.m_nDeptNo);
			csvOut.Add(FALSE);	//TRAINING MODE
			csvOut.Add(1);		//DISCOUNT FLAG
			csvOut.Add(0);		//PAYMENT NUMBER
			csvOut.Add(0);		//WASTAGE FLAG
			csvOut.Add(nPMSDayNumber);
			csvOut.Add(infoSale.m_nGroupNo + 1);
			csvOut.Add(0);		//COVERS
			csvOut.Add(strTaxBand);

			arrayReportSale.Add(csvOut.GetLine());
		}
	}
}

/**********************************************************************/

void CPMSInvoiceArray::PrepareDBBCreditsItem( CPMSBookingCSVRecord& BookingRecord, int nDinnerTaxBand, double& dItemSpend, double& dPostCheckInPaid, double& dAllowance, CReportConsolidationArray<CConsolidatedDoubleByString>& arraySales, bool bAddCredits )
{
	PMSOptions.LoadDinnerDepartmentBuffer( BookingRecord );

	dItemSpend = 0.0;
	dPostCheckInPaid = 0.0;
	dAllowance = 0.0;

	bool bDBBTransaction = FALSE;
	double dDiscountBuffer = 0.0;
	CString strDate = "";

	CArray<int,int> arrayDiscountsToDelete;
	CArray<int,int> arrayDiscountsToDeleteBuffer;
	
	for ( int n = 0; n < GetInvoiceItemCount(); n++ )
	{
		CPMSInvoiceLineInfo InvoiceItem;
		GetInvoiceItem( n, InvoiceItem );

		switch( InvoiceItem.m_nLineType )
		{
		case PMS_LINE_TRAN:
	
			if ( FALSE == bDBBTransaction )
				dItemSpend -= dDiscountBuffer;
			else if ( TRUE == bAddCredits )
			{
				for ( int x = 0; x < arrayDiscountsToDeleteBuffer.GetSize(); x++ )
					arrayDiscountsToDelete.Add( arrayDiscountsToDeleteBuffer.GetAt(x) );
			}

			strDate.Format( "%4.4d%2.2d%2.2d",
				InvoiceItem.m_dateSale.GetYear(),
				InvoiceItem.m_dateSale.GetMonth(),
				InvoiceItem.m_dateSale.GetDay() );

			bDBBTransaction = FALSE;
			dDiscountBuffer = 0.0;
			arrayDiscountsToDeleteBuffer.RemoveAll();

			break;

		case PMS_LINE_SALE:
		case PMS_LINE_REFUND:
			{
				double dValue = InvoiceItem.m_dValue;
				if ( PMS_LINE_REFUND == InvoiceItem.m_nLineType )
					dValue *= -1;

				dItemSpend += dValue;
				
				dDiscountBuffer += InvoiceItem.m_dDiscount;
				arrayDiscountsToDeleteBuffer.Add(n);
				
				if ( strDate != "" )
				{
					if ( PMSOptions.CheckDinnerDepartmentBuffer( InvoiceItem.m_nDeptNo ) == TRUE )
					{
						if ( InvoiceItem.m_nTaxBand == nDinnerTaxBand )
						{
							bDBBTransaction = TRUE;

							CConsolidatedDoubleByString infoSale;
							infoSale.m_strKey = strDate;
							infoSale.m_dVal = dValue;
							arraySales.Consolidate( infoSale );
						}
					}
				}
			}
			break;

		case PMS_LINE_ALLOWANCE:
			dAllowance += InvoiceItem.m_dAllowance;
			break;

		case PMS_LINE_SDISC:
		case PMS_LINE_LDISC:
			dDiscountBuffer += InvoiceItem.m_dValue;
			arrayDiscountsToDeleteBuffer.Add(n);
			break;

		case PMS_LINE_PAYMENT:
			dPostCheckInPaid += InvoiceItem.m_dValue;
			break;

		case PMS_LINE_ACCOM:
		case PMS_LINE_EXTRABED:
		case PMS_LINE_EXTRACOT:
		case PMS_LINE_DEPOSIT:
		case PMS_LINE_BREAKFAST:
		case PMS_LINE_DINNER:
			break;
		}
	}

	if ( FALSE == bDBBTransaction )
		dItemSpend -= dDiscountBuffer;
	
	if ( TRUE == bAddCredits )
	{
		if ( TRUE == bDBBTransaction ) 
		{
			for ( int x = 0; x < arrayDiscountsToDeleteBuffer.GetSize(); x++ )
				arrayDiscountsToDelete.Add( arrayDiscountsToDeleteBuffer.GetAt(x) );
		}

		//REMOVE SUBTOTAL AND ITEM DISCOUNTS FOR DINNER TRANSACTIONS
		for ( int n = arrayDiscountsToDelete.GetSize() - 1; n >= 0; n-- )
		{
			int nIdx = arrayDiscountsToDelete.GetAt(n);

			CPMSInvoiceLineInfo InvoiceItem;
			GetInvoiceItem( nIdx, InvoiceItem );

			switch( InvoiceItem.m_nLineType )
			{
			case PMS_LINE_SALE:
			case PMS_LINE_REFUND:
				InvoiceItem.m_dDiscount = 0.0;
				SetInvoiceItem( nIdx, InvoiceItem );
				break;

			case PMS_LINE_SDISC:
			case PMS_LINE_LDISC:
				RemoveInvoiceItem( nIdx );
				break;
			}
		}
				
		//ADD CREDITS FOR DINNER TRANSACTIONS
		for ( int n = 0; n < arraySales.GetSize(); n++ )
		{
			CConsolidatedDoubleByString infoSale;
			arraySales.GetAt( n, infoSale );

			double dDiscountValue = 0.0;
			double dAllowance = BookingRecord.GetDinnerAllowance() * BookingRecord.GetGuests();
			double dDinnerSales = infoSale.m_dVal;
						
			if ( dDinnerSales >= dAllowance )
				dDiscountValue = dAllowance;
			else if ( dDinnerSales > 0.0 )
				dDiscountValue = dDinnerSales;

			if ( dDiscountValue != 0.0 )
			{
				CPMSBookingCSVRecord BookingRecord;
				CPMSInvoiceLineInfo infoTrans, infoDiscount;
				BookingRecord.GetDinnerAdjustmentLine( strDate, dDiscountValue, infoTrans, infoDiscount, TRUE );
				AddInvoiceItem( infoTrans );
				AddInvoiceItem( infoDiscount );
			}
		}
	}
}

/**********************************************************************/

void CPMSInvoiceArray::PrepareDBBCreditsGroup( CPMSBookingCSVRecord& BookingRecord, int nDinnerTaxBand )
{
	PMSOptions.LoadDinnerDepartmentBuffer( BookingRecord );

	bool bDBBTransaction = FALSE;
	CString strDate = "";

	CReportConsolidationArray<CConsolidatedDoubleByString> arraySales;
	CArray<int,int> arrayDiscountsToDelete;
	CArray<int,int> arrayDiscountsToDeleteBuffer;
	
	for ( int n = 0; n < GetInvoiceItemCount(); n++ )
	{
		CPMSInvoiceLineInfo InvoiceItem;
		GetInvoiceItem( n, InvoiceItem );

		switch( InvoiceItem.m_nLineType )
		{
		case PMS_LINE_TRAN:
	
			if ( TRUE == bDBBTransaction )
			{
				for ( int x = 0; x < arrayDiscountsToDeleteBuffer.GetSize(); x++ )
					arrayDiscountsToDelete.Add( arrayDiscountsToDeleteBuffer.GetAt(x) );
			}

			strDate.Format( "%4.4d%2.2d%2.2d",
				InvoiceItem.m_dateSale.GetYear(),
				InvoiceItem.m_dateSale.GetMonth(),
				InvoiceItem.m_dateSale.GetDay() );

			bDBBTransaction = FALSE;
			arrayDiscountsToDeleteBuffer.RemoveAll();

			break;

		case PMS_LINE_SALE:
		case PMS_LINE_REFUND:
			{
				if ( strDate != "" )
				{
					if ( PMSOptions.CheckDinnerDepartmentBuffer( InvoiceItem.m_nDeptNo ) == TRUE )
					{
						if ( InvoiceItem.m_nTaxBand == nDinnerTaxBand )
						{
							double dValue = InvoiceItem.m_dValue;
							if ( PMS_LINE_REFUND == InvoiceItem.m_nLineType )
								dValue *= -1;

							bDBBTransaction = TRUE;

							CConsolidatedDoubleByString infoSale;
							infoSale.m_strKey = strDate;
							infoSale.m_dVal = dValue;
							arraySales.Consolidate( infoSale );
						}
					}

					arrayDiscountsToDeleteBuffer.Add(n);
				}
			}
			break;

		case PMS_LINE_SDISC:
		case PMS_LINE_LDISC:
			arrayDiscountsToDeleteBuffer.Add(n);
			break;
		}
	}
	
	if ( TRUE == bDBBTransaction ) 
	{
		for ( int x = 0; x < arrayDiscountsToDeleteBuffer.GetSize(); x++ )
			arrayDiscountsToDelete.Add( arrayDiscountsToDeleteBuffer.GetAt(x) );
	}

	//REMOVE SUBTOTAL AND ITEM DISCOUNTS FOR DINNER TRANSACTIONS
	for ( int n = arrayDiscountsToDelete.GetSize() - 1; n >= 0; n-- )
	{
		int nIdx = arrayDiscountsToDelete.GetAt(n);

		CPMSInvoiceLineInfo InvoiceItem;
		GetInvoiceItem( nIdx, InvoiceItem );

		switch( InvoiceItem.m_nLineType )
		{
		case PMS_LINE_SALE:
		case PMS_LINE_REFUND:
			{
				bool bDinnerItem = FALSE;
				
				if ( PMSOptions.CheckDinnerDepartmentBuffer( InvoiceItem.m_nDeptNo ) == TRUE )
				{
					if ( InvoiceItem.m_nTaxBand == nDinnerTaxBand )
					{
						RemoveInvoiceItem( nIdx );
						bDinnerItem = TRUE;
					}
				}

				if ( FALSE == bDinnerItem )
				{
					InvoiceItem.m_dDiscount = 0.0;
					SetInvoiceItem( nIdx, InvoiceItem );
				}
			}
			break;

		case PMS_LINE_SDISC:
		case PMS_LINE_LDISC:
			RemoveInvoiceItem( nIdx );
			break;
		}
	}
				
	//ADD SUPPLEMENTS FOR DINNER TRANSACTIONS
	for ( int n = 0; n < arraySales.GetSize(); n++ )
	{
		CConsolidatedDoubleByString infoSale;
		arraySales.GetAt( n, infoSale );

		double dAllowance = BookingRecord.GetDinnerAllowance() * BookingRecord.GetGuests();
		double dDinnerSales = infoSale.m_dVal;
					
		if ( CPriceHelpers::CompareDoubles( dDinnerSales, dAllowance, 3 ) == 1 )
		{
			double dSupplementValue = dDinnerSales - dAllowance;
		
			CPMSBookingCSVRecord BookingRecord;
			CPMSInvoiceLineInfo infoTrans, infoSupplement;
			BookingRecord.GetDinnerAdjustmentLine( strDate, -dSupplementValue, infoTrans, infoSupplement, FALSE );
			AddInvoiceItem( infoTrans );
			AddInvoiceItem( infoSupplement );
		}
	}
}

/**********************************************************************/

void CPMSInvoiceArray::RebuildTaxBands( int nBookingId, int nAccountNo )
{
	bool bChanged = FALSE;

	CStringArray arrayTemp;

	CString strTransFile;
	strTransFile.Format( "%s\\%d.%d",
		PMSOptions.GetPMSFolderSales(),
		nBookingId,
		nAccountNo );

	{
		CSSFile fileTemp;
		if ( fileTemp.Open( strTransFile, "rb" ) == TRUE )
		{
			CString strBuffer;
			while( fileTemp.ReadString( strBuffer ) == TRUE )
			{
				CString strLineType = strBuffer.SpanExcluding( "," );

				if ( ( strLineType == "$ITEM_EDIT" ) || ( strLineType == "$REFUND_EDIT" ) )
				{
					CPMSTermLineEdit csv( strBuffer );

					CSQLPluNoInfo infoPluNo;
					infoPluNo.m_nEposPluNo = csv.GetItemPluNo();
					::ProcessPluNo( infoPluNo );

					if ( TRUE == infoPluNo.m_bValid )
					{
						int nPluIdx;
						if ( DataManager.Plu.FindPluByNumber( infoPluNo.m_nBasePluNo, nPluIdx ) == TRUE )
						{
							CPluCSVRecord PluRecord;
							DataManager.Plu.GetAt( nPluIdx, PluRecord );
							csv.SetItemNumericTaxBand( PluRecord.GetModifierTaxBandIntForReport( infoPluNo.m_nModifier ) );
							strBuffer = csv.GetLine();
							bChanged = TRUE;
						}
					}
				}

				arrayTemp.Add( strBuffer );
			}
		}
	}

	if ( TRUE == bChanged )
	{
		CSSFile fileTemp;
		if ( fileTemp.Open( strTransFile, "wb" ) == TRUE )
		{
			for ( int n = 0; n < arrayTemp.GetSize(); n++ )
				fileTemp.WriteLine( arrayTemp.GetAt(n) );
		}
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
