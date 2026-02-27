#pragma once
//********************************************************************
#include "resource.h"
//********************************************************************
#include "CommentHandler.h"
//********************************************************************

class CDeleteLeaverDlg : public CSSAutoShutdownDialog
{
public:
	CDeleteLeaverDlg(const char* szCaption, CWnd* pParent = NULL);
	
	enum { IDD = IDD_DELETELEAVER };
	CButton m_radioDelete2;
	CButton m_radioDelete3;
	CSSComboBox m_comboComments;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	CString m_strComment;
	CString m_strMessage;
	int m_nDeleteLeaver;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	afx_msg void OnClickStaticComment();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	CString m_strLeaversYear;

private:
	void AddComments();

private:
	CCommentHandler m_Comments;
	CString m_strCaption;
	CString m_strLeaverYear1;
	CString m_strLeaverYear2;
	CString m_strButton2;
	CString m_strButton3;
};

//********************************************************************
