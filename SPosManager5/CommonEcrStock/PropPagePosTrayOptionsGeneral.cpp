/**********************************************************************/
#include "PosTrayOptions.h"
#include "PropertySheetPosTrayOptions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePosTrayOptionsGeneral.h"
/**********************************************************************/

CPropPagePosTrayOptionsGeneral::CPropPagePosTrayOptionsGeneral() : CSSPropertyPage(CPropPagePosTrayOptionsGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePosTrayOptionsGeneral)
	//}}AFX_DATA_INIT
	m_pPropertySheet = NULL;
}

/**********************************************************************/

CPropPagePosTrayOptionsGeneral::~CPropPagePosTrayOptionsGeneral()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePosTrayOptionsGeneral)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_STARTUP, m_checkStartUp);
	DDX_Control(pDX, IDC_CHECK_HIBERNATE, m_checkHibernate);
	DDX_Control(pDX, IDC_CHECK_REALTIME, m_checkRealTime);
	DDX_Control(pDX, IDC_CHECK_PROCESSTRAN, m_checkProcessTran);
	DDX_Control(pDX, IDC_COMBO_SPEED, m_comboSpeed);
	DDX_Control(pDX, IDC_CHECK_REPORT_EPOS, m_checkReportEpos);
	DDX_Control(pDX, IDC_CHECK_REPORT_HISTORY, m_checkReportHistory);
	DDX_Control(pDX, IDC_CHECK_REPORT_TIMEATTEND, m_checkReportTimeAttend);
	DDX_Control(pDX, IDC_CHECK_MANAGEBACKUP, m_checkManageBackup);
	DDX_Control(pDX, IDC_CHECK_EXPORT_SALES, m_checkExportSales);
	DDX_Control(pDX, IDC_CHECK_EXPORT_PAYMENT, m_checkExportPayment);
	DDX_Control(pDX, IDC_CHECK_EXPORT_VOID, m_checkExportVoid);
	DDX_Control(pDX, IDC_CHECK_EXPORT_DISCOUNT, m_checkExportDiscount);
	DDX_Control(pDX, IDC_CHECK_FNB, m_checkFNB);
	DDX_Control(pDX, IDC_CHECK_CHARTWELLS, m_checkChartwells);
	DDX_Control(pDX, IDC_CHECK_S4LABOUR, m_checkS4Labour);
	DDX_Control(pDX, IDC_CHECK_FILENAME, m_checkFilename);
	DDX_Control(pDX, IDC_CHECK_FIELDNAME, m_checkFieldname);
	DDX_Control(pDX, IDC_CHECK_MANUAL, m_checkManual);
	DDX_Control(pDX, IDC_CHECK_BUSINESSDATE, m_checkBusinessDate);
	DDX_Control(pDX, IDC_CHECK_SKIPEMPTY, m_checkSkipEmpty);
	DDX_Control(pDX, IDC_CHECK_SKIPNOCHANGE, m_checkSkipNoChange);
	DDX_Control(pDX, IDC_COMBO_DATE, m_comboDate);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePosTrayOptionsGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePosTrayOptionsGeneral)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_CHECK_PROCESSTRAN, OnToggleTasks)
	ON_BN_CLICKED( IDC_CHECK_REPORT_EPOS, OnToggleTasks)
	ON_BN_CLICKED( IDC_CHECK_REPORT_HISTORY, OnToggleTasks)
	ON_BN_CLICKED( IDC_CHECK_REPORT_TIMEATTEND, OnToggleTasks)
	ON_BN_CLICKED( IDC_CHECK_EXPORT_SALES, OnToggleTasks)
	ON_BN_CLICKED( IDC_CHECK_EXPORT_PAYMENT, OnToggleTasks)
	ON_BN_CLICKED( IDC_CHECK_EXPORT_VOID, OnToggleTasks)
	ON_BN_CLICKED( IDC_CHECK_EXPORT_DISCOUNT, OnToggleTasks)
	ON_BN_CLICKED( IDC_CHECK_FNB, OnToggleTasks)
	ON_BN_CLICKED( IDC_CHECK_CHARTWELLS, OnToggleTasks)
	ON_BN_CLICKED( IDC_CHECK_S4LABOUR, OnToggleTasks)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePosTrayOptionsGeneral::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	m_checkStartUp.SetCheck( CheckStartupLink() );
	m_checkRealTime.SetCheck( PosTrayOptions.GetPosTrayRealTimeFlag() );
	m_checkProcessTran.SetCheck(PosTrayOptions.GetPosTrayProcessTranFlag());
	m_checkHibernate.SetCheck( PosTrayOptions.GetDisableHibernateFlag() );
	m_checkReportEpos.SetCheck( PosTrayOptions.GetReportTaskEposFlag() );
	m_checkReportHistory.SetCheck( PosTrayOptions.GetReportTaskHistoryFlag() );
	m_checkReportTimeAttend.SetCheck(PosTrayOptions.GetReportTaskTimeAttendFlag());
	m_checkManageBackup.SetCheck(PosTrayOptions.GetManageBackupFlag());
	m_checkExportSales.SetCheck( PosTrayOptions.GetExportTaskSalesFlag() );
	m_checkExportPayment.SetCheck( PosTrayOptions.GetExportTaskPaymentFlag() );
	m_checkExportVoid.SetCheck( PosTrayOptions.GetExportTaskVoidFlag() );
	m_checkExportDiscount.SetCheck( PosTrayOptions.GetExportTaskDiscountFlag() );
	m_checkFNB.SetCheck( PosTrayOptions.GetFNBTaskFlag() );
	m_checkChartwells.SetCheck(PosTrayOptions.GetChartwellsTaskFlag());
	m_checkS4Labour.SetCheck( PosTrayOptions.GetS4LabourTaskFlag() );
	m_checkFilename.SetCheck( PosTrayOptions.GetEditReportFilenamesFlag() );
	m_checkFieldname.SetCheck( PosTrayOptions.GetEditExportFieldnamesFlag() );
	m_checkManual.SetCheck( PosTrayOptions.GetManualExportFlag() );
	m_checkBusinessDate.SetCheck( PosTrayOptions.GetBusinessDateFlag() );
	m_checkSkipEmpty.SetCheck(PosTrayOptions.GetSkipEmptyFlag());
	m_checkSkipNoChange.SetCheck(PosTrayOptions.GetSkipNoChangeFlag());

	m_comboSpeed.AddString( "1 (slowest)" );
	m_comboSpeed.AddString( "2" );
	m_comboSpeed.AddString( "3" );
	m_comboSpeed.AddString( "4" );
	m_comboSpeed.AddString( "5" );
	m_comboSpeed.AddString( "6" );
	m_comboSpeed.AddString( "7" );
	m_comboSpeed.AddString( "8" );
	m_comboSpeed.AddString( "9" );
	m_comboSpeed.AddString( "10 (fastest)" );

	{
		int nSel = PosTrayOptions.GetRealTimeCommsSpeed();
		if ((nSel >= 1) && (nSel <= 10))
		{
			m_comboSpeed.SetCurSel(nSel - 1);
		}
		else
		{
			m_comboSpeed.SetCurSel(0);
		}
	}

	m_comboDate.AddString( "DD/MM/YYYY" );
	m_comboDate.AddString( "YYYY/DD/MM" );
	m_comboDate.AddString( "DDMMYYYY" );
	m_comboDate.AddString( "YYYYMMDD" );

	{
		int nSel = 0;
		switch( PosTrayOptions.GetExportDateType() )
		{
		case 1: nSel = 0;	break;
		case 2: nSel = 2;	break;
		case 3: nSel = 3;	break;
		case 4: nSel = 1;	break;
		}
		m_comboDate.SetCurSel(nSel);
	}

	if ( SysInfo.IsPosTrayTimeAndAttendanceSystem() == FALSE )
	{
		m_checkReportTimeAttend.SetCheck(FALSE);
		ShowAndEnableWindow(&m_checkReportTimeAttend, FALSE);
	}

	if (SysInfo.GetLoyaltyOrSmartPayType() != LOYALTY_TYPE_SMP_V4)
	{
		ShowAndEnableWindow(&m_checkChartwells, FALSE);
	}

	return TRUE;  
}

