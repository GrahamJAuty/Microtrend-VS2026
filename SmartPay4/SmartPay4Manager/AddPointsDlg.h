#pragma once
//*******************************************************************
#include "CommentHandler.h"
//*******************************************************************

class CAddPointsDlg : public CSSAutoShutdownDialog
{
public:
	CAddPointsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddPointsDlg();

// Dialog Data
	enum { IDD = IDD_ADDPOINTS };
	int m_nValue;
	int m_nPointsBalance;
	CSSComboBox m_comboComments;
	CString m_strComment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	afx_msg void OnStnClickedStaticComment();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void AddComments();

private:
	CCommentHandler m_Comments;
};

//*******************************************************************
