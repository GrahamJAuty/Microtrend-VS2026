#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "Consolidation.h"
#include "OfferGroupsCSVArray.h"
#include "ReportConsolidationArray.h"
#include "TwinListHelpers.h"
/**********************************************************************/

class CPropPageOfferGroupEntryDept : public CSSPropertyPage
{
public:
	CPropPageOfferGroupEntryDept();
	~CPropPageOfferGroupEntryDept();

	void SetEntryType( int nEntryType );

	//{{AFX_DATA(CPropPageOfferGroupEntryDept)
	enum { IDD = IDD_PROPPAGE_OFFERGROUP_ENTRY_DEPT };
	//}}AFX_DATA
	CEdit m_editName;
	CButton m_buttonRemove;
	CStatic m_staticSelected;
	CStatic m_staticAvailable;

public:
	//{{AFX_VIRTUAL(CPropPageOfferGroupEntryDept)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnGetDispInfoListSelected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListAvailable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnDoubleClickListSelected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickListAvailable(NMHDR* pNMHDR, LRESULT* pResult);
	
protected:
	//{{AFX_MSG(CPropPageOfferGroupEntryDept)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetRecordControls();

public:
	void SetOfferGroupRecord ( COfferGroupsCSVRecord* pOfferGroupRecord ) { m_pOfferGroupRecord = pOfferGroupRecord; }
	void BuildDeptArray();
	void Refresh();
	void SaveRecord();
	void SaveOfferGroupEntries();

private:
	void GetLineText( CReportConsolidationArray<CSortedIntWithFlag>& arrayItems, int nIdx, CCSV& csv );
	
private:
	COfferGroupsCSVRecord* m_pOfferGroupRecord;
	
private:
	CTwinListHelpers m_TwinListHelpers;
	CSSListMultiSelectCtrlDblClick m_listSelected;
	CSSListMultiSelectCtrlDblClick m_listAvailable;
	CReportConsolidationArray<CSortedIntWithFlag> m_arraySelected;
	CReportConsolidationArray<CSortedIntWithFlag> m_arrayAvailable;
	
private:
	int m_nEntryType;
	bool m_bDialogReady;
};

/**********************************************************************/
#endif
/**********************************************************************/
