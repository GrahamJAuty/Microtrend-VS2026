#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CBarcodeDeleteDlg : public CDialog
{
public:
	CBarcodeDeleteDlg( CArray<int,int>& arrayBarcodeIdx, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CBarcodeDeleteDlg)
	enum { IDD = IDD_BARCODE_DELETE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBarcodeDeleteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CBarcodeDeleteDlg)
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()

private:
	void AddList();

private:
	CSSListTaggedSelectCtrl m_listBarcodes;
	
private:
	CArray<int,int>& m_arrayBarcodeIdx;
};

/**********************************************************************/

