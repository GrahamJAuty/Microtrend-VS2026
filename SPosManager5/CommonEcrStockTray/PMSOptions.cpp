/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "DateRangeHelpers.h"
#include "MaxLengths.h"
#include "PriceHelpers.h"
 /**********************************************************************/
#include "PMSOptions.h"
/**********************************************************************/

CPMSOptions::CPMSOptions()
{
	//DISPLAY TAB
	m_ColourVacantBack.Create		( "ColourVacantBack", 0x0, 0xFFFFFF, 0xB4CECE );
	m_ColourVacantText.Create		( "ColourVacantText", 0x0, 0xFFFFFF, 0x0 );
	m_ColourPendingBack.Create		( "ColourPendingBack", 0x0, 0xFFFFFF, 0xC03030 );
	m_ColourPendingText.Create		( "ColourPendingText", 0x0, 0xFFFFFF, 0xFFFFFF );
	m_ColourActiveBack.Create		( "ColourActiveBack", 0x0, 0xFFFFFF, 0x32A432 );
	m_ColourActiveText.Create		( "ColourActiveText", 0x0, 0xFFFFFF, 0x0 );
	m_ColourCompleteBack.Create		( "ColourCompleteBack", 0x0, 0xFFFFFF, 0x2000C0 );
	m_ColourCompleteText.Create		( "ColourCompleteText", 0x0, 0xFFFFFF, 0xFFFFFF );
	m_ColourClosedBack.Create		( "ColourClosedBack", 0x0, 0xFFFFFF, 0x404040 );
	m_ColourClosedText.Create		( "ColourClosedText", 0x0, 0xFFFFFF, 0xFFFFFF );
	m_ColourSelectedBack.Create		( "ColourSelectedBack", 0x0, 0xFFFFFF, 0x20E0E0 );
	m_ColourSelectedText.Create		( "ColourSelectedText", 0x0, 0xFFFFFF, 0x0 );
	m_ColourHLightBack.Create		( "ColourHighlightedBack", 0x0, 0xFFFFFF, 0x2000C0 );
	m_ColourHLightText.Create		( "ColourHighlightedText", 0x0, 0xFFFFFF, 0xFFFFFF );
	m_ColourUnHLightBack.Create		( "ColourUnhighlightedBack", 0x0, 0xFFFFFF, 0x606060 );
	m_ColourUnHLightText.Create		( "ColourUnhighlightedText", 0x0, 0xFFFFFF, 0xFFFFFF );

	//ADDRESS TAB
	m_HotelName.Create				( "HotelName", MAX_LENGTH_ADDRESS_NAME, "" );
	m_HotelAddress.Create			( "HotelAddress", MAX_LENGTH_ADDRESS_STREET, "", 3 );
	m_HotelPostcode.Create			( "HotelPostcode", MAX_LENGTH_ADDRESS_POSTCODE, "" );
	m_HotelPhone.Create				( "HotelPhone", MAX_LENGTH_ADDRESS_PHONE, "" );
	m_HotelFax.Create				( "HotelFax", MAX_LENGTH_ADDRESS_FAX, "" );
	m_HotelEmail.Create				( "HotelEmail",MAX_LENGTH_ADDRESS_EMAIL, "" );
	m_HotelTaxNo.Create				( "HotelVatNo", MAX_LENGTH_TAXNO, "" );

	//SALES TAB
	m_QuickSaleLegacy.Create		( "QuickSaleEnable", FALSE );
	m_QuickSaleEnable.Create		( "QuickSaleEnableNew", 0, 2, 0, 10 );
	m_QuickSalePlu.Create			( "QuickSalePlu", 0, 99999999999999, 0, 10 );
	m_QuickSaleType.Create			( "QuickSaleType", 0, 3, 0, 10 );

	//PAYMENT TAB
	m_PaymentType.Create			( "PaymentType", 0, 50, 0, 10 );

	//DINNER TAB
	m_DinnerDepartments.Create		( "DinnerDepartments", 1000, "" );
	m_DinnerAllowance.Create		( "DinnerAllowance", 0.0, 100.0, 0.0 );

	//INVOICES TAB
	m_InvoiceDefaultType.Create		( "InvoiceDefaultType", 0, 2, 0 );
	m_InvoiceShowAddress.Create		( "InvoiceShowAddress", TRUE );
	m_InvoiceHeaderLines.Create		( "InvoiceHeaderLines", 0, 20, 0 );
	m_InvoiceHideRef.Create			( "InvoiceHideReference", FALSE );
	m_InvoiceHideNumber.Create		( "InvoiceHideNumber", FALSE );
	m_InvoiceHideInterim.Create		( "InvoiceHideInterim", FALSE );
	m_InvoiceShowNote.Create		( "InvoiceShowNote", FALSE );
	m_InvoiceHideStay.Create		( "InvoiceHideDatesOfStay", FALSE );

	//EPOS REPORTS TAB
	m_ReportsEposHour.Create			( "EposReportHour", 0, 23, 0 );
	m_ReportsEposMinute.Create			( "EposReportMinute", 0, 59, 0 );
	m_ReportsFixedTime.Create			( "FixedTimeReportEnable", TRUE );
	m_ReportsAccomReportHour.Create		( "AccomReportHour", 0, 23, 0 );
	m_ReportsAccomReportMinute.Create	( "AccomReportMinute", 0, 59, 0 );
	m_ReportsAccomChargeHour.Create		( "AccomChargeHour", 0, 23, 0 );
	m_ReportsAccomChargeMinute.Create	( "AccomChargeMinute", 0, 59, 0 );
	m_ReportsAccomNextDay.Create		( "AccomReportNextDay", FALSE );
	m_ReportsBreakfastVal.Create		( "EposReportBreakfastValue", 0.0, 25.0, 0.0 );
	m_ReportsPluRoom.Create				( "EposReportRoomOnlyPlu", 0, 99999999999999, 0 );
	m_ReportsPluBB.Create				( "EposReportRoomBBPlu", 0, 99999999999999, 0 );
	m_ReportsPluDBB.Create				( "EposReportRoomDBBPlu", 0, 99999999999999, 0 );
	m_ReportsPluExtraBed.Create			( "EposReportExtraBedPlu", 0, 99999999999999, 0 );
	m_ReportsPluExtraCot.Create			( "EposReportExtraCotPlu", 0, 99999999999999, 0 );
	m_ReportsPluBreakfast.Create		( "EposReportBreakfastPlu", 0, 99999999999999, 0 );
	m_ReportsPluDinner.Create			( "EposReportDinnerPlu", 0, 99999999999999, 0 );

	//EPOS LINK TAB
	m_EposLinkRoom.Create			( "EposLinkRoomPath", 200, "" );
	m_EposLinkPost.Create			( "EposLinkPostPath", 200, "" );

	//MISC TAB
	m_MiscMultiAccount.Create		( "MiscEnableMultipleAccounts", FALSE );
	m_MiscTraining.Create			( "MiscEnableTrainingMode", FALSE );
	m_MiscEmailConfirm.Create		( "MiscEnableEmailConfirmation", FALSE );
	m_MiscEmailShowNote.Create		( "MiscEmailShowNote", FALSE );
	m_MiscFreeRoom.Create			( "MiscEnableFreeRoom", FALSE );
	m_MiscDefRoomRate.Create		( "MiscDefaultRoomRateType", 0, 2, 0 );
	m_MiscStockpoint.Create			( "MiscManualSalesStockpoint", 0, 999, 0 );
	m_MiscEnableRoom.Create			( "MiscEnableRateRoom", TRUE );
	m_MiscEnableBB.Create			( "MiscEnableRateBB", TRUE );
	m_MiscEnableDBB.Create			( "MiscEnableRateDBB", TRUE );
	
	Reset();
	m_bFatalReadError = FALSE;

	m_bCheckoutFilterTick = FALSE;
	m_bInvoiceAsComplete = FALSE;
	m_olePMSTime = COleDateTime( 2010, 1, 1, 0, 0, 0 );
	m_nPMSDayNumber = 0;

	for ( int n = 0; n < 100; n++ )
		m_bDinnerDepartmentBuffer[n] = 0;

	m_strDinnerDepartmentBuffer = "";
}

