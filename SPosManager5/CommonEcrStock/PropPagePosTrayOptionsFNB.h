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

class CPropPagePosTrayOptionsFNB : public CSSPropertyPage
{
public:
	CPropPagePosTrayOptionsFNB();
	~CPropPagePosTrayOptionsFNB();

	//{{AFX_DATA(CPropPagePosTrayOptionsFNB)
	enum { IDD = IDD_PROPPAGE_POSTRAY_OPTIONS_FNB };
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPagePosTrayOptionsFNB)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnButtonEdit();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);

public:
	void ClearAdjustedListFlag(){ m_listData.ClearRowHeightFlag(); }
	void SetFNBInfoArray( CReportConsolidationArray<CPosTrayFNBInfo>* pArray ){ m_pArrayFNBInfo = pArray; }
	
protected:
	//{{AFX_MSG(CPropPagePosTrayOptionsFNB)
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
	CReportConsolidationArray<CPosTrayFNBInfo>* m_pArrayFNBInfo;
	CString m_strDisplayLine;
};

/**********************************************************************/
#endif
/**********************************************************************/
