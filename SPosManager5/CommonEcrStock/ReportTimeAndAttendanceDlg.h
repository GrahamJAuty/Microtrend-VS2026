#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "ServerSelectionList.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "EposSelectArray.h"
#include "SessionCSVArray.h"
/**********************************************************************/

class CReportTimeAndAttendanceDlg : public CSSDialog
{
public:
	CReportTimeAndAttendanceDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent = NULL);   

	//{{AFX_DATA(CReportTimeAndAttendanceDlg)
	enum { IDD = IDD_REPORT_TIMEATTEND };
	CSSComboBox	m_comboRound;
	CSSComboBox	m_comboDates;
	CSSComboBox	m_comboEdit;
	CStatic			m_staticPreset;
	CButton			m_buttonPreset;
	CSSComboBox		m_comboPreset;
	CSSComboBox	m_comboReportType;
	CButton m_checkOpenShift;
	CButton m_checkClosingTime;
	CDateTimeCtrl	m_DatePickerTo;
	CDateTimeCtrl	m_DatePickerFrom;
	CSSComboBoxEx	m_comboLocation;
	CSSComboBox		m_comboSession;
	CSSComboBox		m_comboGroup;
	CSSComboBox		m_comboTotalHours;
	CButton m_radioAll;
	CButton m_radioRange;
	CButton m_radioList;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CReportTimeAndAttendanceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CReportTimeAndAttendanceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDate();
	afx_msg void OnButtonPreset();
	afx_msg void OnSelectPreset();
	afx_msg void OnSelectReportType();
	afx_msg void OnButtonDisplay();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnRadioAll();
	afx_msg void OnRadioRange();
	afx_msg void OnRadioList();
	DECLARE_MESSAGE_MAP()

private:
	void GetReportDates();
	bool GetAdhocReportSession();
	
private:
	CEdit* GetEditServer1();
	CEdit* GetEditServer2();

public:
	void CreatePreset( CPresetReportInfo& base );
	int GetPresetError(){ return m_nPresetError; }

private:
	void FillPresetCombo( int nSelection );

private:
	COleDateTime m_OleDateFrom;
	COleDateTime m_OleDateTo;

private:
	CDateSelectorRange m_DateSelectorRange;
	CLocationSelector m_LocationSelector;
	CEposSelectArray m_SelectArray;

private:
	CServerSelectionList m_ServerSelectionList;
	int m_nServerPickType;
	int m_nServerStart;
	int m_nServerEnd;

	CSessionCSVRecord m_ReportSession;
	CSessionCSVRecord m_ReportSessionAdhoc;

private:
	int m_nPresetNo;
	int m_nPresetError;
	bool m_bAutoRunPreset;
};

/**********************************************************************/
