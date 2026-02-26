/*********************************************************************/
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
#include "SPOSVersions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "MixMatchCSVArray.h"
#include "NetworkCSVArray.h"
#include "OfferGroupEntriesCSVArray.h"
#include "OfferGroupsCSVArray.h"
#include "PriceHelpers.h"
#include "PromotionsCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CommsExportPromoEtc.h"
/**********************************************************************/

CCommsExportPromoEtc::CCommsExportPromoEtc( CJobListCSVRecord& JobInfo, bool bFromMemory, int nDownloadType ) : CCommsExportBase( JobInfo, bFromMemory )
{
	switch (nDownloadType)
	{
	case DNTYPE_PROMO:
	case DNTYPE_MIXMATCH:
	case DNTYPE_WALLETSTAMPRULE:
	case DNTYPE_ALLOFFER:
		m_nDownloadType = nDownloadType;
		break;

	default:
		m_nDownloadType = DNTYPE_PROMO;
		break;
	}
}

/**********************************************************************/

int CCommsExportPromoEtc::PrepareFile()
{
	if (m_nNetworkIdx < 0 || m_nNetworkIdx >= dbNetwork.GetSize())
	{
		return COMMS_ERROR_BADNETIDX;
	}

	switch( dbNetwork.GetConnectionType( m_nNetworkIdx ) )
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
		return COMMS_ERROR_NOCOMMS;
	}

	if (m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize())
	{
		return COMMS_ERROR_BADLOCIDX;
	}

	m_strSourceFolder = dbNetwork.GetFolderPathData ( m_nNetworkIdx );
	m_strSourceFolder += "\\";

	m_strDestFolder = LocationMasterMap.GetDownloadFolder( m_nLocIdx, m_nTerminalNo, m_bImmediate );

	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo ( m_nLocIdx );
	if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
	{
		return COMMS_ERROR_BADDBLINK;
	}

	int nResult = COMMS_ERROR_NODATA;
	int nSPOSVersion = dbNetwork.GetSPOSVersion( m_nNetworkIdx );

	CDataManagerInfo info;

	if ( FALSE == m_bFromMemory )
	{
		if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4 ) == TRUE )
		{
			DataManager.OpenDatabaseQuickNoFiles( nDbIdx, FALSE );
			
			switch (m_nDownloadType)
			{
			case DNTYPE_MIXMATCH:
				DataManager.ReopenMixMatch(info);
				DataManager.ReopenOfferGroup(info);
				DataManager.ReopenOfferGroupEntry(info);
				DataManager.ReopenPromotion(info);
				break;

			case DNTYPE_WALLETSTAMPRULE:
				DataManager.ReopenWalletStampRule(info);
				DataManager.ReopenOfferGroup(info);
				DataManager.ReopenOfferGroupEntry(info);
				break;

			case DNTYPE_ALLOFFER:
				DataManager.ReopenMixMatch(info);
				DataManager.ReopenWalletStampRule(info);
				DataManager.ReopenOfferGroup(info);
				DataManager.ReopenOfferGroupEntry(info);
				DataManager.ReopenPromotion(info);
				break;

			case DNTYPE_PROMO:
			default:
				DataManager.ReopenPromotion(info);
				break;
			}	
		}
		else
		{
			return COMMS_ERROR_NODATA;
		}
	}

	bool bGotData = FALSE;
	switch (m_nDownloadType)
	{
	case DNTYPE_MIXMATCH:
		bGotData = (DataManager.MixMatch.GetSize() != 0);
		break;

	case DNTYPE_WALLETSTAMPRULE:
		bGotData = (DataManager.WalletStampRule.GetSize() != 0);
		break;

	case DNTYPE_ALLOFFER:
		bGotData = FALSE;
		bGotData |= (DataManager.MixMatch.GetSize() != 0);
		bGotData |= (DataManager.WalletStampRule.GetSize() != 0);
		bGotData |= (DataManager.Promotion.GetSize() != 0);
		break;

	case DNTYPE_PROMO:
		bGotData = (DataManager.Promotion.GetSize() != 0);
		break;
	}

	nResult = PrepareSPOSFile(nSPOSVersion);

	return nResult;
}