/**********************************************************************/

void CPMSOptions::CopyFrom( CPMSOptions& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CPMSOptions::ReloadFromIniFile( CEnhancedIniFile& iniFile )
{
	//DISPLAY TAB
	iniFile.ReadInt( m_ColourVacantBack );
	iniFile.ReadInt( m_ColourVacantText );
	iniFile.ReadInt( m_ColourPendingBack );
	iniFile.ReadInt( m_ColourPendingText );
	iniFile.ReadInt( m_ColourActiveBack );
	iniFile.ReadInt( m_ColourActiveText );
	iniFile.ReadInt( m_ColourCompleteBack );
	iniFile.ReadInt( m_ColourCompleteText );
	iniFile.ReadInt( m_ColourClosedBack );
	iniFile.ReadInt( m_ColourClosedText );
	iniFile.ReadInt( m_ColourSelectedBack );
	iniFile.ReadInt( m_ColourSelectedText );
	iniFile.ReadInt( m_ColourHLightBack );
	iniFile.ReadInt( m_ColourHLightText );
	iniFile.ReadInt( m_ColourUnHLightBack );
	iniFile.ReadInt( m_ColourUnHLightText );

	//ADDRESS TAB
	iniFile.ReadString( m_HotelName );
	iniFile.ReadString( m_HotelAddress, 0 );
	iniFile.ReadString( m_HotelAddress, 1 );
	iniFile.ReadString( m_HotelAddress, 2 );
	iniFile.ReadString( m_HotelPostcode );
	iniFile.ReadString( m_HotelPhone );
	iniFile.ReadString( m_HotelFax );
	iniFile.ReadString( m_HotelEmail );
	iniFile.ReadString( m_HotelTaxNo );

	//SALES TAB
	iniFile.ReadBool( m_QuickSaleLegacy );
	for ( int n = 0; n < 10; n++ )
	{
		iniFile.ReadInt( m_QuickSaleEnable, n );
		iniFile.ReadInt64( m_QuickSalePlu, n );
		iniFile.ReadInt( m_QuickSaleType, n );

		if ( GetQuickSaleEnable(n) == 0 )
			SetQuickSaleEnable( n, GetQuickSaleLegacyFlag() ? 2 : 1 );
	}

	//PAYMENT TAB
	for ( int n = 0; n < 10; n++ )
		iniFile.ReadInt( m_PaymentType, n );
	
	//DINNER TAB
	iniFile.ReadString( m_DinnerDepartments );
	iniFile.ReadDouble( m_DinnerAllowance );

	//INVOICES TAB
	iniFile.ReadInt( m_InvoiceDefaultType );
	iniFile.ReadBool( m_InvoiceShowAddress );
	iniFile.ReadInt( m_InvoiceHeaderLines );
	iniFile.ReadBool( m_InvoiceHideRef );
	iniFile.ReadBool( m_InvoiceHideNumber );
	iniFile.ReadBool( m_InvoiceHideInterim );
	iniFile.ReadBool( m_InvoiceShowNote );
	iniFile.ReadBool( m_InvoiceHideStay );

	//EPOS REPORTS TAB
	iniFile.ReadInt( m_ReportsEposHour );
	iniFile.ReadInt( m_ReportsEposMinute );
	iniFile.ReadBool( m_ReportsFixedTime );
	iniFile.ReadInt( m_ReportsAccomReportHour );
	iniFile.ReadInt( m_ReportsAccomReportMinute );
	iniFile.ReadInt( m_ReportsAccomChargeHour );
	iniFile.ReadInt( m_ReportsAccomChargeMinute );
	iniFile.ReadBool( m_ReportsAccomNextDay );
	iniFile.ReadDouble( m_ReportsBreakfastVal );
	iniFile.ReadInt64( m_ReportsPluRoom );
	iniFile.ReadInt64( m_ReportsPluBB );
	iniFile.ReadInt64( m_ReportsPluDBB );
	iniFile.ReadInt64( m_ReportsPluExtraBed );
	iniFile.ReadInt64( m_ReportsPluExtraCot );
	iniFile.ReadInt64( m_ReportsPluBreakfast );
	iniFile.ReadInt64( m_ReportsPluDinner );

	//EPOS LINK TAB
	iniFile.ReadString( m_EposLinkRoom );
	iniFile.ReadString( m_EposLinkPost );

	//MISC TAB
	iniFile.ReadBool( m_MiscMultiAccount );
	iniFile.ReadBool( m_MiscTraining );
	iniFile.ReadBool( m_MiscEmailConfirm );
	iniFile.ReadBool( m_MiscEmailShowNote );
	iniFile.ReadBool( m_MiscFreeRoom );
	iniFile.ReadInt( m_MiscDefRoomRate );
	iniFile.ReadInt( m_MiscStockpoint );
	iniFile.ReadBool( m_MiscEnableRoom );
	iniFile.ReadBool( m_MiscEnableBB );
	iniFile.ReadBool( m_MiscEnableDBB );
}

/**********************************************************************/

void CPMSOptions::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	//DISPLAY TAB 
	iniFile.WriteInt( m_ColourVacantBack );
	iniFile.WriteInt( m_ColourVacantText );
	iniFile.WriteInt( m_ColourPendingBack );
	iniFile.WriteInt( m_ColourPendingText );
	iniFile.WriteInt( m_ColourActiveBack );
	iniFile.WriteInt( m_ColourActiveText );
	iniFile.WriteInt( m_ColourCompleteBack );
	iniFile.WriteInt( m_ColourCompleteText );
	iniFile.WriteInt( m_ColourClosedBack );
	iniFile.WriteInt( m_ColourClosedText );
	iniFile.WriteInt( m_ColourSelectedBack );
	iniFile.WriteInt( m_ColourSelectedText );
	iniFile.WriteInt( m_ColourHLightBack );
	iniFile.WriteInt( m_ColourHLightText );
	iniFile.WriteInt( m_ColourUnHLightBack );
	iniFile.WriteInt( m_ColourUnHLightText );

	//ADDRESS TAB
	iniFile.WriteString( m_HotelName );
	iniFile.WriteString( m_HotelAddress, 0 );
	iniFile.WriteString( m_HotelAddress, 1 );
	iniFile.WriteString( m_HotelAddress, 2 );
	iniFile.WriteString( m_HotelPostcode );
	iniFile.WriteString( m_HotelPhone );
	iniFile.WriteString( m_HotelFax );
	iniFile.WriteString( m_HotelEmail );
	iniFile.WriteString( m_HotelTaxNo );

	//SALES TAB
	for ( int n = 0; n < 10; n++ )
	{
		iniFile.WriteInt( m_QuickSaleEnable, n );
		iniFile.WriteInt64( m_QuickSalePlu, n );
		iniFile.WriteInt( m_QuickSaleType, n );
	}

	//PAYMENT TAB
	for ( int n = 0; n < 10; n++ )
		iniFile.WriteInt( m_PaymentType, n );
	
	//DINNER TAB
	iniFile.WriteString( m_DinnerDepartments );
	iniFile.WriteDouble( m_DinnerAllowance, 2 );
	
	//INVOICES TAB
	iniFile.WriteInt( m_InvoiceDefaultType );
	iniFile.WriteBool( m_InvoiceShowAddress );
	iniFile.WriteInt( m_InvoiceHeaderLines );
	iniFile.WriteBool( m_InvoiceHideRef );
	iniFile.WriteBool( m_InvoiceHideNumber );
	iniFile.WriteBool( m_InvoiceHideInterim );
	iniFile.WriteBool( m_InvoiceShowNote );
	iniFile.WriteBool( m_InvoiceHideStay );

	//EPOS REPORTS TAB
	iniFile.WriteInt( m_ReportsEposHour );
	iniFile.WriteInt( m_ReportsEposMinute );
	iniFile.WriteBool( m_ReportsFixedTime );
	iniFile.WriteInt( m_ReportsAccomReportHour );
	iniFile.WriteInt( m_ReportsAccomReportMinute );
	iniFile.WriteInt( m_ReportsAccomChargeHour );
	iniFile.WriteInt( m_ReportsAccomChargeMinute );
	iniFile.WriteBool( m_ReportsAccomNextDay );
	iniFile.WriteDouble( m_ReportsBreakfastVal, 2 );
	iniFile.WriteInt64( m_ReportsPluRoom );
	iniFile.WriteInt64( m_ReportsPluBB );
	iniFile.WriteInt64( m_ReportsPluDBB );
	iniFile.WriteInt64( m_ReportsPluExtraBed );
	iniFile.WriteInt64( m_ReportsPluExtraCot );
	iniFile.WriteInt64( m_ReportsPluBreakfast );
	iniFile.WriteInt64( m_ReportsPluDinner );

	//EPOS LINK TAB
	iniFile.WriteString( m_EposLinkRoom );
	iniFile.WriteString( m_EposLinkPost );

	//MISC TAB
	iniFile.WriteBool( m_MiscMultiAccount );
	iniFile.WriteBool( m_MiscTraining );
	iniFile.WriteBool( m_MiscEmailConfirm );
	iniFile.WriteBool( m_MiscEmailShowNote );
	iniFile.WriteBool( m_MiscFreeRoom );
	iniFile.WriteInt( m_MiscDefRoomRate );
	iniFile.WriteInt( m_MiscStockpoint );
	iniFile.WriteBool( m_MiscEnableRoom );
	iniFile.WriteBool( m_MiscEnableBB );
	iniFile.WriteBool( m_MiscEnableDBB );
}

