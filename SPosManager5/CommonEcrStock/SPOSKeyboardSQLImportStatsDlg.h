#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SPOSKeyboardSQLImporter.h"
/**********************************************************************/

class CSPOSKeyboardSQLImportStatsDlg : public CDialog
{
public:
	CSPOSKeyboardSQLImportStatsDlg( CSPOSKeyboardSQLImporter& SQLImporter, CWnd* pParent = NULL);   

// Dialog Data
	//{{AFX_DATA(CSPOSKeyboardSQLImportStatsDlg)
	enum { IDD = IDD_SPOS_KEYBOARD_SQL_IMPORTSTATS };
	CStatic	m_staticMacros;
	CStatic	m_staticScreenModes;
	CStatic	m_staticListItems;
	CStatic	m_staticListHeaders;
	CStatic	m_staticColours;
	BOOL m_bDelete;
	BOOL m_bWantLayout;
	BOOL m_bWantMacros;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardSQLImportStatsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSPOSKeyboardSQLImportStatsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CSPOSKeyboardSQLImporter& m_SQLImporter;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/

