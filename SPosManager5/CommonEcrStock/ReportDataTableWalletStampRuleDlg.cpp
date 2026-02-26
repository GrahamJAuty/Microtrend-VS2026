/**********************************************************************/
#include "..\CommonEcrStockTray\SysInfo.h"
/**********************************************************************/
#include "RepSpawn.h"
/**********************************************************************/
#include "ReportDataTableWalletStampRuleDlg.h"
/**********************************************************************/

CReportDataTableWalletStampRuleDlg::CReportDataTableWalletStampRuleDlg(CWnd* pParent /*=NULL*/)
	: CReportDataTableDlg( pParent)
{
	m_strTitle.Format("Select %s Report Type",
		( SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4 ) ? "Purchase Control" : "Stamp Offer" );
		
	m_arrayReports.Add( "Programming" );
	m_arrayReports.Add( "Epos Version Check" );
}

/**********************************************************************/

void CReportDataTableWalletStampRuleDlg::HandleReport1()
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V4:
		ReportFile.SetStyle1("Purchase Controls");
		ReportFile.AddColumn("ID", TA_LEFT, 120);
		break;

	case LOYALTY_TYPE_LOY_V4:
	default:
		ReportFile.SetStyle1("Stamp Offers");
		ReportFile.AddColumn("ID", TA_LEFT, 120);
		ReportFile.AddColumn("Edition", TA_LEFT, 170);
		break;
	}

	ReportFile.AddColumn("Name", TA_LEFT, 400);
	ReportFile.AddColumn("Enable", TA_LEFT, 150);
	ReportFile.AddColumn("Offer Group", TA_LEFT, 400);
	ReportFile.AddColumn("Threshold", TA_LEFT, 400);
	ReportFile.AddColumn("Expiry", TA_LEFT, 400);
	
	for (int nRuleIdx = 0; nRuleIdx < DataManager.WalletStampRule.GetSize(); nRuleIdx++)
	{
		CWalletStampRuleCSVRecord Rule;
		DataManager.WalletStampRule.GetAt(nRuleIdx, Rule);

		CCSV csvOut('\t');
		csvOut.Add(Rule.GetRuleID());
		
		switch (SysInfo.GetLoyaltyOrSmartPayType())
		{
		case LOYALTY_TYPE_SMP_V4:
			break;

		case LOYALTY_TYPE_LOY_V4:
		default:
			csvOut.Add(Rule.GetEdition());
			break;
		}
		
		csvOut.Add(Rule.GetDisplayName());

		if (Rule.GetEnableFlag())
		{
			csvOut.Add("Yes");
		}
		else
		{
			csvOut.Add("No");
		}

		{
			int nOfferGroupNo = Rule.GetOfferGroupNo();

			if (0 == nOfferGroupNo)
			{
				csvOut.Add("--");
			}
			else
			{
				int nOfferGrpIdx = 0;
				if (DataManager.OfferGroup.FindTableByNumber(nOfferGroupNo, nOfferGrpIdx) == TRUE)
				{
					COfferGroupsCSVRecord OfferGrp(NODE_OFFER_GROUP);
					DataManager.OfferGroup.GetAt(nOfferGrpIdx, OfferGrp);
					csvOut.Add(OfferGrp.GetDisplayName());
				}
				else
				{
					CString strGroup = "";
					strGroup.Format("Offer %d", nOfferGroupNo);
					csvOut.Add(strGroup);
				}
			}
		}

		csvOut.Add(Rule.GetThreshold());
		csvOut.Add(Rule.GetExpiry());

		ReportFile.WriteLine(csvOut.GetLine());
	}
	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp(SysFiles::WalletStampRulePrm);
	ReportManager.DisplayReport(Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this);
}

/**********************************************************************/

void CReportDataTableWalletStampRuleDlg::HandleReport2()
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	{
		CString strTitle = "";
		strTitle.Format("Epos Version Check for %s",
			(SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4) ? "Purchase Controls" : "Stamp Offers");

		ReportFile.SetStyle1(strTitle);
	}

	ReportFile.AddColumn ( "ID", TA_LEFT, 120 );
	ReportFile.AddColumn ( "Name", TA_LEFT, 400 );
	ReportFile.AddColumn ( "Minimum Version", TA_LEFT, 800 );
	ReportFile.AddColumn ( "Reason", TA_LEFT, 800 );

	for (int nRuleIdx = 0; nRuleIdx < DataManager.WalletStampRule.GetSize(); nRuleIdx++)
	{
		CWalletStampRuleCSVRecord Rule;
		DataManager.WalletStampRule.GetAt(nRuleIdx, Rule);

		CMinimumSPOSVersion Version;
		Rule.CheckMinimumSPOSVersion( Version );

		CCSV csvOut ( '\t' );
		csvOut.Add ( Rule.GetRuleID() );
		csvOut.Add ( Rule.GetDisplayName() );
		csvOut.Add ( Version.GetMinVerName() );
		csvOut.Add ( Version.m_strReason );
	
		ReportFile.WriteLine ( csvOut.GetLine() );
	}
	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::WalletStampRuleVersionPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}

/**********************************************************************/


