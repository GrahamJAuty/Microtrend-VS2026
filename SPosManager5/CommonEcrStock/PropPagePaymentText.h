#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PaymentCSVArray.h"
/**********************************************************************/

class CPropPagePaymentText : public CSSPropertyPage
{
public:
	CPropPagePaymentText();
	~CPropPagePaymentText();

	//{{AFX_DATA(CPropPagePaymentText)
	enum { IDD = IDD_PROPPAGE_PAYMENT_TEXT };
	CEdit	m_editText;
	CEdit	m_editEPOSName;
	CEdit	m_editReportName;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPagePaymentText)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPagePaymentText)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetPaymentRecord ( CPaymentCSVRecord* pPaymentRecord ) { m_pPaymentRecord = pPaymentRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	
private:
	CPaymentCSVRecord* m_pPaymentRecord;
};

/**********************************************************************/
#endif // __PROPPAGEPAYMENTTEXT_H__
/**********************************************************************/
