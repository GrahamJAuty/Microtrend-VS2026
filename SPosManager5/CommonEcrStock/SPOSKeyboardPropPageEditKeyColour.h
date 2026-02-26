#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SPOSKeyboardColourButton.h"
/**********************************************************************/

class CSPOSKeyboardPropPageEditKeyColour : public CSSPropertyPage
{
public:
	CSPOSKeyboardPropPageEditKeyColour();
	~CSPOSKeyboardPropPageEditKeyColour();
	void SetParentSheet( void* pParentSheet ){ m_pParentSheet = pParentSheet; }

	//{{AFX_DATA(CSPOSKeyboardPropPageEditKeyColour)
	enum { IDD = IDD_PROPPAGE_EDITKEY_COLOUR };
	CSSComboBox	m_comboVertical;
	CSSComboBox	m_comboHorizontal;
	CSSComboBox	m_comboFontSize;
	int		m_nForeground;
	int		m_nBackground;
	BOOL	m_bBold;
	//}}AFX_DATA
	CSPOSKeyboardColourButton	m_buttonColour[65];

	//{{AFX_VIRTUAL(CSPOSKeyboardPropPageEditKeyColour)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg long OnColourButtonClick ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnColourButtonDoubleClick ( WPARAM wIndex, LPARAM lParam );

public:
	void SetKeyRecord( CSPOSKeyboardKeyRecord* pRecord ) { m_pKeyRecord = pRecord; }
	void SetView( CSPOSKeyboardView* pView ) { m_pView = pView; }

protected:
	//{{AFX_MSG(CSPOSKeyboardPropPageEditKeyColour)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectFont();
	afx_msg void OnKillFocusBackground();
	afx_msg void OnKillFocusForeground();
	afx_msg void OnCheckBold();
	afx_msg void OnSelectAlign();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void RedrawKey();
	void EditColour( int nColourId );
	void ValidateKeySettings();
	void SaveRecord();

private:
	DWORD m_nButtonID[65];
	int m_nFontSize;
	CSPOSKeyboardKeyRecord* m_pKeyRecord;

private:
	CSPOSKeyboardView* m_pView;
	void* m_pParentSheet;
};

/**********************************************************************/

