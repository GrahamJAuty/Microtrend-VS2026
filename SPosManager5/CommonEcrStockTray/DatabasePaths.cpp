/**********************************************************************/
#include "..\SPosStockManager5\ProcessedOrder.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
/**********************************************************************/

const char* CDatabaseCSVArray::GetFolderPathData( int nDbIdx )
{
	m_strFolderPath = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx ); 
		m_strFolderPath.Format ( "Db%3.3d", pDatabase -> m_nDbNo );
		GetDataProgramPath( m_strFolderPath );
	}
	return m_strFolderPath;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFolderPathDataDelete( int nDbIdx )
{
	m_strFolderPathDelete = GetFolderPathData( nDbIdx );
	m_strFolderPathDelete += "XXX";
	return m_strFolderPathDelete;	
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFolderPathDataHistory( int nDbIdx )
{
	m_strFolderPath = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		m_strFolderPath = GetFolderPathData( nDbIdx );
		m_strFolderPath += "\\History";
	}
	return m_strFolderPath;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFolderPathDataPluImage( int nDbIdx )
{
	m_strFolderPath = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		m_strFolderPath = GetFolderPathData( nDbIdx );
		m_strFolderPath += "\\PluImage";
	}
	return m_strFolderPath;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFolderPathDataKeyImage( int nDbIdx )
{
	m_strFolderPath = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		m_strFolderPath = GetFolderPathData( nDbIdx );
		m_strFolderPath += "\\KeyImage";
	}
	return m_strFolderPath;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFolderPathDataPluInfo( int nDbIdx )
{
	m_strFolderPath = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		m_strFolderPath = GetFolderPathData( nDbIdx );
		m_strFolderPath += "\\PluInfo";
	}
	return m_strFolderPath;
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

const char* CDatabaseCSVArray::GetFolderPathDataOrdersRoot( int nDbIdx )
{
	m_strFolderPathOrder = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		m_strFolderPathOrder = GetFolderPathData( nDbIdx );
		m_strFolderPathOrder += "\\Orders";
	}
	return m_strFolderPathOrder;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFolderPathDataOrdersSupplier( int nDbIdx, int nSuppNo )
{
	m_strFolderPathOrder = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		m_strFolderPathOrder.Format( "%s\\Orders\\Supp%3.3d",
			GetFolderPathData( nDbIdx ),
			nSuppNo );
	}
	return m_strFolderPathOrder;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFolderPathDataOrdersType( int nDbIdx, int nSuppNo, int nType )
{
	m_strFolderPathOrder = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		switch( nType )
		{
		case ORDER_TYPE_OPEN:
			m_strFolderPathOrder.Format( "%s\\Orders\\Supp%3.3d\\Open",
				GetFolderPathData( nDbIdx ),
				nSuppNo );
			break;

		case ORDER_TYPE_MIXED:
			m_strFolderPathOrder.Format( "%s\\Orders\\Supp%3.3d\\Mixed",
				GetFolderPathData( nDbIdx ),
				nSuppNo );
			break;

		case ORDER_TYPE_CLOSED:
		default:
			m_strFolderPathOrder.Format( "%s\\Orders\\Supp%3.3d\\Closed",
				GetFolderPathData( nDbIdx ),
				nSuppNo );
			break;
		}
	}
	return m_strFolderPathOrder;
}

const char* CDatabaseCSVArray::GetFilePathDataOrder( int nDbIdx, int nSuppNo, int nType, int nOrderNum, CString& strDate, CString& strTime )
{
	if ( strDate.GetLength() != 8 )
		strDate = "00000000";

	if ( strTime.GetLength() != 6 )
		strTime = "000000";

	m_strFilePath = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		switch( nType )
		{
		case ORDER_TYPE_OPEN:
			m_strFilePath.Format( "%s\\Orders\\Supp%3.3d\\Open\\%6.6d",
				GetFolderPathData( nDbIdx ),
				nSuppNo,
				nOrderNum );
			break;

		case ORDER_TYPE_MIXED:
			m_strFilePath.Format( "%s\\Orders\\Supp%3.3d\\Mixed\\%6.6d",
				GetFolderPathData( nDbIdx ),
				nSuppNo,
				nOrderNum );
			break;

		case ORDER_TYPE_CLOSED:
		default:
			m_strFilePath.Format( "%s\\Orders\\Supp%3.3d\\Closed\\%6.6d",
				GetFolderPathData( nDbIdx ),
				nSuppNo,
				nOrderNum );
			break;
		}

		CString strHyphen = "-";
		m_strFilePath += strHyphen + strDate + strHyphen + strTime + ".dat";
	}
	return m_strFilePath;
}

