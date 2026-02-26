#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StockFilterArray.h"
/**********************************************************************/

class CStockExportDlg : public CSSDialog
{
public:
	CStockExportDlg( CStockFilterArray& FilterArray, int nStartPos, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CStockExportDlg)
	enum { IDD = IDD_STOCK_EXPORT };
	CEdit	m_editDestination;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CStockExportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockExportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnKillFocusStockCode();
	virtual void OnOK();
	afx_msg void OnButtonSet();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	const char* GetStockHeaderLine() { return m_strStockHeaderLine; }

private:
	void AppendStockHeader ( CCSV& csv );
	void CreateStockHeaderLine();
	void AddFieldToList( const char* szText, int nTagState );
	
public:
	CString m_strStockFrom;
	CString m_strStockTo;
	
private:
	CStockFilterArray& m_FilterArray;
	CSSListTaggedSelectCtrl m_listFields;
	CString m_strStockHeaderLine;
	CString m_strSavedSettingsLine;
};

/**********************************************************************/
#endif
/**********************************************************************/
