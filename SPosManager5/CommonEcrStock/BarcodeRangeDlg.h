#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CBarcodeRangeDlg : public CSSDialog
{
public:
	CBarcodeRangeDlg( const char* szTitle, CWnd* pParent = NULL); 

	void SetBarcodeFrom( __int64 n ){ m_nBarcodeFrom = n; }
	void SetBarcodeTo( __int64 n ){ m_nBarcodeTo = n; }
	__int64 GetBarcodeFrom(){ return m_nBarcodeFrom; }
	__int64 GetBarcodeTo(){ return m_nBarcodeTo; }

	//{{AFX_DATA(CBarcodeRangeDlg)
	enum { IDD = IDD_BARCODE_RANGE };
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CBarcodeRangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CBarcodeRangeDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusBarcodeNo();
	afx_msg void OnStaticFrom();
	afx_msg void OnStaticTo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditBarcodeFrom();
	CEdit* GetEditBarcodeTo();

private:
	CString m_strTitle;

	__int64 m_nBarcodeFrom;
	__int64 m_nBarcodeTo;
	CString m_strBarcodeFrom;
	CString m_strBarcodeTo;
};

/**********************************************************************/
