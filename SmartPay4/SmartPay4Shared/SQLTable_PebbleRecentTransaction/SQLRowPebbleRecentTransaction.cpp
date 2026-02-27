//**********************************************************************
#include "SQLRowPebbleRecentTransaction.h"
//**********************************************************************

CSQLRowPebbleRecentTransaction::CSQLRowPebbleRecentTransaction()
{
	Reset();
}

//**********************************************************************

void CSQLRowPebbleRecentTransaction::Reset()
{
	m_strTransactionID = "";
	m_strDateTime = "";
}

//**********************************************************************

