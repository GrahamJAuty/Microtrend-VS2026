#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersSchemeNames : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersSchemeNames(CReportConsolidationArray<CSortedStringByInt>& arrayNames);

public:	int GetArraySize() { return m_arrayNames.GetSize(); }
	bool PrepareLine(int nIndex, CString& strLine);
	void DeleteDatabaseEntry(int nIndex);
	void SetColumns(CSSListCtrlVirtual& list, CWnd* pParent);

private:
	void FilterSelectedItemArray(CReportConsolidationArray<CSortedIntItem>& arraySelected);

private:
	CReportConsolidationArray<CSortedStringByInt>& m_arrayNames;
};

/**********************************************************************/

class CListDataSchemeNamesDlg : public CListDataDlg
{
public:
	CListDataSchemeNamesDlg(CWnd* pParent = NULL);
	~CListDataSchemeNamesDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();
	virtual void HandleButton2();
	virtual void HandleButton3();
	virtual bool HandleOK();
	virtual bool HandleCancel();

private:
	CString m_strTitle;
	CReportConsolidationArray<CSortedStringByInt> m_arrayNames;
};

/**********************************************************************/
