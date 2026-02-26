/**********************************************************************/
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DataManagerNonDbFuncMain.h"
/**********************************************************************/

CDataManagerNonDbFuncMain::CDataManagerNonDbFuncMain() : CDataManagerBase(),
	FunctionDatabase( FUNCTIONFILE_DATABASE, m_FunctionAuthTable ),
	FunctionLocation( FUNCTIONFILE_LOCATION, m_FunctionAuthTable ),
	FunctionSystem( FUNCTIONFILE_SYSTEM, m_FunctionAuthTable ),
	OPGroupDatabase( OPGROUP_DATABASE, m_OPGroupTable ),
	OPGroupLocation( OPGROUP_LOCATION, m_OPGroupTable ),
	OPGroupSystem( OPNAME_SYSTEM, m_OPGroupTable ),
	OPNameDatabase(OPNAME_DATABASE, m_OPNameTable),
	OPNameLocation(OPNAME_LOCATION, m_OPNameTable),
	OPNameSystem(OPGROUP_SYSTEM, m_OPNameTable),
	PluUnit( NODE_PLUUNIT ), 
	PluUnitBuffer( NODE_PLUUNIT ),
	ServerPresetDatabase( SVRPRESETFILE_DATABASE, m_ServerPresetTable ),
	ServerPresetLocation( SVRPRESETFILE_LOCATION, m_ServerPresetTable ),
	ServerPresetSystem( SVRPRESETFILE_SYSTEM, m_ServerPresetTable ),
	ServerPresetTerminal( SVRPRESETFILE_TERMINAL, m_ServerPresetTable )
{
	m_nStatusAllergen = DATAMANAGER_CLOSED;
	m_nStatusCCMacroDatabase = DATAMANAGER_CLOSED;
	m_nStatusCCMacroKeyboard = DATAMANAGER_CLOSED;
	m_nStatusCCMacroLocation = DATAMANAGER_CLOSED;
	m_nStatusCCMacroSystem = DATAMANAGER_CLOSED;
	m_nStatusCCReasonDatabase = DATAMANAGER_CLOSED;
	m_nStatusCCReasonLocation = DATAMANAGER_CLOSED;
	m_nStatusCCReasonSystem = DATAMANAGER_CLOSED;
	m_nStatusCustomerHeader = DATAMANAGER_CLOSED;
	m_nStatusFunctionDatabase = DATAMANAGER_CLOSED;
	m_nStatusFunctionLocation = DATAMANAGER_CLOSED;
	m_nStatusFunctionSystem = DATAMANAGER_CLOSED;
	m_nStatusOPGroupDatabase = DATAMANAGER_CLOSED;
	m_nStatusOPGroupLocation = DATAMANAGER_CLOSED;
	m_nStatusOPGroupSystem = DATAMANAGER_CLOSED;
	m_nStatusOPNameDatabase = DATAMANAGER_CLOSED;
	m_nStatusOPNameLocation = DATAMANAGER_CLOSED;
	m_nStatusOPNameSystem = DATAMANAGER_CLOSED;
	m_nStatusPluUnit = DATAMANAGER_CLOSED;
	m_nStatusSalesHistoryCustomer = DATAMANAGER_CLOSED;
	m_nStatusServerPresetDatabase = DATAMANAGER_CLOSED;
	m_nStatusServerPresetLocation = DATAMANAGER_CLOSED;
	m_nStatusServerPresetSystem = DATAMANAGER_CLOSED;
	m_nStatusServerPresetTerminal = DATAMANAGER_CLOSED;
	m_nStatusServerRestriction = DATAMANAGER_CLOSED;
	m_nStatusTableAreas = DATAMANAGER_CLOSED;
	
	m_nStatusPluUnitBuffer = DATAMANAGER_CLOSED;
	m_nStatusSalesHistoryCustomerBuffer = DATAMANAGER_CLOSED;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenAllergen( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ALLERGEN );
	CFilenameUpdater FnUp ( SysFiles::AllergenList );
	return OpenCSVArray( Allergen, info, FnUp.GetFilenameToUse(), m_nStatusAllergen, nType, FALSE );
}

