#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//$$******************************************************************

class CServerPropertyPage2 : public CSSAutoShutdownPropertyPage
{
public:
	CServerPropertyPage2();
	virtual ~CServerPropertyPage2();

	enum { IDD = IDD_PROPPAGE_SERVER2 };
	/*****/
	CButton m_buttonBrowse;
	CButton m_buttonTest;
	CButton m_radioPhotoID1;
	CButton m_radioPhotoID2;
	CButton m_radioPhotoID3;
	CStatic m_staticSmartPayUNCPath1;
	CStatic m_staticSmartPayUNCPath2;
	CEdit m_editSmartPayUNCPath1;
	CEdit m_editSmartPayUNCPath2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	/*****/
	afx_msg void OnRadioPhotoID1();
	afx_msg void OnRadioPhotoID2();
	afx_msg void OnRadioPhotoID3();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonTest();
	
	DECLARE_MESSAGE_MAP()

private:
	void SetPhotoIDFolderType(int n);

private:
	int m_nPhotoIDFolderType;
};

//$$******************************************************************
