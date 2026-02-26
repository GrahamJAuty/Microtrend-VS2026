#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpers.h"
#include "KeyboardMap.h"
#include "KeyboardNameArray.h"
#include "DataNodeInfoKeyboard.h"
#include "SelectMultipleEntityInfo.h"
/**********************************************************************/

class CDataAccessHelpersKeyboard : public CDataAccessHelpers
{
public:
	CDataAccessHelpersKeyboard();
	void BuildKeyboardMap();
	
public:
	int GetKeyboardMapSize(){ return m_KeyboardMap.GetMapInfoCount(); }
	void GetKeyboardMapInfo( int nIdx, CKeyboardMapInfo& info ){ m_KeyboardMap.GetMapInfo( nIdx, info ); }

public:
	bool EditKeyboard( CDataNodeInfoKeyboard& infoNode );
	int CloneKeyboards( CDataNodeInfoKeyboard& infoNode );
	int DeleteKeyboards( CDataNodeInfoKeyboard& infoNode );
	void DownloadKeyboard( CDataNodeInfoKeyboard& infoNode  );
	bool EditNames( CDataNodeInfoKeyboard& infoNode  );

public:
	void ShowDeletionResult( int nResult );
	void ShowCloneResult( int nResult );

public:
	void ConvertMapInfoToNodeInfo( CKeyboardMapInfo& infoMap, CDataNodeInfoKeyboard& infoNode );
	int GetKbNoByNodeType( int nId );
	const char* GetKeyboardFolder( CDataNodeInfoKeyboard& infoNode );
	const char* GetKeyboardLabel ( CDataNodeInfoKeyboard& infoNode );

public:
	bool KeyboardInUse( CDataNodeInfoKeyboard& infoNode );
	bool LocKeyboardInUse( CDataNodeInfoKeyboard& infoNode );
	bool DbKeyboardInUse( CDataNodeInfoKeyboard& infoNode );

public:
	bool IsLocKeyboardNode( CDataNodeInfoKeyboard& infoNode );
	bool IsDbKeyboardNode( CDataNodeInfoKeyboard& infoNode );
	bool IsKeyboardNode( CDataNodeInfoKeyboard& infoNode );

private:
	void EditKeyboardInternal( CDataNodeInfoKeyboard& infoNode );
	void GetKeyboardListForCloning( CDataNodeInfoKeyboard& infoNode, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arraySets, CArray<int,int>& arrayLocs, bool bSingle );
	int DeleteSingleKeyboard( CDataNodeInfoKeyboard& infoNode );
	int DeleteMultipleKeyboards( CDataNodeInfoKeyboard& infoNode );
	
private:
	CKeyboardNameArray m_KeyboardNameArray;
	CKeyboardMap m_KeyboardMap;

private:
	CString m_strKeyboardFolder;
	CString m_strKeyboardLabel;
};

/**********************************************************************/
#endif
/**********************************************************************/
