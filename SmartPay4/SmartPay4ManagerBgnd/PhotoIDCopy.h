#pragma once
//******************************************************************************

class CPhotoIDCopy
{
public:
	CPhotoIDCopy();

public:
	bool CopyPhotoFile ( const char* strSourcePath, const char* szCardNo );
	int Import();
	int EposExport();

private:
	bool Copy ( const char* szSourcePath, const char* szDestPathname );
};

//******************************************************************************
