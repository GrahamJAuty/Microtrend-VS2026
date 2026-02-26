/**********************************************************************/
#include "ReportHelpers.h"
/**********************************************************************/
#include "ReportPluInfoFinder.h"
/**********************************************************************/

CReportPluInfoFinder::CReportPluInfoFinder()
{
	m_bInConstructor = TRUE;
	Init();
	m_bInConstructor = FALSE;
}

/**********************************************************************/

CReportPluInfoFinder::CReportPluInfoFinder( __int64 nPluNo )
{
	m_bInConstructor = TRUE;
	SetPluNo( nPluNo );
	m_bInConstructor = FALSE;
}

/**********************************************************************/

CReportPluInfoFinder::CReportPluInfoFinder( CCSV& csvIn, int nLineType )
{
	m_bInConstructor = TRUE;	
	SetPluNo( csvIn, nLineType );
	m_bInConstructor = FALSE;
}

/**********************************************************************/

void CReportPluInfoFinder::Init()
{
	m_nDbIdx = -1;
	m_nPluIdx = -1;
	m_nPluNo = 0;
	m_bPluExists = FALSE;

	m_bGotCost = FALSE;
	m_dCost = 0.0;

	m_bIsUnknownSpecialPlu = FALSE;
	m_bGotUnknownSpecialPluText = FALSE;
	m_nModTextLevel = -1;
	m_bModRepText = FALSE;
	m_strSpecialPluText = "";
	m_strEcrText = "";
	m_strRepText = "";
	m_strSortCode = "";

	m_nImportLineLevel = 0;
	m_dImportLineQty =	 0.0;
	m_dImportLineVal = 0.0;
}

/**********************************************************************/

void CReportPluInfoFinder::SetPluNo( __int64 nPluNo )
{
	int nDbIdx = DataManager.GetActiveDbIdx();

	if ( ( nPluNo != m_nPluNo ) || ( nDbIdx != m_nDbIdx ) || ( TRUE == m_bInConstructor ) )
	{
		Init();

		if ( DataManager.Plu.FindPluByNumber( nPluNo, m_nPluIdx ) == TRUE )
		{
			DataManager.Plu.GetAt( m_nPluIdx, m_PluRecord );
			m_bPluExists = TRUE;
			m_bIsUnknownSpecialPlu = FALSE;
		}
		else
		{
			m_bPluExists = FALSE;
			m_nPluIdx = -1;
			m_bIsUnknownSpecialPlu = CheckSpecialPluNo(nPluNo);
		}

		m_nPluNo = nPluNo;
		m_nDbIdx = nDbIdx;
	}
}

/**********************************************************************/

void CReportPluInfoFinder::SetPluNo( CCSV& csvIn, int nLineType )
{
	SetPluNo( csvIn.GetInt64(0) );

	switch( nLineType )
	{
	case 1:
		m_nImportLineLevel = 0;
		m_dImportLineQty = csvIn.GetDouble(1);
		m_dImportLineVal = 0.0;
		break;

	case 2:
		m_nImportLineLevel = csvIn.GetInt(1);
		m_dImportLineQty = csvIn.GetDouble(2);
		m_dImportLineVal = csvIn.GetDouble(3);
		break;

	case 3:
		m_nImportLineLevel = csvIn.GetInt(1);
		m_dImportLineQty = csvIn.GetDouble(2);
		m_dImportLineVal = 0.0;
		break;

	case 0:
		m_nImportLineLevel = 0;
		m_dImportLineQty = csvIn.GetDouble(1);
		m_dImportLineVal = csvIn.GetDouble(2);
		break;
	}

	if (m_nImportLineLevel < 0 || m_nImportLineLevel >= MAX_PLU_PRICES)
	{
		m_nImportLineLevel = 0;
	}

	if ( TRUE == m_bPluExists )
	{
		switch( nLineType )
		{
		case 1:
		case 3:
			m_dImportLineVal = m_dImportLineQty * GetPrice( m_nImportLineLevel );
			break;
		}
	}
}

