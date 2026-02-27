#pragma once

namespace TNoData
{
const recInt	TNo	= { "TNo", 1, 16 };
const recInt	SNo	= { "SNo", 0, 10 };
}

//**********************************************************************

class CTNoDatabase : public CSSIntDatabase
{
public:
	CTNoDatabase(){}
	
public:	
	int OpenForSharing ( CString strFilename = "", int nMode = DB_READONLY );

	int GetTNo()		{ return GetInt ( TNoData::TNo.Label ); }
	int GetSetNo()		{ return GetInt ( TNoData::SNo.Label ); }

	void SetSetNo ( int n )	{ CSSDatabase::Set ( TNoData::SNo.Label, n ); }
};

/**********************************************************************/