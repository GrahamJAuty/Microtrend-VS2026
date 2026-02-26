#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#define MIN_BACKIMAGE_WIDTH 800
#define MIN_BACKIMAGE_UPPER_HEIGHT 400
#define MIN_BACKIMAGE_LOWER_HEIGHT 100
/**********************************************************************/
#define BACKIMAGE_ERROR_NONE 0
#define BACKIMAGE_ERROR_LOAD 1
#define BACKIMAGE_ERROR_NOFILE 2
#define BACKIMAGE_ERROR_NODC 3
/**********************************************************************/
#define BACKIMAGE_ERROR_TOTALWIDTH 101
#define BACKIMAGE_ERROR_TOTALHEIGHT 102
/**********************************************************************/
#include "BitmapInfo.h"
/**********************************************************************/

class CBackgroundLayoutInfo
{
public:
	CBackgroundLayoutInfo(void);
	~CBackgroundLayoutInfo(void);

public:
	void Initialise( CDC* pDC );
	
public:
	int LoadUpperImage();
	int LoadLowerImage();
	bool ValidateLayout();
	bool GotUpperImage(){ return m_ImageUpper.GotImage(); }
	bool GotLowerImage(){ return m_ImageLower.GotImage(); }
	
public:
	void TempImageModeOn();
	void TempImageModeOff( bool bSave );

public:
	void CreateLayoutFolders();
	const char* GetLayoutPath(){ return m_strLayoutPath; }
	const char* GetLayoutPathTemp(){ return m_strLayoutPathTemp; }
	const char* GetButtonImagePath(){ return m_strButtonImagePath; }
	const char* GetButtonImagePathTemp(){ return m_strButtonImagePathTemp; }

public:
	void ClearErrors(){ m_arrayErrors.RemoveAll(); }
	void AddError( const char* szError ){ m_arrayErrors.Add( szError ); }
	int GetErrorCount(){ return m_arrayErrors.GetSize(); }
	const char* GetError( int n );

private:
	int LoadImage( CBitmapInfo& BitmapInfo, int nImageType, const char* szFilename );
	
public:
	bool GotValidLayout()				{ return m_bGotValidLayout; }
	int GetTotalWindowWidth()			{ return m_nTotalWindowWidth; }
	int GetUpperAreaHeight()			{ return m_nUpperAreaHeight; }
	int GetLowerAreaHeight()			{ return m_nLowerAreaHeight; }
	int GetUpperImageWidth()			{ return m_ImageUpper.GetWidth(); }
	int GetLowerImageWidth()			{ return m_ImageLower.GetWidth(); }
	int GetUpperImageHeight()			{ return m_ImageUpper.GetHeight(); }
	int GetLowerImageHeight()			{ return m_ImageLower.GetHeight(); }
	int GetUpperImageXOffset()			{ return m_nUpperImageXOffset; }
	int GetLowerImageXOffset()			{ return m_nLowerImageXOffset; }
	int GetUpperImageYOffset()			{ return m_nUpperImageYOffset; }
	int GetLowerImageYOffset()			{ return m_nLowerImageYOffset; }

public:
	int GetTotalAreaHeight()			{ return m_nUpperAreaHeight + m_nLowerAreaHeight; }
	int GetWindowSizeRequestX()			{ return m_nTotalWindowWidth + m_nFrameToViewAdjustX + m_nNonClientAdjustX; }
	int GetWindowSizeRequestY()			{ return GetTotalAreaHeight() + m_nFrameToViewAdjustY + m_nNonClientAdjustY + m_nStatusBarHeight; }

public:
	CDC* GetUpperBitmapDC()				{ return m_ImageUpper.GetDCImage(); }
	CDC* GetLowerBitmapDC()				{ return m_ImageLower.GetDCImage(); }

public:
	void SetStatusBarHeight( int n );
	void SetFrameToViewAdjustX( int n );
	void SetFrameToViewAdjustY( int n );
	void SetNonClientAdjustX( int n );
	void SetNonClientAdjustY( int n );
	
private:
	bool m_bGotValidLayout;
	CBitmapInfo m_ImageUpper;
	CBitmapInfo m_ImageLower;
	bool m_bTempImageMode;

private:
	int m_nTotalWindowWidth;
	int m_nUpperAreaHeight;
	int m_nLowerAreaHeight;
	int m_nUpperImageXOffset;
	int m_nLowerImageXOffset;
	int m_nUpperImageYOffset;
	int m_nLowerImageYOffset;

private:
	int m_nStatusBarHeight;
	int m_nFrameToViewAdjustX;
	int m_nFrameToViewAdjustY;
	int m_nNonClientAdjustX;
	int m_nNonClientAdjustY;

private:
	CString m_strLayoutPath;
	CString m_strLayoutPathTemp;
	CString m_strButtonImagePath;
	CString m_strButtonImagePathTemp;
	
private:
	CStringArray m_arrayErrors;
	CString m_strError;
};

/**********************************************************************/
extern CBackgroundLayoutInfo BackgroundLayoutInfo;
/**********************************************************************/
#endif
/**********************************************************************/
