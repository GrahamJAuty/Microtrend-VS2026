#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CModifierEditDlg : public CSSDialog
{
public:
	CModifierEditDlg(CWnd* pParent = NULL); 

	//{{AFX_DATA(CModifierEditDlg)
	enum { IDD = IDD_MODIFIER_EDIT };
	double	m_dDummy;
	//}}AFX_DATA
	CStatic m_staticPoints;

	//{{AFX_VIRTUAL(CModifierEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

private:
	CEdit* GetEditPoints( int n );
	
private:
	int m_editNameID[9];
	CString m_strName[9];

	int m_editMultiplierQtyID[9];
	double m_dMultiplierQty[9];

	int m_editMultiplierPointsID[9];
	double m_dMultiplierPoints[9];

protected:
	//{{AFX_MSG(CModifierEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnButtonDownload();
	DECLARE_MESSAGE_MAP()

private:
	void HandleDownload( CArray<int,int>& arrayLocIdx );

private:
	CButton m_buttonDownload;
	bool m_bWarnCancelAfterDownload;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/
