#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ColourButton.h"
/**********************************************************************/

class CPropPagePMSOptionsColour : public CSSPropertyPage
{
public:
	CPropPagePMSOptionsColour();
	~CPropPagePMSOptionsColour();

	//{{AFX_DATA(CPropPagePMSOptionsColour)
	enum { IDD = IDD_PROPPAGE_PMS_OPTIONS_COLOUR };
	//}}AFX_DATA
	CColourButtonText m_buttonColourVacant;
	CColourButtonText m_buttonColourPending;
	CColourButtonText m_buttonColourActive;
	CColourButtonText m_buttonColourComplete;
	CColourButtonText m_buttonColourClosed;
	CColourButtonText m_buttonColourSelected;
	CColourButtonText m_buttonColourHighlighted;
	CColourButtonText m_buttonColourUnhighlighted;
	
public:
	//{{AFX_VIRTUAL(CPropPagePMSOptionsColour)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnButtonVacantBack();
	afx_msg void OnButtonVacantText();
	afx_msg void OnButtonPendingBack();
	afx_msg void OnButtonPendingText();
	afx_msg void OnButtonActiveBack();
	afx_msg void OnButtonActiveText();
	afx_msg void OnButtonCompleteBack();
	afx_msg void OnButtonCompleteText();
	afx_msg void OnButtonClosedBack();
	afx_msg void OnButtonClosedText();
	afx_msg void OnButtonSelectedBack();
	afx_msg void OnButtonSelectedText();
	afx_msg void OnButtonHighlightedBack();
	afx_msg void OnButtonHighlightedText();
	afx_msg void OnButtonUnhighlightedBack();
	afx_msg void OnButtonUnhighlightedText();
	afx_msg void OnButtonDefault();
	afx_msg long OnColourButtonMessage ( WPARAM wIndex, LPARAM lParam );

private:
	void ChangeButtonColour( int& nColour, const char* szText );
	void InitialiseColourButton( CColourButtonText& button, int nNo, const char* szText );
	void UpdateButtonColours();

protected:
	//{{AFX_MSG(CPropPagePMSOptionsColour)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();

private:
	int m_nColourMode;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
