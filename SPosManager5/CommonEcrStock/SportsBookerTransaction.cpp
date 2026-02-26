/**********************************************************************/
#include "SportsBookerTransaction.h"
/**********************************************************************/

CSportsBookerTransaction::CSportsBookerTransaction(void)
{
	Reset();
}

/**********************************************************************/

CSportsBookerTransaction::~CSportsBookerTransaction(void)
{
}

/**********************************************************************/

void CSportsBookerTransaction::Reset()
{
	m_strDate = "";
	m_strType = "";
	m_strTranId = "";
	m_strAmount = "";
	m_strUserId = "";
	m_strPriceBand = "";
	m_nQty = 0;
	m_strPaymentId = "";
	m_strPaymentName = "";
	m_strDescription = "";
	m_arrayItems.RemoveAll();
}

/**********************************************************************/

const char* CSportsBookerTransaction::GetCashRSPDate()
{
	m_strCashRSPDate = "";
	if ( m_strDate.GetLength() == 19 )
	{
		m_strCashRSPDate = "";
		m_strCashRSPDate += m_strDate.Mid(8,2);
		m_strCashRSPDate += "/";
		m_strCashRSPDate += m_strDate.Mid(5,2);
		m_strCashRSPDate += "/";
		m_strCashRSPDate += m_strDate.Mid(2,2);
	}

	return m_strCashRSPDate;
}

/**********************************************************************/

const char* CSportsBookerTransaction::GetCashRSPTime()
{
	m_strCashRSPTime = "";
	if ( m_strDate.GetLength() == 19 )
		m_strCashRSPTime = m_strDate.Right(8);

	return m_strCashRSPTime;
}

/**********************************************************************/

void CSportsBookerTransaction::AddOrderItem( CSportsBookerOrderItem& orderItem )
{
	m_arrayItems.Add( orderItem );
	m_nQty += orderItem.GetQty();
}

/**********************************************************************/

void CSportsBookerTransaction::GetOrderItem( int nIdx, CSportsBookerOrderItem& orderItem )
{
	orderItem.Reset();

	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayItems.GetSize() ) )
		orderItem = m_arrayItems.GetAt( nIdx );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSportsBookerOrderItem::CSportsBookerOrderItem(void)
{
	Reset();
}

/**********************************************************************/

CSportsBookerOrderItem::~CSportsBookerOrderItem(void)
{
}

/**********************************************************************/

void CSportsBookerOrderItem::Reset()
{
	m_strPluNo = "";
	m_strDeptNo = "";
	m_strTaxBand = "";
	m_strPriceBand = "";

	m_nQty = 0;
	m_strCostPerItem = "";
}

/**********************************************************************/
