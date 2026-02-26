#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "EnhancedIniFile.h"
#include "LocationSelectorEntity.h"
/**********************************************************************/

class CButtonOptions 
{
public:
	CButtonOptions();
	void Reset();
	
	bool Read();
	bool Write();

	void RememberSettings();
	void RestoreSettings();
	
	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	void CopyFrom( CButtonOptions& source );	
	void SetDefaults();
	
private:
	bool ReadInternal();
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );
	void PrepareIniFile( CEnhancedIniFile& iniFile );
	void MakeIntSettings();
	
public:
	bool GetLayoutEnable()							{ return m_LayoutEnable.GetValue(); }
	/*****/
	bool GetWindowWidthAuto()						{ return m_WindowWidthAuto.GetValue(); }
	int GetWindowWidthManual()						{ return m_WindowWidthManual.GetValue(); }
	/*****/
	int GetUpperAreaHeightAuto()					{ return m_UpperAreaHeightAuto.GetValue(); }
	int GetUpperAreaHeightManual()					{ return m_UpperAreaHeightManual.GetValue(); }
	const char* GetUpperAreaFillColour()			{ return m_UpperAreaFillColour.GetValue(); }
	/*****/
	const char* GetUpperImageType()					{ return m_UpperImageType.GetValue(); }
	int GetUpperImageMarginTop()					{ return m_UpperImageMarginTop.GetValue(); }
	int GetUpperImageMarginBottom()					{ return m_UpperImageMarginBottom.GetValue(); }
	int GetUpperImageMarginLeft()					{ return m_UpperImageMarginLeft.GetValue(); }
	int GetUpperImageMarginRight()					{ return m_UpperImageMarginRight.GetValue(); }
	const char* GetUpperImageXAlign()				{ return m_UpperImageXAlign.GetValue(); }
	const char* GetUpperImageYAlign()				{ return m_UpperImageYAlign.GetValue(); }
	bool GetUpperImageStretch()						{ return m_UpperImageStretch.GetValue(); }
	/*****/
	int GetLowerAreaHeightManual()					{ return m_LowerAreaHeightManual.GetValue(); }
	const char* GetLowerAreaFillColour()			{ return m_LowerAreaFillColour.GetValue(); }
	int GetLowerAreaTaskbarGap()					{ return m_LowerAreaTaskbarGap.GetValue(); }
	/*****/
	const char* GetLowerImageType()					{ return m_LowerImageType.GetValue(); }
	int GetLowerImageMarginTop()					{ return m_LowerImageMarginTop.GetValue(); }
	int GetLowerImageMarginBottom()					{ return m_LowerImageMarginBottom.GetValue(); }
	int GetLowerImageMarginLeft()					{ return m_LowerImageMarginLeft.GetValue(); }
	int GetLowerImageMarginRight()					{ return m_LowerImageMarginRight.GetValue(); }
	const char* GetLowerImageXAlign()				{ return m_LowerImageXAlign.GetValue(); }
	const char* GetLowerImageYAlign()				{ return m_LowerImageYAlign.GetValue(); }
	bool GetLowerImageStretch()						{ return m_LowerImageStretch.GetValue(); }
	/*****/
	const char* GetButtonBarHome()					{ return m_ButtonBarHome.GetValue(); }
	const char* GetButtonBarDirection()				{ return m_ButtonBarDirection.GetValue(); }
	const char* GetButtonBarControlLinePos()		{ return m_ButtonBarControlLinePos.GetValue(); }
	const char* GetButtonBarControlButtonPos()		{ return m_ButtonBarControlButtonPos.GetValue(); }
	bool GetButtonBarParentButtonFlag()				{ return m_ButtonBarParentButton.GetValue(); }
	bool GetButtonBarCentre()						{ return m_ButtonBarCentre.GetValue(); }
	int GetButtonBarMarginTop()						{ return m_ButtonBarMarginTop.GetValue(); }
	int GetButtonBarMarginBottom()					{ return m_ButtonBarMarginBottom.GetValue(); }
	int GetButtonBarMarginLeft()					{ return m_ButtonBarMarginLeft.GetValue(); }
	int GetButtonBarMarginRight()					{ return m_ButtonBarMarginRight.GetValue(); }
	int GetButtonBarLineSize()						{ return m_ButtonBarLineSize.GetValue(); }
	int GetButtonBarLineCount()						{ return m_ButtonBarLineCount.GetValue(); }
	/*****/
	int GetButtonWidthType()						{ return m_ButtonWidthType.GetValue(); }
	int GetButtonWidthPercent()						{ return m_ButtonWidthPercent.GetValue(); }
	int GetButtonWidthPixel()						{ return m_ButtonWidthPixel.GetValue(); }
	int GetButtonHeightType()						{ return m_ButtonHeightType.GetValue(); }
	int GetButtonHeightPercent()					{ return m_ButtonHeightPercent.GetValue(); }
	int GetButtonHeightPixel()						{ return m_ButtonHeightPixel.GetValue(); }
	int GetButtonXGap()								{ return m_ButtonXGap.GetValue(); }
	int GetButtonYGap()								{ return m_ButtonYGap.GetValue(); }
	int GetButtonRounding()							{ return m_ButtonRounding.GetValue(); }
	const char* GetButtonColourBackAction()			{ return m_ButtonColourBackAction.GetValue(); }
	const char* GetButtonColourBackList()			{ return m_ButtonColourBackList.GetValue(); }
	const char* GetButtonColourBackUnused()			{ return m_ButtonColourBackUnused.GetValue(); }
	const char* GetButtonColourBackControl()		{ return m_ButtonColourBackControl.GetValue(); }
	const char* GetButtonColourBackParent()			{ return m_ButtonColourBackParent.GetValue(); }
	const char* GetButtonColourTextAction()			{ return m_ButtonColourTextAction.GetValue(); }
	const char* GetButtonColourTextList()			{ return m_ButtonColourTextList.GetValue(); }
	const char* GetButtonColourTextControl()		{ return m_ButtonColourTextControl.GetValue(); }
	const char* GetButtonColourTextParent()			{ return m_ButtonColourTextParent.GetValue(); }
	/*****/
	int GetButtonImageTopMarginPercent()			{ return m_ButtonImageTopMarginPercent.GetValue(); }
	int GetButtonImageBottomMarginPercent()			{ return m_ButtonImageBottomMarginPercent.GetValue(); }
	int GetButtonImageLeftMarginPercent()			{ return m_ButtonImageLeftMarginPercent.GetValue(); }
	int GetButtonImageRightMarginPercent()			{ return m_ButtonImageRightMarginPercent.GetValue(); }
	/*****/
	
