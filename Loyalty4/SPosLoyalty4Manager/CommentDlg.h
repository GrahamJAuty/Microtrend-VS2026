//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "CommentHandler.h"
//$$******************************************************************

class CCommentDlg : public CSSDialog
{
public:
	CCommentDlg(const char* szCaption, CWnd* pParent = NULL);   
	
	enum { IDD = IDD_COMMENT };
	CSSComboBox m_comboComments;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	CStatic m_staticMessage;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	afx_msg void OnClickStaticComment();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

public:
	CString m_strMessage;
	CString m_strComment;

private:
	void AddComments();

private:
	CString m_strCaption;
	CCommentHandler m_Comments;
};

//$$******************************************************************
