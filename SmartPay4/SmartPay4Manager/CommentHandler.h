#pragma once

class CCommentHandler
{
public:
	CCommentHandler ( const char* szFilename );
	CCommentHandler();
	void ReadComments ( const char* szFilename );

	int GetSize() { return m_arrayComments.GetSize(); }
	const char* GetAt ( int nIndex ) { return m_arrayComments.GetAt(nIndex); }
	void Delete ( const char* szComment );
	bool Save ( const char* szComment );
	bool SaveFile();

	void SetMaxLength ( int nMax ) { m_nMaxLength = nMax; }
	void ExtractTNos ( const char* szTerminalList, CUIntArray* ptrArrayTNo );

private:
	CString m_strFilename;
	CFileStringArray m_arrayComments;
	int m_nMaxLength;
};