/**********************************************************************/

void CPMSOptions::Reset()
{
	//DISPLAY TAB
	SetDefaultColours();

	//ADDDRESS TAB
	m_HotelName.Reset();
	m_HotelAddress.Reset(0);
	m_HotelAddress.Reset(1);
	m_HotelAddress.Reset(2);
	m_HotelPostcode.Reset();
	m_HotelPhone.Reset();
	m_HotelFax.Reset();
	m_HotelEmail.Reset();
	m_HotelTaxNo.Reset();
	
	//SALES TAB
	m_QuickSaleLegacy.Reset();
	for ( int n = 0; n < 10; n++ )
	{
		m_QuickSaleEnable.Reset(n);
		m_QuickSalePlu.Reset(n);
		m_QuickSaleType.Reset(n);
	}

	//PAYMENTS TAB
	for ( int n = 0; n < 10; n++ )
		m_PaymentType.Reset(n);

	//DINNER TAB
	m_DinnerDepartments.Reset();
	m_DinnerAllowance.Reset();
	
	//INVOICE TAB
	m_InvoiceDefaultType.Reset();
	m_InvoiceShowAddress.Reset();
	m_InvoiceHeaderLines.Reset();
	m_InvoiceHideRef.Reset();
	m_InvoiceHideNumber.Reset();
	m_InvoiceHideInterim.Reset();
	m_InvoiceShowNote.Reset();
	m_InvoiceHideStay.Reset();
	
	//EPOS REPORTS TAB
	m_ReportsEposHour.Reset();
	m_ReportsEposMinute.Reset();
	m_ReportsFixedTime.Reset();
	m_ReportsAccomReportHour.Reset();
	m_ReportsAccomReportMinute.Reset();
	m_ReportsAccomChargeHour.Reset();
	m_ReportsAccomChargeMinute.Reset();
	m_ReportsAccomNextDay.Reset();
	m_ReportsBreakfastVal.Reset();
	m_ReportsPluRoom.Reset();
	m_ReportsPluBB.Reset();
	m_ReportsPluDBB.Reset();
	m_ReportsPluExtraBed.Reset();
	m_ReportsPluExtraCot.Reset();
	m_ReportsPluBreakfast.Reset();
	m_ReportsPluDinner.Reset();

	//EPOS LINK TAB
	m_EposLinkRoom.Reset();
	m_EposLinkPost.Reset();

	//MISC TAB
	m_MiscTraining.Reset();
	m_MiscMultiAccount.Reset();
	m_MiscEmailConfirm.Reset();
	m_MiscEmailShowNote.Reset();
	m_MiscFreeRoom.Reset();
	m_MiscDefRoomRate.Reset();
	m_MiscStockpoint.Reset();
	m_MiscEnableRoom.Reset();
	m_MiscEnableBB.Reset();
	m_MiscEnableDBB.Reset();
}

