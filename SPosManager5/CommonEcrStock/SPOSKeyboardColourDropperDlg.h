#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "PictCtrl.h"
#include "SPOSKeyboardAnalysisCategorySelector.h"
#include "SPOSKeyboardColourButton.h"
/**********************************************************************/

class CSPOSKeyboardColourDropperDlg : public CSSDialog
{
public:
	CSPOSKeyboardColourDropperDlg( int nMode, CWnd* pParent = NULL);  

	//{{AFX_DATA(CSPOSKeyboardColourDropperDlg)
	enum { IDD = IDD_SPOS_KEYBOARD_COLOUR_DROPPER };
	CTabCtrl		m_TabSettings;
	CSPOSKeyboardColourButton	m_buttonColour[65];
	CStatic			m_staticColour;
	CSSComboBox		m_comboColour;
	CStatic			m_staticFontSize;
	CSSComboBox		m_comboFontSize;
	CStatic			m_staticVertical;
	CSSComboBox		m_comboVertical;
	CStatic			m_staticHorizontal;
	CSSComboBox		m_comboHorizontal;
	CStatic			m_staticBold;
	CSSComboBox		m_comboBold;
	CStatic			m_staticDefText;
	CSSComboBox		m_comboDefText;
	CStatic			m_staticDepartment;
	CEdit			m_editDeptNo;
	CSSComboBox		m_comboDeptName;
	CStatic			m_staticCategory;
	CEdit			m_editCatNo;
	CSSComboBox		m_comboCatName;
	CSSListCtrl		m_listKeyInfo;
	CButton			m_checkCopyImage;
	CEdit			m_editImage;
	CPictureCtrl	m_staticImage;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardColourDropperDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CSPOSKeyboardColourDropperDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelectFontSize();
	afx_msg void OnKeyTypeControl();
	afx_msg void OnKeyTypeEmpty();
	afx_msg void OnKeyTypeFunction();
	afx_msg void OnKeyTypeList();
	afx_msg void OnKeyTypeMacro();
	afx_msg void OnKeyTypeModifier();
	afx_msg void OnKeyTypePayment();
	afx_msg void OnKeyTypePluNo();
	afx_msg void OnKeyTypeSvrPreset();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelectValue(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFilter();
	afx_msg void OnSelectColourMode();
	afx_msg void OnSelectHorizontal();
	afx_msg void OnSelectVertical();
	afx_msg void OnSelectBold();
	afx_msg void OnSelectDefTextAlways();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnSelectDepartment();
	afx_msg void OnKillFocusCatNo();
	afx_msg void OnSelectCategory();
	//}}AFX_MSG
	afx_msg long OnColourButtonClick ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnColourButtonDoubleClick ( WPARAM wIndex, LPARAM lParam );
	afx_msg void OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI );
	virtual void OnCancel();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

public:
	void SelectTab();
	int GetMode() { return m_nMode; }
	void SetColoursToDropAsShown();
	void SetColoursToNoChange();
	void SetColoursAndTextToDefault();
	void SetKeyToCopy( CSPOSKeyboardKeyRecord& Key );
	void UpdateKeyRecord( CSPOSKeyboardKeyRecord& KeyRecord );
	void SelectNext();
	bool CanDropKeyNow();

public:
	void SetKeyType( int n );
	void SetKeyValue( __int64 n );
	void SetForeground( int n );
	void SetBackground( int n );
	void SetFontSize( int n );
	void SetHAlign( int n );
	void SetVAlign( int n );
	void SetBold( int n );
	void SelectImageTab();
	void SetImageFlag( bool b );
	void SetImageFilename( const char* sz ){ m_strImageFilename = sz; }
	void SetImageScale( int n ){ m_nImageScale = n; }
	void UpdateImage();
	void SetDefTextAlways( int n );

public:
	void SetCopyImageTitle();
	void UpdateTitle();

private:
	void InitDialogListKey();
	void InitDialogCopyKey();
	void InitDialogCopyColour();
	void UpdateOptionalMenus();
	void SelectKeyTypeFromMenu( int nType );
	bool GetDialogBorderPixels( int& w, int& h );
	void EditDefaultKeySettings( CSPOSKeyboardDefaultTextArray* pTextArray, int nSel );
	void HandleDoubleClickList();

private:
	void UpdatePreviewButton();
	void EditColour( int nColourId );
	void CreateList();
	int GetSafeListSize();

private:
	void FillFontCombo();
	void FillColourCombo();
	void FillHorizontalCombo();
	void FillVerticalCombo();
	void FillBoldCombo();
	void FillDefTextAlwaysCombo();

public:
	int m_nMode;
	int m_nKeyType;
	__int64 m_nKeyValue;
	int m_nForeground;
	int m_nBackground;
	int m_nFontSize;
	int m_nHAlign;
	int m_nVAlign;
	int m_nBold;
	int m_nDefTextAlways;
	bool m_bDeptAndCategory;

	CString m_strCopyText;
	
	CString m_strImageFilename;
	int m_nImageScale;

private:
	HICON m_hIcon;
	DWORD m_nButtonID[65];
	CMenuChanger m_MenuChanger;
	bool m_bInitDialog;
	int m_nActiveTab;
	
private:
	int m_nDialogMinPixelSizeX;
	int m_nDialogMinPixelSizeY;
	int m_nDialogMaxPixelSizeX;
	int m_nDialogMaxPixelSizeY;
	int m_nListPosPixelX;
	int m_nListPosPixelY;
	int m_nListPixelSizeX;
	int m_nListPixelBorderY;
	
private:
	CDepartmentSelector m_DepartmentSelector;
	CSPOSKeyboardAnalysisCategorySelector m_CategorySelector;

private:
	CArray<__int64,__int64> m_arrayKeyValues;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
