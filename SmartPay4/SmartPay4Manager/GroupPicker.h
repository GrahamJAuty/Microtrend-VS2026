#pragma once

class CGroupPicker
{
public:
	CGroupPicker ( const char* szFilename = "" );
	void SetFilename ( const char* szFilename ) { m_strFilename = szFilename; }

	void Reset();
	int GetCount();
	int GetSingleGroupNo();
	bool IsValid ( int nGroupNo );

private:
	void LoadList();

private:
	CUIntArray m_array;
	CString m_strFilename;
};
