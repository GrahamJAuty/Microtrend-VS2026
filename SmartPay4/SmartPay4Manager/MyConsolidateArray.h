#pragma once

//**********************************************************************

class CMyConsolidateObject : public CConsolidateObject
{
public:
	CMyConsolidateObject ( const char* szLabel, double dValue1, double dValue2, double dValue3 )
		: CConsolidateObject(szLabel), m_dValue1(dValue1), m_dValue2(dValue2), m_dValue3(dValue3) {};

	void Consolidate ( double dValue1, double dValue2, double dValue3 );

	double GetValue1() { return m_dValue1; }
	double GetValue2() { return m_dValue2; }
	double GetValue3() { return m_dValue3; }

private:
	double m_dValue1;
	double m_dValue2;
	double m_dValue3;
};

//**********************************************************************

class CMyConsolidateArray : public CConsolidateArray
{
public:
	CMyConsolidateArray ( bool bAscending=TRUE, bool bIgnoreCase=FALSE )
		: CConsolidateArray ( bAscending, bIgnoreCase ) {}

	bool Add ( const char* szLabel, double dValue1, double dValue2, double dValue3 );

	double GetValue1 ( int nIndex )	 { return ((CMyConsolidateObject*) (CObArray::GetAt(nIndex)))->GetValue1(); }
	double GetValue2 ( int nIndex )  { return ((CMyConsolidateObject*) (CObArray::GetAt(nIndex)))->GetValue2(); }
	double GetValue3 ( int nIndex )  { return ((CMyConsolidateObject*) (CObArray::GetAt(nIndex)))->GetValue3(); }
};

//**********************************************************************
