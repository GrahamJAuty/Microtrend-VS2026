#pragma once
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersHeaderChange : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersHeaderChange( CString strOldHeader, CString strNewHeader );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}

private:
	CStringArray m_arrayOldHeader;
	CStringArray m_arrayNewHeader;
};

/**********************************************************************/

class CListDataHeaderChangeDlg : public CListDataDlg
{
public:
	CListDataHeaderChangeDlg( const char* szTitle, CString strOldHeader, CString strNewHeader, CWnd* pParent = NULL); 
	~CListDataHeaderChangeDlg();

public:
	void SetHeader1Description(CString str) { m_strHeader1Description = str; }
	void SetHeader2Description(CString str) { m_strHeader2Description = str; }
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();
	
	virtual UINT GetDefaultButtonID() { return IDCANCEL; }
	virtual void HandleDefaultButton() { OnCancel(); }

private:
	CString m_strTitle;
	CString m_strHeader1Description;
	CString m_strHeader2Description;
};

/**********************************************************************/
