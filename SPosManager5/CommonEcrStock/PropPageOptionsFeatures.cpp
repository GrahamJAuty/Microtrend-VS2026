/**********************************************************************/
#include "ColourEditDlg.h"
#include "DefEditStartupMsgDlg.h"
#include "MyMessageBoxDlg.h"
#include "PropertySheetKeyboardSets.h"
#include "SetupReaderDlg.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "PropPageOptionsFeatures.h"
/**********************************************************************/

CPropPageOptionsFeatures::CPropPageOptionsFeatures() : CSSPropertyPage(CPropPageOptionsFeatures::IDD)
{
	//{{AFX_DATA_INIT(CPropPageOptionsFeatures)
	//}}AFX_DATA_INIT
	m_pMainWnd = NULL;
	m_nEditingColour = 0;
	m_nActiveTab = 0;
}

/**********************************************************************/

CPropPageOptionsFeatures::~CPropPageOptionsFeatures()
{
}

/**********************************************************************/

void CPropPageOptionsFeatures::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOptionsFeatures)

	DDX_Control( pDX, IDC_TAB_ACTIONS, m_TabActions );

	DDX_Control(pDX, IDC_STATIC_GENERAL1, m_staticGeneral1);
	DDX_Control(pDX, IDC_STATIC_GENERAL2, m_staticGeneral2);
	DDX_Control(pDX, IDC_STATIC_GENERAL3, m_staticGeneral3);
	DDX_Control(pDX, IDC_STATIC_GENERAL4, m_staticGeneral4);
	DDX_Control(pDX, IDC_STATIC_GENERAL5, m_staticGeneral5);
	DDX_Control(pDX, IDC_COMBO_EDITING, m_comboEditing);
	DDX_Control(pDX, IDC_COMBO_CONTEXT, m_comboContext);
	DDX_Control(pDX, IDC_COMBO_PLUSTOCK, m_comboPluStock);
	DDX_Control(pDX, IDC_COMBO_DELIVERY, m_comboDelivery);
	DDX_Control(pDX, IDC_COLOURBUTTON_BACKGROUND, m_buttonColourBackground);
	DDX_Control(pDX, IDC_CHECK_ALLERGEN, m_checkAllergen);
	DDX_Control(pDX, IDC_CHECK_OPNAMEFIRST, m_checkOPNameFirst);
	
	DDX_Control(pDX, IDC_CHECK_MESSAGE, m_checkMessage);
	DDX_Control(pDX, IDC_STATIC_MESSAGE1, m_staticMessage1);
	DDX_Control(pDX, IDC_COLOURBUTTON_MESSAGEBACK, m_buttonColourMessageBack);
	DDX_Control(pDX, IDC_STATIC_MESSAGE2, m_staticMessage2);
	DDX_Control(pDX, IDC_COLOURBUTTON_MESSAGETEXT, m_buttonColourMessageText);
	DDX_Control(pDX, IDC_STATIC_MESSAGE3, m_staticMessage3);
	DDX_Control(pDX, IDC_COMBO_MESSAGE_FONT, m_comboMessageFont);
	DDX_Control(pDX, IDC_STATIC_MESSAGE4, m_staticMessage4);
	DDX_Control(pDX, IDC_COMBO_MESSAGE_SPACE, m_comboMessageSpace);
	DDX_Control(pDX, IDC_BUTTON_MESSAGE_PREVIEW, m_buttonMessagePreview);

	DDX_Control(pDX, IDC_CHECK_STARTUPMSG, m_checkStartupMsg);
	DDX_Control(pDX, IDC_STATIC_STARTUPMSG1, m_staticStartupMsg1);
	DDX_Control(pDX, IDC_COLOURBUTTON_STARTUPMSGBACK, m_buttonColourStartupMsgBack);
	DDX_Control(pDX, IDC_STATIC_STARTUPMSG2, m_staticStartupMsg2);
	DDX_Control(pDX, IDC_COLOURBUTTON_STARTUPMSGTEXT, m_buttonColourStartupMsgText);
	DDX_Control(pDX, IDC_STATIC_STARTUPMSG3, m_staticStartupMsg3);
	DDX_Control(pDX, IDC_COMBO_STARTUPMSG_FONT, m_comboStartupMsgFont);
	DDX_Control(pDX, IDC_STATIC_STARTUPMSG4, m_staticStartupMsg4);
	DDX_Control(pDX, IDC_COMBO_STARTUPMSG_SPACE, m_comboStartupMsgSpace);
	DDX_Control(pDX, IDC_BUTTON_STARTUPMSG_EDIT, m_buttonStartupMsgEdit);
	DDX_Control(pDX, IDC_BUTTON_STARTUPMSG_PREVIEW, m_buttonStartupMsgPreview);

	DDX_Control(pDX, IDC_CHECK_MIXMATCH2, m_checkMixMatch2);
	DDX_Control(pDX, IDC_CHECK_PROMOTIONS, m_checkPromotions);
	DDX_Control(pDX, IDC_CHECK_TIMEPLANS, m_checkTimePlans );
	DDX_Control(pDX, IDC_CHECK_MIXMATCH_DOWNLOAD, m_checkMixMatchDownload);
	DDX_Control(pDX, IDC_CHECK_TEMPLATE, m_checkTimePlanTemplates);
	DDX_Control(pDX, IDC_CHECK_WALLETSTAMPRULE, m_checkWalletStampRules);
	DDX_Control(pDX, IDC_CHECK_GLOBALWALLETSTAMPRULE, m_checkGlobalWalletStampRules);
	
	DDX_Control(pDX, IDC_STATIC_KEYBOARDS1, m_staticKeyboards1);
	DDX_Control(pDX, IDC_STATIC_KEYBOARDS2, m_staticKeyboards2);
	DDX_Control(pDX, IDC_COMBO_KEYBOARDS, m_comboKeyboards);
	DDX_Control(pDX, IDC_CHECK_KEYBOARDS_TEXT, m_checkKeyboardsText);
	DDX_Control(pDX, IDC_COMBO_PLUTEXT, m_comboPluText);
	DDX_Control(pDX, IDC_CHECK_KEYBOARDS_SHARE, m_checkKeyboardsShare);
	DDX_Control(pDX, IDC_BUTTON_KEYBOARDS_SHARE, m_buttonKeyboardsShare);
	
	DDX_Control(pDX, IDC_CHECK_SERVER, m_checkServers);
	DDX_Control(pDX, IDC_CHECK_SERVER_TERMINAL, m_checkServersTerminal);
	DDX_Control(pDX, IDC_CHECK_SERVER_PRESET, m_checkServersPreset);
	DDX_Control(pDX, IDC_CHECK_SERVER_LOGON, m_checkServersLogon);
	DDX_Control(pDX, IDC_COMBO_SERVER, m_comboServers);
	DDX_Control(pDX, IDC_BUTTON_MIFARE, m_buttonMiFare);
	DDX_Control(pDX, IDC_CHECK_CUSTOMER, m_checkCustomers);
	DDX_Control(pDX, IDC_CHECK_CUSTOMER2, m_checkCustomers2);
	DDX_Control(pDX, IDC_CHECK_CUSTOMER3, m_checkCustomers3);
	DDX_Control(pDX, IDC_CHECK_CUSTOMER4, m_checkCustomers4);
	DDX_Control(pDX, IDC_STATIC_CUSTOMER1, m_staticCustomers1);
	DDX_Control(pDX, IDC_CHECK_CUSTOMER5, m_checkCustomers5);
	DDX_Control(pDX, IDC_STATIC_CUSTOMER2, m_staticCustomers2);
	
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOptionsFeatures, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOptionsFeatures)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ACTIONS, OnTcnSelchangeTabs)
	ON_CBN_SELCHANGE(IDC_COMBO_KEYBOARDS, SetActiveTabControls)
	ON_CBN_SELCHANGE(IDC_COMBO_PLUSTOCK, SetActiveTabControls)
	ON_BN_CLICKED(IDC_CHECK_MIXMATCH2, SetActiveTabControls)
	ON_BN_CLICKED(IDC_CHECK_TIMEPLANS, SetActiveTabControls)
	ON_BN_CLICKED(IDC_CHECK_SERVER, SetActiveTabControls)
	ON_BN_CLICKED(IDC_CHECK_SERVER_LOGON, SetActiveTabControls)
	ON_BN_CLICKED(IDC_BUTTON_MIFARE, OnButtonMiFare)
	ON_BN_CLICKED(IDC_CHECK_CUSTOMER, SetActiveTabControls)
	ON_BN_CLICKED(IDC_CHECK_CUSTOMER2, SetActiveTabControls)
	ON_BN_CLICKED(IDC_CHECK_CUSTOMER4, OnToggleCustomerAutoDelete1)
	ON_BN_CLICKED(IDC_CHECK_CUSTOMER5, OnToggleCustomerAutoDelete2)
	ON_BN_CLICKED(IDC_CHECK_KEYBOARDS_SHARE, SetActiveTabControls)
	ON_BN_CLICKED(IDC_BUTTON_KEYBOARDS_SHARE, OnSetupKeyboardsShare)
	ON_BN_CLICKED(IDC_CHECK_MESSAGE, SetActiveTabControls )
	ON_BN_CLICKED(IDC_COLOURBUTTON_BACKGROUND, OnButtonBackground )
	ON_BN_CLICKED(IDC_COLOURBUTTON_MESSAGEBACK, OnButtonMessageBack )
	ON_BN_CLICKED(IDC_COLOURBUTTON_MESSAGETEXT, OnButtonMessageText )
	ON_BN_CLICKED(IDC_BUTTON_MESSAGE_PREVIEW, OnButtonMessagePreview )
	ON_BN_CLICKED(IDC_COLOURBUTTON_STARTUPMSGBACK, OnButtonStartupMsgBack)
	ON_BN_CLICKED(IDC_COLOURBUTTON_STARTUPMSGTEXT, OnButtonStartupMsgText)
	ON_BN_CLICKED(IDC_BUTTON_STARTUPMSG_EDIT, OnButtonStartupMsgEdit)
	ON_BN_CLICKED(IDC_BUTTON_STARTUPMSG_PREVIEW, OnButtonStartupMsgPreview)
	ON_BN_CLICKED(IDC_CHECK_WALLETSTAMPRULE, OnToggleWalletStampRule)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_COLOUR_CHANGE, OnColourButtonMessage )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageOptionsFeatures::OnInitDialog()
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	SubclassEdit(IDC_EDIT_CUSTOMER1, SS_NUM, 2, "%d");
	SubclassEdit(IDC_EDIT_CUSTOMER2, SS_NUM, 2, "%d");

	m_TabActions.InsertItem(0, "General");
	m_TabActions.InsertItem(1, "Message Boxes");

	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		m_TabActions.InsertItem(2, "Mix Match");
		m_TabActions.InsertItem(3, "Keyboards");
		m_TabActions.InsertItem(4, "People");
	}
	else
	{
		m_TabActions.InsertItem(2, "Keyboards");
	}

	m_TabActions.SetCurSel(0);

	bool bStock = SysInfo.IsStockSystem();
	MoveControl(&m_staticGeneral1, 19, 40);
	MoveControl(&m_comboEditing, 115, 39);
	MoveControl(&m_staticGeneral2, 19, 60);
	MoveControl(&m_comboContext, 115, 59);
	MoveControl(&m_staticGeneral3, 19, 90);
	MoveControl(&m_comboPluStock, 115, 89);
	{
		int nYOffset = (bStock) ? 0 : 20;
		MoveControl(&m_staticGeneral4, 19, 90 + nYOffset);
		MoveControl(&m_comboDelivery, 115, 89 + nYOffset);
		MoveControl(&m_staticGeneral5, 19, 122 + nYOffset);
		MoveControl(&m_buttonColourBackground, 115, 121 + nYOffset);
		MoveControl(&m_checkAllergen, 19, 151 + nYOffset);
		MoveControl(&m_checkOPNameFirst, 19, 167 + nYOffset);
	}
	/*****/
	MoveControl(&m_checkMessage, 19, 40);
	MoveControl(&m_staticMessage1, 19, 60);
	MoveControl(&m_buttonColourMessageBack, 99, 60);
	MoveControl(&m_staticMessage2, 19, 80);
	MoveControl(&m_buttonColourMessageText, 99, 80);
	MoveControl(&m_staticMessage3, 19, 100);
	MoveControl(&m_comboMessageFont, 99, 100);
	MoveControl(&m_staticMessage4, 19, 120);
	MoveControl(&m_comboMessageSpace, 99, 120);
	MoveControl(&m_buttonMessagePreview, 99, 140);
	/*****/
	MoveControl(&m_checkStartupMsg, 219, 40);
	MoveControl(&m_staticStartupMsg1, 219, 60);
	MoveControl(&m_buttonColourStartupMsgBack, 299, 60);
	MoveControl(&m_staticStartupMsg2, 219, 80);
	MoveControl(&m_buttonColourStartupMsgText, 299, 80);
	MoveControl(&m_staticStartupMsg3, 219, 100);
	MoveControl(&m_comboStartupMsgFont, 299, 100);
	MoveControl(&m_staticStartupMsg4, 219, 120);
	MoveControl(&m_comboStartupMsgSpace, 299, 120);
	MoveControl(&m_buttonStartupMsgEdit, 299, 140);
	MoveControl(&m_buttonStartupMsgPreview, 299, 160);
	/*****/
	MoveControl(&m_checkMixMatch2, 19, 40);
	MoveControl(&m_checkPromotions, 29, 54);
	MoveControl(&m_checkTimePlans, 29, 68);
	MoveControl(&m_checkTimePlanTemplates, 29, 82);
	MoveControl(&m_checkMixMatchDownload, 29, 96);
	MoveControl(&m_checkWalletStampRules, 19, 116);
	MoveControl(&m_checkGlobalWalletStampRules, 29, 130);

	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_LOY_V4:
		m_checkWalletStampRules.SetWindowText("Loyalty Stamp Offers");
		break;

	case LOYALTY_TYPE_SMP_V4:
		m_checkWalletStampRules.SetWindowText("SmartPay Purchase Control");
		break;

	default:
		ShowAndEnableWindow(&m_checkWalletStampRules, FALSE);
		break;
	}

	/*****/
	MoveControl(&m_staticKeyboards1, 19, 40);
	MoveControl(&m_comboKeyboards, 115, 38);
	MoveControl(&m_checkKeyboardsText, 19, 60);

	{
		int nYPos = 68;
		if (SysInfo.IsEcrLiteSystem() == FALSE)
		{
			nYPos += 22;
		}
		else
		{
			ShowAndEnableWindow(&m_checkKeyboardsText, FALSE);
		}

		MoveControl(&m_staticKeyboards2, 19, nYPos);
		MoveControl(&m_comboPluText, 115, nYPos - 1);
		nYPos += 30;
		MoveControl(&m_checkKeyboardsShare, 19, nYPos);
		nYPos += 16;
		MoveControl(&m_buttonKeyboardsShare, 19, nYPos);
	}
	/*****/
	MoveControl(&m_checkServers, 19, 40);
	MoveControl(&m_checkServersTerminal, 29, 54);
	MoveControl(&m_checkServersPreset, 29, 68);
	MoveControl(&m_checkServersLogon, 29, 82);
	MoveControl(&m_comboServers, 29, 98);
	MoveControl(&m_buttonMiFare, 170, 98);
	MoveControl(&m_checkCustomers, 19, 128);
	MoveControl(&m_checkCustomers2, 29, 142);
	MoveControl(&m_checkCustomers3, 39, 156);
	MoveControl(&m_checkCustomers4, 19, 180);
	MoveControl(&m_staticCustomers1, 39, 195);
	MoveControl(GetEditCustomers1(), 144, 193);
	MoveControl(&m_checkCustomers5, 19, 220);
	MoveControl(&m_staticCustomers2, 39, 235);
	MoveControl(GetEditCustomers2(), 144, 233);
	/*****/

	//GENERAL
	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(80);
		m_comboEditing.SetTabStops(arrayStops);

		m_comboEditing.AddItem("From List,(Full System)");
		m_comboEditing.AddItem("From List,(Active Nodes)");

		int nSel = 0;
		switch (EcrmanOptions.GetFeaturesDatabaseEditingType())
		{
		case 2:
			nSel = 1;
			break;
		}
		m_comboEditing.SetCurSel(nSel);
	}

	{
		m_comboContext.AddString("Prompt for destination folder");
		m_comboContext.AddString("Always send to immediate folder");
		m_comboContext.SetCurSel(EcrmanOptions.GetFeaturesContextForceNowFlag() ? 1 : 0);
	}

	if (FALSE == bStock)
	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		m_comboPluStock.SetTabStops(arrayStops);
		m_comboPluStock.AddItem("Disable");
		m_comboPluStock.AddItem("Enable");
		m_comboPluStock.AddItem("Enable with Delivery Audit");

		if (EcrmanOptions.GetFeaturesPluStockFlag() == FALSE)
		{
			m_comboPluStock.SetCurSel(0);
		}
		else if (EcrmanOptions.GetFeaturesPluAuditFlag() == FALSE)
		{
			m_comboPluStock.SetCurSel(1);
		}
		else
		{
			m_comboPluStock.SetCurSel(2);
		}
	}

	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(80);
		m_comboDelivery.SetTabStops(arrayStops);

		m_comboDelivery.AddItem("Disable");
		m_comboDelivery.AddItem("Use wastage");
		m_comboDelivery.AddItem("Use sales,(Price band 2)");
		m_comboDelivery.AddItem("Use sales,(Price band 3)");
		m_comboDelivery.AddItem("Use sales,(Price band 4)");
		m_comboDelivery.AddItem("Use sales,(Price band 5)");
		m_comboDelivery.AddItem("Use sales,(Price band 6)");
		m_comboDelivery.AddItem("Use sales,(Price band 7)");
		m_comboDelivery.AddItem("Use sales,(Price band 8)");
		m_comboDelivery.AddItem("Use sales,(Price band 9)");
		m_comboDelivery.AddItem("Use sales,(Price band 10)");

		int nType = EcrmanOptions.GetFeaturesEposDeliveryType();
		if ((nType >= 0) && (nType <= 10))
		{
			m_comboDelivery.SetCurSel(nType);
		}
		else
		{
			m_comboDelivery.SetCurSel(0);
		}
	}

	m_buttonColourBackground.m_ColourRefBack = COLORREF(EcrmanOptions.GetFeaturesBackgroundColour());
	m_buttonColourBackground.Invalidate();

	m_checkAllergen.SetCheck(EcrmanOptions.GetFeaturesAllergenFlag());
	m_checkOPNameFirst.SetCheck(EcrmanOptions.GetFeaturesOPNameFirstFlag());

	//MESSAGE BOXES
	m_checkMessage.SetCheck(EcrmanOptions.GetCustomMessageBoxFlag());
	m_buttonColourMessageBack.m_bBlackBorder = TRUE;
	m_buttonColourMessageBack.m_ColourRefBack = EcrmanOptions.GetCustomMessageBoxColourBackInt();
	m_buttonColourMessageBack.Invalidate();
	m_buttonColourMessageText.m_bBlackBorder = TRUE;
	m_buttonColourMessageText.m_ColourRefBack = EcrmanOptions.GetCustomMessageBoxColourTextInt();

	m_comboMessageFont.AddString("8");
	m_comboMessageFont.AddString("10");
	m_comboMessageFont.AddString("11");
	m_comboMessageFont.AddString("12");
	m_comboMessageFont.AddString("14");
	switch (EcrmanOptions.GetCustomMessageBoxFont())
	{
	case 10:	m_comboMessageFont.SetCurSel(1);	break;
	case 11:	m_comboMessageFont.SetCurSel(2);	break;
	case 12:	m_comboMessageFont.SetCurSel(3);	break;
	case 14:	m_comboMessageFont.SetCurSel(4);	break;
	case 8:
	default:	m_comboMessageFont.SetCurSel(0);	break;
	}

	{
		for (int n = 5; n <= 15; n++)
		{
			CString str = "";
			str.Format("%d", n);
			m_comboMessageSpace.AddString(str);
		}

		int nSel = EcrmanOptions.GetCustomMessageBoxLineGap() - 5;
		if ((nSel >= 0) && (nSel < m_comboMessageSpace.GetCount()))
		{
			m_comboMessageSpace.SetCurSel(nSel);
		}
		else
		{
			m_comboMessageSpace.SetCurSel(0);
		}
	}

	m_buttonColourMessageText.Invalidate();

	//STARTUP MESSAGE
	m_checkStartupMsg.SetCheck(EcrmanOptions.GetStartupMessageBoxFlag());
	m_buttonColourStartupMsgBack.m_bBlackBorder = TRUE;
	m_buttonColourStartupMsgBack.m_ColourRefBack = EcrmanOptions.GetStartupMessageBoxColourBackInt();
	m_buttonColourStartupMsgBack.Invalidate();
	m_buttonColourStartupMsgText.m_bBlackBorder = TRUE;
	m_buttonColourStartupMsgText.m_ColourRefBack = EcrmanOptions.GetStartupMessageBoxColourTextInt();

	m_comboStartupMsgFont.AddString("8");
	m_comboStartupMsgFont.AddString("10");
	m_comboStartupMsgFont.AddString("11");
	m_comboStartupMsgFont.AddString("12");
	m_comboStartupMsgFont.AddString("14");
	switch (EcrmanOptions.GetStartupMessageBoxFont())
	{
	case 10:	m_comboStartupMsgFont.SetCurSel(1);	break;
	case 11:	m_comboStartupMsgFont.SetCurSel(2);	break;
	case 12:	m_comboStartupMsgFont.SetCurSel(3);	break;
	case 14:	m_comboStartupMsgFont.SetCurSel(4);	break;
	case 8:
	default:	m_comboStartupMsgFont.SetCurSel(0);	break;
	}

	{
		for (int n = 5; n <= 15; n++)
		{
			CString str = "";
			str.Format("%d", n);
			m_comboStartupMsgSpace.AddString(str);
		}

		int nSel = EcrmanOptions.GetStartupMessageBoxLineGap() - 5;
		if ((nSel >= 0) && (nSel < m_comboStartupMsgSpace.GetCount()))
		{
			m_comboStartupMsgSpace.SetCurSel(nSel);
		}
		else
		{
			m_comboStartupMsgSpace.SetCurSel(0);
		}
	}

	m_buttonColourStartupMsgText.Invalidate();

	//MIX MATCH
	m_checkMixMatch2.SetCheck(EcrmanOptions.GetFeaturesMixMatchFlag());
	m_checkMixMatchDownload.SetCheck(EcrmanOptions.GetFeaturesTimePlanDownloadFlag());
	m_checkPromotions.SetCheck(EcrmanOptions.GetFeaturesPromotionFlag());
	m_checkTimePlans.SetCheck(EcrmanOptions.GetFeaturesTimePlanFlag());
	m_checkTimePlanTemplates.SetCheck(EcrmanOptions.GetFeaturesTimePlanTemplateFlag());	
	m_checkWalletStampRules.SetCheck(EcrmanOptions.GetFeaturesWalletStampRuleFlag());
	m_checkGlobalWalletStampRules.SetCheck(EcrmanOptions.GetFeaturesGlobalWalletStampRuleFlag());

	//KEYBOARDS
	{
		m_comboKeyboards.AddString("None");
		for (int n = 1; n <= MAX_KEYBOARDS_PER_LOCATION; n++)
		{
			CString str = "";
			str.Format("%d", n);
			m_comboKeyboards.AddString(str);
		}

		int nKeyboards = EcrmanOptions.GetFeaturesKeyboardsPerLocation();

		if ((nKeyboards < 1) || (nKeyboards > MAX_KEYBOARDS_PER_LOCATION))
		{
			nKeyboards = 1;
		}

		if (EcrmanOptions.GetFeaturesKeyboardFlag() == FALSE)
		{
			nKeyboards = 0;
		}

		m_comboKeyboards.SetCurSel(nKeyboards);
		m_checkKeyboardsText.SetCheck(EcrmanOptions.GetFeaturesKeyboardPriceSetFlag());
		m_checkKeyboardsShare.SetCheck(EcrmanOptions.GetFeaturesKeyboardShareFlag());

		m_comboPluText.AddString("Key \\ Ecr \\ Report");
		m_comboPluText.AddString("Key \\ Report \\ Ecr");
		m_comboPluText.AddString("Ecr \\ Key \\ Report");
		m_comboPluText.AddString("Ecr \\ Report \\ Key");
		m_comboPluText.AddString("Report \\ Key \\ Ecr");
		m_comboPluText.AddString("Report \\ Ecr \\ Key");

		int nSel = EcrmanOptions.GetFeaturesKeyboardTextOrder();
		if ((nSel < 0) || (nSel > 5))
		{
			nSel = 0;
		}

		m_comboPluText.SetCurSel(nSel);
	}

	//PEOPLE
	m_comboServers.AddString("Same file for all locations");
	m_comboServers.AddString("Different file for each database");
	m_comboServers.AddString("Different file for each location");

	{
		int nServerSel = -1;
		switch (EcrmanOptions.GetFeaturesServerSettingsType())
		{
		case SERVER_SYS:	nServerSel = 0;		break;
		case SERVER_DB:		nServerSel = 1;		break;
		case SERVER_LOC:	nServerSel = 2;		break;
		}

		if (-1 == nServerSel)
		{
			m_checkServers.SetCheck(FALSE);
			m_comboServers.SetCurSel(0);
			m_checkServersTerminal.SetCheck(FALSE);
			m_checkServersPreset.SetCheck(FALSE);
			m_checkServersLogon.SetCheck(FALSE);
		}
		else
		{
			m_checkServers.SetCheck(TRUE);
			m_comboServers.SetCurSel(nServerSel);
			m_checkServersTerminal.SetCheck(EcrmanOptions.GetFeaturesServerRestrictionFlag());
			m_checkServersPreset.SetCheck(EcrmanOptions.GetFeaturesServerPresetFlag());
			m_checkServersLogon.SetCheck(EcrmanOptions.GetFeaturesServerLogonFlag());
		}
	}

	bool bCustomers = EcrmanOptions.GetFeaturesCustomerBalanceFlag();
	m_checkCustomers.SetCheck(bCustomers);

	if (FALSE == bCustomers)
	{
		m_checkCustomers2.SetCheck(FALSE);
		m_checkCustomers3.SetCheck(FALSE);
		m_checkCustomers4.SetCheck(FALSE);
		m_checkCustomers5.SetCheck(FALSE);
	}
	else
	{
		int nType = EcrmanOptions.GetFeaturesCustomerNameImportType();
		if ((nType < 0) || (nType > 2))
		{
			nType = 0;
		}

		m_checkCustomers2.SetCheck(nType != 0);
		m_checkCustomers3.SetCheck(nType == 2);

		if (EcrmanOptions.GetFeaturesCustomerOneOffAutoDeleteFlag() == TRUE)
		{
			m_checkCustomers4.SetCheck(TRUE);

			CString strNum;
			strNum.Format("%d", EcrmanOptions.GetFeaturesCustomerOneOffDelDay());
			GetEditCustomers1()->SetWindowText(strNum);
		}
		else
		{
			m_checkCustomers4.SetCheck(FALSE);
			GetEditCustomers1()->SetWindowText("");
		}

		if (EcrmanOptions.GetFeaturesCustomerAccountAutoDeleteFlag() == TRUE)
		{
			m_checkCustomers5.SetCheck(TRUE);

			CString strNum = "";
			strNum.Format("%d", EcrmanOptions.GetFeaturesCustomerAccountDelDay());
			GetEditCustomers2()->SetWindowText(strNum);
		}
		else
		{
			m_checkCustomers5.SetCheck(FALSE);
			GetEditCustomers2()->SetWindowText("");
		}
	}

	SetRecordControls();

	m_nActiveTab = 0;
	SetTabControls(FALSE);

	return TRUE;
}

