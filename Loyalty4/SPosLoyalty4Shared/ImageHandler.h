#pragma once
/******************************************************************************/

class CImageHandler
{
public:
	CImageHandler ( const char* szUNCPath = "" );
	CString GetFirst ( const char* szCardNo );
	void CopyFile ( const char* szFromCardNo, const char* szToCardNo );
	void DeleteFile ( const char* szcardNo );
	void DeleteAllExcept ( const char* szCardNo, CString& strExtToSave );

public:
	CString GetImageFileTitle ( const char* szCardNo );			// full path with no extension

private:
	CString m_strDataPath;
};

/******************************************************************************/
