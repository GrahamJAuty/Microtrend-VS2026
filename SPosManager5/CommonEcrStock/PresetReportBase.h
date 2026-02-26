#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "LocationSelectorEntity.h"
/**********************************************************************/

class CPresetReportInfo
{
public:
	CPresetReportInfo();
	void Reset();

public:
	const char* GetName(){ return m_strName; }
	int GetEntityNo(){ return m_nEntityNo; }
	int GetPresetConLevel(){ return m_nPresetConLevel; }
	int GetPresetEntityNo1(){ return m_nPresetEntityNo1; }
	int GetPresetEntityNo2(){ return m_nPresetEntityNo2; }
	int GetTerminalIdx(){ return m_nTerminalIdx; }
	bool GetPasswordEnableFlag(){ return m_bPasswordEnable; }
	int GetPasswordConLevel(){ return m_nPasswordConLevel; }
	int GetPasswordEntityNo1(){ return m_nPasswordEntityNo1; }
	int GetPasswordEntityNo2(){ return m_nPasswordEntityNo2; }
	const char* GetOtherFields(){ return m_strOtherFields; }

public:
	void SetName( const char* sz ){ m_strName = sz; }
	void SetEntityNo( int n );
	void SetPresetConLevel( int n ){ m_nPresetConLevel = n; }
	void SetPresetEntityNo1( int n ){ m_nPresetEntityNo1 = n; }
	void SetPresetEntityNo2( int n ){ m_nPresetEntityNo2 = n; }
	void SetTerminalIdx( int n ){ m_nTerminalIdx = n; }
	void SetPasswordEnableFlag( bool b ){ m_bPasswordEnable = b; }
	void SetPasswordConLevel( int n ){ m_nPasswordConLevel = n; }
	void SetPasswordEntityNo1( int n ){ m_nPasswordEntityNo1 = n; }
	void SetPasswordEntityNo2( int n ){ m_nPasswordEntityNo2 = n; }
	void SetOtherFields( const char* sz ){ m_strOtherFields = sz; }

public:
	const char* GetListName();
	
private:
	CString m_strName;
	int m_nEntityNo;

private:
	int m_nPresetConLevel;
	int m_nPresetEntityNo1;
	int m_nPresetEntityNo2;
	int m_nTerminalIdx;
	bool m_bPasswordEnable;
	int m_nPasswordConLevel;
	int m_nPasswordEntityNo1;
	int m_nPasswordEntityNo2;
	CString m_strOtherFields;

private:
	CString m_strListName;
};

/**********************************************************************/

class CPresetReportArray
{
public:
	CPresetReportArray(void);
	~CPresetReportArray(void);

public:
	void CopyStrings( CPresetReportArray& source );
	void PrepareForEditing( CPresetReportArray& source );
	void PrepareForEditing();
	void ClearStrings(){ m_arrayFileStrings.RemoveAt( 0, m_arrayFileStrings.GetSize() ); }
	
protected:
	virtual const char* GetFilename(){ return ""; }
	virtual void PrepareForEditingInternal(){}
	virtual void PrepareForWrite(){}
	virtual bool IsStockPreset(){ return FALSE; }
	
public:
	int GetSize(){ return m_arrayPresets.GetSize(); }
	int GetNextFreeEntityNo();
	
public:
	int GetEntityNo( int nIdx );
	const char* GetName( int nIdx );
	void GetEntityNoAndListName( int nIdx, int& nEntityNo, CString& strName );
	
public:
	void SetEntityNo( int nIdx, int nEntityNo );
	void SetName( int nIdx, const char* sz );
	void SwapItems( int nFirst );

public:
	void GetAt( int nIdx, CPresetReportInfo& info );
	void SetAt( int nIdx, CPresetReportInfo& info );
	void InsertAt( int nIdx, CPresetReportInfo& info );
	void RemoveAt( int nIdx );
	void Add( CPresetReportInfo& info );
	
public:
	void SetPasswordAccess();
	void GetPresetConLevelInfo( int nPresetIdx, int& nConLevel, int& nEntityNo1, int& nEntityNo2 );
	void SetPasswordConLevelInfo( int nPresetIdx, bool bAllow, int nConLevel, int nEntityNo1, int nEntityNo2 );
	
public:
	static void GetLegacyConLevelInfo( int nPresetConLevel, int nLegacyIdx, int& nEntityNo1, int& nEntityNo2 );
	static void GetLocSelEntityNo( CLocationSelectorEntity& LocSelEntity, int& nEntityNo1, int& nEntityNo2 );
	static void GetEntityIdx( int nConLevel, int nEntityNo1, int nEntityNo2, int& nEntityIdx );

private:
	bool IsContainedEntity( int nOuterConLevel, int nOuterEntityNo1, int nOuterEntityNo2, int nInnerConLevel, int nInnerEntityNo1, int nInnerEntityNo2 );
	
public:
	CArray<CPresetReportInfo,CPresetReportInfo> m_arrayPresets;
	CSharedStringArray m_arrayFileStrings;

protected:
	CString m_strFilename;
	CString m_strName;

	friend class CPresetReportArrayWrapper;
};

/**********************************************************************/
#endif
/**********************************************************************/
