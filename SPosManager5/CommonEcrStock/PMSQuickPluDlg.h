#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSDefines.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "PMSInvoiceArray.h"
#include "PMSInvoiceLineInfo.h"
#include "PMSRoomStatusInfo.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPMSQuickPluDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CPMSQuickPluDlg)

public:
	CPMSQuickPluDlg( int nRoomGridRoomIdx, int nRoomGridAccountIdx, bool bTraining, CWnd* pParent = NULL);   
	virtual ~CPMSQuickPluDlg();

	enum { IDD = IDD_PMS_QUICKPLU };

public:
	void GetPMSInvoiceSales( CPMSInvoiceArray& arraySales );

public:
	virtual BOOL OnInitDialog();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKillFocusQty1();
	afx_msg void OnKillFocusQty2();
	afx_msg void OnKillFocusQty3();
	afx_msg void OnKillFocusQty4();
	afx_msg void OnKillFocusQty5();
	afx_msg void OnKillFocusQty6();
	afx_msg void OnKillFocusQty7();
	afx_msg void OnKillFocusQty8();
	afx_msg void OnKillFocusQty9();
	afx_msg void OnKillFocusQty10();
	afx_msg void OnButtonBrowse1();
	afx_msg void OnButtonBrowse2();
	afx_msg void OnButtonBrowse3();
	afx_msg void OnButtonBrowse4();
	afx_msg void OnButtonBrowse5();
	afx_msg void OnButtonBrowse6();
	afx_msg void OnButtonBrowse7();
	afx_msg void OnButtonBrowse8();
	afx_msg void OnButtonBrowse9();
	afx_msg void OnButtonBrowse10();
	
public:
	int GetAction(){ return m_nAction; }

private:
	CEdit* GetEditQty( int n );
	CEdit* GetEditValue( int n );

private:
	void KillFocusQty( int n );
	void BrowsePlu( int n );
	
private:
	int m_nPluIdx[10];
	int m_nLineType[10];

private:
	int m_nStaticTypeID[2];
	int m_nStaticQtyID[2];
	int m_nStaticValueID[2];
	int m_nDescriptionID[10];
	int m_nQtyID[10];
	int m_nValueID[10];
	int m_nBrowseID[10];

private:
	CStatic m_StaticType[2];
	CStatic m_StaticQty[2];
	CStatic m_StaticValue[2];
	CEdit m_editDescription[10];
	CButton m_buttonBrowse[10];
	CButton m_buttonOK;
	CButton m_buttonCancel;
	CPluFilterArray m_PluFilterArray;

private:
	double m_dQty[10];
	double m_dValue[10];
	int m_nCharges;
	int m_nAllowances;
	bool m_bTraining;
	
private:
	int m_nAction;
	CPMSRoomStatusInfo m_infoRoom;
	CPMSRoomAccountInfo m_infoAccount;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/

