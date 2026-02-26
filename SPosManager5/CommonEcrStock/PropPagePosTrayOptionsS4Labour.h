#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ReportConsolidationArray.h"
/**********************************************************************/
#include "PosTrayTaskInfo.h"
/**********************************************************************/

class CPropPagePosTrayOptionsS4Labour : public CSSPropertyPage
{
public:
	CPropPagePosTrayOptionsS4Labour();
	~CPropPagePosTrayOptionsS4Labour();

	//{{AFX_DATA(CPropPagePosTrayOptionsS4Labour)
	enum { IDD = IDD_PROPPAGE_POSTRAY_OPTIONS_S4LABOUR };
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPagePosTrayOptionsS4Labour)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnButtonFTP();
	afx_msg void OnButtonLocation();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);

public:
	void ClearAdjustedListFlag(){ m_listData.ClearRowHeightFlag(); }
	void SetS4LabourInfoArray( CReportConsolidationArray<CPosTrayS4LabourInfo>* pArray ){ m_pArrayS4LabourInfo = pArray; }
	
protected:
	//{{AFX_MSG(CPropPagePosTrayOptionsS4Labour)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

private:
	void SaveRecord();
	void GetRecordData();

private:
	bool PrepareLine( int nIndex );
	int GetSafeSelection();
	int GetSafeSelection( int nMin, int nMax );
	void SelectLine( int nIndex );

private:
	CSSListCtrlVirtual m_listData;

private:
	bool m_bHistory;
	CReportConsolidationArray<CPosTrayS4LabourInfo>* m_pArrayS4LabourInfo;
	CString m_strDisplayLine;
};

/**********************************************************************/
#endif
/**********************************************************************/
