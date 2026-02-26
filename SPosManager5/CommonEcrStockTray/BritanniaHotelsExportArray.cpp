 /**********************************************************************/
#include "BritanniaHotelsExportArray.h"
/**********************************************************************/

CBritanniaHotelsExportInfo::CBritanniaHotelsExportInfo()
{
	m_nDeptNo = 0;
	m_nBasePluNo = 0;
	m_nEposPluNo = 0;
	m_nModifier = 0;
	m_strDescription = "";

	for ( int n = 0; n < 10; n++ )
	{
		m_dPriceQty[n] = 0.0;
		m_dPriceValue[n] = 0.0;
	}

	m_dDefaultPriceRetail = 0.0;
}

/**********************************************************************/

CBritanniaHotelsExportArray::CBritanniaHotelsExportArray(void)
{
}

/**********************************************************************/

bool CBritanniaHotelsExportArray::Find( int nDeptNo, __int64 nPluNo, int& nPos )
{
	int nStart = 0;
	int nEnd = m_arrayExport.GetSize() - 1;

	while (nStart <= nEnd)
	{
		nPos = (nStart + nEnd) / 2;

		if (m_arrayExport[nPos].m_nDeptNo > nDeptNo)
		{
			nEnd = nPos - 1;
		}
		else if (m_arrayExport[nPos].m_nDeptNo < nDeptNo)
		{
			nStart = nPos + 1;
		}
		else if (m_arrayExport[nPos].m_nEposPluNo > nPluNo)
		{
			nEnd = nPos - 1;
		}
		else if (m_arrayExport[nPos].m_nEposPluNo < nPluNo)
		{
			nStart = nPos + 1;
		}
		else
		{
			return TRUE;
		}
	}

	nPos = nStart;
	return FALSE;
}

/**********************************************************************/

void CBritanniaHotelsExportArray::Consolidate(CBritanniaHotelsExportInfo& infoExport)
{
	int nPos;
	if (Find(infoExport.m_nDeptNo, infoExport.m_nEposPluNo, nPos) == FALSE)
	{
		m_arrayExport.InsertAt(nPos, infoExport);
	}
	else
	{
		for (int n = 0; n < 10; n++)
		{
			m_arrayExport[nPos].m_dPriceQty[n] += infoExport.m_dPriceQty[n];
			m_arrayExport[nPos].m_dPriceValue[n] += infoExport.m_dPriceValue[n];
		}
	}
}

/**********************************************************************/

void CBritanniaHotelsExportArray::GetAt( int nIdx, CBritanniaHotelsExportInfo& info )
{
	if ((nIdx >= 0) && (nIdx < m_arrayExport.GetSize()))
	{
		info = m_arrayExport[nIdx];
	}
}

/**********************************************************************/

void CBritanniaHotelsExportArray::SetAt(int nIdx, CBritanniaHotelsExportInfo& info)
{
	if ((nIdx >= 0) && (nIdx < m_arrayExport.GetSize()))
	{
		m_arrayExport[nIdx] = info;
	}
}

/**********************************************************************/
