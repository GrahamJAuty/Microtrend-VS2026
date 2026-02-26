#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

class CMultilineTextBuffer
{
public:
	CMultilineTextBuffer();

public:
	void SetPMSGroupSource(int n);
	void SetLocationNotesSource( int nLocIdx );

public:
	void Read();
	void WriteIfChanged();

public:
	void CopyBufferToEditBox( CEdit& edit );
	void CopyEditBoxToBuffer( CEdit& edit );
	void AppendToFile( CReportFile& ReportFile );

private:
	CString m_strFilename;
	CStringArray m_arrayBufferOld;
	CStringArray m_arrayBufferNew;
	int m_nMaxLength;
};

/**********************************************************************/
#endif
/**********************************************************************/
