/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSTermLineEdit.h"
/**********************************************************************/

int CPMSTermLineEdit::GetTransactionNumber()
{
	if ( GetInt(1) == 1 )
		return GetInt(2);
	else
		return 0;
}

/**********************************************************************/

const char* CPMSTermLineEdit::GetTransactionDate()
{
	if ( GetInt(1) == 1 )
		m_strTemp = GetString(3);
	else
		m_strTemp = "";

	return m_strTemp;
}

/**********************************************************************/

const char* CPMSTermLineEdit::GetTransactionTime()
{
	if ( GetInt(1) == 1 )
		m_strTemp = GetString(4);
	else
		m_strTemp = "";

	return m_strTemp;
}

/**********************************************************************/

int CPMSTermLineEdit::GetTransactionValue()
{
	if ( GetInt(1) == 1 )
		return GetInt(5);
	else
		return 0;
}

/**********************************************************************/

__int64 CPMSTermLineEdit::GetItemPluNo()
{
	if ( GetInt(1) == 1 )
		return GetInt64(2);
	else
		return 0;
}

/**********************************************************************/

double CPMSTermLineEdit::GetItemQty()
{
	if ( GetInt(1) == 1 )
		return GetDouble(3);
	else
		return 0;
}

/**********************************************************************/

int CPMSTermLineEdit::GetItemPriceBand()
{
	if ( GetInt(1) == 1 )
		return GetInt(4);
	else
		return 0;
}

/**********************************************************************/

int CPMSTermLineEdit::GetItemLinePrice()
{
	if ( GetInt(1) == 1 )
		return GetInt(5);
	else
		return 0;
}

/**********************************************************************/

int CPMSTermLineEdit::GetItemDeptNo()
{
	if ( GetInt(1) == 1 )
		return GetInt(6);
	else
		return 0;
}

/**********************************************************************/

int CPMSTermLineEdit::GetItemGroupNo()
{
	if ( GetInt(1) == 1 )
		return GetInt(7);
	else
		return 0;
}

/**********************************************************************/

const char* CPMSTermLineEdit::GetItemTaxBand()
{
	if ( GetInt(1) == 1 )
		return GetString(8);
	else
		return "";
}

/**********************************************************************/

int CPMSTermLineEdit::GetItemNumericTaxBand()
{
	return CTaxArray::GetNumericTaxBand(GetItemTaxBand(), TRUE);
}

/**********************************************************************/

void CPMSTermLineEdit::SetItemNumericTaxBand( int n )
{
	if ( GetInt(1) == 1 )
	{
		CString strTaxBand = CTaxArray::GetTaxBandFromNumber(n, 0, "?", "");

		if (strTaxBand != "?")
		{
			SetAt(8, strTaxBand);
		}
	}
}

/**********************************************************************/

__int64 CPMSTermLineEdit::GetItemDiscountPluNo()
{
	if ( GetInt(1) == 1 )
		return GetInt64(2);
	else
		return 0;
}

/**********************************************************************/

int CPMSTermLineEdit::GetItemDiscountAmount()
{
	if ( GetInt(1) == 1 )
		return GetInt(3);
	else
		return 0;
}

/**********************************************************************/

int CPMSTermLineEdit::GetSubtotalDiscountAmount()
{
	if ( GetInt(1) == 1 )
		return GetInt(2);
	else
		return 0;
}

/**********************************************************************/

int CPMSTermLineEdit::GetPaymentNo()
{
	if ( GetInt(1) == 1 )
		return GetInt(2);
	else
		return 0;
}

/**********************************************************************/

const char* CPMSTermLineEdit::GetPaymentText()
{
	if ( GetInt(1) == 1 )
		return GetString(3);
	else
		return "";
}

/**********************************************************************/

int CPMSTermLineEdit::GetPaymentAmount()
{
	if ( GetInt(1) == 1 )
		return GetInt(4);
	else
		return 0;
}

/**********************************************************************/
#endif
/**********************************************************************/
