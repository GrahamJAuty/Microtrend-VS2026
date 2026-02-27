#pragma once
/**********************************************************************/

class CMyCSSLockFile : public CSSFile
{
public:
	bool OpenLock( const char* szFilename ){ return CSSFile::Open( szFilename, "wb", _SH_DENYWR ); }
};

/**********************************************************************/