bool CDataManagerNonDbFuncMain::CloseAllergen( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ALLERGEN );
	return CloseCSVArray( Allergen, info, m_nStatusAllergen );
}

bool CDataManagerNonDbFuncMain::WriteAllergen( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_ALLERGEN );
	return WriteCSVArray( Allergen, info, m_nStatusAllergen );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenCCMacroDatabase( int nDbIdx, int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CCMACRO_DATABASE );
	CFilenameUpdater FnUp ( SysFiles::CascadingMacro, nDbIdx, -1, FNUP_FAMILY_DATABASE );
	return OpenCSVArray( CCMacroDatabase, info, FnUp.GetFilenameToUse(), m_nStatusCCMacroDatabase, nType, FALSE );
}

bool CDataManagerNonDbFuncMain::CloseCCMacroDatabase( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CCMACRO_DATABASE );
	return CloseCSVArray( CCMacroDatabase, info, m_nStatusCCMacroDatabase );
}

bool CDataManagerNonDbFuncMain::WriteCCMacroDatabase( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CCMACRO_DATABASE );
	CCMacroDatabase.PrepareForWrite();
	return WriteCSVArray( CCMacroDatabase, info, m_nStatusCCMacroDatabase );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenCCMacroKeyboard( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CCMACRO_KEYBOARD );
	return OpenCSVArray( CCMacroKeyboard, info, m_strKeyboardMacroFilename, m_nStatusCCMacroKeyboard, nType, FALSE );
}

bool CDataManagerNonDbFuncMain::CloseCCMacroKeyboard( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CCMACRO_KEYBOARD );
	return CloseCSVArray( CCMacroKeyboard, info, m_nStatusCCMacroKeyboard );
}

bool CDataManagerNonDbFuncMain::WriteCCMacroKeyboard( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CCMACRO_KEYBOARD );
	CCMacroKeyboard.PrepareForWrite();
	return WriteCSVArray( CCMacroKeyboard, info, m_nStatusCCMacroKeyboard );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenCCMacroLocation( int nLocIdx, int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CCMACRO_LOCATION );
	CFilenameUpdater FnUp ( SysFiles::CascadingMacro, nLocIdx, -1, FNUP_FAMILY_LOCATION );
	return OpenCSVArray( CCMacroLocation, info, FnUp.GetFilenameToUse(), m_nStatusCCMacroLocation, nType, FALSE );
}

bool CDataManagerNonDbFuncMain::CloseCCMacroLocation( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CCMACRO_LOCATION );
	return CloseCSVArray( CCMacroLocation, info, m_nStatusCCMacroLocation );
}

bool CDataManagerNonDbFuncMain::WriteCCMacroLocation( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CCMACRO_LOCATION );
	CCMacroLocation.PrepareForWrite();
	return WriteCSVArray( CCMacroLocation, info, m_nStatusCCMacroLocation );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenCCMacroSystem( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CCMACRO_SYSTEM );
	CFilenameUpdater FnUp ( SysFiles::CascadingMacro );
	return OpenCSVArray( CCMacroSystem, info, FnUp.GetFilenameToUse(), m_nStatusCCMacroSystem, nType, FALSE );
}

bool CDataManagerNonDbFuncMain::CloseCCMacroSystem( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CCMACRO_SYSTEM );
	return CloseCSVArray( CCMacroSystem, info, m_nStatusCCMacroSystem );
}

