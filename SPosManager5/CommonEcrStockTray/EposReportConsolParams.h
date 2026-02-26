#pragma once
/**********************************************************************/

struct CEposReportConsolParamsPluSales
{
public:
	CEposReportConsolParamsPluSales();

public:
	void LoadSettings( CString& str );
	void SaveSettings( CString& str );

private:
	void Reset();

public:
	bool m_bConsolTerm;
	bool m_bConsolLoc;
	bool m_bConsolDb;
};

/**********************************************************************/

struct CEposReportConsolParamsStandard
{
public:
	CEposReportConsolParamsStandard();

public:
	void LoadSettings( CString& str );
	void SaveSettings( CString& str );

private:
	void Reset();

public:
	bool m_bConsolTerm;
	bool m_bConsolLoc;
	bool m_bConsolDb;
	bool m_bConsolSys;
};

/**********************************************************************/
