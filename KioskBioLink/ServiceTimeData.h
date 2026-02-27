//*******************************************************************
//*** exactly same file used in KioskCardLink \ KioskBioLink ********
//*******************************************************************

#pragma once

#define nMAX_SERVICETIMES		5

typedef struct
{
	bool bReqd;
	int nStart;
	int nEnd;
	bool bBalanceOnly;
} ServiceTime;

//******************************************************************

class CServiceTimeData
{
public:
	CServiceTimeData(void);
	bool Read();
	bool Write();
	bool InService();
	bool IsBalanceOnly() { return Times[m_nCurrentIndex].bBalanceOnly; }

	void SetReqd ( int nIndex, bool bReqd ) { Times[nIndex].bReqd = bReqd; }
	bool GetReqd ( int nIndex ) { return Times[nIndex].bReqd; }

	void GetStartTime ( int nIndex, int& nHour, int &nMins );
	void SetStartTime ( int nIndex, int nHour, int nMins );

	void GetEndTime ( int nIndex, int& nHour, int &nMins );
	void SetEndTime ( int nIndex, int nHour, int nMins );

	void SetBalanceOnly ( int nIndex, bool bBalance ) { Times[nIndex].bBalanceOnly = bBalance; }
	bool GetBalanceOnly ( int nIndex ) { return Times[nIndex].bBalanceOnly; }

private:
	bool CheckPeriod ( int nIndex, int nTimeNow );
	int InService ( int nHour, int nMins );

private:
	ServiceTime Times[nMAX_SERVICETIMES];
	int m_nCurrentIndex;
};
