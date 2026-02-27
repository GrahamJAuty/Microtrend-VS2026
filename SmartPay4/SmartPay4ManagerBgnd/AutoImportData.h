#pragma once

//*******************************************************************

class CAutoImportData
{
public:
	CAutoImportData();
	bool Read();
	bool Save();

public:
	int m_nFileFormat;
	CString m_strImportFilename;
	int m_nImportMethod;
	bool m_bImportPhotoID;

private:
	CString m_strFilename;
};
