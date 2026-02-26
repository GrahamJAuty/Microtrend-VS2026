#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************

class CPasswordLogDlg : public CSSDialog
{
public:
	CPasswordLogDlg( CWnd* pParent = NULL);
	virtual ~CPasswordLogDlg();
		
public:
// Dialog Data
	enum { IDD = IDD_PASSWORD_LOG };
	CSSComboBox m_comboDateRange;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	CButton m_checkSort;
	CButton m_radioType1;
	CButton m_radioType2;
		
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);   
	afx_msg void OnSelectDateRange();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

//$$******************************************************************
