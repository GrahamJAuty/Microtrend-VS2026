#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageCustomerGeneral : public CSSPropertyPage
{
public:
	CPropPageCustomerGeneral( bool& bAdminEnabled, int& nIndex, bool bOneOff );
	~CPropPageCustomerGeneral();

	//{{AFX_DATA(CPropPageCustomerGeneral)
	enum { IDD = IDD_PROPPAGE_CUSTOMER_GENERAL };
	//}}AFX_DATA
	CEdit m_editName1;
	CEdit m_editName2;
	CEdit m_editAddress1;
	CEdit m_editAddress2;
	CEdit m_editAddress3;
	CEdit m_editPostcode;
	CButton m_checkDate;
	CDateTimeCtrl m_DatePickerDate;
	CDateTimeCtrl m_DatePickerTime;
	CStatic m_staticBalance;
	CButton m_buttonAdmin;
	double m_dBalance;
	
	//{{AFX_VIRTUAL(CPropPageCustomerGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	void OnToggleSpecifyDate();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonAdmin();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

private:
	CEdit* GetEditBalance();
	
protected:
	//{{AFX_MSG(CPropPageCustomerGeneral)
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
	bool m_bRightClicked;
	bool& m_bAdminEnabled;
	int& m_nIndex;
	bool m_bOneOff;
};

/**********************************************************************/
#endif
/**********************************************************************/

