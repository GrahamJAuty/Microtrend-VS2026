/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/

void CPluCSVRecord::GetPriceLabel( int nPrice, CString& strLabel )
{
	strLabel = "";

	switch( nPrice )
	{
	case 1:		strLabel = Plu::Price1.Label;	break;
	case 2:		strLabel = Plu::Price2.Label;	break;
	case 3:		strLabel = Plu::Price3.Label;	break;
	case 4:		strLabel = Plu::Price4.Label;	break;
	case 5:		strLabel = Plu::Price5.Label;	break;
	case 6:		strLabel = Plu::Price6.Label;	break;
	case 7:		strLabel = Plu::Price7.Label;	break;
	case 8:		strLabel = Plu::Price8.Label;	break;
	case 9:		strLabel = Plu::Price9.Label;	break;
	case 10:	strLabel = Plu::Price10.Label;	break;
	}
}

/**********************************************************************/

void CPluCSVRecord::GetAllergenLabel( int nNo, CString& strLabel )
{
	strLabel = "";

	switch( nNo )
	{
	case 1:		strLabel = Plu::AllergenNo1.Label;	break;
	case 2:		strLabel = Plu::AllergenNo2.Label;	break;
	case 3:		strLabel = Plu::AllergenNo3.Label;	break;
	case 4:		strLabel = Plu::AllergenNo4.Label;	break;
	case 5:		strLabel = Plu::AllergenNo5.Label;	break;
	case 6:		strLabel = Plu::AllergenNo6.Label;	break;
	}
}

/**********************************************************************/