/**********************************************************************/
#endif
/**********************************************************************/

const char* CDatabaseCSVArray::GetFilePathDataLocalPluField( int nDbIdx, int nSet )
{
	m_strFilePathLocalPluField = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CFilenameUpdater FnUp( SysFiles::DbPriceSetFields, nDbIdx, nSet );
		m_strFilePathLocalPluField = FnUp.GetFilenameToUse();
	}
		
	return m_strFilePathLocalPluField;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFilePathDataLocalPluTemp( int nDbIdx, int nSet )
{
	m_strFilePathLocalPluTemp = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CFilenameUpdater FnUp( SysFiles::DbPriceSetTemp, nDbIdx, nSet );
		m_strFilePathLocalPluTemp = FnUp.GetFilenameToUse();
	}
	return m_strFilePathLocalPluTemp;
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

const char* CDatabaseCSVArray::GetFilePathDataStockExport ( int nDbIdx )
{
	m_strFilePathStockExport = GetFolderPathData ( nDbIdx );
	m_strFilePathStockExport += "\\stkxfer.dat";
	return m_strFilePathStockExport;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFilePathDataGenDeptManualSales ( int nDbIdx )
{
	m_strFilePathGenDeptManualSales = GetFolderPathDataHistory ( nDbIdx );
	m_strFilePathGenDeptManualSales += "\\GenDpt02.dat";
	return m_strFilePathGenDeptManualSales;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFilePathDataGenDeptManualSalesBackup ( int nDbIdx )
{
	m_strFilePathGenDeptManualSalesBackup = GetFolderPathDataHistory ( nDbIdx );
	m_strFilePathGenDeptManualSalesBackup += "\\GenDpt02.bak";
	return m_strFilePathGenDeptManualSalesBackup;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFilePathDataGenDeptImportSales ( int nDbIdx )
{
	m_strFilePathGenDeptImportSales = GetFolderPathDataHistory ( nDbIdx );
	m_strFilePathGenDeptImportSales += "\\GenDpt03.dat";
	return m_strFilePathGenDeptImportSales;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFilePathDataGenDeptImportSalesBackup ( int nDbIdx )
{
	m_strFilePathGenDeptImportSalesBackup = GetFolderPathDataHistory ( nDbIdx );
	m_strFilePathGenDeptImportSalesBackup += "\\GenDpt03.bak";
	return m_strFilePathGenDeptImportSalesBackup;
}

/**********************************************************************/
#else
/**********************************************************************/

const char* CDatabaseCSVArray::GetFilePathDataPluStockAudit( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::PluStockAudit, nDbIdx );
	m_strFilePathPluStockAudit = FnUp.GetFilenameToUse();
	return m_strFilePathPluStockAudit;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFilePathDataPluStockAuditTemp( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::PluStockAuditTmp, nDbIdx );
	m_strFilePathPluStockAuditTemp = FnUp.GetFilenameToUse();
	return m_strFilePathPluStockAuditTemp;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFilePathDataPluStockAuditBackup( int nDbIdx )
{
	CFilenameUpdater FnUp( SysFiles::PluStockAuditBak, nDbIdx );
	m_strFilePathPluStockAuditBackup = FnUp.GetFilenameToUse();
	return m_strFilePathPluStockAuditBackup;
}

/**********************************************************************/
#endif
/**********************************************************************/