/**********************************************************************/
CEdit* CPropPageOptionsFeatures::GetEditCustomers1(){ return GetEdit( IDC_EDIT_CUSTOMER1 ); }
CEdit* CPropPageOptionsFeatures::GetEditCustomers2(){ return GetEdit( IDC_EDIT_CUSTOMER2 ); }
/**********************************************************************/

void CPropPageOptionsFeatures::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	int nMaxTab = 4;
	if (SysInfo.IsEcrLiteSystem() == TRUE)
	{
		nMaxTab = 2;
	}

	m_nActiveTab = m_TabActions.GetCurSel();
	if ((m_nActiveTab < 0) || (m_nActiveTab > nMaxTab))
	{
		m_nActiveTab = 0;
	}

	SetTabControls( FALSE );
}

/**********************************************************************/

void CPropPageOptionsFeatures::SetActiveTabControls()
{
	SetTabControls( TRUE );
};

/**********************************************************************/

void CPropPageOptionsFeatures::SetTabControls(bool bActiveTabOnly)
{
	int nThisTab = m_nActiveTab;
	if (SysInfo.IsEcrLiteSystem() == TRUE)
	{
		switch (m_nActiveTab)
		{
		case 0:
			nThisTab = 0;
			break;

		case 1:
			nThisTab = 1;
			break;

		case 2:
			nThisTab = 3;
			break;

		default:
			nThisTab = 0;
			break;
		}
	}

	if ((FALSE == bActiveTabOnly) || (0 == nThisTab))
	{
		bool bStock = SysInfo.IsStockSystem();
		bool bEnable1 = (m_comboPluStock.GetCurSel() != 0);
		ShowAndEnableWindow(&m_staticGeneral1, (0 == nThisTab));
		ShowAndEnableWindow(&m_staticGeneral2, (0 == nThisTab));
		ShowAndEnableWindow(&m_staticGeneral3, (0 == nThisTab) && (FALSE == bStock));
		ShowAndEnableWindow(&m_staticGeneral4, (0 == nThisTab));
		ShowAndEnableWindow(&m_staticGeneral5, (0 == nThisTab));
		ShowAndEnableWindow(&m_comboEditing, (0 == nThisTab));
		ShowAndEnableWindow(&m_comboContext, (0 == nThisTab));
		ShowAndEnableWindow(&m_comboPluStock, (0 == nThisTab) && (FALSE == bStock));
		ShowAndEnableWindow(&m_comboDelivery, (0 == nThisTab), (bStock || bEnable1));
		ShowAndEnableWindow(&m_buttonColourBackground, (0 == nThisTab));
		ShowAndEnableWindow(&m_checkAllergen, (0 == nThisTab) && (DealerFlags.Get_LIPA_LJMU_ExportType() == 0));
		ShowAndEnableWindow(&m_checkOPNameFirst, (0 == nThisTab));
	}

	if ((FALSE == bActiveTabOnly) || (1 == nThisTab))
	{
		bool bEnable = IsTicked(m_checkMessage);
		ShowAndEnableWindow(&m_checkMessage, (1 == nThisTab));
		ShowAndEnableWindow(&m_staticMessage1, (1 == nThisTab));
		ShowAndEnableWindow(&m_buttonColourMessageBack, (1 == nThisTab), bEnable);
		ShowAndEnableWindow(&m_staticMessage2, (1 == nThisTab));
		ShowAndEnableWindow(&m_buttonColourMessageText, (1 == nThisTab), bEnable);
		ShowAndEnableWindow(&m_staticMessage3, (1 == nThisTab));
		ShowAndEnableWindow(&m_comboMessageFont, (1 == nThisTab), bEnable);
		ShowAndEnableWindow(&m_staticMessage4, (1 == nThisTab));
		ShowAndEnableWindow(&m_comboMessageSpace, (1 == nThisTab), bEnable);
		ShowAndEnableWindow(&m_buttonMessagePreview, (1 == nThisTab), bEnable);

		bEnable = IsTicked(m_checkStartupMsg);
		ShowAndEnableWindow(&m_checkStartupMsg, (1 == nThisTab));
		ShowAndEnableWindow(&m_staticStartupMsg1, (1 == nThisTab));
		ShowAndEnableWindow(&m_buttonColourStartupMsgBack, (1 == nThisTab), bEnable);
		ShowAndEnableWindow(&m_staticStartupMsg2, (1 == nThisTab));
		ShowAndEnableWindow(&m_buttonColourStartupMsgText, (1 == nThisTab), bEnable);
		ShowAndEnableWindow(&m_staticStartupMsg3, (1 == nThisTab));
		ShowAndEnableWindow(&m_comboStartupMsgFont, (1 == nThisTab), bEnable);
		ShowAndEnableWindow(&m_staticStartupMsg4, (1 == nThisTab));
		ShowAndEnableWindow(&m_comboStartupMsgSpace, (1 == nThisTab), bEnable);
		ShowAndEnableWindow(&m_buttonStartupMsgEdit, (1 == nThisTab), bEnable);
		ShowAndEnableWindow(&m_buttonStartupMsgPreview, (1 == nThisTab), bEnable);
	}

	if ((FALSE == bActiveTabOnly) || (2 == nThisTab))
	{
		bool bEnable1 = IsTicked(m_checkMixMatch2);
		bool bEnable2 = IsTicked(m_checkWalletStampRules);
		bool bEnable3 = IsTicked(m_checkTimePlans);
		ShowAndEnableWindow(&m_checkMixMatch2, (2 == nThisTab));
		ShowAndEnableWindow(&m_checkPromotions, (2 == nThisTab), bEnable1);
		ShowAndEnableWindow(&m_checkTimePlans, (2 == nThisTab), bEnable1);
		ShowAndEnableWindow(&m_checkTimePlanTemplates, (2 == nThisTab), bEnable1 && bEnable3);
		ShowAndEnableWindow(&m_checkMixMatchDownload, (2 == nThisTab), bEnable1 && bEnable3);

		bool bShowWalletStampRules = FALSE;
		switch (SysInfo.GetLoyaltyOrSmartPayType())
		{
		case LOYALTY_TYPE_LOY_V4:
		case LOYALTY_TYPE_SMP_V4:
			bShowWalletStampRules = (2 == nThisTab);
			break;
		}

		ShowAndEnableWindow(&m_checkWalletStampRules, bShowWalletStampRules);
		ShowAndEnableWindow(&m_checkGlobalWalletStampRules, bShowWalletStampRules, bEnable2);
	}

	if ((FALSE == bActiveTabOnly) || (3 == nThisTab))
	{
		bool bEcrLite = SysInfo.IsEcrLiteSystem();
		bool bEnable1 = (m_comboKeyboards.GetCurSel() != 0);
		bool bEnable2 = IsTicked(m_checkKeyboardsShare);
		ShowAndEnableWindow(&m_staticKeyboards1, (3 == nThisTab));
		ShowAndEnableWindow(&m_staticKeyboards2, (3 == nThisTab), bEnable1);
		ShowAndEnableWindow(&m_comboKeyboards, (3 == nThisTab));
		ShowAndEnableWindow(&m_checkKeyboardsText, ((3 == nThisTab) && (!bEcrLite)), bEnable1);
		ShowAndEnableWindow(&m_comboPluText, (3 == nThisTab), bEnable1);
		ShowAndEnableWindow(&m_checkKeyboardsShare, (3 == nThisTab), bEnable1);
		ShowAndEnableWindow(&m_buttonKeyboardsShare, (3 == nThisTab), bEnable1 && bEnable2);
	}

	if ((FALSE == bActiveTabOnly) || (4 == nThisTab))
	{
		bool bEnable1 = IsTicked(m_checkServers);
		bool bEnable3 = IsTicked(m_checkCustomers);
		bool bEnable4 = IsTicked(m_checkCustomers2);
		bool bEnable5 = IsTicked(m_checkCustomers4);
		bool bEnable6 = IsTicked(m_checkCustomers5);
		bool bEnable7 = IsTicked(m_checkServersLogon);
		ShowAndEnableWindow(&m_checkServers, (4 == nThisTab));
		ShowAndEnableWindow(&m_checkServersTerminal, (4 == nThisTab), bEnable1);
		ShowAndEnableWindow(&m_checkServersPreset, (4 == nThisTab), bEnable1);
		ShowAndEnableWindow(&m_checkServersLogon, (4 == nThisTab), bEnable1);
		ShowAndEnableWindow(&m_buttonMiFare, (4 == nThisTab), bEnable1 && bEnable7);
		ShowAndEnableWindow(&m_comboServers, (4 == nThisTab), bEnable1);
		ShowAndEnableWindow(&m_checkCustomers, (4 == nThisTab));
		ShowAndEnableWindow(&m_checkCustomers2, (4 == nThisTab), bEnable3);
		ShowAndEnableWindow(&m_checkCustomers3, (4 == nThisTab), (bEnable3 && bEnable4));
		ShowAndEnableWindow(&m_checkCustomers4, (4 == nThisTab), bEnable3);
		ShowAndEnableWindow(&m_staticCustomers1, (4 == nThisTab), (bEnable3 && bEnable5));
		ShowAndEnableWindow(GetEditCustomers1(), (4 == nThisTab), (bEnable3 && bEnable5));
		ShowAndEnableWindow(&m_checkCustomers5, (4 == nThisTab), bEnable3);
		ShowAndEnableWindow(&m_staticCustomers2, (4 == nThisTab), (bEnable3 && bEnable6));
		ShowAndEnableWindow(GetEditCustomers2(), (4 == nThisTab), (bEnable3 && bEnable6));
	}
}

