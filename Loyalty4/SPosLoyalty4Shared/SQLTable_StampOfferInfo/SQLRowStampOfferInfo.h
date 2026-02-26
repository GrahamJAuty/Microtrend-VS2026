#pragma once
//**********************************************************************
#include "SQLFieldValidatorStampOfferInfo.h"
//**********************************************************************

class CSQLRowStampOfferInfo
{
public:
	CSQLRowStampOfferInfo();
	
private:
	void Reset();
	
public:
	int GetStampOfferID() { return m_nStampOfferID; }
	int GetEdition() { return m_nEdition; }
	bool GetEnableFlag() { return m_bEnabled; }
	int GetThreshold() { return m_nThreshold; }
	int GetExpiry() { return m_nExpiry; }
	CString GetDescription() { return m_strDescription; }
	int GetPluCount() { return m_nPluCount; }
	
public:
	void SetStampOfferID(int n) { vdtr.SetStampOfferID(n, m_nStampOfferID); }
	void SetEdition(int n) { vdtr.SetEdition(n, m_nEdition); }
	void SetEnableFlag(bool b ) { m_bEnabled = b; }
	void SetThreshold(int n) { vdtr.SetThreshold(n, m_nThreshold); }
	void SetExpiry(int n) { vdtr.SetExpiry(n, m_nExpiry); }
	void SetDescription(CString str) { vdtr.SetDescription(str, m_strDescription); }
	void SetPluCount( int n ) { m_nPluCount = n; }

public:
	CString GetDisplayDescription();
	
private:
	int m_nStampOfferID;
	int m_nEdition;
	bool m_bEnabled;
	int m_nThreshold;
	int m_nExpiry;
	CString m_strDescription;
	int m_nPluCount;

private:
	CSQLFieldValidatorStampOfferInfo vdtr;
};

//**********************************************************************

