#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "BarcodeFilterArray.h"
#include "DepartmentSelector.h"
//include "MySSDialog.h"
//include "MySSListCtrl.h"
/**********************************************************************/

class CBarcodeFilterDlg : public CSSDialog
{
public:
	CBarcodeFilterDlg( CBarcodeFilterArray& FilterArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CBarcodeFilterDlg)
	enum { IDD = IDD_BARCODE_FILTER };
	CStatic m_staticFilter;
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CStatic m_staticStockCode;
	CEdit m_editStockCode;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	CString	m_strSearchText;
	CString	m_strStockCode;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBarcodeFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	int GetBarcodeIdx() { return m_nBarcodeIdx; }
	int GetRecordCount() { return m_BarcodeFilterArray.GetSize(); }

private:
	void RunSearch( bool bDoSearch, bool bUpdateDisplay );
	void UpdateTitle();

private:
	CSSListCtrlVirtual m_listBarcode;
	CBarcodeFilterArray& m_BarcodeFilterArray;
	int m_nBarcodeIdx;

protected:
	//{{AFX_MSG(CBarcodeFilterDlg)
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnSelectDepartment();
	afx_msg void OnSelectStockCode();
	afx_msg void OnSelectSearchText();
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDepartmentSelector m_DepartmentSelector;
	int m_nDepartmentFilter;
	CString m_strTitle;

private:
	int m_nCacheIndex;
	CCSV m_CacheCSV;
};

/**********************************************************************/
#endif
/**********************************************************************/
