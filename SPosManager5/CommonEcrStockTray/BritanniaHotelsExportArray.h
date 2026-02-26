#pragma once
/**********************************************************************/

struct CBritanniaHotelsExportInfo
{
public:
	CBritanniaHotelsExportInfo();

public:
	int m_nDeptNo;
	__int64 m_nEposPluNo;
	__int64 m_nBasePluNo;
	int m_nModifier;
	CString m_strDescription;
	double m_dPriceQty[10];
	double m_dPriceValue[10];
	double m_dDefaultPriceRetail;
};

/**********************************************************************/

class CBritanniaHotelsExportArray
{
public:
	CBritanniaHotelsExportArray(void);

public:
	void Consolidate( CBritanniaHotelsExportInfo& infoExport );
	bool Find( int nDeptNo, __int64 nPluNo, int& nPos );

public:
	int GetSize(){ return m_arrayExport.GetSize(); }
	void GetAt( int nIdx, CBritanniaHotelsExportInfo& info );
	void SetAt( int nIdx, CBritanniaHotelsExportInfo& info );

private:
	CArray<CBritanniaHotelsExportInfo,CBritanniaHotelsExportInfo> m_arrayExport;
};

/**********************************************************************/
