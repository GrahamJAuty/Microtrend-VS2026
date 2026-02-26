#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
//include "MyComboBox.h"
#include "PosTrayFileSet.h"
#include "PosTrayTask.h"
#include "PosTrayTaskInfo.h"
#include "PropertySheetPosTrayOptions.h"
/**********************************************************************/
//include "MySSDialog.h"
/**********************************************************************/

class CPosTrayManualExportDlg : public CSSDialog
{
public:
	CPosTrayManualExportDlg( CPosTrayTaskInfo& TaskInfo, CPosTrayFileSet& FileSet, CPosTrayManualExportOptions& ExportOptions, CWnd* pParent = NULL);
	virtual ~CPosTrayManualExportDlg();

	enum { IDD = IDD_POSTRAY_MANUAL_EXPORT };
	CSSComboBox	m_comboDates;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	CSSComboBox m_comboInterval;
	CStatic m_staticProgress1;
	CStatic m_staticProgress2;
	CProgressCtrl m_Progress1;
	CProgressCtrl m_Progress2;
	CButton m_buttonCreate;
	CButton m_buttonCancel;
	CButton m_buttonOK;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnSelectDate();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonCreate();
	afx_msg void OnButtonCancel();

public:
	void UpdateFileProgressPos( __int64 nProgress, __int64 nTarget );

private:
	void SetPos( bool bBottom, __int64 nProgress, __int64 nTarget );
	void SetPos( CProgressCtrl& progress, int& nProgressPos, __int64 nProgress, __int64 nTarget );

private:
	void SaveRecord();
	void GetRecordData();
	bool GetMergedTask( CPosTrayTask& merge );
	void CreateSingleExport( COleDateTime& dateFrom, COleDateTime& dateTo, CPosTrayTask& task );
	void CreateYearlyExport( COleDateTime& dateFrom, COleDateTime& dateTo, CPosTrayTask& task );
	void CreateMonthlyExport( COleDateTime& dateFrom, COleDateTime& dateTo, CPosTrayTask& task );
	void CreateWeeklyExport( COleDateTime& dateFrom, COleDateTime& dateTo, CPosTrayTask& task );
	void CreateDailyExport( COleDateTime& dateFrom, COleDateTime& dateTo, CPosTrayTask& task );
	void PerformExport( COleDateTime& DateFrom, COleDateTime& DateTo, CPosTrayTask& task, int nExportNo );
	void UpdateExportOptions();

private:
	CPosTrayManualExportOptions& m_ExportOptions;
	CPosTrayTaskInfo& m_PosTrayTaskInfo;
	CPosTrayFileSet& m_PosTrayFileSet;

private:
	CDateSelectorRange m_DateSelectorRange;
	int m_nProgressPos1;
	int m_nProgressPos2;
	bool m_bRunningExport;
	bool m_bCancelExport;
	bool m_bTwoProgressBars;
	__int64 m_nTargetFiles;
	__int64 m_nNoDataFiles;
	__int64 m_nNoExportFolder;
};

/**********************************************************************/
#endif
/**********************************************************************/
