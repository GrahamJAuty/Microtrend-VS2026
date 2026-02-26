//**********************************************************************
#include "SQLFieldValidatorOfferStamp.h"
//**********************************************************************
#include "SQLRowOfferStamp.h"
//**********************************************************************

CSQLRowOfferStamp::CSQLRowOfferStamp()
{
	Reset();
}

//**********************************************************************

void CSQLRowOfferStamp::Reset()
{
	m_nUserID = 0;
	m_nStampOfferID = 0;
	m_nEdition = 1;
	m_strAwardDate = "";
	m_strExpireDate = "";
	m_nStampCount = 0;
}

//**********************************************************************

bool CSQLRowOfferStamp::Compare(CSQLRowOfferStamp& source)
{
	bool bResult = TRUE;
	bResult &= (m_nUserID == source.m_nUserID);
	bResult &= (m_nStampOfferID == source.m_nStampOfferID);
	bResult &= (m_nEdition == source.m_nEdition);
	bResult &= (m_strAwardDate == source.m_strAwardDate);
	bResult &= (m_strExpireDate == source.m_strExpireDate);
	return bResult;		
}

//**********************************************************************