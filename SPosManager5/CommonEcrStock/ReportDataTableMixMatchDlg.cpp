/**********************************************************************/
#include "CCSVReportLine.h"
#include "RepSpawn.h"
#include "TimePlanTemplateListManager.h"
/**********************************************************************/
#include "ReportDataTableMixMatchDlg.h"
/**********************************************************************/

CReportDataTableMixMatchDlg::CReportDataTableMixMatchDlg(CWnd* pParent /*=NULL*/)
	: CReportDataTableDlg(pParent)
{
	m_strTitle = "Select Mix Match Report Type";
	m_arrayReports.Add("Programming");
	m_arrayReports.Add("Timeplan Summary");
	m_arrayReports.Add("Epos Version Check");
}

/**********************************************************************/

void CReportDataTableMixMatchDlg::HandleReport1()
{
	CreateDetailReport(1);
}

/**********************************************************************/

void CReportDataTableMixMatchDlg::HandleReport2()
{
	CreateDetailReport(2);
}

/**********************************************************************/

void CReportDataTableMixMatchDlg::CreateDetailReport(int nSubType)
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	ReportFile.SetStyle1( ( nSubType != 2 ) ? "Mix Match Settings" : "Mix Match Time Plan Summary");
	ReportFile.AddColumn("No", TA_LEFT, 120);
	ReportFile.AddColumn("Name", TA_LEFT, 400);
	ReportFile.AddColumn("Enable", TA_LEFT, 150);
	ReportFile.AddColumn("Activation", TA_LEFT, 380);
	ReportFile.AddColumn("Timeplan Name", TA_LEFT, 350);
	ReportFile.AddColumn("Timeplan Entry", TA_LEFT, 350);

	if (nSubType != 2)
	{
		ReportFile.AddColumn("Order Mode", TA_LEFT, 380);
		ReportFile.AddColumn("Use Gate", TA_LEFT, 250);
		ReportFile.AddColumn("Autolock", TA_LEFT, 250);
	}

	ReportFile.AddColumn("Action", TA_LEFT, 400);
	ReportFile.AddColumn("Value", TA_RIGHT, 250);
	ReportFile.AddColumn("Detail", TA_LEFT, 500);

	if (nSubType != 2)
	{
		ReportFile.AddColumn("PriceBand", TA_LEFT, 300);
		ReportFile.AddColumn("Start Date", TA_LEFT, 250);
		ReportFile.AddColumn("End Date", TA_LEFT, 250);
		ReportFile.AddColumn("Bucket A", TA_RIGHT, 250);
		ReportFile.AddColumn("Bucket B", TA_RIGHT, 250);
		ReportFile.AddColumn("Bucket C", TA_RIGHT, 250);
		ReportFile.AddColumn("Bucket D", TA_RIGHT, 250);
		ReportFile.AddColumn("Bucket E", TA_RIGHT, 250);
		ReportFile.AddColumn("Min Spend", TA_RIGHT, 200);
	}

	int nLineNo = 0;
	CReportConsolidationArray<CSortedTimeplanEntry> arrayTimeplanEntries;
	CTimePlanTemplateListManager::GetTimeplanReportDetails(2, nLineNo, arrayTimeplanEntries);

	for (int nMixMatchIdx = 0; nMixMatchIdx < DataManager.MixMatch.GetSize(); nMixMatchIdx++)
	{
		CMixMatchCSVRecord MixMatchRecord;
		DataManager.MixMatch.GetAt(nMixMatchIdx, MixMatchRecord);

		if (EcrmanOptions.GetFeaturesTimePlanTemplateFlag() == TRUE)
		{
			CTimePlanTemplateListManager::GetTimeplanReportDetails(nLineNo, MixMatchRecord, arrayTimeplanEntries);
		}

		int nEntryStartIdx = 0; int nEntryEndIdx = 0;
		CTimePlanTemplateListManager::GetReportDetailsEntryRange(MixMatchRecord.GetMixMatchNo(), arrayTimeplanEntries, nEntryStartIdx, nEntryEndIdx);

		CCSV csvOut('\t');
		csvOut.Add(MixMatchRecord.GetMixMatchNo());
		csvOut.Add(MixMatchRecord.GetDisplayName());

		if (MixMatchRecord.GetEnableFlag())
		{
			csvOut.Add("Yes");
		}
		else
		{
			csvOut.Add("No");
		}

		switch (MixMatchRecord.GetActivationType())
		{
		case 0:
			csvOut.Add("Always On");
			break;

		case 1:
			csvOut.Add("Time Plan & Manual");
			break;

		case 2:
			csvOut.Add("Manual Only");
			break;

		default:
			csvOut.Add("Unknown");
			break;
		}

		{
			CString strPlanName = "";
			CString strEntryName = "";

			if (nEntryStartIdx <= nEntryEndIdx)
			{
				CTimePlanTemplateListManager::GetTimeplanEntryDetails(arrayTimeplanEntries, nEntryStartIdx, strPlanName, strEntryName);
			}

			csvOut.Add(strPlanName);
			csvOut.Add(strEntryName);
		}

		if (nSubType != 2)
		{
			switch (MixMatchRecord.GetOrderMode())
			{
			case 0:
				csvOut.Add("Whole Order");
				break;

			case 1:
				csvOut.Add("Seat Only");
				break;

			default:
				csvOut.Add("Unknown");
				break;
			}

			csvOut.Add(MixMatchRecord.GetUseGate());

			if (MixMatchRecord.GetAutoLockFlag())
			{
				csvOut.Add("Yes");
			}
			else
			{
				csvOut.Add("No");
			}
		}

		switch (MixMatchRecord.GetAction())
		{
		case MIXMATCH_ACTION_NONE:				
			csvOut.Add("None");	
			break;

		case MIXMATCH_ACTION_DISCOUNT_PLU:	
			csvOut.Add("Deduct Product Cost");
			break;

		case MIXMATCH_ACTION_FIXCOST_PLU:
			csvOut.Add("Selling Price by PLU");	
			break;

		case MIXMATCH_ACTION_FIXCOST_PLU_2:
			csvOut.Add("Selling Price by PLU (2)");	
			break;

		case MIXMATCH_ACTION_PROMO:	
			csvOut.Add("Promotion");	
			break;

		case MIXMATCH_ACTION_FREE_ITEM:	
			csvOut.Add("Cheapest Item Free");
			break;

		case MIXMATCH_ACTION_FREE_BUCKET:
			csvOut.Add("One Bucket Free");
			break;

		case MIXMATCH_ACTION_FIXCOST_AMOUNT:
			csvOut.Add("Selling Price by Cost");
			break;

		case MIXMATCH_ACTION_FIXCOST_BUCKET:
			csvOut.Add("Set Bucket Price");	
			break;

		default:					
			csvOut.Add("Unknown");	
			break;
		}

		GetDetailAndValueString(MixMatchRecord);
		csvOut.Add(m_strValue);
		csvOut.Add(m_strDetail);

		if (nSubType != 2)
		{
			if (MixMatchRecord.GetUseCurrentPriceBandFlag())
			{
				csvOut.Add("Current");
			}
			else
			{
				csvOut.Add("Always 1");
			}

			{
				CString strDate;
				::GetReportStringFromOleDate(MixMatchRecord.GetStartDate(), strDate);
				csvOut.Add(strDate);
			}

			{
				CString strDate;
				::GetReportStringFromOleDate(MixMatchRecord.GetEndDate(), strDate);
				csvOut.Add(strDate);
			}

			csvOut.Add(GetBucketString(MixMatchRecord.GetBucketCount1(), MixMatchRecord.GetOfferGroup1()));
			csvOut.Add(GetBucketString(MixMatchRecord.GetBucketCount2(), MixMatchRecord.GetOfferGroup2()));
			csvOut.Add(GetBucketString(MixMatchRecord.GetBucketCount3(), MixMatchRecord.GetOfferGroup3()));
			csvOut.Add(GetBucketString(MixMatchRecord.GetBucketCount4(), MixMatchRecord.GetOfferGroup4()));
			csvOut.Add(GetBucketString(MixMatchRecord.GetBucketCount5(), MixMatchRecord.GetOfferGroup5()));

			csvOut.Add(MixMatchRecord.GetMinSpend(), 2);
		}

		ReportFile.WriteLine(csvOut.GetLine());
		CTimePlanTemplateListManager::ReportExtraTimeplans(ReportFile, arrayTimeplanEntries, nEntryStartIdx, nEntryEndIdx, 4);
		ReportFile.WriteLine("<LI>");
	}
	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp(SysFiles::MixMatchPrm);
	ReportManager.DisplayReport(Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this);
}

