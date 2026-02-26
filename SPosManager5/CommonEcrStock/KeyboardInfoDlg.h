#pragma once
/**********************************************************************/
#include "KeyboardMap.h"
#include "KeyboardNameArray.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CKeyboardInfoDlg : public CDialog
{
public:
	CKeyboardInfoDlg( CDataNodeInfoKeyboard& infoNode, CString& strName, CWnd* pParent = NULL);   

	//{{AFX_DATA(CKeyboardInfoDlg)
	enum { IDD = IDD_KEYBOARD_INFO };
	CEdit	m_editModified;
	CEdit m_editName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CKeyboardInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CKeyboardInfoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDataNodeInfoKeyboard& m_infoNode;
	CString& m_strName;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/

