#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "LoyaltySchemeCSVArray.h"
#include "TwinListHelpers.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageLoyaltySchemePayments : public CSSPropertyPage
{
public:
	CPropPageLoyaltySchemePayments();
	~CPropPageLoyaltySchemePayments();

	//{{AFX_DATA(CPropPageLoyaltySchemePayments)
	enum { IDD = IDD_PROPPAGE_LOYALTYSCHEME_PAYMENTS };	
	CEdit	m_editName;
	CButton m_checkActive;
	CSSListMultiSelectCtrlDblClick m_listAllow;
	CSSListMultiSelectCtrlDblClick m_listDeny;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPageLoyaltySchemePayments)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnGetDispInfoListAllow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickListAllow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListDeny(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickListDeny(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAllow();
	afx_msg void OnButtonDeny();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();

protected:
	//{{AFX_MSG(CPropPageLoyaltySchemePayments)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetSchemeRecord ( CLoyaltySchemeCSVRecord* pSchemeRecord ) { m_pSchemeRecord = pSchemeRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	void GetLineText( CReportConsolidationArray<CSortedIntWithFlag>& arrayItems, int nIdx, CString& strLine );
	void RedrawLists();

private:
	CLoyaltySchemeCSVRecord* m_pSchemeRecord;

private:
	CTwinListHelpers m_TwinListHelpers;
	CReportConsolidationArray<CSortedIntWithFlag> m_arrayAllow;
	CReportConsolidationArray<CSortedIntWithFlag> m_arrayDeny;
};

/**********************************************************************/
#endif
/**********************************************************************/