/**********************************************************************/

void CCommsExportPromoEtc::CreateMixMatchHeader(int nSPOSVersion)
{
	CCSV csvHeader;
	csvHeader.Add("[MMNo]");
	csvHeader.Add("[Enable]");
	csvHeader.Add("[UseTP]");
	csvHeader.Add("[DateStart]");
	csvHeader.Add("[DateEnd]");
	csvHeader.Add("[MMAction]");
	csvHeader.Add("[MMParam1]");
	csvHeader.Add("[OfferGroupID1]");
	csvHeader.Add("[OfferGroupID2]");
	csvHeader.Add("[OfferGroupID3]");
	csvHeader.Add("[OfferGroupID4]");
	csvHeader.Add("[OfferGroupID5]");
	csvHeader.Add("[Count1]");
	csvHeader.Add("[Count2]");
	csvHeader.Add("[Count3]");
	csvHeader.Add("[Count4]");
	csvHeader.Add("[Count5]");
	csvHeader.Add("[MMValue]");
	csvHeader.Add("[Description]");

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_614) == TRUE)
	{
		csvHeader.Add("[OrderMode]");
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_992) == TRUE)
	{
		csvHeader.Add("[MMParam2]");
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1092) == TRUE)
	{
		csvHeader.Add("[UseGate]");
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1384) == TRUE)
	{
		csvHeader.Add("[AutoLock]");
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_3_18) == TRUE)
	{
		csvHeader.Add("[PriceBandMode]");
	}

	m_strMixMatchHeader = csvHeader.GetLine();
}

/**********************************************************************/

void CCommsExportPromoEtc::CreatePromoHeader( int nSPOSVersion, bool bLegacyAnalysis )
{
	CCSV csvHeader;
	csvHeader.Add( "[PromNo]" );
	csvHeader.Add( "[Type]" );
	csvHeader.Add( "[pcValue]" );
	csvHeader.Add( "[mdValue]" );
	csvHeader.Add( "[Trip]" );
	csvHeader.Add( "[DptNo]" );
	csvHeader.Add( "[Gratuity]" );
	csvHeader.Add( "[Vat]" );
	csvHeader.Add( "[Description]" );
	csvHeader.Add( "[Usage]" );

	if (TRUE == bLegacyAnalysis)
	{
		csvHeader.Add("[CatFlags]");
	}
	else
	{
		csvHeader.Add( "[AnalysisNew]" );
		csvHeader.Add( "[AnalysisCats]" );
	}

	if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4_423 ) == TRUE )
	{
		csvHeader.Add( "[AllowMyPromoOnPromo]" );
		csvHeader.Add( "[AllowPromoOnMyPromo]" );
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2455) == TRUE)
	{
		csvHeader.Add("[Reasons]");
	}
		
	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1071) == TRUE)
	{
		csvHeader.Add("[TimeplanMode]");
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1096) == TRUE)
	{
		csvHeader.Add("[UseGate]");
	}

	if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4_1901 ) == TRUE )
	{
		csvHeader.Add( "[FilterType]" );
		csvHeader.Add( "[FilterOfferGroup]" );
	}

	if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4_2161 ) == TRUE )
	{
		csvHeader.Add( "[FlexPromoType]" );
		csvHeader.Add( "[FlexBoundByCov1]" );
		csvHeader.Add( "[FlexBoundByCov2]" );
		csvHeader.Add( "[FlexBoundByCov3]" );
		csvHeader.Add( "[FlexBoundByCov4]" );
		csvHeader.Add( "[FlexBoundByVal1]" );
		csvHeader.Add( "[FlexBoundByVal2]" );
		csvHeader.Add( "[FlexBoundByVal3]" );
		csvHeader.Add( "[FlexBoundByVal4]" );
		csvHeader.Add( "[FlexpcValue1]" );
		csvHeader.Add( "[FlexpcValue2]" );
		csvHeader.Add( "[FlexpcValue3]" );
		csvHeader.Add( "[FlexpcValue4]" );
		csvHeader.Add( "[FlexpcValue5]" );
	}
	
	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1618) == TRUE)
	{
		csvHeader.Add("[MarkdownUpTo]");
	}

	m_strPromoHeader = csvHeader.GetLine();
}

