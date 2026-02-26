#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPluSearchDlg : public CSSDialog
{
public:
	CPluSearchDlg( int nMode, CWnd* pParent = NULL);
	
	void SetPluNo( __int64 nPluNo ){ m_nPluNo = nPluNo; }
	void SetBarcodeNo( __int64 nBarcodeNo ){ m_nBarcodeNo = nBarcodeNo; }
	__int64 GetPluNo(){ return m_nPluNo; }
	__int64 GetBarcodeNo(){ return m_nBarcodeNo; }

	//{{AFX_DATA(CPluSearchDlg)
	enum { IDD = IDD_PLU_SEARCH };
	//}}AFX_DATA
	CButton m_radioPlu;
	CButton m_radioBarcode;
	int m_nMode;

	//{{AFX_VIRTUAL(CPluSearchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();

private:
	CEdit* GetEditPlu();
	CEdit* GetEditBarcode();

protected:
	//{{AFX_MSG(CPluSearchDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnRadioPlu();
	afx_msg void OnRadioBarcode();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnKillFocusBarcodeNo();
	DECLARE_MESSAGE_MAP()

private:
	__int64 m_nPluNo;
	__int64 m_nBarcodeNo;
	CString m_strPluNo;
	CString m_strBarcodeNo;
};

/**********************************************************************/
