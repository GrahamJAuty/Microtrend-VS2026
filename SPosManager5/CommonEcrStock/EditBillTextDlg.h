#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "BillAndReceiptTexts.h"
#include "LocationSelectorEntity.h"
/**********************************************************************/

class CEditBillTextDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CEditBillTextDlg)

public:
	CEditBillTextDlg( CLocationSelectorEntity& entity, CBillAndReceiptTexts& Text, CWnd* pParent = NULL );
	virtual ~CEditBillTextDlg();

	enum { IDD = IDD_EDIT_BILLTEXT };
	CTabCtrl m_TabStatus;
	CEdit m_editText1;
	CEdit m_editText2;
	CEdit m_editText3;
	CEdit m_editText4;
	CEdit m_editText5;
	CEdit m_editText6;
	CStatic m_staticText1;
	CStatic m_staticText2;
	CStatic m_staticText3;
	CStatic m_staticText4;
	CStatic m_staticText5;
	CStatic m_staticText6;
	CButton m_checkOverride;

public:
	bool GetUseParentFlag(){ return m_bUseParent; }
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnToggleOverride();
	DECLARE_MESSAGE_MAP()

private:
	void SelectTab();
	void SaveTabChanges();
		
private:
	CBillAndReceiptTexts& m_Text;
	CBillAndReceiptTexts m_ParentText;
	CLocationSelectorEntity& m_LocSelEntity;
	bool m_bUseParent;
	int m_nTabPage;
};

/**********************************************************************/
#endif
/**********************************************************************/