/**********************************************************************/

void CCommsExportPromoEtc::CreateOfferGroupHeader()
{
	CCSV csvHeader;
	csvHeader.Add("[OfferGroupID]");
	csvHeader.Add("[OfferGroupName]");
	m_strOfferGroupHeader = csvHeader.GetLine();
}

/**********************************************************************/

void CCommsExportPromoEtc::CreateOfferGroupEntryHeader()
{
	CCSV csvHeader;
	csvHeader.Add("[OfferGroupID]");
	csvHeader.Add("[OfferGroupType]");
	csvHeader.Add("[ID]");
	m_strOfferGroupEntryHeader = csvHeader.GetLine();
}

/**********************************************************************/

void CCommsExportPromoEtc::CreateWalletStampRuleHeader(int nSPOSVersion)
{
	CCSV csvHeader;
	csvHeader.Add("[StampOfferID]");
	csvHeader.Add("[Edition]");
	csvHeader.Add("[Enabled]");
	csvHeader.Add("[OfferGroupID]");
	csvHeader.Add("[Threshold]");
	csvHeader.Add("[Expiry]");
	csvHeader.Add("[Description]");
	
	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1579) == TRUE)
	{
		csvHeader.Add("[OfferType]");
	}

	csvHeader.Add("[Action]");
	m_strWalletStampRuleHeader = csvHeader.GetLine();
}

/**********************************************************************/

