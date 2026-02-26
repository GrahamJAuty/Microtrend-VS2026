#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

class CBitmapInfo
{
public:
	CBitmapInfo();
	void LoadImage( const char* szFilename, bool bForceReload = FALSE );

	CDC* GetDCImage(){ return &m_dcImage; }
	
	bool CreatedDC(){ return m_bCreatedDC; }
	bool GotImage(){ return m_bGotImage; }
	const char* GetImagePath(){ return m_strImagePath; }
	int GetWidth(){ return m_nWidth; }
	int GetHeight(){ return m_nHeight; }

public:
	void SetWidth( int n ){ m_nWidth = n; }
	void SetHeight( int n ){ m_nHeight = n; }
	void SetGotImageFlag( bool b ){ m_bGotImage = b; }
	void SetImagePath( const char* sz ){ m_strImagePath = sz; }

public:
	void CreateOwnDC( CDC* pDC );
	void DeleteOwnDC();

private:
	bool m_bCreatedDC;
	bool m_bGotImage;
	CString m_strImagePath;
	int m_nWidth;
	int m_nHeight;
	
private:
	CDC m_dcImage;
};

/**********************************************************************/
#endif
/**********************************************************************/
