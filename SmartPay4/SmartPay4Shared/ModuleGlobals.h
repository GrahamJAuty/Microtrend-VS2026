#pragma once
//*******************************************************************
#include "Prompter.h"
//*******************************************************************

//******************************************************************
#ifdef SYSTEMTYPE_MANAGER
//******************************************************************

class CModuleGlobals
{
public:
	static bool IsManagerModule() { return TRUE; }
	static bool IsServerModule() { return FALSE; }
	static bool IsBackgroundModule() { return FALSE; }
	static void PrompterError(CString strError) { Prompter.Error(strError); }
	static void MessageBeepExclamation() { MessageBeep(MB_ICONEXCLAMATION); }
	static void MessageBeepQuestion() { MessageBeep(MB_ICONQUESTION); }
};

//******************************************************************
#endif
//******************************************************************

//******************************************************************
#ifdef SYSTEMTYPE_SERVER
//******************************************************************

class CModuleGlobals
{
public:
	static bool IsManagerModule() { return FALSE; }
	static bool IsServerModule() { return TRUE; }
	static bool IsBackgroundModule() { return FALSE; }
	static void PrompterError(CString strError) {}
	static void MessageBeepExclamation() {}
	static void MessageBeepQuestion() {}
};

//******************************************************************
#endif
//******************************************************************

//******************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//******************************************************************

class CModuleGlobals
{
public:
	static bool IsManagerModule() { return FALSE; }
	static bool IsServerModule() { return FALSE; }
	static bool IsBackgroundModule() { return TRUE; }
	static void PrompterError(CString strError) {}
	static void MessageBeepExclamation() {}
	static void MessageBeepQuestion() {}
};

//******************************************************************
#endif
//******************************************************************

