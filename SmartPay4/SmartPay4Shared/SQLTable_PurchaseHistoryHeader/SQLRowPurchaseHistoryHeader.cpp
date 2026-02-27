//**********************************************************************
#include "SQLRowPurchaseHistoryHeader.h"
//**********************************************************************

CSQLRowPurchaseHistoryHeader::CSQLRowPurchaseHistoryHeader()
{
	Reset();
}

//**********************************************************************

void CSQLRowPurchaseHistoryHeader::Reset()
{
	m_nLineID = 1;
	m_strDate = "";
	m_strTime = "";
}

//**********************************************************************

