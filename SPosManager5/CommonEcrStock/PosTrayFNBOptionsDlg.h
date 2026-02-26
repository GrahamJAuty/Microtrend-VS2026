#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "FNBDatabaseOptions.h"
#include "..\CommonEcrStock\LocationSelector.h"
//include "MyComboBoxEx.h"
/**********************************************************************/

class CPosTrayFNBOptionsDlg : public CSSDialog
{
public:
	CPosTrayFNBOptionsDlg( int nDbIdx, CFNBDatabaseOptions& Options, CWnd* pParent = NULL); 
	virtual ~CPosTrayFNBOptionsDlg();

	enum { IDD = IDD_POSTRAY_FNB };
	CButton m_checkEnable;
	CSSComboBoxEx m_comboLocation;
	CEdit m_editServer;
	CEdit m_editUserName;
	CEdit m_editPassword;
	CButton m_buttonPassword;
	CEdit m_editFolder;
	CDateTimeCtrl m_DatePickerExport;
	CStatic m_staticExport;
	int m_nPause;

private:
	CEdit* GetEditPause();

public:
	void GetDateNextExport( COleDateTime& date){ date = m_oleDateNextExport; }

private:
	void UpdateNextExportTime();
	void ShowNextExportTime( COleDateTime& time );

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);  
	afx_msg void OnCheckEnable();
	afx_msg void OnChangeDate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKillFocusPause();
	afx_msg void OnButtonPassword();
	virtual void OnOK();
	
	DECLARE_MESSAGE_MAP()

private:
	CLocationSelector m_LocationSelector;

private:
	int m_nDbIdx;
	CString m_strNewPassword;
	bool m_bGotOldPassword;
	CFNBDatabaseOptions& m_Options;
	COleDateTime m_oleDateNextExport;
};

/**********************************************************************/
#endif
/**********************************************************************/