bool CDataManagerNonDbFuncMain::WriteCCMacroSystem( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CCMACRO_SYSTEM );
	CCMacroSystem.PrepareForWrite();	
	return WriteCSVArray( CCMacroSystem, info, m_nStatusCCMacroSystem );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CDataManagerNonDbFuncMain::GetSysInfoReasonText(int nReasonType, SysFileInfo& SysInfo )
{
	switch (nReasonType)
	{
	case REASONTEXT_TYPE_TRANVOID:
		SysInfo = SysFiles::ReasonTextTranVoid;
		break;

	case REASONTEXT_TYPE_REFUND:
		SysInfo = SysFiles::ReasonTextRefund;
		break;

	case REASONTEXT_TYPE_VIP:
		SysInfo = SysFiles::ReasonTextVIP;
		break;

	case REASONTEXT_TYPE_PROMO:
		SysInfo = SysFiles::ReasonTextPromo;
		break;

	case REASONTEXT_TYPE_WASTAGE:
		SysInfo = SysFiles::ReasonTextWastage;
		break;

	case REASONTEXT_TYPE_NOSALE:
		SysInfo = SysFiles::ReasonTextNoSale;
		break;

	case REASONTEXT_TYPE_VOIDSURCHARGE:
		SysInfo = SysFiles::ReasonTextVoidSurcharge;
		break;

	case REASONTEXT_TYPE_ITEMVOID:
	default:
		SysInfo = SysFiles::ReasonTextItemVoid;
		break;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenCCReasonDatabase(int nDbIdx, int nAccessType, int nReasonType, CDataManagerInfo& info)
{
	ResetInfo(info, NODE_CCREASON_DATABASE);

	SysFileInfo SysInfo;
	GetSysInfoReasonText(nReasonType, SysInfo);
	CFilenameUpdater FnUp(SysInfo, nDbIdx, -1, FNUP_FAMILY_DATABASE);
	CCReasonDatabase.SetReasonType( nReasonType );
	return OpenCSVArray(CCReasonDatabase, info, FnUp.GetFilenameToUse(), m_nStatusCCReasonDatabase, nAccessType, FALSE);
}

bool CDataManagerNonDbFuncMain::CloseCCReasonDatabase(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_CCREASON_DATABASE);
	return CloseCSVArray(CCReasonDatabase, info, m_nStatusCCReasonDatabase);
}

bool CDataManagerNonDbFuncMain::WriteCCReasonDatabase(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_CCREASON_DATABASE);
	CCReasonDatabase.PrepareForWrite();
	return WriteCSVArray(CCReasonDatabase, info, m_nStatusCCReasonDatabase);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenCCReasonLocation(int nLocIdx, int nAccessType, int nReasonType, CDataManagerInfo& info)
{
	ResetInfo(info, NODE_CCREASON_LOCATION);
	
	SysFileInfo SysInfo;
	GetSysInfoReasonText(nReasonType, SysInfo);	
	CFilenameUpdater FnUp(SysInfo, nLocIdx, -1, FNUP_FAMILY_LOCATION);
	CCReasonLocation.SetReasonType(nReasonType);
	return OpenCSVArray(CCReasonLocation, info, FnUp.GetFilenameToUse(), m_nStatusCCReasonLocation, nAccessType, FALSE);
}

bool CDataManagerNonDbFuncMain::CloseCCReasonLocation(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_CCREASON_LOCATION);
	return CloseCSVArray(CCReasonLocation, info, m_nStatusCCReasonLocation);
}

bool CDataManagerNonDbFuncMain::WriteCCReasonLocation(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_CCREASON_LOCATION);
	CCReasonLocation.PrepareForWrite();
	return WriteCSVArray(CCReasonLocation, info, m_nStatusCCReasonLocation);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenCCReasonSystem(int nAccessType, int nReasonType, CDataManagerInfo& info)
{
	ResetInfo(info, NODE_CCREASON_SYSTEM);
	
	SysFileInfo SysInfo;
	GetSysInfoReasonText(nReasonType, SysInfo);
	CFilenameUpdater FnUp(SysInfo);
	CCReasonSystem.SetReasonType(nReasonType);
	return OpenCSVArray(CCReasonSystem, info, FnUp.GetFilenameToUse(), m_nStatusCCReasonSystem, nAccessType, FALSE);
}

bool CDataManagerNonDbFuncMain::CloseCCReasonSystem(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_CCREASON_SYSTEM);
	return CloseCSVArray(CCReasonSystem, info, m_nStatusCCReasonSystem);
}

