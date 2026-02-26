#pragma once
//**********************************************************************
#include "SQLFieldValidatorPluInfo.h"
//**********************************************************************

class CSQLRowPluInfo
{
public:
	CSQLRowPluInfo();
	
private:
	void Reset();
	
public:
	int GetDbNo() { return m_nDbNo; }
	__int64 GetPluNo() { return m_nPluNo; }
	CString GetDescription() { return m_strDescription; }
	int GetDeptNo() { return m_nDeptNo; }
	CString GetTaxCode() { return m_strTaxCode; }
	int GetPoints() { return m_nPoints; }
	int GetModType() { return m_nModType; }
	int GetStampOfferID() { return m_nStampOfferID; }

public:
	void SetDbNo(int n) { vdtr.SetDbNo(n, m_nDbNo); }
	void SetPluNo(__int64 n) { vdtr.SetPluNo(n, m_nPluNo); }
	void SetDescription(CString str) { vdtr.SetDescription(str, m_strDescription); }
	void SetDeptNo(int n) { vdtr.SetDeptNo(n, m_nDeptNo); }
	void SetTaxCode(CString str) { vdtr.SetTaxCode(str, m_strTaxCode); }
	void SetPoints(int n) { vdtr.SetPoints(n, m_nPoints); }
	void SetModType(int n) { vdtr.SetModType(n, m_nModType); }
	void SetStampOfferID( int n ) { vdtr.SetStampOfferID(n, m_nStampOfferID); }

public:
	bool IsBasePlu();
	CString GetPluNoString();
	CString GetMicrotrendBasePluNo(bool bPadout);
	static CString FormatPluNo(const char* szPluNo);

private:
	int m_nDbNo;
	__int64 m_nPluNo;
	CString m_strDescription;
	int m_nDeptNo;
	CString m_strTaxCode;
	int m_nPoints;
	int m_nModType;
	int m_nStampOfferID;

private:
	CSQLFieldValidatorPluInfo vdtr;
};

//**********************************************************************

