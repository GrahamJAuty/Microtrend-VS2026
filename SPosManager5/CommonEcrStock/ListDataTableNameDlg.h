#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersTableName : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersTableName();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

public:
	const char* GetGroupText( CTableNameCSVRecord& Table, int nLevel );
	void ChangeGroupLevel();

private:
	CString m_strGroupText;
	int m_nGroupLevel;
};

/**********************************************************************/

class CListDataTableNameDlg : public CListDataDlg
{
public:
	CListDataTableNameDlg( int nLocIdx, CWnd* pParent = NULL); 
	~CListDataTableNameDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	
	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//COPY
	virtual void HandleButton4();	//DELETE
	virtual void HandleButton5();	//PRINT
	virtual void HandleButton6();	//DOWNLOAD
	virtual void HandleDefaultButton() { HandleButton2(); }
	virtual void HandleClickHeader( int nColumn );
	
private:
	void HandleDownload();

private:
	int m_nLocIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
