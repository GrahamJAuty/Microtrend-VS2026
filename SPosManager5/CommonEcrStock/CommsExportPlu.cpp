/**********************************************************************/
#include "..\SPosEcrManager5\PluMonitorEcrman.h"
/**********************************************************************/
#include "..\SPosStockManager5\PluMonitorStockman.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\PluTypeHelpers.h"
/**********************************************************************/
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
#include "SPOSVersions.h"
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "OPGroupArray.h"
#include "NetworkCSVArray.h"
#include "PriceSetItemManager.h"
#include "SpecialChangesHelper.h"
#include "SQLOut.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CommsExportPlu.h"
/**********************************************************************/

CCommsExportPlu::CCommsExportPlu( CJobListCSVArray& JobArray, CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
	m_nPriceSet = JobInfo.GetPriceSet();
	m_strEffectiveDate = JobArray.GetDateFolder( JobInfo );

	if ( m_strEffectiveDate == "" )
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		m_strEffectiveDate.Format( "%4.4d%2.2d%2.2d", timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay() );
	}
}

/**********************************************************************/

int CCommsExportPlu::PrepareFile(int nParam1)
{
	if (m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize())
	{
		return COMMS_ERROR_BADLOCIDX;
	}

	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo(m_nLocIdx);
	if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
	{
		return COMMS_ERROR_BADDBLINK;
	}

	CDataManagerInfo info;
	DataManagerNonDb.OpenOPGroupSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPGroupDatabase(nDbIdx, DB_READONLY, info);
	DataManagerNonDb.OpenOPGroupLocation(m_nLocIdx, DB_READONLY, info);
	DataManager.OpenModifier(DB_READONLY, info);

	int nResult = PrepareFileInternal(nParam1, nDbIdx);

	DataManagerNonDb.CloseOPGroupSystem(info);
	DataManagerNonDb.CloseOPGroupDatabase(info);
	DataManagerNonDb.CloseOPGroupLocation(info);
	DataManager.CloseModifier(info);

	return nResult;
}

/**********************************************************************/

int CCommsExportPlu::PrepareFileInternal( int nParam1, int nDbIdx )
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

	m_strSourceFolder = dbNetwork.GetFolderPathData ( m_nNetworkIdx );
	m_strSourceFolder += "\\";

	m_strDestFolder = LocationMasterMap.GetDownloadFolder( m_nLocIdx, m_nTerminalNo, m_bImmediate );
	
	if (m_nPriceSet < 0 || m_nPriceSet > dbLocation.GetPluPriceSetCount(m_nLocIdx))
	{
		return COMMS_ERROR_PRICESETRANGE;
	}

	int nResult = COMMS_ERROR_NODATA;

	if ( m_bFromMemory == FALSE )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseQuickNoFiles( nDbIdx, TRUE );
		DataManager.ReopenPlu( info );
		DataManager.ReopenModifier( info );
	}

#ifdef STOCKMAN_SYSTEM

	CPluMonitorStockman PluMonitor;

	for ( int nPluIdx = 0; nPluIdx < DataManager.Plu.GetSize(); nPluIdx++ )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		PluMonitor.InsertRecord( PluRecord, "", FALSE );
	}

#else

	CPluMonitorEcrman PluMonitor;

	for ( int nPluIdx = 0; nPluIdx < DataManager.Plu.GetSize(); nPluIdx++ )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		PluMonitor.InsertRecord( PluRecord, FALSE );
	}

