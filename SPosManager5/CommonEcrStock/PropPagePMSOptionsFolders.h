#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPagePMSOptionsFolders : public CPropertyPage
{
public:
	CPropPagePMSOptionsFolders();
	~CPropPagePMSOptionsFolders();

	//{{AFX_DATA(CPropPagePMSOptionsFolders)
	enum { IDD = IDD_PROPPAGE_PMS_OPTIONS_FOLDERS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePMSOptionsFolders)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnButtonSetRoom();
	afx_msg void OnButtonSetSales();
	
protected:
	//{{AFX_MSG(CPropPagePMSOptionsFolders)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CButton m_buttonSetRoom;
	CButton m_buttonSetSales;
	CEdit m_editFolderRoom;
	CEdit m_editFolderSales;

private:
	CString m_strFolderRoom;
	CString m_strFolderSales;

private:
	void SaveRecord();
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
