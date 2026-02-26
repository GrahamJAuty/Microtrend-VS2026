/**********************************************************************/
#include "CashRSPImporter.h"
/**********************************************************************/
#include "CashRSPCSVLines.h"
/**********************************************************************/

CCSVEposTermLine::CCSVEposTermLine(const char* szCSVLine) : CCSV(szCSVLine)
{
	m_nApplicationType = 0;
	m_nVersionNo = 0;
	m_strField = "";
	m_strSubtotalDiscountAnalysisCategories = "";
}

/**********************************************************************/

//CHECK FOR LINE TYPES IN (ROUGHLY ESTIMATED) ORDER OF THEIR
//EXPECTED FREQUENCY 

int CCSVEposTermLine::GetLineTypeInt()
{
	CString strType = GetLineTypeString();

	int nLength = strType.GetLength();

	if (strType.Left(16) == "$CASHRSP_VERSION")
	{
		if ((nLength >= 18) && (nLength <= 20))
		{
			m_nVersionNo = atoi(strType.Right(nLength - 17));
		}
		else
		{
			m_nVersionNo = 0;
		}

		return CASHRSP_LINETYPE_VERSION;
	}

	if (strType.Left(5) == "$APPL")
	{
		if ((nLength >= 7) && (nLength <= 9))
			m_nApplicationType = atoi(strType.Right(nLength - 6));
		else
			m_nApplicationType = 0;

		return CASHRSP_LINETYPE_APPLICATION;
	}

	if (strType == "$ITEM")
	{
		return CASHRSP_LINETYPE_ITEM;
	}

	if (strType == "$PAYM")
	{
		return CASHRSP_LINETYPE_PAYMENT;
	}

	if (strType == "$TRAN")
	{
		return CASHRSP_LINETYPE_TRAN_NORMAL;
	}

	if (strType == "#PMS_TRAN")
	{
		return CASHRSP_LINETYPE_TRAN_PMS;
	}

	if (strType == "#PMS_DEPO")
	{
		return CASHRSP_LINETYPE_DEPOSIT_PMS;
	}

	if (strType == "$IDISC")
	{
		return CASHRSP_LINETYPE_DISCOUNT_ITEM;
	}

	if ((strType == "$SDISC") || (strType == "$SS_DISC"))
	{
		return CASHRSP_LINETYPE_DISCOUNT_SUBTOTAL;
	}

	if (strType == "$LDISC")
	{
		return CASHRSP_LINETYPE_DISCOUNT_LOYALTY;
	}

	if (strType == "$PDISC")
	{
		return CASHRSP_LINETYPE_DISCOUNT_PROMO;
	}

	if (strType == "$MMDISC")
	{
		return CASHRSP_LINETYPE_DISCOUNT_MIXMATCH;
	}

	if (strType == "$MMUSE")
	{
		return CASHRSP_LINETYPE_MIXMATCH_USE;
	}

	if (strType == "$SVDISC")
	{
		return CASHRSP_LINETYPE_DISCOUNTTAX_SUBTOTAL;
	}

	if (strType == "$LVDISC")
	{
		return CASHRSP_LINETYPE_DISCOUNTTAX_LOYALTY;
	}

	if (strType == "$VDDISC")
	{
		return CASHRSP_LINETYPE_DISCOUNT_VOID;
	}

	if (strType == "$AGEVERIFY")
	{
		return CASHRSP_LINETYPE_AGEVERIFY;
	}

	if (strType == "$VTRN")
	{
		return CASHRSP_LINETYPE_TRAN_VOID;
	}

	if (strType == "$NOSALE")
	{
		return CASHRSP_LINETYPE_NOSALE;
	}

	if (strType == "$WAST")
	{
		return CASHRSP_LINETYPE_WASTAGE;
	}

	if (strType == "$ATTENDANCE")
	{
		return CASHRSP_LINETYPE_ATTENDANCE;
	}

	if (strType == "$PAIDIO")
	{
		return CASHRSP_LINETYPE_PAIDIO;
	}

	if (strType == "$PMS_COMMENT")
	{
		return CASHRSP_LINETYPE_COMMENT_PMS;
	}

	if (strType == "$PMS_HEADER")
	{
		return CASHRSP_LINETYPE_HEADER_PMS;
	}

	if (strType == "$")
	{
		return CASHRSP_LINETYPE_COMMENT_GENERAL;
	}

	if (strType == "$SSFT_DELIVERY")
	{
		return CASHRSP_LINETYPE_SSFT_DELIVERY;
	}

	if (strType == "$DEPOSIT")
	{
		return CASHRSP_LINETYPE_DEPOSITSTART;
	}

	if (strType == "$DEPOSITEND")
	{
		return CASHRSP_LINETYPE_DEPOSITEND;
	}

	if (strType.Left(3) == "EOD")
	{
		return CASHRSP_LINETYPE_EODHEADER;
	}

	if (strType == "$VAR")
	{
		return CASHRSP_LINETYPE_EODVAR;
	}

	if (strType == "$VARTOT")
	{
		return CASHRSP_LINETYPE_EODVARTOT;
	}

	if (strType == "$VARFX")
	{
		return CASHRSP_LINETYPE_EODVARFX;
	}

	if (strType == "$VARFXTOT")
	{
		return CASHRSP_LINETYPE_EODVARFXTOT;
	}

	if (strType == "$EODEND")
	{
		return CASHRSP_LINETYPE_EODEND;
	}

	return CASHRSP_LINETYPE_UNKNOWN;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

const char* CCSVEposTermLine::GetTransactionTradingDate( int nVersionNo )
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 23, FALSE) == TRUE)
	{
		m_strField = GetString(10);
	}

	return m_strField;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetTransactionEmail(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 24, FALSE) == TRUE)
	{
		m_strField = GetString(11);
	}

	return m_strField;
}

