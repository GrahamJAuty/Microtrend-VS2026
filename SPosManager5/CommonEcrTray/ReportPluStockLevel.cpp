/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "..\SPosEcrManager5\EcrmanSupplierSelector.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "SalesHistoryFields.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ReportPluStockLevel.h"
/**********************************************************************/

CPluStockLevelBuffer::CPluStockLevelBuffer()
{
	m_strZeroDateTime = "";
	m_bWorkBackFromCurrent = TRUE;
	m_dApparentQty = 0.0;
	m_dDeliveryQty = 0.0;
}

/**********************************************************************/
/**********************************************************************/

CReportPluStockLevel::CReportPluStockLevel()
{
	m_strReportName = "";
	m_strKey = "";

	m_bLastLineWasBlank = TRUE;
	
	m_bPluRange = FALSE;
	m_nPluFrom = 0;
	m_nPluTo = 0;

	m_DepartmentSet.CreateDepartmentSet( DEPARTMENTSET_ALL );
	m_bDepartmentSort = FALSE;
	
	m_nSupplierFilter = ECRMANSUPPLIER_ALL;
	m_bSupplierSort = FALSE;
	
	m_bZeroSkip = FALSE;
	m_nReportType = REPORT_PLUSTOCK_APPARENT;
	m_nDbIdx = 0;

	m_oleDateFrom = COleDateTime ( 2003, 1, 1, 0, 0, 0 );
	m_oleDateTo = COleDateTime ( 2003, 1, 1, 0, 0, 0 );

	m_strEmailSubject = "";
	m_bGotReportLine = FALSE;

	m_pStockLevelArray = NULL;
}

/**********************************************************************/

void CReportPluStockLevel::SetReportType( int nReportType )
{
	switch( nReportType )
	{
	case REPORT_PLUSTOCK_APPARENT:
	case REPORT_PLUSTOCK_VALUATION:
	case REPORT_PLUSTOCK_REORDER:
	case REPORT_PLUSTOCK_DEL_TOTAL:
	case REPORT_PLUSTOCK_DEL_VALUE:
	case REPORT_PLUSTOCK_APPARENT_DATE:
	case REPORT_PLUSTOCK_VALUATION_DATE:
	case REPORT_PLUSTOCK_REPLENISH_DATE:
		m_nReportType = nReportType;
		break;
	}
}

/**********************************************************************/