bool CDataManagerNonDbFuncMain::WriteCCReasonSystem(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_CCREASON_SYSTEM);
	CCReasonSystem.PrepareForWrite();
	return WriteCSVArray(CCReasonSystem, info, m_nStatusCCReasonSystem);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenCustomerHeader( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CUSTOMER_HEADER );
	CFilenameUpdater FnUp ( SysFiles::CustomerHeader );
	return OpenStringArray( CustomerHeader, info, FnUp.GetFilenameToUse(), m_nStatusCustomerHeader, nType, FALSE );
}

bool CDataManagerNonDbFuncMain::CloseCustomerHeader( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CUSTOMER_HEADER );
	return CloseStringArray( CustomerHeader, info, m_nStatusCustomerHeader );
}

bool CDataManagerNonDbFuncMain::WriteCustomerHeader( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_CUSTOMER_HEADER );
	return WriteStringArray( CustomerHeader, info, m_nStatusCustomerHeader );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenFunctionDatabase( int nDbIdx, int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_FUNCTION_DATABASE );
		CFilenameUpdater FnUp ( SysFiles::FunctionAuthority, nDbIdx, -1, FNUP_FAMILY_DATABASE );
		return OpenStringArray( FunctionDatabase, info, FnUp.GetFilenameToUse(), m_nStatusFunctionDatabase, nType, FALSE );
	}
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseFunctionDatabase( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_FUNCTION_DATABASE );
	return CloseStringArray( FunctionDatabase, info, m_nStatusFunctionDatabase );
}

bool CDataManagerNonDbFuncMain::WriteFunctionDatabase( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_FUNCTION_DATABASE );
		FunctionDatabase.PrepareForWrite();
		return WriteStringArray( FunctionDatabase, info, m_nStatusFunctionDatabase );
	}
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenFunctionLocation( int nLocIdx, int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_FUNCTION_LOCATION );
		CFilenameUpdater FnUp ( SysFiles::FunctionAuthority, nLocIdx, -1, FNUP_FAMILY_LOCATION );
		return OpenStringArray( FunctionLocation, info, FnUp.GetFilenameToUse(), m_nStatusFunctionLocation, nType, FALSE );
	}
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseFunctionLocation( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_FUNCTION_LOCATION );
	return CloseStringArray( FunctionLocation, info, m_nStatusFunctionLocation );
}

bool CDataManagerNonDbFuncMain::WriteFunctionLocation( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_FUNCTION_LOCATION );
		FunctionLocation.PrepareForWrite();
		return WriteStringArray( FunctionLocation, info, m_nStatusFunctionLocation );
	}
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenFunctionSystem( int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_FUNCTION_SYSTEM );
		CFilenameUpdater FnUp ( SysFiles::FunctionAuthority );
		return OpenStringArray( FunctionSystem, info, FnUp.GetFilenameToUse(), m_nStatusFunctionSystem, nType, FALSE );
	}
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseFunctionSystem( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_FUNCTION_SYSTEM );
	return CloseStringArray( FunctionSystem, info, m_nStatusFunctionSystem );
}

bool CDataManagerNonDbFuncMain::WriteFunctionSystem( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_FUNCTION_SYSTEM );
		FunctionSystem.PrepareForWrite();	
		return WriteStringArray( FunctionSystem, info, m_nStatusFunctionSystem );
	}
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenOPGroupDatabase( int nDbIdx, int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_OPGROUP_DATABASE );
		CFilenameUpdater FnUp ( SysFiles::OPGroup, nDbIdx, -1, FNUP_FAMILY_DATABASE );
		return OpenStringArray( OPGroupDatabase, info, FnUp.GetFilenameToUse(), m_nStatusOPGroupDatabase, nType, FALSE );
	}
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseOPGroupDatabase( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_OPGROUP_DATABASE );
	return CloseStringArray( OPGroupDatabase, info, m_nStatusOPGroupDatabase );
}

