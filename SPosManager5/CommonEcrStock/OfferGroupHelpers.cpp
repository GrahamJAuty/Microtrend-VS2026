/**********************************************************************/
#include "NewRecordNumberDlg.h" 
#include "PropertySheetOfferGroup.h"
/**********************************************************************/
#include "OfferGroupHelpers.h"
/**********************************************************************/

void COfferGroupHelpers::FillOfferGroupCombo( CSSComboBox& combo, int nOfferGroup )
{
	combo.ResetContent();

	combo.AddString( "None" );
	combo.SetItemData( 0, 0 );

	int nIdx = -1;
	
	if ( nOfferGroup == 0 )
		nIdx = 0;

	for ( int n = 0; n < DataManager.OfferGroup.GetSize(); n++ )
	{
		COfferGroupsCSVRecord Offer( NODE_OFFER_GROUP );
		DataManager.OfferGroup.GetAt( n, Offer );
		int nGroupNo = Offer.GetGroupNo();

		CString strName;
		strName.Format ( "G%2.2d %s",
			nGroupNo,
			Offer.GetDisplayName() );

		combo.AddString( strName );
		combo.SetItemData( n + 1, nGroupNo );

		if ( nGroupNo == nOfferGroup )
			nIdx = n + 1;
	}

	if ( nIdx == -1 )
	{
		CString strName;
		strName.Format ( "G%2.2d, Offer Group %d",
			nOfferGroup,
			nOfferGroup );

		combo.InsertString( 1, strName );
		combo.SetItemData( 1, nOfferGroup );

		nIdx = 1;
	}

	combo.SetCurSel( nIdx );
}

/**********************************************************************/

int COfferGroupHelpers::GetOfferGroup( CSSComboBox& combo )
{
	int nSel = combo.GetCurSel();

	if ( ( nSel >= 0 ) && ( nSel < combo.GetCount() ) )
		return combo.GetItemData(nSel);
	else
		return 0;
}

/**********************************************************************/

bool COfferGroupHelpers::EditOfferGroupInternal( bool bWalletStampRule, CSSComboBox& combo, CWnd* pParent ) 
{
	int nSel = combo.GetCurSel();
	if (nSel < 0 || nSel >= combo.GetCount())
	{
		return FALSE;
	}

	int nOldOffer = combo.GetItemData( nSel );

	if ( nOldOffer == 0 )
	{
		int nNewOffer = DataManager.OfferGroup.FindFirstFreeNumber();

		if ( nNewOffer < 1 )
		{
			Prompter.Error ( "All available offer groups are already in use." );
			return FALSE;
		}

		CNewRecordNumberDlg dlg ( NODE_OFFER_GROUP, pParent );
		dlg.m_nNumber = nNewOffer;
		if (dlg.DoModal() != IDOK)
		{
			return FALSE;
		}

		int nOfferIdx;
		nNewOffer = dlg.m_nNumber;
		if ( DataManager.OfferGroup.FindTableByNumber ( nNewOffer, nOfferIdx ) )
		{
			Prompter.Error ( "The selected offer group is already in use." );
			return FALSE;
		}

		COfferGroupsCSVRecord Offer( NODE_OFFER_GROUP );
		Offer.SetGroupNo( nNewOffer );
		Offer.SetGroupName ( "" );
		DataManager.OfferGroup.InsertAt ( nOfferIdx, Offer );

		CPropertySheetOfferGroup propSheet( IDC_SPIN, nOfferIdx, FALSE, bWalletStampRule, pParent );
		if (propSheet.DoModal() == IDOK)
		{
			propSheet.SaveRecord();
		}

		FillOfferGroupCombo( combo, nNewOffer );
	}
	else
	{
		int nOfferIdx;
		if ( DataManager.OfferGroup.FindTableByNumber( nOldOffer, nOfferIdx ) == TRUE )
		{
			CPropertySheetOfferGroup propSheet( IDC_SPIN, nOfferIdx, FALSE, bWalletStampRule, pParent );
			if ( propSheet.DoModal() == IDOK )
			{
				propSheet.SaveRecord();
				FillOfferGroupCombo( combo, nOldOffer );
			}
		}
		else
		{
			if ( nOldOffer >= 1 && nOldOffer <= OfferGroupEntries::GroupNo.Max )
			{
				COfferGroupsCSVRecord Offer( NODE_OFFER_GROUP );
				Offer.SetGroupNo( nOldOffer );
				Offer.SetGroupName ( "" );
				DataManager.OfferGroup.InsertAt ( nOfferIdx, Offer );

				CPropertySheetOfferGroup propSheet( IDC_SPIN, nOfferIdx, FALSE, bWalletStampRule, pParent );
				if (propSheet.DoModal() == IDOK)
				{
					propSheet.SaveRecord();
				}

				FillOfferGroupCombo( combo, nOldOffer );
			}
		}
	}

	return TRUE;
}

/**********************************************************************/

void COfferGroupHelpers::RefillOfferGroupCombo( CSSComboBox& combo )
{
	int nOffer = 0;

	int nSel = combo.GetCurSel();
	if ( nSel >= 0 && nSel < combo.GetCount() )
		nOffer = combo.GetItemData( nSel );

	FillOfferGroupCombo( combo, nOffer );
}

/**********************************************************************/

