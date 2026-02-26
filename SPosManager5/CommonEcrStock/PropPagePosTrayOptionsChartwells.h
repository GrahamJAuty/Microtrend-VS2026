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

class CPropPagePosTrayOptionsChartwells : public CSSPropertyPage
{
public:
	CPropPagePosTrayOptionsChartwells();
	~CPropPagePosTrayOptionsChartwells();

	//{{AFX_DATA(CPropPagePosTrayOptionsChartwells)
	enum { IDD = IDD_PROPPAGE_POSTRAY_OPTIONS_CHARTWELLS };
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPagePosTrayOptionsChartwells)
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
	void SetChartwellsInfoArray( CReportConsolidationArray<CPosTrayChartwellsInfo>* pArray ){ m_pArrayChartwellsInfo = pArray; }
	
protected:
	//{{AFX_MSG(CPropPagePosTrayOptionsChartwells)
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
	CReportConsolidationArray<CPosTrayChartwellsInfo>* m_pArrayChartwellsInfo;
	CString m_strDisplayLine;
};

/**********************************************************************/
#endif
/**********************************************************************/
