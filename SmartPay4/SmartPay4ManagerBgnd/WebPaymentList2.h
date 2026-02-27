//***********************************************************************************
// Used by Tucasi to convert Tucasi transactionItemNumbers from 9??9 to range 0 - 99
//***********************************************************************************

#pragma once

//**********************************************************************

class CWebPaymentList2Database : public CSSInt64Database
{
public:
	CWebPaymentList2Database() {}
	int Open ( const char* szFilename, int nMode );
	int ConvertItemNo ( __int64 nTransactionNo, const char* szItemNo );
};

//**********************************************************************
