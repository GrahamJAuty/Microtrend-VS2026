#pragma once
//*******************************************************************

//******************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//******************************************************************

class CImportFlag
{
public:
	CImportFlag(){}

public:
	void SetImportFlag( bool b ){}
	bool GetImportFlag(){ return TRUE; }
};

//******************************************************************
#else
//******************************************************************

class CImportFlag
{
public:
	CImportFlag(){ m_bImportFlag = FALSE; }

public:
	void SetImportFlag( bool b ){ m_bImportFlag = b; }
	bool GetImportFlag(){ return m_bImportFlag; }

private:
	bool m_bImportFlag;
};

//******************************************************************
#endif
//******************************************************************



