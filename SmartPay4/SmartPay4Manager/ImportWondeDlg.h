#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\WondeData.h"
//$$******************************************************************

class CImportWondeDlg : public CSSAutoShutdownDialog
{
public:
	CImportWondeDlg( CWnd* pParent = NULL);   

	enum { IDD = IDD_IMPORTWONDE };
	CButton m_buttonView;
	CButton m_buttonView2;
	CButton m_buttonExecute;
	CButton m_buttonExecute2;
	CEdit m_editImportFilename;
	CButton m_checkDeleteFile;
	CString m_strImportFilename;
	BOOL m_bDeleteFile;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	afx_msg void OnButtonView();
	afx_msg void OnButtonView2();
	afx_msg void OnButtonExecute();
	afx_msg void OnButtonExecute2();
	afx_msg void OnButtonBrowse();
	afx_msg void OnKillFocusImportFilename();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetDisplay();

private:
	CWondeData m_data;
};

//$$******************************************************************
