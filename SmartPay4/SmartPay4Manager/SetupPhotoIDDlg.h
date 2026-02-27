#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "PhotoIDListData.h"
//*******************************************************************

class CSetupPhotoIDDlg : public CSSAutoShutdownDialog
{
public:
	CSetupPhotoIDDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_SETUP_PHOTOID };
	CSSComboBox m_comboLabel;
	CSSComboBox m_comboField1;
	CSSComboBox m_comboField2;
	CSSComboBox m_comboField3;
	CSSComboBox m_comboField4;
	CSSComboBox m_comboField5;
	CSSComboBox m_comboField6;
	CString	m_strLabelName;
	CString	m_strFieldText1;
	CString	m_strFieldText2;
	CString	m_strFieldText3;
	CString	m_strFieldText4;
	CString	m_strFieldText5;
	CString	m_strFieldText6;
	CString	m_strField1;
	CString	m_strField2;
	CString	m_strField3;
	CString	m_strField4;
	CString	m_strField5;
	CString	m_strField6;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelchangeComboLabels();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonSetup();
	afx_msg void OnButtonTest();
	afx_msg void OnBnClickedButtonClear();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	bool Save();
	void AddFieldName ( int nOpt, const char* szText );
	void SetLabelData();

	void SetTranslateTable();
	void CreateDisplayText ( const char* szHeaderText, const char* szUserText );
	const char* GetDisplayText ( const char* szHeaderText );
	const char* GetHeaderText ( const char* szDisplayName );

private:
	int m_nField1Opt;
	int m_nField2Opt;
	int m_nField3Opt;
	int m_nField4Opt;
	int m_nField5Opt;
	int m_nField6Opt;

	CPhotoIDListData m_data;					// reads default label data on class contruction

	CStringArray m_arrayFieldIn;
	CStringArray m_arrayFieldOut;
};

//*******************************************************************