void CCommsExportPromoEtc::AppendMixMatch(int nSPOSVersion, CSQLOut& SQLOut)
{
	int nSize = DataManager.MixMatch.GetSize();
	for (int n = 0; n < nSize; n++)
	{
		StatusProgress.SetPos(n, nSize);

		CMixMatchCSVRecord MixMatchRecord;
		DataManager.MixMatch.GetAt(n, MixMatchRecord);

		__int64 nSharpPluNo = 0;
		CMinimumSPOSVersion Version;
		MixMatchRecord.CheckMinimumSPOSVersion(Version, nSharpPluNo);

		int nErrorType = -1;
		Version.CheckSPOSVersion(nSPOSVersion, nErrorType);

		if (nErrorType != -1)
		{
			TallyDownloadExclusion(NODE_MIXMATCH, nErrorType);
			continue;
		}

		SQLOut.Add(MixMatchRecord.GetMixMatchNo());
		SQLOut.Add(MixMatchRecord.GetEnableFlag());

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_321) == TRUE)
		{
			SQLOut.Add(MixMatchRecord.GetActivationType());
		}
		else
		{
			SQLOut.Add((MixMatchRecord.GetActivationType() == 0) ? 0 : 1);
		}

		COleDateTime oleStartDate = MixMatchRecord.GetStartDate();
		COleDateTime oleEndDate = MixMatchRecord.GetEndDate();

		if (oleStartDate.GetStatus() == COleDateTime::valid && oleEndDate.GetStatus() == COleDateTime::valid)
		{
			SQLOut.Add(oleStartDate.Format("%Y%m%d"));
			SQLOut.Add(oleEndDate.Format("%Y%m%d"));
		}
		else
		{
			SQLOut.Add("20050101");
			SQLOut.Add("20050101");
		}

		int nAction = MixMatchRecord.GetAction();

		SQLOut.Add(nAction);

		switch (nAction)
		{
		case MIXMATCH_ACTION_DISCOUNT_PLU:
		case MIXMATCH_ACTION_FIXCOST_PLU:
		case MIXMATCH_ACTION_FIXCOST_PLU_2:
			SQLOut.Add(nSharpPluNo);
			break;

		case MIXMATCH_ACTION_FIXCOST_AMOUNT:
			SQLOut.Add(MixMatchRecord.GetActionCost());
			break;

		case MIXMATCH_ACTION_PROMO:
			SQLOut.Add(MixMatchRecord.GetActionPromo());
			break;

		case MIXMATCH_ACTION_FREE_ITEM:
			SQLOut.Add(MixMatchRecord.GetActionBucketFreeItem());
			break;

		case MIXMATCH_ACTION_FREE_BUCKET:
			SQLOut.Add(MixMatchRecord.GetActionBucketFreeBucket());
			break;

		case MIXMATCH_ACTION_FIXCOST_BUCKET:
			SQLOut.Add(MixMatchRecord.GetActionCost());
			break;

		case MIXMATCH_ACTION_NONE:
		default:
			SQLOut.Add(1);
			break;
		}

		SQLOut.Add(MixMatchRecord.GetOfferGroup1());
		SQLOut.Add(MixMatchRecord.GetOfferGroup2());
		SQLOut.Add(MixMatchRecord.GetOfferGroup3());
		SQLOut.Add(MixMatchRecord.GetOfferGroup4());
		SQLOut.Add(MixMatchRecord.GetOfferGroup5());
		SQLOut.Add(MixMatchRecord.GetBucketCount1());
		SQLOut.Add(MixMatchRecord.GetBucketCount2());
		SQLOut.Add(MixMatchRecord.GetBucketCount3());
		SQLOut.Add(MixMatchRecord.GetBucketCount4());
		SQLOut.Add(MixMatchRecord.GetBucketCount5());
		SQLOut.Add(MixMatchRecord.GetMinSpend(), 2);
		SQLOut.Add(MixMatchRecord.GetDisplayName());

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_614) == TRUE)
		{
			SQLOut.Add(MixMatchRecord.GetOrderMode());
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_992) == TRUE)
		{
			switch (nAction)
			{
			case MIXMATCH_ACTION_FIXCOST_BUCKET:
				SQLOut.Add(MixMatchRecord.GetActionBucketFixedBucket());
				break;

			default:
				SQLOut.Add(0);
				break;
			}
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1092) == TRUE)
		{
			SQLOut.Add(MixMatchRecord.GetUseGate());
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1384) == TRUE)
		{
			SQLOut.Add(MixMatchRecord.GetAutoLockFlag());
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_3_18) == TRUE)
		{
			SQLOut.Add(MixMatchRecord.GetUseCurrentPriceBandFlag());
		}

		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}
}

#/**********************************************************************/

