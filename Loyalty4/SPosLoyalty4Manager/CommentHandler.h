#pragma once
//$$******************************************************************

class CCommentHandler
{
public:
	CCommentHandler(CString strFilename);
	
public:
	int GetSize() { return m_arrayComments.GetSize(); }
	const char* GetAt(int nIndex) { return m_arrayComments.GetAt(nIndex); }
	void Delete(CString Comment);
	bool Save(CString szComment);
	bool SaveFile();

	void SetMaxLength(int nMax) { m_nMaxLength = nMax; }
	void ExtractTNos(CString strTerminalList, CUIntArray* ptrArrayTNo);

private:
	CString m_strFilename;
	CFileStringArray m_arrayComments;
	int m_nMaxLength;
};

//$$******************************************************************