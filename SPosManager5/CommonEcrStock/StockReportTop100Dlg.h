#pragma once
/**********************************************************************/
//NOTE - PLU SALES HISTORY USES THIS SO CANNOT REMOVE FROM ECRMAN
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SalesHistoryCSVArrayPlu.h"
#include "..\SPosStockManager5\SalesHistoryCSVArrayStock.h"
/**********************************************************************/
#define TOP100_MESSAGE 9999
/**********************************************************************/
#define TOP100_STOCKLEVEL_SALES_PERIOD 0
#define TOP100_STOCKLEVEL_SALES_YTD 1
#define TOP100_STOCKLEVEL_SALES_YTD_PLUS_PERIOD 2
#define TOP100_STOCKLEVEL_APPARENT 3
#define TOP100_SALESHISTORY_PLU 4
#define TOP100_SALESHISTORY_STOCK 5
#define TOP100_SALESHISTORY_DEPT 6
#define TOP100_EMAILTASK 7
/**********************************************************************/

class CStockReportTop100Dlg : public CSSDialog
{
public:
	CStockReportTop100Dlg( int nContext, const char* szText, CSalesHistoryCSVRecordPlu& chartinfo, CWnd* pParent = NULL);   
	
#ifdef STOCKMAN_SYSTEM
	CStockReportTop100Dlg( int nContext, const char* szText, CSalesHistoryCSVRecordStock& chartinfo, CWnd* pParent = NULL);   
#endif

	//{{AFX_DATA(CStockReportTop100Dlg)
	enum { IDD = IDD_STOCK_REPORT_TOP100 };
	int		m_nRadioQtyValue;
	int		m_nRadioTopBottom;
	int		m_nRadioUnlimited;
	int		m_nChartMaxItems;
	double	m_dChartCutOff;
	//}}AFX_DATA
	CButton	m_radioTop;
	CButton	m_radioBottom;
	
	//{{AFX_VIRTUAL(CStockReportTop100Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void GetChartInfo( CSalesHistoryCSVRecordPlu& chartinfo );
	
#ifdef STOCKMAN_SYSTEM
	void GetChartInfo( CSalesHistoryCSVRecordStock& chartinfo );
#endif

private:
	CEdit* GetEditMaxItems();
	CEdit* GetEditCutOff();

private:
	CSalesHistoryCSVRecordPlu m_chartinfo;

protected:
	//{{AFX_MSG(CStockReportTop100Dlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioUnlimited();
	afx_msg void OnRadioCutOff();
	afx_msg void OnRadioMaxItems();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nContext;			//period, ytd or plu sales history

private:
	CString m_strText;
};

/**********************************************************************/

