#pragma once
/**********************************************************************/

class CCashRSPSpooler
{
public:
	CCashRSPSpooler( CSSFile& File );

public:
	bool ReadString(CString& strLine);
	void CloseFile();

public:
	bool GotManualLines() { return m_nManualPos < m_arrayManual.GetSize(); }
	bool IsManualMode() { return m_bManualMode; }
	void AddManualLine(CString strLine);

private:
	bool m_bManualMode;
	int m_nManualPos;

private:
	CSSFile& m_File;
	CStringArray m_arrayManual;
};

/**********************************************************************/