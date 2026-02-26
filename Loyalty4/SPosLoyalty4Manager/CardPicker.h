#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************

class CCardPicker
{
public:
	CCardPicker();
	void Reset();
	bool IsValid ( const char* szCardNo, const char* szFilename );

private:
	bool FindCardNo ( const char* szCardNo, int& nIndex );

private:
	CStringArray m_array;
};
