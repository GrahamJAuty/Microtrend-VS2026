//**********************************************************************
#include "SQLRowBromComPendingItem.h"
//**********************************************************************

CSQLRowBromComPendingItem::CSQLRowBromComPendingItem()
{
	Reset();
}

//**********************************************************************

void CSQLRowBromComPendingItem::Reset()
{
	m_nOrderID = 1;
	m_nPersonID = 1;
	m_dAmount = 0.0;
}

//**********************************************************************

