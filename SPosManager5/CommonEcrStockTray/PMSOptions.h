#pragma once
/**********************************************************************/
#include "EnhancedIniFile.h"
#include "PMSBookingCSVArray.h"
/**********************************************************************/
#define QUICK_ACCESS_PLU_SALE 0
#define QUICK_ACCESS_PLU_SALE_QTY 1
#define QUICK_ACCESS_PLU_ALLOW 2
#define QUICK_ACCESS_PLU_ALLOW_QTY 3
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

class CPMSOptions 
{
public:
	CPMSOptions();
	void Reset();

	bool Read();
	bool Write();
	
	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	void CopyFrom( CPMSOptions& source );

private:
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );
	void LegacyReloadFromString( CString& strBuffer );
	void PrepareIniFile( CEnhancedIniFile& iniFile );

public:
	//FIX TIME FOR ONGOING CHECKIN OR CHECKOUT
	void SetPMSTime();
	COleDateTime GetPMSTime(){ return m_olePMSTime; }
	int GetPMSDayNumber(){ return m_nPMSDayNumber; }
	COleDateTime GetPMSTimeAccom(){ return m_olePMSTimeAccom; }
	int GetPMSDayNumberAccom(){ return m_nPMSDayNumberAccom; }

	//DISPLAY TAB
	int GetColourVacantBack()		{ return m_ColourVacantBack.GetValue(); }
	int GetColourVacantText()		{ return m_ColourVacantText.GetValue(); }
	int GetColourPendingBack()		{ return m_ColourPendingBack.GetValue(); }
	int GetColourPendingText()		{ return m_ColourPendingText.GetValue(); }
	int GetColourActiveBack()		{ return m_ColourActiveBack.GetValue(); }
	int GetColourActiveText()		{ return m_ColourActiveText.GetValue(); }
	int GetColourCompleteBack()		{ return m_ColourCompleteBack.GetValue(); }
	int GetColourCompleteText()		{ return m_ColourCompleteText.GetValue(); }
	int GetColourClosedBack()		{ return m_ColourClosedBack.GetValue(); }
	int GetColourClosedText()		{ return m_ColourClosedText.GetValue(); }
	int GetColourSelectedBack()		{ return m_ColourSelectedBack.GetValue(); }
	int GetColourSelectedText()		{ return m_ColourSelectedText.GetValue(); }
	int GetColourHighlightedBack()	{ return m_ColourHLightBack.GetValue(); }
	int GetColourHighlightedText()	{ return m_ColourHLightText.GetValue(); }
	int GetColourUnhighlightedBack(){ return m_ColourUnHLightBack.GetValue(); }
	int GetColourUnhighlightedText(){ return m_ColourUnHLightText.GetValue(); }

	//ADDRESS TAB
	const char* GetHotelName()		{ return m_HotelName.GetValue(); }
	const char* GetHotelAddress1()	{ return m_HotelAddress.GetValue(0); }
	const char* GetHotelAddress2()	{ return m_HotelAddress.GetValue(1); }
	const char* GetHotelAddress3()	{ return m_HotelAddress.GetValue(2); }
	const char* GetHotelPostcode()	{ return m_HotelPostcode.GetValue(); }
	const char* GetHotelPhone()		{ return m_HotelPhone.GetValue(); }
	const char* GetHotelFax()		{ return m_HotelFax.GetValue(); }
	const char* GetHotelEmail()		{ return m_HotelEmail.GetValue(); }
	const char* GetHotelTaxNo()		{ return m_HotelTaxNo.GetValue(); }

	//SALES TAB
	bool GetQuickSaleLegacyFlag()		{ return m_QuickSaleLegacy.GetValue(); }
	int GetQuickSaleEnable( int n )		{ return m_QuickSaleEnable.GetValue(n); }
	__int64 GetQuickSalePlu( int n )	{ return m_QuickSalePlu.GetValue(n); }
	int GetQuickSaleType( int n )		{ return m_QuickSaleType.GetValue(n); }

	//PAYMENT TAB
	int GetPaymentType( int n )		{ return m_PaymentType.GetValue(n); }

	//DINNER TAB
	const char* GetDinnerDepartments()	{ return m_DinnerDepartments.GetValue(); }
	double GetDinnerAllowance()			{ return m_DinnerAllowance.GetValue(); }
	
	//INVOICE TAB
	int GetInvoiceDefaultType()			{ return m_InvoiceDefaultType.GetValue(); }
	bool GetInvoiceShowAddressFlag()	{ return m_InvoiceShowAddress.GetValue(); }
	int GetInvoiceHeaderLines()			{ return m_InvoiceHeaderLines.GetValue(); }
	bool GetInvoiceHideReferenceFlag()	{ return m_InvoiceHideRef.GetValue(); }
	bool GetInvoiceHideNumberFlag()		{ return m_InvoiceHideNumber.GetValue(); }
	bool GetInvoiceHideInterimFlag()	{ return m_InvoiceHideInterim.GetValue(); }
	bool GetInvoiceShowNoteFlag()		{ return m_InvoiceShowNote.GetValue(); }
	bool GetInvoiceHideDatesOfStayFlag(){ return m_InvoiceHideStay.GetValue(); }

	//EPOS REPORTS TAB
	int GetEposReportHour()				{ return m_ReportsEposHour.GetValue(); }
	int GetEposReportMinute()			{ return m_ReportsEposMinute.GetValue(); }
	bool GetFixedTimeReportEnableFlag()	{ return m_ReportsFixedTime.GetValue(); }
	int GetAccomReportHour()			{ return m_ReportsAccomReportHour.GetValue(); }
	int GetAccomReportMinute()			{ return m_ReportsAccomReportMinute.GetValue(); }
	int GetAccomChargeHour()			{ return m_ReportsAccomChargeHour.GetValue(); }
	int GetAccomChargeMinute()			{ return m_ReportsAccomChargeMinute.GetValue(); }
	bool GetAccomReportNextDayFlag()	{ return m_ReportsAccomNextDay.GetValue(); }
	double GetEposReportBreakfastValue(){ return m_ReportsBreakfastVal.GetValue(); }
	__int64 GetEposReportRoomOnlyPlu()	{ return m_ReportsPluRoom.GetValue(); }
	__int64 GetEposReportRoomBBPlu()	{ return m_ReportsPluBB.GetValue(); }
	__int64 GetEposReportRoomDBBPlu()	{ return m_ReportsPluDBB.GetValue(); }
	__int64 GetEposReportExtraBedPlu()	{ return m_ReportsPluExtraBed.GetValue(); }
	__int64 GetEposReportExtraCotPlu()	{ return m_ReportsPluExtraCot.GetValue(); }
	__int64 GetEposReportBreakfastPlu()	{ return m_ReportsPluBreakfast.GetValue(); }
	__int64 GetEposReportDinnerPlu()	{ return m_ReportsPluDinner.GetValue(); }
	
	//EPOS LINK TAB
	const char* GetEposLinkRoomPath()	{ return m_EposLinkRoom.GetValue(); }
	const char* GetEposLinkPostPath()	{ return m_EposLinkPost.GetValue(); }

	//MISC TAB
	bool GetMiscRetainSelectionFlag()		{ return TRUE; }
	bool GetMiscEnableMultipleAccounts()	{ return m_MiscMultiAccount.GetValue(); }
	bool GetMiscEnableTrainingMode()		{ return m_MiscTraining.GetValue(); }
	bool GetMiscEnableFreeRoom()			{ return m_MiscFreeRoom.GetValue(); }
	bool GetMiscEnableEmailConfirmation()	{ return m_MiscEmailConfirm.GetValue(); }
	bool GetMiscEmailShowNote()				{ return m_MiscEmailShowNote.GetValue(); }
	bool GetMiscEnableRateRoom();			
	bool GetMiscEnableRateBB()				{ return m_MiscEnableBB.GetValue(); }
	bool GetMiscEnableRateDBB()				{ return m_MiscEnableDBB.GetValue(); }
	int GetMiscDefaultRoomRateType();		
	int GetMiscManualSalesStockpoint()		{ return m_MiscStockpoint.GetValue(); }

	//DISPLAY TAB
	void SetDefaultColours();
	void SetColourVacantBack( int n )			{ m_ColourVacantBack.SetValue(n); }
	void SetColourVacantText( int n )			{ m_ColourVacantText.SetValue(n); }
	void SetColourPendingBack( int n )			{ m_ColourPendingBack.SetValue(n); }	
	void SetColourPendingText( int n )			{ m_ColourPendingText.SetValue(n); }
	void SetColourActiveBack( int n )			{ m_ColourActiveBack.SetValue(n); }
	void SetColourActiveText( int n )			{ m_ColourActiveText.SetValue(n); }
	void SetColourCompleteBack( int n )			{ m_ColourCompleteBack.SetValue(n); }
	void SetColourCompleteText( int n )			{ m_ColourCompleteText.SetValue(n); }
	void SetColourClosedBack( int n )			{ m_ColourClosedBack.SetValue(n); }
	void SetColourClosedText( int n )			{ m_ColourClosedText.SetValue(n); }
	void SetColourSelectedBack( int n )			{ m_ColourSelectedBack.SetValue(n); }
	void SetColourSelectedText( int n )			{ m_ColourSelectedText.SetValue(n); }
	void SetColourHighlightedBack( int n )		{ m_ColourHLightBack.SetValue(n); }
	void SetColourHighlightedText( int n )		{ m_ColourHLightText.SetValue(n); }
	void SetColourUnhighlightedBack( int n )	{ m_ColourUnHLightBack.SetValue(n); }
	void SetColourUnhighlightedText( int n )	{ m_ColourUnHLightText.SetValue(n); }
	
	//ADDRESS TAB
	void SetHotelName( const char* sz )			{ m_HotelName.SetValue(sz); }
	void SetHotelAddress1 ( const char* sz )	{ m_HotelAddress.SetValue(0,sz); }
	void SetHotelAddress2 ( const char* sz )	{ m_HotelAddress.SetValue(1,sz); }
	void SetHotelAddress3 ( const char* sz )	{ m_HotelAddress.SetValue(2,sz); }
	void SetHotelPostcode ( const char* sz )	{ m_HotelPostcode.SetValue(sz); }
	void SetHotelPhone ( const char* sz )		{ m_HotelPhone.SetValue(sz); }
	void SetHotelFax ( const char* sz )			{ m_HotelFax.SetValue(sz); }
	void SetHotelEmail ( const char* sz )		{ m_HotelEmail.SetValue(sz); }
	void SetHotelTaxNo ( const char* sz )		{ m_HotelTaxNo.SetValue(sz); }

	//SALES TAB
	void SetQuickSaleLegacyFlag( bool b )		{ m_QuickSaleLegacy.SetValue(b); }
	void SetQuickSaleEnable( int n, int e )		{ m_QuickSaleEnable.SetValue(n, e); }
	void SetQuickSalePlu( int n, __int64 nPlu )	{ m_QuickSalePlu.SetValue(n, nPlu); }
	void SetQuickSaleType( int n, int nType )	{ m_QuickSaleType.SetValue(n, nType); }

	//PAYMENT TAB
	void SetPaymentType( int n, int t )			{ m_PaymentType.SetValue(n,t); }

	//DINNER TAB
	void SetDinnerDepartments( const char* sz )	{ m_DinnerDepartments.SetValue(sz); }
	void SetDinnerAllowance( double d )			{ m_DinnerAllowance.SetValue(d); }
	
	//INVOICE TAB
	void SetInvoiceDefaultType( int n )			{ m_InvoiceDefaultType.SetValue(n); }
	void SetInvoiceShowAddressFlag( bool b )	{ m_InvoiceShowAddress.SetValue(b); }
	void SetInvoiceHeaderLines( int n )			{ m_InvoiceHeaderLines.SetValue(n); }
	void SetInvoiceHideReferenceFlag( bool b )	{ m_InvoiceHideRef.SetValue(b); }
	void SetInvoiceHideNumberFlag( bool b )		{ m_InvoiceHideNumber.SetValue(b); }
	void SetInvoiceHideInterimFlag( bool b )	{ m_InvoiceHideInterim.SetValue(b); }
	void SetInvoiceShowNoteFlag( bool b )		{ m_InvoiceShowNote.SetValue(b); }
	void SetInvoiceHideDatesOfStayFlag( bool b ){ m_InvoiceHideStay.SetValue(b); }

	//EPOS REPORTS TAB
	void SetEposReportHour( int n )				{ m_ReportsEposHour.SetValue(n); }
	void SetEposReportMinute ( int n )			{ m_ReportsEposMinute.SetValue(n); }
	void SetFixedTimeReportEnableFlag( bool b )	{ m_ReportsFixedTime.SetValue(b); }
	void SetAccomReportHour( int n )			{ m_ReportsAccomReportHour.SetValue(n); }
	void SetAccomReportMinute( int n )			{ m_ReportsAccomReportMinute.SetValue(n); }
	void SetAccomChargeHour( int n )			{ m_ReportsAccomChargeHour.SetValue(n); }
	void SetAccomChargeMinute( int n )			{ m_ReportsAccomChargeMinute.SetValue(n); }
	void SetAccomReportNextDayFlag( bool b )	{ m_ReportsAccomNextDay.SetValue(b); }
	void SetEposReportBreakfastValue( double d ){ m_ReportsBreakfastVal.SetValue(d); }
	void SetEposReportRoomOnlyPlu( __int64 n )	{ m_ReportsPluRoom.SetValue(n); }
	void SetEposReportRoomBBPlu( __int64 n )	{ m_ReportsPluBB.SetValue(n); }
	void SetEposReportRoomDBBPlu( __int64 n )	{ m_ReportsPluDBB.SetValue(n); }
	void SetEposReportExtraBedPlu( __int64 n )	{ m_ReportsPluExtraBed.SetValue(n); }
	void SetEposReportExtraCotPlu( __int64 n )	{ m_ReportsPluExtraCot.SetValue(n); }
	void SetEposReportBreakfastPlu( __int64 n )	{ m_ReportsPluBreakfast.SetValue(n); }
	void SetEposReportDinnerPlu( __int64 n )	{ m_ReportsPluDinner.SetValue(n); }

	//EPOS LINK TAB
	void SetEposLinkRoomPath( const char* sz )	{ m_EposLinkRoom.SetValue(sz); }
	void SetEposLinkPostPath( const char* sz )	{ m_EposLinkPost.SetValue(sz); }

	//MISC TAB
	void SetMiscEnableMultipleAccounts( bool b )	{ m_MiscMultiAccount.SetValue(b); }
	void SetMiscEnableTrainingMode( bool b )		{ m_MiscTraining.SetValue(b); }
	void SetMiscEnableEmailConfirmation( bool b )	{ m_MiscEmailConfirm.SetValue(b); }
	void SetMiscEmailShowNote( bool b )				{ m_MiscEmailShowNote.SetValue(b); }
	void SetMiscEnableFreeRoom( bool b)				{ m_MiscFreeRoom.SetValue(b); }
	void SetMiscDefaultRoomRateType( int n )		{ m_MiscDefRoomRate.SetValue(n); }
	void SetMiscManualSalesStockpoint( int n )		{ m_MiscStockpoint.SetValue(n); }
	void SetMiscEnableRateRoom( bool b )			{ m_MiscEnableRoom.SetValue(b); }
	void SetMiscEnableRateBB( bool b )				{ m_MiscEnableBB.SetValue(b); }
	void SetMiscEnableRateDBB( bool b )				{ m_MiscEnableDBB.SetValue(b); }
	
	//PMS FOLDERS
	void CreatePMSFolders();
	const char* GetPMSFolder();
	const char* GetPMSFolderSysFiles();
	const char* GetPMSFolderSales();
	const char* GetPMSFolderDownload();
	const char* GetPMSFolderPending();
	const char* GetPMSFolderReports();
	const char* GetPMSFolderBooking();
	const char* GetPMSFolderCustomer();
	
	//SPECIAL MEMORY ONLY OPTIONS, NOT SAVED TO FILE
	bool GetCheckoutFilterTick(){ return m_bCheckoutFilterTick; }
	void SetCheckoutFilterTick( bool b ){ m_bCheckoutFilterTick = b; }

	//PAYMENT TYPE LIST
	void BuildValidatedPaymentTypeArray();
	int GetValidatedPaymentTypeArraySize();
	const char* GetValidatedPaymentTypeString( int n );

	bool AreAnyQuickSalesEnabled();

