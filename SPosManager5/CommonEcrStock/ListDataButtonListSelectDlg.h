#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersButtonListSelect : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersButtonListSelect();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}
};

/**********************************************************************/

class CListDataButtonListSelectDlg : public CListDataDlg
{
public:
	CListDataButtonListSelectDlg( int nInitialSel, CWnd* pParent = NULL); 
	~CListDataButtonListSelectDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDOK; }
	virtual void HandleDefaultButton() { OnOK(); }

private:
	int m_nInitialSel;
};

/**********************************************************************/
#endif
/**********************************************************************/

