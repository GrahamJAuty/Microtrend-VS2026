#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPMSPrintRoomsDlg : public CSSDialog
{
public:
	CPMSPrintRoomsDlg( CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CPMSPrintRoomsDlg)
	enum { IDD = IDD_PMS_PRINT_ROOMS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPMSPrintRoomsDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CButton m_checkOccupied1;
	CButton m_checkOccupied2;
	CButton m_checkOccupied3;
	CButton m_checkVacant1;
	CButton m_checkVacant2;
	CButton m_checkVacant3;
	CButton m_checkTraining;
	CButton m_checkBalance;
	CButton m_checkLines;

protected:
	//{{AFX_MSG(CPMSPrintRoomsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	virtual void OnOK();
	afx_msg void OnButtonDefault();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	const char* GetCSVLine();

private:
	CString m_strFieldSelect;
};

/**********************************************************************/

class CCSVPMSPrintRoomKey : public CCSV
{
public:
	CCSVPMSPrintRoomKey ( const char* szCSVLine ) : CCSV(szCSVLine){}
	bool GetFilterOccupied1(){ return GetBool(0); }
	bool GetFilterOccupied2(){ return GetBool(1); }
	bool GetFilterOccupied3(){ return GetBool(2); }
	bool GetFilterVacant1(){ return GetBool(3); }
	bool GetFilterVacant2(){ return GetBool(4); }
	bool GetFilterVacant3(){ return GetBool(5); }
	bool GetBalanceFlag() { return GetBool(6); }
	bool GetLinesFlag() { return GetBool(7); }
	bool GetTrainingFlag(){ return GetBool(8); }
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