/**********************************************************************/

void CPropPageOptionsFeatures::OnToggleCustomerAutoDelete1()
{
	if (m_checkCustomers4.GetCheck() != 0)
	{
		GetEditCustomers1()->SetWindowText("21");
	}
	else
	{
		GetEditCustomers1()->SetWindowText("");
	}

	SetActiveTabControls();
}

/**********************************************************************/

void CPropPageOptionsFeatures::OnToggleCustomerAutoDelete2()
{
	if (m_checkCustomers5.GetCheck() != 0)
	{
		GetEditCustomers2()->SetWindowText("21");
	}
	else
	{
		GetEditCustomers2()->SetWindowText("");
	}

	SetActiveTabControls();
}

/**********************************************************************/

void CPropPageOptionsFeatures::OnButtonMiFare()
{
	CSetupReaderDlg dlg(this);
	dlg.DoModal();
}

/**********************************************************************/

BOOL CPropPageOptionsFeatures::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOptionsFeatures::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageOptionsFeatures::GetRecordData()
{
}

/**********************************************************************/

void CPropPageOptionsFeatures::SaveRecord()
{
	EcrmanOptions.SetFeaturesEposDeliveryType(m_comboDelivery.GetCurSel());

	{
		int nType = 0;
		switch (m_comboEditing.GetCurSel())
		{
		case 1:	
			nType = 2;	
			break;
		}

		EcrmanOptions.SetFeaturesDBaseEditType(nType);
	}

	EcrmanOptions.SetFeaturesContextForceNowFlag(m_comboContext.GetCurSel() == 1);
	EcrmanOptions.SetFeaturesAllergenFlag(m_checkAllergen.GetCheck() == 1);
	EcrmanOptions.SetFeaturesOPNameFirstFlag(m_checkOPNameFirst.GetCheck() == 1);

	int nKeyboards = m_comboKeyboards.GetCurSel();
	if (0 == nKeyboards)
	{
		EcrmanOptions.SetFeaturesKeyboardFlag(FALSE);
		EcrmanOptions.SetFeaturesKeyboardsPerLocation(1);
	}
	else
	{
		EcrmanOptions.SetFeaturesKeyboardFlag(TRUE);
		EcrmanOptions.SetFeaturesKeyboardsPerLocation(nKeyboards);
	}

	EcrmanOptions.SetFeaturesKeyboardFlag(m_comboKeyboards.GetCurSel() != 0);
	EcrmanOptions.SetFeaturesKeyboardPriceSetFlag(IsTicked(m_checkKeyboardsText));
	EcrmanOptions.SetFeaturesKeyboardShareFlag(IsTicked(m_checkKeyboardsShare));
	EcrmanOptions.SetFeaturesKeyboardTextOrder(m_comboPluText.GetCurSel());

	if (SysInfo.IsEcrLiteSystem() == TRUE)
	{
		EcrmanOptions.SetFeaturesKeyboardPriceSetFlag(FALSE);
		EcrmanOptions.SetFeaturesKeyboardShareFlag(FALSE);
	}

	EcrmanOptions.SetFeaturesMixMatchFlag(IsTicked(m_checkMixMatch2));
	EcrmanOptions.SetFeaturesTimePlanDownloadFlag((IsTicked(m_checkMixMatchDownload)) && (IsTicked(m_checkMixMatch2)));
	EcrmanOptions.SetFeaturesPromotionFlag((IsTicked(m_checkPromotions)) && (IsTicked(m_checkMixMatch2)));
	EcrmanOptions.SetFeaturesTimePlanFlag((IsTicked(m_checkTimePlans)) && (IsTicked(m_checkMixMatch2)));
	EcrmanOptions.SetFeaturesTimePlanTemplateFlag((IsTicked(m_checkTimePlanTemplates)) && (IsTicked(m_checkMixMatch2)) && (IsTicked(m_checkTimePlans)) && (IsTicked(m_checkTimePlanTemplates)));

	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_LOY_V4:
	case LOYALTY_TYPE_SMP_V4:
		EcrmanOptions.SetFeaturesWalletStampFlag(IsTicked(m_checkWalletStampRules));
		EcrmanOptions.SetFeaturesGlobalWalletStampFlag(IsTicked(m_checkGlobalWalletStampRules));
		break;

	default:
		EcrmanOptions.SetFeaturesWalletStampFlag(FALSE);
		EcrmanOptions.SetFeaturesGlobalWalletStampFlag(FALSE);
		break;
	}

