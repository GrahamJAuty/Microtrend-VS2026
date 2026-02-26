#pragma once
//$$******************************************************************

class CUserTexts
{
public:
	CUserTexts() {}
	bool Read() { return m_file.Read(Filenames.GetUserTextsFilename()); }
	bool Write() { return m_file.Write(Filenames.GetUserTextsFilename()); }

private:
	void GetFieldData(int nField, CString& strField, CString& strData, CString strDefault);
	void GetFieldInfo(int nField, CString& strField, CString& strFallback);

public:
	CString GetText(int nField, CString strDefault);
	bool GetCompulsoryFlag(int nField);
	bool GetDateFlag(int nField);

public:
	void SetText(int nField, CString strText);
	void SetCompulsoryFlag(int nField, bool b);
	void SetDateFlag(int nField, bool b);

private:
	CSSIniFile m_file;
};

//$$******************************************************************
