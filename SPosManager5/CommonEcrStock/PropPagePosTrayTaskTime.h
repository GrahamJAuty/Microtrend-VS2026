#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
 
/**********************************************************************/
//include "MyComboBox.h"
#include "PosTrayTask.h"
/**********************************************************************/
//include "MySSPropertyPage.h"
/**********************************************************************/

class CPropPagePosTrayTaskTime : public CSSPropertyPage
{
public:
	CPropPagePosTrayTaskTime();
	~CPropPagePosTrayTaskTime();

	//{{AFX_DATA(CPropPagePosTrayTaskTime)
	enum { IDD = IDD_PROPPAGE_POSTRAY_TASK_TIME };
	//}}AFX_DATA
	CDateTimeCtrl m_TimePickerFrom;
	CDateTimeCtrl m_TimePickerTo;
	CSSComboBox m_comboInterval;
	CButton m_checkSunday;
	CButton m_checkMonday;
	CButton m_checkTuesday;
	CButton m_checkWednesday;
	CButton m_checkThursday;
	CButton m_checkFriday;
	CButton m_checkSaturday;
	CEdit m_editMonthDay;
	CDateTimeCtrl m_DatePickerNotBefore;
	CDateTimeCtrl m_TimePickerNotBefore;
	CDateTimeCtrl m_DatePickerNext;
	CDateTimeCtrl m_TimePickerNext;
	CEdit m_editNext;
	CStatic m_staticRestart;
	CSSComboBox m_comboRestart;
	
	//{{AFX_VIRTUAL(CPropPagePosTrayTaskTime)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnDateTimeChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeLastTaskTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeSettings();
	afx_msg void OnButtonMonthDay();
	afx_msg void OnTimer(UINT nIDEvent);
	
protected:
	//{{AFX_MSG(CPropPagePosTrayTaskTime)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetEmailTask( CPosTrayTask* pTask ){ m_pTask = pTask; }
	
public:
	bool GetNotBeforeTime( COleDateTime& date);
	void UpdateNotBeforeTime();
	void UpdateTask();
	void ShowNextTaskTime();

private:
	void SaveRecord();
	void GetRecordData();

private:
	CPosTrayTask* m_pTask;

private:
	COleDateTime m_oleNotBeforeTime;
	bool m_bGotNotBeforeTime;
	COleDateTime m_timeLastTaskCheck;
	bool m_bTimerReady;
	UINT m_hTimer;
};

/**********************************************************************/
#endif
/**********************************************************************/
