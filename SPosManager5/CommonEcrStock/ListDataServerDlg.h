#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersServer : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersServer();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
};

/**********************************************************************/

class CListDataServerDlg : public CListDataDlg
{
public:
	CListDataServerDlg( int nServerDbIdx, int nServerLocIdx, int nTerminalLocIdx, CWnd* pParent = NULL); 
	~CListDataServerDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	
	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//PRINT
	virtual void HandleButton5();	//DOWNLOAD
	virtual void HandleButtonHidden1(); //EXPORT
	virtual void HandleButtonHidden2(); //IMPORT
	virtual void HandleDefaultButton() { HandleButton2(); }
	virtual void HandleClickHeader(int nColumn);

	void HandleDownload( CArray<int,int>& arrayLocIdx );
	bool HandleCSVImport(CString& strFilename);

	bool SearchText(int nIndex, CString strSearchText);

private:
	int m_nServerDbIdx;
	int m_nServerLocIdx;
	int m_nTerminalLocIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
