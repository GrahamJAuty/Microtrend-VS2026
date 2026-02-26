/**********************************************************************/
#include "CCSVReportLine.h"
#include "RepSpawn.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ReportDataTableTimePlanDlg.h"
/**********************************************************************/

CReportDataTableTimePlanDlg::CReportDataTableTimePlanDlg(CWnd* pParent)
	: CReportDataTableDlg(pParent)
{
	m_strTitle = "Select Time Plan Report Type";
	/*****/
	m_arrayReports.Add("Time Plans");
	m_arrayReports.Add("Templates");
	m_arrayReports.Add("Epos Version Check");
}

/**********************************************************************/

void CReportDataTableTimePlanDlg::HandleReport1()
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	ReportFile.SetStyle1("Time Plans");

	ReportFile.AddColumn("No", TA_LEFT, 120);
	ReportFile.AddColumn("Description", TA_LEFT, 500);
	ReportFile.AddColumn("Enable", TA_LEFT, 150);
	ReportFile.AddColumn("Type", TA_LEFT, 220);
	ReportFile.AddColumn("Detail", TA_LEFT, 350);
	ReportFile.AddColumn("Start Date", TA_LEFT, 250);
	ReportFile.AddColumn("End Date", TA_LEFT, 250);
	ReportFile.AddColumn("Start Time", TA_LEFT, 230);
	ReportFile.AddColumn("End Time", TA_LEFT, 230);
	ReportFile.AddColumn("End Period", TA_LEFT, 250);
	ReportFile.AddColumn("Weekdays", TA_LEFT, 550);

	for (int nIdx = 0; nIdx < DataManager.TimePlan.GetSortArraySize(); nIdx++)
	{
		int nPlanIdx = DataManager.TimePlan.GetArrayIdxFromSortIdx(nIdx);

		CTimePlansCSVRecord Plan;
		DataManager.TimePlan.GetAt(nPlanIdx, Plan);

		CString str;
		str.Format("<..>Time Plan %d, %s",
			Plan.GetPlanNo(),
			Plan.GetPlanName());

		ReportFile.WriteLine(str);
		ReportFile.WriteLine("<LI>");

		int nStart = 0, nEnd = 0;
		DataManager.TimePlanEntry.GetPlanRange(Plan.GetPlanNo(), nStart, nEnd);

		for (int nEntryIdx = nStart; nEntryIdx <= nEnd; nEntryIdx++)
		{
			CTimePlanEntriesCSVRecord Entry;
			DataManager.TimePlanEntry.GetAt(nEntryIdx, Entry);

			CCSV csvOut('\t');
			csvOut.Add(Entry.GetEntryNo());
			csvOut.Add(Entry.GetName());

			if (Entry.GetActiveFlag() == TRUE)
			{
				csvOut.Add("Yes");
			}
			else
			{
				csvOut.Add("No");
			}

			switch (Entry.GetType())
			{
			case 0:
				csvOut.Add("Price Band");
				break;

			case 1:
				csvOut.Add("Promotion");
				break;

			case 2:
				csvOut.Add("Mix Match");
				break;

			case 3:
				csvOut.Add("Operate Gate");
				break;

			case 4:
				csvOut.Add("Screen Mode");
				break;

			default:
				csvOut.Add("Unknown");
				break;
			}

			csvOut.Add(Entry.GetDefaultText(TRUE));

			{
				CString strDate = "";
				::GetReportStringFromOleDate(Entry.GetStartDate(), strDate);
				csvOut.Add(strDate);
			}

			{
				CString strDate = "";
				::GetReportStringFromOleDate(Entry.GetEndDate(), strDate);
				csvOut.Add(strDate);
			}

			{
				CString str = "";
				str.Format("%2.2d:%2.2d",
					Entry.GetStartHour(),
					Entry.GetStartMinute());
				csvOut.Add(str);
			}

			{
				CString str = "";
				str.Format("%2.2d:%2.2d",
					Entry.GetEndHour(),
					Entry.GetEndMinute());
				csvOut.Add(str);
			}

			csvOut.Add(Entry.GetEndPeriod());

			{
				CCSV csvDays;

				if (Entry.GetSundayFlag() == TRUE)
				{
					csvDays.Add("Sun");
				}

				if (Entry.GetMondayFlag() == TRUE)
				{
					csvDays.Add("Mon");
				}

				if (Entry.GetTuesdayFlag() == TRUE)
				{
					csvDays.Add("Tue");
				}

				if (Entry.GetWednesdayFlag() == TRUE)
				{
					csvDays.Add("Wed");
				}

				if (Entry.GetThursdayFlag() == TRUE)
				{
					csvDays.Add("Thu");
				}

				if (Entry.GetFridayFlag() == TRUE)
				{
					csvDays.Add("Fri");
				}

				if (Entry.GetSaturdayFlag() == TRUE)
				{
					csvDays.Add("Sat");
				}

				csvOut.Add(csvDays.GetLine());
			}

			ReportFile.WriteLine(csvOut.GetLine());
		}

		ReportFile.WriteLine("");
	}

	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp(SysFiles::TimePlanPrm);
	ReportManager.DisplayReport(Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this);
}

/**********************************************************************/

