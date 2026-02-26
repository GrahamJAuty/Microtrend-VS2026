//**********************************************************************
#include "SQLFieldValidatorPluInfo.h"
//**********************************************************************
#include "SQLRowPluInfo.h"
//**********************************************************************

CSQLRowPluInfo::CSQLRowPluInfo()
{
	Reset();
}

//**********************************************************************

void CSQLRowPluInfo::Reset()
{
	m_nDbNo = 0;
	m_nPluNo = 0;
	m_strDescription = "";
	m_nDeptNo = 0;
	m_strTaxCode = "A";
	m_nPoints = 0;
	m_nModType = 0;
	m_nStampOfferID = 0;
}

//**********************************************************************

CString CSQLRowPluInfo::GetPluNoString()
{
	CString strPluNo = ::SimpleFormatInt64Value(GetPluNo());

	int nLength = strPluNo.GetLength();
	int nMaxPadout = nLength;

	if (nLength <= 6)
	{
		nMaxPadout = 6;
	}
	else if (nLength > 8)
	{
		nMaxPadout = 13;
	}

	::AddLeading(strPluNo, nMaxPadout, '0');
	return strPluNo;
}

//*********************************************************************

CString CSQLRowPluInfo::FormatPluNo(const char* szPluNo)
{
	CString strPluNo = szPluNo;
	int nLength = strPluNo.GetLength();
	int nMaxPadout = nLength;

	if (nLength <= 6)
	{
		nMaxPadout = 6;
	}
	else if (nLength > 8)
	{
		nMaxPadout = 13;
	}

	::AddLeading(strPluNo, nMaxPadout, '0');
	return strPluNo;
}

//*********************************************************************

bool CSQLRowPluInfo::IsBasePlu()
{
	bool bResult = TRUE;

	switch (m_nModType)
	{
	case 1:	//BASE PLU WITH MODIFIER DIGIT
	case 3:	//LITERAL PLU WITH NO MODIFIER DIGIT
		bResult = TRUE;
		break;

	case 2:	//MODIFIER PLU
		bResult = FALSE;
		break;

	case 0:
	default:	
		if (m_nPluNo > 9999999 )
		{
			bResult = TRUE;
		}
		else if (m_nPluNo < 10 )
		{
			bResult = FALSE;
		}
		else
		{
			bResult = ((m_nPluNo % 10) == 0);
		}
		break;
	}

	return bResult;
}

//*********************************************************************

CString CSQLRowPluInfo::GetMicrotrendBasePluNo( bool bPadout)
{
	bool bHasModifierDigit = TRUE;
	
	switch (m_nModType)
	{
	case 1:	//BASE PLU WITH MODIFIER DIGIT
	case 2: //MODIFIER PLU
		bHasModifierDigit = TRUE;
		break;

	case 3: //LITERAL PLU WITH NO MODIFIER DIGIT
		bHasModifierDigit = FALSE;
		break;

	case 0:
	default:
		bHasModifierDigit = ((m_nPluNo >= 10) && (m_nPluNo <= 9999999));
		break;
	}

	CString strBasePluNo = "";
	strBasePluNo.Format("%I64d", m_nPluNo);

	if (TRUE == bHasModifierDigit)					
	{
		strBasePluNo.Delete(strBasePluNo.GetLength() - 1, 1);
	}

	if (bPadout == TRUE)
	{
		strBasePluNo = FormatPluNo(strBasePluNo);
	}

	return strBasePluNo;						
}

//*********************************************************************
