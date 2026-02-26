#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ChartwellsOptions.h"
/**********************************************************************/

class CPosTrayChartwellsOptionsDlg : public CSSDialog
{
public:
	CPosTrayChartwellsOptionsDlg( CChartwellsOptions& Options, CWnd* pParent = NULL);
	virtual ~CPosTrayChartwellsOptionsDlg();

	enum { IDD = IDD_POSTRAY_CHARTWELLS };
	CEdit m_editServer;
	CEdit m_editUserName;
	CEdit m_editPassword;
	CEdit m_editSiteNo;
	CButton m_buttonPassword;
	CDateTimeCtrl m_DatePickerExport;
	CDateTimeCtrl m_TimePickerExport;
	CStatic m_staticExport;
	CSSComboBox m_comboExportDay;
	
public:
	void GetDateNextExport( COleDateTime& date){ date = m_oleDateNextExport; }

private:
	CEdit* GetEditSmartPay();

private:
	void UpdateNextExportTime();
	void ShowNextExportTime( COleDateTime& time );
	bool ValidateSerialNo();

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);  
	afx_msg void OnChangeDate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelectExportDay();
	afx_msg void OnChangeExportTime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonPassword();
	afx_msg void OnButtonTest();
	virtual void OnOK();
	
	DECLARE_MESSAGE_MAP()

private:
	CString m_strNewPassword;
	bool m_bGotOldPassword;
	CChartwellsOptions& m_Options;
	COleDateTime m_oleDateNextExport;
};

/**********************************************************************/
#endif
/**********************************************************************/

