#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PMSEmailSignatureBuffer.h"
/**********************************************************************/

class CPropPagePMSOptionsMisc : public CSSPropertyPage
{
public:
	CPropPagePMSOptionsMisc();
	~CPropPagePMSOptionsMisc();

	//{{AFX_DATA(CPropPagePMSOptionsMisc)
	enum { IDD = IDD_PROPPAGE_PMS_OPTIONS_MISC };
	//}}AFX_DATA
	CButton m_checkAccounts;
	CButton m_checkEmail;
	CButton m_checkEmailNote;
	CButton m_checkTraining;
	CButton m_checkRateRoom;
	CButton m_checkRateBB;
	CButton m_checkRateDBB;
	CButton m_checkRateFree;
	CStatic m_staticStockpoint;
	CSSComboBox m_comboRate;
	CSSComboBoxEx m_comboStockpoint;
	CSSComboBox m_comboGroup;
	CEdit m_editSignature;
	
public:
	//{{AFX_VIRTUAL(CPropPagePMSOptionsMisc)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void OnSelectGroup();
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPagePMSOptionsMisc)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetEmailSignatures( CPMSEmailSignatures* p ){ m_pEmailSignatures = p; }

private:
	void FillRoomGroupCombo( int nSel );
	void AddStockpointToCombo ( const char* szText, int nImage );

private:
	void SaveRecord();

private:
	int m_nCurrentGroup;
	CPMSEmailSignatures* m_pEmailSignatures;
	CImageList m_ImageList;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
