/**********************************************************************/
#include "SmartEntertainmentTransaction.h"
/**********************************************************************/

CSmartEntertainmentTransaction::CSmartEntertainmentTransaction(void)
{
	Reset();
}

/**********************************************************************/

CSmartEntertainmentTransaction::~CSmartEntertainmentTransaction(void)
{
}

/**********************************************************************/

void CSmartEntertainmentTransaction::Reset()
{
	m_strSaleDate = "";
	m_strPaymentDate = "";
	m_strTranId = "";
	m_strAmount = "";
	m_nQty = 0;
	m_arrayItems.RemoveAll();
	m_arrayPayments.RemoveAll();
}

/**********************************************************************/

const char* CSmartEntertainmentTransaction::GetCashRSPSaleDate()
{
	return GetCashRSPDate( m_strSaleDate );
}

/**********************************************************************/

const char* CSmartEntertainmentTransaction::GetCashRSPPaymentDate()
{
	return GetCashRSPDate( m_strPaymentDate );
}

/**********************************************************************/

const char* CSmartEntertainmentTransaction::GetCashRSPDate( CString& strDateTime )
{
	m_strCashRSPDate = "";
	if ( strDateTime.GetLength() == 19 )
	{
		m_strCashRSPDate = "";
		m_strCashRSPDate += strDateTime.Mid(8,2);
		m_strCashRSPDate += "/";
		m_strCashRSPDate += strDateTime.Mid(5,2);
		m_strCashRSPDate += "/";
		m_strCashRSPDate += strDateTime.Mid(2,2);
	}

	return m_strCashRSPDate;
}

/**********************************************************************/

const char* CSmartEntertainmentTransaction::GetCashRSPSaleTime()
{
	return GetCashRSPTime( m_strSaleDate );
}

/**********************************************************************/

const char* CSmartEntertainmentTransaction::GetCashRSPPaymentTime()
{
	return GetCashRSPTime( m_strPaymentDate );
}

/**********************************************************************/

const char* CSmartEntertainmentTransaction::GetCashRSPTime( CString& strDateTime )
{
	m_strCashRSPTime = "";
	if ( strDateTime.GetLength() == 19 )
		m_strCashRSPTime = strDateTime.Right(8);

	return m_strCashRSPTime;
}

/**********************************************************************/

void CSmartEntertainmentTransaction::AddItem( CSmartEntertainmentItem& Item )
{
	m_arrayItems.Add( Item );
	m_nQty += Item.GetQty();
}

/**********************************************************************/

void CSmartEntertainmentTransaction::GetItem( int nIdx, CSmartEntertainmentItem& Item )
{
	Item.Reset();

	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayItems.GetSize() ) )
		Item = m_arrayItems.GetAt( nIdx );
}

/**********************************************************************/

void CSmartEntertainmentTransaction::AddPayment( CSmartEntertainmentPayment& Payment )
{
	m_arrayPayments.Add( Payment );
}

/**********************************************************************/

void CSmartEntertainmentTransaction::GetPayment( int nIdx, CSmartEntertainmentPayment& Payment )
{
	Payment.Reset();

	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayPayments.GetSize() ) )
		Payment = m_arrayPayments.GetAt( nIdx );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSmartEntertainmentItem::CSmartEntertainmentItem(void)
{
	Reset();
}

/**********************************************************************/

CSmartEntertainmentItem::~CSmartEntertainmentItem(void)
{
}

/**********************************************************************/

void CSmartEntertainmentItem::Reset()
{
	m_strPluNo = "";
	m_nDeptNo = 0;
	m_nQty = 0;
	m_strLineCost = "";
	m_strDescription = "";
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSmartEntertainmentPayment::CSmartEntertainmentPayment(void)
{
	Reset();
}

/**********************************************************************/

CSmartEntertainmentPayment::~CSmartEntertainmentPayment(void)
{
}

/**********************************************************************/

void CSmartEntertainmentPayment::Reset()
{
	m_nID = 0;
	m_strAmount = "";
}

/**********************************************************************/