#ifndef STOCKMAN_SYSTEM
	EcrmanOptions.SetFeaturesPluStockFlag(m_comboPluStock.GetCurSel() != 0);
	EcrmanOptions.SetFeaturesPluAuditFlag(m_comboPluStock.GetCurSel() == 2);
#endif

	if (m_checkServers.GetCheck() == 0)
	{
		EcrmanOptions.SetFeaturesServerSettingsType(0);
		EcrmanOptions.SetFeaturesServerRestrictionFlag(FALSE);
		EcrmanOptions.SetFeaturesServerPresetFlag(FALSE);
		EcrmanOptions.SetFeaturesServerLogonFlag(FALSE);
	}
	else
	{
		EcrmanOptions.SetFeaturesServerSettingsType(m_comboServers.GetCurSel() + 1);
		EcrmanOptions.SetFeaturesServerRestrictionFlag(IsTicked(m_checkServersTerminal));
		EcrmanOptions.SetFeaturesServerPresetFlag(IsTicked(m_checkServersPreset));
		EcrmanOptions.SetFeaturesServerLogonFlag(IsTicked(m_checkServersLogon));
	}

	bool bCustomers = (m_checkCustomers.GetCheck() != 0);
	EcrmanOptions.SetFeaturesCustomerBalanceFlag(bCustomers);

	int nType = 0;
	if ((TRUE == bCustomers) && (IsTicked(m_checkCustomers2)))
	{
		if (IsTicked(m_checkCustomers3))
		{
			nType = 2;
		}
		else
		{
			nType = 1;
		}
	}

	EcrmanOptions.SetFeaturesCustomerNameImportType(nType);

	if (IsTicked(m_checkCustomers4))
	{
		CString strNum = "";
		GetEditCustomers1()->GetWindowText(strNum);
		EcrmanOptions.SetFeaturesCustomerOneOffDelDay(atoi(strNum));
	}
	else
	{
		EcrmanOptions.SetFeaturesCustomerOneOffDelDay(0);
	}

	if (IsTicked(m_checkCustomers5))
	{
		CString strNum = "";
		GetEditCustomers2()->GetWindowText(strNum);
		EcrmanOptions.SetFeaturesCustomerAccountDelDay(atoi(strNum));
	}
	else
	{
		EcrmanOptions.SetFeaturesCustomerAccountDelDay(0);
	}

	//CUSTOM MESSAGE BOX
	EcrmanOptions.SetCustomMessageBoxFlag(m_checkMessage.GetCheck() != 0);

	switch (m_comboMessageFont.GetCurSel())
	{
	case 1:		EcrmanOptions.SetCustomMessageBoxFont(10);	break;
	case 2:		EcrmanOptions.SetCustomMessageBoxFont(11);	break;
	case 3:		EcrmanOptions.SetCustomMessageBoxFont(12);	break;
	case 4:		EcrmanOptions.SetCustomMessageBoxFont(14);	break;
	case 0:
	default:	EcrmanOptions.SetCustomMessageBoxFont(8);	break;
	}

	EcrmanOptions.SetCustomMessageBoxLineGap(5 + m_comboMessageSpace.GetCurSel());

	//STARTUP MESSAGE BOX
	EcrmanOptions.SetStartupMessageBoxFlag(m_checkStartupMsg.GetCheck() != 0);

	switch (m_comboStartupMsgFont.GetCurSel())
	{
	case 1:		EcrmanOptions.SetStartupMessageBoxFont(10);	break;
	case 2:		EcrmanOptions.SetStartupMessageBoxFont(11);	break;
	case 3:		EcrmanOptions.SetStartupMessageBoxFont(12);	break;
	case 4:		EcrmanOptions.SetStartupMessageBoxFont(14);	break;
	case 0:
	default:	EcrmanOptions.SetStartupMessageBoxFont(8);	break;
	}

	EcrmanOptions.SetStartupMessageBoxLineGap(5 + m_comboStartupMsgSpace.GetCurSel());
}

