/**********************************************************************/
#include "CCSVReportLine.h"
#include "RepSpawn.h"
#include "SageOptions.h"
#include "TimePlanTemplateListManager.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ReportDataTablePromoDlg.h"
/**********************************************************************/

CReportDataTablePromoDlg::CReportDataTablePromoDlg(CWnd* pParent /*=NULL*/)
	: CReportDataTableDlg(pParent)
{
	m_strTitle = "Select Promotion Report Type";
	m_arrayReports.Add("Programming");
	m_arrayReports.Add("Timeplan Summary");
	m_arrayReports.Add("Flex Promo Detail");
	m_arrayReports.Add("Epos Version Check");
}

/**********************************************************************/

void CReportDataTablePromoDlg::HandleReport1()
{
	CreateDetailReport(1);
}

/**********************************************************************/

void CReportDataTablePromoDlg::HandleReport2()
{
	CreateDetailReport(2);
}

/**********************************************************************/

void CReportDataTablePromoDlg::CreateDetailReport(int nSubType)
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	ReportFile.SetStyle1( nSubType != 2 ? "Promotion Settings" : "Promotion Time Plan Summary");

	AddBasicPromoHeaders(ReportFile, nSubType);

	if (nSubType != 2)
	{
		ReportFile.AddColumn("Trip", TA_RIGHT, 200);
		ReportFile.AddColumn("Hotel Dept", TA_RIGHT, 250);
		ReportFile.AddColumn("Gratuity", TA_LEFT, 200);
		ReportFile.AddColumn(EcrmanOptions.GetSalesTaxName(), TA_LEFT, 200);
		ReportFile.AddColumn("Apply to Promo", TA_LEFT, 300);
		ReportFile.AddColumn("Discountable", TA_LEFT, 300);
		ReportFile.AddColumn("Reasons", TA_LEFT, 300);
		ReportFile.AddColumn("S/C Vat Lock", TA_LEFT, 300);
		ReportFile.AddColumn("Force Vatable", TA_LEFT, 300);
	}

	ReportFile.AddColumn("Timeplan Mode", TA_LEFT, 350);
	ReportFile.AddColumn("Timeplan Name", TA_LEFT, 350);
	ReportFile.AddColumn("Timeplan Entry", TA_LEFT, 350);

	if (nSubType != 2)
	{
		ReportFile.AddColumn("Filter Type", TA_LEFT, 250);
		ReportFile.AddColumn("Filter Detail", TA_LEFT, 500);
		ReportFile.AddColumn("Report As Plu", TA_LEFT, 250);
		ReportFile.AddColumn("Report Dept", TA_RIGHT, 250);
		ReportFile.AddColumn("Report Qty", TA_LEFT, 250);

		if (SysInfo.IsSageExportSystem() == TRUE)
		{
			ReportFile.AddColumn("Sage Nominal Code", TA_LEFT, (2 == nSubType) ? 30 : 400);
		}
	}

	int nLineNo = 0;
	CReportConsolidationArray<CSortedTimeplanEntry> arrayTimeplanEntries;
	CTimePlanTemplateListManager::GetTimeplanReportDetails(1, nLineNo, arrayTimeplanEntries);

	for (int nPromoIdx = 0; nPromoIdx < DataManager.Promotion.GetSize(); nPromoIdx++)
	{
		CPromotionsCSVRecord Promo;
		DataManager.Promotion.GetAt(nPromoIdx, Promo);

		if (EcrmanOptions.GetFeaturesTimePlanTemplateFlag() == TRUE)
		{
			CTimePlanTemplateListManager::GetTimeplanReportDetails(nLineNo, Promo, arrayTimeplanEntries);
		}
		
		int nEntryStartIdx = 0; int nEntryEndIdx = 0;
		CTimePlanTemplateListManager::GetReportDetailsEntryRange(Promo.GetPromoNo(), arrayTimeplanEntries, nEntryStartIdx, nEntryEndIdx );

		CCSVReportLine csvOut;
		AddBasicPromoSettings(csvOut, Promo, nSubType);

		if (nSubType != 2)
		{
			csvOut.AppendLineQty(Promo.GetTrip(), 2);
			csvOut.AppendInt(Promo.GetDeptNoEpos());
			csvOut.AppendYesOrBlank(Promo.GetGratuityFlag());
			csvOut.AppendYesOrBlank(Promo.GetSPOSApplyTaxFlag());
			csvOut.AppendYesOrBlank(Promo.GetMyPromoOnPromoFlag());
			csvOut.AppendYesOrBlank(Promo.GetPromoOnMyPromoFlag());
			csvOut.AppendYesOrBlank(Promo.GetReasonsFlag());
			csvOut.AppendYesOrBlank(Promo.GetServiceChargeVATFixFlag());

			if (DealerFlags.GetAllDiscountsVATableFlag() == TRUE)
			{
				csvOut.AppendString("System");
			}
			else
			{
				csvOut.AppendYesOrBlank(Promo.GetDiscountAlwaysVATableFlag());
			}
		}

		switch (Promo.GetTimeplanMode())
		{
		case 0:
			csvOut.AppendString("Override prompt");
			break;

		case 1:
			csvOut.AppendString("Silent reject");
			break;

		case 2:
			csvOut.AppendString("Warn & reject");
			break;

		default:
			csvOut.AppendString("Unknown");
			break;
		}

		{
			CString strPlanName = "";
			CString strEntryName = "";

			if (nEntryStartIdx <= nEntryEndIdx)
			{
				CTimePlanTemplateListManager::GetTimeplanEntryDetails(arrayTimeplanEntries, nEntryStartIdx, strPlanName, strEntryName);
			}

			csvOut.AppendString(strPlanName);
			csvOut.AppendString(strEntryName);
		}

		if (nSubType != 2)
		{
			switch (Promo.GetFilterType())
			{
			case 0:
				csvOut.AppendString("None");
				csvOut.AppendString("");
				break;

			case 1:
				csvOut.AppendString("Analysis");
				csvOut.AppendString(Promo.GetAnalysisList(FALSE));
				break;

			case 2:
			{
				CString strGroupInfo = "";

				int nGroupNo = Promo.GetFilterOfferGroup();

				if (0 == nGroupNo)
				{
					strGroupInfo = "None";
				}
				else
				{
					int nGroupIdx;
					if (DataManager.OfferGroup.FindTableByNumber(nGroupNo, nGroupIdx) == TRUE)
					{
						COfferGroupsCSVRecord OfferGroup(NODE_OFFER_GROUP);
						DataManager.OfferGroup.GetAt(nGroupIdx, OfferGroup);

						strGroupInfo.Format("G%3.3d %s",
							nGroupNo,
							OfferGroup.GetDisplayName());
					}
					else
					{
						strGroupInfo.Format("G%3.3d Unknown",
							nGroupNo);
					}
				}

				csvOut.AppendString("Offer Group");
				csvOut.AppendString(strGroupInfo);
			}
			break;

			default:
				csvOut.AppendString("Unknown");
				csvOut.AppendString("");
				break;
			}

			switch (Promo.GetReportAsPluType())
			{
			case PROMO_REPORTASPLU_NEVER:
				csvOut.AppendString("Never");
				break;

			case PROMO_REPORTASPLU_ALWAYS:
				csvOut.AppendString("Always");
				break;

			case PROMO_REPORTASPLU_NONVATABLE:
			default:
				csvOut.AppendString("Non Taxable");
				break;
			}

			csvOut.AppendInt(Promo.GetDeptNoReport());
			csvOut.AppendString(Promo.GetReportAsZeroQtyFlag() ? "Zero" : "One");

			if (SysInfo.IsSageExportSystem() == TRUE)
			{
				int nCode = Promo.GetSageNominalCode();
				int nMaxLen = SageOptions.GetNominalCodeLength();

				if (0 == nCode)
				{
					csvOut.AppendString("");
				}
				else
				{
					CString str = "";
					str.Format("%*.*d", nMaxLen, nMaxLen, nCode);
					csvOut.AppendString(str);
				}
			}
		}

		ReportFile.WriteLine(csvOut.GetLine());
		CTimePlanTemplateListManager::ReportExtraTimeplans(ReportFile, arrayTimeplanEntries, nEntryStartIdx, nEntryEndIdx, nSubType != 2 ? 18 : 7);
		ReportFile.WriteLine("<LI>");	
	}
	ReportFile.Close();

	CReportManager ReportManager;
	SysFileInfo SysFileInfo = (2 == nSubType) ? SysFiles::PromoPrm2 : SysFiles::PromoPrm;	
	CFilenameUpdater FnUp(SysFileInfo);
	ReportManager.DisplayReport(Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this);
}

