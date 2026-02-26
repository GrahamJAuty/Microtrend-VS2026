#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CServerPropertyPage2 : public CSSPropertyPage
{
public:
	CServerPropertyPage2();
	virtual ~CServerPropertyPage2();

	enum { IDD = IDD_PROPPAGE_SERVER2 };
	/*****/
	CSSComboBox m_comboLineType[8];
	CEdit m_editLineText[8];
	/*****/
	CButton m_buttonBrowse2;
	CButton m_buttonTest2;
	CButton m_radioPhotoID1;
	CButton m_radioPhotoID2;
	CButton m_radioPhotoID3;
	CStatic m_staticLoyaltyUNCPath1;
	CStatic m_staticLoyaltyUNCPath2;
	CEdit m_editLoyaltyUNCPath1;
	CEdit m_editLoyaltyUNCPath2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg void OnSelectField1();
	afx_msg void OnSelectField2();
	afx_msg void OnSelectField3();
	afx_msg void OnSelectField4();
	afx_msg void OnSelectField5();
	afx_msg void OnSelectField6();
	afx_msg void OnSelectField7();
	afx_msg void OnSelectField8();
	/*****/
	afx_msg void OnRadioPhotoID1();
	afx_msg void OnRadioPhotoID2();
	afx_msg void OnRadioPhotoID3();
	afx_msg void OnButtonBrowse2();
	afx_msg void OnButtonTest2();
	/*****/
	
	DECLARE_MESSAGE_MAP()

private:
	void SetDisplay();
	void EnableField ( CEdit& Edit, const char* szField );
	void AddFieldName ( const char* szText );
	const char* TextFromEditBox( CEdit& Edit );
	const char* TextFromComboBox( CComboBox& Combo );
	void SetPhotoIDFolderType( int n );
	void SelectField( int n );
	
private:
	CString m_strEditBoxText;
	CString m_strComboText;
	int m_nPhotoIDFolderType;
};

//$$******************************************************************
