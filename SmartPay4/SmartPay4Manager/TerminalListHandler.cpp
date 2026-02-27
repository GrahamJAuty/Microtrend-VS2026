//$$******************************************************************
#include "TerminalListHandler.h"
//$$******************************************************************

CTerminalListHandler::CTerminalListHandler(CString strFilename) : m_TNoList(strFilename)
{
	m_nTerminalSelectType = 0;
	m_nTargetFolderSet = 0;
}

//$$******************************************************************

bool CTerminalListHandler::FindTNo(int nTNo)
{
	for (int nIndex = 0; nIndex < m_arrayTNo.GetCount(); nIndex++)
	{
		if (m_arrayTNo.GetAt(nIndex) == nTNo)
		{
			return TRUE;
		}
	}
	return FALSE;
}

//$$******************************************************************

bool CTerminalListHandler::CheckTerminalFilter(int nFolderSet, int nTNo)
{
	if (0 == m_nTargetFolderSet)
	{
		return TRUE;
	}

	if (nFolderSet != m_nTargetFolderSet)
	{
		return FALSE;
	}

	if (m_nTerminalSelectType != 1)
	{
		return TRUE;
	}

	for (int i = 0; i < m_arrayTNo.GetCount(); i++)
	{
		if (nTNo == m_arrayTNo.GetAt(i))
		{
			return TRUE;
		}
	}

	return FALSE;
}

//$$******************************************************************