/**********************************************************************/

void CPMSOptions::SetDefaultColours()
{
	m_ColourVacantBack.Reset();
	m_ColourVacantText.Reset();
	m_ColourPendingBack.Reset();
	m_ColourPendingText.Reset();
	m_ColourActiveBack.Reset();
	m_ColourActiveText.Reset();
	m_ColourCompleteBack.Reset();
	m_ColourCompleteText.Reset();
	m_ColourClosedBack.Reset();
	m_ColourClosedText.Reset();
	m_ColourSelectedBack.Reset();
	m_ColourSelectedText.Reset();
	m_ColourHLightBack.Reset();
	m_ColourHLightText.Reset();
	m_ColourUnHLightBack.Reset();
	m_ColourUnHLightText.Reset();
}

/**********************************************************************/

bool CPMSOptions::Read()
{
	::CreateSyssetSubdirectory( "SysFiles" );

	CFilenameUpdater FnUpOld( SysFiles::PMSOptionsOld );
	CString strFilenameOld = FnUpOld.GetFilenameToUse();
	
	CFilenameUpdater FnUpNew( SysFiles::PMSOptionsNew );
	CString strFilenameNew = FnUpNew.GetFilenameToUse();
	
	if ( ::FileExists( strFilenameNew ) == TRUE )
	{
		CEnhancedIniFile iniFile;
		if ( iniFile.Read( strFilenameNew ) == FALSE )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile(iniFile);
		return TRUE;
	}
	else
	{	
		CSSFile fileOptions;
		if ( fileOptions.Open( strFilenameOld, "rb" ) == FALSE )
		{
			if ( GetLastError() != ERROR_FILE_NOT_FOUND )
				m_bFatalReadError = TRUE;
			
			return FALSE;
		}
	
		CString strBuffer;
		if ( fileOptions.ReadString ( strBuffer ) == FALSE )
			return TRUE;

		LegacyReloadFromString( strBuffer );
		return TRUE;
	}
}

