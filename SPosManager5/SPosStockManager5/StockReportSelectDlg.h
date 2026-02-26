#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockReportSelectDlg : public CSSDialog
{
public:
	CStockReportSelectDlg( CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CStockReportSelectDlg)
	enum { IDD = IDD_STOCK_REPORT_SELECT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockReportSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockReportSelectDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	afx_msg void OnButtonResetOrder();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()

private:
	void BuildReportList();
	void CreateSavedSettingsLine();
	void CreateReportFieldsLine();
	void AppendFieldTicks( CCSV& csv );
	void AddReportToList( int nReportType );

private:
	CSSListTaggedSelectCtrl m_listReports;
};

/**********************************************************************/
#endif
/**********************************************************************/