/**********************************************************************/

void CPropPagePosTrayOptionsGeneral::OnToggleTasks()
{
	PosTrayOptions.SetPosTrayProcessTranFlag(IsTicked(m_checkProcessTran));
	PosTrayOptions.SetReportTaskEposFlag(IsTicked(m_checkReportEpos));
	PosTrayOptions.SetReportTaskHistoryFlag(IsTicked(m_checkReportHistory));
	PosTrayOptions.SetReportTaskTimeAttendFlag(IsTicked(m_checkReportTimeAttend));
	PosTrayOptions.SetExportTaskSalesFlag(IsTicked(m_checkExportSales));
	PosTrayOptions.SetExportTaskPaymentFlag(IsTicked(m_checkExportPayment));
	PosTrayOptions.SetExportTaskVoidFlag(IsTicked(m_checkExportVoid));
	PosTrayOptions.SetExportTaskDiscountFlag(IsTicked(m_checkExportDiscount));
	PosTrayOptions.SetFNBTaskFlag(IsTicked(m_checkFNB));
	PosTrayOptions.SetChartwellsTaskFlag(IsTicked(m_checkChartwells));
	PosTrayOptions.SetS4LabourTaskFlag(IsTicked(m_checkS4Labour));
	PosTrayOptions.SetEditReportFilenamesFlag(IsTicked(m_checkFilename));
	PosTrayOptions.SetEditExportFieldnamesFlag(IsTicked(m_checkFieldname));
	PosTrayOptions.SetManualExportFlag(IsTicked(m_checkManual));
	PosTrayOptions.SetBusinessDayFlag(IsTicked(m_checkBusinessDate));
	PosTrayOptions.SetSkipEmptyFlag(IsTicked(m_checkSkipEmpty));
	PosTrayOptions.SetSkipNoChangeFlag(IsTicked(m_checkSkipNoChange));
	PosTrayOptions.SetManageBackupFlag(IsTicked(m_checkManageBackup));

	if (SysInfo.IsPosTrayTimeAndAttendanceSystem() == FALSE)
	{
		PosTrayOptions.SetReportTaskTimeAttendFlag(FALSE);
	}

	((CPropertySheetPosTrayOptions*)m_pPropertySheet)->AddTaskPages();
}

