#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CSystemPropertyPage4 : public CSSPropertyPage
{
public:
	CSystemPropertyPage4();
	virtual ~CSystemPropertyPage4();
	
	enum { IDD = IDD_PROPPAGE_SYSTEM4 };
	CStatic m_staticGeneral;
	CSSComboBox m_comboStartOfWeek;
	CButton m_buttonPhotoID;
	CButton m_checkEnablePhotoID;
	CButton m_checkShowPurchases;
	CSSComboBox m_comboSetFieldNames;
	CEdit m_editPrinterReport;
	CStatic m_staticBarcode1;
	CStatic m_staticBarcode2;
	CButton m_buttonPrinterBarcode;
	CEdit m_editPrinterBarcode;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnButtonPhotoID();
	afx_msg void OnTogglePhotoID();
	afx_msg void OnButtonSet();
	afx_msg void OnButtonPrinterBarcode();
	afx_msg void OnButtonPrinterReport();
	
	DECLARE_MESSAGE_MAP()

private:
	void SetDisplay();

public:
	void ShowPrinterList( CSSPropertyPage* pParentWnd, CEdit& EditPrinter, CString& strDefaultPrinter, const char* szAction );
	void PrinterListError( int n );

private:
	CString m_strSystemDefaultPrinter;
};

//$$******************************************************************
