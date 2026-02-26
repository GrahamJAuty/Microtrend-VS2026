/**********************************************************************/
#include "..\CommonEcrStock\DataAccessHelpersTask.h"
#include "Password.h"
/**********************************************************************/
#include "MainFrmStockman.h"
/**********************************************************************/

void CMainFrameStockman::OnTasksAdhoc() 
{
	CDataAccessHelpersTask Helper;
	Helper.SetParentWnd( this );
	Helper.AdhocTask();
}

/**********************************************************************/
void CMainFrameStockman::OnJobPreset1() { DoPresetTask(0x81); }
void CMainFrameStockman::OnJobPreset2() { DoPresetTask(0x82); }
void CMainFrameStockman::OnJobPreset3() { DoPresetTask(0x83); }
void CMainFrameStockman::OnJobPreset4() { DoPresetTask(0x84); }
void CMainFrameStockman::OnJobPreset5() { DoPresetTask(0x85); }
void CMainFrameStockman::OnJobPreset6() { DoPresetTask(0x86); }
void CMainFrameStockman::OnJobPreset7() { DoPresetTask(0x87); }
void CMainFrameStockman::OnJobPreset8() { DoPresetTask(0x88); }
void CMainFrameStockman::OnJobPreset9() { DoPresetTask(0x89); }
void CMainFrameStockman::OnJobPreset10() { DoPresetTask(0x8A); }
void CMainFrameStockman::OnJobPreset11() { DoPresetTask(0x8B); }
void CMainFrameStockman::OnJobPreset12() { DoPresetTask(0x8C); }
void CMainFrameStockman::OnJobPreset13() { DoPresetTask(0x8D); }
void CMainFrameStockman::OnJobPreset14() { DoPresetTask(0x8E); }
void CMainFrameStockman::OnJobPreset15() { DoPresetTask(0x8F); }
void CMainFrameStockman::OnJobPreset16() { DoPresetTask(0x90); }
void CMainFrameStockman::OnJobPreset17() { DoPresetTask(0x91); }
void CMainFrameStockman::OnJobPreset18() { DoPresetTask(0x92); }
void CMainFrameStockman::OnJobPreset19() { DoPresetTask(0x93); }
void CMainFrameStockman::OnJobPreset20() { DoPresetTask(0x94); }
/**********************************************************************/

void CMainFrameStockman::DoPresetTask( int nType )
{
	CDataAccessHelpersTask Helper;
	Helper.SetParentWnd( this );
	Helper.PresetTask( nType );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateTasksAdhoc(CCmdUI* pCmdUI) 
{
	pCmdUI -> Enable ( PasswordArray.GetEnable( PasswordTicks::TasksMenuAdhoc ) == TRUE );
}

/**********************************************************************/

void CMainFrameStockman::UpdateTaskMenuAccess( CCmdUI* pCmdUI, int nTaskNo )
{
	CDataAccessHelpersTask Helper;
	pCmdUI -> Enable ( Helper.CanRunTask( nTaskNo ) );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset1(CCmdUI* pCmdUI) 
{
	UpdateTaskMenuAccess( pCmdUI, 0x81 );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset2(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x82 );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset3(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x83 );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset4(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x84 );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset5(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x85 );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset6(CCmdUI* pCmdUI) 
{
	UpdateTaskMenuAccess( pCmdUI, 0x86 );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset7(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x87 );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset8(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x88 );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset9(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x89 );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset10(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x8A );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset11(CCmdUI* pCmdUI) 
{
	UpdateTaskMenuAccess( pCmdUI, 0x8B );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset12(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x8C );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset13(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x8D);
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset14(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x8E );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset15(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x8F );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset16(CCmdUI* pCmdUI) 
{
	UpdateTaskMenuAccess( pCmdUI, 0x90 );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset17(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x91 );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset18(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x92 );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset19(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x93 );
}

/**********************************************************************/

void CMainFrameStockman::OnUpdateJobPreset20(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x94 );
}

/**********************************************************************/

