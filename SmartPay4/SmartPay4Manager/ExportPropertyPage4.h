#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//$$******************************************************************

class CExportPropertyPage4 : public CSSAutoShutdownPropertyPage
{
public:
	CExportPropertyPage4();
	virtual ~CExportPropertyPage4();
	void SetPointer ( CDbExportHandler* pDbExportHandler );

// Dialog Data
	enum { IDD = IDD_PROPPAGE_EXPORT4 };
	CEdit m_editFilename;
	CButton m_radioReportOutput1;
	CButton m_radioReportOutput2;
	CButton m_radioReportOutput3;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnRadioReportOutput1();
	afx_msg void OnRadioReportOutput2();
	afx_msg void OnRadioReportOutput3();
	afx_msg void OnButtonBrowse();
	DECLARE_MESSAGE_MAP()
	
private:
	void SetDisplay();

private:
	int m_nSaveAsType;
	CString m_strFilename;

private:
	CDbExportHandler* m_pDbExportHandler;
};

//$$******************************************************************
