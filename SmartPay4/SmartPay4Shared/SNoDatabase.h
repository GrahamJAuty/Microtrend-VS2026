#pragma once

namespace SNoData
{
const recInt	SNo		= { "SNo", 1, 10 };
const recString	SName	= { "Name", 0, 80 };
const recString	TList	= { "TList", 0, 80 };
}

//**********************************************************************

class CSNoDatabase : public CSSIntDatabase
{
public:
	CSNoDatabase(){}
	int OpenForSharing ( CString strFilename = "", int nMode = DB_READONLY );

	CString GetName()	{ return GetString ( SNoData::SName.Label ); }
	CString GetTList()	{ return GetString ( SNoData::TList.Label ); }

	void SetSNo ( int n )					{ CSSDatabase::Set ( SNoData::SNo.Label, n ); }
	void SetName ( CString strName )		{ CSSDatabase::Set ( SNoData::SName.Label, strName); }
	void SetTList ( CString strList )		{ CSSDatabase::Set ( SNoData::TList.Label, strList); }
};

/**********************************************************************/