#pragma once
//$$******************************************************************
#include "CommentHandler.h"
//$$******************************************************************

class CTerminalListHandler
{
public:
	CTerminalListHandler(CString strFilename);
	void SetTerminalSelectType(int n) { m_nTerminalSelectType = n; }
	void SetTargetDbNo(int n) { m_nTargetDbNo = n; }
	void SetTargetFolderSet(int n) { m_nTargetFolderSet = n; }

public:
	int TNoListGetSize() { return m_TNoList.GetSize(); }
	CString TNoListGetItem(int n) { return m_TNoList.GetAt(n); }
	void TNoListDeleteItem(CString str) { m_TNoList.Delete(str); }
	void TNoListSetMaxLength(int n) { m_TNoList.SetMaxLength(n); }
	void TNoListSave(CString str) { m_TNoList.Save(str); }
	void TNoListExtractTNos(const char* szList) { m_TNoList.ExtractTNos(szList, &m_arrayTNo); }

public:
	int TNoArrayGetCount() { return m_arrayTNo.GetCount(); }
	void TNoArrayRemoveAll() { m_arrayTNo.RemoveAll(); }
	UINT TNoArrayGetAt(int n) { return m_arrayTNo.GetAt(n); }

public:
	bool CheckTerminalFilter(int nDbNo, int nFolderSet, int nTNo);
	bool FindTNo(int nTNo);

private:
	int m_nTerminalSelectType;
	CUIntArray m_arrayTNo;
	CCommentHandler m_TNoList;
	int m_nTargetDbNo;
	int m_nTargetFolderSet;
};

//$$******************************************************************