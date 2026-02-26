#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
//include "MySSDialog.h"
#include "SessionCSVArray.h"
/**********************************************************************/

class CSessionEditDlg : public CSSDialog
{
public:
	CSessionEditDlg( CSessionCSVRecord& Session, bool bAdhoc, CWnd* pParent = NULL);   

	//{{AFX_DATA(CSessionEditDlg)
	enum { IDD = IDD_SESSION_EDIT };
	CStatic m_staticGeneral;
	CStatic m_staticName;
	CEdit m_editName;
	CStatic m_staticStart;
	CDateTimeCtrl	m_TimePickerStart;
	CStatic m_staticEnd;
	CDateTimeCtrl	m_TimePickerEnd;
	CStatic m_staticMatch;
	CSSComboBox	m_comboMatch;
	CStatic m_staticSession;
	CButton	m_radioStartTime2;
	CStatic m_staticList;
	CSSComboBox	m_comboList;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	/****/
	CString	m_strName;
	int		m_nSessionStartType;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CSessionEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSessionEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CSessionCSVRecord& m_Session;
	bool m_bAdhoc;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
