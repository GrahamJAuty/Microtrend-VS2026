#pragma once
/**********************************************************************/

class CTrackedFile
{
public:
	CTrackedFile();

public:
	void SetFilename( CString strFilename );
	bool CheckFile( bool bForce );

public:
	CString GetFilename(){ return m_strFilename; }

private:
	CString m_strFilename;
	CTime m_ModifiedTime;
};

/**********************************************************************/

