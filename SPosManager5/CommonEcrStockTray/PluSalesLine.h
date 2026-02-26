#pragma once
/**********************************************************************/
#define CSVPLUDATA_PLU_SALE 0
#define CSVPLUDATA_PLU_DISCOUNT 1
#define CSVPLUDATA_PROMO_NONTAXABLE 2
#define CSVPLUDATA_PROMO_TAXABLE 3
#define CSVPLUDATA_PLU_MIXMATCH 4
#define CSVPLUDATA_MIXMATCH_NONTAXABLE 5
#define CSVPLUDATA_PLU_CHEAPESTFREE 6
#define CSVPLUDATA_MIXMATCH_USAGE 7
/**********************************************************************/

class CPluSalesLine
{
public:
	CPluSalesLine( bool bLegacy );
	
public:

	int GetLineType() { return m_csvLine.GetInt(m_nFieldPos[0]); }

	void ParseLine(const char* szLine)
	{
		m_csvLine.ParseLine(szLine);

		if (FALSE == m_bLegacy)
		{
			m_bIsMixMatchUsageLine = (GetLineType() == CSVPLUDATA_MIXMATCH_USAGE);
		}
		else
		{
			m_bIsMixMatchUsageLine = FALSE;
		}
	}

public:
	//FIELD POS 1 WAS OBSOLETE TRAINING MODE
	
	CString GetTranTime()	
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetString(m_nFieldPos[2]);
		}
		else
		{
			return m_csvLine.GetString(1);
		}
	}

	CString GetSaleTime()
	{
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetString(m_nFieldPos[3]);
		}
		else
		{
			return m_csvLine.GetString(1);
		}
	}

	__int64 GetPluNoNew()	
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetInt64(m_nFieldPos[4]);
		}
		else
		{
			return 0;
		}
	}

	int GetPromoNoSeparate()	
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return (int)m_csvLine.GetInt64(m_nFieldPos[5]);
		}
		else
		{
			return 0;
		}
	}

	int GetPromoNoForPlu()	
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetInt(m_nFieldPos[6]);
		}
		else
		{
			return 0;
		}
	}

	int GetPriceBand()		
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetInt(m_nFieldPos[7]);
		}
		else
		{
			return 0;
		}
	}

	double GetSaleQty()
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetDouble(m_nFieldPos[8]);
		}
		else
		{
			return 0.0;
		}
	}

	double GetDiscountQty()	
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetDouble(m_nFieldPos[9]);
		}
		else
		{
			return 0.0;
		}
	}

	double GetValue()		
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetDouble(m_nFieldPos[10]);
		}
		else
		{
			return 0.0;
		}
	}

	CString GetTaxBand()
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetString(m_nFieldPos[11]);
		}
		else
		{
			return "";
		}
	}

	int GetCovers()			
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetInt(m_nFieldPos[12]);
		}
		else
		{
			return m_csvLine.GetInt(2);
		}
	}

	int GetDeptNo()				
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetInt(m_nFieldPos[13]);
		}
		else
		{
			return 0;
		}
	}

	int GetGroupNo()
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetInt(m_nFieldPos[14]);
		}
		else
		{
			return 0;
		}
	}

	int GetAnalysisNo()			
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetInt(m_nFieldPos[15]);
		}
		else
		{
			return 0;
		}
	}

	int GetTranServerNo()		
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetInt(m_nFieldPos[16]);
		}
		else
		{
			return m_csvLine.GetInt(3);
		}
	}

	int GetItemServerNo();	

	int GetCustomerID()			
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetInt(m_nFieldPos[18]);
		}
		else
		{
			return m_csvLine.GetInt(4);
		}
	}

	int GetPluPaymentNo()	
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetInt(m_nFieldPos[19]);
		}
		else
		{
			return 0;
		}
	}

	int GetMixMatchNo()			
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetInt(m_nFieldPos[20]);
		}
		else
		{
			return m_csvLine.GetInt(7);
		}
	}

	int GetMixMatchType()	
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetInt(m_nFieldPos[21]);
		}
		else
		{
			return m_csvLine.GetInt(8);
		}
	}

	bool GetPluSaleWastageFlag()	
	{ 
		if (FALSE == m_bIsMixMatchUsageLine)
		{
			return m_csvLine.GetBool(m_nFieldPos[22]);
		}
		else
		{
			return FALSE;
		}
	}

	int GetDiscountMode();
	CString GetDiscountType();
	int GetDiscountRate();
	int GetVIPStatus();
	int GetTranSeqNo();
	CString GetCashRSPText();
	int GetTranTableNo();
	CString GetAttributeText1();
	CString GetMixMatchUseID();
	int GetMixMatchUseCount();
	__int64 GetLoyaltyIDAsInt64();
	CString GetLoyaltyIDAsString();
	int GetLineTNo();
	bool GetPluSaleRefundFlag();
	bool GetPluSaleVoidFlag();
	
public:
	int GetNumericTaxBand();
	bool IsVIPSale();
	bool IsPremium();
	
private:
	CCSV m_csvLine;
	int m_nFieldPos[23];
	bool m_bLegacy;
	bool m_bIsMixMatchUsageLine;
};

/**********************************************************************/