/**********************************************************************/

void CPropPageOptionsFeatures::SetRecordControls()
{
}

/**********************************************************************/

void CPropPageOptionsFeatures::OnSetupKeyboardsShare()
{
	CPropertySheetKeyboardSets propSheet;
	propSheet.DoModal();
	propSheet.SaveSystem();
}

/**********************************************************************/

void CPropPageOptionsFeatures::OnButtonBackground()
{
	int nNewColour = EcrmanOptions.GetFeaturesBackgroundColour();
	CColourEditDlg dlg( nNewColour, "Set Main Window Background Colour", this, FALSE, this );
	dlg.SetSuggestedColour( 16, 0x80C080 );

	m_nEditingColour = 1;
	if (dlg.DoModal() == IDOK)
	{
		nNewColour = dlg.GetColourRef();
	}

	EcrmanOptions.SetFeaturesBackgroundColour( nNewColour );
	
	m_buttonColourBackground.m_ColourRefBack = nNewColour;
	m_buttonColourBackground.Invalidate();
	
	m_pMainWnd -> Invalidate();
}

/**********************************************************************/

void CPropPageOptionsFeatures::OnButtonMessageBack()
{
	int nNewColour = EcrmanOptions.GetCustomMessageBoxColourBackInt();
	CColourEditDlg dlg( nNewColour, "Set Message Box Background Colour", this, FALSE, this );
	dlg.SetSuggestedColour( 16, 0xFFFFFF );

	m_nEditingColour = 2;
	if (dlg.DoModal() == IDOK)
	{
		nNewColour = dlg.GetColourRef();
	}
	
	EcrmanOptions.SetCustomMessageBoxColourBackInt( nNewColour );
	
	m_buttonColourMessageBack.m_ColourRefBack = nNewColour;
	m_buttonColourMessageBack.Invalidate();
	
	m_pMainWnd -> Invalidate();
}

