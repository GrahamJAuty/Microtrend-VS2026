/**********************************************************************/
#include "..\CommonEcrStockTray\SysInfo.h"
/**********************************************************************/
#include "SPOSVersions.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "NetworkSPOSVersionArray.h"
/**********************************************************************/
extern const char* szVERSION_ABOUT;
/**********************************************************************/

CNetworkSPOSVersionInfo::CNetworkSPOSVersionInfo()
{
	Reset();
}

/**********************************************************************/

void CNetworkSPOSVersionInfo::Reset()
{
	m_nSPOSVersion = 0;
	m_strVersion = "";
	m_strCashRSP = "";
	m_strFeatures = "";
}

/**********************************************************************/

CNetworkSPOSVersionArray::CNetworkSPOSVersionArray(void)
{
	AddSPOSVersion(SPOS_V4, "V4.1.183", "", "Original SPOS V4");
	AddSPOSVersion(SPOS_V4_210, "V4.1.210", "", "Mix Match Free Bucket");
	AddSPOSVersion(SPOS_V4_321, "V4.1.321", "", "Mix Match Override, Plu Tracking");
	AddSPOSVersion(SPOS_V4_389, "V4.1.389", "", "EAN Number on Receipt");
	AddSPOSVersion(SPOS_V4_423, "V4.1.423", "", "Promo on Promo");
	AddSPOSVersion(SPOS_V4_425, "V4.1.425", "", "Mix Match by Plu 2");
	AddSPOSVersion(SPOS_V4_494, "V4.1.494", "", "Plu Free Text with Description");
	AddSPOSVersion(SPOS_V4_614, "V4.1.614", "", "Mix Match offers by Seat");
	AddSPOSVersion(SPOS_V4_634, "V4.1.634", "", "Link Plu, Payment Cash Limit Flag");
	AddSPOSVersion(SPOS_V4_682, "V4.1.682", "", "BOGOF Options for Offer Group");

	AddSPOSVersion(SPOS_V4_719, "V4.1.719", "", "Age Verification 16, 18, 21");
	AddSPOSVersion(SPOS_V4_753, "V4.1.753", "", "Age Verification 25");
	AddSPOSVersion(SPOS_V4_780, "V4.1.780", "", "Vouchers as change for Deposits");
	AddSPOSVersion(SPOS_V4_827, "V4.1.827", "", "Learning PLU, 999 Mix Match offers");
	AddSPOSVersion(SPOS_V4_837, "V4.1.837", "", "Payment Type Charge to Account");
	AddSPOSVersion(SPOS_V4_861, "V4.1.861", "", "Handheld Export Requests");
	AddSPOSVersion(SPOS_V4_958, "V4.1.958", "", "Default Text for List Keys");
	AddSPOSVersion(SPOS_V4_992, "V4.1.992", "", "Set Bucket Price, Download Plu Changes Only");
	AddSPOSVersion(SPOS_V4_999, "V4.1.999", "V13", "Information option for Plu Type");
	AddSPOSVersion(SPOS_V4_1071, "V4.1.1071", "V13", "10 Plu Prices, Timeplan mode for Promo");

	AddSPOSVersion(SPOS_V4_1092, "V4.1.1092", "V14", "Mix Match use gate, List Stack Mode");
	AddSPOSVersion(SPOS_V4_1096, "V4.1.1096", "V14", "Promotion use gate");
	AddSPOSVersion(SPOS_V4_1124, "V4.1.1124", "V15", "9999 Servers");
	AddSPOSVersion(SPOS_V4_1216, "V4.1.1216", "V17", "Plu Key Text");
	AddSPOSVersion(SPOS_V4_1300, "V4.1.1300", "V18", "Plu and Payment link to external program");
	AddSPOSVersion(SPOS_V4_1392, "V4.1.1392", "V19", "Deferred subtotal promo, Payment Account Allow");
	AddSPOSVersion(SPOS_V4_1510, "V4.1.1510", "V20", "Table area in List Headers");
	AddSPOSVersion(SPOS_V4_1519, "V4.1.1519", "V21", "Plu Type Loyalty Top-up, CashRSP Application Type");
	AddSPOSVersion(SPOS_V4_1522, "V4.1.1522", "V21", "Full Product option for Modifier List Items");
	AddSPOSVersion(SPOS_V4_1525, "V4.1.1525", "V21", "Plu KP Consolidate Top Item");
	AddSPOSVersion(SPOS_V4_1734, "V4.1.1734", "V21", "Department Tickets Mode");
	AddSPOSVersion(SPOS_V4_1901, "V4.1.1901", "V21", "Promotion Filter Type");
	AddSPOSVersion(SPOS_V4_1977, "V4.1.1977", "V21", "Payment Methods Cash Declaration Flag");
	AddSPOSVersion(SPOS_V4_2012, "V4.1.2012", "V21", "Plu Type External");
	AddSPOSVersion(SPOS_V4_2067, "V4.1.2067", "V21", "Server Restrictions");
	AddSPOSVersion(SPOS_V4_2133, "V4.1.2133", "V22", "99 Analysis Categories");
	AddSPOSVersion(SPOS_V4_2161, "V4.1.2161", "V22", "Flexible Promotions");
	AddSPOSVersion(SPOS_V4_2455, "V4.1.2455", "V26", "Promotion Reasons, TimePlan Operate Gate");
	AddSPOSVersion(SPOS_V4_2606, "V4.1.2606", "V28", "Allergens");
	AddSPOSVersion(SPOS_V4_2_369, "V4.2.369", "V35", "20 Paid In / Out Texts");
	AddSPOSVersion(SPOS_V4_2_631, "V4.2.631", "V39", "Payment Methods Change as Cashback Flag");
	AddSPOSVersion(SPOS_V4_2_714, "V4.2.714", "V40", "Department & Plu Images, 8,10,12x5 lists");
	AddSPOSVersion(SPOS_V4_2_919, "V4.2.919", "V40", "TimePlan Screen Modes");
	AddSPOSVersion(SPOS_V4_2_959, "V4.2.959", "V40", "Plu KP Not Alone Flag");
	AddSPOSVersion(SPOS_V4_2_1081, "V4.2.1081", "V42", "12 VAT Bands");
	AddSPOSVersion(SPOS_V4_2_1087, "V4.2.1087", "V42", "16 Order Printers");
	AddSPOSVersion(SPOS_V4_2_1384, "V4.2.1384", "V42", "Mix Match Autolock");
	AddSPOSVersion(SPOS_V4_2_1456, "V4.2.1456", "V42", "Plu Type Loyalty Staff Gift");
	AddSPOSVersion(SPOS_V4_2_1530, "V4.2.1530", "V43", "Loyalty Stamp Offers");
	AddSPOSVersion(SPOS_V4_2_1579, "V4.2.1579", "V43", "SmartPay Purchase Controls");
	AddSPOSVersion(SPOS_V4_2_1605, "V4.2.1605", "V43", "Sell Tied Items Flag for Link PLUs");
	AddSPOSVersion(SPOS_V4_2_1618, "V4.2.1618", "V44", "UpTo Flag for Markdown Promotions");
	AddSPOSVersion(SPOS_V4_2_1622, "V4.2.1622", "V44", "Server Trading Colour Scheme");
	AddSPOSVersion(SPOS_V4_3_18, "V4.3.18", "V44", "Mix Match Current Price Band Flag");
	AddSPOSVersion(SPOS_V4_3_93, "V4.3.93", "V44", "Voucher Card Payment Method");
	AddSPOSVersion(SPOS_V4_3_104, "V4.3.104", "V44", "Plu Account Required Flag");

}