/**********************************************************************/

void CReportDataTablePromoDlg::HandleReport3()
{
	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;

	ReportFile.SetStyle1 ( "Promotions Flex Detail" );
	AddBasicPromoHeaders( ReportFile, 3 );
	ReportFile.AddColumn ( "Threshold", TA_RIGHT, 300 );
	ReportFile.AddColumn ( "Amount", TA_RIGHT, 300 );
		
	for ( int nPromoIdx = 0; nPromoIdx < DataManager.Promotion.GetSize(); nPromoIdx++ )
	{
		CPromotionsCSVRecord Promo;
		DataManager.Promotion.GetAt ( nPromoIdx, Promo );

		CCSVReportLine csvOut;
		AddBasicPromoSettings( csvOut, Promo, 3 );
		
		ReportFile.WriteLine ( csvOut.GetLine() );

		CStringArray arrayDetail;

		switch( Promo.GetFlexType() )
		{
		case 1: //BY VALUE
			{
				bool bError = FALSE;
				double dLastValue = -1;

				for ( int n = 0; (n < 5) && (FALSE == bError); n++ )
				{
					CCSVReportLine csvOut;
					csvOut.AppendBlanks(7);

					double dValue = ( 0 == n ) ? 0.0 : Promo.GetFlexBoundValue(n);
						
					if ( dValue <= dLastValue )
					{
						bError = TRUE;
					}
					else
					{
						csvOut.AppendVal(dValue, 2);
						csvOut.AppendPercent(Promo.GetFlexPercent(n));
						arrayDetail.Add( csvOut.GetLine() );
						dLastValue = dValue;
					}
				}
			}
			break;

		case 2: //BY COVERS
			{
				bool bError = FALSE;
				int nLastValue = -1;

				for ( int n = 0; (n < 5) && (FALSE == bError); n++ )
				{
					CCSVReportLine csvOut;
					csvOut.AppendBlanks(7);

					int nValue = ( 0 == n ) ? 0 : Promo.GetFlexBoundCovers(n);
						
					if ( nValue <= nLastValue )
					{
						bError = TRUE;
					}
					else
					{
						csvOut.AppendInt(nValue);
						csvOut.AppendPercent(Promo.GetFlexPercent(n));
						arrayDetail.Add( csvOut.GetLine() );
						nLastValue = nValue;
					}
				}
			}
			break;
		}

		arrayDetail.Add( "<LI>" );
		for ( int x = 0; x < arrayDetail.GetSize(); x++ )
			ReportFile.WriteLine( arrayDetail.GetAt(x) );
	}
	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::FlexDetailPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}

