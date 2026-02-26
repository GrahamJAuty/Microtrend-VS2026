#pragma once
/**********************************************************************/

class CReportPluInfoFinder
{
public:
	CReportPluInfoFinder();
	CReportPluInfoFinder( __int64 nPluNo );
	CReportPluInfoFinder( CCSV& csvIn, int nLineType );

public:
	void SetPluNo( __int64 nPluNo );
	void SetPluNo( CCSV& csvIn, int nLineType );

public:
	bool PluExists(){ return m_bPluExists; }
	bool IsUnknownSpecialPlu(){ return m_bIsUnknownSpecialPlu; }

public:
	int GetPluIdx(){ return m_bPluExists ? m_nPluIdx : -1; }
	int GetSupplierNo(){ return m_bPluExists ? m_PluRecord.GetSupplierNo() : 0; }
	double GetPrice( int nLevel ){ return m_bPluExists ? m_PluRecord.GetPrice(nLevel) : 0.0; }
	const char* GetSupplierRef();

public:
	const char* GetPluInfoEcrText( bool bKludgeSpecialPlu,	int nMod = 0, bool bModRepText = FALSE );
	const char* GetPluInfoRepText( bool bKludgeSpecialPlu,	int nMod = 0, bool bModRepText = FALSE );
	const char* GetPluInfoSortCode( bool bKludgeSpecialPlu, int nMod = 0, bool bModRepText = FALSE );
	int GetModifierDeptNo( int nMod );
	int GetModifierAnalysisCategory( int nMod );
	int GetModifierTaxBand( int nMod );
	double GetCost();

public:
	int GetImportLineLevel(){ return m_nImportLineLevel; }
	double GetImportLineQty(){ return m_dImportLineQty; }
	double GetImportLineVal(){ return m_dImportLineVal; }

private:
	void Init();
	void CheckText( bool bKludgeSpecialPlu,	int nMod, bool bModRepText );
	void UpdatePluText( int nMod, bool bModRepText );
	void UpdateSpecialPluText();
	bool CheckSpecialPluNo(__int64 nPluNo);
	
private:
	int m_nDbIdx;
	int m_nPluIdx;
	__int64 m_nPluNo;
	CPluCSVRecord m_PluRecord;
	bool m_bPluExists;
	bool m_bInConstructor;

private:
	bool m_bGotCost;
	double m_dCost;
	bool m_bIsUnknownSpecialPlu;
	bool m_bGotUnknownSpecialPluText;
	int m_nModTextLevel;
	bool m_bModRepText;
	CString m_strSpecialPluText;
	CString m_strEcrText;
	CString m_strRepText;
	CString m_strSortCode;
	CString m_strSupplierRef;

private:
	int m_nImportLineLevel;
	double m_dImportLineQty;
	double m_dImportLineVal;
};

/**********************************************************************/
