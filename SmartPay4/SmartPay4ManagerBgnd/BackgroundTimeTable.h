#pragma once
//******************************************************************
#define BTT_MAXTIMES 10
//******************************************************************

class CBackgroundTimeTable
{
public:
	CBackgroundTimeTable();

//******************************************************************
#ifndef SYSTEMTYPE_BACKGROUND
//******************************************************************

public:
	void Show(CWnd* pParent = NULL);

//******************************************************************
#endif
//******************************************************************

public:
	bool ReadTimetable();
	bool WriteTimetable();

	void SetTime(int n, const char* szTime);
	CString GetTime(int n);

	void SetPurchaseDay(int nDay) { m_nPurchaseDay = nDay; }
	int  GetPurchaseDay() { return m_nPurchaseDay; }

	void SetWeeklyPointsDay(int nDay) { m_nWeeklyPointsDay = nDay; }
	int  GetWeeklyPointsDay() { return m_nWeeklyPointsDay; }

	void SetAccountLinkDay(int nDay) { m_nAccountLinkDay = nDay; }
	int  GetAccountLinkDay() { return m_nAccountLinkDay; }

private:
	CString m_strFilename = "";
	CString m_strTime[BTT_MAXTIMES + 1] = { "" };
	CString m_strTimeLabel[BTT_MAXTIMES + 1] = { "" };
	// 0=sun=1=mon etc
	int m_nPurchaseDay = 0;					
	int m_nWeeklyPointsDay = 0;
	int m_nAccountLinkDay = 0;
};

//******************************************************************
