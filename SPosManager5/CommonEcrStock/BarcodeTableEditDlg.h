#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "BarcodeCSVArray.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CBarcodeTableEditDlg : public CSSDialog
{
public:
	CBarcodeTableEditDlg( int nInputLen, CBarcodePluNoTableInfo& info, CWnd* pParent = NULL);   

	//{{AFX_DATA(CBarcodeTableEditDlg)
	enum { IDD = IDD_BARCODETABLE_EDIT };
	BOOL	m_bChecksumBarcode;
	BOOL	m_bChecksumPlu;
	CStatic m_staticBarcode;
	CStatic m_staticPlu;
	CButton m_checkPluChecksum;
	CSpinButtonCtrl	m_spinPlu;
	CButton m_bitmapPlu1;
	CButton m_bitmapPlu2;
	CButton m_buttonCopy;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBarcodeTableEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CBarcodeTableEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaPosSpinBarcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaPosSpinPlu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToggleChecksumBarcode();
	afx_msg void OnToggleChecksumPlu();
	afx_msg void OnKillFocusEditBarcodeNo();
	afx_msg void OnKillFocusEditPluNo();
	afx_msg void OnButtonCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateEditBoxStringsBarcode();
	void UpdateEditBoxStringsPlu();
	void UpdateEditBoxEnablesBarcode();
	void UpdateEditBoxEnablesPlu();
	void UpdateFormatStringBarcode();
	void UpdateFormatStringPlu();

private:
	CEdit* GetEditBarcodeNo( int n );
	CEdit* GetEditPluNo( int n );

private:
	CBarcodePluNoTableInfo& m_infoFormat;
	
	CString m_strBarcodeNo[15];
	int m_nBarcodeNoID[15];

	CString m_strPluNo[14];
	int m_nPluNoID[14];

	int m_nInputLen;
};

/**********************************************************************/

