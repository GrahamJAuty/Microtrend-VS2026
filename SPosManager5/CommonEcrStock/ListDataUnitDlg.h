#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DefTextUnit.h"
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersUnit : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersUnit( CDefTextUnit& UnitText );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

private:
	CDefTextUnit& m_UnitText;
};

/**********************************************************************/

class CListDataUnitDlg : public CListDataDlg
{
public:
	CListDataUnitDlg( CDefTextUnit& UnitText, const char* szUnit, CWnd* pParent = NULL); 
	~CListDataUnitDlg();

public:
	void SetTitleInfo( const char* sz ){ m_strTitleInfo = sz; }
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//MOVE UP
	virtual void HandleButton5();	//MOVE DOWN
	virtual void HandleButton6();	//DEFAULTS
	virtual void HandleDefaultButton() { HandleButton2(); }
	
public:
	const char* GetSelectedText();

private:
	CDefTextUnit& m_UnitText;
	CString m_strInitialUnit;
	CString m_strSelectedText;
	CString m_strTitleInfo;
	int m_nMaxLength;
	int m_nType;
};

/**********************************************************************/
#endif
/**********************************************************************/
