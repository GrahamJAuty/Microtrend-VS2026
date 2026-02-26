#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentCSVArray.h"
#include "PictCtrl.h"
/**********************************************************************/

class CPropPageDepartmentImage : public CSSPropertyPage
{
public:
	CPropPageDepartmentImage();
	~CPropPageDepartmentImage();

	//{{AFX_DATA(CPropPageDepartmentImage)
	enum { IDD = IDD_PROPPAGE_DEPARTMENT_IMAGE };
	CEdit	m_editWidth;
	CEdit	m_editHeight;
	CEdit	m_editImageName;
	CButton m_buttonImage;
	CButton m_buttonDelete;
	CPictureCtrl m_staticImage;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPageDepartmentImage)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void OnButtonImage();
	afx_msg void OnButtonDelete();
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPageDepartmentImage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:
	void SetDepartmentRecord ( CDepartmentCSVRecord* pDeptRecord ) { m_pDeptRecord = pDeptRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	CDepartmentCSVRecord* m_pDeptRecord;
	CString m_strInitialDir;
};

/**********************************************************************/
#endif
/**********************************************************************/

