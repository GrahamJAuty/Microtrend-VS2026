#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "LabelNameHandler.h"
/**********************************************************************/

class CPropPageOptionsLabels2 : public CPropertyPage
{
public:
	CPropPageOptionsLabels2();
	~CPropPageOptionsLabels2();

	//{{AFX_DATA(CPropPageOptionsLabels2)
	enum { IDD = IDD_PROPPAGE_OPTIONS_LABELS2 };
	
	CSSComboBox	m_comboShelfEdgeName;
	
	CButton m_checkShelfEdgeSingle;
	CSSComboBox	m_comboShelfEdgeNameSingle;
	CButton m_checkShelfEdgeSinglePrompt;
	
	CButton	m_checkShelfEdge;
	CButton	m_checkShelfEdgePrompt;

	CSSComboBox	m_comboProductName;
	
	CButton m_checkProductSingle;
	CSSComboBox	m_comboProductNameSingle;
	CButton m_checkProductSinglePrompt;
	
	CButton	m_checkProduct;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageOptionsLabels2)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageOptionsLabels2)
	virtual BOOL OnInitDialog();
	afx_msg void OnToggleAutoShelf();
	afx_msg void OnToggleSingle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetRecordControls();

private:
	void SaveRecord();
	void GetRecordData();

private:
	CLabelNameHandler m_LabelNameHandler;
};

/**********************************************************************/
#endif
/**********************************************************************/
