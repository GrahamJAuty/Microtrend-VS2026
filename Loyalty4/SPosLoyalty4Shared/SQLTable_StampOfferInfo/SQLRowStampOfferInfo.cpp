//**********************************************************************
#include "SQLFieldValidatorStampOfferInfo.h"
//**********************************************************************
#include "SQLRowStampOfferInfo.h"
//**********************************************************************

CSQLRowStampOfferInfo::CSQLRowStampOfferInfo()
{
	Reset();
}

//**********************************************************************

void CSQLRowStampOfferInfo::Reset()
{
	m_nStampOfferID = 1;
	m_nEdition = 1;
	m_bEnabled = FALSE;
	m_nThreshold = 1;
	m_nExpiry = 0;
	m_strDescription = "";
	m_nPluCount = 0;
}

//**********************************************************************

CString CSQLRowStampOfferInfo::GetDisplayDescription()
{
	CString strResult = m_strDescription;
	if (strResult == "")
	{
		if (TRUE == m_bEnabled)
		{
			strResult.Format("Stamp Offer %d", m_nStampOfferID);
		}
		else
		{
			strResult = "----";
		}
	}

	return strResult;
}

//**********************************************************************
