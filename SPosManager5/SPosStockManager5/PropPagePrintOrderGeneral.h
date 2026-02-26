#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ProcessedOrderDetails.h"
/**********************************************************************/

class CPropPagePrintOrderGeneral : public CPropertyPage
{
public:
	CPropPagePrintOrderGeneral();
	void SetOrderDetails( CProcessedOrderDetails* pDetails ) { m_pOrderDetails = pDetails; }

	//{{AFX_DATA(CPropPagePrintOrderGeneral)
	enum { IDD = IDD_PROPPAGE_PRINTORDER_GENERAL };
	CString	m_strLine1;
	CString	m_strLine2;
	CString	m_strLine3;
	CString	m_strLine4;
	CString	m_strLine5;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePrintOrderGeneral)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPagePrintOrderGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonRestore();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void RestoreDefaults();

private:
	CProcessedOrderDetails* m_pOrderDetails;
};

/**********************************************************************/
#endif
/**********************************************************************/

