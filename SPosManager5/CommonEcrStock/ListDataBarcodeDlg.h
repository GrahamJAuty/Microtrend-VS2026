#pragma once
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CSingleListContentHelpersBarcode : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersBarcode();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
};

/**********************************************************************/

class CListDataBarcodeDlg : public CListDataDlg
{
public:
	CListDataBarcodeDlg( CWnd* pParent = NULL);
	~CListDataBarcodeDlg();
	
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
	virtual void HandleButton6();	//IMPORT
	virtual void HandleDefaultButton() { HandleButton2(); }

	void HandleDownload( CArray<int,int>& arrayLocIdx );
};

/**********************************************************************/