/**********************************************************************/

void CPropPageOptionsFeatures::OnButtonMessageText()
{
	int nNewColour = EcrmanOptions.GetCustomMessageBoxColourTextInt();
	CColourEditDlg dlg( nNewColour, "Set Message Box Text Colour", this, FALSE, this );
	dlg.SetSuggestedColour( 16, 0x0 );

	m_nEditingColour = 3;
	if (dlg.DoModal() == IDOK)
	{
		nNewColour = dlg.GetColourRef();
	}
	
	EcrmanOptions.SetCustomMessageBoxColourTextInt( nNewColour );
	
	m_buttonColourMessageText.m_ColourRefBack = nNewColour;
	m_buttonColourMessageText.Invalidate();
	
	m_pMainWnd -> Invalidate();
}

/**********************************************************************/

void CPropPageOptionsFeatures::OnButtonStartupMsgBack()
{
	int nNewColour = EcrmanOptions.GetStartupMessageBoxColourBackInt();
	CColourEditDlg dlg(nNewColour, "Set Startup Message Background Colour", this, FALSE, this);
	dlg.SetSuggestedColour(16, 0xFFFFFF);

	m_nEditingColour = 4;
	if (dlg.DoModal() == IDOK)
	{
		nNewColour = dlg.GetColourRef();
	}

	EcrmanOptions.SetStartupMessageBoxColourBackInt(nNewColour);

	m_buttonColourStartupMsgBack.m_ColourRefBack = nNewColour;
	m_buttonColourStartupMsgBack.Invalidate();

	m_pMainWnd->Invalidate();
}