/**********************************************************************/

int CCSVEposTermLine::GetTransactionReasonCode(int nVersionNo)
{
	int nCode = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 26, FALSE) == TRUE)
	{
		nCode = GetInt(12);
	}

	return nCode;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetTransactionReasonText(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 26, FALSE) == TRUE)
	{
		m_strField = GetString(13);
	}

	return m_strField;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetTransactionPaidIOLabel(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 30, FALSE) == TRUE)
	{
		m_strField = GetString(14);
	}
	else if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 27, FALSE) == TRUE)
	{
		CString str = GetLineTypeString();

		if (str == "$VTRN")
		{
			m_strField = GetString(14);
		}
		else
		{
			m_strField = GetString(15);
		}
	}

	return m_strField;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetTransactionReverseLabel(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 30, FALSE) == TRUE)
	{
		m_strField = GetString(15);
	}
	else if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 29, FALSE) == TRUE)
	{
		CString str = GetLineTypeString();

		if (str == "$VTRN")
		{
			m_strField = GetString(15);
		}
		else
		{
			m_strField = GetString(16);
		}
	}

	return m_strField;
}

/**********************************************************************/

__int64 CCSVEposTermLine::GetTransactionLoyaltyIDAsInt64(int nVersionNo)
{
	__int64 nResult = FALSE;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 43, FALSE) == TRUE)
	{
		nResult = GetInt64(19);
	}
	
	return nResult;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetTransactionLoyaltyIDAsString(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 43, FALSE) == TRUE)
	{
		m_strField = GetString(19);
	}

	return m_strField;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CCSVEposTermLine::GetItemTypeAndID(CCashRSPVersionChecker& VersionChecker, int& nType, int& nID32, __int64& nID64)
{
	nType = CASHRSP_ITEMPAYTYPE_NORMAL;
	nID32 = 0;
	nID64 = 0;

	int nVersionNo = VersionChecker.GetCurrentTransactionVersionNo();
	int nAppType = VersionChecker.GetCurrentTransactionAppType();

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 14, FALSE) == TRUE)
	{
		nID32 = GetInt(17);
		nID64 = GetInt64(17);

		switch (GetInt(16))
		{
		case 1:
			nType = CASHRSP_ITEMPAYTYPE_CUSTOMER;
			break;

		case 3:
			nType = CASHRSP_ITEMPAYTYPE_DEPOSIT;
			break;

		case 4:
			nType = CASHRSP_ITEMPAYTYPE_ROOM;
			break;

		case 6:
			nType = CASHRSP_ITEMPAYTYPE_SPTBOOK;
			break;

		case 5:

			nType = CASHRSP_ITEMPAYTYPE_NORMAL;

			if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 21, FALSE) == TRUE)
			{
				switch (nAppType)
				{
				case 2:
					nType = CASHRSP_ITEMPAYTYPE_SMARTPAY;
					break;

				case 1:
					if (GetItemProductType(nVersionNo) == 32)
					{
						nType = CASHRSP_ITEMPAYTYPE_SMARTPHONE;
					}
					else
					{
						nType = CASHRSP_ITEMPAYTYPE_LOYALTY;
					}
					break;
				}
			}
			else if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 19, FALSE) == TRUE)
			{
				if (DealerFlags.GetCashRSPV19SmartPayFlag() == TRUE)
				{
					nType = CASHRSP_ITEMPAYTYPE_SMARTPAY;
				}
			}

			break;

		case 0:
		default:
			if (GetItemProductType(nVersionNo) == 34)
			{
				switch (nAppType)
				{
				case 2:
					nType = CASHRSP_ITEMPAYTYPE_SMARTPAY;
					break;

				case 1:
					nType = CASHRSP_ITEMPAYTYPE_LOYALTY;
					break;

				default:
					nType = CASHRSP_ITEMPAYTYPE_NORMAL;
					break;
				}
			}
			else
			{
				nType = CASHRSP_ITEMPAYTYPE_NORMAL;
			}
			break;
		}
	}
	else
	{
		nID32 = GetInt(14);
		nID64 = nID32;

		if (nID32 != 0)
		{
			nType = CASHRSP_ITEMPAYTYPE_DEPOSIT;
		}
	}
}

