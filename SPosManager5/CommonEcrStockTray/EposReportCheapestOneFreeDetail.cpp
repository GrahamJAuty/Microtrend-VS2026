/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "MixMatchCSVArray.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportCheapestOneFreeDetail.h"
/**********************************************************************/

CEposReportConsolCheapestOneFreeDetail::CEposReportConsolCheapestOneFreeDetail()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCheapestOneFreeDetail::Reset()
{
	m_nDbIdx = 0;
	m_nDbTotalFlag = 0;
	m_nMixMatchNo = 0;
	m_nSubtotalFlag = 0;
	m_nPluNo = 0;
	m_nQty = 0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CEposReportConsolCheapestOneFreeDetail::Add ( CEposReportConsolCheapestOneFreeDetail& source )
{
	m_nQty += source.m_nQty;
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CEposReportConsolCheapestOneFreeDetail::Compare( CEposReportConsolCheapestOneFreeDetail& source, int nHint )
{
	if ( m_nDbIdx != source.m_nDbIdx )
		return ( m_nDbIdx > source.m_nDbIdx ? 1 : -1 );

	if ( m_nDbTotalFlag != source.m_nDbTotalFlag )
		return ( m_nDbTotalFlag > source.m_nDbTotalFlag ? 1 : -1 );

	if ( m_nMixMatchNo != source.m_nMixMatchNo )
		return ( m_nMixMatchNo > source.m_nMixMatchNo ? 1 : -1 );

	if ( m_nSubtotalFlag != source.m_nSubtotalFlag )
		return ( m_nSubtotalFlag > source.m_nSubtotalFlag ? 1 : -1 );

	if ( m_strDate != source.m_strDate )
		return ( m_strDate > source.m_strDate ? 1 : -1 );

	if ( m_strTime != source.m_strTime )
		return ( m_strTime > source.m_strTime ? 1 : -1 );

	if ( m_nLineNo != source.m_nLineNo )
		return ( m_nLineNo > source.m_nLineNo ? 1 : -1 );

	if ( m_nPluNo != source.m_nPluNo )
		return ( m_nPluNo > source.m_nPluNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCheapestOneFreeDetail::CEposReportCheapestOneFreeDetail( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_CHEAPESTONEFREE_DETAIL;
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
}

/**********************************************************************/

void CEposReportCheapestOneFreeDetail::Consolidate()
{
	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_SALES, m_SelectArray, FALSE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );

	__int64 nLineNo = 0;
	
	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		if ( m_PMSModes.GetPMSLocationStatus( infoFile ) != 2 )
			continue;

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CString strThisSaleDate = "";
		CString strThisSaleTime = "";
		m_PMSModes.GetInitialSaleDateTime( FALSE, infoFile, m_bSaleTime, strThisSaleDate, strThisSaleTime );

		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );

		CPluSalesLine csv( infoFile.IsLegacySalesFolder() );

		int nLinesToRead;
		CString strBuffer;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

				csv.ParseLine( strBuffer );

				//FILTER FOR WASTAGE
				if (csv.GetPluSaleWastageFlag() != IsWastageReport())
				{
					continue;
				}

				if (csv.GetPluSaleVoidFlag() == TRUE)
				{
					continue;
				}

				//FILTER FOR POST TO ROOM
				if ( SysInfo.IsPMSSystem() == TRUE )
					if ( m_PMSModes.GetEposFlag() == FALSE )
						if ( DataManager.Payment.IsPMSPaymentType( csv.GetPluPaymentNo() ) )
							continue;

				//NORMAL CASHRSP SALES TAKES DATE FROM FILE AND TIME FROM CSV LINE
				{
					COleDateTime oleDummy;
					if ( CheckPluSaleDateTime( FALSE, infoFile, csv, oleDummy, strThisSaleDate, strThisSaleTime ) == FALSE )
						continue;
				}
				
				//FILTER FOR DEPARTMENT
				int nDeptNo = csv.GetDeptNo();
				if ( m_ReportFilters.CheckDepartment( nDeptNo ) == FALSE )
					continue;

				switch( csv.GetLineType() )
				{
				case CSVPLUDATA_PLU_SALE:
				case CSVPLUDATA_PLU_CHEAPESTFREE:

					if ( csv.GetMixMatchType() == MIXMATCH_ACTION_FREE_ITEM )
					{	
						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nEposPluNo = csv.GetPluNoNew();
						::ProcessPluNo( infoPluNo, FALSE, TRUE );
						
						if ( TRUE == infoPluNo.m_bValid )
						{
							CEposReportConsolCheapestOneFreeDetail infoPromo;
							infoPromo.m_nDbIdx = m_ReportFilters.GetDbIdx();
							infoPromo.m_nDbTotalFlag = 0;
							infoPromo.m_nMixMatchNo = csv.GetMixMatchNo();
							infoPromo.m_nSubtotalFlag = 0;
							infoPromo.m_strDate = strThisSaleDate;
							infoPromo.m_strTime = strThisSaleTime;
							infoPromo.m_nLineNo = nLineNo++;
							infoPromo.m_nPluNo = infoPluNo.m_nBasePluNo;
							infoPromo.m_nQty = 1;
							infoPromo.m_dVal = ( csv.GetValue() * -1 );
							m_ConsolArray.Consolidate( infoPromo );

							{
								CEposReportConsolCheapestOneFreeDetail infoTotal = infoPromo;
								infoTotal.m_nSubtotalFlag = 1;
								infoTotal.m_nPluNo = 0;
								infoTotal.m_strDate = "";
								infoTotal.m_strTime = "";
								infoTotal.m_nLineNo = 0;
								m_ConsolArray.Consolidate( infoTotal );
							}

							{
								CEposReportConsolCheapestOneFreeDetail infoTotal = infoPromo;
								infoTotal.m_nDbTotalFlag = 1;
								infoTotal.m_nMixMatchNo = 0;
								infoTotal.m_strDate = "";
								infoTotal.m_strTime = "";
								infoTotal.m_nPluNo = infoPromo.m_nMixMatchNo;
								infoTotal.m_nLineNo = 0;
								m_ConsolArray.Consolidate( infoTotal );
							}

							{
								CEposReportConsolCheapestOneFreeDetail infoTotal = infoPromo;
								infoTotal.m_nDbTotalFlag = 1;
								infoTotal.m_nMixMatchNo = 0;
								infoTotal.m_nSubtotalFlag = 1;
								infoTotal.m_nPluNo = 0;
								infoTotal.m_strDate = "";
								infoTotal.m_strTime = "";
								infoTotal.m_nLineNo = 0;
								m_ConsolArray.Consolidate( infoTotal );
							}
						}
					}
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportCheapestOneFreeDetail::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Plu No", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Quantity", TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( "Discount", TA_RIGHT, 250 );

	StatusProgress.Lock( TRUE, "Creating report" );

	int nCurrentDbIdx = -1;
	bool bWantHeader = TRUE;
	
	int nSize = m_ConsolArray.GetSize();
	for ( int i = 0 ; i < nSize ; i++ )
	{
		StatusProgress.SetPos( i, nSize );

		CEposReportConsolCheapestOneFreeDetail infoConsol;
		m_ConsolArray.GetAt( i, infoConsol );

		if ( infoConsol.m_nDbIdx != nCurrentDbIdx )
		{
			CDataManagerInfo infoDb;
			DataManager.OpenDatabaseReadOnly( infoConsol.m_nDbIdx, infoDb, FALSE );
			nCurrentDbIdx = infoConsol.m_nDbIdx;
		}

		if ( TRUE == bWantHeader )
		{
			CString strHeader = "";

			if ( 1 == infoConsol.m_nDbTotalFlag )
			{
				strHeader.Format( "<..>%s", dbDatabase.GetName( infoConsol.m_nDbIdx ) );
			}
			else
			{
				int nMixMatchIdx;
				CString strMixMatchName = "";
				if ( DataManager.MixMatch.FindTableByNumber( infoConsol.m_nMixMatchNo, nMixMatchIdx ) == TRUE )
				{
					CMixMatchCSVRecord MixMatch;
					DataManager.MixMatch.GetAt( nMixMatchIdx, MixMatch );
					strMixMatchName = MixMatch.GetDisplayName();
				}

				if (strMixMatchName == "")
				{
					strMixMatchName.Format("Mix Match %d", infoConsol.m_nMixMatchNo);
				}

				strHeader.Format( "<..>M%3.3d: %s", infoConsol.m_nMixMatchNo, (const char*) strMixMatchName );
			}

			m_ReportFile.RequestBlankLines(1);
			m_ReportFile.WriteReportMiscLine( strHeader );
			m_ReportFile.WriteReportMiscLine( "<LI>" );
			bWantHeader = FALSE;
		}

		if ( 1 == infoConsol.m_nSubtotalFlag )
		{
			m_ReportFile.WriteReportMiscLine( "\t\t\t\t<LI>\t<LI>" );

			CCSV csvOut( '\t' );
			csvOut.Add( "" );
			csvOut.Add( "" );
			csvOut.Add( "" );
			csvOut.Add( "Total" );
			csvOut.Add( infoConsol.m_nQty );
			csvOut.Add( infoConsol.m_dVal, SysInfo.GetDPValue() );
			m_ReportFile.WriteReportMiscLine( csvOut.GetLine() );
			m_ReportFile.WriteReportMiscLine( "" );
			bWantHeader = TRUE;
		}
		else if ( 1 == infoConsol.m_nDbTotalFlag )
		{
			int nMixMatchNo = int( infoConsol.m_nPluNo );

			CString strMixMatchNo;
			strMixMatchNo.Format( "M%3.3d:", nMixMatchNo );

			int nMixMatchIdx;
			CString strMixMatchName = "";
			if ( DataManager.MixMatch.FindTableByNumber( nMixMatchNo, nMixMatchIdx ) == TRUE )
			{
				CMixMatchCSVRecord MixMatch;
				DataManager.MixMatch.GetAt( nMixMatchIdx, MixMatch );
				strMixMatchName = MixMatch.GetDisplayName();
			}

			if (strMixMatchName == "")
			{
				strMixMatchName.Format("Mix Match %d", nMixMatchNo);
			}

			CCSV csvOut( '\t' );
			csvOut.Add( "" );
			csvOut.Add( "" );
			csvOut.Add( strMixMatchNo );
			csvOut.Add( strMixMatchName );
			csvOut.Add( infoConsol.m_nQty );
			csvOut.Add( infoConsol.m_dVal, SysInfo.GetDPValue() );
			m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
		}
		else
		{
			CString strDate = "";
			if ( infoConsol.m_strDate.GetLength() == 8 )
			{
				strDate.Format( "%s/%s/%s",
					(const char*) infoConsol.m_strDate.Right(2),
					(const char*) infoConsol.m_strDate.Mid(4,2),
					(const char*) infoConsol.m_strDate.Left(4) );
			}

			CString strTime = "";
			if ( infoConsol.m_strTime.GetLength() == 6 )
			{
				strTime.Format( "%s:%s:%s",
					(const char*) infoConsol.m_strTime.Left(2),
					(const char*) infoConsol.m_strTime.Mid(2,2),
					(const char*) infoConsol.m_strTime.Right(2) );
			}
	
			CString strPluNo = ReportHelpers.GetDisplayPluNo( infoConsol.m_nPluNo, TRUE );
			CString strDescription = "Unknown";

			{
				int nPluIdx;
				if ( DataManager.Plu.FindPluByNumber( infoConsol.m_nPluNo, nPluIdx ) == TRUE )
				{
					CPluCSVRecord PluRecord;
					DataManager.Plu.GetAt( nPluIdx, PluRecord );
					strDescription = PluRecord.GetEposText();
				}
			}

			CCSV csvOut( '\t' );
			csvOut.Add( strDate );
			csvOut.Add( strTime );
			csvOut.Add( strPluNo );
			csvOut.Add( strDescription );
			csvOut.Add( infoConsol.m_nQty );
			csvOut.Add( infoConsol.m_dVal, SysInfo.GetDPValue() );
			m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
		}
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