void CCommsExportPromoEtc::AppendPromos( int nSPOSVersion, bool bLegacyAnalysis, CSQLOut& SQLOut )
{
	int nSize = DataManager.Promotion.GetSize();
	for (int n = 0; n < nSize; n++)
	{
		StatusProgress.SetPos(n, nSize);

		CPromotionsCSVRecord Promo;
		DataManager.Promotion.GetAt(n, Promo);

		int nLegacyCats = 0;
		CMinimumSPOSVersion Version;
		Promo.CheckMinimumSPOSVersion(Version, nLegacyCats);

		if (TRUE == Version.m_bInvalidData)
		{
			continue;
		}

		int nErrorType = -1;
		Version.CheckSPOSVersion(nSPOSVersion, nErrorType);

		if (nErrorType != -1)
		{
			TallyDownloadExclusion(NODE_PROMOTION, nErrorType);
			continue;
		}

		SQLOut.Add(Promo.GetPromoNo());
		SQLOut.Add(Promo.GetPromoType());
		SQLOut.Add(Promo.GetPercent(), 2);
		SQLOut.Add(Promo.GetMarkdown(), 2);
		SQLOut.Add(Promo.GetTrip(), 2);
		SQLOut.Add(Promo.GetDeptNoEpos());
		SQLOut.Add(Promo.GetGratuityFlag());
		SQLOut.Add(Promo.GetSPOSApplyTaxFlag());
		SQLOut.Add(Promo.GetDisplayName());
		SQLOut.Add(Promo.GetUsage());

		if (TRUE == bLegacyAnalysis)
		{
			SQLOut.Add(nLegacyCats);
		}
		else
		{
			SQLOut.Add(1);
			SQLOut.Add(Promo.GetAnalysisList(TRUE));
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_423) == TRUE)
		{
			SQLOut.Add(Promo.GetMyPromoOnPromoFlag());
			SQLOut.Add(Promo.GetPromoOnMyPromoFlag());
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2455) == TRUE)
		{
			SQLOut.Add(Promo.GetReasonsFlag());
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1071) == TRUE)
		{
			SQLOut.Add(Promo.GetTimeplanMode());
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1096) == TRUE)
		{
			SQLOut.Add(Promo.GetUseGate());
		}

		if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4_1901 ) == TRUE )
		{
			SQLOut.Add( Promo.GetFilterType() );
			SQLOut.Add( Promo.GetFilterOfferGroup() );
		}

		if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4_2161 ) == TRUE )
		{
			SQLOut.Add( Promo.GetFlexType() );
			SQLOut.Add( Promo.GetFlexBoundCovers(1) );
			SQLOut.Add( Promo.GetFlexBoundCovers(2) );
			SQLOut.Add( Promo.GetFlexBoundCovers(3) );
			SQLOut.Add( Promo.GetFlexBoundCovers(4) );
			SQLOut.Add( Promo.GetFlexBoundValue(1), 2 );
			SQLOut.Add( Promo.GetFlexBoundValue(2), 2 );
			SQLOut.Add( Promo.GetFlexBoundValue(3), 2 );
			SQLOut.Add( Promo.GetFlexBoundValue(4), 2 );
			SQLOut.Add( Promo.GetFlexPercent(0), 2 );
			SQLOut.Add( Promo.GetFlexPercent(1), 2 );
			SQLOut.Add( Promo.GetFlexPercent(2), 2 );
			SQLOut.Add( Promo.GetFlexPercent(3), 2 );
			SQLOut.Add( Promo.GetFlexPercent(4), 2 );
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1618) == TRUE)
		{
			Promo.ValidateUpToFlag();
			SQLOut.Add(Promo.GetUpToFlag());
		}

		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}
}

/**********************************************************************/

void CCommsExportPromoEtc::AppendOfferGroup(CSQLOut& SQLOut)
{
	int nSize = DataManager.OfferGroup.GetSize();
	for (int n = 0; n < nSize; n++)
	{
		StatusProgress.SetPos(n, nSize);

		COfferGroupsCSVRecord Group(NODE_OFFER_GROUP);
		DataManager.OfferGroup.GetAt(n, Group);

		SQLOut.Add(Group.GetGroupNo());
		SQLOut.Add(Group.GetGroupName());

		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}
}

/**********************************************************************/

void CCommsExportPromoEtc::AppendOfferGroupEntry(int nSPOSVersion, CSQLOut& SQLOut)
{
	int nSize = DataManager.OfferGroupEntry.GetSize();
	for (int n = 0; n < nSize; n++)
	{
		StatusProgress.SetPos(n, nSize);

		COfferGroupEntriesCSVRecord Entry;
		DataManager.OfferGroupEntry.GetAt(n, Entry);

		__int64 nSharpPluNo = 0;
		bool bModifiable = FALSE;
		CMinimumSPOSVersion Version;
		Entry.CheckMinimumSPOSVersion(Version, nSharpPluNo, bModifiable);

		int nErrorType = -1;
		Version.CheckSPOSVersion(nSPOSVersion, nErrorType);

		if (nErrorType != -1)
		{
			TallyDownloadExclusion(NODE_OFFER_ENTRY, nErrorType);
			continue;
		}

		int nEntryType = Entry.GetEntryType();
		switch (nEntryType)
		{
		case OFFERENTRY_PLU:
		case OFFERENTRY_PLU_EXCLUDE:
			if (TRUE == bModifiable)
			{
				int nMask = 1;
				int nFlags = Entry.GetModifierFlags();

				for (int nMod = 0; nMod <= 9; nMod++)
				{
					if ((nFlags & nMask) != 0)
					{
						SQLOut.Add(Entry.GetGroupNo());
						SQLOut.Add(nEntryType);
						SQLOut.Add(nSharpPluNo + nMod);
						SQLOut.WriteInsert();
						TallyDownloadAccept();
					}
					nMask *= 2;
				}
			}
			else
			{
				SQLOut.Add(Entry.GetGroupNo());
				SQLOut.Add(nEntryType);
				SQLOut.Add(nSharpPluNo);
				SQLOut.WriteInsert();
				TallyDownloadAccept();
			}
			break;

		case OFFERENTRY_DEPT:
		case OFFERENTRY_DEPT_SAMEPLU:
			SQLOut.Add(Entry.GetGroupNo());
			SQLOut.Add(nEntryType);
			SQLOut.Add(Entry.GetEntryDeptNo());
			SQLOut.WriteInsert();
			TallyDownloadAccept();
			break;

		case OFFERENTRY_GROUP:
			SQLOut.Add(Entry.GetGroupNo());
			SQLOut.Add(nEntryType);
			SQLOut.Add(Entry.GetEntryGroupNo());
			SQLOut.WriteInsert();
			TallyDownloadAccept();
			break;
		}
	}
}

