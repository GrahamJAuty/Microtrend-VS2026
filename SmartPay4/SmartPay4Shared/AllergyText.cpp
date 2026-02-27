//*******************************************************************
#include "AllergyText.h"
/**********************************************************************/

CAllergyText::CAllergyText()
{
	m_nAllergyFlags = 0;
}

/**********************************************************************/

bool CAllergyText::HaveAllergy(int nAllergyCode)
{
	int nMask = 0x01;
	if (nAllergyCode > 1)
	{
		nMask = nMask << (nAllergyCode - 1);
	}
	return ((m_nAllergyFlags & nMask) == nMask) ? TRUE : FALSE;
}

/**********************************************************************/

void CAllergyText::SetAllergy(int nAllergyCode, bool bSet)
{
	int nMask = 0x01;
	if (nAllergyCode > 1)
	{
		nMask = nMask << (nAllergyCode - 1);
	}

	if (bSet == TRUE)
	{
		m_nAllergyFlags |= nMask;
	}
	else
	{
		m_nAllergyFlags &= ~nMask;
	}
}

/**********************************************************************/

void CAllergyText::SetAllergy(const char* szText)
{
	CString strText = szText;
	strText.MakeUpper();

	::TrimSpacesFromString(strText);

	if (strText == "CLEAR")
	{
		m_nAllergyFlags = 0;						// clear any allergies
	}
	else
	{
		for (int n = 0; n < GetRecordCount(); n++)
		{
			if (MoveTo(n) == TRUE)
			{
				CString strAllergyText = GetText();
				strAllergyText.MakeUpper();

				if (strText == strAllergyText)
				{
					SetAllergy(GetNo(), TRUE);
					break;
				}
			}
		}
	}
}

//*******************************************************************

CString CAllergyText::GetAllergyString()
{
	CString strMsg = "";
	int nCount = 0;

	for (int n = 0; n < GetRecordCount(); n++)
	{
		if (MoveTo(n) == TRUE)
		{
			if (HaveAllergy(GetNo()) == TRUE)
			{
				if (nCount > 0)	strMsg += ",";
				strMsg += GetText();
				++nCount;
			}
		}
	}
	return strMsg;
}

//*******************************************************************
// number of matching allergies - given an allergy code

int CAllergyText::MatchAllergies(int nCode)
{
	int nCount = 0;
	int nMask = 0x01;

	for (int nIndex = 0; nIndex < nMAX_ALLERGIES; nIndex++)
	{
		int nFlag = (nMask << nIndex);

		int n1 = m_nAllergyFlags & nFlag;
		int n2 = nCode & nFlag;

		if ((n1 == n2) && (n1 != 0))
		{
			++nCount;
		}
	}

	return nCount;
}

//*******************************************************************
