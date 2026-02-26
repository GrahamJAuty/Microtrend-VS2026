//*******************************************************
#include "..\SPosLoyalty4Shared\SQLTable_EcrmanDatabase\SQLRepositoryEcrmanDatabase.h"
#include "..\SPosLoyalty4Shared\SQLWhereBuilder.h"
//*******************************************************
#include "DbExportHandler.h"
//*******************************************************
#include "StandardCombos.h"
//*******************************************************

CStandardCombos::CStandardCombos(void)
{
}

//*******************************************************

void CStandardCombos::AddGenderTexts( CSSComboBox& combo, bool bIgnore )
{
	combo.ResetContent();

	if (TRUE == bIgnore)
	{
		combo.AddString("Ignore");
	}

	combo.AddString( "Unknown" );
	combo.AddString( "Male" );
	combo.AddString( "Female" );
}

//*******************************************************
//*******************************************************
//*******************************************************

void CStandardCombos::FillGenderCombo( CSSComboBox& combo, int nSel, bool bIgnore )
{
	AddGenderTexts( combo, bIgnore );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillIgnoreSelectCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Ignore" );
	combo.AddString( "Select" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillNoYesCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "No" );
	combo.AddString( "Yes" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillAutoRefreshCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "No auto refresh" );
	combo.AddString( "Daily" );
	combo.AddString( "Weekly" );
	combo.AddString( "Monthly" );
	combo.AddString( "Yearly" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillMemberTypeCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Standard" );
	combo.AddString( "Special Needs" );
	combo.AddString( "Free Member" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillDailySpendCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Single Daily Limit" );
	combo.AddString( "Record Specific" );
	combo.AddString( "Use Audit Period Settings" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillPCBonusCombo(CSSComboBox& combo, int nSel)
{
	CString strBT = "Use Topup ";

	combo.ResetContent();
	combo.AddString("None");

	for (int n = 1; n <= REVALUE_PLUNO_PERPURSE_COUNT; n++)
	{
		CString strTrip = "";
		strTrip.Format("%s%d", (const char*) strBT, n);
		combo.AddString(strTrip);
	}

	SetComboSel(combo, nSel);
}

//*******************************************************

void CStandardCombos::FillFieldNamesCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Database Fields" );
	combo.AddString( "Audit Periods" );
	combo.AddString( "Alert Texts" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillStartOfWeekCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Sunday" );
	combo.AddString( "Monday" );
	combo.AddString( "Tuesday" );
	combo.AddString( "Wednesday" );
	combo.AddString( "Thursday" );
	combo.AddString( "Friday" );
	combo.AddString( "Saturday" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillEditPageCombo(CSSComboBox& combo, int nSel)
{
	combo.ResetContent();
	combo.AddString("Details");
	combo.AddString("Balances");
	SetComboSel(combo, nSel);
}

//*******************************************************

void CStandardCombos::FillFormatCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Format1" );
	combo.AddString( "Format2" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillExtRefCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "MiFare" );
	//FIDDLE combo.AddString( "Paxton" );
	//FIDDLE combo.AddString( "Innerrange" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillRenewTypeCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Day" );
	combo.AddString( "Week" );
	combo.AddString( "Month" );
	combo.AddString( "Annual" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::SetComboSel( CSSComboBox& combo, int nSel, int nDefSel )
{
	if ( ( nSel >= 0 ) && ( nSel < combo.GetCount() ) )
		combo.SetCurSel( nSel );
	else if ( ( nDefSel >= 0 ) && ( nDefSel < combo.GetCount() ) )
		combo.SetCurSel( nDefSel );
}

//*******************************************************
//*******************************************************
//*******************************************************

void CStandardCombos::FillGenderCombo( CSSComboBox& combo, const char* sz )
{
	AddGenderTexts( combo );
	SelectComboString( combo, sz );
}

//*******************************************************

void CStandardCombos::FillAndOrCombo( CSSComboBox& combo, const char* sz )
{
	combo.ResetContent();
	combo.AddString( szAND );
	combo.AddString( szOR );
	SelectComboString( combo, sz );
}

//*******************************************************

void CStandardCombos::FillConditionCombo( CSSComboBox& combo, const char* sz )
{
	combo.ResetContent();
	combo.AddString( szDONTCARE );
	combo.AddString( szLESSTHAN );
	combo.AddString( szGREATER );
	combo.AddString( szEQUAL );
	combo.AddString( szNOTEQUAL );
	SelectComboString( combo, sz );
}

//*******************************************************

void CStandardCombos::FillBaudCombo( CSSComboBox& combo, const char* sz )
{
	combo.ResetContent();
	combo.AddString( "9600" );
	combo.AddString( "19200" );
	combo.AddString( "38400" );
	combo.AddString( "57600" );
	SelectComboString( combo, sz );
}

//*******************************************************

void CStandardCombos::FillPortCombo( CSSComboBox& combo, const char* sz )
{
	combo.ResetContent();
	
	for ( int n = 1; n <= 16; n++ )
	{
		CString str;
		str.Format( "COM%d",n );
		combo.AddString( str );
	}

	SelectComboString( combo, sz );
}

//*******************************************************

void CStandardCombos::SelectComboString( CSSComboBox& combo, const char* sz )
{
	if ( combo.SelectString( -1, sz ) == CB_ERR )
		combo.SetWindowText(sz);
}

//*******************************************************

void CStandardCombos::SelectComboStringWithDefaultPos(CSSComboBox& combo, const char* sz, int nPos)
{
	if (combo.SelectString(-1, sz) == CB_ERR)
	{
		if ((nPos >= 0) && (nPos < combo.GetCount()))
		{
			combo.SetCurSel(nPos);
		}
		else if (combo.GetCount() > 0)
		{
			combo.SetCurSel(0);
		}
	}
}

//*******************************************************

void CStandardCombos::FillDatabaseCombo(CSSTabbedComboBox& combo, bool bHasAll, int nDbNo)
{
	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(50);
	combo.SetTabStops(arrayStops);

	CStringArray arrayDbNames;
	CSQLRepositoryEcrmanDatabase repoDb;
	repoDb.GetDatabaseNameList(arrayDbNames, NULL);

	if (System.GetMaxEcrmanDatabases() == 1)
	{
		bHasAll = FALSE;
	}
	
	if (TRUE == bHasAll)
	{
		combo.AddItem("--,All");
		combo.SetItemData(0, 0);
	}

	int nMaxDb = min(arrayDbNames.GetSize(), System.GetMaxEcrmanDatabases());
	for (int n = 0; n < nMaxDb; n++)
	{
		CString str = "";
		str.Format("%2.2d,%s",
			n + 1,
			(const char*)arrayDbNames.GetAt(n));
	
		combo.AddItem(str);
		combo.SetItemData(n + 1, n + 1);
	}

	int nSel = nDbNo;

	if (FALSE == bHasAll)
	{
		nSel--;
	}

	if ((nSel < 0) || (nSel >= combo.GetCount()))
	{
		nSel = 0;
	}
	
	combo.SetCurSel(nSel);
}

//*******************************************************

void CStandardCombos::FillFolderSetCombo(CSSTabbedComboBox& combo, int nDbNo, int nTargetSetNo)
{
	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(50);
	combo.SetTabStops(arrayStops);

	combo.ResetContent();
	combo.AddItem("--,All");
	combo.SetItemData(0, 0);

	if (nDbNo > 0)
	{
		CServerDataFolderSetIndex FolderSetIndex;
		FolderSetIndex.m_nDbNo = nDbNo;

		for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
		{
			FolderSetIndex.m_nSetNo = nSetNo;

			CString strName = Server.GetEposPathLabel(FolderSetIndex);
			TrimSpacesFromString(strName);

			if (strName == "")
			{
				strName.Format("Set %d", nSetNo);
			}

			CString strComboLine = "";
			strComboLine.Format("%2.2d,%s",
				nSetNo,
				(const char*)strName);

			combo.AddItem(strComboLine);
			combo.SetItemData(nSetNo, nSetNo);
		}
	}

	int nSel = nTargetSetNo;

	if ((nSel < 0) || (nSel >= combo.GetCount()))
	{
		nSel = 0;
	}

	combo.SetCurSel(nSel);
	combo.EnableWindow(nDbNo != 0);
}

//*******************************************************