/**********************************************************************/

void CReportDataTableMixMatchDlg::HandleReport3()
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	ReportFile.SetStyle1 ( "Epos Version Check for Mix Match Offers" );
		
	ReportFile.AddColumn ( "No", TA_LEFT, 120 );
	ReportFile.AddColumn ( "Name", TA_LEFT, 400 );
	ReportFile.AddColumn ( "Minimum Version", TA_LEFT, 800 );
	ReportFile.AddColumn ( "Reason", TA_LEFT, 800 );

	for ( int nMixMatchIdx = 0; nMixMatchIdx < DataManager.MixMatch.GetSize(); nMixMatchIdx++ )
	{
		CMixMatchCSVRecord MixMatchRecord;
		DataManager.MixMatch.GetAt ( nMixMatchIdx, MixMatchRecord );

		{
			__int64 nSharpPluNo = 0;
			CMinimumSPOSVersion Version;
			MixMatchRecord.CheckMinimumSPOSVersion(Version, nSharpPluNo, TRUE);

			if (Version.m_strReason != "")
			{
				CCSV csvOut('\t');
				csvOut.Add(MixMatchRecord.GetMixMatchNo());
				csvOut.Add(MixMatchRecord.GetDisplayName());
				csvOut.Add(Version.GetMinVerName());
				csvOut.Add(Version.m_strReason);
				ReportFile.WriteLine(csvOut.GetLine());
			}
		}

		{
			__int64 nSharpPluNo = 0;
			CMinimumSPOSVersion Version;
			MixMatchRecord.CheckMinimumSPOSVersion(Version, nSharpPluNo, FALSE);

			CCSV csvOut('\t');
			csvOut.Add(MixMatchRecord.GetMixMatchNo());
			csvOut.Add(MixMatchRecord.GetDisplayName());
			csvOut.Add(Version.GetMinVerName());
			csvOut.Add(Version.m_strReason);
			ReportFile.WriteLine(csvOut.GetLine());
		}
	}
	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::MixMatchVersionPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}

