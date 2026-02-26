/**********************************************************************/
#include "PluRangeModes.h"
/**********************************************************************/
#include "PluAddOrCopyInfo.h"
/**********************************************************************/

CPluAddOrCopyInfo::CPluAddOrCopyInfo(void)
{
	m_nSourcePluIdx = -1;
	m_nPluRangeMode = PLURANGE_MODE_ADD;
	/*****/
	m_strNewPluNo = "";
	m_nNewPluDeptNo = 0;
	m_nNewPluSuppNo = 0;
	m_lNewPluCopyCount = 0;
	/*****/
	m_bNewPluCreateStock = FALSE;
	m_strNewPluStockCode = "";
}

/**********************************************************************/

int CPluAddOrCopyInfo::MaxNewPluLen()
{
	CString strTest;
	strTest.Format ( "%I64d", NewPluNoAsInt64() + m_lNewPluCopyCount - 1 );
	return strTest.GetLength();
}

/**********************************************************************/
