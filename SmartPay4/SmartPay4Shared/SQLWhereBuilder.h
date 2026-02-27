#pragma once
//**********************************************************************
static const char* szLESSTHAN = "Less than";
static const char* szLESSTHANOREQUAL = "LT=";
static const char* szGREATER = "Greater than";
static const char* szGREATEROREQUAL = "GT=";
static const char* szEQUAL = "Equal to";
static const char* szNOTEQUAL = "Not equal to";
static const char* szLIKE = "Like";
/**********************************************************************/
#define SQL_WHERE_NONE 0
#define SQL_WHERE_OPENBRACKET 1
#define SQL_WHERE_CLOSEBRACKET 2
#define SQL_WHERE_AND 3
#define SQL_WHERE_OR 4
#define SQL_WHERE_CONDITION 5
/**********************************************************************/
#include "SQLBuilderBind.h"
/**********************************************************************/

struct CSQLWhereToken
{
public:
	int m_nType;
	CString m_strCondition;

public:
	CSQLWhereToken() { m_nType = 0; m_strCondition = ""; }
};

/**********************************************************************/

class CSQLWhereBuilder
{
public:
	CSQLWhereBuilder();
	CString GetWhereString();

public:
	void AppendOpenBracket();
	void AppendCloseBracket();
	void AppendAnd();
	void AppendOr();
	void AppendConditionDirect(CString strCondition);
	void AppendConditionString(CSQLBuilderBind& BuilderBind, CString strField, CString strCondition, CString strValue);
	void AppendConditionDouble(CSQLBuilderBind& BuilderBind, CString strField, CString strCondition, double dValue);
	void AppendConditionInt(CSQLBuilderBind& BuilderBind, CString strField, CString strCondition, int nValue);
	void AppendConditionInt64(CSQLBuilderBind& BuilderBind, CString strField, CString strCondition, __int64 nValue);

public:
	CString GetOperatorForCondition(CString strCondition);

private:
	int GetLastTokenType();
	bool AppendCondition(CString strField, CString strCondition);
	bool IsValid() { return m_bValid; }

private:
	CArray<CSQLWhereToken, CSQLWhereToken> m_arrayTokens;
	bool m_bValid;
	int m_nBracketDepth;
};

//**********************************************************************

