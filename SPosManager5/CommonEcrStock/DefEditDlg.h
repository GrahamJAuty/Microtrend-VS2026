#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditDlg : public CSSDialog
{
public:
	CDefEditDlg(CWnd* pParent = NULL);
	CDefEditDlg( const char* szTitle, const char* szName, int nMaxLen, const char* szPrompt = "", CWnd* pParent = NULL);

	//{{AFX_DATA(CDefEditDlg)
	enum { IDD = IDD_DEF_EDIT };
	CStatic	m_staticRef;
	CString	m_strName;
	CButton m_radioPrevious;
	CButton m_radioNext;
	CEdit m_editName;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioPrevious();
	afx_msg void OnRadioNext();
	afx_msg void OnOK() { HandleOK(); }
	afx_msg void OnCancel() { HandleCancel(); }
	virtual void PostNcDestroy() { CDialog::PostNcDestroy(); }
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	bool GetPreviousFlag() { return m_bPrevious; }

protected:
	virtual bool AdjustDialogControls();
	virtual void HandleOK();
	virtual void HandleCancel();
	
protected:
	CString m_strTitle;
	CString m_strPrompt;
	int m_nMaxLen;

protected:
	bool m_bPrevious;
};

/**********************************************************************/

class CChangePasswordDlg : public CDefEditDlg
{
public:
	CChangePasswordDlg( const char* szTitle, const char* szName, int nMaxLen, const char* szPrompt = "", CWnd* pParent = NULL);
	bool AdjustDialogControls();
};

/**********************************************************************/

class CChangeReportPointNameDlg : public CDefEditDlg
{
public:
	CChangeReportPointNameDlg( int nIndex, CWnd* pParent = NULL);
};

/**********************************************************************/

class CChangeKeyboardSetNameDlg : public CDefEditDlg
{
public:
	CChangeKeyboardSetNameDlg( int nIndex, CWnd* pParent = NULL);
};

/**********************************************************************/

class CChangeLocationSetNameDlg : public CDefEditDlg
{
public:
	CChangeLocationSetNameDlg( int nIndex, CWnd* pParent = NULL);
};

/**********************************************************************/

class CChangeSBLocationNameDlg : public CDefEditDlg
{
public:
	CChangeSBLocationNameDlg( int nIndex, CWnd* pParent = NULL);
};

/**********************************************************************/

class CChangePaymentNameDlg : public CDefEditDlg
{
public:
	CChangePaymentNameDlg( int nIndex, CWnd* pParent = NULL);
};

/**********************************************************************/

class CChangeListNameDlg : public CDefEditDlg
{
public:
	CChangeListNameDlg( int nIndex, CWnd* pParent = NULL);
};

/**********************************************************************/

class CListDataSearchDlg : public CDefEditDlg
{
public:
	CListDataSearchDlg(CListDataSearchDlg** pParentToThis, CString strTitle, CListDataDlg* pParent);
	~CListDataSearchDlg(){};

public:
	bool AdjustDialogControls();
	void LoadSettings(bool bPrevious, CString strText);

protected:
	virtual void HandleOK();
	virtual void HandleCancel();
	virtual void PostNcDestroy();

private:
	CListDataSearchDlg** m_pParentToThis;
	CListDataDlg* m_pParent;
};

/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

class CChangeRoomGroupNameDlg : public CDefEditDlg
{
public:
	CChangeRoomGroupNameDlg( int nIndex, CWnd* pParent = NULL);
};

/**********************************************************************/
#endif
/**********************************************************************/

class CChangeSystemDepartmentNameDlg : public CDefEditDlg
{
public:
	CChangeSystemDepartmentNameDlg( int nIndex, CWnd* pParent = NULL);
};

/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/

class CChangeEcrmanSupplierNameDlg : public CDefEditDlg
{
public:
	CChangeEcrmanSupplierNameDlg( int nIndex, CWnd* pParent = NULL);
};

/**********************************************************************/

class CFindEcrmanSuppRefDlg : public CDefEditDlg
{
public:
	CFindEcrmanSuppRefDlg( CWnd* pParent = NULL);
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
