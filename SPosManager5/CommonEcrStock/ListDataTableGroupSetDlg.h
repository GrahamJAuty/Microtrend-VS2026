#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersTableGroupSet : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersTableGroupSet( int nLevel );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
	void DeleteDatabaseEntry( int n ){}

private:
	int m_nLevel;
};

/**********************************************************************/

class CListDataTableGroupSetDlg : public CListDataDlg
{
public:
	CListDataTableGroupSetDlg( int nLocIdx, int nLevel, CWnd* pParent = NULL); 
	~CListDataTableGroupSetDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	
	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//EDIT
	virtual void HandleButton3();	//PRINT
	virtual void HandleDefaultButton() { HandleButton1(); }

private:
	void HandleDownload();

private:
	int m_nLocIdx;
	int m_nLevel;
};

/**********************************************************************/
#endif
/**********************************************************************/
