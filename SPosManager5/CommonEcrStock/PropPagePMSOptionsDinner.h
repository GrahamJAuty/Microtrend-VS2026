#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "Consolidation.h"
#include "PMSOptions.h"
#include "ReportConsolidationArray.h"
#include "TwinListHelpers.h"
/**********************************************************************/

class CPropPagePMSOptionsDinner : public CSSPropertyPage
{
public:
	CPropPagePMSOptionsDinner();
	~CPropPagePMSOptionsDinner();

	//{{AFX_DATA(CPropPagePMSOptionsDinner)
	enum { IDD = IDD_PROPPAGE_PMS_OPTIONS_DINNER };
	//}}AFX_DATA

	void SetPMSOptions( CPMSOptions* p ){ m_pPMSOptions = p; }

	double m_dDinnerAllowance;
		
public:
	//{{AFX_VIRTUAL(CPropPagePMSOptionsDinner)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnGetDispInfoListSelected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListAvailable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToggleLines();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnDoubleClickListSelected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickListAvailable(NMHDR* pNMHDR, LRESULT* pResult);
	
protected:
	//{{AFX_MSG(CPropPagePMSOptionsDinner)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GetLineText( CReportConsolidationArray<CSortedIntWithFlag>& arrayItems, int nIdx, CCSV& csv );
	
private:
	void SaveRecord();

private:
	CPMSOptions* m_pPMSOptions;
	CTwinListHelpers m_TwinListHelpers;
	CSSListMultiSelectCtrlDblClick m_listSelected;
	CSSListMultiSelectCtrlDblClick m_listAvailable;
	CReportConsolidationArray<CSortedIntWithFlag> m_arraySelected;
	CReportConsolidationArray<CSortedIntWithFlag> m_arrayAvailable;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
