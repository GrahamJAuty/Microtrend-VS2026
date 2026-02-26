/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "ReportDataTableOfferGroupDlg.h"
/**********************************************************************/

CReportDataTableOfferGroupDlg::CReportDataTableOfferGroupDlg(CWnd* pParent /*=NULL*/)
	: CReportDataTableDlg( pParent)
{
	m_strTitle = "Select Offer Group Report Type";
	m_arrayReports.Add( "Programming" );
	m_arrayReports.Add( "Epos Version Check" );
}

/**********************************************************************/
void CReportDataTableOfferGroupDlg::HandleReport1(){ CreateReport( FALSE ); }
void CReportDataTableOfferGroupDlg::HandleReport2(){ CreateReport( TRUE ); }
/**********************************************************************/

void CReportDataTableOfferGroupDlg::CreateReport( bool bSPOSVersion )
{
	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;

	ReportFile.SetStyle1 ( "Mix Match Offer Groups" );
		
	ReportFile.AddColumn ( "No", TA_LEFT, 120 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Type", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Entry", TA_LEFT, 500 );

	if ( TRUE == bSPOSVersion )
	{
		ReportFile.AddColumn ( "Minimum Version", TA_LEFT, 800 );
		ReportFile.AddColumn ( "Reason", TA_LEFT, 800 );
	}
	
	for ( int nOfferIdx = 0; nOfferIdx < DataManager.OfferGroup.GetSize(); nOfferIdx++ )
	{
		COfferGroupsCSVRecord Offer( NODE_OFFER_GROUP );
		DataManager.OfferGroup.GetAt ( nOfferIdx, Offer );
		int nGroupNo = Offer.GetGroupNo();
			
		CCSV csvOut ( '\t' );
		csvOut.Add( nGroupNo );
		csvOut.Add( Offer.GetDisplayName() );
		ReportFile.WriteLine ( csvOut.GetLine() );

		CArray<int,int> arrayEntryTypes;
		arrayEntryTypes.Add( OFFERENTRY_PLU );
		arrayEntryTypes.Add( OFFERENTRY_DEPT );
		arrayEntryTypes.Add( OFFERENTRY_DEPT_SAMEPLU );
		arrayEntryTypes.Add( OFFERENTRY_GROUP );
		arrayEntryTypes.Add( OFFERENTRY_PLU_EXCLUDE );
		
		bool bGotEntry = FALSE;

		for ( int nTypeIdx = 0; nTypeIdx < arrayEntryTypes.GetSize(); nTypeIdx++ )
		{	
			int nEntryType = arrayEntryTypes.GetAt( nTypeIdx );

			int nIdx1, nIdx2;
			DataManager.OfferGroupEntry.GetGroupSubRange( nGroupNo, nEntryType, nIdx1, nIdx2 );

			for ( int nIdx = nIdx1; nIdx <= nIdx2; nIdx++ )
			{
				COfferGroupEntriesCSVRecord entry;
				DataManager.OfferGroupEntry.GetAt( nIdx, entry );

				__int64 nSharpPluNo = 0;
				bool bModifiable = FALSE;
				CMinimumSPOSVersion Version;
				entry.CheckMinimumSPOSVersion( Version, nSharpPluNo, bModifiable );
				
				if ( nIdx == nIdx1 )
				{
					if ( bGotEntry == TRUE )
						ReportFile.WriteLine( "" );
				}

				switch( nEntryType )
				{
				case OFFERENTRY_PLU:
				case OFFERENTRY_PLU_EXCLUDE:
					{
						int nPluIdx;
						CString strPluNo = BarcodePluNoTable.ConvertPluInt( entry.GetEntryPluNo() );
						
						if ( DataManager.Plu.FindPluByNumber( entry.GetEntryPluNo(), nPluIdx ) == TRUE )
						{
							CPluCSVRecord PluRecord;
							DataManager.Plu.GetAt( nPluIdx, PluRecord );

							strPluNo += " ";
							strPluNo += PluRecord.GetReportText();
						}
						else
							strPluNo += " Unknown";

						int nFlags = entry.GetModifierFlags();
						
						if ( ( nFlags & 1 ) == 0 )
							strPluNo += " ****";

						csvOut.RemoveAll();
						csvOut.Add ( "" );
						csvOut.Add ( "" );

						if ( OFFERENTRY_PLU_EXCLUDE == nEntryType )
							csvOut.Add( "PLU (Exclude)" );
						else
							csvOut.Add( "PLU" );

						csvOut.Add( strPluNo );

						if ( TRUE == bSPOSVersion )
						{
							csvOut.Add ( Version.GetMinVerName() );
							csvOut.Add ( Version.m_strReason );
						}

						ReportFile.WriteLine ( csvOut.GetLine() );

						bGotEntry = TRUE;

						int nMask = 2;
						for ( int n = 1; n <= 9; n++ )
						{
							if ( ( nFlags & nMask ) != 0 )
							{
								
								CString strModifier;
								strModifier.Format( "    M%2.2d %s",
									n,
									DataManager.Modifier.GetDisplayName(n) );

								csvOut.RemoveAll();
								csvOut.Add( "" );
								csvOut.Add( "" );
								csvOut.Add( strModifier );
								ReportFile.WriteLine ( csvOut.GetLine() );
							}

							nMask *= 2;
						}

					}
					break;

				case OFFERENTRY_DEPT:
				case OFFERENTRY_DEPT_SAMEPLU:
					{
						int nDeptNo = entry.GetEntryDeptNo();

						CString strInfo;
						strInfo.Format( "D%2.2d %s",
							nDeptNo,
							DataManager.Department.GetReportTextByDeptNo( nDeptNo ) );
						
						csvOut.RemoveAll();
						csvOut.Add ( "" );
						csvOut.Add ( "" );

						if ( OFFERENTRY_DEPT_SAMEPLU == nEntryType )
							csvOut.Add( "Department (Same PLU)" );
						else
							csvOut.Add( "Department" );

						csvOut.Add( strInfo );

						if ( TRUE == bSPOSVersion )
						{
							csvOut.Add ( Version.GetMinVerName() );
							csvOut.Add ( Version.m_strReason );
						}

						ReportFile.WriteLine ( csvOut.GetLine() );
						bGotEntry = TRUE;
					}
					break;

				case OFFERENTRY_GROUP:
					{
						int nGroupNo = entry.GetEntryGroupNo();

						CString strInfo;
						strInfo.Format( "G%2.2d %s",
							nGroupNo,
							DataManager.EposGroup.GetGroupReportText( nGroupNo ) );
						
						csvOut.RemoveAll();
						csvOut.Add( "" );
						csvOut.Add( "" );
						csvOut.Add( "Group" );
						csvOut.Add( strInfo );

						if ( TRUE == bSPOSVersion )
						{
							csvOut.Add ( Version.GetMinVerName() );
							csvOut.Add ( Version.m_strReason );
						}

						ReportFile.WriteLine ( csvOut.GetLine() );
						bGotEntry = TRUE;
					}
					break;
				}
			}
		}

		ReportFile.WriteLine ( "<LI>" );
	}
	ReportFile.Close();

	CString strPrmFile;
	if ( FALSE == bSPOSVersion )
	{
		CFilenameUpdater FnUp ( SysFiles::OfferGroupPrm );
		strPrmFile = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp ( SysFiles::OfferGroupVersionPrm );
		strPrmFile = FnUp.GetFilenameToUse();
	}

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), strPrmFile, "", this );
}

/**********************************************************************/

