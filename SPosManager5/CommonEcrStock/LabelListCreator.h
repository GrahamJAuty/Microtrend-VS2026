#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "LabelPriceInfo.h"
/**********************************************************************/
#include "LabelListCSVArray.h"
/**********************************************************************/

class CLabelListCreator 
{
public:
	CLabelListCreator( CLabelListCSVArray& LabelList );   
		
public:
	int CreateLabelList();
	int GetPriceLevel2(){ return m_nPriceLevel2; }
	void GetPriceString( CPluCSVRecord& PluRecord, int nMod, CLabelPriceInfo& info, int nMask );

private:
	CLabelListCSVArray& m_LabelList;

private:
	bool m_bBarcode;
	int m_nTaxType1;
	int m_nTaxType2;
	bool m_bPrintPluNumber;
	bool m_bPrintStockCode;
	bool m_bPrintSuppRef;
	bool m_bHideZeros;
	int m_nPriceLevel1;
	int m_nPriceLevel2;
	CString m_strCurrency1;
	CString m_strCurrency2;
};

/**********************************************************************/
