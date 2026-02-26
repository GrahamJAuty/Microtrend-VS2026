#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CDefEditRoomTypeDlg : public CSSDialog
{
public:
	CDefEditRoomTypeDlg( int nSleeps, CString& strLine, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditRoomTypeDlg)
	enum { IDD = IDD_DEF_EDIT_ROOMTYPE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditRoomTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditRoomTypeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCheckType2();
	afx_msg void OnCheckType3();
	afx_msg void OnCheckType4();
	afx_msg void OnCheckType5();
	afx_msg void OnCheckType6();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	void InitialiseType( CCSV& csvLine, int nOffset, CButton& check, CEdit& edit );
	void AddToLine( CCSV& csvLine, bool bEnable, CEdit& edit, bool bForceText );

private:
	CButton m_checkType2;
	CButton m_checkType3;
	CButton m_checkType4;
	CButton m_checkType5;
	CButton m_checkType6;
	CEdit m_editType1;
	CEdit m_editType2;
	CEdit m_editType3;
	CEdit m_editType4;
	CEdit m_editType5;
	CEdit m_editType6;

private:
	int m_nSleeps;
	CString& m_strLine;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