bool CDataManagerNonDbFuncMain::WriteOPGroupDatabase( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_OPGROUP_DATABASE );
		OPGroupDatabase.PrepareForWrite();
		return WriteStringArray( OPGroupDatabase, info, m_nStatusOPGroupDatabase );
	}
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenOPGroupLocation( int nLocIdx, int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_OPGROUP_LOCATION );
		CFilenameUpdater FnUp ( SysFiles::OPGroup, nLocIdx, -1, FNUP_FAMILY_LOCATION );
		return OpenStringArray( OPGroupLocation, info, FnUp.GetFilenameToUse(), m_nStatusOPGroupLocation, nType, FALSE );
	}
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseOPGroupLocation( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_OPGROUP_LOCATION );
	return CloseStringArray( OPGroupLocation, info, m_nStatusOPGroupLocation );
}

bool CDataManagerNonDbFuncMain::WriteOPGroupLocation( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_OPGROUP_LOCATION );
		OPGroupLocation.PrepareForWrite();
		return WriteStringArray( OPGroupLocation, info, m_nStatusOPGroupLocation );
	}
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenOPGroupSystem( int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_OPGROUP_SYSTEM );
		CFilenameUpdater FnUp ( SysFiles::OPGroup );
		return OpenStringArray( OPGroupSystem, info, FnUp.GetFilenameToUse(), m_nStatusOPGroupSystem, nType, FALSE );
	}
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseOPGroupSystem( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_OPGROUP_SYSTEM );
	return CloseStringArray( OPGroupSystem, info, m_nStatusOPGroupSystem );
}

bool CDataManagerNonDbFuncMain::WriteOPGroupSystem( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_OPGROUP_SYSTEM );
		OPGroupSystem.PrepareForWrite();	
		return WriteStringArray( OPGroupSystem, info, m_nStatusOPGroupSystem );
	}
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenOPNameDatabase(int nDbIdx, int nType, CDataManagerInfo& info)
{
	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		ResetInfo(info, NODE_OPNAME_DATABASE);
		CFilenameUpdater FnUp(SysFiles::OPName, nDbIdx, -1, FNUP_FAMILY_DATABASE);
		return OpenStringArray(OPNameDatabase, info, FnUp.GetFilenameToUse(), m_nStatusOPNameDatabase, nType, FALSE);
	}
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseOPNameDatabase(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_OPNAME_DATABASE);
	return CloseStringArray(OPNameDatabase, info, m_nStatusOPNameDatabase);
}

bool CDataManagerNonDbFuncMain::WriteOPNameDatabase(CDataManagerInfo& info)
{
	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		ResetInfo(info, NODE_OPNAME_DATABASE);
		OPNameDatabase.PrepareForWrite();
		return WriteStringArray(OPNameDatabase, info, m_nStatusOPNameDatabase);
	}
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenOPNameLocation(int nLocIdx, int nType, CDataManagerInfo& info)
{
	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		ResetInfo(info, NODE_OPNAME_LOCATION);
		CFilenameUpdater FnUp(SysFiles::OPName, nLocIdx, -1, FNUP_FAMILY_LOCATION);
		return OpenStringArray(OPNameLocation, info, FnUp.GetFilenameToUse(), m_nStatusOPNameLocation, nType, FALSE);
	}
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseOPNameLocation(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_OPNAME_LOCATION);
	return CloseStringArray(OPNameLocation, info, m_nStatusOPNameLocation);
}

bool CDataManagerNonDbFuncMain::WriteOPNameLocation(CDataManagerInfo& info)
{
	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		ResetInfo(info, NODE_OPNAME_LOCATION);
		OPNameLocation.PrepareForWrite();
		return WriteStringArray(OPNameLocation, info, m_nStatusOPNameLocation);
	}
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenOPNameSystem(int nType, CDataManagerInfo& info)
{
	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		ResetInfo(info, NODE_OPNAME_SYSTEM);
		CFilenameUpdater FnUp(SysFiles::OPName);
		return OpenStringArray(OPNameSystem, info, FnUp.GetFilenameToUse(), m_nStatusOPNameSystem, nType, FALSE);
	}
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseOPNameSystem(CDataManagerInfo& info)
{
	ResetInfo(info, NODE_OPNAME_SYSTEM);
	return CloseStringArray(OPNameSystem, info, m_nStatusOPNameSystem);
}

