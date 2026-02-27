#pragma once
//**********************************************************************
#include "resource.h"
//**********************************************************************
#include "..\SmartPay4Shared\GroupShiftValidator.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//**********************************************************************

class CGroupShiftDlg : public CSSAutoShutdownDialog
{
public:
	CGroupShiftDlg(CSQLRowAccountFull& RowAccount, CGroupShiftValidator& GroupShiftValidator, CWnd* pParent = NULL); 
	virtual ~CGroupShiftDlg();

	enum { IDD = IDD_GROUPSHIFT };
	CEdit m_editUserName;
	CEdit m_editGroupName;
	CStatic m_static0;
	CStatic m_static1;
	CStatic m_static2;
	CStatic m_static3; 
	CStatic m_static4;
	CStatic m_staticStatus;
	CButton m_buttonGroupShift;
	CSSTabbedComboBox m_comboGroupShift;
	CSSComboBox m_comboDuration;
	CDateTimeCtrl m_DatePickerStart;
	CDateTimeCtrl m_DatePickerEnd;
	CButton m_buttonOK;
	CButton m_buttonCancel;

private:
	void ShowGroupShiftControls();
	void HideGroupShiftControls();
	void CopyGroupShiftFutureGroupToControls();
	void CopyGroupShiftDatesToControls();

private:
	void FillDurationCombo(int nDays);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnButtonLog();
	afx_msg void OnButtonGroupShift();
	afx_msg void OnSelectDuration();
	afx_msg void OnKillFocusDuration();
	afx_msg void OnDatePickerStart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatePickerEnd(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CSQLRowAccountFull& m_RowAccount;
	CGroupShiftValidator& m_GroupShiftValidator;
	CString m_strGroupShiftDurations;
	CArray<int, int> m_arrayShiftGroups;
	CArray<int, int> m_arrayDurations;
	CArray<int, int> m_arrayStandardDurations;
	bool m_bGroupShiftEnabled; 
	COleDateTime m_oleDateShiftStart;
	COleDateTime m_oleDateShiftEnd;
	int m_nPendingStatus;
};

//**********************************************************************
