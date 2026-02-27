#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "CommentHandler.h"
//*******************************************************************

class CMoveLeaversDlg : public CSSAutoShutdownDialog
{
public:
	CMoveLeaversDlg(CWnd* pParent = NULL);   
	virtual ~CMoveLeaversDlg();

	enum { IDD = IDD_MOVELEAVERS };
	CSSComboBox m_comboComments;
	CButton m_radioDelete2;
	CButton m_radioDelete3;
	int m_nMoveLeaver;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual void OnOK();
	afx_msg void OnClickStaticComment();
	DECLARE_MESSAGE_MAP()

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	CString m_strLeaversYear;

private:
	void AddComments();

public:
	CString m_strComment;

private:
	CCommentHandler m_Comments;
	CString m_strLeaverYear1;
	CString m_strLeaverYear2;
	CString m_strButton2;
	CString m_strButton3;
};

//*******************************************************************
