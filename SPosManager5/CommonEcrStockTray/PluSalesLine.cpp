/**********************************************************************/
#include "PriceHelpers.h"
/**********************************************************************/
#include "PluSalesLine.h"
/**********************************************************************/

CPluSalesLine::CPluSalesLine(bool bLegacy) : m_csvLine( "" )
{
	m_bLegacy = bLegacy;
	m_bIsMixMatchUsageLine = FALSE;

	if ( FALSE == bLegacy )
	{
		for (int n = 0; n <= 5; n++)
		{
			m_nFieldPos[n] = n;
		}

		for (int n = 6; n <= 22; n++)
		{
			m_nFieldPos[n] = n - 1;
		}
	}
	else
	{
		m_nFieldPos[0] = 8;		//LINE TYPE
		m_nFieldPos[1] = 7;		//OBSOLETE TRAINING MODE
		m_nFieldPos[2] = 0;		//TRAN TIME
		m_nFieldPos[3] = 0;		//SALE TIME (MAPPED TO TRAN TIME)
		m_nFieldPos[4] = 2;		//PLU NO
		m_nFieldPos[5] = 2;		//PROMO NO WHEN REPORTED SEPARATELY (MAPPED TO PLU FIELD)
		m_nFieldPos[6] = 20;	//PROMO NO WHEN USED WITH PLU SALE OR DISCOUNT
		m_nFieldPos[7] = 3;		//PRICE BAND
		m_nFieldPos[8] = 4;		//SALE QTY
		m_nFieldPos[9] = 21;	//DISCOUNT QTY
		m_nFieldPos[10] = 5;	//VALUE
		m_nFieldPos[11] = 14;	//TAX BAND
		m_nFieldPos[12] = 13;	//COVERS
		m_nFieldPos[13] = 6;	//DEPT NO
		m_nFieldPos[14] = 12;	//GROUP NO
		m_nFieldPos[15] = 17;	//ANALYSIS CATEGORY
		m_nFieldPos[16] = 1;	//TRANSACTION SERVER
		m_nFieldPos[17] = 16;	//ITEM SERVER
		m_nFieldPos[18] = 15;	//CUSTOMER
		m_nFieldPos[19] = 9;	//PAYMENT NO
		m_nFieldPos[20] = 18;	//MIX MATCH NO
		m_nFieldPos[21] = 19;	//MIX MATCH TYPE
		m_nFieldPos[22] = 10;	//WASTAGE
	}
}

/**********************************************************************/

int CPluSalesLine::GetNumericTaxBand()
{
	return CTaxArray::GetNumericTaxBand(GetTaxBand(), TRUE, 0, -1);
}

/**********************************************************************/

int CPluSalesLine::GetItemServerNo()
{
	int nServerNo = 0;

	if (TRUE == m_bIsMixMatchUsageLine)
	{
		nServerNo = m_csvLine.GetInt(3);
	}
	else
	{
		nServerNo = m_csvLine.GetInt(m_nFieldPos[17]);

		if ((0 == nServerNo) && (DealerFlags.GetCashRSPFixItemServerZeroFlag() == TRUE))
		{
			nServerNo = GetTranServerNo();
		}
	}

	return nServerNo;
}

/**********************************************************************/

int CPluSalesLine::GetDiscountMode()
{
	//0 = UNKNOWN
	//1 = ITEM
	//2 = SUBTOTAL
	//3 = LOYALTY
	//4 = DIFFER
	//5 = MIXMATCH
	if ((TRUE == m_bLegacy) || (TRUE == m_bIsMixMatchUsageLine))
	{
		return 0;
	}
	else
	{
		return m_csvLine.GetInt(22);
	}
}

/**********************************************************************/

CString CPluSalesLine::GetDiscountType()
{
	if ((TRUE == m_bLegacy) || (TRUE == m_bIsMixMatchUsageLine))
	{
		return "";
	}
	else
	{
		return m_csvLine.GetString(23);
	}
}

/**********************************************************************/

bool CPluSalesLine::IsPremium()
{
	CString str = GetDiscountType();
	return (str == "P");
}

/**********************************************************************/

int CPluSalesLine::GetDiscountRate()
{
	if ((TRUE == m_bLegacy) || (TRUE == m_bIsMixMatchUsageLine))
	{
		return 0;
	}
	else
	{
		return m_csvLine.GetInt(24);
	}
}

/**********************************************************************/