/**********************************************************************/

void CPropPageOptionsFeatures::OnButtonStartupMsgText()
{
	int nNewColour = EcrmanOptions.GetStartupMessageBoxColourTextInt();
	CColourEditDlg dlg(nNewColour, "Set Startup Message Text Colour", this, FALSE, this);
	dlg.SetSuggestedColour(16, 0x0);

	m_nEditingColour = 5;
	if (dlg.DoModal() == IDOK)
	{
		nNewColour = dlg.GetColourRef();
	}

	EcrmanOptions.SetStartupMessageBoxColourTextInt(nNewColour);

	m_buttonColourStartupMsgText.m_ColourRefBack = nNewColour;
	m_buttonColourStartupMsgText.Invalidate();

	m_pMainWnd->Invalidate();
}

/**********************************************************************/

long CPropPageOptionsFeatures::OnColourButtonMessage( WPARAM w, LPARAM l )
{
	switch( m_nEditingColour )
	{
	case 1:
		m_buttonColourBackground.m_ColourRefBack = l;
		m_buttonColourBackground.Invalidate();
		EcrmanOptions.SetFeaturesBackgroundColour(l);
		m_pMainWnd -> Invalidate();
		break;

	case 2:
		m_buttonColourMessageBack.m_ColourRefBack = l;
		m_buttonColourMessageBack.Invalidate();
		EcrmanOptions.SetCustomMessageBoxColourBackInt(l);
		break;

	case 3:
		m_buttonColourMessageText.m_ColourRefBack = l;
		m_buttonColourMessageText.Invalidate();
		EcrmanOptions.SetCustomMessageBoxColourTextInt(l);
		break;

	case 4:
		m_buttonColourStartupMsgBack.m_ColourRefBack = l;
		m_buttonColourStartupMsgBack.Invalidate();
		EcrmanOptions.SetStartupMessageBoxColourBackInt(l);
		break;

	case 5:
		m_buttonColourStartupMsgText.m_ColourRefBack = l;
		m_buttonColourStartupMsgText.Invalidate();
		EcrmanOptions.SetStartupMessageBoxColourTextInt(l);
		break;
	}

	return 0l;
}

