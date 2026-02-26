//**********************************************************************
#include "SQLWhereBuilder.h"
//**********************************************************************

CSQLWhereBuilder::CSQLWhereBuilder()
{
	m_bValid = TRUE;
	m_nBracketDepth = 0;
}

//**********************************************************************

CString CSQLWhereBuilder::GetOperatorForCondition(CString strCondition)
{
	if (strCondition == szLESSTHAN)
	{
		return " < ";
	}

	if (strCondition == szLESSTHANOREQUAL)
	{
		return " <= ";
	}

	if (strCondition == szGREATER)
	{
		return " > ";
	}

	if (strCondition == szGREATEROREQUAL)
	{
		return " >= ";
	}

	if (strCondition == szEQUAL)
	{
		return " = ";
	}

	if (strCondition == szNOTEQUAL)
	{
		return " != ";
	}

	if (strCondition == szLIKE)
	{
		return " LIKE ";
	}

	return "";
}

//**********************************************************************

int CSQLWhereBuilder::GetLastTokenType()
{
	if (m_arrayTokens.GetSize() == 0)
	{
		return SQL_WHERE_NONE;
	}
	else
	{
		return m_arrayTokens[m_arrayTokens.GetSize() - 1].m_nType;
	}
}

//**********************************************************************

bool CSQLWhereBuilder::AppendCondition( CString strField, CString strCondition)
{
	bool bResult = FALSE;

	if ( (TRUE == m_bValid) && ( strField != "" ) )
	{
		switch (GetLastTokenType())
		{
		case SQL_WHERE_NONE:
		case SQL_WHERE_OPENBRACKET:
		case SQL_WHERE_OR:
		case SQL_WHERE_AND:
			{
				CString strOP = GetOperatorForCondition(strCondition);

				if (strOP != "")
				{
					CSQLWhereToken Token;
					Token.m_nType = SQL_WHERE_CONDITION;
					Token.m_strCondition = "";
					Token.m_strCondition += strField;
					Token.m_strCondition += strOP;
					Token.m_strCondition += "?";
					m_arrayTokens.Add(Token);
					bResult = TRUE;
				}
			}
			break;

		default:
			m_bValid = FALSE;
			break;
		}
	}

	return bResult;
}

//**********************************************************************

void CSQLWhereBuilder::AppendConditionDirect(CString strCondition)
{
	if (TRUE == m_bValid)
	{
		switch (GetLastTokenType())
		{
		case SQL_WHERE_NONE:
		case SQL_WHERE_OPENBRACKET:
		case SQL_WHERE_OR:
		case SQL_WHERE_AND:
			{
				CSQLWhereToken Token;
				Token.m_nType = SQL_WHERE_CONDITION;
				Token.m_strCondition = strCondition;
				m_arrayTokens.Add(Token);
			}
			break;

		default:
			m_bValid = FALSE;
			break;
		}
	}
}

//**********************************************************************

void CSQLWhereBuilder::AppendConditionString(CSQLBuilderBind& BuilderBind, CString strField, CString strCondition, CString strValue)
{
	if ( AppendCondition(strField, strCondition) == TRUE)
	{
		BuilderBind.AddString(strValue);
	}
}

//**********************************************************************

void CSQLWhereBuilder::AppendConditionDouble(CSQLBuilderBind& BuilderBind, CString strField, CString strCondition, double dValue)
{
	if (AppendCondition(strField, strCondition) == TRUE)
	{
		BuilderBind.AddDouble(dValue);
	}
}

//**********************************************************************

void CSQLWhereBuilder::AppendConditionInt(CSQLBuilderBind& BuilderBind, CString strField, CString strCondition, int nValue)
{
	if (AppendCondition(strField, strCondition) == TRUE)
	{
		BuilderBind.AddInt32(nValue);
	}
}

//**********************************************************************

void CSQLWhereBuilder::AppendConditionInt64(CSQLBuilderBind& BuilderBind, CString strField, CString strCondition, __int64 nValue)
{
	if (AppendCondition(strField, strCondition) == TRUE)
	{
		BuilderBind.AddInt64(nValue);
	}
}

