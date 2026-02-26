#pragma once
/**********************************************************************/

class CPluAddOrCopyInfo
{
public:
	CPluAddOrCopyInfo();

public:
	__int64 NewPluNoAsInt64(){ return _atoi64( m_strNewPluNo ); }
	int MaxNewPluLen();

public:
	int m_nSourcePluIdx;
	int m_nPluRangeMode;

public:
	CString m_strNewPluNo;
	int m_nNewPluDeptNo;
	int m_nNewPluSuppNo;
	long m_lNewPluCopyCount;

public:
	bool m_bNewPluCreateStock;
	CString m_strNewPluStockCode;
	
};

/**********************************************************************/