/**********************************************************************/

void CPropPageOptionsFeatures::OnButtonMessagePreview()
{
	int nFontSize = 8;
	switch( m_comboMessageFont.GetCurSel() )
	{
	case 1:		nFontSize = 10;	break;
	case 2:		nFontSize = 11;	break;
	case 3:		nFontSize = 12;	break;
	case 4:		nFontSize = 14;	break;
	case 0:	
	default:	nFontSize = 8;	break;
	}

	CMyMessageBoxInfo infoMessage;
	infoMessage.SetIconType( MB_ICONINFORMATION );
	infoMessage.SetButtonMode( MB_OK );
	infoMessage.SetLineGap( 5 + m_comboMessageSpace.GetCurSel() );
	infoMessage.SetFontSize( nFontSize );
	infoMessage.SetBackColour( EcrmanOptions.GetCustomMessageBoxColourBackInt() );
	infoMessage.SetTextColour( EcrmanOptions.GetCustomMessageBoxColourTextInt() );
	infoMessage.SetTitle( AfxGetAppName() );
	infoMessage.SetTexts( "This is an example message box\nwith three lines of text\nautomatically sized to fit the longest line" );	
	CMyMessageBoxDlg dlg( infoMessage, this );
	dlg.DoModal();
}

/**********************************************************************/

void CPropPageOptionsFeatures::OnButtonStartupMsgEdit()
{
	CDefEditStartupMsgDlg dlg;
	dlg.DoModal();
}

/**********************************************************************/

void CPropPageOptionsFeatures::OnButtonStartupMsgPreview()
{
	int nFontSize = 8;
	switch (m_comboStartupMsgFont.GetCurSel())
	{
	case 1:		nFontSize = 10;	break;
	case 2:		nFontSize = 11;	break;
	case 3:		nFontSize = 12;	break;
	case 4:		nFontSize = 14;	break;
	case 0:
	default:	nFontSize = 8;	break;
	}

	CMyMessageBoxInfo infoMessage;
	infoMessage.SetIconType(MB_ICONINFORMATION);
	infoMessage.SetButtonMode(MB_OK);
	infoMessage.SetLineGap(5 + m_comboStartupMsgSpace.GetCurSel());
	infoMessage.SetFontSize(nFontSize);
	infoMessage.SetBackColour(EcrmanOptions.GetStartupMessageBoxColourBackInt());
	infoMessage.SetTextColour(EcrmanOptions.GetStartupMessageBoxColourTextInt());
	infoMessage.SetTitle(AfxGetAppName());

	CStringArray arrayTexts;

	CSSFile fileMessage;
	CFilenameUpdater FnUp(SysFiles::StartupMessage);
	if (fileMessage.Open(FnUp.GetFilenameToUse(), "rb") == TRUE)
	{
		CString strText = "";
		while ((fileMessage.ReadString(strText) == TRUE) && (arrayTexts.GetSize() < MAX_STARTUPMSG_LINES))
		{
			arrayTexts.Add(strText.Left(MAX_STARTUPMSG_LINELENGTH));
		}
	}

	if (arrayTexts.GetSize() == 0)
	{
		arrayTexts.Add("Example text for system startup message box.");
		arrayTexts.Add("Please click Edit Message to view and edit the message text.");
	}

	infoMessage.SetTexts(arrayTexts);
	CMyMessageBoxDlg dlg(infoMessage, this);
	dlg.DoModal();
}

/**********************************************************************/

void CPropPageOptionsFeatures::OnToggleWalletStampRule()
{
	m_checkGlobalWalletStampRules.EnableWindow(IsTicked(m_checkWalletStampRules));
}

/**********************************************************************/