bool CReportPluStockLevel::IsValuationReport()
{
	switch( m_nReportType )
	{
	case REPORT_PLUSTOCK_VALUATION:
	case REPORT_PLUSTOCK_VALUATION_DATE:
	case REPORT_PLUSTOCK_REORDER:
	case REPORT_PLUSTOCK_DEL_VALUE:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

CReportPluStockLevel::~CReportPluStockLevel()
{
	m_PluStockTable.ClearConsolidatedLevels();
}

/**********************************************************************/

void CReportPluStockLevel::ConsolidateStockLevels()
{
	m_strKey = "";
	m_strKey += ( m_bSupplierSort ) ? "1" : "0";
	m_strKey += ( m_bDepartmentSort ) ? "1" : "0";

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( m_nDbIdx, info, FALSE );

	m_PluStockTable.ClearConsolidatedLevels();
	m_PluStockTable.ClearFieldStructure();
	m_arrayReportFields.RemoveAll();

	m_arrayHeaderFlags.RemoveAll();
	m_arrayHeaderFlags.Add(0);
	m_arrayTotalFlags.Add(1);

	if ( m_bSupplierSort == TRUE )
	{
		m_PluStockTable.AddField( SORT_TYPE_INT );
		m_arrayReportFields.Add ( SH_FIELD_SUPP );
		m_arrayHeaderFlags.Add( ( m_bDepartmentSort == TRUE ) ? 0 : 1 );
		m_arrayTotalFlags.Add( 1 );
	}

	if ( m_bDepartmentSort == TRUE ) 
	{	
		m_PluStockTable.AddField( SORT_TYPE_INT );
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_ITEM );
		m_arrayHeaderFlags.Add(1);
		m_arrayTotalFlags.Add(1);
	}

	m_PluStockTable.AddField( SORT_TYPE_INT );
	m_arrayReportFields.Add( SH_FIELD_PLUNO );
	m_arrayHeaderFlags.Add(0);
	m_arrayTotalFlags.Add(0);

	if ( m_bDepartmentSort == FALSE ) 
	{	
		m_PluStockTable.AddField( SORT_TYPE_INT );
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_ITEM );
		m_arrayHeaderFlags.Add(0);
		m_arrayTotalFlags.Add(0);
	}

	int nSize = DataManager.Plu.GetSize();
	StatusProgress.Lock( TRUE, "Creating report" );
	
	for ( int nPluIdx = 0; nPluIdx < nSize; nPluIdx++ )
	{
		StatusProgress.SetPos( nPluIdx, nSize );

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );

		//IGNORE NON STOCK ITEMS
		if ( PluRecord.GetNonStockExternalFlag() == TRUE )
			continue;

		__int64 nPluNo = PluRecord.GetPluNoInt();
		
		double dApparentQty = 0.0;
		double dDeliveryQty = 0.0;
		if ( NULL == m_pStockLevelArray )
		{
			dApparentQty = PluRecord.GetApparentLevel();
		}
		else
		{
			dApparentQty = (*m_pStockLevelArray)[nPluIdx].m_dApparentQty;
			dDeliveryQty = (*m_pStockLevelArray)[nPluIdx].m_dDeliveryQty;
		}
		
		//FILTER BY DEPARTMENT
		int nDeptNo = PluRecord.GetBaseDeptNo();
		if ( m_DepartmentSet.GetDepartmentStatus( nDeptNo ) == FALSE )
			continue;

		//FILTER BY SUPPLIER NO
		int nSuppNo = PluRecord.GetSupplierNo();
		if ( m_nSupplierFilter != ECRMANSUPPLIER_ALL && m_nSupplierFilter != nSuppNo )
			continue;
		
		if ( m_bPluRange == TRUE )
		{
			if ( ( m_nPluFrom != 0 ) && ( nPluNo < m_nPluFrom ) )
				continue;

			if ( ( m_nPluTo != 0 ) && ( nPluNo > m_nPluTo ) )
				continue;
		}

		//SKIP ITEMS THAT DON'T NEED REORDERING
		if ( REPORT_PLUSTOCK_REORDER == m_nReportType )
		{
			if ( dApparentQty >= PluRecord.GetMinimumLevel() )
				continue;
		}

		//ZERO SKIP
		if ( m_bZeroSkip == TRUE )
		{
			if ( dApparentQty == 0.0 )
				continue;
		}

		for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
		{
			switch ( m_arrayReportFields.GetAt(n) )
			{
			case SH_FIELD_SUPP:		
				m_PluStockTable.SetBufferInt( n, PluRecord.GetSupplierNo() );	
				break;
			
			case SH_FIELD_DEPTNO_ITEM:	
				m_PluStockTable.SetBufferInt( n, PluRecord.GetBaseDeptNo() );		
				break;
			
			case SH_FIELD_PLUNO:	
				m_PluStockTable.SetBufferInt( n, nPluIdx );					
				break;
			}
		}

		CEntityTreePluStockNode Node;
		Node.SetApparentQty( dApparentQty );
		Node.SetApparentCost( dApparentQty * PluRecord.GetPurchaseCost() );
		Node.SetApparentRetail( dApparentQty * PluRecord.GetPrice(0) );
		Node.SetDeliveryQty( dDeliveryQty );
		m_PluStockTable.Consolidate( Node ); 
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

void CReportPluStockLevel::ConsolidateDeliveryTotals()
{
	m_strKey = "";
	m_strKey += ( m_bSupplierSort ) ? "1" : "0";
	m_strKey += ( m_bDepartmentSort ) ? "1" : "0";

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( m_nDbIdx, info, FALSE );

	m_PluStockTable.ClearConsolidatedLevels();
	m_PluStockTable.ClearFieldStructure();
	m_arrayReportFields.RemoveAll();

	m_arrayHeaderFlags.RemoveAll();
	m_arrayHeaderFlags.Add(0);
	m_arrayTotalFlags.Add(1);

	if ( m_bSupplierSort == TRUE )
	{
		m_PluStockTable.AddField( SORT_TYPE_INT );
		m_arrayReportFields.Add ( SH_FIELD_SUPP );
		m_arrayHeaderFlags.Add( ( m_bDepartmentSort == TRUE ) ? 0 : 1 );
		m_arrayTotalFlags.Add( 1 );
	}

	if ( m_bDepartmentSort == TRUE ) 
	{	
		m_PluStockTable.AddField( SORT_TYPE_INT );
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_ITEM );
		m_arrayHeaderFlags.Add(1);
		m_arrayTotalFlags.Add(1);
	}

	m_PluStockTable.AddField( SORT_TYPE_INT64 );
	m_arrayReportFields.Add( SH_FIELD_PLUNO );
	m_arrayHeaderFlags.Add(0);
	m_arrayTotalFlags.Add(0);

	if ( m_bDepartmentSort == FALSE ) 
	{	
		m_PluStockTable.AddField( SORT_TYPE_INT );
		m_arrayReportFields.Add( SH_FIELD_DEPTNO_ITEM );
		m_arrayHeaderFlags.Add(0);
		m_arrayTotalFlags.Add(0);
	}

	CSSFile fileAudit;
	CString strFilename = dbDatabase.GetFilePathDataPluStockAudit( m_nDbIdx );
	if ( fileAudit.Open ( strFilename, "rb" ) == FALSE )
		return;

	CString strBuffer;
	StatusProgress.Lock( TRUE, "Creating report" );
	while ( fileAudit.ReadString ( strBuffer ) == TRUE )
	{
		StatusProgress.SetPos ( fileAudit );

		CCSVPluDelivery csvIn ( strBuffer );

		COleDateTime oleLineDate ( 2003, 1, 1, 0, 0, 0 );
		
		CString strDate = csvIn.GetDate();
		if ( strDate.GetLength() == 8 )
			oleLineDate = COleDateTime ( atoi(strDate.Right(4)), atoi(strDate.Mid(2,2)), atoi(strDate.Left(2)), 0, 0, 0 ); 	
		
		//FILTER BY DATE
		if ( oleLineDate < m_oleDateFrom || oleLineDate > m_oleDateTo )
			continue;

		__int64 nPluNo = csvIn.GetPluNo();
			
		//FILTER BY PLU NO
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

		for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
		{
			switch ( m_arrayReportFields.GetAt(n) )
			{
			case SH_FIELD_SUPP:		
				m_PluStockTable.SetBufferInt( n, nSuppNo );	
				break;
			
			case SH_FIELD_DEPTNO_ITEM:	
				m_PluStockTable.SetBufferInt( n, nDeptNo );		
				break;
			
			case SH_FIELD_PLUNO:	
				m_PluStockTable.SetBufferInt64( n, nPluNo );					
				break;
			}
		}

		double dCost = 0.0;
		if ( IsValuationReport() == TRUE )
		{
			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt( nPluIdx, PluRecord );
				dCost = csvIn.GetQty() * PluRecord.GetPurchaseCost();
			}
		}
		
		CEntityTreePluStockNode Node;
		Node.SetDeliveryQty( csvIn.GetQty() );
		Node.SetDeliveryVal( dCost );
		m_PluStockTable.Consolidate( Node ); 
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

bool CReportPluStockLevel::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	bool bGotData = FALSE;
	
	CString strName;

	switch ( m_nReportType )
	{
	case REPORT_PLUSTOCK_DEL_TOTAL:
	case REPORT_PLUSTOCK_DEL_VALUE:
		{
			CString strDateFrom = m_oleDateFrom.Format ( "%d/%m/%Y" );
			CString strDateTo = m_oleDateTo.Format ( "%d/%m/%Y" );

			strName.Format ( "%s (%s, %s - %s)",
				(const char*) m_strReportName,
				dbDatabase.GetName( m_nDbIdx ),
				(const char*) strDateFrom,
				(const char*) strDateTo );
		}
		break;

	default:
		{
			strName.Format ( "%s (%s)",
				(const char*) m_strReportName,
				dbDatabase.GetName( m_nDbIdx) );
		}
		break;	
	}

	m_ReportFile.SetStyle1 ( strName );
	m_strEmailSubject = strName;

	for ( int nPos = 0; nPos < m_arrayReportFields.GetSize(); nPos++ )
	{
		switch ( m_arrayReportFields.GetAt( nPos ) )
		{
		case SH_FIELD_SUPP:		
			m_ReportFile.AddColumn ( "Supplier", TA_LEFT, 350 );
			break;

		case SH_FIELD_DEPTNO_ITEM:	
			m_ReportFile.AddColumn ( "Dpt", TA_LEFT, 100 );
			break;
				
		case SH_FIELD_PLUNO:
			m_ReportFile.AddColumn ( "Plu Number", TA_LEFT, 380 );
			m_ReportFile.AddColumn ( "Description", TA_LEFT, 500 );

			switch( m_nReportType )
			{
			case REPORT_PLUSTOCK_REORDER:
			case REPORT_PLUSTOCK_APPARENT:
				m_ReportFile.AddColumn( "Supplier Ref", TA_LEFT, 350 );
				break;
			}
		}
	}

	switch( m_nReportType )
	{
	case REPORT_PLUSTOCK_APPARENT:
		m_ReportFile.AddColumn ( "Apparent", TA_RIGHT, 350 );
		break;
		
	case REPORT_PLUSTOCK_APPARENT_DATE:
		m_ReportFile.AddColumn ( "Previous Stock", TA_RIGHT, 350 );
		break;
			
	case REPORT_PLUSTOCK_REORDER:
		m_ReportFile.AddColumn ( "Apparent", TA_RIGHT, 350 );
		m_ReportFile.AddColumn ( "Minimum", TA_RIGHT, 350 );
		m_ReportFile.AddColumn ( "Below", TA_RIGHT, 350 );
		m_ReportFile.AddColumn ( "Valuation", TA_RIGHT, 350 );
		break;

	case REPORT_PLUSTOCK_VALUATION:
		m_ReportFile.AddColumn ( "Apparent", TA_RIGHT, 350 );
		m_ReportFile.AddColumn ( "Valuation", TA_RIGHT, 350 );
		m_ReportFile.AddColumn ( "Retail", TA_RIGHT, 350 );
		break;

	case REPORT_PLUSTOCK_VALUATION_DATE:
		m_ReportFile.AddColumn ( "Previous Stock", TA_RIGHT, 350 );
		m_ReportFile.AddColumn ( "Valuation", TA_RIGHT, 350 );
		m_ReportFile.AddColumn ( "Retail", TA_RIGHT, 350 );
		break;

	case REPORT_PLUSTOCK_DEL_TOTAL:
		m_ReportFile.AddColumn ( "Quantity", TA_RIGHT, 350 );
		break;

	case REPORT_PLUSTOCK_DEL_VALUE:
		m_ReportFile.AddColumn ( "Quantity", TA_RIGHT, 350 );
		m_ReportFile.AddColumn ( "Valuation", TA_RIGHT, 350 );
		break;

	case REPORT_PLUSTOCK_REPLENISH_DATE:
		m_ReportFile.AddColumn ( "Previous Stock", TA_RIGHT, 350 );
		m_ReportFile.AddColumn ( "Deliveries", TA_RIGHT, 350 );
		break;
	}

	m_nProgress = 0;
	CEntityTreePluStockNode BlockTotalsNode;
	m_PluStockTable.GetGrandTotalNode( BlockTotalsNode );
	ProcessPluStockArray( 0, 0, BlockTotalsNode );

	if ( FALSE == m_bGotReportLine )
		WriteBlankLine( FALSE );
	
	m_ReportFile.Close();
	return TRUE;
}

