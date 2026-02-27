#pragma once
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersRestore : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersRestore( CStringArray& arrayBackups );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}

private:
	CStringArray& m_arrayBackups;
};

/**********************************************************************/

class CListDataRestoreDlg : public CListDataDlg
{
public:
	CListDataRestoreDlg( CStringArray& arrayBackups, CWnd* pParent = NULL); 
	~CListDataRestoreDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	
	virtual UINT GetDefaultButtonID() { return IDOK; }
	virtual void HandleButton1();	//OTHER
	virtual void HandleDefaultButton() { OnOK(); }

private:
	CStringArray& m_arrayBackups;
};

/**********************************************************************/
