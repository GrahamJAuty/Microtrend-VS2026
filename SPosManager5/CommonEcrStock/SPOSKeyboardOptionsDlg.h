#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardOptionsDlg : public CSSDialog
{
public:
	CSPOSKeyboardOptionsDlg(CWnd* pParent = NULL);  

	//{{AFX_DATA(CSPOSKeyboardOptionsDlg)
	enum { IDD = IDD_SPOS_KEYBOARD_OPTIONS };
	int		m_nXMarginDisplay;
	BOOL	m_bShowPlu;
	BOOL	m_bBold;
	BOOL	m_bEditKeyTopLeft;
	BOOL	m_bImageWithStyle;
	//}}AFX_DATA
	CSSComboBox m_comboView;
	CSSComboBox m_comboZoom;
	CSSTabbedComboBox m_comboScreen;

	//{{AFX_VIRTUAL(CSPOSKeyboardOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSPOSKeyboardOptionsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
