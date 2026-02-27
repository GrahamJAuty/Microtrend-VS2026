#pragma once
//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************

class CBackgroundTimetableDlg : public CSSAutoShutdownDialog
{
public:
	CBackgroundTimetableDlg(CWnd* pParent = NULL); 
	
	enum { IDD = IDD_BACKGROUNDTIMETABLE };
	CStatic m_staticWebPaymentMessage;
	CEdit m_editTime[11];
	CEdit m_editDate4;
	CEdit m_editWeekday6;
	CEdit m_editWeekday9;
	CEdit m_editWeekday10;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CString GetWeekdayString(int nWeekday);	
	void UpdateWeekdayBox(CString strTime, CEdit& editBox, int nDay);
};

//$$******************************************************************
