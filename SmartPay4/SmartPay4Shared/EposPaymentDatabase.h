#pragma once

namespace EposPaymentData
{
const recInt	No		= { "No",	1, 50 };
const recString	Text	= { "Text",	0, 20 };
const recInt	Group	= { "Group",1, 9 };
}

//**********************************************************************

class CEposPaymentDatabase: public CSSIntDatabase
{
public:
	CEposPaymentDatabase();
	int Read();

	int GetNo()			{ return GetInt ( EposPaymentData::No.Label ); }
	CString GetText()	{ return GetString ( EposPaymentData::Text.Label ); }
	int GetGroup()		{ return GetInt ( EposPaymentData::Group.Label ); }
};

/**********************************************************************/