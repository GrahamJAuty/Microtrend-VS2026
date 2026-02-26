#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPluLengthDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CPluLengthDlg)

public:
	CPluLengthDlg(CWnd* pParent = NULL); 
	virtual ~CPluLengthDlg();

	enum { IDD = IDD_PLU_LENGTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnSelectLength0();
	afx_msg void OnSelectLength1();
	afx_msg void OnSelectLength2();
	afx_msg void OnSelectLength3();
	afx_msg void OnSelectLength4();
	afx_msg void OnSelectLength5();
	afx_msg void OnSelectLength6();
	afx_msg void OnSelectLength7();
	afx_msg void OnSelectLength8();
	afx_msg void OnSelectLength9();
	afx_msg void OnSelectLength10();
	afx_msg void OnSelectLength11();
	afx_msg void OnSelectLength12();
	afx_msg void OnSelectLength13();
	
	DECLARE_MESSAGE_MAP()

private:
	void SelectLength( int nLength );
	void GetTexts( int nLength, CString& strText1, CString& strText2, CString& strText3 );

private:
	WORD m_IDLength[14];

private:
	CButton m_radioLength[14];
	CButton m_checkAltFiles;
	CStatic m_staticText1;
	CStatic m_staticText2;
	CStatic m_staticText3;
};

/**********************************************************************/
