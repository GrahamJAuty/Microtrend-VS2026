#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ComboListCtrl.h"
#include "MixMatchCSVArray.h"
#include "TimePlanTemplateListManager.h"
/**********************************************************************/

class CPropPageMixMatchTimeplan : public CSSPropertyPage
{
public:
	CPropPageMixMatchTimeplan();
	~CPropPageMixMatchTimeplan();

	//{{AFX_DATA(CPropPageMixMatchTimeplan)
	enum { IDD = IDD_PROPPAGE_MIXMATCH_TIMEPLAN };
	CEdit m_editDescription;
	CButton m_checkEnable;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPageMixMatchTimeplan)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT GetCellType(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCellButton(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageMixMatchTimeplan)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetMixMatchRecord ( CMixMatchCSVRecord* pMixMatchRecord ) { m_pMixMatchRecord = pMixMatchRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void SetRecordControls();

private:
	CTimePlanTemplateListManager m_ListManager;
	CComboListCtrl m_listSettings;
		
private:
	CMixMatchCSVRecord* m_pMixMatchRecord = nullptr;
};

/**********************************************************************/
#endif
/**********************************************************************/
