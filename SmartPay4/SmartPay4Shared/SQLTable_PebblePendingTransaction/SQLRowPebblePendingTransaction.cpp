//**********************************************************************
#include "SQLRowPebblePendingTransaction.h"
//**********************************************************************

CSQLRowPebblePendingTransaction::CSQLRowPebblePendingTransaction()
{
	Reset();
}

//**********************************************************************

void CSQLRowPebblePendingTransaction::Reset()
{
	m_strTransactionID = "";
	m_strMemberID = "";
	m_strDateTime = "";
	m_strName = "";
	m_nAmount = 0;
	m_bIsSale = FALSE;
}

//**********************************************************************