public:
	void LoadDinnerDepartmentBuffer( CPMSBookingCSVRecord& BookingRecord );
	bool CheckDinnerDepartmentBuffer( int nDeptNo );
	
public:
	void NoPaymentTypeError();

private:
	//DISPLAY TAB
	CIniFileInt m_ColourVacantBack;
	CIniFileInt m_ColourVacantText;
	CIniFileInt m_ColourPendingBack;
	CIniFileInt m_ColourPendingText;
	CIniFileInt m_ColourActiveBack;
	CIniFileInt m_ColourActiveText;
	CIniFileInt m_ColourCompleteBack;
	CIniFileInt m_ColourCompleteText;
	CIniFileInt m_ColourClosedBack;
	CIniFileInt m_ColourClosedText;
	CIniFileInt m_ColourSelectedBack;
	CIniFileInt m_ColourSelectedText;
	CIniFileInt m_ColourHLightText;
	CIniFileInt m_ColourHLightBack;
	CIniFileInt m_ColourUnHLightText;
	CIniFileInt m_ColourUnHLightBack;

	//ADDRESS TAB
	CIniFileString m_HotelName;
	CIniFileStringArray m_HotelAddress;
	CIniFileString m_HotelPostcode;
	CIniFileString m_HotelPhone;
	CIniFileString m_HotelFax;
	CIniFileString m_HotelEmail;
	CIniFileString m_HotelTaxNo;

	//SALES TAB
	CIniFileBool m_QuickSaleLegacy;
	CIniFileIntArray m_QuickSaleEnable;
	CIniFileInt64Array m_QuickSalePlu;
	CIniFileIntArray m_QuickSaleType;

	//PAYMENT TAB
	CIniFileIntArray m_PaymentType;

	//DINNER TAB
	CIniFileString m_DinnerDepartments;
	CIniFileDouble m_DinnerAllowance;
	
	//INVOICES TAB
	CIniFileInt m_InvoiceDefaultType;
	CIniFileBool m_InvoiceShowAddress;
	CIniFileInt m_InvoiceHeaderLines;
	CIniFileBool m_InvoiceHideRef;
	CIniFileBool m_InvoiceHideNumber;
	CIniFileBool m_InvoiceHideInterim;
	CIniFileBool m_InvoiceShowNote;
	CIniFileBool m_InvoiceHideStay;

	//EPOS REPORTS TAB
	CIniFileInt m_ReportsEposHour;
	CIniFileInt m_ReportsEposMinute;
	CIniFileBool m_ReportsFixedTime;
	CIniFileInt m_ReportsAccomReportHour;
	CIniFileInt m_ReportsAccomReportMinute;
	CIniFileInt m_ReportsAccomChargeHour;
	CIniFileInt m_ReportsAccomChargeMinute;
	CIniFileBool m_ReportsAccomNextDay;
	CIniFileDouble m_ReportsBreakfastVal;
	CIniFileInt64 m_ReportsPluRoom;
	CIniFileInt64 m_ReportsPluBB;
	CIniFileInt64 m_ReportsPluDBB;
	CIniFileInt64 m_ReportsPluExtraBed;
	CIniFileInt64 m_ReportsPluExtraCot;
	CIniFileInt64 m_ReportsPluBreakfast;
	CIniFileInt64 m_ReportsPluDinner;

	//EPOS LINK TAB
	CIniFileString m_EposLinkRoom;
	CIniFileString m_EposLinkPost;

	//MISC TAB
	CIniFileBool m_MiscMultiAccount;
	CIniFileBool m_MiscTraining;
	CIniFileBool m_MiscEmailConfirm;
	CIniFileBool m_MiscEmailShowNote;
	CIniFileBool m_MiscFreeRoom;
	CIniFileInt m_MiscDefRoomRate;
	CIniFileInt m_MiscStockpoint;
	CIniFileBool m_MiscEnableRoom;
	CIniFileBool m_MiscEnableBB;
	CIniFileBool m_MiscEnableDBB;

private:
	CString m_strPMSFolder;
	CString m_strPMSFolderSysFiles;
	CString m_strPMSFolderActive;
	CString m_strPMSFolderSales;
	CString m_strPMSFolderDownload;
	CString m_strPMSFolderPending;
	CString m_strPMSFolderReports;
	CString m_strPMSFolderBooking;
	CString m_strPMSFolderCustomer;
	
private:
	bool m_bCheckoutFilterTick;
	bool m_bInvoiceAsComplete;

private:
	CString m_strDinnerDepartmentBuffer;
	bool m_bDinnerDepartmentBuffer[ 100 ];

private:
	CStringArray m_arrayPaymentTypes;
	CString m_strPaymentType;

private:
	COleDateTime m_olePMSTime;
	int m_nPMSDayNumber;
	COleDateTime m_olePMSTimeAccom;
	int m_nPMSDayNumberAccom;

public:
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CPMSOptions PMSOptions; /* global */
/**********************************************************************/
#endif
/**********************************************************************/
