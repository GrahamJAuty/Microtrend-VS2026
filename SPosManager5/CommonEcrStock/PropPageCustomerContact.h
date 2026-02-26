#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageCustomerContact : public CSSPropertyPage
{
public:
	CPropPageCustomerContact( int& nIndex, bool bOneOff );
	~CPropPageCustomerContact();

	//{{AFX_DATA(CPropPageCustomerContact)
	enum { IDD = IDD_PROPPAGE_CUSTOMER_CONTACT };
	//}}AFX_DATA
	CEdit m_editName1;
	CEdit m_editName2;
	CEdit m_editEmail;
	CButton m_checkRedirect;
	CEdit m_editPhone1;
	CEdit m_editPhone2;
	CEdit m_editPhone3;
	
	//{{AFX_VIRTUAL(CPropPageCustomerContact)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPageCustomerContact)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	int& m_nIndex;
	bool m_bOneOff;
};

/**********************************************************************/
#endif
/**********************************************************************/