//**********************************************************************

void CSQLWhereBuilder::AppendOpenBracket()
{
	if (TRUE == m_bValid)
	{
		switch (GetLastTokenType())
		{
		case SQL_WHERE_NONE:
		case SQL_WHERE_OPENBRACKET:
		case SQL_WHERE_OR:
		case SQL_WHERE_AND:
			{
				CSQLWhereToken Token;
				Token.m_nType = SQL_WHERE_OPENBRACKET;
				Token.m_strCondition = "";
				m_arrayTokens.Add(Token);
				m_nBracketDepth++;
			}
			break;
		}
	}
}

//**********************************************************************

void CSQLWhereBuilder::AppendCloseBracket()
{
	if (TRUE == m_bValid)
	{
		switch (GetLastTokenType())
		{
		case SQL_WHERE_OPENBRACKET:
			if (m_nBracketDepth <= 0)
			{
				m_bValid = FALSE;
			}
			else
			{
				m_arrayTokens.RemoveAt(m_arrayTokens.GetSize() - 1);
				m_nBracketDepth--;
			}
			break;

		case SQL_WHERE_CLOSEBRACKET:
		case SQL_WHERE_CONDITION:
			if (m_nBracketDepth <= 0)
			{
				m_bValid = FALSE;
			}
			else
			{
				CSQLWhereToken Token;
				Token.m_nType = SQL_WHERE_CLOSEBRACKET;
				Token.m_strCondition = "";
				m_arrayTokens.Add(Token);
				m_nBracketDepth--;
			}
			break;

		default:
			m_bValid = FALSE;
			break;
		}
	}
}

//**********************************************************************

void CSQLWhereBuilder::AppendAnd()
{
	if (TRUE == m_bValid)
	{
		switch (GetLastTokenType())
		{
		case SQL_WHERE_NONE:
		case SQL_WHERE_OPENBRACKET:
			break;

		case SQL_WHERE_CLOSEBRACKET:
		case SQL_WHERE_CONDITION:
			{
				CSQLWhereToken Token;
				Token.m_nType = SQL_WHERE_AND;
				Token.m_strCondition = "";
				m_arrayTokens.Add(Token);
			}
			break;

		default:
			m_bValid = FALSE;
			break;
		}
	}
}

//**********************************************************************

void CSQLWhereBuilder::AppendOr()
{
	if (TRUE == m_bValid)
	{
		switch (GetLastTokenType())
		{
		case SQL_WHERE_NONE:
		case SQL_WHERE_OPENBRACKET:
			break;

		case SQL_WHERE_CLOSEBRACKET:
		case SQL_WHERE_CONDITION:
			{
				CSQLWhereToken Token;
				Token.m_nType = SQL_WHERE_OR;
				Token.m_strCondition = "";
				m_arrayTokens.Add(Token);
			}
			break;

		default:
			m_bValid = FALSE;
			break;
		}
	}
}

//**********************************************************************

CString CSQLWhereBuilder::GetWhereString()
{
	CString strWhere = "";

	if ((TRUE == m_bValid) && (0 == m_nBracketDepth))
	{
		for (int n = 0; n < m_arrayTokens.GetSize(); n++)
		{
			if (strWhere != "")
			{
				strWhere += " ";
			}

			switch (m_arrayTokens[n].m_nType)
			{
			case SQL_WHERE_OPENBRACKET:
				strWhere += "(";
				break;

			case SQL_WHERE_CLOSEBRACKET:
				strWhere += ")";
				break;

			case SQL_WHERE_AND:
				strWhere += "AND";
				break;

			case SQL_WHERE_OR:
				strWhere += "OR";
				break;

			case SQL_WHERE_CONDITION:
				strWhere += m_arrayTokens[n].m_strCondition;
				break;
			}
		}
	}

	return strWhere;
}

//**********************************************************************
