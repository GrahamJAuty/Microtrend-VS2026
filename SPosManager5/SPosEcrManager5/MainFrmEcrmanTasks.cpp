/**********************************************************************/
#include "Share.h"
/**********************************************************************/
#include "..\CommonEcrStock\DataAccessHelpersTask.h"
#include "Password.h"
/**********************************************************************/
#include "MainFrmEcrman.h"
/**********************************************************************/

void CMainFrameEcrman::OnTasksAdhoc() 
{
	CDataAccessHelpersTask Helper;
	Helper.SetParentWnd( this );
	Helper.AdhocTask();
}

/**********************************************************************/
void CMainFrameEcrman::OnJobPreset1() { DoPresetTask(0x81); }
void CMainFrameEcrman::OnJobPreset2() { DoPresetTask(0x82); }
void CMainFrameEcrman::OnJobPreset3() { DoPresetTask(0x83); }
void CMainFrameEcrman::OnJobPreset4() { DoPresetTask(0x84); }
void CMainFrameEcrman::OnJobPreset5() { DoPresetTask(0x85); }
void CMainFrameEcrman::OnJobPreset6() { DoPresetTask(0x86); }
void CMainFrameEcrman::OnJobPreset7() { DoPresetTask(0x87); }
void CMainFrameEcrman::OnJobPreset8() { DoPresetTask(0x88); }
void CMainFrameEcrman::OnJobPreset9() { DoPresetTask(0x89); }
void CMainFrameEcrman::OnJobPreset10() { DoPresetTask(0x8A); }
void CMainFrameEcrman::OnJobPreset11() { DoPresetTask(0x8B); }
void CMainFrameEcrman::OnJobPreset12() { DoPresetTask(0x8C); }
void CMainFrameEcrman::OnJobPreset13() { DoPresetTask(0x8D); }
void CMainFrameEcrman::OnJobPreset14() { DoPresetTask(0x8E); }
void CMainFrameEcrman::OnJobPreset15() { DoPresetTask(0x8F); }
void CMainFrameEcrman::OnJobPreset16() { DoPresetTask(0x90); }
void CMainFrameEcrman::OnJobPreset17() { DoPresetTask(0x91); }
void CMainFrameEcrman::OnJobPreset18() { DoPresetTask(0x92); }
void CMainFrameEcrman::OnJobPreset19() { DoPresetTask(0x93); }
void CMainFrameEcrman::OnJobPreset20() { DoPresetTask(0x94); }
/**********************************************************************/

void CMainFrameEcrman::DoPresetTask( int nType )
{
	CDataAccessHelpersTask Helper;
	Helper.SetParentWnd( this );
	Helper.PresetTask( nType );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateTasksAdhoc(CCmdUI* pCmdUI) 
{
	pCmdUI -> Enable ( PasswordArray.GetEnable( PasswordTicks::TasksMenuAdhoc ) == TRUE );
}

/**********************************************************************/

void CMainFrameEcrman::UpdateTaskMenuAccess( CCmdUI* pCmdUI, int nTaskNo )
{
	CDataAccessHelpersTask Helper;
	pCmdUI -> Enable ( Helper.CanRunTask( nTaskNo ) );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset1(CCmdUI* pCmdUI) 
{
	UpdateTaskMenuAccess( pCmdUI, 0x81 );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset2(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x82 );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset3(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x83 );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset4(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x84 );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset5(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x85 );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset6(CCmdUI* pCmdUI) 
{
	UpdateTaskMenuAccess( pCmdUI, 0x86 );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset7(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x87 );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset8(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x88 );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset9(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x89 );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset10(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x8A );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset11(CCmdUI* pCmdUI) 
{
	UpdateTaskMenuAccess( pCmdUI, 0x8B );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset12(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x8C );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset13(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x8D);
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset14(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x8E );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset15(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x8F );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset16(CCmdUI* pCmdUI) 
{
	UpdateTaskMenuAccess( pCmdUI, 0x90 );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset17(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x91 );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset18(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x92 );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset19(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x93 );
}

/**********************************************************************/

void CMainFrameEcrman::OnUpdateJobPreset20(CCmdUI* pCmdUI) 
{	
	UpdateTaskMenuAccess( pCmdUI, 0x94 );
}

/**********************************************************************/