int CPluSalesLine::GetVIPStatus()
{
	if ((TRUE == m_bLegacy) || (TRUE == m_bIsMixMatchUsageLine))
	{
		return 0;
	}
	else
	{
		return m_csvLine.GetInt(25);
	}
}

/**********************************************************************/

bool CPluSalesLine::IsVIPSale()
{
	bool bResult = FALSE;

	switch (GetVIPStatus())
	{
	case 1:
		bResult = FALSE;
		break;

	case 2:
		bResult = TRUE;
		break;

	default:
		{
			bResult = FALSE;

			int nLegacyLevel = EcrmanOptions.GetReportsVIPLevel();

			if (nLegacyLevel > 0)
			{
				int nThisSaleLevel = GetPriceBand() + 1;

				if (nThisSaleLevel == nLegacyLevel)
				{
					if (EcrmanOptions.GetReportsVIPType() != 2)
					{
						bResult = TRUE;
					}
					else
					{
						bResult = (CPriceHelpers::CompareDoubles(GetValue(), 0.0, 3) == 0);
					}
				}
			}
		}
		break;
	}

	return bResult;
}

/**********************************************************************/

int CPluSalesLine::GetTranSeqNo()
{
	if (TRUE == m_bLegacy)
	{
		return 0;
	}
	else if (TRUE == m_bIsMixMatchUsageLine)
	{
		return m_csvLine.GetInt(5);
	}
	else
	{
		return m_csvLine.GetInt(26);
	}
}

/**********************************************************************/

CString CPluSalesLine::GetCashRSPText()
{
	if ((TRUE == m_bLegacy) || (TRUE == m_bIsMixMatchUsageLine))
	{
		return "";
	}
	else
	{
		return m_csvLine.GetString(27);
	}
}

/**********************************************************************/

int CPluSalesLine::GetTranTableNo()
{
	if (TRUE == m_bLegacy)
	{
		return 0;
	}
	else if (TRUE == m_bIsMixMatchUsageLine)
	{
		return m_csvLine.GetInt(6);
	}
	else
	{
		return m_csvLine.GetInt(28);
	}
}

/**********************************************************************/

CString CPluSalesLine::GetAttributeText1()
{
	if ((TRUE == m_bLegacy) || (TRUE == m_bIsMixMatchUsageLine))
	{
		return "";
	}
	else
	{
		return m_csvLine.GetString(29);
	}
}

/**********************************************************************/

CString CPluSalesLine::GetMixMatchUseID()
{
	if (FALSE == m_bIsMixMatchUsageLine)
	{
		return "";
	}
	else
	{
		return m_csvLine.GetString(9);
	}
}

/**********************************************************************/

int CPluSalesLine::GetMixMatchUseCount()
{
	if (FALSE == m_bIsMixMatchUsageLine)
	{
		return 0;
	}
	else
	{
		return m_csvLine.GetInt(10);
	}
}

/**********************************************************************/

__int64 CPluSalesLine::GetLoyaltyIDAsInt64()
{
	if ((TRUE == m_bLegacy) || (TRUE == m_bIsMixMatchUsageLine))
	{
		return 0;
	}
	else
	{
		return m_csvLine.GetInt64(30);
	}
}

/**********************************************************************/

CString CPluSalesLine::GetLoyaltyIDAsString()
{
	CString strResult = "";

	if ((FALSE == m_bLegacy) && (FALSE == m_bIsMixMatchUsageLine))
	{
		strResult = m_csvLine.GetString(30);
	}

	return strResult;
}

/**********************************************************************/

int CPluSalesLine::GetLineTNo()
{
	if ((TRUE == m_bLegacy) || (TRUE == m_bIsMixMatchUsageLine))
	{
		return 0;
	}
	else
	{
		return m_csvLine.GetInt(31);
	}
}

/**********************************************************************/

bool CPluSalesLine::GetPluSaleRefundFlag()
{
	if ((TRUE == m_bLegacy) || (TRUE == m_bIsMixMatchUsageLine))
	{
		return FALSE;
	}
	else
	{
		return m_csvLine.GetBool(32);
	}
}

/**********************************************************************/

bool CPluSalesLine::GetPluSaleVoidFlag()
{
	if ((TRUE == m_bLegacy) || (TRUE == m_bIsMixMatchUsageLine))
	{
		return FALSE;
	}
	else
	{
		return m_csvLine.GetBool(33);
	}
}

/**********************************************************************/