/**********************************************************************/

double CReportPluInfoFinder::GetCost()
{
	double dCost = 0.0;

	if ( TRUE == m_bPluExists )
	{
		if ( TRUE == m_bGotCost )
		{
			dCost = m_dCost;
		}
		else
		{
			dCost = m_dCost = DataManager.Plu.GetPluPurchaseCost( m_PluRecord );
			m_bGotCost = TRUE;
		}
	}	

	return dCost;
}

/**********************************************************************/

int CReportPluInfoFinder::GetModifierDeptNo(int nMod)
{
	if (TRUE == m_bPluExists)
	{
		return m_PluRecord.GetModifierDeptNoForReport(nMod);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

int CReportPluInfoFinder::GetModifierAnalysisCategory( int nMod )
{
	if (TRUE == m_bPluExists)
	{
		return m_PluRecord.GetModifierAnalysisCategoryForReport(nMod);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

int CReportPluInfoFinder::GetModifierTaxBand(int nMod)
{
	if (TRUE == m_bPluExists)
	{
		return m_PluRecord.GetModifierTaxBandIntForReport(nMod);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

const char* CReportPluInfoFinder::GetPluInfoEcrText( bool bKludgeSpecialPlu, int nMod, bool bModRepText )
{
	CheckText( bKludgeSpecialPlu, nMod, bModRepText );
	return m_strEcrText;
}

/**********************************************************************/

const char* CReportPluInfoFinder::GetPluInfoRepText( bool bKludgeSpecialPlu, int nMod, bool bModRepText )
{
	CheckText( bKludgeSpecialPlu, nMod, bModRepText );
	return m_strRepText;
}

/**********************************************************************/

const char* CReportPluInfoFinder::GetPluInfoSortCode( bool bKludgeSpecialPlu, int nMod, bool bModRepText )
{
	CheckText( bKludgeSpecialPlu, nMod, bModRepText );
	return m_strSortCode;
}

/**********************************************************************/

void CReportPluInfoFinder::CheckText( bool bKludgeSpecialPlu, int nMod, bool bModRepText )
{
	if (FALSE == m_bPluExists)
	{
		m_strEcrText = "";

		if (TRUE == m_bIsUnknownSpecialPlu)
		{
			if (TRUE == bKludgeSpecialPlu)
			{
				if (FALSE == m_bGotUnknownSpecialPluText) 
				{
					UpdateSpecialPluText();
					m_bGotUnknownSpecialPluText = TRUE;
				}
				
				m_strEcrText = m_strSpecialPluText;
			}
		}

		if (m_strEcrText == "")
		{
			m_strEcrText = "**Unknown";
		}

		m_strRepText = m_strEcrText;
		m_strSortCode = m_strEcrText;
	}
	else if ( ( nMod != m_nModTextLevel ) || ( bModRepText != m_bModRepText ) )
	{
		UpdatePluText( nMod, bModRepText );
	}

	m_nModTextLevel = nMod;
	m_bModRepText = bModRepText;
}

/**********************************************************************/

void CReportPluInfoFinder::UpdatePluText( int nMod, bool bModRepText )
{
	m_strEcrText = m_PluRecord.GetModifierEposText( nMod );
	m_strRepText = "";
		
	if ( TRUE == bModRepText )
	{
		m_strRepText = m_PluRecord.GetModifierReportText( nMod );
		::TrimSpaces( m_strRepText, FALSE );
	}
		
	if (m_strRepText == "")
	{
		m_strRepText = ReportHelpers.GetReportText(m_PluRecord);
	}

	m_strSortCode = m_PluRecord.GetSortCode();
}

/**********************************************************************/

bool CReportPluInfoFinder::CheckSpecialPluNo(__int64 nPluNo)
{
	bool bSpecialPlu = FALSE;

	if ((nPluNo >= 90000000000000) && (nPluNo <= 90000000099999))
	{
		bSpecialPlu = TRUE;
	}
	else if ((nPluNo >= 99000000000000) && (nPluNo <= 99000000000099))
	{
		bSpecialPlu = TRUE;
	}

	return bSpecialPlu;
}

/**********************************************************************/

void CReportPluInfoFinder::UpdateSpecialPluText()
{
	bool bPromotion = FALSE;
	int nPromoNo = 0;

	if ( ( m_nPluNo >= 90000000000000 ) && ( m_nPluNo <= 90000000099999 ) )
	{
		int nPluDetail = int ( m_nPluNo % 100000 );

		if ( ( nPluDetail >= 1000 ) && ( nPluDetail <= 1999  ) )
		{
			int nMMOfferIdx;
			int nMMOfferNo = nPluDetail - 1000;

			if ( DataManager.MixMatch.FindTableByNumber( nMMOfferNo, nMMOfferIdx ) == TRUE )
			{
				CMixMatchCSVRecord MixMatchRecord;
				DataManager.MixMatch.GetAt( nMMOfferIdx, MixMatchRecord );
				m_strSpecialPluText = MixMatchRecord.GetDisplayName();
			}
			else
			{
				m_strSpecialPluText.Format ( "Mix Match Offer %d", nMMOfferNo );
			}
		}
		else if ( ( nPluDetail >= 0 ) && ( nPluDetail <= 9 ) )
		{
			m_strSpecialPluText = "Table Status Level";
		}
		else if ( ( nPluDetail >= 100 ) && ( nPluDetail <= 199 ) )
		{
			m_strSpecialPluText = "Deposit Prepayment";
		}
		else if ( ( nPluDetail >= 900 ) && ( nPluDetail <= 999 ) )
		{
			//PROMOTION REPORTED VIA EOD REASON FILE (NOT CASHRSP)
			bPromotion = TRUE;
			nPromoNo = nPluDetail - 900;
		}
		else
		{
			switch( nPluDetail )
			{
			case 10000:	
				m_strSpecialPluText = "Customer RA";		
				break;

			case 10010:	
				m_strSpecialPluText = "Room RA";
				break;

			case 10020:
				m_strSpecialPluText = "Cashless Payment System Top-up";	
				break;

			case 10030:	
				m_strSpecialPluText = "Sports Booker RA";	
				break;

			case 10200:
				m_strSpecialPluText = "BlueRunner";
				break;

			case 90001:
				m_strSpecialPluText = "Sports Booker Refund";
				break;

			case 90002:	
				m_strSpecialPluText = "Sports Booker Discount";	
				break;

			case 90003:	
				m_strSpecialPluText = "Sports Booker Supplement";
				break;

			case 90004:
				m_strSpecialPluText = "Sports Booker Admission";	
				break;

			case 90005:
				m_strSpecialPluText = "Sports Booker Credit";	
			}
		}
	}
	//NOT TAXABLE PROMOTION FLAGGED INTERNALLY BY ECR MANAGER 
	else if ( ( m_nPluNo >= 99000000000000 ) && ( m_nPluNo <= 99000000000099 ) )
	{
		bPromotion = TRUE;
		nPromoNo = int ( m_nPluNo % 100 );
	}

	if (TRUE == bPromotion)
	{
		int nPromoIdx;
		if (DataManager.Promotion.FindTableByNumber(nPromoNo, nPromoIdx) == TRUE)
		{
			CPromotionsCSVRecord PromoRecord;
			DataManager.Promotion.GetAt(nPromoIdx, PromoRecord);
			m_strSpecialPluText = PromoRecord.GetDisplayName();
		}
		else
		{
			m_strSpecialPluText.Format("Promo %d", nPromoNo);
		}
	}
}

/**********************************************************************/

const char* CReportPluInfoFinder::GetSupplierRef()
{
	m_strSupplierRef = m_bPluExists ? m_PluRecord.GetSupplierRef() : "";
	return m_strSupplierRef;
}

/**********************************************************************/
