#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

struct CReportLoyaltySchemeInfo
{
public:
	CReportLoyaltySchemeInfo();

public:
	bool m_bLines;
	bool m_bRange;
	bool m_bAction;
	bool m_bLimit;
	bool m_bPayment;
};

/**********************************************************************/

class CReportLoyaltySchemeDlg : public CSSDialog
{
public:
	CReportLoyaltySchemeDlg( CReportLoyaltySchemeInfo& info, CWnd* pParent = NULL);   

	//{{AFX_DATA(CReportLoyaltySchemeDlg)
	enum { IDD = IDD_REPORT_LOYALTY_SCHEME };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CReportLoyaltySchemeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CButton m_checkLines;
	CButton m_checkRange;
	CButton m_checkAction;
	CButton m_checkLimit;
	CButton m_checkPayment;

protected:
	//{{AFX_MSG(CReportLoyaltySchemeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddColumn( CReportFile& ReportFile, const char* sz, int nAlign, int nWidth );
	const char* GetPromoName( int nPromoNo );
	const char* GetPaymentName( int nPayNo );

private:
	CReportLoyaltySchemeInfo& m_reportInfo;
	CString m_strPromoName;
	CString m_strPaymentName;
	int m_nColCount;
};

/**********************************************************************/
#endif
/**********************************************************************/
