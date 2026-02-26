/**********************************************************************/
#ifndef __SORT_HPP__
#define __SORT_HPP__
/**********************************************************************/

#include <afxcmn.h>

/**********************************************************************/

class CSortStringArray : public CStringArray
{
public:
	CSortStringArray ( int nOffset=0, bool bAscending=TRUE, bool bIgnoreCase=FALSE );
	int Add ( const char* szText );

private:
	CString GetLabel ( const char* szRecord );
	bool Find ( const char* szLabel, int& nIndex );

private:
	int m_nOffset;
	bool m_bAscending;
	bool m_bIgnoreCase;
};

/**********************************************************************/

class CIndexSort
{
public:
	CIndexSort();
	bool Sort ( CStringArray* pTextArray, CString (CALLBACK EXPORT* lpGetLabel)(const char*)=NULL, int nOffset=0, bool bAscending=TRUE, bool bIgnoreCase=FALSE );
	int GetSize() { return m_IndexArray.GetSize(); }
	CString GetAt ( int nIndex );
	void RemoveAll() { m_IndexArray.RemoveAll(); }

protected:
	virtual CString GetLabel ( const char* szRecord );
	virtual bool Progress ( int nTextIndex ) { return FALSE; };

private:
	int Add ( int nTextIndex );
	bool Find ( const char* szLabel, int& nIndex );

private:
	CUIntArray m_IndexArray;
	CStringArray* m_pTextArray;
	CString (CALLBACK EXPORT* m_lpGetLabel)(const char*);
	int m_nOffset;
	bool m_bAscending;
	bool m_bIgnoreCase;
};

/**********************************************************************/

class CProgressIndexSort : public CIndexSort
{
public:
	CProgressIndexSort();
	bool Sort ( CProgressCtrl* pProgress, CStringArray* pTextArray, CString (CALLBACK EXPORT* lpGetLabel)(const char*)=NULL, int nOffset=0, bool bAscending=TRUE, bool bIgnoreCase=FALSE );
	void Stop();

private:
	bool Progress ( int nTextIndex );

private:
	CProgressCtrl* m_pProgress;
	bool m_bStopSort;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CConsolidateObject : public CObject
{
public:
	CConsolidateObject ( const char* szLabel ) : m_strLabel(szLabel), m_nCount(1) {};
	void Consolidate() { m_nCount++; }
	const char* GetLabel() { return m_strLabel; }
	int GetCount() { return m_nCount; }

private:
	CString m_strLabel;
	int m_nCount;
};

/**********************************************************************/

class CConsolidateArray : public CObArray
{
public:
	CConsolidateArray ( bool bAscending=TRUE, bool bIgnoreCase=FALSE );
	virtual ~CConsolidateArray();
	void RemoveAll();
	bool Add ( const char* szLabel );

	const char* GetLabel ( int nIndex ) { return ((CConsolidateObject*) (CObArray::GetAt(nIndex)))->GetLabel(); }
	int GetCount ( int nIndex ) { return ((CConsolidateObject*) (CObArray::GetAt(nIndex)))->GetCount(); }

protected:
	bool Find ( const char* szLabel, int& nIndex );
	bool Add ( int nIndex, CConsolidateObject* pObj );

private:
	bool m_bAscending;
	bool m_bIgnoreCase;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CQVConsolidateObject : public CConsolidateObject
{
public:
	CQVConsolidateObject ( const char* szLabel, double dQty, double dValue )
		: CConsolidateObject(szLabel), m_dQty(dQty), m_dValue(dValue) {};

	void Consolidate ( double dQty, double dValue )
	{
		m_dQty += dQty;
		m_dValue += dValue;
		CConsolidateObject::Consolidate();		// consolidate base class
	}

	double GetQty() { return m_dQty; }
	double GetValue() { return m_dValue; }

private:
	double m_dQty;
	double m_dValue;
};

/**********************************************************************/

class CQVConsolidateArray : public CConsolidateArray
{
public:
	CQVConsolidateArray ( bool bAscending=TRUE, bool bIgnoreCase=FALSE )
		: CConsolidateArray ( bAscending, bIgnoreCase ) {}

	bool Add ( const char* szLabel, double dQty, double dValue );
	bool AddIfExists ( const char* szLabel, double dQty, double dValue );

	double GetQty ( int nIndex ) { return ((CQVConsolidateObject*) (CObArray::GetAt(nIndex)))->GetQty(); }
	double GetValue ( int nIndex )  { return ((CQVConsolidateObject*) (CObArray::GetAt(nIndex)))->GetValue(); }
};

/**********************************************************************/

extern const char* szVERSION_SORT;

/**********************************************************************/
/*	SAMPLE CALLBACK FUNCTION									*/
/**********************************************************************/
/*
static CString CALLBACK EXPORT GetLabel ( const char* szRecord );
{
	CString strLabel;

	strLabel += szRecord[6];		// mm
	strLabel += szRecord[7];		// mm
	strLabel += szRecord[4];		// dd
	strLabel += szRecord[5];		// dd

	return strLabel;
}
*/
/**********************************************************************/
/*	SAMPLE DERRIVED CONSOLIDATE OBJECT							*/
/**********************************************************************/
/*
class CMyConsolidateObject : public CConsolidateObject
{
public:
	CMyConsolidateObject ( const char* szLabel, double dQty, double dValue )
		: CConsolidateObject(szLabel), m_dQty(dQty), m_dValue(dValue) {};

	void Consolidate ( double dQty, double dValue )
	{
		m_dQty += dQty;
		m_dValue += dValue;
		CConsolidateObject::Consolidate();		// consolidate base class
	}

	double GetQty() { return m_dQty; }
	double GetValue() { return m_dValue; }

private:
	double m_dQty;
	double m_dValue;
};
*/
/**********************************************************************/
/*	SAMPLE DERRIVED CONSOLIDATE ARRAY							*/
/**********************************************************************/
/*
class CMyConsolidateArray : public CConsolidateArray
{
public:
	CMyConsolidateArray ( bool bAscending=TRUE, bool bIgnoreCase=FALSE )
		: CConsolidateArray ( bAscending, bIgnoreCase ) {}

	bool Add ( const char* szLabel, double dQty, double dValue );

	double GetQty ( int nIndex ) { return ((CMyConsolidateObject*) (CObArray::GetAt(nIndex)))->GetQty(); }
	double GetValue ( int nIndex )  { return ((CMyConsolidateObject*) (CObArray::GetAt(nIndex)))->GetValue(); }
};

bool CMyConsolidateArray::Add ( const char* szLabel, double dQty, double dValue )
{
	int nIndex;

	if ( Find ( szLabel, nIndex ) == TRUE )	// label exists
	{
		((CMyConsolidateObject*) (CObArray::GetAt(nIndex)))->Consolidate ( dQty, dValue );

		return TRUE;
	}
	else								// label not found
	{
		CMyConsolidateObject* pObj = new CMyConsolidateObject ( szLabel, dQty, dValue );

		return CConsolidateArray::Add ( nIndex, pObj );
	}
}
*/
/**********************************************************************/
#endif	/* __SORT_HPP__ */
/**********************************************************************/