/**********************************************************************/

void CReportDataTablePromoDlg::AddBasicPromoHeaders(CReportFile& ReportFile, int nSubType)
{
	ReportFile.AddColumn("No", TA_LEFT, 120);
	ReportFile.AddColumn("Description", TA_LEFT, 500);
	ReportFile.AddColumn("Promo Type", TA_LEFT, 250);
	ReportFile.AddColumn("Usage", TA_LEFT, 250);

	if (nSubType != 2)
	{
		ReportFile.AddColumn("Use Gate", TA_LEFT, 250);
	}

	ReportFile.AddColumn("Value Type", TA_LEFT, 250);
	ReportFile.AddColumn("Amount", TA_LEFT, 250);

	if (nSubType != 2)
	{
		ReportFile.AddColumn("UpTo", TA_LEFT, 200);
	}
}

/**********************************************************************/

void CReportDataTablePromoDlg::AddBasicPromoSettings(CCSVReportLine& csvOut, CPromotionsCSVRecord& Promo, int nSubType)
{
	csvOut.AppendInt(Promo.GetPromoNo());
	csvOut.AppendString(Promo.GetDisplayName());
	csvOut.AppendString(Promo.GetPromoTypeName());

	switch (Promo.GetUsage())
	{
	case 1:
		csvOut.AppendString("Item");
		break;

	case 2:
		csvOut.AppendString("Subtotal");
		break;

	case 3:
		csvOut.AppendString("Item or Subtotal");
		break;

	case 6:
		csvOut.AppendString("Deferred Subtotal Only");
		break;

	default:
	{
		CString str = "";
		str.Format("Unknown (%d)", Promo.GetUsage());
		csvOut.AppendString(str);
	}
	break;
	}

	if (nSubType != 2)
	{
		csvOut.AppendInt(Promo.GetUseGate());
	}

	csvOut.AppendString(Promo.GetFlexTypeName());

	if (Promo.GetFlexType() == 1)
	{
		csvOut.AppendString("Flex (Value)");
	}
	else if (Promo.GetFlexType() == 2)
	{
		csvOut.AppendString("Flex (Covers)");
	}
	else if (Promo.GetPromoType() != PROMOTION_TYPE_MARKDOWN)
	{
		CString strPercent = "";
		strPercent.Format("%.2f%%", Promo.GetPercent());
		csvOut.AppendString(strPercent);
	}
	else
	{
		CString strMoney = "";
		strMoney.Format("£%.2f", Promo.GetMarkdown());
		csvOut.AppendString(strMoney);
	}

	if (nSubType != 2)
	{
		CString strUpTo = "--";
		if (Promo.GetPromoType() == 2)
		{
			if (Promo.GetUpToFlag() == TRUE)
			{
				strUpTo = "Yes";
			}
			else
			{
				strUpTo = "No";
			}
		}

		csvOut.AppendString(strUpTo);
	}
}

