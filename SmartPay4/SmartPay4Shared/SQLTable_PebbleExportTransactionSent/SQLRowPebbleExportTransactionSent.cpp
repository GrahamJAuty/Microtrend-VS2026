//**********************************************************************
#include "SQLRowPebbleExportTransactionSent.h"
//**********************************************************************

CSQLRowPebbleExportTransactionSent::CSQLRowPebbleExportTransactionSent()
{
	Reset();
}

//**********************************************************************

void CSQLRowPebbleExportTransactionSent::Reset()
{
	m_nLineID = 1;
	m_nTranLineID = 1;
}

//**********************************************************************