/**********************************************************************/

int CCSVEposTermLine::GetItemServerNo(int nVersionNo)
{
	int nServerNo = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 16, FALSE) == TRUE)
	{
		nServerNo = GetInt(18);
	}

	return nServerNo;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemAnalysisCategory(int nVersionNo)
{
	int nCategory = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 17, FALSE) == TRUE)
	{
		nCategory = GetInt(19);
	}

	return nCategory;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemMixMatchNumber(int nVersionNo)
{
	int nNumber = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 18, FALSE) == TRUE)
	{
		nNumber = GetInt(20);
	}

	return nNumber;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemMixMatchType(int nVersionNo)
{
	int nType = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 18, FALSE) == TRUE)
	{
		nType = GetInt(21);
	}

	return nType;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetItemVoidBAType(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 19, FALSE) == TRUE)
	{
		m_strField = GetString(22);
	}

	return m_strField;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetItemSaleDate(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 19, FALSE) == TRUE)
	{
		m_strField = GetString(23);
	}

	return m_strField;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetItemSaleTime( int nVersionNo )
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 19, FALSE) == TRUE)
	{
		m_strField = GetString(24);
	}

	return m_strField;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemGeneralReasonCode(int nVersionNo)
{
	int nCode = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 19, FALSE) == TRUE)
	{
		nCode = GetInt(25);
	}

	return nCode;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetItemGeneralReasonText(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 19, FALSE) == TRUE)
	{
		m_strField = GetString(26);
	}

	return m_strField;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetItemDescription(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 20, FALSE) == TRUE)
	{
		CCSV csv(GetString(27), ',', '\'');
		m_strField = csv.GetString(0);
	}

	return m_strField;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemProductType(int nVersionNo)
{
	int nType = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 21, FALSE) == TRUE)
	{
		nType = GetInt(28);
	}

	return nType;
}

