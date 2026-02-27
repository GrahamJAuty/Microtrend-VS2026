//**********************************************************************
#include "SQLRowImportFilenameCache.h"
//**********************************************************************

CSQLRowImportFilenameCache::CSQLRowImportFilenameCache()
{
	Reset();
}

//**********************************************************************

void CSQLRowImportFilenameCache::Reset()
{
	m_nLineID = 1;
	m_nType = 0;
	m_strFilename = "";
	m_strDateTime = "";
}

//**********************************************************************