public:
	int GetUpperAreaFillColourInt()					{ return m_nUpperAreaFillColour; }
	int GetUpperImageTypeInt()						{ return m_nUpperImageType; }
	int GetUpperImageXAlignInt()					{ return m_nUpperImageXAlign; }
	int GetUpperImageYAlignInt()					{ return m_nUpperImageYAlign; }
	/*****/
	int GetLowerAreaFillColourInt()					{ return m_nLowerAreaFillColour; }
	int GetLowerImageTypeInt()						{ return m_nLowerImageType; }
	int GetLowerImageXAlignInt()					{ return m_nLowerImageXAlign; }
	int GetLowerImageYAlignInt()					{ return m_nLowerImageYAlign; }
	/*****/
	int GetButtonBarHomeInt()						{ return m_nButtonBarHome; }
	int GetButtonBarDirectionInt()					{ return m_nButtonBarDirection; }
	int GetButtonBarControlLinePosInt()				{ return m_nButtonBarControlLinePos; }
	int GetButtonBarControlButtonPosInt()			{ return m_nButtonBarControlButtonPos; }
	/*****/
	int GetButtonColourBackActionInt()				{ return m_nButtonColourBackAction; }
	int GetButtonColourBackListInt()				{ return m_nButtonColourBackList; }
	int GetButtonColourBackUnusedInt()				{ return m_nButtonColourBackUnused; }
	int GetButtonColourBackControlInt()				{ return m_nButtonColourBackControl; }
	int GetButtonColourBackParentInt()				{ return m_nButtonColourBackParent; }
	int GetButtonColourTextActionInt()				{ return m_nButtonColourTextAction; }
	int GetButtonColourTextListInt()				{ return m_nButtonColourTextList; }
	int GetButtonColourTextControlInt()				{ return m_nButtonColourTextControl; }
	int GetButtonColourTextParentInt()				{ return m_nButtonColourTextParent; }

private:
	int MakeColourInt( CIniFileString& iniString );
	int MakeImageTypeInt( CIniFileString& iniString );
	int MakeImageXAlignInt( CIniFileString& iniString );
	int MakeImageYAlignInt( CIniFileString& iniString );
	int MakeButtonBarHomeInt( CIniFileString& iniString );
	int MakeButtonBarDirectionInt( CIniFileString& iniString );
	int MakeButtonBarControlLinePosInt( CIniFileString& iniString );
	int MakeButtonBarControlButtonPosInt( CIniFileString& iniString );
	bool MakeColourString( int nColour, CString& strColour );

