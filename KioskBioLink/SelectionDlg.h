#pragma once
//*******************************************************************
#include "CardLink.h"
//*******************************************************************
#include "UserIDRecord.h"
//*******************************************************************

class CSelectionDlg : public CSSColourDialog
{
public:
	CSelectionDlg( CStringArray* parrayUserLines, CUserIDRecord* pRecord, CWnd* pParent); 
	
	enum { IDD = IDD_SELECTIONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPrevious();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonImage1();
	afx_msg void OnButtonImage2();
	afx_msg void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	void DisplayScreen ( int nScreenNo );
	void SetImageButtonText (CSSColourButton& button);

public:
	CString m_strUserID = "";

private:
	CStringArray* m_parrayUserLines;
	CUserIDRecord* m_pRecord = nullptr;

	int m_nScreenNo = 0;
	int m_nIndexPtrButton1 = 0;
	int m_nIndexPtrButton2 = 0;

	CSSColourStatic m_staticTitle;
	CSSColourButton m_buttonPrevious;
	CSSColourButton m_buttonNext;
	CSSColourButton m_buttonFinish;

	CSSColourStatic m_staticCount1;
	CSSColourStatic m_staticCardNo1;
	CSSColourStatic m_staticSurname1;
	CSSColourStatic m_staticForename1;
	CSSColourStatic m_staticTextGroupNo1;
	CSSColourStatic m_staticGroupNo1;
	CSSColourButton m_buttonImage1;

	CSSColourStatic m_staticImage2;
	CSSColourStatic m_staticCount2;
	CSSColourStatic m_staticCardNo2;
	CSSColourStatic m_staticSurname2;
	CSSColourStatic m_staticForename2;
	CSSColourStatic m_staticTextGroupNo2;
	CSSColourStatic m_staticGroupNo2;
	CSSColourButton m_buttonImage2;
};

//*******************************************************************