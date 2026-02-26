#pragma once
//**********************************************************************
#include "SQLFieldValidatorGroupTopupBonus.h"
//**********************************************************************

class CSQLRowGroupTopupBonus
{
public:
	CSQLRowGroupTopupBonus();
	
public:
	void Reset();
	
public:
	int GetGroupNo() { return m_nGroupNo; }
	int GetPurseNo() { return m_nPurseNo; }
	int GetTopupNo() { return m_nTopupNo; }
	/*****/
	/*****/
	bool GetBonusFlag() { return m_bBonusFlag; }
	bool GetBonusDisable() { return m_bBonusDisable; }
	int GetBonusType() { return m_nBonusType; }
	double GetBonusTrip() { return m_nBonusTrip; }
	int GetBonusPoints() { return m_nBonusPoints; }	
	double GetBonusValue() { return m_nBonusValue; }
	int GetBonusPercent() { return m_nBonusPercent; }

public:
	void SetGroupNo(int n) { vdtr.SetGroupNo(n, m_nGroupNo); }
	void SetPurseNo(int n) { vdtr.SetPurseNo(n, m_nPurseNo); }
	void SetTopupNo(int n) { vdtr.SetTopupNo(n, m_nTopupNo); }
	/*****/
	/*****/
	void SetBonusFlag(bool b) { m_bBonusFlag = b; }
	void SetBonusDisable(bool b) { m_bBonusDisable = b; }
	void SetBonusType(int n) { vdtr.SetBonusType(n, m_nBonusType); }
	void SetBonusTrip(double d) { vdtr.SetBonusTrip(d, m_nBonusTrip); }
	void SetBonusPoints(int n) { vdtr.SetBonusPoints(n, m_nBonusPoints); }
	void SetBonusValue(double d) { vdtr.SetBonusValue(d, m_nBonusValue); }
	void SetBonusPercent(int n) { vdtr.SetBonusPercent(n, m_nBonusPercent); }

public:
	void Add(CSQLRowGroupTopupBonus& source);
	int Compare(CSQLRowGroupTopupBonus& source, int nHint);

private:
	int m_nGroupNo;
	int m_nPurseNo;
	int m_nTopupNo;
	/*****/
	/*****/
	bool m_bBonusFlag;
	bool m_bBonusDisable;
	int m_nBonusType;
	double m_nBonusTrip;
	int m_nBonusPoints;
	double m_nBonusValue;
	int m_nBonusPercent;

private:
	CSQLFieldValidatorGroupTopupBonus vdtr;
};

//**********************************************************************