void CReportDataTableTimePlanDlg::HandleReport2()
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	ReportFile.SetStyle1("Time Plan Templates");

	ReportFile.AddColumn("No", TA_LEFT, 120);
	ReportFile.AddColumn("Description", TA_LEFT, 500);
	ReportFile.AddColumn("Enable", TA_LEFT, 150);
	ReportFile.AddColumn("Start Date", TA_LEFT, 250);
	ReportFile.AddColumn("End Date", TA_LEFT, 250);
	ReportFile.AddColumn("Start Time", TA_LEFT, 230);
	ReportFile.AddColumn("End Time", TA_LEFT, 230);
	ReportFile.AddColumn("End Period", TA_LEFT, 250);
	ReportFile.AddColumn("Weekdays", TA_LEFT, 550);

	for (int nIdx = 0; nIdx < DataManager.TimePlan.GetSortArraySize(); nIdx++)
	{
		int nPlanIdx = DataManager.TimePlan.GetArrayIdxFromSortIdx(nIdx);

		CTimePlansCSVRecord Plan;
		DataManager.TimePlan.GetAt(nPlanIdx, Plan);

		CString str;
		str.Format("<..>Time Plan %d, %s",
			Plan.GetPlanNo(),
			Plan.GetPlanName());

		ReportFile.WriteLine(str);
		ReportFile.WriteLine("<LI>");

		int nStart = 0, nEnd = 0;
		DataManager.TimePlanTemplateEntry.GetPlanRange(Plan.GetPlanNo(), nStart, nEnd);

		for (int nEntryIdx = nStart; nEntryIdx <= nEnd; nEntryIdx++)
		{
			CTimePlanEntriesCSVRecord Entry;
			DataManager.TimePlanTemplateEntry.GetAt(nEntryIdx, Entry);

			CCSV csvOut('\t');
			csvOut.Add(Entry.GetEntryNo());
			csvOut.Add(Entry.GetName());

			if (Entry.GetActiveFlag() == TRUE)
			{
				csvOut.Add("Yes");
			}
			else
			{
				csvOut.Add("No");
			}

			{
				CString strDate = "";
				::GetReportStringFromOleDate(Entry.GetStartDate(), strDate);
				csvOut.Add(strDate);
			}

			{
				CString strDate = "";
				::GetReportStringFromOleDate(Entry.GetEndDate(), strDate);
				csvOut.Add(strDate);
			}

			{
				CString str = "";
				str.Format("%2.2d:%2.2d",
					Entry.GetStartHour(),
					Entry.GetStartMinute());
				csvOut.Add(str);
			}

			{
				CString str = "";
				str.Format("%2.2d:%2.2d",
					Entry.GetEndHour(),
					Entry.GetEndMinute());
				csvOut.Add(str);
			}

			csvOut.Add(Entry.GetEndPeriod());

			{
				CCSV csvDays;

				if (Entry.GetSundayFlag() == TRUE)
				{
					csvDays.Add("Sun");
				}

				if (Entry.GetMondayFlag() == TRUE)
				{
					csvDays.Add("Mon");
				}

				if (Entry.GetTuesdayFlag() == TRUE)
				{
					csvDays.Add("Tue");
				}

				if (Entry.GetWednesdayFlag() == TRUE)
				{
					csvDays.Add("Wed");
				}

				if (Entry.GetThursdayFlag() == TRUE)
				{
					csvDays.Add("Thu");
				}

				if (Entry.GetFridayFlag() == TRUE)
				{
					csvDays.Add("Fri");
				}

				if (Entry.GetSaturdayFlag() == TRUE)
				{
					csvDays.Add("Sat");
				}

				csvOut.Add(csvDays.GetLine());
			}

			ReportFile.WriteLine(csvOut.GetLine());
		}

		ReportFile.WriteLine("");
	}

	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp(SysFiles::TimePlanTemplatePrm);
	ReportManager.DisplayReport(Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this);
}

/**********************************************************************/

void CReportDataTableTimePlanDlg::HandleReport3()
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	ReportFile.SetStyle1 ( "EPOS Version Check for Time Plan Entries" );
		
	ReportFile.AddColumn ( "No", TA_LEFT, 120 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Minimum Version", TA_LEFT, 800 );
	ReportFile.AddColumn ( "Reason", TA_LEFT, 800 );
	
	for ( int nIdx = 0; nIdx < DataManager.TimePlan.GetSortArraySize(); nIdx++ )
	{
		int nPlanIdx = DataManager.TimePlan.GetArrayIdxFromSortIdx( nIdx );

		CTimePlansCSVRecord Plan;
		DataManager.TimePlan.GetAt ( nPlanIdx, Plan );
		
		CString str;
		str.Format( "<..>Plan %d, %s",
			Plan.GetPlanNo(),
			Plan.GetPlanName() );
		
		ReportFile.WriteLine ( str );
		ReportFile.WriteLine ( "<LI>" );

		int nStart, nEnd;
		DataManager.TimePlanEntry.GetPlanRange( Plan.GetPlanNo(), nStart, nEnd );

		for ( int nEntryIdx = nStart; nEntryIdx <= nEnd; nEntryIdx++ )
		{
			CTimePlanEntriesCSVRecord Entry;
			DataManager.TimePlanEntry.GetAt( nEntryIdx, Entry );

			CMinimumSPOSVersion Version;
			Entry.CheckMinimumSPOSVersion( Version );

			CCSV csvOut ( '\t' );
			csvOut.Add( Entry.GetEntryNo() );
			csvOut.Add( Entry.GetName() );
			csvOut.Add ( Version.GetMinVerName() );
			csvOut.Add ( Version.m_strReason );

			ReportFile.WriteLine( csvOut.GetLine() );
		}
		
		ReportFile.WriteLine ( "" );
	}

	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::TimePlanVersionPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CTimePlanTypeSelectDlg::CTimePlanTypeSelectDlg(CWnd* pParent)
	: CReportDataTableDlg(pParent)
{
	m_bKeepOpen = FALSE;
	m_strTitle = "Edit Time Plan Entries";
	/*****/
	m_arrayReports.Add("Edit Entries");
	m_arrayReports.Add("Edit Templates");
}

/**********************************************************************/