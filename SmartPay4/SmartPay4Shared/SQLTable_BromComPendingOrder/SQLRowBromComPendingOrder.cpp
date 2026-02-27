//**********************************************************************
#include "SQLRowBromComPendingOrder.h"
//**********************************************************************

CSQLRowBromComPendingOrder::CSQLRowBromComPendingOrder()
{
	Reset();
}

//**********************************************************************

void CSQLRowBromComPendingOrder::Reset()
{
	m_nOrderID = 1;
	m_strStatus = "";
	m_strLastModified = "";
	m_bNewOrder = FALSE;
}

//**********************************************************************

