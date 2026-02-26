#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SupplierSetCSVArray.h"
/**********************************************************************/

class CSupplierSetEditDlg : public CDialog
{
public:
	CSupplierSetEditDlg( CSupplierSetCSVRecord& BatchRecord, CWnd* pParent = NULL);   

	//{{AFX_DATA(CSupplierSetEditDlg)
	enum { IDD = IDD_SUPPLIERSET_EDIT };
	CString	m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSupplierSetEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSupplierSetEditDlg)
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddList();

private:
	CSSListTaggedSelectCtrl m_listSuppliers;
	CSupplierSetCSVRecord& m_SupplierSetRecord;
};

/**********************************************************************/
//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/