/**********************************************************************/

void CCommsExportPromoEtc::AppendWalletStampRule(int nSPOSVersion,CSQLOut& SQLOut)
{
	int nSize = DataManager.WalletStampRule.GetSize();
	for (int nRuleIdx = 0; nRuleIdx < nSize; nRuleIdx++)
	{
		StatusProgress.SetPos(nRuleIdx, nSize);

		CWalletStampRuleCSVRecord Rule;
		DataManager.WalletStampRule.GetAt(nRuleIdx, Rule);

		SQLOut.Add(Rule.GetRuleID());
		
		switch (SysInfo.GetLoyaltyOrSmartPayType())
		{
		case LOYALTY_TYPE_SMP_V4:
			SQLOut.Add(1);
			break;

		case LOYALTY_TYPE_LOY_V4:
		default:
			SQLOut.Add(Rule.GetEdition());
			break;
		}

		SQLOut.Add(Rule.GetEnableFlag());
		SQLOut.Add(Rule.GetOfferGroupNo());
		SQLOut.Add(Rule.GetThreshold());
		SQLOut.Add(Rule.GetExpiry());
		SQLOut.Add(Rule.GetDescription());

		switch (SysInfo.GetLoyaltyOrSmartPayType())
		{
		case LOYALTY_TYPE_SMP_V4:
			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1579) == TRUE)
			{
				SQLOut.Add(1);
			}
			SQLOut.Add(3);
			break;

		case LOYALTY_TYPE_LOY_V4:
		default:
			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1579) == TRUE)
			{
				SQLOut.Add(0);
			}
			SQLOut.Add(1);
			break;
		}

		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}
}

/**********************************************************************/

