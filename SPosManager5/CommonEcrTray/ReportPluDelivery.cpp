/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "..\SPosEcrManager5\EcrmanSupplierSelector.h"
#include "RepCsv.h"
#include "ReportHelpers.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ReportPluDelivery.h"
/**********************************************************************/

CReportPluDelivery::CReportPluDelivery()  
{
	m_nDbIdx = 0;

	m_bPluRange = FALSE;
	m_nPluFrom = 0;
	m_nPluTo = 0;
	
	SetDepartmentFilter(0);
	m_nSupplierFilter = ECRMANSUPPLIER_ALL;

	m_oleDateFrom = COleDateTime ( 2003, 1, 1, 0, 0, 0 );
	m_oleDateTo = COleDateTime ( 2003, 1, 1, 0, 0, 0 );

	m_strEmailSubject = "";
}

/**********************************************************************/

bool CReportPluDelivery::CreateDeliveryAuditReport()
{
	StatusProgress.Lock( TRUE, "Creating report" );
	bool bResult = CreateAuditInternal();
	StatusProgress.Unlock();
	return bResult;
}

/**********************************************************************/

bool CReportPluDelivery::CreateAuditInternal()
{
	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	int nDbIdx = DataManager.GetActiveDbIdx();
		
	CString strDateFrom = m_oleDateFrom.Format ( "%d/%m/%Y" );
	CString strDateTo = m_oleDateTo.Format ( "%d/%m/%Y" );

	CString strName;
	strName.Format ( "Plu Delivery Audit (%s, %s - %s)",
		dbDatabase.GetName( m_nDbIdx ),
		(const char*) strDateFrom,
		(const char*) strDateTo );

	m_strEmailSubject = strName;
	
	ReportFile.SetStyle1 ( strName );
	ReportFile.AddColumn ( "Date", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	ReportFile.AddColumn ( "PluNo", TA_LEFT, 380 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	ReportFile.AddColumn ( "Dpt", TA_LEFT, 100 );
	ReportFile.AddColumn ( "Supp", TA_LEFT, 120 );
	ReportFile.AddColumn ( "Quantity", TA_RIGHT, 350 );
	ReportFile.AddColumn ( "", TA_LEFT, 175 );

	CSSFile fileAudit;
	if ( fileAudit.Open ( dbDatabase.GetFilePathDataPluStockAudit( m_nDbIdx ), "rb" ) == FALSE )
	{
		ReportFile.WriteLine ( "" );
		return TRUE;
	}

	bool bGotLine = FALSE;

	CString strBuffer;
	while ( fileAudit.ReadString ( strBuffer ) == TRUE )
	{
		StatusProgress.SetPos ( fileAudit );

		CCSVPluDelivery csvIn ( strBuffer );
		
		COleDateTime oleLineDate ( 2003, 1, 1, 0, 0, 0 );
		
		CString strDate = csvIn.GetDate();
		
		if ( strDate.GetLength() == 8 )
		{
			::MakeYearFirstDate( strDate );
			oleLineDate = COleDateTime ( atoi(strDate.Left(4)), atoi(strDate.Mid(4,2)), atoi(strDate.Right(2)), 0, 0, 0 ); 	
		}
		
		if ( oleLineDate.m_status != COleDateTime::valid )
			continue;

		//FILTER BY DATE
		if ( oleLineDate < m_oleDateFrom || oleLineDate > m_oleDateTo )
			continue;

		//FILTER BY PLU NO
		__int64 nPluNo = csvIn.GetPluNo();
				
		if ( m_bPluRange == TRUE )
		{
			if ( ( m_nPluFrom != 0 ) && ( nPluNo < m_nPluFrom ) )
				continue;

			if ( ( m_nPluTo != 0 ) && ( nPluNo > m_nPluTo ) )
				continue;
		}

		//FILTER BY DEPARTMENT
		int nDeptNo = csvIn.GetDeptNo();
		if ( m_DepartmentSet.GetDepartmentStatus ( nDeptNo ) == FALSE )
			continue;

		//FILTER BY SUPPLIER NO
		int nSuppNo = csvIn.GetSupplierNo();
		if ( m_nSupplierFilter != ECRMANSUPPLIER_ALL && m_nSupplierFilter != nSuppNo )
			continue;

		CString strReportTime = "";
		CString strLineTime = csvIn.GetTime();
		if ( strLineTime.GetLength() == 4 )
		{
			strReportTime = strLineTime.Left(2) + ":";
			strReportTime += strLineTime.Right(2);
		}
					
		CCSV csvOut ( '\t' );
		csvOut.Add ( oleLineDate.Format ( "%d/%m/%Y" ) );
		csvOut.Add ( strReportTime );		
		csvOut.Add ( ReportHelpers.GetDisplayPluNo ( nPluNo ) );
		csvOut.Add ( csvIn.GetRepText() );
		csvOut.Add ( nDeptNo );
		csvOut.Add ( nSuppNo );
		csvOut.Add ( csvIn.GetQty(), SysInfo.GetDPQty() );
					
		if ( csvIn.GetClearFlag() == TRUE )
			csvOut.Add ( "Cleared" );

		ReportFile.WriteLine ( csvOut.GetLine() );
		bGotLine = TRUE;		
	}
	
	if ( bGotLine == FALSE )
		ReportFile.WriteLine ( "" );
		
	ReportFile.Close();
	return TRUE;
}

/**********************************************************************/