bool CDataManagerNonDbFuncMain::WriteOPNameSystem(CDataManagerInfo& info)
{
	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		ResetInfo(info, NODE_OPNAME_SYSTEM);
		OPNameSystem.PrepareForWrite();
		return WriteStringArray(OPNameSystem, info, m_nStatusOPNameSystem);
	}
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenPluUnit( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUUNIT );
	CFilenameUpdater FnUp ( SysFiles::PluUnit );
	return OpenStringArray( PluUnit, info, FnUp.GetFilenameToUse(), m_nStatusPluUnit, nType, FALSE );
}

bool CDataManagerNonDbFuncMain::ClosePluUnit( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUUNIT );
	return CloseStringArray( PluUnit, info, m_nStatusPluUnit );
}

bool CDataManagerNonDbFuncMain::WritePluUnit( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUUNIT );
	return WriteStringArray( PluUnit, info, m_nStatusPluUnit );
}

bool CDataManagerNonDbFuncMain::OpenPluUnitBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUUNIT );

	CFilenameUpdater FnUp ( SysFiles::PluUnit );
	if ( OpenStringArray( PluUnitBuffer, info, FnUp.GetFilenameToUse(), m_nStatusPluUnitBuffer, DB_READWRITE, FALSE ) == FALSE )
		return FALSE;

	PluUnit.CopyFrom( PluUnitBuffer );
	return TRUE;
}

bool CDataManagerNonDbFuncMain::ClosePluUnitBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUUNIT );
	PluUnit.CopyFrom( PluUnitBuffer );
	return CloseStringArray( PluUnitBuffer, info, m_nStatusPluUnitBuffer );
}

bool CDataManagerNonDbFuncMain::WritePluUnitBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PLUUNIT );
	
	CDefTextUnit temp( NODE_PLUUNIT );
	temp.CopyFrom( PluUnitBuffer );
	PluUnitBuffer.CopyFrom( PluUnit);

	bool bResult = WriteStringArray( PluUnitBuffer, info, m_nStatusPluUnitBuffer );

	if ( bResult == FALSE )
		PluUnitBuffer.CopyFrom( temp );

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenSalesHistoryCustomer( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_CUSTOMER );
	CFilenameUpdater FnUp ( SysFiles::CustomerHistory );
	return OpenCSVArray( SalesHistoryCustomer, info, FnUp.GetFilenameToUse(), m_nStatusSalesHistoryCustomer, nType, FALSE );
}

bool CDataManagerNonDbFuncMain::CloseSalesHistoryCustomer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_CUSTOMER );
	return CloseCSVArray( SalesHistoryCustomer, info, m_nStatusSalesHistoryCustomer );
}

bool CDataManagerNonDbFuncMain::WriteSalesHistoryCustomer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_CUSTOMER );
	return WriteCSVArray( SalesHistoryCustomer, info, m_nStatusSalesHistoryCustomer );
}

bool CDataManagerNonDbFuncMain::OpenSalesHistoryCustomerBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_CUSTOMER );

	CFilenameUpdater FnUp ( SysFiles::CustomerHistory );
	if ( OpenCSVArray( SalesHistoryCustomerBuffer, info, FnUp.GetFilenameToUse(), m_nStatusSalesHistoryCustomerBuffer, DB_READWRITE, FALSE ) == FALSE )
		return FALSE;

	SalesHistoryCustomer.CopyFrom( SalesHistoryCustomerBuffer );
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseSalesHistoryCustomerBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_CUSTOMER );
	SalesHistoryCustomer.CopyFrom( SalesHistoryCustomerBuffer );
	return CloseCSVArray( SalesHistoryCustomerBuffer, info, m_nStatusSalesHistoryCustomerBuffer );
}

