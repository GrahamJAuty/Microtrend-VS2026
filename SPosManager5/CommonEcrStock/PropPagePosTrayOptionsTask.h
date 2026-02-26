#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PosTrayTaskArrays.h"
#include "PosTrayFileSet.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#include "PosTrayTaskInfo.h"
/**********************************************************************/

struct CPosTrayManualExportOptions
{
public:
	CString m_strComboSelect;
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;
	int m_nInterval;
};

/**********************************************************************/

class CPropPagePosTrayOptionsTask : public CSSPropertyPage
{
public:
	CPropPagePosTrayOptionsTask();
	~CPropPagePosTrayOptionsTask();

	//{{AFX_DATA(CPropPagePosTrayOptionsTask)
	enum { IDD = IDD_PROPPAGE_POSTRAY_OPTIONS_TASK };
	//}}AFX_DATA
	CButton m_buttonFilename;
	CButton m_buttonExport;
	
	//{{AFX_VIRTUAL(CPropPagePosTrayOptionsTask)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonCopy();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonFilename();
	afx_msg void OnButtonExport();
	afx_msg void OnSelectType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickListTask(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListTask(NMHDR* pNMHDR, LRESULT* pResult);

public:
	void SetTaskArrays( CPosTrayTaskArrays& arrays ){ m_PosTrayTaskArrays = arrays; }
	void ClearAdjustedListTypeFlag(){ m_listType.ClearRowHeightFlag(); }
	void ClearAdjustedListTaskFlag(){ m_listTask.ClearRowHeightFlag(); }

protected:
	//{{AFX_MSG(CPropPagePosTrayOptionsTask)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();
	void GetRecordData();

private:
	int GetSafeSelectionType();
	int GetSafeSelectionType( int nMin, int nMax );
	void SelectLineType( int nIndex );
	void SelectTaskType();

private:
	bool PrepareLineTask( int nIndex );
	int GetSafeSelectionTask();
	int GetSafeSelectionTask( int nMin, int nMax );
	void SelectLineTask( int nIndex );
	void EditItem( int nIndex, bool bNewTask );

private:
	bool GetNextFreeTaskNo( int& nTaskNo );
	void AddTask( int nTaskNo, CPosTrayTask& TaskToCopy );
	void UpdateSelectedTaskArray();
	
private:
	CSSListCtrlVirtual m_listType;
	CSSListCtrlVirtual m_listTask;

private:
	CPosTrayFileSet m_PosTrayFileSet;
	CReportConsolidationArray<CPosTrayTaskInfo>* m_pArrayTaskInfo;
	CReportConsolidationArray<CPosTrayBatchInfo>* m_pArrayBatchInfo;
	CReportConsolidationArray<CSortedIntItem> m_arraySelected;
	CString m_strDisplayLineTask;
	
private:
	CPosTrayManualExportOptions m_ExportOptions;
	CPosTrayTaskArrays m_PosTrayTaskArrays;
	CArray<int,int> m_arrayTaskTypes;
	int m_nSelectedType;
	int m_nSelectedTask;
};

/**********************************************************************/
#endif
/**********************************************************************/