/**********************************************************************/

void CReportPluStockLevel::ProcessPluStockArray( int nDepth, int nBranchIdx, CEntityTreePluStockNode BlockTotalsNode )
{
	int nFieldType = m_arrayReportFields.GetAt( nDepth );	
	bool bHeader = ( m_arrayHeaderFlags.GetAt( nDepth ) == 1 );
	bool bTotal = ( m_arrayTotalFlags.GetAt( nDepth ) == 1 );
	
	int nArraySize = m_PluStockTable.GetBranchSize(nBranchIdx);
	if ( nArraySize == 0 )
		return;

	if ( bHeader == TRUE )
		WriteHeader( nDepth );

	for ( int nNodeIdx = 0; nNodeIdx < nArraySize; nNodeIdx++ )
	{
		StatusProgress.SetPos( m_nProgress++, m_PluStockTable.GetNodeCount() );
		
		switch ( nFieldType )
		{
		case SH_FIELD_DEPTNO_ITEM:
			m_nLabelDeptNo = m_PluStockTable.GetLabelInt( nBranchIdx, nNodeIdx );
			m_strLabelDeptName = DataManager.Department.GetReportTextByDeptNo( m_nLabelDeptNo );
			break;

		case SH_FIELD_PLUNO:
			switch( m_nReportType )
			{
			case REPORT_PLUSTOCK_DEL_TOTAL:
			case REPORT_PLUSTOCK_DEL_VALUE:
				m_nLabelPluNo = m_PluStockTable.GetLabelInt64( nBranchIdx, nNodeIdx );
				if ( DataManager.Plu.FindPluByNumber( m_nLabelPluNo, m_nLabelPluIdx ) == FALSE )
					m_nLabelPluIdx = -1;
				break;

			default:
				m_nLabelPluIdx = m_PluStockTable.GetLabelInt( nBranchIdx, nNodeIdx );
				m_nLabelPluNo = DataManager.Plu.GetPluNoInt( m_nLabelPluIdx );
				break;
			}
			break;

		case SH_FIELD_SUPP:
			{
				int nSuppNo = m_PluStockTable.GetLabelInt( nBranchIdx, nNodeIdx );

				int nSuppIdx;
				if ( DataManagerNonDb.EcrmanSupplier.FindSupplierByNumber( nSuppNo, nSuppIdx ) == TRUE )
				{
					CEcrmanSupplierCSVRecord Supplier;
					DataManagerNonDb.EcrmanSupplier.GetAt( nSuppIdx, Supplier );
					m_strLabelSuppName = Supplier.GetSuppName();
				}
				else
					m_strLabelSuppName.Format ( "Supplier %d", nSuppNo );
			}
			break;
		}

		CEntityTreePluStockNode CurrentNode;
		m_PluStockTable.GetNode( nBranchIdx, nNodeIdx, CurrentNode );
				
		if ( nDepth == m_arrayReportFields.GetSize() - 1 )
			CreateReportLine( CurrentNode );		
		else
			ProcessPluStockArray( nDepth + 1, m_PluStockTable.GetNextBranchIdx( nBranchIdx, nNodeIdx ), CurrentNode );
	}

	if ( bTotal == TRUE )
		WriteTotals( nDepth, BlockTotalsNode );
}

