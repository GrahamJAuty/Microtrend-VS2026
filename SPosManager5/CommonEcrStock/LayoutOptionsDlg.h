#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ColourButton.h"
/**********************************************************************/

class CLayoutOptionsDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CLayoutOptionsDlg)

public:
	CLayoutOptionsDlg(CWnd* pParent = NULL);
	virtual ~CLayoutOptionsDlg();

	enum { IDD = IDD_LAYOUT_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	CButton m_buttonShrink;
	CTabCtrl m_TabSettings;
	CColourButtonText m_buttonColourStatus;
	/*****/
	CButton m_checkEnable;
	CStatic m_staticWidth;
	CButton m_checkWidth;
	CStatic m_staticHeight;
	CStatic m_staticHeight2;
	CButton m_checkHeight;
	CStatic m_staticTaskbar;
	CStatic m_staticColourFill;
	CColourButtonBase m_buttonColourFill;
	CStatic m_staticImageAction;
	CButton m_buttonImage;
	CButton m_buttonClear;
	CStatic m_staticImageSize;
	CSSComboBox m_comboImageSize;
	CStatic m_staticXAlign;
	CSSComboBox m_comboXAlign;
	CStatic m_staticMarginLeft;
	CStatic m_staticMarginRight;
	CStatic m_staticYAlign;
	CSSComboBox m_comboYAlign;
	CStatic m_staticMarginTop;
	CStatic m_staticMarginBottom;
	/*****/
	CStatic m_staticButtonBarHome;
	CStatic m_staticButtonBarDirection;
	CStatic m_staticButtonBarAlign;
	CStatic m_staticButtonBarControlLine;
	CStatic m_staticButtonBarControlKey;
	CStatic m_staticButtonBarSize;
	CStatic m_staticButtonBarLines;
	CSSComboBox m_comboButtonBarHome;
	CSSComboBox m_comboButtonBarDirection;
	CSSComboBox m_comboButtonBarAlign;
	CSSComboBox m_comboButtonBarControlLine;
	CSSComboBox m_comboButtonBarControlKey;
	CButton m_checkButtonBarParent;
	CStatic m_staticButtonBarParent;
	CSSComboBox m_comboButtonBarSize;
	CSSComboBox m_comboButtonBarLines;
	/*****/
	CStatic m_staticButtonWidth;
	CSSTabbedComboBox m_comboButtonWidth;
	CStatic m_staticGapWidth;
	CStatic m_staticButtonHeight;
	CSSTabbedComboBox m_comboButtonHeight;
	CStatic m_staticGapHeight;
	CStatic m_staticButtonRound;
	CSSComboBox m_comboButtonRound;
	/*****/
	CStatic m_staticColourAction;
	CColourButtonText m_buttonColourAction;
	CButton m_buttonActionBack;
	CButton m_buttonActionText;
	CStatic m_staticColourList;
	CColourButtonText m_buttonColourList;
	CButton m_buttonListBack;
	CButton m_buttonListText;
	CStatic m_staticColourParent;
	CColourButtonText m_buttonColourParent;
	CButton m_buttonParentBack;
	CButton m_buttonParentText;
	CStatic m_staticColourControl;
	CColourButtonText m_buttonColourControl;
	CButton m_buttonControlBack;
	CButton m_buttonControlText;
	CStatic m_staticColourUnused;
	CColourButtonBase m_buttonColourUnused;
	CButton m_buttonUnusedBack;
	/*****/
	CButton m_buttonOK;
	CButton m_buttonCancel;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnButtonShrink();
	/*****/
	afx_msg void OnToggleEnable();
	afx_msg void OnToggleWidth();
	afx_msg void OnToggleUpperHeight();
	afx_msg void OnKillFocusWidth();
	afx_msg void OnKillFocusHeight();
	afx_msg void OnKillFocusTaskbar();
	afx_msg void OnButtonFillColour();
	afx_msg void OnButtonImage();
	afx_msg void OnButtonClear();
	afx_msg void OnSelectImageSize();
	afx_msg void OnSelectXAlign();
	afx_msg void OnKillFocusMarginLeft();
	afx_msg void OnKillFocusMarginRight();
	afx_msg void OnSelectYAlign();
	afx_msg void OnKillFocusMarginTop();
	afx_msg void OnKillFocusMarginBottom();
	/*****/
	afx_msg void OnSelectButtonBarHome();
	afx_msg void OnSelectButtonBarDirection();
	afx_msg void OnSelectButtonBarAlign();
	afx_msg void OnSelectButtonBarControlLine();
	afx_msg void OnSelectButtonBarControlKey();
	afx_msg void OnToggleButtonBarParent();
	afx_msg void OnSelectButtonBarSize();
	afx_msg void OnSelectButtonBarLines();
	afx_msg void OnSelectButtonWidth();
	afx_msg void OnKillFocusButtonWidthPercent();
	afx_msg void OnKillFocusButtonWidthPixel();
	afx_msg void OnKillFocusGapWidth();
	afx_msg void OnSelectButtonHeight();
	afx_msg void OnKillFocusButtonHeightPercent();
	afx_msg void OnKillFocusButtonHeightPixel();
	afx_msg void OnKillFocusGapHeight();
	afx_msg void OnSelectButtonRounding();
	/*****/
	afx_msg void OnButtonActionBack();
	afx_msg void OnButtonActionText();
	afx_msg void OnButtonListBack();
	afx_msg void OnButtonListText();
	afx_msg void OnButtonParentBack();
	afx_msg void OnButtonParentText();
	afx_msg void OnButtonControlBack();
	afx_msg void OnButtonControlText();
	afx_msg void OnButtonUnusedBack();
	/*****/
	afx_msg long OnColourButtonMessage ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

private:
	void SetEditBoxTextFromInt( CEdit* pEdit, int nNum );
	CEdit* GetEditWidth();
	CEdit* GetEditHeight();
	CEdit* GetEditTaskbar();
	CEdit* GetEditMarginLeft();
	CEdit* GetEditMarginRight();
	CEdit* GetEditMarginTop();
	CEdit* GetEditMarginBottom();
	CEdit* GetEditButtonWidthPercent();
	CEdit* GetEditButtonWidthPixel();
	CEdit* GetEditGapWidth();
	CEdit* GetEditButtonHeightPercent();
	CEdit* GetEditButtonHeightPixel();
	CEdit* GetEditGapHeight();
	
private:
	void SelectTab( int nPage );
	void UpdateParent();
	void UpdateErrorBar();

private:
	bool m_bShrunk;
	int m_nTabPage;
	
private:
	CWnd* m_pMainWnd;
	CString m_strImagePath;

private:
	int m_nColourMode;
};

/**********************************************************************/
#endif
/**********************************************************************/
