//**********************************************************************
#include "SQLRowPebbleExportTransaction.h"
//**********************************************************************

CSQLRowPebbleExportTransaction::CSQLRowPebbleExportTransaction()
{
	Reset();
}

//**********************************************************************

void CSQLRowPebbleExportTransaction::Reset()
{
	m_nLineID = 1;
	m_nUserID = 1;
	m_nXFerUserID = 0; 
	m_strMemberID = "";
	m_strDate = "";
	m_strTime = "";
	m_nCCNo = 0;
	m_nSourceType = 0;
	m_nAppNo = 0;
	m_dAmount = 0.0;
	m_dPurse1 = 0.0;
	m_dPurse2 = 0.0;
	m_dCash = 0.0;
	m_strWebComment = "";
}

//**********************************************************************