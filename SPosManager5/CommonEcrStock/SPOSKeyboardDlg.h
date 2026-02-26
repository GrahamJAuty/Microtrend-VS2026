#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SPOSKeyboardListManager.h"
#include "SPOSKeyboardTabbedComboBoxView.h"
#include "SPOSKeyboardView.h"
/**********************************************************************/

class CSPOSKeyboardDlg : public CDialog
{
public:
	CSPOSKeyboardDlg(CWnd* pParent = NULL);   
	virtual ~CSPOSKeyboardDlg();

	enum { IDD = IDD_SPOS_KEYBOARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	DECLARE_MESSAGE_MAP()
	void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	/*****/
	afx_msg void OnSaveKeyboard();
	afx_msg void OnImportWizardData();
	afx_msg void OnImportImages();
	afx_msg void OnExitKeyboard();
	afx_msg void OnZoomSmall();
	afx_msg void OnZoomNormal();
	afx_msg void OnZoomBig();
	afx_msg void OnEditKeyTypes();
	afx_msg void OnGlobalSettings();
	afx_msg void OnEditMacros();
	afx_msg void OnSetOptions();
	/*****/
	afx_msg void OnSelectViewMode();
	afx_msg void OnCheckInvisible();
	afx_msg void OnCheckImages();
	afx_msg void OnSelectList();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonPrintList();
	afx_msg void OnSelectPage();
	afx_msg void OnButtonAddPage();
	afx_msg void OnButtonEditPage();
	afx_msg void OnButtonDeletePage();
	afx_msg void OnButtonPluSort();
	virtual void OnOK();
	virtual void OnCancel();

public:
	bool SelectViewMode();
	bool SelectList();
	void SelectPage();

	void RefreshListCombo();
	void RefreshPageCombo();

	void SetListNo( __int64 nListNo );
	void SetPageNo( int nPageNo );

	void GetPreviousComboItem( CSPOSKeyboardListComboItem& item );
	void SetPreviousComboItem( CSPOSKeyboardListComboItem& item );

	int GetViewMode(){ return m_nViewMode; }
	void GetVisibleListDetails( int& nScreenNo, __int64& nListNo, int& nPageNo, int& nKeyCount );

	bool GetInvisibleTick(){ return m_checkInvisible.GetCheck() != 0; }
	bool GetImagesTick(){ return m_checkImages.GetCheck() != 0; }

private:
	bool SaveModified();

private:
	CSPOSKeyboardView* m_pView;
	bool m_bInitDialog;

private:
	HICON m_hIcon;
	CSSComboBox m_comboView;
	CSPOSKeyboardTabbedComboBoxView m_comboList;
	CSSComboBox m_comboPage;
	CButton m_checkInvisible;
	CButton m_checkImages;
	CButton m_buttonPluSort;

private:
	int m_nViewMode;
	CArray<CSPOSKeyboardListComboItem,CSPOSKeyboardListComboItem> m_arrayListComboItems;
	CArray<CSPOSKeyboardListComboItem,CSPOSKeyboardListComboItem> m_arrayPreviousComboItems;
};

/**********************************************************************/
#endif
/**********************************************************************/
