#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/

class CDefEditDlg : public CSSAutoShutdownDialog
{
public:
	CDefEditDlg(CWnd* pParent = NULL);
	CDefEditDlg( const char* szTitle, const char* szName, int nMaxLen, const char* szPrompt = "", CWnd* pParent = NULL);

	//{{AFX_DATA(CDefEditDlg)
	enum { IDD = IDD_DEF_EDIT };
	CStatic	m_staticRef;
	CString	m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

protected:
	virtual void SetDialogSize(){}

protected:
	CString m_strTitle;
	CString m_strPrompt;
	int m_nMaxLen;

protected:
	CEdit m_editName;
	CButton m_buttonOK;
	CButton m_buttonCancel;
};

/**********************************************************************/

class CDefEditASDlg : public CSSAutoShutdownDialog
{
public:
	CDefEditASDlg( CWnd* pParent = NULL);
	CDefEditASDlg( const char* szTitle, const char* szName, int nMaxLen, const char* szPrompt = "", CWnd* pParent = NULL);

	//{{AFX_DATA(CDefEditASDlg)
	enum { IDD = IDD_DEF_EDIT };
	CStatic	m_staticRef;
	CString	m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditASDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditASDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void SetDialogSize() {}

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	
protected:
	CString m_strTitle;
	CString m_strPrompt;
	int m_nMaxLen;

protected:
	CEdit m_editName;
	CButton m_buttonOK;
	CButton m_buttonCancel;
};

/**********************************************************************/
