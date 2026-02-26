#pragma once
/**********************************************************************/

class CSPOSKeyboardColourTable 
{
public:
	CSPOSKeyboardColourTable();
	void Reset();

public:
	int GetCurrentRGB( int nColourId );
	int GetDefaultRGB( int nColourId );
	int GetCursorColour( int nKeyColour );

	void SetCurrentRGB( int nColourId, int nRGB );
	void SetDefaultRGB( int nColourId, int nRGB );
	void RestoreDefaultRGB( int nStartColourId, int nEndColourId );

	void RememberUserColours();
	void RestoreUserColours();
	bool CompareUserColours();

public:
	void SetFilename( const char* sz ) { m_strFilename = sz; }
	void Read();
	void Write();
	
private:
	bool ReadInternal();

private:
	CString m_strFilename;
	int m_nCurrentRGB[64];
	int m_nDefaultRGB[64];
	int m_nBufferRGB[32];
};


/**********************************************************************/




	










	
