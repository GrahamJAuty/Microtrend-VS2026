//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorImportFilenameCache.h"
//**********************************************************************

void CSQLFieldValidatorImportFilenameCache::SetLineID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, ImportFilenameCache::LineID);
}

//**********************************************************************

void CSQLFieldValidatorImportFilenameCache::SetType(int n, int& nTarget)
{
	SetInt(n, nTarget, ImportFilenameCache::Type);
}

//**********************************************************************

void CSQLFieldValidatorImportFilenameCache::SetFilename(CString str, CString& strTarget)
{
	SetString(str, strTarget, ImportFilenameCache::Filename);
}

//**********************************************************************

void CSQLFieldValidatorImportFilenameCache::SetDateTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, ImportFilenameCache::DateTime);
}

//**********************************************************************

