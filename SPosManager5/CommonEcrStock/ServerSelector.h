#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CServerSelector  
{
public:
	CServerSelector( CEdit& edit, CSSComboBox& combo );
	
public:
	void FillServerComboSystem( int nServerNo );
	void FillServerComboDatabase( int nDbIdx, int nServerNo );
	void FillServerComboLocation( int nLocIdx, int nServerNo );
	
private:
	void FillServerCombo( int nServerPos, int nStartPos, int nEndPos );

public:
	void SelectServerFromCombo();
	void SelectServerFromEditBox( bool bUseCombo );
	bool UpdateServerCombo( int nNewServerNo );
	void UpdateServerEditBox();
	
public:
	int GetServerNo() { return m_nServerNo; }
	
private:
	CEdit& m_EditBox;
	CSSComboBox& m_ComboBox;
	int m_nServerNo;
};

/**********************************************************************/
#endif
/**********************************************************************/
