#pragma once

namespace EposPaymentGroupData
{
const recInt	No		= { "No",	1, 50 };
const recString	Text	= { "Text",	0, 20 };
}

//**********************************************************************

class CEposPaymentGroupDatabase: public CSSIntDatabase
{
public:
	CEposPaymentGroupDatabase();
	int Read();

	int GetNo()			{ return GetInt ( EposPaymentGroupData::No.Label ); }
	CString GetText()	{ return GetString ( EposPaymentGroupData::Text.Label ); }
};

/**********************************************************************/