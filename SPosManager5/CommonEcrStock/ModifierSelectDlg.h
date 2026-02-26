#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CModifierSelectDlg : public CDialog
{
public:
	CModifierSelectDlg( int nNodeType, int nCommonBits, int nMask, const char* szPluNo, CWnd* pParent = NULL);  

	//{{AFX_DATA(CModifierSelectDlg)
	enum { IDD = IDD_MODIFIER_SELECT };
	CButton	m_checkChange[10];
	CButton	m_checkModifier[10];
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CModifierSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CModifierSelectDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnToggleChange();

public:
	int GetFinalCommonBits(){ return m_nFinalCommonBits; }
	int GetFinalMask(){ return m_nFinalMask; }
	void SetNoDescriptionFlag( bool b ){ m_bNoDescription = b; }

private:
	CString m_strPluNo;
	int m_nNodeType;
	int m_nInitialCommonBits;
	int m_nInitialMask;
	int m_nFinalCommonBits;
	int m_nFinalMask;
	bool m_bNoDescription;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