/**********************************************************************/

bool CPMSOptions::Write()
{
	bool bResult = FALSE;

	if ( DataManager.LockAllUsers() == TRUE )
	{
		::CreateSyssetSubdirectory( "SysFiles" );

		CEnhancedIniFile iniFile;
		PrepareIniFile( iniFile );
		
		CFilenameUpdater FnUp( SysFiles::PMSOptionsNew );
		bResult = iniFile.Write( FnUp.GetFilenameToUse() );

		DataManager.UnlockAllUsers();
	}
		
	return bResult;
}

/**********************************************************************/

bool CPMSOptions::GetMiscEnableRateRoom()
{
	if ( TRUE == m_MiscEnableRoom.GetValue() )
		return TRUE;

	if ( ( FALSE == m_MiscEnableBB.GetValue() ) && ( FALSE == m_MiscEnableDBB.GetValue() ) )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

int CPMSOptions::GetMiscDefaultRoomRateType()
{
	bool bAccept = FALSE;
	switch( m_MiscDefRoomRate.GetValue() )
	{
	case PMS_BOOKING_RATE_ROOM:		bAccept =  m_MiscEnableRoom.GetValue();	break;
	case PMS_BOOKING_RATE_BB:		bAccept =  m_MiscEnableBB.GetValue();	break;
	case PMS_BOOKING_RATE_DBB:		bAccept =  m_MiscEnableDBB.GetValue();	break;
	}

	if ( TRUE == bAccept )
		return m_MiscDefRoomRate.GetValue();

	if ( TRUE == m_MiscEnableRoom.GetValue() )
		return PMS_BOOKING_RATE_ROOM;

	if ( TRUE == m_MiscEnableBB.GetValue() )
		return PMS_BOOKING_RATE_BB;

	if ( TRUE == m_MiscEnableDBB.GetValue() )
		return PMS_BOOKING_RATE_DBB;

	return PMS_BOOKING_RATE_ROOM;
}

/**********************************************************************/

void CPMSOptions::SetPMSTime()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	m_olePMSTime = COleDateTime( timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 0, 0, 0 );
	m_nPMSDayNumber = GetDayNumberSince2011( m_olePMSTime );

	int nAccomHour = PMSOptions.GetAccomChargeHour();
	int nAccomMinute = PMSOptions.GetAccomChargeMinute();

	if ( ( 0 == nAccomHour ) && ( 0 == nAccomMinute ) )
	{
		m_olePMSTimeAccom = m_olePMSTime;
		m_nPMSDayNumberAccom = m_nPMSDayNumber;
	}
	else
	{
		CDateRangeHelpers DateRangeHelpers;
		DateRangeHelpers.AddDaysToTime( timeNow, 1 );
		timeNow -= COleDateTimeSpan( 0, PMSOptions.GetAccomChargeHour(), PMSOptions.GetAccomChargeMinute(), 0 );
		m_olePMSTimeAccom = COleDateTime( timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 0, 0, 0 );
		m_nPMSDayNumberAccom = GetDayNumberSince2011( m_olePMSTimeAccom );
	}
}

