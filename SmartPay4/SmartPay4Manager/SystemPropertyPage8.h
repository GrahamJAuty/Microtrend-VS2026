#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CSystemPropertyPage8 : public CSSAutoShutdownPropertyPage
{
public:
	CSystemPropertyPage8();
	virtual ~CSystemPropertyPage8();

	enum { IDD = IDD_PROPPAGE_SYSTEM8 };
	CButton m_checkExportPhoto;
	CEdit m_editEposFolder;
	CButton m_checkCopyPhoto;
	CButton m_buttonCopyNow;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleExportPhoto();
	afx_msg void OnToggleCopyPhoto();
	afx_msg void OnButtonCopyNow();
	/*****/
	afx_msg void OnButtonDeleteNotification();
	
	DECLARE_MESSAGE_MAP()

private:
	void SetDisplay();
};

//$$******************************************************************
