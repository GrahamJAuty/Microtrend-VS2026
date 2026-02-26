#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "BarcodeFilterArray.h"
#include "LabelListCreator.h"
#include "LabelListCSVArray.h"
#include "LabelNameHandler.h"
#include "LabelPriceInfo.h"
//include "MySSDialog.h"
//include "MySSListCtrl.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CListLabelsDlg : public CSSDialog
{
public:
	CListLabelsDlg( bool bAdhoc, bool bReadOnly, bool bAutoPrint, CLabelListCSVArray& LabelList, const char* szStockpointName, CWnd* pParent = NULL);   
	~CListLabelsDlg();

	//{{AFX_DATA(CListLabelsDlg)
	enum { IDD = IDD_LIST_LABELS };
	CStatic	m_staticPage;
	CSSComboBox	m_comboLabelName;
	CButton	m_buttonAdd;
	CButton	m_buttonEdit;
	CButton	m_buttonDelete;
	CButton	m_buttonDisplay;
	CButton	m_buttonPrint;
	CButton	m_buttonSave;
	CButton	m_buttonOK;
	CButton	m_buttonCancel;
	int		m_nFirstLabel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CListLabelsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CListLabelsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonEdit();
	virtual void OnOK();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonDisplay();
	afx_msg void OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
protected:
	void DisplayScreen ( int nIndex );

private:
	void PrintLabels ( bool bPrint );

protected:
	CSSListCtrlVirtual m_list;
	
private:
	CBarcodeFilterArray m_BarcodeFilterArray;
	CPluFilterArray m_PluFilterArray;
	CLabelListCSVArray& m_LabelList;

private:
	bool m_bBarcode;
	CString m_strStockpointName;

private:
	CLabelNameHandler m_LabelNameHandler;
	CLabelListCreator m_LabelListCreator;

private:
	bool m_bAdhoc;
	bool m_bReadOnly;
	bool m_bAutoPrint;
	bool m_bPrinted;
	int m_nLabelCount;
};

/**********************************************************************/
#endif
/**********************************************************************/
