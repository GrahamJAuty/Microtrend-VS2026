#pragma once
/**********************************************************************/
#include "SPOSKeyboardDefaultTextArray.h"
#include "SPOSKeyboardKeyRecord.h"
#include "SPOSKeyboardManager.h"
/**********************************************************************/

class CSPOSKeyboardKeyHelpers  
{
public:
	CSPOSKeyboardKeyHelpers();
	
public:
	const char* GetCombiText( int nType, __int64 nValue);
	void GetDefaultText( int nType, __int64 nValue, CString& strText, bool bSharpPos );
	void ValidateKeyText( CString& strText );
	void GetDefaultTextRecord( CSPOSKeyboardKeyRecord& KeyRecord, CSPOSKeyboardDefaultTextRecord& TextRecord );

private:
	void MakeEditableKeyText( const char* sz, CString& strText );

private:
	CString m_strCombiText;
};

/**********************************************************************/
