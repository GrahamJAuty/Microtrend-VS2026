//$$******************************************************************
#include "ListDataDatabaseTestDlg.h"
#include "LoyaltyManager.h"
#include "ReportCSVFile.h"
#include "SetupEposPathsDlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "ServerPropertyPage3.h"
//$$******************************************************************

CServerPropertyPage3::CServerPropertyPage3()
	: CSSPropertyPage(CServerPropertyPage3::IDD)
{
}

//$$******************************************************************

void CServerPropertyPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_STAFFGIFT_PLU, m_strStaffGiftPlu);
	DDX_Text(pDX, IDC_EDIT_STAFFGIFT_REDIR_PLU, m_strStaffGiftRedirPlu);
	/**********/
	DDX_Control(pDX, IDC_EDIT_SALES_PLUDATAPATH, m_editEcrmanImportFolder);
	DDX_Control(pDX, IDC_CHECK_SALES_PLUBONUS, m_checkSalesPluBonus);
	DDX_Control(pDX, IDC_CHECK_SALES_DEPTBONUS, m_checkSalesDeptBonus);
	DDX_Control(pDX, IDC_CHECK_SALES_NOPURSEBONUS, m_checkSalesNoPurseBonus);
	DDX_Control(pDX, IDC_CHECK_PURSE2BONUS, m_checkPurse2Bonus);
	DDX_Control(pDX, IDC_CHECK_PURSE1SALES, m_checkPurse1Sales);
	DDX_Control(pDX, IDC_CHECK_SALES_TLOG, m_checkSalesTLog);
	DDX_Control(pDX, IDC_BUTTON_SALES_BROWSE, m_buttonSalesBrowse);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_buttonTest);
	DDX_Control(pDX, IDC_CHECK_GLOBALDEPT, m_checkGlobalDept);
	DDX_Control(pDX, IDC_CHECK_GLOBALOFFER, m_checkGlobalOffer);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CServerPropertyPage3, CPropertyPage)
	
	ON_BN_CLICKED(IDC_BUTTON_SALES_BROWSE, OnButtonSalesBrowse)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_CHECK_SALES_PLUBONUS, OnTogglePluOrDeptBonus)
	ON_BN_CLICKED(IDC_CHECK_SALES_DEPTBONUS, OnTogglePluOrDeptBonus)
	
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CServerPropertyPage3::OnInitDialog()
{
	m_strStaffGiftPlu = Server.GetStaffGiftPluNo();
	m_strStaffGiftRedirPlu = Server.GetStaffGiftRedirectPluNo();
	m_strStaffGiftRedirPlu += "0000";
	m_strStaffGiftRedirPlu.TrimLeft("0");
	/**********/
	
	CPropertyPage::OnInitDialog();

	m_checkSalesPluBonus.SetCheck( Server.GetEnablePluBonusFlag() );
	m_checkSalesDeptBonus.SetCheck(Server.GetEnableDeptBonusFlag());
	m_checkSalesNoPurseBonus.SetCheck( Server.GetNoPurseBonusFlag() );
	m_checkPurse2Bonus.SetCheck( Server.GetSpendBonusToPurse2Flag() );
	m_checkPurse1Sales.SetCheck( Server.GetSpendUsePurse1FirstFlag() );
	m_checkSalesTLog.SetCheck( Server.GetPurchaseHistoryFlag() );

	m_editEcrmanImportFolder.SetWindowText(Server.GetEcrManagerImportFolder());
	m_checkGlobalDept.SetCheck(Server.GetEcrManagerGlobalDeptFlag());
	m_checkGlobalOffer.SetCheck(Server.GetEcrManagerGlobalStampOfferFlag());

	SubclassEdit(IDC_EDIT_STAFFGIFT_PLU, SS_NUM, MAX_PLU_LEN, "%s");
	SubclassEdit(IDC_EDIT_STAFFGIFT_REDIR_PLU, SS_NUM, MAX_PLU_LEN, "%s");

	if ( System.GetEnablePurse2Flag() == FALSE )
	{
		ShowAndEnableWindow(&m_checkPurse2Bonus, FALSE);
		ShowAndEnableWindow(&m_checkPurse1Sales, FALSE);
		MoveControl(&m_checkSalesTLog, 25, 132);		
	}

	m_checkPurse2Bonus.EnableWindow(System.GetEnablePurse2Flag());

	OnTogglePluOrDeptBonus();

	return TRUE;
}

//$$******************************************************************
CEdit* CServerPropertyPage3::GetEditStaffGiftPlu() { return GetEdit(IDC_EDIT_STAFFGIFT_PLU); }
CEdit* CServerPropertyPage3::GetEditStaffGiftRedirPlu() { return GetEdit(IDC_EDIT_STAFFGIFT_REDIR_PLU); }
//$$******************************************************************

BOOL CServerPropertyPage3::OnSetActive()
{
	return CPropertyPage::OnSetActive();
}

//$$******************************************************************

BOOL CServerPropertyPage3::OnKillActive()
{
	if (UpdateData() == TRUE)
	{
		Server.SetStaffGiftPluNo(m_strStaffGiftPlu);

		{
			int nLength = m_strStaffGiftRedirPlu.GetLength();

			if (nLength > 4)
			{
				Server.SetStaffGiftRedirectPluNo(m_strStaffGiftRedirPlu.Left(nLength - 4));
			}
			else
			{
				Server.SetStaffGiftRedirectPluNo("");
			}
		}

		/**********/

		Server.SetEcrManagerImportFolder(GetEditBoxText(m_editEcrmanImportFolder));
		Server.SetEcrManagerGlobalDeptFlag(IsTicked(m_checkGlobalDept));
		Server.SetEcrManagerGlobalStampOfferFlag(IsTicked(m_checkGlobalOffer));
		Server.SetEnablePluBonusFlag(IsTicked(m_checkSalesPluBonus));
		Server.SetEnableDeptBonusFlag(IsTicked(m_checkSalesDeptBonus));
		Server.SetNoPurseBonusFlag(IsTicked(m_checkSalesNoPurseBonus));
		Server.SetSpendBonusToPurse2Flag(IsTicked(m_checkPurse2Bonus));
		Server.SetSpendUsePurse1FirstFlag(IsTicked(m_checkPurse1Sales));
		Server.SetPurchaseHistoryFlag(IsTicked(m_checkSalesTLog));
	}

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CServerPropertyPage3::OnButtonSalesBrowse()
{
	CString strPathname = "";
	if (BrowseFolder(strPathname, "Locate the Ecr / Stock Manager import folder", NULL, this) == TRUE)
	{
		m_editEcrmanImportFolder.SetWindowText(strPathname);
	}
}

//$$******************************************************************

void CServerPropertyPage3::OnButtonTest()
{
	Server.SetEcrManagerImportFolder(GetEditBoxText(m_editEcrmanImportFolder));
	Server.SetEcrManagerGlobalDeptFlag(IsTicked(m_checkGlobalDept));
	CListDataDatabaseTestDlg dlg(Server.GetEcrManagerImportFolder(), this);
	dlg.DoModal();
}

//$$******************************************************************

void CServerPropertyPage3::OnTogglePluOrDeptBonus()
{
	m_checkSalesNoPurseBonus.EnableWindow(IsTicked(m_checkSalesPluBonus) || (IsTicked(m_checkSalesDeptBonus)));
}

//$$******************************************************************