/**********************************************************************/

void CReportPluStockLevel::CreateReportLine( CEntityTreePluStockNode CurrentNode )
{
	double dApparentQty = CurrentNode.GetApparentQty();
	double dApparentCost = CurrentNode.GetApparentCost();
	double dApparentRetail = CurrentNode.GetApparentRetail();
	double dDeliveryQty = CurrentNode.GetDeliveryQty();
	double dDeliveryVal = CurrentNode.GetDeliveryVal();

	CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );

	CPluCSVRecord PluRecord;

	if ( m_nLabelPluIdx != -1 )
		DataManager.Plu.GetAt( m_nLabelPluIdx, PluRecord );
	else
		PluRecord.SetRepText ( "*Unknown" );

	for ( int nPos = 0; nPos < m_arrayReportFields.GetSize(); nPos++ )
	{
		switch ( m_arrayReportFields.GetAt( nPos ) )
		{
		case SH_FIELD_DEPTNO_ITEM:		
			csvOut.Add( m_nLabelDeptNo );		
			break;

		case SH_FIELD_PLUNO:
			{
				csvOut.Add( ReportHelpers.GetDisplayPluNo ( m_nLabelPluNo ) );
				csvOut.Add( ReportHelpers.GetReportText( PluRecord ) );

				switch( m_nReportType )
				{
				case REPORT_PLUSTOCK_REORDER:
				case REPORT_PLUSTOCK_APPARENT:
					csvOut.Add( PluRecord.GetSupplierRef() );
					break;
				}
			}
			break;

		case SH_FIELD_SUPP:
			csvOut.Add( m_strLabelSuppName );
			break;
		}
	}

	switch( m_nReportType )
	{
	case REPORT_PLUSTOCK_APPARENT:
	case REPORT_PLUSTOCK_APPARENT_DATE:
		csvOut.Add( dApparentQty, SysInfo.GetDPQty() );
		break;
			
	case REPORT_PLUSTOCK_REORDER:
		csvOut.Add( dApparentQty, SysInfo.GetDPQty() );
		csvOut.Add( PluRecord.GetMinimumLevel(), SysInfo.GetDPQty() );
		csvOut.Add( PluRecord.GetMinimumLevel() - PluRecord.GetApparentLevel(), SysInfo.GetDPQty() );
		csvOut.Add( dApparentCost, SysInfo.GetDPValue() );
		break;

	case REPORT_PLUSTOCK_VALUATION:
	case REPORT_PLUSTOCK_VALUATION_DATE:
		csvOut.Add( dApparentQty, SysInfo.GetDPQty() );
		csvOut.Add( dApparentCost, SysInfo.GetDPValue() );
		csvOut.Add( dApparentRetail, SysInfo.GetDPValue() );break;

	case REPORT_PLUSTOCK_DEL_TOTAL:
		csvOut.Add( dDeliveryQty, SysInfo.GetDPQty() );
		break;

	case REPORT_PLUSTOCK_DEL_VALUE:
		csvOut.Add( dDeliveryQty, SysInfo.GetDPQty() );
		csvOut.Add( dDeliveryVal, SysInfo.GetDPValue() );
		break;

	case REPORT_PLUSTOCK_REPLENISH_DATE:
		csvOut.Add( dApparentQty, SysInfo.GetDPQty() );
		csvOut.Add( dDeliveryQty, SysInfo.GetDPQty() );
		break;
	}

	WriteReportLine( csvOut.GetLine() );
}

