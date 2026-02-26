/**********************************************************************/
#include "DemoWarning.h"
/**********************************************************************/

CDemoWarning::CDemoWarning()
{
	int nSerialNo = Sysset.GetSerialNo();
	int nExternal = Sysset.GetExternalLink();

	if ( ( nSerialNo >= 9001 ) && ( nSerialNo <= 9099 ) && ( ( nExternal & 0x400 ) == 0x400 ) )
	{
		bool bFlag = EcrmanOptions.GetCustomMessageBoxFlag();
		int nFont = EcrmanOptions.GetCustomMessageBoxFont();
		int nGap = EcrmanOptions.GetCustomMessageBoxLineGap();
		int nBack = EcrmanOptions.GetCustomMessageBoxColourBackInt();
		int nText = EcrmanOptions.GetCustomMessageBoxColourTextInt();

		EcrmanOptions.SetCustomMessageBoxFlag(TRUE);
		EcrmanOptions.SetCustomMessageBoxFont(12);
		EcrmanOptions.SetCustomMessageBoxLineGap(10);
		EcrmanOptions.SetCustomMessageBoxColourBackInt( 0xFFFF00 );
		EcrmanOptions.SetCustomMessageBoxColourTextInt( 0x0 );

		CString strInfo = ""; 
		
		strInfo += "This is a dealer demo of Microtrend ";
		strInfo += AfxGetAppName();
		strInfo += ".\n\n";

		strInfo += "This software is NOT licensed for commercial use.\n\n";
		
		if ( Sysset.IsNobbledDemo() == TRUE )
		{
			strInfo += "Please note that some transactions may be intentionally\n";
			strInfo += "deleted or changed to discourage unauthorised use.";
		}

		Prompter.Info(strInfo);

		EcrmanOptions.SetCustomMessageBoxFlag( bFlag );
		EcrmanOptions.SetCustomMessageBoxFont( nFont );
		EcrmanOptions.SetCustomMessageBoxLineGap( nGap );
		EcrmanOptions.SetCustomMessageBoxColourBackInt( nBack );
		EcrmanOptions.SetCustomMessageBoxColourTextInt( nText );
	}
}

/**********************************************************************/
