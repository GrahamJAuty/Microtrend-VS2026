#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include <vector>
//**********************************************************************
#include "SQLBuilderBind.h"	
#include "SQLDefines.h"
//**********************************************************************
#define MAX_GENERAL_SQLPARAMS 20
//**********************************************************************

struct CRecordSetStringField
{
public:
	CRecordSetStringField() :
		m_pField(nullptr),
		m_strLabel(""),
		m_bCheckExists(false)
	{
	}

	CRecordSetStringField(CString* pField, CString strLabel, bool bCheckExists) :
		m_pField(pField),
		m_strLabel(strLabel),
		m_bCheckExists(bCheckExists)
	{
	}

	CString* m_pField;
	CString m_strLabel;
	bool m_bCheckExists;
};

//**********************************************************************

struct CRecordSetIntField
{
public:
	CRecordSetIntField() :
		m_pField(nullptr),
		m_strLabel(""),
		m_bCheckExists(false)
	{
	}

	CRecordSetIntField(int* pField, CString strLabel, bool bCheckExists) :
		m_pField(pField),
		m_strLabel(strLabel),
		m_bCheckExists(bCheckExists)
	{
	}

	int* m_pField;
	CString m_strLabel;
	bool m_bCheckExists;
};

//**********************************************************************

struct CRecordSetInt64Field
{
public:
	CRecordSetInt64Field() :
		m_pField(nullptr),
		m_strLabel(""),
		m_bCheckExists(false)
	{
	}

	CRecordSetInt64Field(__int64* pField, CString strLabel, bool bCheckExists) :
		m_pField(pField),
		m_strLabel(strLabel),
		m_bCheckExists(bCheckExists)
	{
	}

	__int64* m_pField;
	CString m_strLabel;
	bool m_bCheckExists;
};

//**********************************************************************

struct CRecordSetBOOLField
{
public:
	CRecordSetBOOLField() :
		m_pField(nullptr),
		m_strLabel(""),
		m_bCheckExists(false)
	{
	}

	CRecordSetBOOLField(BOOL* pField, CString strLabel, bool bCheckExists) :
		m_pField(pField),
		m_strLabel(strLabel),
		m_bCheckExists(bCheckExists)
	{
	}

	BOOL* m_pField;
	CString m_strLabel;
	bool m_bCheckExists;
};

//**********************************************************************

class CSQLRecordSetMetadata : public CRecordset
{
public:
	CSQLRecordSetMetadata();

public:
	CString GetBasicFilterString();
	CString GetByRangeFilterString();
	void BindGeneralParams(CSQLBuilderBind* pBind);

public:
    const std::vector<CRecordSetStringField>& GetTextFields() const { return m_vTextFields; }
    const std::vector<CRecordSetIntField>& GetIntFields() const { return m_vIntFields; }
    const std::vector<CRecordSetInt64Field>& GetInt64Fields() const { return m_vInt64Fields; }
    const std::vector<CRecordSetStringField>& GetDecimalFields() const { return m_vDecimalFields; }
    const std::vector<CRecordSetBOOLField>& GetBOOLFields() const { return m_vBOOLFields; }

public:
	const std::vector<int>& GetIntParamTypes() const { return m_vIntParamTypes; }
	const std::vector<CRecordSetStringField>& GetTextParams() const { return m_vTextParams; }
	const std::vector<CRecordSetIntField>& GetInt32Params() const { return m_vInt32Params; }

protected:
	void AddTextField(CString* pField, const CString strLabel, const CString strDef = "", const bool bCheckExists = FALSE);
	void AddIntField(int* pField, const CString strLabel, const int nDef = 0, const bool bCheckExists = FALSE);
	void AddInt64Field(__int64* pField, const CString strLabel, const __int64 nDef = 0, const bool bCheckExists = FALSE);
	void AddDecimalField(CString* pField, const CString strLabel, const CString strDef = "0.00", const bool bCheckExists = FALSE);
	void AddBOOLField(BOOL* pField, const CString strLabel, const BOOL bDef = FALSE, const bool bCheckExists = FALSE);

protected:
	void AddTextParam(CString strLabel, const CString strDef = "");
	void AddIntParam(CString strLabel, const int nDef = 0);
	
public:
	int GetFieldCount() const { return m_nFields; }
	int GetParamCount() const { return m_nParams; }

public:
	void CheckDecimalFields();
	void CheckValidDecimalString(CString& strField, int nDPs = 2);

protected:
	CString m_strParamValue[MAX_GENERAL_SQLPARAMS];
	int m_nParamValue[MAX_GENERAL_SQLPARAMS];

private:
	int m_nFields;
	int m_nParams;

private:
	std::vector<CRecordSetStringField> m_vTextFields;
	std::vector<CRecordSetStringField> m_vDecimalFields;
	std::vector<CRecordSetIntField> m_vIntFields;
	std::vector<CRecordSetInt64Field> m_vInt64Fields;
	std::vector<CRecordSetBOOLField> m_vBOOLFields;
	/****/
	std::vector<int> m_vIntParamTypes;
	std::vector<CRecordSetStringField> m_vTextParams;
	std::vector<CRecordSetIntField> m_vInt32Params;

private:
	CString m_strBasicFilterString;
	CString m_strByRangeFilterString;
};

//**********************************************************************

