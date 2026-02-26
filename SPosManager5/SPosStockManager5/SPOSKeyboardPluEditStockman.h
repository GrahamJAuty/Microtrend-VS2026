#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM 
/**********************************************************************/
#include "MyCSSLockFile.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CSPOSKeyboardPluEditStockman
{
public:
	CSPOSKeyboardPluEditStockman( __int64 nPluNo, int nMode, int& nNewPluDeptNo, const char* szImageFilename, CWnd* pParent );
	~CSPOSKeyboardPluEditStockman();

public:
	bool LoadDatabase();
	bool EditPlu();
	void LoadReadOnlyDatabase();
	void SaveChanges();
	void Finalise();

public:
	__int64 GetSharpPosPluNo(){ return m_nSharpPosPluNo; }

private:
	bool LoadDatabaseMaster();
	bool LoadDatabasePriceSet( int nPriceSet );
	bool EditExistingPlu();
	bool CreateNewPlu();
	bool LinkPlu();
	void SaveChangesMaster();
	void SaveChangesPriceSet();
	void FinaliseMaster();
	void FinalisePriceSet();

private:
	bool GetNextPluNo( CString& strPluNo );
	bool EditPluInternal( int nPluIdx, bool bNewStock, bool bAllowSimple );

private:
	CPluFilterArray m_FilterArray;

private:
	__int64 m_nPluNo;
	__int64 m_nSharpPosPluNo;
	int m_nMode;
	CString m_strImageFilename;
	int& m_nNewPluDeptNo;

private:
	CWnd* m_pParent;
	CMyCSSLockFile m_PriceSetLockFile;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
