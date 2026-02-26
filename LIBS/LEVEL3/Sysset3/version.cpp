/****************************************************************/
/*	library version number										*/
/****************************************************************/

const char* szVERSION_SYSSET3 = "2.26, sysset3.lib";

/****************************************************************/
/*																*/
/*  V2.26	17-02-26	Internal update token has flag to say	*/
/*						whether EXE has been upgreded			*/	
/*																*/	
/*  V2.25	13-02-26	Removed all encoded messages			*/
/*																*/
/*  V2.24	12-02-26	Reg Tokens allowed for KioskBioLink		*/
/*						Removed encoding for more messages		*/
/*																*/
/*  V2.23	06-02-26	Reg Token Errors are not displayed		*/
/*						unless the faulty reg token is the		*/
/*						direct cause of registration failure.	*/
/*						Removing encoding for some messages.	*/
/*																*/
/*  V2.22	30-01-26	Reg Tokens are not checked for helper	*/
/*						applications such as Loyalty Server		*/
/*																*/
/*  V2.21	27-01-26	Supports Reg Tokens						*/
/*						Hidden static for entering redsky6 in	*/
/*						registration dialog replaced by button	*/
/*						Functions to convert version numbers	*/
/*						to and from strings.					*/
/*																*/
/*  V2.20	02-12-25	Separate Host and Client PC IDs			*/
/*																*/
/*  V2.19	01-08-25	Built with c++ 20						*/
/*																*/
/*  V2.18	13-07-24	Uses MS Shell Dlg 2 font for dialogs	*/
/*																*/
/*  V2.17	15-03-24	Update token waiver for helper modules	*/
/*																*/
/*  V2.16	11-03-24	Added more dealers for upgrade tokens	*/
/*																*/
/*  V2.15	09-03-24	Demos can use SPOS upgrade token		*/
/*																*/
/*  V2.14	07-03-24	Upgrade tokens for web updates			*/
/*																*/
/*  V2.13	21-02-24	Rebuilt with updated WINSUBS library	*/
/*																*/
/*  V2.12	07-09-23	Rebuilt with updated EXP library		*/
/*	V2.12				Built with Visual Studio 2022			*/
/*																*/
/*	V2.11	05-07-23	DemoTokens can change maximum locations	*/
/*						databases or users.						*/	
/*																*/	
/*	V2.10	18-08-22	Added GetEmbeddedSerialNo()				*/
/*						Added serial number check for			*/
/*						SERIALNO.DAT file						*/
/*																*/			
/*	V2.09	01-08-22	SmartPay3 changed to SmartPay4			*/
/*																*/	
/*	v2.08	09-04-22	Loyalty4 implemented					*/																
/*						SmartPay3 implemented					*/
/*						Removed SharpSoft from System Texts		*/
/*																*/																		
/*	v2.07	01-01-22	Rebuild for changes in WinSubs			*/
/*																*/
/*	v2.06	25-11-21	Rebuild for changes in WinSubs			*/
/*																*/			
/*  v2.05   30-10-21	Option to NOT create PCId if it does	*/
/*						not already exist during registration	*/
/*																*/	
/*  v2.04   30-05-21	Programs can store a username for the	*/
//						current instance in a PGxx file			*/
/*																*/	
/*  v2.03   23-04-21	Moved controls in registration dialog	*/
/*						to allow for longer passwords.			*/
/*																*/
/*	V2.02	14-03-21	Checks for any token filename matching	*/
/*						DEMOTOKEN_*.DAT pattern.				*/
/*						New flag in token data, intended to		*/
/*						signal to app that incoming sales data	*/
/*						should be distorted to some extent to	*/
/*						discourage unauthorised use of demo		*/
/*						software for commercial purposes.		*/
/*																*/
/*	V2.01	11-03-21	Supports token files to access demo		*/
/*						software without full registration		*/
/*																*/
/*	V2.00	26-02-21	Built with Visual Studio 2019			*/
/*																*/
/****************************************************************/