bool CDataManagerNonDbFuncMain::WriteSalesHistoryCustomerBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_HISTORY_CUSTOMER );
	
	CSalesHistoryCSVArrayCustomer temp;
	temp.CopyFrom( SalesHistoryCustomerBuffer );
	SalesHistoryCustomerBuffer.CopyFrom( SalesHistoryCustomer);

	bool bResult = WriteCSVArray( SalesHistoryCustomerBuffer, info, m_nStatusSalesHistoryCustomerBuffer );

	if ( bResult == FALSE )
		SalesHistoryCustomerBuffer.CopyFrom( temp );

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenServerPresetDatabase( int nDbIdx, int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_SVRPRESET_DATABASE );
		CFilenameUpdater FnUp ( SysFiles::ServerPreset, nDbIdx, -1, FNUP_FAMILY_DATABASE );
		return OpenStringArray( ServerPresetDatabase, info, FnUp.GetFilenameToUse(), m_nStatusServerPresetDatabase, nType, FALSE );
	}
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseServerPresetDatabase( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SVRPRESET_DATABASE );
	return CloseStringArray( ServerPresetDatabase, info, m_nStatusServerPresetDatabase );
}

bool CDataManagerNonDbFuncMain::WriteServerPresetDatabase( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_SVRPRESET_DATABASE );
		ServerPresetDatabase.PrepareForWrite();	
		return WriteStringArray( ServerPresetDatabase, info, m_nStatusServerPresetDatabase );
	}
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenServerPresetLocation( int nLocIdx, int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_SVRPRESET_LOCATION );
		CFilenameUpdater FnUp ( SysFiles::ServerPreset, nLocIdx, -1, FNUP_FAMILY_LOCATION );
		return OpenStringArray( ServerPresetLocation, info, FnUp.GetFilenameToUse(), m_nStatusServerPresetLocation, nType, FALSE );
	}
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseServerPresetLocation( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SVRPRESET_LOCATION );
	return CloseStringArray( ServerPresetLocation, info, m_nStatusServerPresetLocation );
}

bool CDataManagerNonDbFuncMain::WriteServerPresetLocation( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_SVRPRESET_LOCATION );
		ServerPresetLocation.PrepareForWrite();	
		return WriteStringArray( ServerPresetLocation, info, m_nStatusServerPresetLocation );
	}
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenServerPresetSystem( int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_SVRPRESET_SYSTEM );
		CFilenameUpdater FnUp ( SysFiles::ServerPreset );
		return OpenStringArray( ServerPresetSystem, info, FnUp.GetFilenameToUse(), m_nStatusServerPresetSystem, nType, FALSE );
	}
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseServerPresetSystem( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SVRPRESET_SYSTEM );
	return CloseStringArray( ServerPresetSystem, info, m_nStatusServerPresetSystem );
}

bool CDataManagerNonDbFuncMain::WriteServerPresetSystem( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_SVRPRESET_SYSTEM );
		ServerPresetSystem.PrepareForWrite();	
		return WriteStringArray( ServerPresetSystem, info, m_nStatusServerPresetSystem );
	}
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenServerPresetTerminal( int nLocIdx, int nTNo, int nType, CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		int nNetWkIdx = 0;
		if ( dbNetwork.FindNetworkByNumber( dbLocation.GetNetworkNo( nLocIdx ), nNetWkIdx ) == FALSE )
			nNetWkIdx = 0;

		ResetInfo( info, NODE_SVRPRESET_TERMINAL );
		CFilenameUpdater FnUp ( SysFiles::ServerPreset, nNetWkIdx, nTNo, FNUP_FAMILY_TERM );
		return OpenStringArray( ServerPresetTerminal, info, FnUp.GetFilenameToUse(), m_nStatusServerPresetTerminal, nType, FALSE );
	}
	return TRUE;
}

bool CDataManagerNonDbFuncMain::CloseServerPresetTerminal( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SVRPRESET_TERMINAL );
	return CloseStringArray( ServerPresetTerminal, info, m_nStatusServerPresetTerminal );
}