public:
	void SetLayoutEnable( bool b )					{ m_LayoutEnable.SetValue(b); }
	/*****/
	void SetWindowWidthAuto( bool b )				{ m_WindowWidthAuto.SetValue(b); }
	void SetWindowWidthManual( int n )				{ m_WindowWidthManual.SetValue(n); }
	/*****/
	void SetUpperAreaHeightAuto( bool b )			{ m_UpperAreaHeightAuto.SetValue(b); }
	void SetUpperAreaHeightManual( int n )			{ m_UpperAreaHeightManual.SetValue(n); }
	void SetUpperAreaFillColourInt( int n );
	/*****/
	void SetUpperImageTypeInt( int n );
	void SetUpperImageMarginTop( int n )			{ m_UpperImageMarginTop.SetValue(n); }
	void SetUpperImageMarginBottom( int n )			{ m_UpperImageMarginBottom.SetValue(n); }
	void SetUpperImageMarginLeft( int n )			{ m_UpperImageMarginLeft.SetValue(n); }
	void SetUpperImageMarginRight( int n )			{ m_UpperImageMarginRight.SetValue(n); }
	void SetUpperImageXAlignInt( int n );
	void SetUpperImageYAlignInt( int n );
	void SetUpperImageStretch( bool b )				{ m_UpperImageStretch.SetValue(b); }
	/*****/
	void SetLowerAreaHeightManual( int n )			{ m_LowerAreaHeightManual.SetValue(n); }
	void SetLowerAreaFillColourInt( int n );
	void SetLowerAreaTaskbarGap( int n )			{ m_LowerAreaTaskbarGap.SetValue(n); }
	/*****/
	void SetLowerImageTypeInt( int n );
	void SetLowerImageMarginTop( int n )			{ m_LowerImageMarginTop.SetValue(n); }
	void SetLowerImageMarginBottom( int n )			{ m_LowerImageMarginBottom.SetValue(n); }
	void SetLowerImageMarginLeft( int n )			{ m_LowerImageMarginLeft.SetValue(n); }
	void SetLowerImageMarginRight( int n )			{ m_LowerImageMarginRight.SetValue(n); }
	void SetLowerImageXAlignInt( int n );
	void SetLowerImageYAlignInt( int n );
	void SetLowerImageStretch( bool b )				{ m_LowerImageStretch.SetValue(b); }
	/*****/
	void SetButtonBarHomeInt( int n );
	void SetButtonBarDirectionInt( int n );
	void SetButtonBarControlLinePosInt( int n );
	void SetButtonBarControlButtonPosInt( int n );
	void SetButtonBarParentButtonFlag( bool b )		{ m_ButtonBarParentButton.SetValue(b); }
	void SetButtonBarCentre( bool b )				{ m_ButtonBarCentre.SetValue(b); }
	void SetButtonBarMarginTop( int n )				{ m_ButtonBarMarginTop.SetValue(n); }
	void SetButtonBarMarginBottom( int n )			{ m_ButtonBarMarginBottom.SetValue(n); }
	void SetButtonBarMarginLeft( int n )			{ m_ButtonBarMarginLeft.SetValue(n); }
	void SetButtonBarMarginRight( int n )			{ m_ButtonBarMarginRight.SetValue(n); }
	void SetButtonBarLineSize( int n )				{ m_ButtonBarLineSize.SetValue(n); }
	void SetButtonBarLineCount( int n )				{ m_ButtonBarLineCount.SetValue(n); }
	/*****/
	void SetButtonWidthType( int n )				{ m_ButtonWidthType.SetValue(n); }
	void SetButtonWidthPercent( int n )				{ m_ButtonWidthPercent.SetValue(n); }
	void SetButtonWidthPixel( int n )				{ m_ButtonWidthPixel.SetValue(n); }
	void SetButtonHeightType( int n )				{ m_ButtonHeightType.SetValue(n); }
	void SetButtonHeightPercent( int n )			{ m_ButtonHeightPercent.SetValue(n); }
	void SetButtonHeightPixel( int n )				{ m_ButtonHeightPixel.SetValue(n); }
	void SetButtonXGap( int n )						{ m_ButtonXGap.SetValue(n); }
	void SetButtonYGap( int n )						{ m_ButtonYGap.SetValue(n); }
	void SetButtonRounding( int n )					{ m_ButtonRounding.SetValue(n); }
	void SetButtonColourBackActionInt( int n );
	void SetButtonColourBackListInt( int n );
	void SetButtonColourBackParentInt( int n );
	void SetButtonColourBackControlInt( int n );
	void SetButtonColourBackUnusedInt( int n );
	void SetButtonColourTextActionInt( int n );
	void SetButtonColourTextListInt( int n );
	void SetButtonColourTextParentInt( int n );
	void SetButtonColourTextControlInt( int n );
	/*****/
	void SetButtonImageTopMarginPercent( int n )	{ m_ButtonImageTopMarginPercent.SetValue(n); }
	void SetButtonImageBottomMarginPercent( int n )	{ m_ButtonImageBottomMarginPercent.SetValue(n); }
	void SetButtonImageLeftMarginPercent( int n )	{ m_ButtonImageLeftMarginPercent.SetValue(n); }
	void SetButtonImageRightMarginPercent( int n ) 	{ m_ButtonImageRightMarginPercent.SetValue(n); }
	/*****/