/**********************************************************************/

void CPMSOptions::CreatePMSFolders()
{
	CStringArray arrayFolders;
	arrayFolders.Add( GetPMSFolder() );
	arrayFolders.Add( GetPMSFolderSysFiles() );
	arrayFolders.Add( GetPMSFolderSales() );
	arrayFolders.Add( GetPMSFolderDownload() );
	arrayFolders.Add( GetPMSFolderPending() );
	arrayFolders.Add( GetPMSFolderReports() );
	arrayFolders.Add( GetPMSFolderBooking() );
	arrayFolders.Add( GetPMSFolderCustomer() );
	
	for ( int n = 0; n < arrayFolders.GetSize(); n++ )
	{
		CString strDir = arrayFolders.GetAt(n);
		if ( ::ExistSubdirectory ( strDir ) == FALSE )
			::MakeSubdirectory ( strDir );
	}
}

/**********************************************************************/

const char* CPMSOptions::GetPMSFolder()
{
	m_strPMSFolder = "PMS2";
	GetDataProgramPath( m_strPMSFolder );
	return m_strPMSFolder;
}

/**********************************************************************/

const char* CPMSOptions::GetPMSFolderSysFiles()
{
	m_strPMSFolderSysFiles = "PMS2\\SysFiles";
	GetDataProgramPath( m_strPMSFolderSysFiles );
	return m_strPMSFolderSysFiles;
}

