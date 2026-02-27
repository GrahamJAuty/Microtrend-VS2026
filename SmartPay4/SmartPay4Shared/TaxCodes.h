#pragma once
//$$******************************************************************
#define nMAX_TAXCODES 12
//$$******************************************************************

struct Tax
{
	double dValue = 0;
	CString strText = "";
};

//$$******************************************************************

class CTaxCodes
{
public:
	CTaxCodes(void);
	bool Read();

	double GetTaxRate ( int nIndex );
	CString GetTaxText ( int nIndex );
	double CalcVatContent ( double dValue, int nIndex );

private:
	Tax m_tax[nMAX_TAXCODES];
};

//$$******************************************************************
