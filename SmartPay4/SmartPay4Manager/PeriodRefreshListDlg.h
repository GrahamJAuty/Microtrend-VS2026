#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLPurse2Handler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//*******************************************************************

class CPeriodRefreshListDlg : public CSSAutoShutdownDialog
{
public:
	CPeriodRefreshListDlg( CSQLRowAccountFull& SQLRowAccount, CSQLRowGroupFull& SQLRowGroup, CWnd* pParent = NULL );   // standard constructor
	virtual ~CPeriodRefreshListDlg();

// Dialog Data
	enum { IDD = IDD_PERIODREFRESHLIST };
	CString m_strPeriod1;
	CString m_strPeriod2;
	CString m_strPeriod3;
	CString m_strPeriod4;
	CString m_strPeriod5;
	CString m_strPeriod6;
	CString	m_strPeriodText1;
	CString	m_strPeriodText2;
	CString	m_strPeriodText3;
	CString	m_strPeriodText4;
	CString	m_strPeriodText5;
	CString	m_strPeriodText6;
	double m_dRefreshPeriod1;
	double m_dRefreshPeriod2;
	double m_dRefreshPeriod3;
	double m_dRefreshPeriod4;
	double m_dRefreshPeriod5;
	double m_dRefreshPeriod6;
	CString m_strBalancePeriod1;
	CString m_strBalancePeriod2;
	CString m_strBalancePeriod3;
	CString m_strBalancePeriod4;
	CString m_strBalancePeriod5;
	CString m_strBalancePeriod6;
	CString m_strStartPeriod1;
	CString m_strStartPeriod2;
	CString m_strStartPeriod3;
	CString m_strStartPeriod4;
	CString m_strStartPeriod5;
	CString m_strStartPeriod6;
	CString m_strLastRefreshPeriod1;
	CString m_strLastRefreshPeriod2;
	CString m_strLastRefreshPeriod3;
	CString m_strLastRefreshPeriod4;
	CString m_strLastRefreshPeriod5;
	CString m_strLastRefreshPeriod6;
	CString m_strRefreshTypePeriod1;
	CString m_strRefreshTypePeriod2;
	CString m_strRefreshTypePeriod3;
	CString m_strRefreshTypePeriod4;
	CString m_strRefreshTypePeriod5;
	CString m_strRefreshTypePeriod6;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CString GetDisplayBalance ( const char* szDate, double dValue );
	CString GetDisplayPeriodNo ( int nIndex, int nCurrentIndex );

private:
	CSQLPurse2Handler m_purse2;
};

//*******************************************************************