/**********************************************************************/

const char* CPMSOptions::GetPMSFolderSales()
{
	m_strPMSFolderSales = "PMS2\\Sales";
	GetDataProgramPath( m_strPMSFolderSales );
	return m_strPMSFolderSales;
}

/**********************************************************************/

const char* CPMSOptions::GetPMSFolderDownload()
{
	m_strPMSFolderDownload = "PMS2\\Download";
	GetDataProgramPath( m_strPMSFolderDownload );
	return m_strPMSFolderDownload;
}

/**********************************************************************/

const char* CPMSOptions::GetPMSFolderPending()
{
	m_strPMSFolderPending = "PMS2\\Pending";
	GetDataProgramPath( m_strPMSFolderPending );
	return m_strPMSFolderPending;
}

/**********************************************************************/

const char* CPMSOptions::GetPMSFolderReports()
{
	m_strPMSFolderReports = "PMS2\\Reports2";
	GetDataProgramPath( m_strPMSFolderReports );
	return m_strPMSFolderReports;
}

/**********************************************************************/

const char* CPMSOptions::GetPMSFolderBooking()
{
	m_strPMSFolderBooking = "PMS2\\Booking";
	GetDataProgramPath( m_strPMSFolderBooking );
	return m_strPMSFolderBooking;
}

/**********************************************************************/

const char* CPMSOptions::GetPMSFolderCustomer()
{
	m_strPMSFolderCustomer = "PMS2\\Customer";
	GetDataProgramPath( m_strPMSFolderCustomer );
	return m_strPMSFolderCustomer;
}

/**********************************************************************/

void CPMSOptions::BuildValidatedPaymentTypeArray()
{
	m_arrayPaymentTypes.RemoveAll();

	for ( int n = 0; n < 10; n++ )
	{
		int nPaymentNo = GetPaymentType(n);

		if ( nPaymentNo != 0 )
		{
			int nPaymentIdx;
			if ( DataManager.Payment.FindPaymentByNumber( nPaymentNo, nPaymentIdx ) == TRUE )
			{
				CPaymentCSVRecord Payment;
				DataManager.Payment.GetAt( nPaymentIdx, Payment );

				switch( Payment.GetPaymentType() )
				{
				case PAYMENT_TYPE_ROOM_AUTO:
				case PAYMENT_TYPE_ROOM_MANUAL:
					break;

				default:
					{
						CCSV csv;
						csv.Add( nPaymentNo );
						csv.Add( Payment.GetDisplayName() );
						m_arrayPaymentTypes.Add( csv.GetLine() );
					}
					break;
				}
			}
		}
	}
}

/**********************************************************************/

int CPMSOptions::GetValidatedPaymentTypeArraySize()
{
	return m_arrayPaymentTypes.GetSize();
}

/**********************************************************************/

const char* CPMSOptions::GetValidatedPaymentTypeString( int n )
{
	if ( ( n >= 0 ) && ( n < m_arrayPaymentTypes.GetSize() ) )
		m_strPaymentType = m_arrayPaymentTypes.GetAt(n);
	else
		m_strPaymentType = "";

	return m_strPaymentType;
}

/**********************************************************************/

void CPMSOptions::NoPaymentTypeError()
{
	CString strMsg = "";
	strMsg += "You must enable at least one payment type for use with the\n";
	strMsg += "guest account system before you can access this option.";
	Prompter.Error( strMsg );
}

/**********************************************************************/

void CPMSOptions::LoadDinnerDepartmentBuffer( CPMSBookingCSVRecord& BookingRecord )
{
	if ( m_strDinnerDepartmentBuffer != BookingRecord.GetDinnerDepartments() )
	{
		m_strDinnerDepartmentBuffer = BookingRecord.GetDinnerDepartments();

		for ( int n = 0; n < 100; n++ )
			m_bDinnerDepartmentBuffer[n] = FALSE;

		CCSV csv( m_strDinnerDepartmentBuffer );

		for ( int n = 0; ( n < csv.GetSize() ) && ( n < 99 ); n++ )
		{
			int nDeptNo = csv.GetInt(n);
			if ( ( nDeptNo >= 1 ) && ( nDeptNo <= 99 ) )
				m_bDinnerDepartmentBuffer[nDeptNo] = TRUE;
		}
	}
}

/**********************************************************************/

