#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
#include "ProcessedOrderDetails.h"
/**********************************************************************/

class CPropPagePrintOrderAddress : public CPropertyPage
{
public:
	CPropPagePrintOrderAddress();
	void SetOrderDetails( CProcessedOrderDetails* pDetails ) { m_pOrderDetails = pDetails; }
	void SetDefSpIdx( int n ){ m_nDefSpIdx = n; }

	//{{AFX_DATA(CPropPagePrintOrderAddress)
	enum { IDD = IDD_PROPPAGE_PRINTORDER_ADDRESS };
	CEdit	m_editPostcode;
	CEdit	m_editPhone;
	CEdit	m_editFax;
	CEdit	m_editEmail;
	CEdit	m_editAddress3;
	CEdit	m_editAddress2;
	CEdit	m_editAddress1;
	CSSComboBoxEx	m_comboSite;
	CString	m_strAddress1;
	CString	m_strAddress2;
	CString	m_strAddress3;
	CString	m_strEmail;
	CString	m_strFax;
	CString	m_strPhone;
	CString	m_strPostcode;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePrintOrderAddress)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPagePrintOrderAddress)
	afx_msg void OnSelectSite();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DisplayAddress ( CLocationSelectorEntity& LocSelEntity );

private:
	CLocationSelector m_LocationSelector;
	CProcessedOrderDetails* m_pOrderDetails;
	int m_nDefSpIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
