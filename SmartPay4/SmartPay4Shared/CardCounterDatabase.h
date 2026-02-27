#pragma once

//**********************************************************************

class CCardCounterDatabase : public CSSStringDatabase
{
public:
	CCardCounterDatabase() {}
	int Open ( const char* szFilename );
	bool AddCard ( const char* szCardNo );
};
 