/**********************************************************************/

bool CCSVEposTermLine::GetItemWastageVoidFlag(int nVersionNo)
{
	bool bResult = FALSE;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 22, FALSE) == TRUE)
	{
		CString str = GetString(29);
		bResult = (str == "W");
	}

	return bResult;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemVIPStatus(int nVersionNo)
{
	int nStatus = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 26, FALSE) == TRUE)
	{
		CString str = GetString(30);
		nStatus = (str == "V") ? 2 : 1;
	}

	return nStatus;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemWastageReasonCode(int nVersionNo)
{
	int nCode = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 26, FALSE) == TRUE)
	{
		nCode = GetInt(31);
	}

	return nCode;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetItemWastageReasonText(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 26, FALSE) == TRUE)
	{
		m_strField = GetString(32);
	}

	return m_strField;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemPromoExportType(int nVersionNo)
{
	int nType = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 26, FALSE) == TRUE)
	{
		nType = GetInt(33);
	}

	return nType;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemTerminalNo(int nVersionNo)
{
	int nTerminalNo = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 28, FALSE) == TRUE)
	{
		nTerminalNo = GetInt(34);
	}

	return nTerminalNo;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemAuthServerNo(int nVersionNo)
{
	int nServerNo = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 35, FALSE) == TRUE)
	{
		nServerNo = GetInt(35);
	}

	return nServerNo;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetItemFreeText(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 38, FALSE) == TRUE)
	{
		m_strField = GetString(36);
	}

	return m_strField;
}

/**********************************************************************/

double CCSVEposTermLine::GetItemLineValueReport()
{
	double dValue = 0.0;

	if (GetSize() >= 13)
	{
		dValue = (double)GetItemLinePrice();
	}
	else
	{
		dValue = (double)GetItemUnitPrice();
		dValue *= GetItemQty();
	}

	dValue /= 100.0;
	return dValue;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

int CCSVEposTermLine::GetItemDiscountServerNo(int nVersionNo)
{
	int nServerNo = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 16, FALSE) == TRUE)
	{
		nServerNo = GetInt(8);
	}

	return nServerNo;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemDiscountPromoNo(int nVersionNo)
{
	int nPromoNo = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 17, FALSE) == TRUE)
	{
		nPromoNo = GetInt(9);
	}

	return nPromoNo;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemDiscountAnalysisCategory(int nVersionNo)
{
	int nCategory = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 17, FALSE) == TRUE)
	{
		nCategory = GetInt(10);
	}

	return nCategory;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetItemDiscountDescription(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 20, FALSE) == TRUE)
	{
		m_strField = GetString(11);
	}

	return m_strField;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemDiscountReasonCode(int nVersionNo)
{
	int nCode = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 26, FALSE) == TRUE)
	{
		nCode = GetInt(12);
	}

	return nCode;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetItemDiscountReasonText(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 26, FALSE) == TRUE)
	{
		m_strField = GetString(13);
	}

	return m_strField;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemDiscountAuthorisedServerNo(int nVersionNo)
{
	int nServerNo = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 28, FALSE) == TRUE)
	{
		nServerNo = GetInt(14);
	}

	return nServerNo;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetItemDiscountDate(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 28, FALSE) == TRUE)
	{
		m_strField = GetString(15);
	}

	return m_strField;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetItemDiscountTime(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 28, FALSE) == TRUE)
	{
		m_strField = GetString(16);
	}

	return m_strField;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemDiscountTerminalNo(int nVersionNo)
{
	int nTerminalNo = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 28, FALSE) == TRUE)
	{
		nTerminalNo = GetInt(17);
	}

	return nTerminalNo;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetItemDiscountAppliedAs(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 28, FALSE) == TRUE)
	{
		m_strField = GetString(18);
	}

	return m_strField;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemDiscountPluCost(int nVersionNo)
{
	int nPluCost = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 28, FALSE) == TRUE)
	{
		nPluCost = GetInt(19);
	}

	return nPluCost;
}

