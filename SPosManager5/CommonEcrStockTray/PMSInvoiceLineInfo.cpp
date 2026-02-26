/**********************************************************************/
#include "PriceHelpers.h"
/**********************************************************************/
#include "PMSInvoiceLineInfo.h"
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

CPMSInvoiceLineInfo::CPMSInvoiceLineInfo()
{
	m_nSection = 0;
	m_dateSale = COleDateTime( 1980, 1, 1, 0, 0, 0 );
	m_nTransactionNo = 0;
	m_nLineType = PMS_LINE_TRAN;
	m_nBasePluNo = 1;
	m_nDeptNo = 0;
	m_nGroupNo = 0;
	m_nTaxBand = 0;
	m_nModifier = 0;
	m_nLevel = 0;
	m_strDescription = "";
	m_dEposPluQty = 0.0;
	m_dBasePluQty = 0.0;
	m_dValue = 0.0;
	m_dDiscount = 0.0;
	m_dAllowance = 0.0;
}

/**********************************************************************/

bool CPMSInvoiceLineInfo::IsBlankItemLine()
{
	switch( m_nLineType )
	{
	case PMS_LINE_SALE:
	case PMS_LINE_REFUND:
	case PMS_LINE_ALLOWANCE:
	case PMS_LINE_SDISC:
	case PMS_LINE_LDISC:
	case PMS_LINE_PAYMENT:
	case PMS_LINE_ACCOM:
	case PMS_LINE_EXTRABED:
	case PMS_LINE_EXTRACOT:
	case PMS_LINE_DEPOSIT:
	case PMS_LINE_BREAKFAST:
	case PMS_LINE_DINNER:
		break;

	case PMS_LINE_TRAN:
	default:
		return FALSE;
	}

	if ( CPriceHelpers::CompareDoubles( m_dEposPluQty, 0.0, 3 ) != 0 )
		return FALSE;

	if ( CPriceHelpers::CompareDoubles( m_dValue, 0.0, 3 ) != 0 )
		return FALSE;

	if ( CPriceHelpers::CompareDoubles( m_dDiscount, 0.0, 3 ) != 0 )
		return FALSE;

	if ( CPriceHelpers::CompareDoubles( m_dAllowance, 0.0, 3 ) != 0 )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/
