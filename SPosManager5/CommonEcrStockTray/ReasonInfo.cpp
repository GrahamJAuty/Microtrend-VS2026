/**********************************************************************/
 
/**********************************************************************/
#include "ReasonInfo.h"
/**********************************************************************/

CReasonInfo::CReasonInfo(void)
{
	Reset();
}

/**********************************************************************/

void CReasonInfo::Reset()
{
	m_nReasonType = 0;
	m_nReasonCode = 0;
	m_strReasonText = "";
	m_strTimeTran = "";
	m_strTimeAction = "";
	m_nTranSeqNo = 0;
	m_nTranTableNo = 0;
	m_nTranCovers = 0;
	m_nTranServerNo = 0;

	m_bItemVoid = FALSE;
	m_bItemRefund = FALSE;
	m_bItemWastageVoid = FALSE;
	m_nItemServerNo = 0;
	m_nItemPluNo = 0;
	m_nItemPriceBand = 0;
	m_dItemQty = 0.0;
	m_dItemVal = 0.0;
	m_strItemTaxBand = "";
	m_nItemDeptNo = 0;
	m_nItemGroupNo = 0;
	m_nItemACatNo = 0;
	m_strItemVoidBAType = "";
	m_nItemAuthServerNo = 0;

	m_nVoidDiscountAmount = 0;
	m_strVoidDiscountType = "";
	m_nVoidDiscountRate = 0;
	m_nVoidDiscountPromoNo = 0;
	m_nVoidDiscountUsage = 0;
	m_strVoidDiscountBAType = "";

	m_nSDiscPromoNo = 0;
	m_strSDiscType= "";
	m_nSDiscRate = 0;
	m_nSDiscAmount = 0;

	m_nIDiscServerNo = 0;
	m_nIDiscPluNo = 0;
	m_nIDiscPromoNo = 0;
	m_nIDiscACatNo = 0;
	m_strIDiscType = "";
	m_nIDiscRate = 0;
	m_nIDiscAmount = 0;
}

/**********************************************************************/

void CReasonInfo::LoadCSVLine( CCSV& csv )
{
	Reset();
	m_nReasonType = csv.GetInt(0);
	m_nReasonCode = csv.GetInt(1);
	m_strReasonText = csv.GetString(2);
	m_strTimeTran = csv.GetString(3);
	m_strTimeAction = csv.GetString(4);
	m_nTranSeqNo = csv.GetInt(5);
	m_nTranTableNo = csv.GetInt(6);
	//SKIP OBSOLETE TRAN TRAIN MODE
	m_nTranCovers = csv.GetInt(8);
	m_nTranServerNo = csv.GetInt(9);

	switch( m_nReasonType )
	{
	case SS_REASON_REFUNDVOID:
	case SS_REASON_WASTAGE:
	case SS_REASON_VIP:
		switch( csv.GetInt(10) )
		{
		case 1:
			m_bItemVoid = csv.GetBool(11);
			m_bItemRefund = csv.GetBool(12);
			m_bItemWastageVoid = csv.GetBool(13);
			m_nItemServerNo = csv.GetInt(14);
			m_nItemPluNo = csv.GetInt64(15);
			m_nItemPriceBand = csv.GetInt(16);
			m_dItemQty = csv.GetDouble(17);
			m_dItemVal = csv.GetDouble(18);
			m_strItemTaxBand = csv.GetString(19);
			m_nItemDeptNo = csv.GetInt(20);
			m_nItemGroupNo = csv.GetInt(21);
			m_nItemACatNo = csv.GetInt(22);
			m_strItemVoidBAType = csv.GetString(23);
			m_nItemAuthServerNo = csv.GetInt(24);
			break;
		}
		break;

	case SS_REASON_DISCOUNT_VOID:
		switch( csv.GetInt(10) )
		{
		case 1:
			m_nVoidDiscountAmount = csv.GetInt(11);
			m_strVoidDiscountType = csv.GetString(12);
			m_nVoidDiscountRate = csv.GetInt(13);
			m_nVoidDiscountPromoNo = csv.GetInt(14);
			m_nVoidDiscountUsage = csv.GetInt(15);
			m_strVoidDiscountBAType = csv.GetString(16);
			break;
		}
		break;

	case SS_REASON_DISCOUNT_SUBT:
		switch( csv.GetInt(10) )
		{
		case 1:
			m_nSDiscPromoNo = csv.GetInt(11);
			m_strSDiscType= csv.GetString(12);
			m_nSDiscRate = csv.GetInt(13);
			m_nSDiscAmount = csv.GetInt(14);
			break;
		}
		break;

	case SS_REASON_DISCOUNT_ITEM:
		switch( csv.GetInt(10) )
		{
		case 1:
			m_nIDiscServerNo = csv.GetInt(11);
			m_nIDiscPluNo = csv.GetInt64(12);
			m_nIDiscPromoNo = csv.GetInt(13);
			m_nIDiscACatNo = csv.GetInt(14);
			m_strIDiscType = csv.GetString(15);
			m_nIDiscRate = csv.GetInt(16);
			m_nIDiscAmount = csv.GetInt(17);
			break;
		}
		break;
	}
}

