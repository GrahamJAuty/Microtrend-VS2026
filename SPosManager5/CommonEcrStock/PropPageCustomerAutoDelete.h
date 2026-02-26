#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageCustomerAutoDelete : public CSSPropertyPage
{
public:
	CPropPageCustomerAutoDelete( int& nIndex, bool bOneOff );
	~CPropPageCustomerAutoDelete();

	//{{AFX_DATA(CPropPageCustomerAutoDelete)
	enum { IDD = IDD_PROPPAGE_CUSTOMER_AUTODELETE };
	//}}AFX_DATA
	CEdit m_editName1;
	CEdit m_editName2;
	CEdit m_editCreateDateEpos;
	CDateTimeCtrl m_DatePickerCreateEpos;
	CEdit m_editCreateTimeEpos;
	CStatic m_staticProgram;
	CEdit m_editCreateDateRecord;
	CDateTimeCtrl m_DatePickerCreateRecord;
	CEdit m_editCreateTimeRecord;
	CEdit m_editDelete;
	CButton m_checkDelete;
	CDateTimeCtrl m_DatePickerDelete;
	
	//{{AFX_VIRTUAL(CPropPageCustomerAutoDelete)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	void OnToggleSpecifyDate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageCustomerAutoDelete)
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
	bool m_bAutoDelete;
};

/**********************************************************************/
#endif
/**********************************************************************/