/**********************************************************************/

void CNetworkSPOSVersionArray::AddSPOSVersion( int nVersion, const char* szVersion, const char* szCashRSP, const char* szFeatures )
{
	CNetworkSPOSVersionInfo info;
	info.m_nSPOSVersion = nVersion;
	info.m_strVersion = szVersion;
	info.m_strCashRSP = szCashRSP;
	info.m_strFeatures = szFeatures;
	m_arrayVersions.Add( info );
}

/**********************************************************************/

void CNetworkSPOSVersionArray::GetAt( int n, CNetworkSPOSVersionInfo& info )
{
	info.Reset();

	if ( ( n >= 0 ) && ( n < m_arrayVersions.GetSize() ) )
		info = m_arrayVersions.GetAt( n );
}

/**********************************************************************/

const char* CNetworkSPOSVersionArray::GetSPOSVersionText( int nSPOSVersion )
{
	m_strVersionText = "Unknown";

	for ( int n = 0; n < m_arrayVersions.GetSize(); n++ )
	{
		if ( m_arrayVersions[n].m_nSPOSVersion == nSPOSVersion )
		{
			m_strVersionText = "SPos ";
			m_strVersionText += m_arrayVersions[n].m_strVersion + " ";

			break;
		}
	}

	return m_strVersionText;
}

/**********************************************************************/