bool CPMSOptions::CheckDinnerDepartmentBuffer( int nDeptNo )
{
	if ( ( nDeptNo >= 1 ) && ( nDeptNo <= 99 ) )
		return m_bDinnerDepartmentBuffer[nDeptNo];
	else
		return FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
//LEGACY FUNCTION FOR READING OLD OPTIONS FILE FORMAT
/**********************************************************************/

void CPMSOptions::LegacyReloadFromString( CString& strBuffer )
{
	CCSV csv ( strBuffer );

	switch( csv.GetInt(0) )
	{
	case 1:
		{
			m_EposLinkRoom.Reset( csv.GetString(1) );
			m_EposLinkPost.Reset( csv.GetString(2) );
			m_HotelName.Reset( csv.GetString(3) );
			m_HotelAddress.Reset( 0, csv.GetString(4) );
			m_HotelAddress.Reset( 1, csv.GetString(5) );
			m_HotelAddress.Reset( 2, csv.GetString(6) );
			m_HotelPostcode.Reset( csv.GetString(7) );
			m_HotelPhone.Reset( csv.GetString(8) );
			m_HotelFax.Reset( csv.GetString(9) );
			m_HotelEmail.Reset( csv.GetString(10) );
			m_HotelTaxNo.Reset( csv.GetString(11) );
			m_InvoiceDefaultType.Reset( csv.GetInt(12) );
			m_InvoiceShowAddress.Reset( csv.GetBool(13) );
			m_InvoiceHeaderLines.Reset( csv.GetInt(14) );
			m_ReportsEposHour.Reset( csv.GetInt(15) );
			m_ReportsEposMinute.Reset( csv.GetInt(16) );
			m_ColourVacantBack.Reset( csv.GetInt(17) );
			m_ColourVacantText.Reset( csv.GetInt(18) );
			m_ColourPendingBack.Reset( csv.GetInt(19) );
			m_ColourPendingText.Reset( csv.GetInt(20) );
			m_ColourActiveBack.Reset( csv.GetInt(21) );
			m_ColourActiveText.Reset( csv.GetInt(22) );
			m_ColourCompleteBack.Reset( csv.GetInt(23) );
			m_ColourCompleteText.Reset( csv.GetInt(24) );
			m_ColourClosedBack.Reset( csv.GetInt(25));
			m_ColourClosedText.Reset( csv.GetInt(26) );
			m_ColourSelectedBack.Reset( csv.GetInt(27) );
			m_ColourSelectedText.Reset( csv.GetInt(28) );
			m_QuickSaleEnable.Reset( csv.GetBool(29) );
		
			int nOffset = 30;

			for ( int n = 0; n < 8; n++ )
			{
				m_QuickSalePlu.Reset( n, csv.GetInt64( nOffset++ ) );
				m_QuickSaleType.Reset( n, csv.GetInt( nOffset++ ) );
			}

			for ( int n = 0; n < 6; n++ )
				m_PaymentType.Reset( n, csv.GetInt( nOffset++ ) );

			nOffset++;	//SHIP RETAIN SELECTION FLAG
			m_MiscMultiAccount.Reset( csv.GetBool( nOffset++ ) );
			m_MiscEmailConfirm.Reset( csv.GetBool( nOffset++ ) );
			m_InvoiceHideRef.Reset( csv.GetBool( nOffset++ ) );
			m_MiscTraining.Reset( csv.GetBool( nOffset++ ) );
			m_ReportsPluRoom.Reset( csv.GetInt64( nOffset++ ) );
			m_InvoiceShowNote.Reset( csv.GetBool( nOffset++ ) );
			m_InvoiceHideNumber.Reset( csv.GetBool( nOffset++ ) );
			m_MiscStockpoint.Reset( csv.GetInt( nOffset++ ) );
			m_ReportsPluBB.Reset( csv.GetInt64( nOffset++ ) );
			m_ReportsPluDBB.Reset( csv.GetInt64( nOffset++ ) );
			m_DinnerAllowance.Reset( csv.GetDouble( nOffset++ ) );
			m_DinnerDepartments.Reset( csv.GetString( nOffset++ ) );
			m_ReportsPluDinner.Reset( csv.GetInt64( nOffset++ ) );
			m_ReportsFixedTime.SetValue( TRUE );
		}
		break;
	}
}
	
/**********************************************************************/

bool CPMSOptions::AreAnyQuickSalesEnabled()
{
	for ( int n = 0; n < 10; n++ )
		if ( GetQuickSaleEnable(n) == 2 )
			return TRUE;

	return FALSE;
}

/**********************************************************************/
#endif
/**********************************************************************/
