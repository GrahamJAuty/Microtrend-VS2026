/**********************************************************************/
#include "Consolidation.h"
#include "NodeTypes.h"
#include "ReportConsolidationArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DealerFlags.h"
/**********************************************************************/

CDealerFlags::CDealerFlags()
{
	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CDealerFlags::Reset()
{
	m_nReadStatus = READ_STATUS_NONE;

	//REPORT
	m_bLocComboOrder = FALSE; //1
	m_strLocComboOrder = ""; //1
	m_nPriorityDb = 0; //1
	m_nPriorityRp = 0; //1
	m_nPriorityLs = 0; //1
	m_nPriorityLoc = 0; //1
	m_nPrioritySite = 0; //1
	m_nPrioritySp = 0; //1
	m_nPrioritySys = 0; //1
	m_bSalesHistoryDateControl = FALSE; //8
	m_nServerConsolidationType = 2; //9
	m_bHideNoData = FALSE; //17
	m_bPrice1SeparateLine = FALSE; //18
	m_bAssumeCovers = FALSE; //19
	m_bLoyaltyReconReport = FALSE; //20

	//PLU
	m_nAddPluMode = 0;	//1 & 2
	m_bPriceSetExcludeByDefault = FALSE; //6
	m_bSpecialChangesOnly = FALSE; //8
	m_nPluCSVImportType = 0; //9
	m_strDeptZeroText = ""; //10
	m_strGroupZeroText = ""; //10
	m_bLegacyPluExport = FALSE; //12

	//CASHRSP
	m_bUseCashRSPTillNo = FALSE; //1
	m_bCashRSPPromoFix = FALSE; //2
	m_bLogAllCashRSPFiles = FALSE; //3
	m_bCashRSPV19SmartPay = FALSE; //4
	m_bCashRSPFixItemServerZero = FALSE; //6
	m_bAutoRetryTermFileImport = FALSE; //7
	m_bCashRSPCatchDuplicates = FALSE; //8

	//MISC
	m_bIDraughtExport = FALSE; //3
	m_bBatchExport = FALSE; //4
	m_nProdCSVImportExistingMode = 0; //5
	m_nPrinterRedirectMode = 0; //7
	m_bSageExport = FALSE; //8
	m_bCascadeReport = FALSE; //9
	m_bCostleyTransactionLog = FALSE; //15
	m_bWeeklyReportNoZeroSkip = FALSE; //16
	m_nSortCodeRenameType = 0;	//17
	m_bLogFileWrites = FALSE; //18
	m_bReportCostPrice = FALSE; //19
	m_bHoltsDiscount = FALSE; //21
	m_bLocationNotes = FALSE; //25
	m_bSpawnRepman = FALSE; //26
	m_nLIPA_LJMU_ExportType = 0; //27
	m_bLogNonDuplicates = FALSE; //28
	m_bPMSExtendedRoomRate = FALSE; //29
	m_bLogWebAPICalls = FALSE; //30
	m_bRemoveLoyaltyTopup = FALSE; //32
	m_arrayNoLoyaltyTopupPLUs.RemoveAll(); //32
	m_bGlobalPromotion = FALSE; //33
	m_bPosTraySetupNoLockAllUsers = FALSE; //34
	m_bReasonTicksNoDownload = FALSE; //35
	m_bReasonTicksVoidAfter = FALSE; //36
	m_bServerHomeLocation = FALSE; //37
	m_bServerImportExport = FALSE; //38
	m_bRefundFix = FALSE; //39
	m_bIgnoreUnpaidTransactions = FALSE; //40
	m_bRefund2Fix = FALSE; //41
	m_bGenRecExcludeLoyalty = FALSE; //42
	m_bUseDatabaseVATBand = FALSE; //43
	m_bAllDiscountsVATable = FALSE; //44
	m_b15DigitKludgeDeptGroup = FALSE; //45
	m_bGratuityFix = FALSE; //46
	m_bGratuityFix2 = FALSE; //47
	m_bLinkPluFix = FALSE; //48
	m_bDuplicateCheckIgnoreTime = FALSE; //49
	SetFixDepositRecycle(FALSE); //50
	m_bCOFAllDiscount = FALSE; //51

	//LABEL
	m_bEcrTextOnLabels = FALSE; //1
	m_nSPosBarcodeLength = 14; //2
 	
	//GRAPH
	m_bWeekNoGraph = FALSE; //1

#if defined (STOCKMAN_SYSTEM) || defined (POSTRAY_UTILITY)

	//PLU
	m_bImportBarcode = FALSE; //11

	//MISC
	m_bBritanniaHotelsExport = FALSE; //2
	m_nMaxBritanniaPriceLevel = 2; //2
	m_bBritanniaAllItems = FALSE; //2
	m_nSortCodeRenameType = 0; //17
	m_bRebuildStockSalesHistory = FALSE; //20
	m_bDorbiereImport = FALSE; //22
	
	//STOCK
	m_bSuppRef = FALSE; //1
	m_bCopyPluToStock = FALSE; //2
	m_bObsoleteValuationSummary = FALSE; //3
	m_nStockLevelsPluType = 0; //4
	m_nStockLevelsStockType = 0; //5
	m_bTransitionalOrder = FALSE; //6
	m_bStockLevelFilter = FALSE; //7
	m_bExtendedStocktakeReports = FALSE; //8
	m_bObsoleteManualStockSales = FALSE; //9
	m_bWarehouseToggle = FALSE; //10
	m_bExportLiveApparentStock = FALSE; //11
	m_bPeriodDeliveryCostProfit = FALSE; //12
	m_bStockReportUseRecipeCostLink = FALSE; //13

	//AUDIT
	m_bDeliveryLines = FALSE; //1
	m_bDeliveryDetail = FALSE; //2

	//ORDER
	m_bPurchaseOrderLines = FALSE; //1

	//NON FILE
	m_nCurrentStockLevelsAccessType = 0;
	m_bBlockExportLiveApparentStock = FALSE;

#else

	m_bApparentStockDate = FALSE;
	
#endif
}

/**********************************************************************/

void CDealerFlags::CheckFlag( int nBase, const char* szType, CString& strFlag, int& nResult )
{
	if ( 0 == nResult )
	{
		CString strType = szType;
		int nFlagLen = strFlag.GetLength();
		int nTypeLen = strType.GetLength();

		if ( ( nFlagLen > nTypeLen ) && ( strFlag.Left( nTypeLen ) == strType ) )
		{
			CString strNum = strFlag.Right( nFlagLen - nTypeLen );

			if (::TestNumeric(strNum) == TRUE)
			{
				int nNum = atoi(strNum);

				if ((nNum >= 1) && (nNum <= 99))
				{
					nResult = nBase + nNum;
				}
			}
		
			if ( ( 0 == nResult ) && ( nFlagLen > nTypeLen + 4 ) )
			{
				strType += "FLAG";
				
				if ( strFlag.Left( nTypeLen + 4 ) == strType )
				{
					strNum = strFlag.Right( nFlagLen - ( nTypeLen + 4 ) );

					if (::TestNumeric(strNum) == TRUE)
					{
						int nNum = atoi(strNum);

						if ((nNum >= 1) && (nNum <= 99))
						{
							nResult = nBase + nNum;
						}
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CDealerFlags::WriteFlag(bool bFlag, const char* szText, int nExtra)
{
	if (TRUE == bFlag)
	{
		if (nExtra == 9999)
		{
			WriteNextLine(szText);
		}
		else
		{
			CCSV csv;
			csv.Add(szText);
			csv.Add(nExtra);
			WriteNextLine(csv.GetLine());
		}
	}
}

/**********************************************************************/

void CDealerFlags::WriteNextLine( const char* szLine )
{
	if (m_bFromFile == TRUE)
	{
		m_pFile->WriteLine(szLine);
	}
	else
	{
		m_pArray->Add(szLine);
	}
}

/**********************************************************************/

bool CDealerFlags::ReadFromFile()
{
	Reset();

	CFilenameUpdater FnUp(SysFiles::DealerFlags);
	CString strFilename = FnUp.GetFilenameToUse();

	CSSFile fileFlags;
	if (fileFlags.Open(strFilename, "rb") == FALSE)
	{
		if (GetLastError() != ERROR_FILE_NOT_FOUND)
		{
			m_bFatalReadError = TRUE;
			m_nReadStatus = READ_STATUS_FAILED;
		}
		else
		{
			m_nReadStatus = READ_STATUS_NOFILE;
		}

		return FALSE;
	}

	m_bFromFile = TRUE;
	m_pFile = &fileFlags;

	ReadInternal();

	fileFlags.Close();
	m_nReadStatus = READ_STATUS_OK;
	return TRUE;
}

/**********************************************************************/

void CDealerFlags::ReadFromArray( CStringArray& arrayFlags )
{
	int n = m_nReadStatus;

	Reset();

	m_bFromFile = FALSE;
	m_pArray = &arrayFlags;
	m_nArrayPos = 0;

	ReadInternal();

	m_nReadStatus = n;
}

/**********************************************************************/

void CDealerFlags::ReadInternal()
{
	CString strLine;
	while ( ReadNextLine ( strLine ) )
	{
		CCSV csvIn ( strLine );
		CString strFlag = csvIn.GetString(0);
		strFlag.MakeUpper();

		int nFlagType = 0;
		CheckFlag( DEALERFLAG_REPORT, "REPORT", strFlag, nFlagType );
		CheckFlag( DEALERFLAG_PLU, "PLU", strFlag, nFlagType );
		CheckFlag( DEALERFLAG_CASHRSP, "CASHRSP", strFlag, nFlagType );
		CheckFlag( DEALERFLAG_MISC, "MISC", strFlag, nFlagType );
		CheckFlag( DEALERFLAG_LABEL, "LABEL", strFlag, nFlagType );
		CheckFlag( DEALERFLAG_GRAPH, "GRAPH", strFlag, nFlagType );
		CheckFlag( DEALERFLAG_SALESMAN, "SALESMAN", strFlag, nFlagType );
		CheckFlag( DEALERFLAG_STOCK, "STOCK", strFlag, nFlagType );
		CheckFlag( DEALERFLAG_AUDIT, "AUDIT", strFlag, nFlagType );
		CheckFlag( DEALERFLAG_ORDER, "ORDER", strFlag, nFlagType );
		CheckFlag( DEALERFLAG_CUSTOMER, "CUSTOMER", strFlag, nFlagType );

		switch( nFlagType )
		{
		case DEALERFLAG_REPORT1:
			{
				CCSV csvOut;
				for ( int n = 1; n < csvIn.GetSize(); n++ )
				{
					CString strTest = csvIn.GetString(n);
					strTest.MakeUpper();
					AddToLocComboString( csvOut, strTest );
				}
					
				if ( csvOut.GetSize() > 0 )
				{
					m_bLocComboOrder = TRUE;
					m_strLocComboOrder = csvOut.GetLine();
				}
				else
				{
					m_bLocComboOrder = FALSE;
					m_strLocComboOrder = "";
				}
				ProcessLocationSelectorOrder();
			}
			break;

		case DEALERFLAG_REPORT8:	
			m_bSalesHistoryDateControl = TRUE;	
			break;

		case DEALERFLAG_REPORT9:	
			SetServerConsolidationType( csvIn.GetInt(1) );	
			break;

		case DEALERFLAG_REPORT17:	
			m_bHideNoData = TRUE;	
			break;

		case DEALERFLAG_REPORT18:	
			m_bPrice1SeparateLine = TRUE;
			break;

		case DEALERFLAG_REPORT19:
			m_bAssumeCovers = TRUE;	
			break;

		case DEALERFLAG_REPORT20:	
			m_bLoyaltyReconReport = (!m_bFromFile);	
			break;

		case DEALERFLAG_PLU1:		
			m_nAddPluMode = 1;	
			break;

		case DEALERFLAG_PLU2:	
			m_nAddPluMode = 2;	
			break;

		case DEALERFLAG_PLU6:
			m_bPriceSetExcludeByDefault = TRUE;	
			break;

		case DEALERFLAG_PLU8:		
			m_bSpecialChangesOnly = TRUE;	
			break;

		case DEALERFLAG_PLU9:		
			SetPluCSVImportType( csvIn.GetInt(1) );	
			break;
		
		case DEALERFLAG_PLU10:
			m_strDeptZeroText = csvIn.GetString(1);
			m_strGroupZeroText = csvIn.GetString(2);
			break;

		case DEALERFLAG_PLU12:		
			m_bLegacyPluExport = TRUE;	
			break;

		case DEALERFLAG_CASHRSP1:	
			m_bUseCashRSPTillNo = TRUE;
			break;

		case DEALERFLAG_CASHRSP2:
			m_bCashRSPPromoFix = TRUE;	
			break;

		case DEALERFLAG_CASHRSP3:	
			m_bLogAllCashRSPFiles = TRUE;
			break;

		case DEALERFLAG_CASHRSP4:
			m_bCashRSPV19SmartPay = TRUE;
			break;

		case DEALERFLAG_CASHRSP6:	
			m_bCashRSPFixItemServerZero = TRUE;	
			break;

		case DEALERFLAG_CASHRSP7:	
			m_bAutoRetryTermFileImport = (!m_bFromFile);
			break;

		case DEALERFLAG_CASHRSP8:	
			m_bCashRSPCatchDuplicates = TRUE;
			break;

		case DEALERFLAG_MISC3:	
			m_bIDraughtExport = TRUE;
			break;

		case DEALERFLAG_MISC4:	
			m_bBatchExport = TRUE;	
			break;

		case DEALERFLAG_MISC5:
			if (csvIn.GetSize() == 1)
			{
				m_nProdCSVImportExistingMode = 2;
			}
			else
			{
				int nFileMode = csvIn.GetInt(1);
				switch( nFileMode )
				{
				case 1:
				case 2:
				case 3:
					m_nProdCSVImportExistingMode = nFileMode;
					break;

				default:
					m_nProdCSVImportExistingMode = 0;
					break;
				}
			}
			break;

		case DEALERFLAG_MISC7:			
			SetPrinterRedirectMode( csvIn.GetInt(1) );			
			break;

		case DEALERFLAG_MISC8:			
			m_bSageExport = TRUE;	
			break;

		case DEALERFLAG_MISC9:	
			m_bCascadeReport = TRUE;
			break;

		case DEALERFLAG_MISC15:	
			m_bCostleyTransactionLog = TRUE;
			break;

		case DEALERFLAG_MISC16:		
			m_bWeeklyReportNoZeroSkip = TRUE;	
			break;
		
		case DEALERFLAG_MISC17:			
			SetSortCodeRenameType( csvIn.GetInt(1) );			
			break;
		
		case DEALERFLAG_MISC18:			
			m_bLogFileWrites = TRUE;
			break;

		case DEALERFLAG_MISC19:	
			m_bReportCostPrice = TRUE;
			break;

		case DEALERFLAG_MISC21:	
			m_bHoltsDiscount = TRUE;
			break;

		case DEALERFLAG_MISC25:		
			m_bLocationNotes = TRUE;
			break;

		case DEALERFLAG_MISC26:		
			m_bSpawnRepman = TRUE;	
			break;
		
		case DEALERFLAG_MISC27:	
			if ( csvIn.GetSize() == 1 )
			{
				m_nLIPA_LJMU_ExportType = 1;
			}
			else
			{
				Set_LIPA_LJMU_ExportType(csvIn.GetInt(1));
			}
			break;

		case DEALERFLAG_MISC28:		
			m_bLogNonDuplicates = TRUE;	
			break;

		case DEALERFLAG_MISC29:	
			m_bPMSExtendedRoomRate = TRUE;
			break;

		case DEALERFLAG_MISC30:		
			m_bLogWebAPICalls = TRUE;
			break;

		case DEALERFLAG_MISC32:
			{
				m_bRemoveLoyaltyTopup = TRUE;
				ClearNoLoyaltyTopupPLUs();

				for (int nIndex = 1; nIndex < csvIn.GetSize(); nIndex++)
				{
					AddNoLoyaltyTopupPLU(csvIn.GetInt64(nIndex));

					if ( GetNoLoyaltyTopupPLUCount() >= MAX_NOTOPUP_PLU )
					{
						break;
					}
				}
			}
			break;

		case DEALERFLAG_MISC33:	
			m_bGlobalPromotion = TRUE;	
			break;

		case DEALERFLAG_MISC34:		
			m_bPosTraySetupNoLockAllUsers = TRUE;	
			break;

		case DEALERFLAG_MISC35:	
			m_bReasonTicksNoDownload = TRUE;
			break;

		case DEALERFLAG_MISC36:		
			m_bReasonTicksVoidAfter = TRUE;	
			break;

		case DEALERFLAG_MISC37:	
			m_bServerHomeLocation = TRUE;
			break;

		case DEALERFLAG_MISC38:		
			m_bServerImportExport = TRUE;
			break;

		case DEALERFLAG_MISC39:	
			m_bRefundFix = TRUE;	
			break;

		case DEALERFLAG_MISC40:	
			m_bIgnoreUnpaidTransactions = TRUE;	
			break;

		case DEALERFLAG_MISC41:	
			m_bRefund2Fix = TRUE;	
			break;

		case DEALERFLAG_MISC42:	
			m_bGenRecExcludeLoyalty = TRUE;	
			break;

		case DEALERFLAG_MISC43:		
			m_bUseDatabaseVATBand = TRUE;
			break;

		case DEALERFLAG_MISC44:	
			m_bAllDiscountsVATable = TRUE;
			break;

		case DEALERFLAG_MISC45:	
			m_b15DigitKludgeDeptGroup = TRUE;
			break;

		case DEALERFLAG_MISC46:	
			m_bGratuityFix = TRUE;
			break;

		case DEALERFLAG_MISC47:
			m_bGratuityFix2 = TRUE;
			break;

		case DEALERFLAG_MISC48:
			m_bLinkPluFix = TRUE;
			break;

		case DEALERFLAG_MISC49:
			m_bDuplicateCheckIgnoreTime = TRUE;
			break;

		case DEALERFLAG_MISC50:
			SetFixDepositRecycle(TRUE, csvIn.GetInt(1), csvIn.GetInt(2), csvIn.GetInt64(3));
			break;

		case DEALERFLAG_MISC51:
			m_bCOFAllDiscount = TRUE;
			break;

		case DEALERFLAG_LABEL1:	
			m_bEcrTextOnLabels = TRUE;
			break;

		case DEALERFLAG_LABEL2:	
			SetSPosBarcodeLength( csvIn.GetInt(1) );
			break;

		case DEALERFLAG_GRAPH1:	
			m_bWeekNoGraph = TRUE;		
			break;
		
#if defined (STOCKMAN_SYSTEM) || defined (POSTRAY_UTILITY)

		case DEALERFLAG_PLU11:			
			m_bImportBarcode = TRUE;	
			break;

		case DEALERFLAG_MISC2:
			m_bBritanniaHotelsExport = TRUE;
			m_nMaxBritanniaPriceLevel = csvIn.GetInt(1);
			m_bBritanniaAllItems = csvIn.GetBool(2);
			break;

		case DEALERFLAG_MISC20:			
			m_bRebuildStockSalesHistory = TRUE;	
			break;

		case DEALERFLAG_MISC22:	
			m_bDorbiereImport = TRUE;	
			break;

		case DEALERFLAG_STOCK1:	
			m_bSuppRef = TRUE;		
			break;

		case DEALERFLAG_STOCK2:	
			m_bCopyPluToStock = TRUE;
			break;

		case DEALERFLAG_STOCK3:	
			m_bObsoleteValuationSummary = TRUE;	
			break;

		case DEALERFLAG_STOCK4:	
			SetStockLevelsPluType( csvIn.GetInt(1) );	
			break;

		case DEALERFLAG_STOCK5:		
			SetStockLevelsStockType( csvIn.GetInt(1) );	
			break;

		case DEALERFLAG_STOCK6:	
			m_bTransitionalOrder = TRUE;
			break;

		case DEALERFLAG_STOCK7:
			m_bStockLevelFilter = TRUE;	
			break;

		case DEALERFLAG_STOCK8:	
			m_bExtendedStocktakeReports = TRUE;	
			break;

		case DEALERFLAG_STOCK9:		
			m_bObsoleteManualStockSales = TRUE;	
			break;

		case DEALERFLAG_STOCK10:
			m_bWarehouseToggle = TRUE;	
			break;

		case DEALERFLAG_STOCK11:	
			m_bExportLiveApparentStock = TRUE;
			break;

		case DEALERFLAG_STOCK12:	
			m_bPeriodDeliveryCostProfit = TRUE;	
			break;

		case DEALERFLAG_STOCK13:	
			m_bStockReportUseRecipeCostLink = TRUE;	
			break;

		case DEALERFLAG_AUDIT1:		
			m_bDeliveryLines = TRUE;	
			break;

		case DEALERFLAG_AUDIT2:	
			m_bDeliveryDetail = TRUE;	
			break;

		case DEALERFLAG_ORDER1:		
			m_bPurchaseOrderLines = TRUE;	
			break;
#else
		case DEALERFLAG_MISC23:			
			m_bApparentStockDate = TRUE;	
			break;
#endif
		}

		if (strFlag == "PRINTERFLAG999")
		{
			m_nPrinterRedirectMode = 1;
		}

#if defined (STOCKMAN_SYSTEM) || defined (POSTRAY_UTILITY)
		
		if (strFlag == "SUPPREF")
		{
			m_bSuppRef = TRUE;
		}

		if ( GetDorbiereImportFlag() == TRUE )
		{
			CString strFolder = "DORBIERE";
			GetDataProgramPath( strFolder );
			CreateSubdirectory( strFolder );
		}
#endif
	}
}

/**********************************************************************/

bool CDealerFlags::ReadNextLine( CString& strField )
{
	if (m_bFromFile == TRUE)
	{
		return m_pFile->ReadString(strField);
	}
	else if (m_nArrayPos >= m_pArray->GetSize())
	{
		return FALSE;
	}
	else
	{
		strField = m_pArray -> GetAt( m_nArrayPos++ );
		return TRUE;
	}
}

/**********************************************************************/

bool CDealerFlags::CheckNoLoyaltyTopupPLU(__int64 nPluNo)
{
	//IF NO PLUs SPECIFIED THEN TOPUPS ARE EXCLUDED FOR ALL PLUs
	if ( m_arrayNoLoyaltyTopupPLUs.GetSize() == 0 )
	{
		return TRUE;
	}

	for (int nIndex = 0; nIndex < m_arrayNoLoyaltyTopupPLUs.GetSize(); nIndex++)
	{
		if (m_arrayNoLoyaltyTopupPLUs.GetAt(nIndex) == nPluNo)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/

CString CDealerFlags::GetMisc32String()
{
	CString strOut = "";
	if ( m_bRemoveLoyaltyTopup == TRUE )
	{
		CCSV csv;
		csv.Add("MISC32");
		for (int nIndex = 0; nIndex < m_arrayNoLoyaltyTopupPLUs.GetSize(); nIndex++)
		{
			__int64 nPluNo = m_arrayNoLoyaltyTopupPLUs.GetAt(nIndex);

			if ((nPluNo >= Plu::PluNo.Min) && (nPluNo <= Plu::PluNo.Max))
			{
				csv.Add(m_arrayNoLoyaltyTopupPLUs.GetAt(nIndex));
			}

			if ( csv.GetSize() >= MAX_NOTOPUP_PLU + 1 )
			{
				break;
			}
		}
		strOut = csv.GetLine();
	}
	return strOut;
}

/**********************************************************************/

void CDealerFlags::SetPluCSVImportType(int n)
{
	if (n >= 0 && n <= 3)
	{
		m_nPluCSVImportType = n;
	}
}

/**********************************************************************/

void CDealerFlags::SetServerConsolidationType( int n )
{
	if (n >= 0 && n <= 2)
	{
		m_nServerConsolidationType = n;
	}
}

/**********************************************************************/

void CDealerFlags::SetPrinterRedirectMode( int n )
{
	if (n >= 0 && n <= 2)
	{
		m_nPrinterRedirectMode = n;
	}
}

/**********************************************************************/

bool CDealerFlags::WriteToFile()
{
	bool bResult = FALSE;

	if ( DataManager.LockAllUsers() == TRUE )
	{
		CreateDataSubdirectory ( "SysFiles" );
	
		CFilenameUpdater FnUp( SysFiles::DealerFlags );
	
		CSSFile fileFlags;
		if ( fileFlags.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		{
			CString strHeader;
			COleDateTime timeNow = COleDateTime::GetCurrentTime();
	
			strHeader.Format ( "//File generated by Hidden Options Editor on %2.2d/%2.2d/%4.4d at %2.2d:%2.2d:%2.2d",
				timeNow.GetDay(),
				timeNow.GetMonth(),
				timeNow.GetYear(),
				timeNow.GetHour(),
				timeNow.GetMinute(),
				timeNow.GetSecond() );	

			fileFlags.WriteLine( strHeader );

			m_bFromFile = TRUE;
			m_pFile = &fileFlags;
			
			WriteInternal();

			bResult = TRUE;
		}

		DataManager.UnlockAllUsers();
	}

#if defined (STOCKMAN_SYSTEM) || defined (POSTRAY_UTILITY)

	if ( GetDorbiereImportFlag() == TRUE )
	{
		CString strFolder = "DORBIERE";
		GetDataProgramPath( strFolder );
		CreateSubdirectory( strFolder );
		CreateSubdirectory( strFolder + "\\IMPORT" );
		CreateSubdirectory( strFolder + "\\EXPORT" );
	}

#endif

	return bResult;
}

/**********************************************************************/

void CDealerFlags::WriteToArray( CStringArray& arrayFlags )
{
	arrayFlags.RemoveAll();
	m_bFromFile = FALSE;
	m_pArray = &arrayFlags;
	WriteInternal();
}

/**********************************************************************/

void CDealerFlags::WriteInternal()
{
	if (TRUE == m_bLocComboOrder)
	{
		CString strLine = "REPORT1,";
		strLine += m_strLocComboOrder;
		WriteNextLine(strLine);
	}

	WriteFlag(m_bSalesHistoryDateControl, "REPORT8");
	WriteFlag((m_nServerConsolidationType != 2), "REPORT9", m_nServerConsolidationType);
	WriteFlag(m_bHideNoData, "REPORT17");
	WriteFlag(m_bPrice1SeparateLine, "REPORT18");
	WriteFlag(m_bAssumeCovers, "REPORT19");
	WriteFlag(m_bLoyaltyReconReport && (!m_bFromFile), "REPORT20");
	WriteFlag((1 == m_nAddPluMode), "PLU1");
	WriteFlag((2 == m_nAddPluMode), "PLU2");
	WriteFlag(m_bPriceSetExcludeByDefault, "PLU6");
	WriteFlag(m_bSpecialChangesOnly, "PLU8");
	WriteFlag((m_nPluCSVImportType != 0), "PLU9", m_nPluCSVImportType);

	if (m_strDeptZeroText != "" || m_strGroupZeroText != "")
	{
		CCSV csv;
		csv.Add("PLU10");
		csv.Add(m_strDeptZeroText);
		csv.Add(m_strGroupZeroText);
		WriteNextLine(csv.GetLine());
	}

	WriteFlag(m_bLegacyPluExport, "PLU12");
	WriteFlag(m_bUseCashRSPTillNo, "CASHRSP1");
	WriteFlag(m_bCashRSPPromoFix, "CASHRSP2");
	WriteFlag(m_bLogAllCashRSPFiles, "CASHRSP3");
	WriteFlag(m_bCashRSPV19SmartPay, "CASHRSP4");
	WriteFlag(m_bCashRSPFixItemServerZero, "CASHRSP6");
	WriteFlag(m_bAutoRetryTermFileImport && (!m_bFromFile), "CASHRSP7");
	WriteFlag(m_bCashRSPCatchDuplicates, "CASHRSP8");
	WriteFlag(m_bIDraughtExport, "MISC3");
	WriteFlag(m_bBatchExport, "MISC4");

	{
		int nMode = m_nProdCSVImportExistingMode;
		WriteFlag((nMode >= 1) && (nMode <= 3), "MISC5", nMode);
	}

	{
		int nMode = m_nPrinterRedirectMode;
		WriteFlag((nMode >= 1) && (nMode <= 2), "MISC7", nMode);
	}

	WriteFlag(m_bSageExport, "MISC8");
	WriteFlag(m_bCascadeReport, "MISC9");
	WriteFlag(m_bCostleyTransactionLog, "MISC15");
	WriteFlag(m_bWeeklyReportNoZeroSkip, "MISC16");
	WriteFlag((m_nSortCodeRenameType != 0), "MISC17", m_nSortCodeRenameType);
	WriteFlag(m_bLogFileWrites, "MISC18");
	WriteFlag(m_bReportCostPrice, "MISC19");
	WriteFlag(m_bHoltsDiscount, "MISC21");
	WriteFlag(m_bLocationNotes, "MISC25");
	WriteFlag(m_bSpawnRepman, "MISC26");

	{
		int nType = m_nLIPA_LJMU_ExportType;
		WriteFlag((nType >= 1) && (nType <= 2), "MISC27", nType);
	}
	
	WriteFlag(m_bLogNonDuplicates, "MISC28");
	WriteFlag(m_bPMSExtendedRoomRate, "MISC29");
	WriteFlag(m_bLogWebAPICalls, "MISC30");

	if ( TRUE == m_bRemoveLoyaltyTopup)
	{
		WriteNextLine(GetMisc32String());
	}

	WriteFlag(m_bGlobalPromotion, "MISC33");
	WriteFlag(m_bPosTraySetupNoLockAllUsers, "MISC34");
	WriteFlag(m_bReasonTicksNoDownload, "MISC35");
	WriteFlag(m_bReasonTicksVoidAfter, "MISC36");
	WriteFlag(m_bServerHomeLocation, "MISC37");
	WriteFlag(m_bServerImportExport, "MISC38");
	WriteFlag(m_bRefundFix, "MISC39");
	WriteFlag(m_bIgnoreUnpaidTransactions, "MISC40");
	WriteFlag(m_bRefund2Fix, "MISC41");
	WriteFlag(m_bGenRecExcludeLoyalty, "MISC42");
	WriteFlag(m_bUseDatabaseVATBand, "MISC43");
	WriteFlag(m_bAllDiscountsVATable, "MISC44");
	WriteFlag(m_b15DigitKludgeDeptGroup, "MISC45");
	WriteFlag(m_bGratuityFix, "MISC46");
	WriteFlag(m_bGratuityFix2, "MISC47");
	WriteFlag(m_bLinkPluFix, "MISC48");
	WriteFlag(m_bDuplicateCheckIgnoreTime, "MISC49");

	SetFixDepositRecycle(m_bFixDepositRecycle, m_nFixDepositRedeemPayNo, m_nFixDepositRecylePayNo, m_nFixDepositRecyclePluNo);
	if (GetFixDepositRecycleFlag() == TRUE)
	{
		CCSV csv;
		csv.Add("MISC50");
		csv.Add(m_nFixDepositRedeemPayNo);
		csv.Add(m_nFixDepositRecylePayNo);
		csv.Add(m_nFixDepositRecyclePluNo);
		WriteNextLine(csv.GetLine());
	}

	WriteFlag(m_bCOFAllDiscount, "MISC51");

	WriteFlag(m_bEcrTextOnLabels, "LABEL1");
	WriteFlag(TRUE, "LABEL2", m_nSPosBarcodeLength);
	WriteFlag(m_bWeekNoGraph, "GRAPH1");

#if defined (STOCKMAN_SYSTEM) || defined (POSTRAY_UTILITY)

	WriteFlag(m_bImportBarcode, "PLU11");

	if (TRUE == m_bBritanniaHotelsExport)
	{
		CCSV csv;
		csv.Add("MISC2");
		csv.Add(m_nMaxBritanniaPriceLevel);
		csv.Add(m_bBritanniaAllItems);
		WriteNextLine(csv.GetLine());
	}

	WriteFlag(m_bRebuildStockSalesHistory, "MISC20");
	WriteFlag(m_bDorbiereImport, "MISC22");
	WriteFlag(m_bSuppRef, "STOCK1");
	WriteFlag(m_bCopyPluToStock, "STOCK2");
	WriteFlag(m_bObsoleteValuationSummary, "STOCK3");
	WriteFlag((m_nStockLevelsPluType != 0), "STOCK4", m_nStockLevelsPluType);
	WriteFlag((m_nStockLevelsStockType != 0), "STOCK5", m_nStockLevelsStockType);
	WriteFlag(m_bTransitionalOrder, "STOCK6");
	WriteFlag(m_bStockLevelFilter, "STOCK7");
	WriteFlag(m_bExtendedStocktakeReports, "STOCK8");
	WriteFlag(m_bObsoleteManualStockSales, "STOCK9");
	WriteFlag(m_bWarehouseToggle, "STOCK10");
	WriteFlag(m_bExportLiveApparentStock, "STOCK11");
	WriteFlag(m_bPeriodDeliveryCostProfit, "STOCK12");
	WriteFlag(m_bStockReportUseRecipeCostLink, "STOCK13");
	WriteFlag(m_bDeliveryLines, "AUDIT1");
	WriteFlag(m_bDeliveryDetail, "AUDIT2");
	WriteFlag(m_bPurchaseOrderLines, "ORDER1");

#else

	WriteFlag(m_bApparentStockDate, "MISC23");

#endif
}

/**********************************************************************/

void CDealerFlags::AddNoLoyaltyTopupPLU(__int64 nPluNo)
{
	if (m_arrayNoLoyaltyTopupPLUs.GetSize() >= MAX_NOTOPUP_PLU)
	{
		return;
	}

	if ((nPluNo < Plu::PluNo.Min) || (nPluNo > Plu::PluNo.Max))
	{
		return;
	}

	for (int nIndex = 0; nIndex < m_arrayNoLoyaltyTopupPLUs.GetSize(); nIndex++)
	{
		if (m_arrayNoLoyaltyTopupPLUs.GetAt(nIndex) == nPluNo)
		{
			return;
		}
	}

	m_arrayNoLoyaltyTopupPLUs.Add(nPluNo);
}

/**********************************************************************/

void CDealerFlags::SetSPosBarcodeLength( int n )
{
	switch ( n )
	{
	case 0:
	case 1:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		m_nSPosBarcodeLength = n;
		break;
	}
}

/**********************************************************************/

void CDealerFlags::AddToLocComboString( CCSV& csvOut, CString& strTest )
{
#if defined (STOCKMAN_SYSTEM) || defined (POSTRAY_UTILITY)
	if ( strTest != "DB" && strTest != "LOC" && strTest != "RP" && 
		strTest != "SP" && strTest != "SITE" && strTest != "SYS" && 
		strTest != "LS" )
			return;
#else
	if ( strTest != "DB" && strTest != "LOC" && strTest != "RP" &&
		strTest != "SYS" && strTest != "LS" )
			return;
#endif

	for (int n = 0; n < csvOut.GetSize(); n++)
	{
		if (strTest == csvOut.GetString(n))
		{
			return;
		}
	}

	csvOut.Add( strTest );
}

/**********************************************************************/

void CDealerFlags::ProcessLocationSelectorOrder()
{
	m_nPriorityDb = 0;
	m_nPriorityRp = 0;
	m_nPriorityLs = 0;
	m_nPriorityLoc = 0;
	m_nPrioritySite = 0;
	m_nPrioritySp = 0;
	m_nPrioritySys = 0;

	int nPriority = 1;
	
	CCSV csvIn( m_strLocComboOrder );

	for ( int nPos = csvIn.GetSize() - 1; nPos >= 0; nPos-- )
	{
		CString strField = csvIn.GetString ( nPos );
		strField.MakeUpper();
		::TrimSpaces ( strField, FALSE );

		if ((strField == "DB") && (m_nPriorityDb == 0))
		{
			m_nPriorityDb = nPriority++;
		}
		else if ((strField == "LOC") && (m_nPriorityLoc == 0))
		{
			m_nPriorityLoc = nPriority++;
		}
		else if ((strField == "RP") && (m_nPriorityRp == 0))
		{
			m_nPriorityRp = nPriority++;
		}
		else if ((strField == "LS") && (m_nPriorityLs == 0))
		{
			m_nPriorityLs = nPriority++;
		}
		else if ((strField == "SP") && (m_nPrioritySp == 0))
		{
			m_nPrioritySp = nPriority++;
		}
		else if ((strField == "SITE") && (m_nPrioritySite == 0))
		{
			m_nPrioritySite = nPriority++;
		}
		else if ((strField == "SYS") && (m_nPrioritySys == 0))
		{
			m_nPrioritySys = nPriority++;
		}
	}

	m_nPriorityDb *= 1000;
	m_nPriorityRp *= 1000;
	m_nPriorityLs *= 1000;
	m_nPriorityLoc *= 1000;
	m_nPrioritySite *= 1000;
	m_nPrioritySp *= 1000;
	m_nPrioritySys *= 1000;
}

/**********************************************************************/

void CDealerFlags::GetLocationSelectorOrder ( CWordArray& arrayNodeTypes )
{
	CReportConsolidationArray<CSortedStringItem> PriorityList;

	int nSize = arrayNodeTypes.GetSize();
	for ( int nIndex = 0; nIndex < nSize; nIndex++ )
	{
		int nPriority = ( nSize - nIndex );
		switch ( arrayNodeTypes.GetAt ( nIndex ) )
		{
		case NODE_PLACEHOLDER:	
			nPriority = 99999;
			break;

		case NODE_DATABASE:	
			nPriority += m_nPriorityDb;
			break;

		case NODE_LOCATION:	
			nPriority += m_nPriorityLoc;
			break;

		case NODE_STOCKPOINT:
			nPriority += m_nPrioritySp;
			break;

		case NODE_SITE:	
			nPriority += m_nPrioritySite;
			break;

		case NODE_REPORTPOINT:	
			nPriority += m_nPriorityRp;
			break;

		case NODE_LOCATIONSET:	
			nPriority += m_nPriorityLs;	
			break;

		case NODE_DATABASE_ALL:
		case NODE_DATABASE_GLOBAL:
		case NODE_DATABASE_CONSOLIDATED:
		case NODE_SYSTEM:			
			nPriority += m_nPrioritySys;	
			break;
		}
		CString strLabel;
		strLabel.Format ( "%5.5d%3.3d", nPriority, nIndex );

		CSortedStringItem item;
		item.m_strItem = strLabel;
		PriorityList.Consolidate ( item );
	}

	CWordArray arraySortedNodeTypes;
	for ( int nIndex = PriorityList.GetSize() - 1; nIndex >= 0; nIndex-- )
	{
		CSortedStringItem item;
		PriorityList.GetAt( nIndex, item );
		CString strLabel = item.m_strItem;
		
		if (strLabel.GetLength() != 8)
		{
			continue;
		}

		int nArrayIdx = atoi ( strLabel.Right(3) );
		if (nArrayIdx < 0 || nArrayIdx >= arrayNodeTypes.GetSize())
		{
			continue;
		}

		arraySortedNodeTypes.Add ( arrayNodeTypes.GetAt ( nArrayIdx ) );
	}

	if (arraySortedNodeTypes.GetSize() == arrayNodeTypes.GetSize())
	{
		arrayNodeTypes.RemoveAll();
		for (int nIndex = 0; nIndex < arraySortedNodeTypes.GetSize(); nIndex++)
		{
			arrayNodeTypes.Add(arraySortedNodeTypes.GetAt(nIndex));
		}
	}
}

/**********************************************************************/

void CDealerFlags::SetSortCodeRenameType(int n)
{
	if (n >= 0 && n <= 2)
	{
		m_nSortCodeRenameType = n;
	}
}

/**********************************************************************/

const char* CDealerFlags::GetSortCodeLabel()
{
	switch( m_nSortCodeRenameType )
	{
	case 1:	
		m_strSortCodeLabel = "SKU";	
		break;

	case 2:		
		m_strSortCodeLabel = "Stock code";
		break;

	default:	
		m_strSortCodeLabel = "Sort code";
		break;
	}

	return m_strSortCodeLabel;
}

/**********************************************************************/

void CDealerFlags::SetFixDepositRecycle(bool bFlag, int nRedeemPayNo, int nRecyclePayNo, __int64 nPluNo)
{
	bool bEnable = FALSE;

	if (TRUE == bFlag)
	{
		if ((nRedeemPayNo >= 1) && (nRedeemPayNo <= Payment::PaymentNo.Max))
		{
			if ((nRecyclePayNo >= 1) && (nRecyclePayNo <= Payment::PaymentNo.Max))
			{
				if (nRedeemPayNo != nRecyclePayNo)
				{
					if ((nPluNo >= 1) && (nPluNo <= Plu::PluNo.Max))
					{
						bEnable = TRUE;
					}
				}
			}
		}
	}

	if (TRUE == bEnable)
	{
		m_bFixDepositRecycle = TRUE;
		m_nFixDepositRedeemPayNo = nRedeemPayNo;
		m_nFixDepositRecylePayNo = nRecyclePayNo;
		m_nFixDepositRecyclePluNo = nPluNo;
	}
	else
	{
		m_bFixDepositRecycle = FALSE;
		m_nFixDepositRedeemPayNo = 0;
		m_nFixDepositRecylePayNo = 0;
		m_nFixDepositRecyclePluNo = 0;
	}
}

/**********************************************************************/
#if defined (STOCKMAN_SYSTEM) || defined (POSTRAY_UTILITY)
/**********************************************************************/

void CDealerFlags::SetStockLevelsPluType(int n)
{
	if (n >= 0 && n <= 3)
	{
		m_nStockLevelsPluType = n;
	}
}

/**********************************************************************/

void CDealerFlags::SetStockLevelsStockType(int n)
{
	if (n >= 0 && n <= 3)
	{
		m_nStockLevelsStockType = n;
	}
}

/**********************************************************************/

void CDealerFlags::SetCurrentStockLevelsAccessType(int n)
{
	if (n >= 0 && n <= 2)
	{
		m_nCurrentStockLevelsAccessType = n;
	}
}

/**********************************************************************/

const char* CDealerFlags::GetWarehouseType(bool bFirstLetterCaps)
{
	bool bWarehouse = (Sysset.IsBarmanSystem());

	if (TRUE == m_bWarehouseToggle)
	{
		bWarehouse = !bWarehouse;
	}

	if (TRUE == bWarehouse)
	{
		if (TRUE == bFirstLetterCaps)
		{
			m_strWarehouseType = "Warehouse";
		}
		else
		{
			m_strWarehouseType = "warehouse";
		}
	}
	else
	{
		if (TRUE == bFirstLetterCaps)
		{
			m_strWarehouseType = "Cellar";
		}
		else
		{
			m_strWarehouseType = "cellar";
		}
	}

	return m_strWarehouseType;
}

/**********************************************************************/

int CDealerFlags::GetMaxBritanniaPriceLevel()
{
	if ((m_nMaxBritanniaPriceLevel >= 2) && (m_nMaxBritanniaPriceLevel <= 10))
	{
		return m_nMaxBritanniaPriceLevel;
	}
	else
	{
		return 2;
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
