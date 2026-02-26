#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ButtonInfo.h"
#include "Consolidation.h"
#include "ColourButton.h"
#include "DataAccessHelpersKeyboard.h"
#include "EposTaskDatabase.h"
#include "..\CommonEcrStock\PresetReportBase.h"
/**********************************************************************/
#define BUTTON_EDIT_NORMAL 0
#define BUTTON_EDIT_COPYALL 1
#define BUTTON_EDIT_COPYSTYLE 2
/**********************************************************************/

class CButtonEditDlg : public CSSDialog
{
public:
	CButtonEditDlg( CButtonInfo& infoDisplay, int nMode, CWnd* pParent = NULL);
	~CButtonEditDlg();

public:
	void SetStyleControlsFromButton( CButtonInfo& infoDisplay );
	void SetActionControlsFromButton( CButtonInfo& infoDisplay );
	void SetCopyStyleControls();
	void SetCopyAllControls();

private:
	void CreatePresetList( CPresetReportArray& arrayPreset, CArray<CSortedIntAndStringItem,CSortedIntAndStringItem>& listPresets );

public:
	//{{AFX_DATA(CButtonEditDlg)
	enum { IDD = IDD_BUTTON_EDIT };
	
	CStatic m_staticType;
	CButton m_radioUnused;
	CButton m_radioMenu;
	CButton m_radioAction;
	
	CStatic m_staticColour;
	CButton m_checkColour;
	CColourButtonText m_buttonColour;
	CButton m_buttonBack;
	CButton m_buttonText;
	CButton m_buttonImage;
	CButton m_buttonClear;
	CStatic m_staticFont;
	CSSComboBox m_comboFont;
	CButton m_checkTransparent;

	CStatic m_staticDetail;
	CStatic m_staticDetail1;
	CSSComboBox m_comboDetail1;
	CStatic m_staticDetail2;
	CSSComboBox m_comboDetail2;
	CSSComboBox m_comboDetail3;
	CButton m_checkPreset;
	CSSTabbedComboBox m_comboMenu;
	CSSComboBox m_comboUnused;
	CStatic m_staticAuto;
	CSSComboBox m_comboAuto;

	CStatic m_staticText;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CButtonEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	
private:
	int m_editLineID[3];
	int m_staticLineID[3];
	CEdit m_editLine[3];
	CStatic m_staticLine[3];
	CString m_strOriginalLine[3];

protected:
	//{{AFX_MSG(CButtonEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnRadioUnused();
	afx_msg void OnRadioMenu();
	afx_msg void OnRadioAction();
	afx_msg void OnToggleOwnColour();
	afx_msg void OnButtonBackColour();
	afx_msg void OnButtonTextColour();
	afx_msg void OnButtonImage();
	afx_msg void OnButtonClear();
	afx_msg void OnToggleTransparent();
	afx_msg void OnSelectMenu();
	afx_msg void OnSelectFont();
	afx_msg void OnSelectDetail1();
	afx_msg void OnSelectDetail2();
	afx_msg void OnSelectDetail3();
	afx_msg long OnColourButtonMessage ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

private:
	void FillDetail3ComboPreset( CArray<CSortedIntAndStringItem,CSortedIntAndStringItem>& listPresets );
	void FillDetail3ComboTask();
	void SelectButtonType( int nType );
	void SelectComboItem( CComboBox& combo, int& nTarget );
	void FillDetail2Combo();
	void FillDetail3Combo();
	void ShowOrHideDetail3();
	void AddDatabasesToDetail3( int nLocationType );
	void CreateButton( CButtonInfo& infoButton, bool bUpdateImage );
	void UpdateText();

public:
	void MergeSettings( CButtonInfo& LayoutButton );

public:
	CString m_strButtonFilename;
	int m_nButtonType;
	int m_nMenuNo;
	int m_nDetail1;
	int m_nDetail2;
	int m_nDetail3;

private:
	CButtonInfo& m_infoOriginal;
	int m_nOriginalButtonType;
	int m_nOriginalMenuNo;
	int m_nOriginalDetail1;
	int m_nOriginalDetail2;
	int m_nOriginalDetail3;

private:
	bool m_bEditTextColour;

private:
	CEposTaskDatabase m_TaskDatabase;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> m_listPresetsEpos;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> m_listPresetsHistoryPlu;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> m_listPresetsHistoryCustomer;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> m_listPresetsHistoryStock;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> m_listPresetsTimeAttend;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> m_listPresetsStockLevel;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> m_listPresetsStockAudit;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> m_listPresetsEcrmanStock;
	CDataAccessHelpersKeyboard m_DataAccessHelpersKeyboard;

private:
	CString m_strImageImportPath;
	CString m_strNewImageType;
	bool m_bGotOriginalImage;
	int m_nMode;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/
