#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/

class CSystemPropertyPage10 : public CSSAutoShutdownPropertyPage
{
public:
	CSystemPropertyPage10();
	~CSystemPropertyPage10();

	//{{AFX_DATA(CSystemPropertyPage10)
	enum { IDD = IDD_PROPPAGE_SYSTEM10 };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSystemPropertyPage10)
public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
	afx_msg void OnRadioNone();
	afx_msg void OnRadioHourly();
	afx_msg void OnRadioDaily();
	afx_msg void OnRadioWeekly();
	afx_msg void OnRadioCustom();
	afx_msg void OnTogglePrompt();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonCheck();
	afx_msg void OnButtonSchedule();
	afx_msg void OnButtonInfo();

protected:
	//{{AFX_MSG(CSystemPropertyPage10)
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void SetActiveTabControls();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();
	void SetRecordControls();
	void SetType(int n);
	void SetTabControls(bool bActiveTabOnly);

private:
	void CheckBackupPath(bool bMessageIfOK);
	bool ValidateFolderAccess(const char* szPath);

private:

	CTabCtrl m_TabActions;

	CButton m_radioNone;
	CButton m_radioHourly;
	CButton m_radioDaily;
	CButton m_radioWeekly;
	CButton m_radioCustom;
	CSSComboBox m_comboMinuteHourly;
	CStatic m_staticSchedule1;
	CDateTimeCtrl m_TimePickerDaily;
	CDateTimeCtrl m_TimePickerWeekly;
	CSSComboBox m_comboWeekday;
	CSSComboBox m_comboMinuteCustom;
	CStatic m_staticSchedule2;
	CButton m_buttonInfo;
	CButton m_buttonSchedule;

	CStatic m_staticOptions1;
	CEdit m_editFolder;
	CButton m_buttonBrowse;
	CButton m_buttonEdit;
	CButton m_buttonCheck;
	CStatic m_staticOptions2;
	CSSComboBox m_comboPrevious;
	CButton m_checkAlwaysPrompt;
	CButton m_checkPrompt;
	CSSComboBox m_comboPrompt;
	CStatic m_staticOptions3;
	CSSComboBox m_comboPrompt2;

	CButton m_checkExcludeZero;
	CStatic m_staticAccessType;
	CSSComboBox m_comboAccessType;
	CStatic m_staticCompression;
	CSSComboBox m_comboCompression;

	CStatic m_staticWarning;

private:
	int m_nType;
	CString m_strFolder;
	int m_nActiveTab;
};

/**********************************************************************/