/**********************************************************************/

int CCSVEposTermLine::GetItemDiscountPluQty(int nVersionNo)
{
	int nPluQty = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 28, FALSE) == TRUE)
	{
		nPluQty = GetInt(20);
	}

	return nPluQty;
}

/**********************************************************************/

bool CCSVEposTermLine::GetItemDiscountCashRSPTaxFlag(int nVersionNo)
{
	bool bResult = FALSE;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 28, FALSE) == TRUE)
	{
		bResult = GetBool(21);
	}

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CCSVEposTermLine::GetSubtotalDiscountMicrotrendTaxFlag()
{
	if (GetSubtotalDiscountCashRSPTaxFlag() == TRUE)
	{
		return TRUE;
	}

	if (DealerFlags.GetAllDiscountsVATableFlag() == TRUE)
	{
		CString strType = GetSubtotalDiscountType();
		return (strType != "P");
	}

	return FALSE;
}


/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CCSVEposTermLine::GetPromoDiscountMicrotrendTaxFlag()
{
	if (GetPromoDiscountCashRSPTaxFlag() == TRUE)
	{
		return TRUE;
	}

	if (DealerFlags.GetAllDiscountsVATableFlag() == TRUE)
	{
		CString strType = GetPromoDiscountType();
		return (strType != "P");
	}

	return FALSE;
}


/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CCSVEposTermLine::GetPaymentAccountTypeAndID(CCashRSPVersionChecker& VersionChecker, int& nType, int& nID32, __int64& nID64, bool& bIsCash)
{
	nType = CASHRSP_ITEMPAYTYPE_NORMAL;
	nID32 = 0;
	nID64 = 0;
	bIsCash = (GetPaymentNumber() == 1);

	int nVersionNo = VersionChecker.GetCurrentTransactionVersionNo();
	int nAppType = VersionChecker.GetCurrentTransactionAppType();

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 14, FALSE) == TRUE)
	{
		nID32 = GetInt(9);
		nID64 = GetInt64(9);

		switch (GetInt(8))
		{
		case 10:
			nType = CASHRSP_ITEMPAYTYPE_CUSTOMER;
			break;

		case 4:
		case 5:
			nType = CASHRSP_ITEMPAYTYPE_ROOM;
			break;

		case 8:
			nType = CASHRSP_ITEMPAYTYPE_DEPOSIT;
			break;

		case 6:

			nType = CASHRSP_ITEMPAYTYPE_NORMAL;

			if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 21, FALSE) == TRUE)
			{
				switch (nAppType)
				{
				case 1:
					nType = CASHRSP_ITEMPAYTYPE_LOYALTY;
					break;

				case 2:
					nType = CASHRSP_ITEMPAYTYPE_SMARTPAY;
					break;
				}
			}
			else if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 19, FALSE) == TRUE)
			{
				if (DealerFlags.GetCashRSPV19SmartPayFlag() == TRUE)
				{
					nType = CASHRSP_ITEMPAYTYPE_SMARTPAY;
				}
			}
			break;

		case 13:
			nType = CASHRSP_ITEMPAYTYPE_SMARTPHONE;
			break;

		case 20:
			nType = CASHRSP_ITEMPAYTYPE_SPTBOOK;
			break;

		case 0:
			bIsCash = TRUE;
			nType = CASHRSP_ITEMPAYTYPE_NORMAL;
			break;

		default:
			nType = CASHRSP_ITEMPAYTYPE_NORMAL;
			break;
		}
	}
	else
	{
		nID32 = GetInt(7);
		nID64 = nID32;

		if (nID32 != 0)
		{
			nType = CASHRSP_ITEMPAYTYPE_DEPOSIT;
		}
	}
}

