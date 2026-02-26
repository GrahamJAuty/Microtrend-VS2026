#pragma once
//*******************************************************************

class CPhotoIDFilenameStack
{
public:
	CPhotoIDFilenameStack();
	void Initialise( const char* szCardNo );
	
public:
	const char* GetImageFilename( int nLevel );
	void ImportFile( int nLevel, const char* szFilename );
	void RemoveFile( int nLevel );
	void PromoteFile( int nLevel );
	void RemoveOldFiles();

public:
	bool GetChangeFlag() { return m_bChange; }
	void SetChangeFlag() { m_bChange = TRUE; }

private:
	CString m_strBaseFilename;
	CString m_strExt1;
	CString m_strExt2;
	CString m_strExt3;
	CString m_strCardNo;

private:
	CString m_strImageFilename;
	bool m_bChange;
};

//*******************************************************************