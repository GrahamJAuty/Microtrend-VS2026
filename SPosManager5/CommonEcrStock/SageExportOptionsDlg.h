#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CSageExportOptionsDlg : public CSSDialog
{
public:
	CSageExportOptionsDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CSageExportOptionsDlg)
	enum { IDD = IDD_SAGE_EXPORT_OPTIONS };
	int		m_nDelay;
	CButton	m_checkEnable;
	CEdit m_editReference;
	CStatic m_staticDate;
	CDateTimeCtrl m_DatePicker;
	CString m_strReference;
	CSSComboBox m_comboNominal;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CSageExportOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual void OnOK();
	afx_msg void OnCheckEnable();
	//}}AFX_VIRTUAL

public:
	CEdit* GetEditDelay();

private:
	bool m_bInitialEnable;

protected:
	//{{AFX_MSG(CSageExportOptionsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
#endif
/**********************************************************************/
