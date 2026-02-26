#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPropPagePMSOptionsInvoice : public CSSPropertyPage
{
public:
	CPropPagePMSOptionsInvoice();
	~CPropPagePMSOptionsInvoice();

	//{{AFX_DATA(CPropPagePMSOptionsInvoice)
	enum { IDD = IDD_PROPPAGE_PMS_OPTIONS_INVOICE };
	//}}AFX_DATA
	CSSComboBox m_comboInvoice;
	CButton m_checkAddress;
	CButton m_checkLines;
	CButton m_checkReference;
	CButton m_checkNumber;
	CButton m_checkInterim;
	CButton m_checkNote;
	CButton m_checkStay;

private:
	CEdit* GetEditLines();
	
public:
	//{{AFX_VIRTUAL(CPropPagePMSOptionsInvoice)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnToggleLines();
	
protected:
	//{{AFX_MSG(CPropPagePMSOptionsInvoice)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();
	
private:
	int m_nLines;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
