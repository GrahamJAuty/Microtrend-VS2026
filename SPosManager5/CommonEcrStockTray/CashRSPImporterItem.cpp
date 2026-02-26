/**********************************************************************/
#include "CashRSPImporterItem.h"
/**********************************************************************/

CCashRSPImporterItem::CCashRSPImporterItem() 
{
	m_nPluNo = 0;
	m_strPluNo = "";
	m_bVoid = FALSE;
	m_bRefund = FALSE;
	m_bWastageVoid = FALSE;
	m_b15Digit = FALSE;
	m_bRA = FALSE;
	m_nAccountType = 0;
	m_nAccountID = 0;
	m_nPriceBand = 0;
	m_nDeptNo = 0;
	m_nGroupNo = 0;
	m_strTaxBand = "";
	m_nServerNo = 0;
	m_dQty = 0.0;
	m_nLineValueAsInt = 0;
	m_dLineValueAsDouble = 0.0;
	m_nACatNo = 0;
	m_nMixMatchNo = 0;
	m_nMixMatchType = 0;
	m_strSaleDate = "";
	m_strSaleTime = "";
	m_strVoidBAType = "";
	m_nAuthServerNo = 0;
	m_nVIPStatus = 0;
	m_nPromoExportType = 0;
	m_nReasonCode = 0;
	m_strReasonText = "";
	m_nWastageCode = 0;
	m_strWastageText = "";
	m_nTranSeqNo = 0;
	m_strDescription = "";
	m_strAttributeText = "";
}

/**********************************************************************/

const char* CCashRSPImporterItem::GetPluNoString()
{
	m_strPluNo.Format( "%I64d", m_nPluNo );
	return m_strPluNo;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