/**********************************************************************/

void CReportDataTablePromoDlg::HandleReport4()
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	ReportFile.SetStyle1("Epos Version Check for Promotions");

	ReportFile.AddColumn("No", TA_LEFT, 120);
	ReportFile.AddColumn("Description", TA_LEFT, 500);
	ReportFile.AddColumn("Minimum Version", TA_LEFT, 800);
	ReportFile.AddColumn("Reason", TA_LEFT, 800);

	for (int nPromoIdx = 0; nPromoIdx < DataManager.Promotion.GetSize(); nPromoIdx++)
	{
		CPromotionsCSVRecord PromoRecord;
		DataManager.Promotion.GetAt(nPromoIdx, PromoRecord);

		int nLegacyCats = 0;
		CMinimumSPOSVersion Version;
		PromoRecord.CheckMinimumSPOSVersion(Version, nLegacyCats);

		CCSVReportLine csvOut;
		csvOut.AppendInt(PromoRecord.GetPromoNo());
		csvOut.AppendString(PromoRecord.GetDisplayName());
		csvOut.AppendString(Version.GetMinVerName());
		csvOut.AppendString(Version.m_strReason);

		ReportFile.WriteLine(csvOut.GetLine());
	}
	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp(SysFiles::PromoVersionPrm);
	ReportManager.DisplayReport(Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this);
}

/**********************************************************************/
