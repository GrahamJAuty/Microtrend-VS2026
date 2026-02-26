#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PropPagePMSOptionsRooms.h"
#include "afxwin.h"
/**********************************************************************/

class CDefEditRoomDlg : public CSSDialog
{
public:
	CDefEditRoomDlg( int nIndex, bool bAdd, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditRoomDlg)
	enum { IDD = IDD_DEF_EDIT_ROOM };
	CEdit	m_editName;
	CSSComboBox m_comboGroup;
	CSSComboBox m_comboType;
	CButton m_buttonSpin;
	CButton m_checkSingle;
	CString	m_strName;
	CStatic m_staticRate[3];
	CButton m_checkExtraBed;
	CButton m_checkExtraCot;
	int m_nSleeps;
	double m_dStandardRate[3];
	double m_dSingleRate[3];
	double m_dExtraBed;
	double m_dExtraCot;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditRoomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditRoomDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	afx_msg void OnSelectType();
	afx_msg void OnKillFocusSleeps();
	afx_msg void OnButtonSet();
	afx_msg void OnButtonGroup();
	afx_msg void OnToggleRates();
	afx_msg void OnToggleExtraBed();
	afx_msg void OnToggleExtraCot();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int GetIndex() { return m_nIndex; }
	void SetPropPage( CPropPagePMSOptionsRooms* pDlg ) { m_pPropPage = pDlg; }
	bool ValidateRoomInfo();

private:
	void GetData();
	void SaveData();
	void SpinPrevious();
	void SpinNext();

	CEdit* GetEditSleeps();
	CEdit* GetEditStandardRate( int n );
	CEdit* GetEditSingleRate( int n );
	CEdit* GetEditExtraBed();
	CEdit* GetEditExtraCot();

	void FillRoomTypeCombo( int nRoomSleeps, int nRoomType );
	void FillRoomGroupCombo( int nSel );

private:
	CPropPagePMSOptionsRooms* m_pPropPage;

private:
	int m_nIndex;
	bool m_bAdd;
	int m_nSize;
	int m_nSleepsOld;

private:
	CArray<int,int> m_arrayRateTypes;
	WORD m_IDStaticRate[3];
	WORD m_IDStandardRate[3];
	WORD m_IDSingleRate[3];
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
