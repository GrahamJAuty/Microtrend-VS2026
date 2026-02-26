#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************

class CDbExportEditor  
{
public:
	CDbExportEditor ( bool bLockBasicFields, CWnd* pParent );
	bool EditLabel ( const char* szLabel, const char* szTitle = NULL  );

private:
	CWnd* m_pParent;
	bool m_bLockBasicFields;
};

//*******************************************************************
