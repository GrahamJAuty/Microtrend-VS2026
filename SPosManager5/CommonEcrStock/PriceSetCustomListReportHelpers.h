#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CCSVReportLine.h"
/**********************************************************************/

struct CPriceSetCustomListFieldInfo
{
public:
	int m_nFieldType;
	bool m_bPriceSet;
};

/**********************************************************************/

struct CPriceSetCustomListLocInfo
{
public:
	CPriceSetCustomListLocInfo();

public:
	void UpdateLocPluCache( __int64 nPluNo );
	bool GetModifierPrice( int nMod, int nPrice, double& dValue );
	bool GetModifierEnableFlag( int nMod, bool& bValue );
	bool GetModifierEposTextFlag( int nMod, bool& bValue );
	bool GetModifierKeyTextFlag( int nMod, bool& bValue );
	bool GetModifierEposText( int nMod, CString& strValue );
	bool GetModifierKeyText( int nMod, CString& strValue );
	bool GetModifierOrderGroup( int nMod, int& nValue );
	bool GetModifierMenu( int nMod, int& nValue );
	bool GetModifierQualifier( int nMod, int& nValue );
	bool GetModifierExtra( int nMod, int& nValue );
	bool GetModifierTaxBand(int nMod, CString& strValue);
	bool GetReportCost( double& dValue );
	bool GetPriority( int& nValue );

private:
	bool GetDouble( int nCol, double& dValue );
	bool GetInt( int nCol, int& nValue );
	bool GetBool( int nCol, bool& bValue );
	bool GetString( int nCol, CString& strValue );

private:
	bool FindPluNo( __int64 nPluNo, int& nIndex );
	
public:
	int m_nLocIdx;

public:
	int m_nModifierEposTextColumn[10];
	int m_nModifierKeyTextColumn[10];
	int m_nModifierPriceColumn[10][10];
	int m_nModifierEnableColumn[10];
	int m_nModifierKeyTextFlagColumn[10];
	int m_nModifierEposTextFlagColumn[10];
	int m_nModifierOrderGroupColumn[10];
	int m_nModifierMenuColumn[10];
	int m_nModifierQualifierColumn[10];
	int m_nModifierExtraColumn[10];
	int m_nModifierTaxBandColumn[10];
	int m_nReportCostColumn;
	int m_nPriorityColumn;
	
public:
	__int64 m_nPluNo;
	bool m_bGotPlu;
	CCSV* m_pPluCSVLine;
	CStringArray* m_pPluItems;
};

/**********************************************************************/

class CPriceSetCustomListReportHelpers
{
public:
	CPriceSetCustomListReportHelpers( int nPriceSet );
	~CPriceSetCustomListReportHelpers();

public:
	void SetFieldList( const char* szFields );
	void AddLocation( int nLocIdx );

public:
	int GetPriceSetNo(){ return m_nPriceSet; }
	int GetLocationCount(){ return m_arrayLocInfo.GetSize(); }
	int GetLocIdx( int n );
	
public:
	int GetFieldCount(){ return m_arrayFields.GetSize(); }
	void GetFieldInfo( int n, CPriceSetCustomListFieldInfo& info );
	const char* GetFieldKey(){ return m_strFieldKey; }

public:
	void AppendPriceSetPrices( CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nMod, int nPrice, bool bPriceSet );
	void AppendPriceSetEposTexts( CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nMod, bool bPriceSet );
	void AppendPriceSetKeyTexts( CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nMod, bool bPriceSet );
	void AppendPriceSetOrderGroups( CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nMod, bool bPriceSet );
	void AppendPriceSetTaxBands(CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nMod, bool bPriceSet);
	void AppendPriceSetInt( CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nType, int nMod, bool bPriceSet );
	void AppendPriceSetDouble( CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nType, bool bPriceSet );
	
private:
	int FindColumn( CString& strLabel, CReportConsolidationArray<CSortedIntByString>& arrayColumns );
	
private:
	bool GotPlu()													{ return m_arrayLocInfo[m_nLocInfoIdx].m_bGotPlu; }
	void UpdateLocPluCache( __int64 nPluNo )						{ m_arrayLocInfo[m_nLocInfoIdx].UpdateLocPluCache( nPluNo ); }
	bool GetModifierPrice( int nMod, int nPrice, double& dValue )	{ return m_arrayLocInfo[m_nLocInfoIdx].GetModifierPrice( nMod, nPrice, dValue ); }
	bool GetModifierEnableFlag( int nMod, bool& bValue )			{ return m_arrayLocInfo[m_nLocInfoIdx].GetModifierEnableFlag( nMod, bValue ); }
	bool GetModifierEposTextFlag( int nMod, bool& bValue )			{ return m_arrayLocInfo[m_nLocInfoIdx].GetModifierEposTextFlag( nMod, bValue ); }
	bool GetModifierKeyTextFlag( int nMod, bool& bValue )			{ return m_arrayLocInfo[m_nLocInfoIdx].GetModifierKeyTextFlag( nMod, bValue ); }
	bool GetModifierEposText( int nMod, CString& strValue )			{ return m_arrayLocInfo[m_nLocInfoIdx].GetModifierEposText( nMod, strValue ); }
	bool GetModifierKeyText( int nMod, CString& strValue )			{ return m_arrayLocInfo[m_nLocInfoIdx].GetModifierKeyText( nMod, strValue ); }
	bool GetModifierOrderGroup( int nMod, int& nValue )				{ return m_arrayLocInfo[m_nLocInfoIdx].GetModifierOrderGroup( nMod, nValue ); }
	bool GetModifierMenu( int nMod, int& nValue )					{ return m_arrayLocInfo[m_nLocInfoIdx].GetModifierMenu( nMod, nValue ); }
	bool GetModifierQualifier( int nMod, int& nValue )				{ return m_arrayLocInfo[m_nLocInfoIdx].GetModifierQualifier( nMod, nValue ); }
	bool GetModifierExtra( int nMod, int& nValue )					{ return m_arrayLocInfo[m_nLocInfoIdx].GetModifierExtra( nMod, nValue ); }
	bool GetModifierTaxBand(int nMod, CString& strValue)			{ return m_arrayLocInfo[m_nLocInfoIdx].GetModifierTaxBand(nMod, strValue); }
	bool GetReportCost( double& dValue )							{ return m_arrayLocInfo[m_nLocInfoIdx].GetReportCost( dValue ); }
	bool GetPriority( int& nValue )									{ return m_arrayLocInfo[m_nLocInfoIdx].GetPriority( nValue ); }

private:
	int m_nPriceSet;
	int m_nLocInfoIdx;
	CArray<CPriceSetCustomListFieldInfo,CPriceSetCustomListFieldInfo> m_arrayFields;
	CArray<CPriceSetCustomListLocInfo,CPriceSetCustomListLocInfo> m_arrayLocInfo;
	CString m_strFieldKey;
	CString m_strPriceSetColour;
};

/**********************************************************************/
#endif
/**********************************************************************/
