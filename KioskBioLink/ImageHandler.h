//*******************************************************************
//*** exactly same file used in KioskCardLink \ KioskBioLink ********
//*******************************************************************

#pragma once

class CImageHandler
{
public:
	CImageHandler ( const char* szPath, const char* szFilename );
	CString GetFilenameMask() { return m_strFilenameMask; }
	bool GetFirst();
	CString GetFilepath() { return m_strFilename; }			// NB** must call IsFound() first

private:
	CString m_strFilenameMask;
	CString m_strFilename;
};
