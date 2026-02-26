#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "LocationSelectorEntity.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersServerPreset : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersServerPreset( int nConLevel, int nEntityIdx );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){};
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
	
private:
	int m_nConLevel;
	int m_nEntityIdx;
	CArray<int,int> m_arrayConLevels;
};

/**********************************************************************/

class CListDataServerPresetDlg : public CListDataDlg
{
public:
	CListDataServerPresetDlg( int nConLevel, int nEntityIdx, int nTermIdx, CWnd* pParent = NULL); 
	~CListDataServerPresetDlg();

public:
	bool GetCopyToLocationFlag(){ return m_bCopyToLocation; }
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();
	
	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//EDIT
	virtual void HandleButton3();	//RESET
	virtual void HandleButton5();	//COPY
	virtual void HandleDefaultButton() { HandleButton1(); }
	
private:
	int m_nConLevel;
	int m_nEntityIdx;
	int m_nTermIdx;

private:
	bool m_bCopyToLocation;
};

/**********************************************************************/
#endif
/**********************************************************************/
