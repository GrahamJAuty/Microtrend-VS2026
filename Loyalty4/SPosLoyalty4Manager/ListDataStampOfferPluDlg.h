#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLTable_StampOfferInfo\SQLRowStampOfferInfo.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersStampOfferPlu : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersStampOfferPlu(CReportConsolidationArray<CSortedStringByIntAndInt64>& arrayPlu);

public:
	int GetArraySize();
	bool PrepareLine(int nIndex, CString& strLine);
	void DeleteDatabaseEntry(int nIndex);
	void SetColumns(CSSListCtrlVirtual& list, CWnd* pParent);

private:
	void FilterSelectedItemArray(CReportConsolidationArray<CSortedIntItem>& arraySelected);

private:
	CReportConsolidationArray<CSortedStringByIntAndInt64>& m_arrayPlu;
};

/**********************************************************************/

class CListDataStampOfferPluDlg : public CListDataDlg
{
public:
	CListDataStampOfferPluDlg(CSQLRowStampOfferInfo& RowSO, CWnd* pParent = NULL);
	~CListDataStampOfferPluDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual bool HandleOK();
	virtual bool HandleCancel();

private:
	CReportConsolidationArray<CSortedStringByIntAndInt64> m_arrayPlu;
	CString m_strTitle;
};

/**********************************************************************/
