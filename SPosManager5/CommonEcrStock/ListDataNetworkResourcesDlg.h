#pragma once
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CSingleListContentHelpersNetworkResources : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersNetworkResources();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
};

/**********************************************************************/

class CListDataNetworkResourcesDlg : public CListDataDlg
{
public:
	CListDataNetworkResourcesDlg( CWnd* pParent = NULL);
	~CListDataNetworkResourcesDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual void HandleButton1();	//EDIT
	virtual void HandleButton3();	//CLEAR

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleDefaultButton() { HandleButton1(); }
};

/**********************************************************************/