void CPluCSVRecord::GetOrderPrinterLabel(int nNo, CString& strLabel)
{
	strLabel = "";

	switch (nNo)
	{
	case 1:		strLabel = Plu::OrderPrinter1.Label;	break;
	case 2:		strLabel = Plu::OrderPrinter2.Label;	break;
	case 3:		strLabel = Plu::OrderPrinter3.Label;	break;
	case 4:		strLabel = Plu::OrderPrinter4.Label;	break;
	case 5:		strLabel = Plu::OrderPrinter5.Label;	break;
	case 6:		strLabel = Plu::OrderPrinter6.Label;	break;
	case 7:		strLabel = Plu::OrderPrinter7.Label;	break;
	case 8:		strLabel = Plu::OrderPrinter8.Label;	break;
	case 9:		strLabel = Plu::OrderPrinter9.Label;	break;
	case 10:	strLabel = Plu::OrderPrinter10.Label;	break;
	case 11:	strLabel = Plu::OrderPrinter11.Label;	break;
	case 12:	strLabel = Plu::OrderPrinter12.Label;	break;
	case 13:	strLabel = Plu::OrderPrinter13.Label;	break;
	case 14:	strLabel = Plu::OrderPrinter14.Label;	break;
	case 15:	strLabel = Plu::OrderPrinter15.Label;	break;
	case 16:	strLabel = Plu::OrderPrinter16.Label;	break;
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelEnable( int nMod, bool bLegacy, CString& strLabel )
{
	strLabel = "";

	if ( FALSE == bLegacy )
	{
		switch( nMod )
		{
		case 1:		strLabel = Plu::Modifier1Enable.Label;	break;
		case 2:		strLabel = Plu::Modifier2Enable.Label;	break;
		case 3:		strLabel = Plu::Modifier3Enable.Label;	break;
		case 4:		strLabel = Plu::Modifier4Enable.Label;	break;
		case 5:		strLabel = Plu::Modifier5Enable.Label;	break;
		case 6:		strLabel = Plu::Modifier6Enable.Label;	break;
		case 7:		strLabel = Plu::Modifier7Enable.Label;	break;
		case 8:		strLabel = Plu::Modifier8Enable.Label;	break;
		case 9:		strLabel = Plu::Modifier9Enable.Label;	break;
		}
	}
	else
	{
		if ( ( nMod >= 1 ) && ( nMod <= 9 ) )
		{
			strLabel.Format ( "ModifierEnable%d", nMod );
		}
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelEposTextFlag( int nMod, bool bLegacy, CString& strLabel )
{
	strLabel = "";

	if ( FALSE == bLegacy )
	{
		switch( nMod )
		{
		case 1:		strLabel = Plu::Modifier1EposTextFlag.Label;	break;
		case 2:		strLabel = Plu::Modifier2EposTextFlag.Label;	break;
		case 3:		strLabel = Plu::Modifier3EposTextFlag.Label;	break;
		case 4:		strLabel = Plu::Modifier4EposTextFlag.Label;	break;
		case 5:		strLabel = Plu::Modifier5EposTextFlag.Label;	break;
		case 6:		strLabel = Plu::Modifier6EposTextFlag.Label;	break;
		case 7:		strLabel = Plu::Modifier7EposTextFlag.Label;	break;
		case 8:		strLabel = Plu::Modifier8EposTextFlag.Label;	break;
		case 9:		strLabel = Plu::Modifier9EposTextFlag.Label;	break;
		}
	}
	else
	{
		if ( ( nMod >= 1 ) && ( nMod <= 9 ) )
		{
			strLabel.Format ( "ModifierEcrTextFlag%d", nMod );
		}
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelEposText( int nMod, bool bLegacy, CString& strLabel )
{
	strLabel = "";

	if ( FALSE == bLegacy )
	{
		switch( nMod )
		{
		case 1:		strLabel = Plu::Modifier1EposText.Label;	break;
		case 2:		strLabel = Plu::Modifier2EposText.Label;	break;
		case 3:		strLabel = Plu::Modifier3EposText.Label;	break;
		case 4:		strLabel = Plu::Modifier4EposText.Label;	break;
		case 5:		strLabel = Plu::Modifier5EposText.Label;	break;
		case 6:		strLabel = Plu::Modifier6EposText.Label;	break;
		case 7:		strLabel = Plu::Modifier7EposText.Label;	break;
		case 8:		strLabel = Plu::Modifier8EposText.Label;	break;
		case 9:		strLabel = Plu::Modifier9EposText.Label;	break;
		}
	}
	else
	{
		if ( ( nMod >= 1 ) && ( nMod <= 9 ) )
		{
			strLabel.Format ( "ModifierEcrText%d", nMod );
		}
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelKeyTextFlag( int nMod, bool bLegacy, CString& strLabel )
{
	strLabel = "";

	if ( FALSE == bLegacy )
	{
		switch( nMod )
		{
		case 1:		strLabel = Plu::Modifier1KeyTextFlag.Label;	break;
		case 2:		strLabel = Plu::Modifier2KeyTextFlag.Label;	break;
		case 3:		strLabel = Plu::Modifier3KeyTextFlag.Label;	break;
		case 4:		strLabel = Plu::Modifier4KeyTextFlag.Label;	break;
		case 5:		strLabel = Plu::Modifier5KeyTextFlag.Label;	break;
		case 6:		strLabel = Plu::Modifier6KeyTextFlag.Label;	break;
		case 7:		strLabel = Plu::Modifier7KeyTextFlag.Label;	break;
		case 8:		strLabel = Plu::Modifier8KeyTextFlag.Label;	break;
		case 9:		strLabel = Plu::Modifier9KeyTextFlag.Label;	break;
		}
	}
	else
	{
		if ( ( nMod >= 1 ) && ( nMod <= 9 ) )
		{
			strLabel.Format ( "ModifierKeyTextFlag%d", nMod );
		}
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelKeyText( int nMod, bool bLegacy, CString& strLabel )
{
	strLabel = "";

	if ( FALSE == bLegacy )
	{
		switch( nMod )
		{
		case 1:		strLabel = Plu::Modifier1KeyText.Label;	break;
		case 2:		strLabel = Plu::Modifier2KeyText.Label;	break;
		case 3:		strLabel = Plu::Modifier3KeyText.Label;	break;
		case 4:		strLabel = Plu::Modifier4KeyText.Label;	break;
		case 5:		strLabel = Plu::Modifier5KeyText.Label;	break;
		case 6:		strLabel = Plu::Modifier6KeyText.Label;	break;
		case 7:		strLabel = Plu::Modifier7KeyText.Label;	break;
		case 8:		strLabel = Plu::Modifier8KeyText.Label;	break;
		case 9:		strLabel = Plu::Modifier9KeyText.Label;	break;
		}
	}
	else
	{
		if ( ( nMod >= 1 ) && ( nMod <= 9 ) )
		{
			strLabel.Format ( "ModifierKeyText%d", nMod );
		}
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelOrderPrinterGroup( int nMod, CString& strLabel )
{
	strLabel = "";

	switch( nMod )
	{
	case 1: strLabel = Plu::Modifier1OrderPrinterGroup.Label;	break;
	case 2: strLabel = Plu::Modifier2OrderPrinterGroup.Label;	break;
	case 3: strLabel = Plu::Modifier3OrderPrinterGroup.Label;	break;
	case 4: strLabel = Plu::Modifier4OrderPrinterGroup.Label;	break;
	case 5: strLabel = Plu::Modifier5OrderPrinterGroup.Label;	break;
	case 6: strLabel = Plu::Modifier6OrderPrinterGroup.Label;	break;
	case 7: strLabel = Plu::Modifier7OrderPrinterGroup.Label;	break;
	case 8: strLabel = Plu::Modifier8OrderPrinterGroup.Label;	break;
	case 9: strLabel = Plu::Modifier9OrderPrinterGroup.Label;	break;
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelTaxBand( int nMod, bool bLegacy, CString& strLabel )
{
	strLabel = "";

	if ( FALSE == bLegacy )
	{
		switch( nMod )
		{
		case 1:		strLabel = Plu::Modifier1TaxBand.Label;	break;
		case 2:		strLabel = Plu::Modifier2TaxBand.Label;	break;
		case 3:		strLabel = Plu::Modifier3TaxBand.Label;	break;
		case 4:		strLabel = Plu::Modifier4TaxBand.Label;	break;
		case 5:		strLabel = Plu::Modifier5TaxBand.Label;	break;
		case 6:		strLabel = Plu::Modifier6TaxBand.Label;	break;
		case 7:		strLabel = Plu::Modifier7TaxBand.Label;	break;
		case 8:		strLabel = Plu::Modifier8TaxBand.Label;	break;
		case 9:		strLabel = Plu::Modifier9TaxBand.Label;	break;
		}
	}
	else
	{
		if ( ( nMod >= 1 ) && ( nMod <= 9 ) )
		{
			strLabel.Format ( "Modifier%d:TaxCode", nMod );
		}
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelAnalysisCategory( int nMod, CString& strLabel )
{
	strLabel = "";

	switch( nMod )
	{
	case 1: strLabel = Plu::Modifier1AnalysisCategory.Label;	break;
	case 2: strLabel = Plu::Modifier2AnalysisCategory.Label;	break;
	case 3: strLabel = Plu::Modifier3AnalysisCategory.Label;	break;
	case 4: strLabel = Plu::Modifier4AnalysisCategory.Label;	break;
	case 5: strLabel = Plu::Modifier5AnalysisCategory.Label;	break;
	case 6: strLabel = Plu::Modifier6AnalysisCategory.Label;	break;
	case 7: strLabel = Plu::Modifier7AnalysisCategory.Label;	break;
	case 8: strLabel = Plu::Modifier8AnalysisCategory.Label;	break;
	case 9: strLabel = Plu::Modifier9AnalysisCategory.Label;	break;
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelMicrotrendPluType( int nMod, CString& strLabel )
{
	strLabel = "";

	switch( nMod )
	{
	case 1: strLabel = Plu::Modifier1MicrotrendPluType.Label;	break;
	case 2: strLabel = Plu::Modifier2MicrotrendPluType.Label;	break;
	case 3: strLabel = Plu::Modifier3MicrotrendPluType.Label;	break;
	case 4: strLabel = Plu::Modifier4MicrotrendPluType.Label;	break;
	case 5: strLabel = Plu::Modifier5MicrotrendPluType.Label;	break;
	case 6: strLabel = Plu::Modifier6MicrotrendPluType.Label;	break;
	case 7: strLabel = Plu::Modifier7MicrotrendPluType.Label;	break;
	case 8: strLabel = Plu::Modifier8MicrotrendPluType.Label;	break;
	case 9: strLabel = Plu::Modifier9MicrotrendPluType.Label;	break;
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelDeptNo( int nMod, CString& strLabel )
{
	strLabel = "";

	switch( nMod )
	{
	case 1: strLabel = Plu::Modifier1DeptNo.Label;	break;
	case 2: strLabel = Plu::Modifier2DeptNo.Label;	break;
	case 3: strLabel = Plu::Modifier3DeptNo.Label;	break;
	case 4: strLabel = Plu::Modifier4DeptNo.Label;	break;
	case 5: strLabel = Plu::Modifier5DeptNo.Label;	break;
	case 6: strLabel = Plu::Modifier6DeptNo.Label;	break;
	case 7: strLabel = Plu::Modifier7DeptNo.Label;	break;
	case 8: strLabel = Plu::Modifier8DeptNo.Label;	break;
	case 9: strLabel = Plu::Modifier9DeptNo.Label;	break;
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelFirstMenu( int nMod, CString& strLabel )
{
	strLabel = "";

	switch( nMod )
	{
	case 1: strLabel = Plu::Modifier1FirstMenu.Label;	break;
	case 2: strLabel = Plu::Modifier2FirstMenu.Label;	break;
	case 3: strLabel = Plu::Modifier3FirstMenu.Label;	break;
	case 4: strLabel = Plu::Modifier4FirstMenu.Label;	break;
	case 5: strLabel = Plu::Modifier5FirstMenu.Label;	break;
	case 6: strLabel = Plu::Modifier6FirstMenu.Label;	break;
	case 7: strLabel = Plu::Modifier7FirstMenu.Label;	break;
	case 8: strLabel = Plu::Modifier8FirstMenu.Label;	break;
	case 9: strLabel = Plu::Modifier9FirstMenu.Label;	break;
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelFirstQualifier( int nMod, CString& strLabel )
{
	strLabel = "";

	switch( nMod )
	{
	case 1: strLabel = Plu::Modifier1FirstQualifier.Label;	break;
	case 2: strLabel = Plu::Modifier2FirstQualifier.Label;	break;
	case 3: strLabel = Plu::Modifier3FirstQualifier.Label;	break;
	case 4: strLabel = Plu::Modifier4FirstQualifier.Label;	break;
	case 5: strLabel = Plu::Modifier5FirstQualifier.Label;	break;
	case 6: strLabel = Plu::Modifier6FirstQualifier.Label;	break;
	case 7: strLabel = Plu::Modifier7FirstQualifier.Label;	break;
	case 8: strLabel = Plu::Modifier8FirstQualifier.Label;	break;
	case 9: strLabel = Plu::Modifier9FirstQualifier.Label;	break;
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelFirstExtra( int nMod, CString& strLabel )
{
	strLabel = "";

	switch( nMod )
	{
	case 1: strLabel = Plu::Modifier1FirstExtra.Label;	break;
	case 2: strLabel = Plu::Modifier2FirstExtra.Label;	break;
	case 3: strLabel = Plu::Modifier3FirstExtra.Label;	break;
	case 4: strLabel = Plu::Modifier4FirstExtra.Label;	break;
	case 5: strLabel = Plu::Modifier5FirstExtra.Label;	break;
	case 6: strLabel = Plu::Modifier6FirstExtra.Label;	break;
	case 7: strLabel = Plu::Modifier7FirstExtra.Label;	break;
	case 8: strLabel = Plu::Modifier8FirstExtra.Label;	break;
	case 9: strLabel = Plu::Modifier9FirstExtra.Label;	break;
	}
}

/**********************************************************************/

void CPluCSVRecord::GetModifierLabelPrice( int nMod, int nPrice, bool bLegacy, CString& strLabel )
{
	strLabel = "";

	if ( FALSE == bLegacy )
	{
		switch( nMod )
		{
		case 1:
			switch( nPrice )
			{
			case 1:		strLabel = Plu::Modifier1Price1.Label;	break;
			case 2:		strLabel = Plu::Modifier1Price2.Label;	break;
			case 3:		strLabel = Plu::Modifier1Price3.Label;	break;
			case 4:		strLabel = Plu::Modifier1Price4.Label;	break;
			case 5:		strLabel = Plu::Modifier1Price5.Label;	break;
			case 6:		strLabel = Plu::Modifier1Price6.Label;	break;
			case 7:		strLabel = Plu::Modifier1Price7.Label;	break;
			case 8:		strLabel = Plu::Modifier1Price8.Label;	break;
			case 9:		strLabel = Plu::Modifier1Price9.Label;	break;
			case 10:	strLabel = Plu::Modifier1Price10.Label;	break;
			}
			break;

		case 2:
			switch( nPrice )
			{
			case 1:		strLabel = Plu::Modifier2Price1.Label;	break;
			case 2:		strLabel = Plu::Modifier2Price2.Label;	break;
			case 3:		strLabel = Plu::Modifier2Price3.Label;	break;
			case 4:		strLabel = Plu::Modifier2Price4.Label;	break;
			case 5:		strLabel = Plu::Modifier2Price5.Label;	break;
			case 6:		strLabel = Plu::Modifier2Price6.Label;	break;
			case 7:		strLabel = Plu::Modifier2Price7.Label;	break;
			case 8:		strLabel = Plu::Modifier2Price8.Label;	break;
			case 9:		strLabel = Plu::Modifier2Price9.Label;	break;
			case 10:	strLabel = Plu::Modifier2Price10.Label;	break;
			}
			break;

		case 3:
			switch( nPrice )
			{
			case 1:		strLabel = Plu::Modifier3Price1.Label;	break;
			case 2:		strLabel = Plu::Modifier3Price2.Label;	break;
			case 3:		strLabel = Plu::Modifier3Price3.Label;	break;
			case 4:		strLabel = Plu::Modifier3Price4.Label;	break;
			case 5:		strLabel = Plu::Modifier3Price5.Label;	break;
			case 6:		strLabel = Plu::Modifier3Price6.Label;	break;
			case 7:		strLabel = Plu::Modifier3Price7.Label;	break;
			case 8:		strLabel = Plu::Modifier3Price8.Label;	break;
			case 9:		strLabel = Plu::Modifier3Price9.Label;	break;
			case 10:	strLabel = Plu::Modifier3Price10.Label;	break;
			}
			break;

		case 4:
			switch( nPrice )
			{
			case 1:		strLabel = Plu::Modifier4Price1.Label;	break;
			case 2:		strLabel = Plu::Modifier4Price2.Label;	break;
			case 3:		strLabel = Plu::Modifier4Price3.Label;	break;
			case 4:		strLabel = Plu::Modifier4Price4.Label;	break;
			case 5:		strLabel = Plu::Modifier4Price5.Label;	break;
			case 6:		strLabel = Plu::Modifier4Price6.Label;	break;
			case 7:		strLabel = Plu::Modifier4Price7.Label;	break;
			case 8:		strLabel = Plu::Modifier4Price8.Label;	break;
			case 9:		strLabel = Plu::Modifier4Price9.Label;	break;
			case 10:	strLabel = Plu::Modifier4Price10.Label;	break;
			}
			break;

		case 5:
			switch( nPrice )
			{
			case 1:		strLabel = Plu::Modifier5Price1.Label;	break;
			case 2:		strLabel = Plu::Modifier5Price2.Label;	break;
			case 3:		strLabel = Plu::Modifier5Price3.Label;	break;
			case 4:		strLabel = Plu::Modifier5Price4.Label;	break;
			case 5:		strLabel = Plu::Modifier5Price5.Label;	break;
			case 6:		strLabel = Plu::Modifier5Price6.Label;	break;
			case 7:		strLabel = Plu::Modifier5Price7.Label;	break;
			case 8:		strLabel = Plu::Modifier5Price8.Label;	break;
			case 9:		strLabel = Plu::Modifier5Price9.Label;	break;
			case 10:	strLabel = Plu::Modifier5Price10.Label;	break;
			}
			break;

		case 6:
			switch( nPrice )
			{
			case 1:		strLabel = Plu::Modifier6Price1.Label;	break;
			case 2:		strLabel = Plu::Modifier6Price2.Label;	break;
			case 3:		strLabel = Plu::Modifier6Price3.Label;	break;
			case 4:		strLabel = Plu::Modifier6Price4.Label;	break;
			case 5:		strLabel = Plu::Modifier6Price5.Label;	break;
			case 6:		strLabel = Plu::Modifier6Price6.Label;	break;
			case 7:		strLabel = Plu::Modifier6Price7.Label;	break;
			case 8:		strLabel = Plu::Modifier6Price8.Label;	break;
			case 9:		strLabel = Plu::Modifier6Price9.Label;	break;
			case 10:	strLabel = Plu::Modifier6Price10.Label;	break;
			}
			break;

		case 7:
			switch( nPrice )
			{
			case 1:		strLabel = Plu::Modifier7Price1.Label;	break;
			case 2:		strLabel = Plu::Modifier7Price2.Label;	break;
			case 3:		strLabel = Plu::Modifier7Price3.Label;	break;
			case 4:		strLabel = Plu::Modifier7Price4.Label;	break;
			case 5:		strLabel = Plu::Modifier7Price5.Label;	break;
			case 6:		strLabel = Plu::Modifier7Price6.Label;	break;
			case 7:		strLabel = Plu::Modifier7Price7.Label;	break;
			case 8:		strLabel = Plu::Modifier7Price8.Label;	break;
			case 9:		strLabel = Plu::Modifier7Price9.Label;	break;
			case 10:	strLabel = Plu::Modifier7Price10.Label;	break;
			}
			break;

		case 8:
			switch( nPrice )
			{
			case 1:		strLabel = Plu::Modifier8Price1.Label;	break;
			case 2:		strLabel = Plu::Modifier8Price2.Label;	break;
			case 3:		strLabel = Plu::Modifier8Price3.Label;	break;
			case 4:		strLabel = Plu::Modifier8Price4.Label;	break;
			case 5:		strLabel = Plu::Modifier8Price5.Label;	break;
			case 6:		strLabel = Plu::Modifier8Price6.Label;	break;
			case 7:		strLabel = Plu::Modifier8Price7.Label;	break;
			case 8:		strLabel = Plu::Modifier8Price8.Label;	break;
			case 9:		strLabel = Plu::Modifier8Price9.Label;	break;
			case 10:	strLabel = Plu::Modifier8Price10.Label;	break;
			}
			break;

		case 9:
			switch( nPrice )
			{
			case 1:		strLabel = Plu::Modifier9Price1.Label;	break;
			case 2:		strLabel = Plu::Modifier9Price2.Label;	break;
			case 3:		strLabel = Plu::Modifier9Price3.Label;	break;
			case 4:		strLabel = Plu::Modifier9Price4.Label;	break;
			case 5:		strLabel = Plu::Modifier9Price5.Label;	break;
			case 6:		strLabel = Plu::Modifier9Price6.Label;	break;
			case 7:		strLabel = Plu::Modifier9Price7.Label;	break;
			case 8:		strLabel = Plu::Modifier9Price8.Label;	break;
			case 9:		strLabel = Plu::Modifier9Price9.Label;	break;
			case 10:	strLabel = Plu::Modifier9Price10.Label;	break;
			}
			break;
		}
	}
	else
	{
		if ( ( nMod >= 1 ) && ( nMod <= 9 ) && ( nPrice >=1 ) && ( nPrice <= 10 ) )
		{
			strLabel.Format ( "ModifierPrice%d%d", nMod, nPrice );
		}
	}
}

/**********************************************************************/