/**********************************************************************/

void CReportDataTableMixMatchDlg::GetDetailAndValueString( CMixMatchCSVRecord& record )
{
	m_strValue = "";
	m_strDetail = "";

	switch( record.GetAction() )
	{
	case MIXMATCH_ACTION_FIXCOST_PLU:
	case MIXMATCH_ACTION_FIXCOST_PLU_2:
	case MIXMATCH_ACTION_DISCOUNT_PLU:
		{
			CString strPluNo;
			::FormatPluNo( record.GetActionPluNo(), strPluNo );

			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber( record.GetActionPluNo(), nPluIdx ) == TRUE )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt( nPluIdx, PluRecord );
				
				m_strDetail.Format( "%s %s",
					(const char*) strPluNo,
					PluRecord.GetReportText() );

				m_strValue.Format( "%.2f", PluRecord.GetPrice(0) );
			}
			else
			{
				m_strDetail.Format( "%s unknown",
					(const char*) strPluNo );
			}
		}
		break;

	case MIXMATCH_ACTION_PROMO:
		{
			int nPromoNo = record.GetActionPromo();

			if ( nPromoNo == 0 )
				m_strDetail = "None";
			else
			{
				int nPromoIdx;
				if ( DataManager.Promotion.FindTableByNumber( nPromoNo, nPromoIdx ) == TRUE )
				{
					CPromotionsCSVRecord promo;
					DataManager.Promotion.GetAt( nPromoIdx, promo );

					m_strDetail.Format( "P%2.2d %s",
						nPromoNo,
						promo.GetDisplayName() );
				}
				else
				{
					m_strDetail.Format( "P%2.2d unknown",
						nPromoNo );
				}
			}
		}
		break;

	case MIXMATCH_ACTION_FREE_ITEM:
		switch( record.GetActionBucketFreeItem() )
		{
		case 0:	m_strDetail = "Any bucket";	break;
		case 1:	m_strDetail = "Bucket A";	break;
		case 2:	m_strDetail = "Bucket B";	break;	
		case 3:	m_strDetail = "Bucket C";	break;
		case 4:	m_strDetail = "Bucket D";	break;
		case 5:	m_strDetail = "Bucket E";	break;
		default:	m_strDetail = "Bucket ?";	break;
		}
		break;

	case MIXMATCH_ACTION_FREE_BUCKET:
		switch( record.GetActionBucketFreeBucket() )
		{
		case 1:	m_strDetail = "Bucket A";	break;
		case 2:	m_strDetail = "Bucket B";	break;	
		case 3:	m_strDetail = "Bucket C";	break;
		case 4:	m_strDetail = "Bucket D";	break;
		case 5:	m_strDetail = "Bucket E";	break;
		default:	m_strDetail = "Bucket ?";	break;
		}
		break;

	case MIXMATCH_ACTION_FIXCOST_BUCKET:
		switch( record.GetActionBucketFixedBucket() )
		{
		case 1:	m_strDetail = "Bucket A";	break;
		case 2:	m_strDetail = "Bucket B";	break;	
		case 3:	m_strDetail = "Bucket C";	break;
		case 4:	m_strDetail = "Bucket D";	break;
		case 5:	m_strDetail = "Bucket E";	break;
		default:	m_strDetail = "Bucket ?";	break;
		}
		m_strValue.Format( "%.2f", ( (double )record.GetActionCost() ) / 100.0 );
		break;

	case MIXMATCH_ACTION_FIXCOST_AMOUNT:
		m_strValue.Format( "%.2f", ( (double )record.GetActionCost() ) / 100.0 );
		break;
	}
}

/**********************************************************************/

const char* CReportDataTableMixMatchDlg::GetBucketString( int nTrip, int nOfferGroup )
{
	if ( ( nTrip == 0 ) || ( nOfferGroup == 0 ) )
		m_strBucket = "";
	else
	{
		CString strGroup;

		int nOfferIdx;
		if ( DataManager.OfferGroup.FindTableByNumber( nOfferGroup, nOfferIdx ) == TRUE )
		{
			COfferGroupsCSVRecord offer( NODE_OFFER_GROUP );
			DataManager.OfferGroup.GetAt( nOfferIdx, offer );
			strGroup = offer.GetDisplayName();
		}
		else
			strGroup.Format( "Offer %d", nOfferGroup );

		m_strBucket.Format( "%d x %s",
			nTrip,
			(const char*) strGroup );
	}

	return m_strBucket;
}

/**********************************************************************/

