#pragma once
//****************************************************************************

class CGTData  
{
public:
	CGTData();
	bool Read();
	bool Write();

	bool UpdateGTTotals ( long lValue );

	int GetCCNo();
	bool SetCCNo ( int nCCNo );
	int NextCCNo( bool bSaveReqd );

	bool SetGT ( double dValue );
	long GetGT();
	double GetGTValue();

	long GetCID();
	CString GetCIDValue();

	bool ClearCID();

	int GetQty2000()	{ return m_nQty2000; }
	int GetQty1000()	{ return m_nQty1000; }
	int GetQty500()		{ return m_nQty500; }
	int GetQty200()		{ return m_nQty200; }
	int GetQty100()		{ return m_nQty100; }
	int GetQty50()		{ return m_nQty50; }
	int GetQty20()		{ return m_nQty20; }
	int GetQty10()		{ return m_nQty10; }

	CString GetValue2000();
	CString GetValue1000();
	CString GetValue500();
	CString GetValue200();
	CString GetValue100();
	CString GetValue50();
	CString GetValue20();
	CString GetValue10();

private:
	CString FormatValue ( long lValue );
	bool ExportCID();

private:
	bool m_bReadReqd = TRUE;
	int m_nCCNo = 0;
	long m_lGT = 0;
	long m_lCID = 0;

private:
	int m_nQty2000 = 0;
	int m_nQty1000 = 0;
	int m_nQty500 = 0;
	int m_nQty200 = 0;
	int m_nQty100 = 0;
	int m_nQty50 = 0;
	int m_nQty20 = 0;
	int m_nQty10 = 0;
	int m_lQtyOther = 0;
};

//****************************************************************************
