#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "FunctionAuthArray.h"
#include "ListDataDlg.h"
#include "LocationSelectorEntity.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersFunctionInfo : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersFunctionInfo( int nConLevel );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){};
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
	
private:
	int m_nConLevel;
	CArray<int,int> m_arrayConLevels;
};

/**********************************************************************/

class CListDataFunctionInfoDlg : public CListDataDlg
{
public:
	CListDataFunctionInfoDlg( int nConLevel, int nEntityIdx, CWnd* pParent = NULL); 
	~CListDataFunctionInfoDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();
	
	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//EDIT
	virtual void HandleButton3();	//RESET
	virtual void HandleDefaultButton() { HandleButton1(); }
	
private:
	int m_nConLevel;
	int m_nEntityIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