/**********************************************************************/

bool CPropPagePosTrayOptionsGeneral::CheckStartupLink()
{
	CString strPath = ::GetStartupPath();	
	if (strPath == "")
	{
		return FALSE;
	}

	int nDataPath = SysInfo.GetDataPathNo();

	if (0 == nDataPath)
	{
		strPath += "\\SPosTray.lnk";
	}
	else
	{
		CString strExtra;
		strExtra.Format( "\\SPosTray_%d.lnk", nDataPath );
		strPath += strExtra;
	}

	return ::FileExists( strPath );
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePosTrayOptionsGeneral::OnSetActive() 
{
	GetRecordData();
	return ( CPropertyPage::OnSetActive() != 0 );
}

/**********************************************************************/

void CPropPagePosTrayOptionsGeneral::GetRecordData()
{
}

/**********************************************************************/

void CPropPagePosTrayOptionsGeneral::SaveRecord()
{
	PosTrayOptions.SetTemporaryStartupFlag(IsTicked(m_checkStartUp));
	PosTrayOptions.SetPosTrayRealTimeFlag(IsTicked(m_checkRealTime));
	PosTrayOptions.SetPosTrayProcessTranFlag(IsTicked(m_checkProcessTran));
	PosTrayOptions.SetDisableHibernateFlag(IsTicked(m_checkHibernate));
	PosTrayOptions.SetReportTaskEposFlag(IsTicked(m_checkReportEpos));
	PosTrayOptions.SetReportTaskHistoryFlag(IsTicked(m_checkReportHistory));
	PosTrayOptions.SetReportTaskTimeAttendFlag(IsTicked(m_checkReportTimeAttend));
	PosTrayOptions.SetExportTaskSalesFlag(IsTicked(m_checkExportSales));
	PosTrayOptions.SetExportTaskPaymentFlag(IsTicked(m_checkExportPayment));
	PosTrayOptions.SetExportTaskVoidFlag(IsTicked(m_checkExportVoid));
	PosTrayOptions.SetExportTaskDiscountFlag(IsTicked(m_checkExportDiscount));
	PosTrayOptions.SetFNBTaskFlag(IsTicked(m_checkFNB));
	PosTrayOptions.SetChartwellsTaskFlag(IsTicked(m_checkChartwells));
	PosTrayOptions.SetS4LabourTaskFlag(IsTicked(m_checkS4Labour));
	PosTrayOptions.SetEditReportFilenamesFlag(IsTicked(m_checkFilename));
	PosTrayOptions.SetEditExportFieldnamesFlag(IsTicked(m_checkFieldname));
	PosTrayOptions.SetManualExportFlag(IsTicked(m_checkManual));
	PosTrayOptions.SetBusinessDayFlag(IsTicked(m_checkBusinessDate));
	PosTrayOptions.SetSkipEmptyFlag(IsTicked(m_checkSkipEmpty));
	PosTrayOptions.SetSkipNoChangeFlag(IsTicked(m_checkSkipNoChange));
	PosTrayOptions.SetManageBackupFlag(IsTicked(m_checkManageBackup));

	if (SysInfo.GetLoyaltyOrSmartPayType() != LOYALTY_TYPE_SMP_V4)
	{
		PosTrayOptions.SetChartwellsTaskFlag(FALSE);
	}

	if (SysInfo.IsPosTrayTimeAndAttendanceSystem() == FALSE)
	{
		PosTrayOptions.SetReportTaskTimeAttendFlag(FALSE);
	}

	{
		int nSel = m_comboSpeed.GetCurSel();
		if ((nSel >= 0) && (nSel <= 9))
		{
			PosTrayOptions.SetRealTimeCommsSpeed(nSel + 1);
		}
		else
		{
			PosTrayOptions.SetRealTimeCommsSpeed(1);
		}
	}

	{
		int nType = 1;
		switch (m_comboDate.GetCurSel())
		{
		case 0:	nType = 1;	break;
		case 1:	nType = 4;	break;
		case 2:	nType = 2;	break;
		case 3:	nType = 3;	break;
		}
		PosTrayOptions.SetExportDateType(nType);
	}
}

/**********************************************************************/