#endif

	if ( m_bFromMemory == FALSE )
	{
		if ( PluMonitor.GetSize() != 0 )
		{
			//SELECT PLU PRICE SET IF REQUIRED
			int nPluPriceSet = -1;

			if (m_nPriceSet != 0)
			{
				nPluPriceSet = m_nPriceSet;
			}
			else if (EcrmanOptions.GetPluPriceSetsFlag())
			{
				nPluPriceSet = dbLocation.GetDefaultPriceSet(m_nLocIdx);
			}

			//READ IN LOCAL PLU DATA IF AVAILABLE
			PriceSetItemManager.SelectPriceSet ( m_nLocIdx, nPluPriceSet );
			PriceSetItemManager.ReadPriceSetFile( TRUE, PluMonitor );
		}
	}

	if ( PluMonitor.GetSize() != 0 )
	{
		int nSPOSVersion = dbNetwork.GetSPOSVersion( m_nNetworkIdx );
		
		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4) == TRUE)
		{
			nResult = PrepareSPOSFile(nSPOSVersion, PluMonitor, nParam1);
		}
		else
		{
			nResult = COMMS_ERROR_CREATEFILE;
		}
	}

	return nResult;
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
int CCommsExportPlu::PrepareSPOSFile( int nSPOSVersion, CPluMonitorStockman& PluMonitor, int nParam1 )
/**********************************************************************/
#else
/**********************************************************************/
int CCommsExportPlu::PrepareSPOSFile(int nSPOSVersion, CPluMonitorEcrman& PluMonitor, int nParam1)
/**********************************************************************/
#endif
/**********************************************************************/
{
	CString strLastDownloadDate = dbLocation.GetPluPresetDate(m_nLocIdx, m_nTerminalNo);

	if (TRUE == m_bImmediate)
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		m_strSourceFilename.Format("PROD_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d.SQL",
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond());
	}
	else
	{
		m_strSourceFilename = "PROD.SQL";
	}

	CSSFile filePlu;
	if (filePlu.Open(m_strSourceFolder + m_strSourceFilename, "wb") == FALSE)
	{
		return COMMS_ERROR_CREATEFILE;
	}

	CString strHeaderPlu;
	CString strHeaderKPImage;
	CStringArray arrayHeaderLinkPlu;

	{
		CCSV csvHeader;
		csvHeader.Add("[PLU]");
		csvHeader.Add("[Description]");

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1216) == TRUE)
		{
			csvHeader.Add("[ShortDescription]");
		}

		csvHeader.Add("[Price1]");
		csvHeader.Add("[Price2]");
		csvHeader.Add("[Price3]");
		csvHeader.Add("[Price4]");

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1071) == TRUE)
		{
			csvHeader.Add("[Price5]");
			csvHeader.Add("[Price6]");
			csvHeader.Add("[Price7]");
			csvHeader.Add("[Price8]");
			csvHeader.Add("[Price9]");
			csvHeader.Add("[Price10]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1579) == TRUE)
		{
			csvHeader.Add("[StampWeight]");
		}

		csvHeader.Add("[Dept]");
		csvHeader.Add("[VAT]");
		csvHeader.Add("[ProdType]");

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1300) == TRUE)
		{
			csvHeader.Add("[ExternalProcess]");
		}

		csvHeader.Add("[ProdCat]");
		csvHeader.Add("[ListMenu]");
		csvHeader.Add("[ListExtra]");
		csvHeader.Add("[ListQual]");
		csvHeader.Add("[Dsp1]");
		csvHeader.Add("[Dsp2]");
		csvHeader.Add("[Dsp3]");
		csvHeader.Add("[Dsp4]");
		csvHeader.Add("[Dsp5]");
		csvHeader.Add("[Dsp6]");
		csvHeader.Add("[Dsp7]");
		csvHeader.Add("[Dsp8]");
		csvHeader.Add("[Dsp9]");

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1087) == TRUE)
		{
			csvHeader.Add("[Dsp10]");
			csvHeader.Add("[Dsp11]");
			csvHeader.Add("[Dsp12]");
			csvHeader.Add("[Dsp13]");
			csvHeader.Add("[Dsp14]");
			csvHeader.Add("[Dsp15]");
			csvHeader.Add("[Dsp16]");
		}

		csvHeader.Add("[DspPriority]");
		csvHeader.Add("[Discount]");
		csvHeader.Add("[PriceType]");
		csvHeader.Add("[FreeText]");
		csvHeader.Add("[DiscountStaff]");

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_3_104) == TRUE)
		{
			csvHeader.Add("[AccountReqd]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4) == TRUE)
		{
			csvHeader.Add("[AllowModifier]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_321) == TRUE)
		{
			csvHeader.Add("[AllowTracking]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_389) == TRUE)
		{
			csvHeader.Add("[AllowPLUEANOnReceipt]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_494) == TRUE)
		{
			csvHeader.Add("[ShowDescriptionWithFreetext]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_634) == TRUE)
		{
			csvHeader.Add("[LinkType]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1605) == TRUE)
		{
			csvHeader.Add("[LinkTallyMode]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_719) == TRUE)
		{
			csvHeader.Add("[AgeVerify]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1392) == TRUE)
		{
			csvHeader.Add("[AutoQuantity]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_959) == TRUE)
		{
			csvHeader.Add("[KPNotAlone]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1525) == TRUE)
		{
			csvHeader.Add("[KPConsolidateTop]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2606) == TRUE)
		{
			csvHeader.Add("[AllergyCode1]");
			csvHeader.Add("[AllergyCode2]");
			csvHeader.Add("[AllergyCode3]");
			csvHeader.Add("[AllergyCode4]");
			csvHeader.Add("[AllergyCode5]");
			csvHeader.Add("[AllergyCode6]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_714) == TRUE)
		{
			csvHeader.Add("[ImageFilename]");
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4) == TRUE)
		{
			csvHeader.Add("[V$SoldOut]");
		}

		strHeaderPlu = csvHeader.GetLine();

		csvHeader.RemoveAll();
		csvHeader.Add("[KPImage]");
		csvHeader.Add("[KPImagePath]");

		strHeaderKPImage = ",";
		strHeaderKPImage += csvHeader.GetLine();

		for (int n = 1; n <= MAX_LINK_PLU_ITEMS; n++)
		{
			CString strField = "";
			strField.Format(",[TiedPLU%d]", n);
			arrayHeaderLinkPlu.Add(strField);
		}
	}

	filePlu.WriteLine("-->FILEBEGIN");
	filePlu.WriteLine("-->TABLE: Products");

	//REMOVE EXISTING PRODUCTS FOR DOWNLOAD ALL WITH CLEAR
	if (0 == nParam1)
	{
		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4) == TRUE)
		{
			filePlu.WriteLine("DELETE FROM Products WHERE Plu<100000000000000");
		}
		else
		{
			filePlu.WriteLine("DELETE FROM Products WHERE Plu<99999000");
		}

		filePlu.WriteLine("--->GO");
	}

	CSQLOut SQLOut(filePlu, "Products", strHeaderPlu);

	StatusProgress.Lock();
	int nSize = PluMonitor.GetSize();
	bool bGotItemToDownload = FALSE;
	bool bPluByDate = (EcrmanOptions.GetPluDownloadByDateFlag());

	for (int nPluIdx = 0; nPluIdx < nSize; nPluIdx++)
	{
		StatusProgress.SetPos(nPluIdx, nSize);

		//EXCLUDED PRICE SET ITEMS
		if (PriceSetItemManager.IsPluMaster() == FALSE)
		{
			if (PriceSetItemManager.GetPluStatus(nPluIdx) == LOCALPLU_EXCLUDE)
			{
				continue;
			}
		}

		CPluCSVRecord PluRecord;
		PluMonitor.GetAt(nPluIdx, PluRecord);

		if (dbLocation.CheckPluDownloadRange(m_nLocIdx, PluRecord.GetPluNoInt()) == FALSE)
		{
			continue;
		}

		if (EcrmanOptions.GetPluInactiveNoDownloadFlag() == TRUE)
		{
			if (PluRecord.GetActiveItemFlag() == FALSE)
			{
				continue;
			}
		}

		{
			int nOPGroup = PluRecord.GetOrderPrinterGroup();

			if (nOPGroup != 0)
			{
				COPGroupMiniInfo infoMini;
				DataManagerNonDb.OPGroupLocation.GetOPGroup(nOPGroup - 1, infoMini);

				if (TRUE == infoMini.m_bDefer)
				{
					DataManagerNonDb.OPGroupDatabase.GetOPGroup(nOPGroup - 1, infoMini);
				}

				if (TRUE == infoMini.m_bDefer)
				{
					DataManagerNonDb.OPGroupSystem.GetOPGroup(nOPGroup - 1, infoMini);
				}

				COPGroupInfo infoGroup;
				infoGroup.SetEnableFlagsSystem(infoMini.m_nFlags);

				for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
				{
					PluRecord.SetOrderPrinterFlag(n, infoGroup.GetEnableSystem(n));
				}
			}
		}

		bool bBasePrinters[PLU_ORDER_PRINTERS_NOW];
		for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
		{
			bBasePrinters[n] = PluRecord.GetOrderPrinterFlag(n);
		}

		{
			bool bIncludeItem = TRUE;

			CString strModifiedDate = PluRecord.GetModifiedDateTimePlu();
			CString strDownloadDate = PluRecord.GetDownloadDatePlu();

			//CHANGES ONLY
			if (2 == nParam1)
			{
				bool bModifiedSinceDownload = FALSE;
				if (DealerFlags.GetSpecialChangesOnlyFlag() == TRUE)
				{
					bModifiedSinceDownload = SpecialChangesHelper.HasPluChanged(PluRecord);
				}
				else
				{
					bModifiedSinceDownload = (strModifiedDate > strLastDownloadDate);
				}

				bool bRejectedByDownloadDate = FALSE;

				if (TRUE == bPluByDate)
				{
					if (m_strEffectiveDate < strDownloadDate)
					{
						bIncludeItem = FALSE;
						bRejectedByDownloadDate = TRUE;
					}
					else if ((strDownloadDate <= strLastDownloadDate) && (FALSE == bModifiedSinceDownload))
					{
						bIncludeItem = FALSE;
					}
				}
				else
				{
					bIncludeItem = bModifiedSinceDownload;
				}

				if (FALSE == bIncludeItem)
				{
					if (DealerFlags.GetSpecialChangesOnlyFlag() == FALSE)
					{
						if (PriceSetItemManager.IsPluMaster() == FALSE)
						{
							if (TRUE == bRejectedByDownloadDate)
							{
								if (PriceSetItemManager.GetPluStatus(m_nLocIdx) == LOCALPLU_LOCAL)
								{
									bIncludeItem = TRUE;
								}
							}
						}
					}
				}
			}
			else
			{
				if ((TRUE == bPluByDate) && (m_strEffectiveDate < strDownloadDate))
				{
					bIncludeItem = FALSE;
				}
			}

			if (FALSE == bIncludeItem)
			{
				continue;
			}
		}

		__int64 nSharpPluNo = 0;
		bool bModifiable = FALSE;
		CMinimumSPOSVersion Version;
		PluRecord.CheckMinimumSPOSVersion(Version, nSharpPluNo, bModifiable);

		int nErrorType = -1;
		Version.CheckSPOSVersion(nSPOSVersion, nErrorType);

		if (nErrorType != -1)
		{
			TallyDownloadExclusion(NODE_PLU, nErrorType);
			continue;
		}

		int nModMax = (bModifiable ? 9 : 0);

		//SPECIAL CASE HANDLING FOR DEPOSIT ITEMS
		//(1) SET PRICES TO ZERO BEFORE DOWNLOAD
		//(2) DO NOT DOWNLOAD MODIFIERS	
		if (PluRecord.GetMicrotrendPluType() == 7)
		{
			nModMax = 0;

			for (int nPrice = 0; nPrice < 10; nPrice++)
			{
				PluRecord.SetPrice(nPrice, 0.0);
			}

			PluRecord.SetPriceType(0);
		}

		for (int nMod = 0; nMod <= nModMax; nMod++)
		{
			if (0 != nMod)
			{
				if (PluRecord.GetModifierEnable(nMod) == FALSE)
				{
					continue;
				}

				int nModifierOPG = PluRecord.GetModifierOrderPrinterGroup(nMod);

				if (nModifierOPG != 0)
				{
					COPGroupMiniInfo infoMini;
					DataManagerNonDb.OPGroupLocation.GetOPGroup(nModifierOPG - 1, infoMini);

					if (TRUE == infoMini.m_bDefer)
					{
						DataManagerNonDb.OPGroupDatabase.GetOPGroup(nModifierOPG - 1, infoMini);
					}

					if (TRUE == infoMini.m_bDefer)
					{
						DataManagerNonDb.OPGroupSystem.GetOPGroup(nModifierOPG - 1, infoMini);
					}

					COPGroupInfo infoModifierOPG;
					infoModifierOPG.SetEnableFlagsSystem(infoMini.m_nFlags);

					for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
					{
						PluRecord.SetOrderPrinterFlag(n, infoModifierOPG.GetEnableSystem(n));
					}
				}
				else
				{
					for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
					{
						PluRecord.SetOrderPrinterFlag(n, bBasePrinters[n]);
					}
				}
			}

			SQLOut.Add(nSharpPluNo + nMod);
			SQLOut.Add(PluRecord.GetModifierEposText(nMod));

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1216) == TRUE)
			{
				SQLOut.Add(PluRecord.GetModifierKeyText(nMod));
			}

			int nMaxPrice = 4;
			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1071) == TRUE)
			{
				nMaxPrice = 10;
			}

			for (int nPrice = 0; nPrice < nMaxPrice; nPrice++)
			{
				SQLOut.Add(PluRecord.GetModifierPrice(nMod, nPrice), 2);
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1579) == TRUE)
			{
				int nStampWeight = PluRecord.GetStampWeight();
				if (nMod != 0)
				{
					double dStampWeight = nStampWeight;
					dStampWeight *= DataManager.Modifier.GetMultiplierForQty(nMod);
					CString strStampWeight = "";
					strStampWeight.Format("%.0f", dStampWeight);
					nStampWeight = atoi(strStampWeight);
				}

				SQLOut.Add(nStampWeight);
			}

			SQLOut.Add(PluRecord.GetModifierDeptNoForReport(nMod));

			{
				CString strTaxBand = PluRecord.GetBaseTaxBandString();

				if (nMod != 0)
				{
					CString strModTaxBand = PluRecord.GetModifierTaxBandStringRaw(nMod);
					if ((strModTaxBand.GetLength() == 1) && (strModTaxBand >= "A") && (strModTaxBand <= "L"))
					{
						strTaxBand = strModTaxBand;
					}
				}

				SQLOut.Add(strTaxBand);
			}

			{
				int nPluType = PluRecord.GetMicrotrendPluType();

				if (nMod != 0)
				{
					int nModPluType = PluRecord.GetModifierMicrotrendPluType(nMod);

					if (nModPluType != -1)
					{
						nPluType = nModPluType;
					}
				}

				SQLOut.Add(CPluTypeHelpers::MicrotrendToSPOS(nPluType));
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1300) == TRUE)
			{
				SQLOut.Add(PluRecord.GetExternalProcess());
			}

			SQLOut.Add(PluRecord.GetModifierAnalysisCategoryForReport(nMod) - 1);

			{
				int nFirstMenu = PluRecord.GetFirstMenu();

				if (nMod != 0)
				{
					int nModFirstMenu = PluRecord.GetModifierFirstMenu(nMod);

					if (nModFirstMenu >= 0)
					{
						nFirstMenu = nModFirstMenu;
					}
				}

				SQLOut.Add(nFirstMenu);
			}

			{
				int nFirstExtra = PluRecord.GetFirstExtra();

				if (nMod != 0)
				{
					int nModFirstExtra = PluRecord.GetModifierFirstExtra(nMod);

					if (nModFirstExtra >= 0)
					{
						nFirstExtra = nModFirstExtra;
					}
				}

				SQLOut.Add(nFirstExtra);
			}

			{
				int nFirstQualifier = PluRecord.GetFirstQualifier();

				if (nMod != 0)
				{
					int nModFirstQualifier = PluRecord.GetModifierFirstQualifier(nMod);

					if (nModFirstQualifier >= 0)
					{
						nFirstQualifier = nModFirstQualifier;
					}
				}

				SQLOut.Add(nFirstQualifier);
			}

			for (int n = 0; n <= 8; n++)
			{
				SQLOut.Add(PluRecord.GetOrderPrinterFlag(n));
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1087) == TRUE)
			{
				for (int n = 9; n <= 15; n++)
				{
					SQLOut.Add(PluRecord.GetOrderPrinterFlag(n));
				}
			}

			SQLOut.Add(PluRecord.GetPriority());
			SQLOut.Add(PluRecord.GetDiscountableFlag());
			SQLOut.Add(PluRecord.GetPriceType());
			SQLOut.Add(PluRecord.GetFreeTextFlag());
			SQLOut.Add(PluRecord.GetStaffDiscountFlag());

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_3_104) == TRUE)
			{
				SQLOut.Add(PluRecord.GetAccountRequiredFlag());
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4) == TRUE)
			{
				SQLOut.Add(PluRecord.GetAllowModifierFlag() && bModifiable && ( 0 == nMod ));
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_321) == TRUE)
			{
				SQLOut.Add(PluRecord.GetSalesTrackingFlag());
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_389) == TRUE)
			{
				SQLOut.Add(PluRecord.GetEANOnReceiptFlag());
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_494) == TRUE)
			{
				if (PluRecord.GetFreeTextFlag() == TRUE)
				{
					SQLOut.Add(PluRecord.GetFreeTextDescriptionFlag());
				}
				else
				{
					SQLOut.Add(TRUE);
				}
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_634) == TRUE)
			{
				SQLOut.Add(PluRecord.GetLinkType());
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_1605) == TRUE)
			{
				if (PluRecord.GetLinkType() == 1)
				{
					SQLOut.Add(PluRecord.GetLinkSellTiedItemFlag());
				}
				else
				{
					SQLOut.Add(0);
				}
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_719) == TRUE)
			{
				SQLOut.Add(PluRecord.GetAgeVerificationType());
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1392) == TRUE)
			{
				SQLOut.Add(PluRecord.GetAutoCoversQtyFlag());
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_959) == TRUE)
			{
				SQLOut.Add(PluRecord.GetKPNotAloneFlag());
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1525) == TRUE)
			{
				SQLOut.Add(PluRecord.GetKPConsolidateTopItemFlag());
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2606) == TRUE)
			{
				for (int n = 0; n <= 5; n++)
				{
					SQLOut.Add(PluRecord.GetAllergenNo(n));
				}
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_714) == TRUE)
			{
				SQLOut.Add(PluRecord.GetImageFilename());
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4) == TRUE)
			{
				SQLOut.Add(PluRecord.GetSoldOutFlag());
			}

			CString strHeader = strHeaderPlu;

			bool bWantKPImage;

			if (0 == nMod)
			{
				bWantKPImage = (PluRecord.GetKPImagePrintFlag());
			}
			else
			{
				bWantKPImage = (PluRecord.GetKPImageModifierFlag());
			}

			CString strImageData = "";
			CString strImageFilename = "";

			if (TRUE == bWantKPImage)
			{
				switch (GetKPImage(nSharpPluNo, strImageData, strImageFilename))
				{
				case IMAGE_TYPE_BMP:
					strImageFilename += ".bmp";
					break;

				case IMAGE_TYPE_GIF:
					strImageFilename += ".gif";	
					break;

				case IMAGE_TYPE_JPG:
					strImageFilename += ".jpg";	
					break;

				case IMAGE_TYPE_NONE:
				default:				
					bWantKPImage = FALSE;
					break;
				}
			}

			if (TRUE == bWantKPImage)
			{
				strHeader += strHeaderKPImage;
				SQLOut.AddHexString(strImageData);
				SQLOut.Add(strImageFilename);
			}

			if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_634) == TRUE)
			{
				if (0 == nMod)
				{
					CArray<__int64, __int64> arrayLinkPluNo;
					PluRecord.GetLinkTableArray(arrayLinkPluNo);

					for (int n = 0; n < arrayLinkPluNo.GetSize(); n++)
					{
						strHeader += arrayHeaderLinkPlu.GetAt(n);
						SQLOut.Add(arrayLinkPluNo.GetAt(n));
					}
				}
			}

			if (0 != nParam1)
			{
				CString strDelete = "";
				strDelete.Format("DELETE FROM Products WHERE Plu=%I64d", nSharpPluNo + nMod);
				filePlu.WriteLine(strDelete);
				filePlu.WriteLine("--->GO");
			}

			SQLOut.SetHeader(strHeader);
			SQLOut.WriteInsert();
			TallyDownloadAccept();
			bGotItemToDownload = TRUE;

			if (DealerFlags.GetSpecialChangesOnlyFlag() == TRUE)
			{
				SpecialChangesHelper.NoteDownloadedPlu(PluRecord);
			}
		}
	}
	StatusProgress.Unlock();

	filePlu.WriteLine("-->FILEEND");
	filePlu.Close();

	if (FALSE == bGotItemToDownload)
	{
		return COMMS_ERROR_NODATA;
	}
	else
	{
		return COMMS_ERROR_NONE;
	}
}

/**********************************************************************/

int CCommsExportPlu::GetKPImage( __int64 nSharpPluNo, CString& strImageData, CString& strImageFilename )
{
	int nDbIdx = DataManager.GetActiveDbIdx();

	strImageFilename.Format( "%I64d", nSharpPluNo );
	if (strImageFilename.GetLength() <= 8)
	{
		::AddLeading(strImageFilename, 8, '0');
	}
	else
	{
		::AddLeading(strImageFilename, 14, '0');
	}

	CString strFile = "";
	strFile.Format( "%s\\%s.dat",
		dbDatabase.GetFolderPathDataPluImage( nDbIdx ),
		(const char*) strImageFilename );

	return ::GetImageType( strFile, strImageData, TRUE );
}

/**********************************************************************/

void CCommsExportPlu::RenameTempFile()
{
	CString strCurrentFile = m_strSourceFolder + m_strSourceFilename;
	CString strGenericFile = m_strSourceFolder + "PROD.CSV";
	
	CFileRemover FileRemover;
	FileRemover.RemoveFile( strGenericFile );
	::CopyFile( strCurrentFile, strGenericFile, FALSE );
	FileRemover.RemoveFile( strCurrentFile );
}

/**********************************************************************/