private:
	CIniFileBool	m_LayoutEnable;
	/*****/
	CIniFileBool	m_WindowWidthAuto;
	CIniFileInt		m_WindowWidthManual;
	/*****/
	CIniFileBool	m_UpperAreaHeightAuto;
	CIniFileInt		m_UpperAreaHeightManual;
	CIniFileString	m_UpperAreaFillColour;
	/*****/
	CIniFileString	m_UpperImageType;
	CIniFileInt		m_UpperImageMarginTop;
	CIniFileInt		m_UpperImageMarginBottom;
	CIniFileInt		m_UpperImageMarginLeft;
	CIniFileInt		m_UpperImageMarginRight;
	CIniFileString	m_UpperImageXAlign;
	CIniFileString	m_UpperImageYAlign;
	CIniFileBool	m_UpperImageStretch;
	/*****/
	CIniFileInt		m_LowerAreaHeightManual;
	CIniFileString	m_LowerAreaFillColour;
	CIniFileInt		m_LowerAreaTaskbarGap;
	/*****/
	CIniFileString	m_LowerImageType;
	CIniFileInt		m_LowerImageMarginTop;
	CIniFileInt		m_LowerImageMarginBottom;
	CIniFileInt		m_LowerImageMarginLeft;
	CIniFileInt		m_LowerImageMarginRight;
	CIniFileString	m_LowerImageXAlign;
	CIniFileString	m_LowerImageYAlign;
	CIniFileBool	m_LowerImageStretch;
	/*****/
	CIniFileString	m_ButtonBarHome;
	CIniFileString	m_ButtonBarDirection;
	CIniFileString	m_ButtonBarControlLinePos;
	CIniFileString	m_ButtonBarControlButtonPos;
	CIniFileBool	m_ButtonBarParentButton;
	CIniFileBool	m_ButtonBarCentre;
	CIniFileInt		m_ButtonBarMarginTop;
	CIniFileInt		m_ButtonBarMarginBottom;
	CIniFileInt		m_ButtonBarMarginLeft;
	CIniFileInt		m_ButtonBarMarginRight;
	CIniFileInt		m_ButtonBarLineSize;
	CIniFileInt		m_ButtonBarLineCount;
	/*****/
	CIniFileInt		m_ButtonWidthType;
	CIniFileInt		m_ButtonWidthPercent;
	CIniFileInt		m_ButtonWidthPixel;
	CIniFileInt		m_ButtonHeightType;
	CIniFileInt		m_ButtonHeightPercent;
	CIniFileInt		m_ButtonHeightPixel;
	CIniFileInt		m_ButtonXGap;
	CIniFileInt		m_ButtonYGap;
	CIniFileInt		m_ButtonRounding;
	CIniFileString	m_ButtonColourBackAction;
	CIniFileString	m_ButtonColourBackList;
	CIniFileString	m_ButtonColourBackUnused;
	CIniFileString	m_ButtonColourBackControl;
	CIniFileString	m_ButtonColourBackParent;
	CIniFileString	m_ButtonColourTextAction;
	CIniFileString	m_ButtonColourTextList;
	CIniFileString	m_ButtonColourTextControl;
	CIniFileString	m_ButtonColourTextParent;
	/*****/
	CIniFileInt		m_ButtonImageTopMarginPercent;
	CIniFileInt		m_ButtonImageBottomMarginPercent;
	CIniFileInt		m_ButtonImageLeftMarginPercent;
	CIniFileInt		m_ButtonImageRightMarginPercent;
	/*****/
	
private:
	int m_nUpperAreaFillColour;
	int m_nUpperImageType;
	int m_nUpperImageXAlign;
	int m_nUpperImageYAlign;
	/*****/
	int m_nLowerAreaFillColour;
	int m_nLowerImageType;
	int m_nLowerImageXAlign;
	int m_nLowerImageYAlign;
	/*****/
	int m_nButtonBarHome;
	int m_nButtonBarDirection;
	int m_nButtonBarControlLinePos;
	int m_nButtonBarControlButtonPos;
	/*****/
	int m_nButtonColourBackAction;
	int m_nButtonColourBackList;
	int m_nButtonColourBackUnused;
	int m_nButtonColourBackControl;
	int m_nButtonColourBackParent;
	int m_nButtonColourTextAction;
	int m_nButtonColourTextList;
	int m_nButtonColourTextControl;
	int m_nButtonColourTextParent;

private:
	bool m_bFatalReadError;

	friend class CLayoutOptionsDlg;
};

/**********************************************************************/
extern CButtonOptions ButtonOptions;
/**********************************************************************/
#endif
/**********************************************************************/
