#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "CommentHandler.h"
/**********************************************************************/

class CCommentDlg : public CSSAutoShutdownDialog
{
public:
	CCommentDlg(const char* szCaption, CWnd* pParent = NULL);   
	virtual ~CCommentDlg();

	enum { IDD = IDD_COMMENT };
	CSSComboBox m_comboComments;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	CString m_strComment;
	CString m_strMessage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	afx_msg void OnClickStaticComment();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void AddComments();

private:
	CCommentHandler m_Comments;
	CString m_strCaption;
};


/**********************************************************************/