/**********************************************************************/

const char* CReportPluStockLevel::GetHeaderLine( int nDepth )
{
	CString strSpace = " ";
	CCSV csvHeader;
	
	for ( int nPos = 0; nPos < nDepth; nPos++ )
	{
		int nFieldType = m_arrayReportFields.GetAt( nPos );
		switch ( nFieldType )
		{
		case SH_FIELD_DEPTNO_ITEM:
			{
				CString strDeptNo;
				strDeptNo.Format ( "D%2.2d:", m_nLabelDeptNo );
				csvHeader.Add( strSpace + strDeptNo + m_strLabelDeptName );
			}
			break;

		case SH_FIELD_PLUNO:
			csvHeader.Add ( strSpace + ReportHelpers.GetDisplayPluNo ( m_nLabelPluNo ) );
			break;

		case SH_FIELD_SUPP:
			csvHeader.Add ( strSpace + m_strLabelSuppName );
			break;
		}
	}
	m_strHeader = csvHeader.GetLine();
	return m_strHeader;
}

/**********************************************************************/

void CReportPluStockLevel::WriteTotals( int nDepth, CEntityTreePluStockNode BlockTotalsNode )
{
	if ( IsValuationReport() == FALSE )
		return;

	CCSV csvUnderline( '\t', '"', TRUE, FALSE, FALSE );
	CCSV csvTotals( '\t', '"', TRUE, FALSE, FALSE );

	// CREATE THE UNDERLINE

	int nBlanks = 1;
	int nTotals = 1;

	for ( int nPos = 0; nPos < m_arrayReportFields.GetSize(); nPos++ )
	{
		switch ( m_arrayReportFields.GetAt( nPos ) )
		{
		case SH_FIELD_PLUNO:	
			nBlanks += ( REPORT_PLUSTOCK_REORDER == m_nReportType ) ? 3 : 2;
			break;	
		
		default:
			nBlanks += 1;	
			break;
		}
	}

	switch( m_nReportType )
	{
	case REPORT_PLUSTOCK_REORDER:
		nBlanks += 2;
		break;

	case REPORT_PLUSTOCK_VALUATION:
	case REPORT_PLUSTOCK_VALUATION_DATE:
		nTotals += 1;
		break;
	}

	for ( int n = 0; n < nBlanks; n++ )
		csvUnderline.Add ( "" );

	for ( int n = 0; n < nTotals; n++ )
		csvUnderline.Add ( "<LI>" );

	//CREATE THE TOTALS LINE
	
	CString strTotal;
	if ( nDepth == 0 )
		strTotal = "<..>Grand Total";
	else
	{
		strTotal = "<..>Totals (";
		strTotal += GetHeaderLine ( nDepth );
		strTotal += " )";
	}

	csvTotals.Add( strTotal );

	for ( int n = 0; n < nBlanks - 1; n++ )
		csvTotals.Add ( "" );

	switch( m_nReportType )
	{
	case REPORT_PLUSTOCK_DEL_VALUE:
		csvTotals.Add( BlockTotalsNode.GetDeliveryVal(), SysInfo.GetDPValue() );
		break;

	case REPORT_PLUSTOCK_VALUATION:
	case REPORT_PLUSTOCK_VALUATION_DATE:
		csvTotals.Add( BlockTotalsNode.GetApparentCost(), SysInfo.GetDPValue() );
		csvTotals.Add( BlockTotalsNode.GetApparentRetail(), SysInfo.GetDPValue() );
		break;

	case REPORT_PLUSTOCK_REORDER:
		csvTotals.Add( BlockTotalsNode.GetApparentCost(), SysInfo.GetDPValue() );
		break;
	}

	WriteReportLine( csvUnderline.GetLine() );
	WriteReportLine( csvTotals.GetLine() );
	WriteBlankLine( FALSE );
}

/**********************************************************************/

void CReportPluStockLevel::WriteBlankLine( bool bOnlyIfNeeded )
{
	if ( bOnlyIfNeeded && m_bLastLineWasBlank )
		return;

	m_ReportFile.WriteLine ( "" );
	m_bLastLineWasBlank = TRUE;
}

/**********************************************************************/

void CReportPluStockLevel::WriteHeader( int nDepth )
{
	CString strTemp = GetHeaderLine(nDepth);
	if ( strTemp.GetLength() >= 1 ) strTemp = strTemp.Right( strTemp.GetLength() - 1 );
	CString strHeader = "<..>";
	strHeader += strTemp;
	WriteBlankLine( TRUE );
	WriteReportLine( strHeader );
	WriteReportLine( "<LI>" );
}

/**********************************************************************/

void CReportPluStockLevel::WriteReportLine( const char* szLine )
{
	CString strLine = szLine;
	if ( strLine == "" )
		return;

	m_bGotReportLine = TRUE;
	m_ReportFile.WriteLine( strLine );
	m_bLastLineWasBlank = FALSE;
}

/**********************************************************************/
