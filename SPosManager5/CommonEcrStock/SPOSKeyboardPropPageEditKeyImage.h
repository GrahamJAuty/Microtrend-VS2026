#pragma once
/**********************************************************************/
#include "PictCtrl.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardPropPageEditKeyImage : public CSSPropertyPage
{
public:
	CSPOSKeyboardPropPageEditKeyImage();
	~CSPOSKeyboardPropPageEditKeyImage();
	void SetParentSheet( void* pParentSheet ){ m_pParentSheet = pParentSheet; }

	//{{AFX_DATA(CSPOSKeyboardPropPageEditKeyImage)
	enum { IDD = IDD_PROPPAGE_EDITKEY_IMAGE };
	CEdit	m_editWidth;
	CEdit	m_editHeight;
	CButton m_buttonPlu;
	CButton m_buttonDept;
	CButton m_buttonRemove;
	CEdit m_editImage;
	CSSComboBox	m_comboFill;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardPropPageEditKeyImage)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
public:
	void SetKeyRecord( CSPOSKeyboardKeyRecord* pRecord );
	
protected:
	//{{AFX_MSG(CSPOSKeyboardPropPageEditKeyImage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonImport();
	afx_msg void OnButtonPlu();
	afx_msg void OnButtonDept();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();

private:
	void RedrawKey();
	void RedrawImage();
	void FillScaleCombo( int nScale );

private:
	CSPOSKeyboardKeyRecord* m_pKeyRecord;

private:
	CPictureCtrl m_staticImage;
	void* m_pParentSheet;
};

/**********************************************************************/

