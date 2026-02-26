#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersPrinterSelect : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersPrinterSelect( CReportConsolidationArray<CSortedStringItem>& arrayPrinters );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}

private:
	CReportConsolidationArray<CSortedStringItem>& m_arrayPrinters;
};

/**********************************************************************/

class CListDataPrinterSelectDlg : public CListDataDlg
{
public:
	CListDataPrinterSelectDlg( const char* szTitle, CReportConsolidationArray<CSortedStringItem>& arrayPrinters, CWnd* pParent = NULL); 
	~CListDataPrinterSelectDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();
	virtual bool HandleOK();
	
	virtual UINT GetDefaultButtonID() { return IDOK; }
	virtual void HandleDefaultButton() { OnOK(); }

public:
	int m_nSelection;

private:
	CString m_strTitle;
	CReportConsolidationArray<CSortedStringItem>& m_arrayPrinters;
};

/**********************************************************************/
#endif
/**********************************************************************/