/**********************************************************************/

void CReasonInfo::LoadLegacyLine( CCSV& csv )
{
	Reset();

	switch( csv.GetInt(0) )
	{
	//REFUND OR VOID
	case 0:
		m_nReasonType = SS_REASON_REFUNDVOID;
		m_nReasonCode = 0;
		m_nItemAuthServerNo = 0;
		m_strReasonText = csv.GetString(21);
		m_strTimeTran = csv.GetString(7);
		m_strTimeAction = csv.GetString(8);
		m_nTranSeqNo = csv.GetInt(4);
		m_nTranTableNo = csv.GetInt(5);
		m_nTranCovers = csv.GetInt(14);
		m_nTranServerNo = csv.GetInt(18);
		m_bItemVoid = csv.GetBool(1);
		m_bItemRefund = csv.GetBool(2);
		m_bItemWastageVoid = csv.GetBool(3);
		m_nItemServerNo = csv.GetInt(19);
		m_nItemPluNo = csv.GetInt64(9);
		m_nItemPriceBand = csv.GetInt(10);
		m_dItemQty = csv.GetDouble(11);
		m_dItemVal = csv.GetDouble(12);
		m_strItemTaxBand = csv.GetString(13);
		m_nItemDeptNo = csv.GetInt(15);
		m_nItemGroupNo = csv.GetInt(16);
		m_nItemACatNo = csv.GetInt(17);
		m_strItemVoidBAType = csv.GetString(20);
		break;

	//VOID DISCOUNT
	case 2:
		m_nReasonType = SS_REASON_DISCOUNT_VOID;
		m_nReasonCode = csv.GetInt(12);
		m_strReasonText = csv.GetString(13);
		m_strTimeTran = csv.GetString(3);
		m_strTimeAction = csv.GetString(4);
		m_nTranSeqNo = csv.GetInt(1);
		m_nTranTableNo = csv.GetInt(2);
		m_nTranCovers = 0;
		m_nTranServerNo = csv.GetInt(5);
		m_nVoidDiscountAmount = csv.GetInt(6);
		m_strVoidDiscountType = csv.GetString(7);
		m_nVoidDiscountRate = csv.GetInt(8);
		m_nVoidDiscountPromoNo = csv.GetInt(9);
		m_nVoidDiscountUsage = csv.GetInt(10);
		m_strVoidDiscountBAType = csv.GetString(11);
		break;
	}
}

/**********************************************************************/

void CReasonInfo::SaveCSVLine( CCSV& csv )
{
	csv.RemoveAll();
	csv.Add( m_nReasonType );
	csv.Add( m_nReasonCode );
	csv.Add( m_strReasonText );
	csv.Add( m_strTimeTran );
	csv.Add( m_strTimeAction );
	csv.Add( m_nTranSeqNo );
	csv.Add( m_nTranTableNo );
	csv.Add( 0 );	//OBSOLETE TRAN TRAIN MODE
	csv.Add( m_nTranCovers );
	csv.Add( m_nTranServerNo );

	switch( m_nReasonType )
	{
	case SS_REASON_REFUNDVOID:
	case SS_REASON_WASTAGE:
	case SS_REASON_VIP:
		csv.Add( 1 );
		csv.Add( m_bItemVoid );
		csv.Add( m_bItemRefund );
		csv.Add( m_bItemWastageVoid );
		csv.Add( m_nItemServerNo );
		csv.Add( m_nItemPluNo );
		csv.Add( m_nItemPriceBand );
		csv.Add( m_dItemQty, 5 );
		csv.Add( m_dItemVal, 2);
		csv.Add( m_strItemTaxBand );
		csv.Add( m_nItemDeptNo );
		csv.Add( m_nItemGroupNo );
		csv.Add( m_nItemACatNo );
		csv.Add( m_strItemVoidBAType );
		csv.Add( m_nItemAuthServerNo );
		break;

	case SS_REASON_DISCOUNT_VOID:
		csv.Add( 1 );
		csv.Add( m_nVoidDiscountAmount );
		csv.Add( m_strVoidDiscountType );
		csv.Add( m_nVoidDiscountRate );
		csv.Add( m_nVoidDiscountPromoNo );
		csv.Add( m_nVoidDiscountUsage );
		csv.Add( m_strVoidDiscountBAType );
		break;

	case SS_REASON_DISCOUNT_SUBT:
		csv.Add( 1 );
		csv.Add( m_nSDiscPromoNo );
		csv.Add( m_strSDiscType );
		csv.Add( m_nSDiscRate );
		csv.Add( m_nSDiscAmount );
		break;

	case SS_REASON_DISCOUNT_ITEM:
		csv.Add( 1 );
		csv.Add( m_nIDiscServerNo );
		csv.Add( m_nIDiscPluNo );
		csv.Add( m_nIDiscPromoNo );
		csv.Add( m_nIDiscACatNo );
		csv.Add( m_strIDiscType );
		csv.Add( m_nIDiscRate );
		csv.Add( m_nIDiscAmount );
		break;
	}
}

/**********************************************************************/