#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPagePMSOptionsPayment : public CSSPropertyPage
{
public:
	CPropPagePMSOptionsPayment();
	~CPropPagePMSOptionsPayment();

	//{{AFX_DATA(CPropPagePMSOptionsPayment)
	enum { IDD = IDD_PROPPAGE_PMS_OPTIONS_PAYMENT };
	//}}AFX_DATA
	CSSComboBox m_comboPayment[10];
	
public:
	//{{AFX_VIRTUAL(CPropPagePMSOptionsPayment)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPagePMSOptionsPayment)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