/**********************************************************************/

int CCSVEposTermLine::GetPaymentTerminalNo(int nVersionNo)
{
	int nTerminalNo = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 28, FALSE) == TRUE)
	{
		nTerminalNo = GetInt(10);
	}

	return nTerminalNo;
}

/**********************************************************************/

int CCSVEposTermLine::GetPaymentSurplus(int nVersionNo)
{
	int nSurplus = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 31, FALSE) == TRUE)
	{
		nSurplus = GetInt(11);
	}

	return nSurplus;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

const char* CCSVEposTermLine::GetSubtotalDiscountAnalysisCategories(int nVersionNo)
{
	m_strSubtotalDiscountAnalysisCategories = "0";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 17, FALSE) == TRUE)
	{
		m_strSubtotalDiscountAnalysisCategories = GetString(6);
	}

	return m_strSubtotalDiscountAnalysisCategories;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

const char* CCSVEposTermLine::GetVoidDiscountBAType(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 19, FALSE) == TRUE)
	{
		m_strField = GetString(6);
	}

	return m_strField;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetVoidDiscountReasonDate(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 19, FALSE) == TRUE)
	{
		m_strField = GetString(7);
	}

	return m_strField;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetVoidDiscountReasonTime(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 19, FALSE) == TRUE)
	{
		m_strField = GetString(8);
	}

	return m_strField;
}

/**********************************************************************/

int CCSVEposTermLine::GetVoidDiscountReasonCode(int nVersionNo)
{
	int nCode = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 19, FALSE) == TRUE)
	{
		nCode = GetInt(9);
	}

	return nCode;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetVoidDiscountReasonText(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 19, FALSE) == TRUE)
	{
		m_strField = GetString(10);
	}

	return m_strField;
}

/**********************************************************************/

const char* CCSVEposTermLine::GetVoidDiscountDescription(int nVersionNo)
{
	m_strField = "";

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 20, FALSE) == TRUE)
	{
		m_strField = GetString(11);
	}

	return m_strField;
}

/**********************************************************************/

int CCSVEposTermLine::GetVoidDiscountAuthServerNo(int nVersionNo)
{
	int nServer = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 40, FALSE) == TRUE)
	{
		nServer = GetInt(12);
	}

	return nServer;
}

/**********************************************************************/

int CCSVEposTermLine::GetMixMatchDiscountPromoNo(int nVersionNo)
{
	int nPromoNo = 0;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 33, FALSE) == TRUE)
	{
		nPromoNo = GetInt(11);
	}

	return nPromoNo;
}

/**********************************************************************/

int CCSVEposTermLine::GetMixMatchDiscountNSplit(int nVersionNo)
{
	int nSplit = 1;

	if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 38, FALSE) == TRUE)
	{
		nSplit = GetInt(12);
	}

	return nSplit;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

int CCSVPMSTermLine::GetItemNumericTaxBand()
{
	return CTaxArray::GetNumericTaxBand(GetItemTaxBand(), TRUE);
}

/**********************************************************************/

void CCSVPMSTermLine::SetItemNumericTaxBand(int n)
{
	CString strTaxBand = CTaxArray::GetTaxBandFromNumber(n, 0, "?", "");

	if (strTaxBand != "?")
	{
		SetAt(6, strTaxBand);
	}
}

/**********************************************************************/

double CCSVPMSTermLine::GetItemLineValueReport()
{
	double dValue = 0.0;

	if (GetSize() >= 13)
	{
		dValue = (double)GetItemLinePrice();
	}
	else
	{
		dValue = (double)GetItemUnitPrice();
		dValue *= GetItemQty();
	}

	dValue /= 100.0;
	return dValue;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

