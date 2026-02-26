#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "Consolidation.h"
#include "LocationCSVArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageLocationTerminal : public CSSPropertyPage
{
public:
	CPropPageLocationTerminal();
	~CPropPageLocationTerminal();

	//{{AFX_DATA(CPropPageLocationTerminal)
	enum { IDD = IDD_PROPPAGE_LOCATION_TERMINAL };
	//}}AFX_DATA
	CStatic m_staticTerminals;
	CButton m_buttonAdd;
	CButton m_buttonEdit;
	CButton m_buttonRemove;
	CButton m_buttonToggle;
	
	//{{AFX_VIRTUAL(CPropPageLocationTerminal)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonToggle();

private:
	void HandleAddTerminals();

protected:
	//{{AFX_MSG(CPropPageLocationTerminal)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void Refresh();
	bool UpdateRecord();
	bool PrepareLine( int nIndex );

public:
	void SetLocIdx( int nLocIdx ){ m_nLocIdx = nLocIdx; }
	void SetLocationBuffer( CLocationCSVArray* pBuffer ) { m_pLocationBuffer = pBuffer; }
	void SetTNoOldArray( CReportConsolidationArray<CSortedIntItem>* pArray ) { m_pTNoOldArray = pArray; }
	void SetTNoNewArray( CReportConsolidationArray<CSortedIntItem>* pArray ) { m_pTNoNewArray = pArray; }
	
private:
	void GetRecordData();
	void SaveRecord();
	void UpdateTerminalCount();

private:
	int m_nLocIdx;
	CLocationCSVArray* m_pLocationBuffer;
	CReportConsolidationArray<CSortedIntItem>* m_pTNoOldArray;
	CReportConsolidationArray<CSortedIntItem>* m_pTNoNewArray;

private:
	int m_nColumnMode;
	CSSListCtrlVirtual m_listTerminals;
	CString m_strDisplayLine;
};

/**********************************************************************/
#endif
/**********************************************************************/