bool CDataManagerNonDbFuncMain::WriteServerPresetTerminal( CDataManagerInfo& info )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		ResetInfo( info, NODE_SVRPRESET_TERMINAL );
		ServerPresetTerminal.PrepareForWrite();	
		return WriteStringArray( ServerPresetTerminal, info, m_nStatusServerPresetTerminal );
	}
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenServerRestriction( int nType, int nLocIdx, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SERVER_RESTRICT );
	CString strFilename = GetFilePathServerRestriction( nLocIdx );
	OpenCSVArray( ServerRestriction, info, strFilename, m_nStatusServerRestriction, nType, TRUE );
	ServerRestriction.SetCurrentLocIdx( nLocIdx );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ); 
}

bool CDataManagerNonDbFuncMain::CloseServerRestriction( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SERVER_RESTRICT );
	ServerRestriction.SetCurrentLocIdx( -1 );
	return CDataManagerBase::CloseCSVArray( ServerRestriction, info, m_nStatusServerRestriction );
}

bool CDataManagerNonDbFuncMain::WriteServerRestriction( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_SERVER_RESTRICT );
	return CDataManagerBase::WriteCSVArray( ServerRestriction, info, m_nStatusServerRestriction );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CDataManagerNonDbFuncMain::OpenTableAreas( int nType, int nLocIdx, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TABLE_AREA );
	CString strFilename = GetFilePathTableArea( nLocIdx );
	OpenCSVArray( TableAreas, info, strFilename, m_nStatusTableAreas, nType, TRUE );
	return ( info.m_nErrorType == DATAMANAGER_ERROR_NONE ); 
}

bool CDataManagerNonDbFuncMain::CloseTableAreas( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TABLE_AREA );
	return CDataManagerBase::CloseCSVArray( TableAreas, info, m_nStatusTableAreas );
}

bool CDataManagerNonDbFuncMain::WriteTableAreas( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_TABLE_AREA );
	return CDataManagerBase::WriteCSVArray( TableAreas, info, m_nStatusTableAreas );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

const char* CDataManagerNonDbFuncMain::GetFilePathServerRestriction( int nLocIdx )
{
	CFilenameUpdater FnUp( SysFiles::ServerRestriction, nLocIdx );
	m_strFilePathMain = FnUp.GetFilenameToUse();
	return m_strFilePathMain;
}

/**********************************************************************/

const char* CDataManagerNonDbFuncMain::GetFilePathTableArea( int nLocIdx )
{
	CFilenameUpdater FnUp( SysFiles::TableAreas, nLocIdx );
	m_strFilePathMain = FnUp.GetFilenameToUse();
	return m_strFilePathMain;
}

/**********************************************************************/

void CDataManagerNonDbFuncMain::CloseFilesMain( CWordArray& arrayFiles )
{
	CDataManagerInfo info;
	for ( int n = 0; n < arrayFiles.GetSize(); n++ )
	{
		switch( arrayFiles.GetAt(n) )
		{
		case NODE_ALLERGEN:				CloseAllergen( info );					break;
		case NODE_CCMACRO_SYSTEM:		CloseCCMacroSystem( info );				break;
		case NODE_CCMACRO_DATABASE:		CloseCCMacroDatabase( info );			break;
		case NODE_CCMACRO_LOCATION:		CloseCCMacroLocation( info );			break;
		case NODE_CCREASON_SYSTEM:		CloseCCReasonSystem(info);				break;
		case NODE_CCREASON_DATABASE:	CloseCCReasonDatabase(info);			break;
		case NODE_CCREASON_LOCATION:	CloseCCReasonLocation(info);			break;
		case NODE_CCMACRO_KEYBOARD:		CloseCCMacroKeyboard( info );			break;
		case NODE_SERVER_RESTRICT:		CloseServerRestriction( info );			break;
		case NODE_FUNCTION_SYSTEM:		CloseFunctionSystem( info );			break;
		case NODE_FUNCTION_DATABASE:	CloseFunctionDatabase( info );			break;
		case NODE_FUNCTION_LOCATION:	CloseFunctionLocation( info );			break;
		case NODE_TABLE_AREA:			CloseTableAreas( info );				break;
		}
	}
}

/**********************************************************************/
