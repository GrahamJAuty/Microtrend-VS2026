/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "DataManagerNonDbFuncEcrman.h"
/**********************************************************************/

CDataManagerNonDbFuncEcrman::CDataManagerNonDbFuncEcrman() : CDataManagerBase()
{
	m_nStatusEcrmanSupplier = DATAMANAGER_CLOSED;
	
	m_nStatusEcrmanSupplierBuffer = DATAMANAGER_CLOSED;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncEcrman::OpenEcrmanSupplier( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ECR_SUPPLIER );
	CFilenameUpdater FnUp ( SysFiles::EcrmanSupplier );
	return OpenCSVArray( EcrmanSupplier, info, FnUp.GetFilenameToUse(), m_nStatusEcrmanSupplier, nType, TRUE );
}

bool CDataManagerNonDbFuncEcrman::CloseEcrmanSupplier( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ECR_SUPPLIER );
	return CloseCSVArray( EcrmanSupplier, info, m_nStatusEcrmanSupplier );
}

bool CDataManagerNonDbFuncEcrman::WriteEcrmanSupplier( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ECR_SUPPLIER );
	return WriteCSVArray( EcrmanSupplier, info, m_nStatusEcrmanSupplier );
}

bool CDataManagerNonDbFuncEcrman::OpenEcrmanSupplierBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ECR_SUPPLIER );

	CFilenameUpdater FnUp ( SysFiles::EcrmanSupplier );
	if ( OpenCSVArray( EcrmanSupplierBuffer, info, FnUp.GetFilenameToUse(), m_nStatusEcrmanSupplierBuffer, DB_READWRITE, FALSE ) == FALSE )
		return FALSE;

	EcrmanSupplier.CopyFrom( EcrmanSupplierBuffer );
	return TRUE;
}

bool CDataManagerNonDbFuncEcrman::CloseEcrmanSupplierBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ECR_SUPPLIER );
	EcrmanSupplier.CopyFrom( EcrmanSupplierBuffer );
	return CloseCSVArray( EcrmanSupplierBuffer, info, m_nStatusEcrmanSupplierBuffer );
}

bool CDataManagerNonDbFuncEcrman::WriteEcrmanSupplierBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ECR_SUPPLIER );
	
	CEcrmanSupplierCSVArray temp;
	temp.CopyFrom( EcrmanSupplierBuffer );
	EcrmanSupplierBuffer.CopyFrom( EcrmanSupplier);

	bool bResult = WriteCSVArray( EcrmanSupplierBuffer, info, m_nStatusEcrmanSupplierBuffer );

	if ( bResult == FALSE )
		EcrmanSupplierBuffer.CopyFrom( temp );

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
