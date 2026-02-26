#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersDatabaseTest : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersDatabaseTest(CStringArray& arrayDbNames, bool m_bSchemeNames);

public:
	int GetArraySize() { return System.GetMaxEcrmanDatabases(); }
	bool PrepareLine(int nIndex, CString& strLine);
	void DeleteDatabaseEntry(int nIndex);
	void SetColumns(CSSListCtrlVirtual& list, CWnd* pParent);

private:
	void FilterSelectedItemArray(CReportConsolidationArray<CSortedIntItem>& arraySelected);

private:
	bool m_bSchemeNames;
	CStringArray& m_arrayDbNames;
};

/**********************************************************************/

class CListDataDatabaseTestDlg : public CListDataDlg
{
public:
	CListDataDatabaseTestDlg(CString strDatabaseRootFolder, CWnd* pParent = NULL);
	~CListDataDatabaseTestDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();
	virtual void HandleButton2();
	virtual void HandleButton3();
	virtual void HandleButton4();
	virtual bool HandleOK();
	virtual bool HandleCancel();

private:
	void HandleTestButton(CString strFilepath, CString strFilename, CString strText, CString strReportType);

private:
	CString m_strTitle;
	CString m_strDatabaseRootFolder;
	CStringArray m_arrayDbNames;
};

/**********************************************************************/
