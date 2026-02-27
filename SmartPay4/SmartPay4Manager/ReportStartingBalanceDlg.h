#pragma once
//**********************************************************************
#include "resource.h"
//**********************************************************************
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\PaymentAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//**********************************************************************

class CReportStartingBalanceDlg : public CSSAutoShutdownDialog
{
public:
	CReportStartingBalanceDlg( CWnd* pParent = NULL);  
	virtual ~CReportStartingBalanceDlg();

	enum { IDD = IDD_REPORT_STARTINGBALANCE };
	CSSComboBox	m_comboDateRange;
	CDateTimeCtrl m_DatePicker;
	CString	m_strDateRange;
	CTime	m_ctimeDateFrom;
	CButton m_checkGroupSort;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	afx_msg void OnSelectDate();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	void SetLabel ( const char* szLabel ) { m_strReportLabel = szLabel; }
	int IsValidAtc ( CDbReportHandler* pExport, CSQLAuditRecord* atcRecord );
	int IsValidRecord (CSQLRowAccountFull& RowAccount, CDbReportHandler* pExport );

public:
	CSSDate m_dateReference;
	CSSDate m_dateBegin;
	CSSDate m_dateEnd;
	bool m_bGroupSort;

private:
	CString m_strReportLabel;
};

//**********************************************************************