int CCommsExportPromoEtc::PrepareSPOSFile(int nSPOSVersion)
{
	CArray<int, int> arraySubJobs;

	switch (m_nDownloadType)
	{
	case DNTYPE_MIXMATCH:
		m_strSourceFilename = "MxMatV3.sql";
		arraySubJobs.Add(DN_SUBJOB_MIXMATCH);
		arraySubJobs.Add(DN_SUBJOB_OFFERGROUP);
		arraySubJobs.Add(DN_SUBJOB_OFFERENTRY);

		if (EcrmanOptions.GetFeaturesPromotionFlag() == TRUE)
		{
			arraySubJobs.Add(DN_SUBJOB_PROMO);
		}

		break;

	case DNTYPE_WALLETSTAMPRULE:
		m_strSourceFilename = "StampOffer.sql";
		arraySubJobs.Add(DN_SUBJOB_WALLETSTAMPRULE);
		arraySubJobs.Add(DN_SUBJOB_OFFERGROUP);
		arraySubJobs.Add(DN_SUBJOB_OFFERENTRY);
		break;

	case DNTYPE_ALLOFFER:
		m_strSourceFilename = "AllOffer.sql";
		arraySubJobs.Add(DN_SUBJOB_MIXMATCH);
		arraySubJobs.Add(DN_SUBJOB_WALLETSTAMPRULE);
		arraySubJobs.Add(DN_SUBJOB_OFFERGROUP);
		arraySubJobs.Add(DN_SUBJOB_OFFERENTRY);

		if (EcrmanOptions.GetFeaturesPromotionFlag() == TRUE)
		{
			arraySubJobs.Add(DN_SUBJOB_PROMO);
		}

		break;

	case DNTYPE_PROMO:
	default:
		m_strSourceFilename = "Promo.sql";
		arraySubJobs.Add(DN_SUBJOB_PROMO);
		break;
	}

	CSSFile fileOffer;
	if (fileOffer.Open(m_strSourceFolder + m_strSourceFilename, "wb") == FALSE)
	{
		return COMMS_ERROR_CREATEFILE;
	}

	StatusProgress.Lock();

	for (int n = 0; n < arraySubJobs.GetSize(); n++)
	{
		switch (arraySubJobs.GetAt(n))
		{
		case DN_SUBJOB_MIXMATCH:
			{
				CreateMixMatchHeader(nSPOSVersion);
				CSQLOut SQLOut(fileOffer, "MixMatchPlus", m_strMixMatchHeader);

				if (0 == n)
				{
					SQLOut.WriteFileBegin();
				}
				
				SQLOut.WriteDelete();
				AppendMixMatch(nSPOSVersion, SQLOut);

				if (n == arraySubJobs.GetSize() - 1)
				{
					SQLOut.WriteFileEnd();
				}
			}
			break;

		case DN_SUBJOB_PROMO:
			{
				bool bLegacyAnalysis = (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2133) == FALSE);

				CreatePromoHeader(nSPOSVersion, bLegacyAnalysis);
				CSQLOut SQLOut(fileOffer, "Promos", m_strPromoHeader);

				if (0 == n)
				{
					SQLOut.WriteFileBegin();
				}

				SQLOut.WriteDelete();
				AppendPromos(nSPOSVersion, bLegacyAnalysis, SQLOut);

				if (n == arraySubJobs.GetSize() - 1)
				{
					SQLOut.WriteFileEnd();
				}
			}
			break;

		case DN_SUBJOB_OFFERGROUP:
			{
				CreateOfferGroupHeader();
				CSQLOut SQLOut(fileOffer, "OfferGroups", m_strOfferGroupHeader);

				if (0 == n)
				{
					SQLOut.WriteFileBegin();
				}

				SQLOut.WriteDelete();
				AppendOfferGroup(SQLOut);

				if (n == arraySubJobs.GetSize() - 1)
				{
					SQLOut.WriteFileEnd();
				}
			}
			break;

		case DN_SUBJOB_OFFERENTRY:
			{
				CreateOfferGroupEntryHeader();
				CSQLOut SQLOut(fileOffer, "OfferGroupEntries", m_strOfferGroupEntryHeader);
				
				if (0 == n)
				{
					SQLOut.WriteFileBegin();
				}

				SQLOut.WriteDelete();
				AppendOfferGroupEntry(nSPOSVersion, SQLOut);

				if (n == arraySubJobs.GetSize() - 1)
				{
					SQLOut.WriteFileEnd();
				}			
			}
			break;

		case DN_SUBJOB_WALLETSTAMPRULE:
			{
				CreateWalletStampRuleHeader(nSPOSVersion);
				CSQLOut SQLOut(fileOffer, "StampOffers", m_strWalletStampRuleHeader);

				if (0 == n)
				{
					SQLOut.WriteFileBegin();
				}

				SQLOut.WriteDelete();
				AppendWalletStampRule(nSPOSVersion,SQLOut);

				if (n == arraySubJobs.GetSize() - 1)
				{
					SQLOut.WriteFileEnd();
				}
			}
			break;
		}
	}

	StatusProgress.Unlock();

	fileOffer.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/

