#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "PosTrayTask.h"
#include "PosTrayTaskEditHelpers.h"
/**********************************************************************/

class CPosTrayBatchReportTimeAttendDlg : public CSSDialog
{
public:
	CPosTrayBatchReportTimeAttendDlg( CPosTrayTask& BatchEmail, CWnd* pParent = NULL);
	~CPosTrayBatchReportTimeAttendDlg();

	//{{AFX_DATA(CPosTrayBatchReportTimeAttendDlg)
	enum { IDD = IDD_POSTRAY_BATCH_REPORT_TIMEATTEND };

	CTabCtrl m_TabFilter;

	CStatic m_staticName;
	CEdit m_editName;

	CStatic m_staticDate;
	CStatic m_staticDate1;
	CStatic m_staticDate2;
	CStatic m_staticDate3;
	CSSComboBox	m_comboDates;
	CSSComboBox	m_comboRound;
	CSSComboBox	m_comboSession;
	
	CStatic m_staticDatabase;
	CStatic m_staticDatabase1;
	CSSComboBoxEx m_comboLocation;
	
	CStatic m_staticServers;
	CStatic m_staticServers1;
	CButton m_radioAll;
	CButton m_radioRange;

	CStatic m_staticOptions;
	CStatic m_staticOptions1;
	CSSComboBox	m_comboGroup;
	CStatic m_staticOptions2;
	CSSComboBox	m_comboEdit;
	CStatic m_staticOptions3;
	CSSComboBox	m_comboTotalHours;
	CButton m_checkOpenShift;
	CButton m_checkClosingTime;
	
	CSSListTaggedSelectCtrl m_listReport;

	int m_nServerPickType;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPosTrayBatchReportTimeAttendDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPosTrayBatchReportTimeAttendDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectTabFilter(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRadioAll();
	afx_msg void OnRadioRange();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditServer1();
	CEdit* GetEditServer2();
	
private:
	void SelectTabFilter();
	
private:
	void FillReportList();
	
private:
	void LoadEmailBatchSettings();
	void SaveEmailBatchSettings();
			
private:
	CSessionCSVRecord m_ReportSession;
	
private:
	CLocationSelector m_LocationSelector;
	
private:
	CPosTrayTask& m_BatchEmail;
	CStringArray m_arrayReportEntityIDs;

private:
	int m_nTabPageFilter;
};

/**********************************************************************/
#endif
/**********************************************************************/
