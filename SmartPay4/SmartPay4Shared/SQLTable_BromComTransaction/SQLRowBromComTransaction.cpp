//**********************************************************************
#include "SQLRowBromComTransaction.h"
//**********************************************************************

CSQLRowBromComTransaction::CSQLRowBromComTransaction()
{
	Reset();
}

//**********************************************************************

void CSQLRowBromComTransaction::Reset()
{
	m_nLineID = 1;
	m_nPersonID = 1;
	m_strDetails = "";
	m_dAmount = 0.0;
	m_dBalance = 0.0;
	m_strDateTime = "";
	m_strType = "";
	m_nTransactionID = 0;
	m_strWebComment = "";
}

//**********************************************************************

