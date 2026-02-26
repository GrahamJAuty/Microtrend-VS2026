#pragma once
/**********************************************************************/
#include "KeyboardMap.h"
#include "DataNodeInfoKeyboard.h"
/**********************************************************************/

class CKeyboardNameArray
{
public:
	CKeyboardNameArray();

public:
	void GetKeyboardName( CDataNodeInfoKeyboard& infoNode, CString& strName );
	void SetKeyboardName( CDataNodeInfoKeyboard& infoNode, CString& strName );

	const char* GetKeyboardDisplayName( CDataNodeInfoKeyboard& infoNode );
	
	void CopyKeyboardNames( CDataNodeInfoKeyboard& infoSource, CDataNodeInfoKeyboard& infoDest );
	void WriteKeyboardNames( CDataNodeInfoKeyboard& infoNode );

public:
	void ReadNames();
	
private:
	int GetLocStartIdx( int nLocIdx );
	int GetKbSetStartIdx( int nKbdIdx );

public:
	void GetLocKeyboardName( int nLocIdx, int nKbNo, CString& strName );
	void SetLocKeyboardName( int nLocIdx, int nKbNo, CString& strName );
	void GetKbSetKeyboardName( int nKbsIdx, int nKbNo, CString& strName );
	void SetKbSetKeyboardName( int nKbsIdx, int nKbNo, CString& strName );

private:
	const char* GetLocKeyboardDisplayName( int nLocIdx, int nKbNo );
	const char* GetKbSetKeyboardDisplayName( int nDbIdx, int nKbsIdx, int nKbNo );

private:
	void WriteLocNames( int nLocIdx );
	void WriteKbSetNames( int nKbsIdx );

private:
	CStringArray m_arrayNames;
	CString m_strKeyboardDisplayName;
};

/**********************************************************************/
