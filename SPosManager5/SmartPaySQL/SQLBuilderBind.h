#pragma once
//**********************************************************************

struct CSQLBuilderBindField
{
public:
	CSQLBuilderBindField();
	CString GetFieldAsString();

public:
	int m_nFieldType;
	CString m_strFieldValue;
	int m_nFieldValue32;
	__int64 m_nFieldValue64;
	double m_dFieldValue;

};

//**********************************************************************

class CSQLBuilderBind
{
public:
	CSQLBuilderBind();
	
public:
	void Reset() { m_arrayFields.RemoveAll(); }
	void AddString(CString strFieldValue);
	void AddDouble(double dFieldValue);
	void AddInt32(int nFieldValue);
	void AddInt64(__int64 nFieldValue);
	
public:
	int GetFieldCount() { return m_arrayFields.GetSize(); }
	void GetField(int n, CSQLBuilderBindField& field) { field = m_arrayFields.GetAt(n); }

private:
	CArray<CSQLBuilderBindField, CSQLBuilderBindField> m_arrayFields;
};

//**********************************************************************

