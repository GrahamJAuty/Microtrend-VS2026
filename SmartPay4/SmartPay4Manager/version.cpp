//v4.14
//*********************************************************************

//SQL database connection uses OBC driver v17 with INI file option to change.

//v4.13
//*********************************************************************

//Replaced backup library with Chilkat.
//ZIP64 used for large backups, removes need for mini zip file workarounds.
//Compression level can be set between 0 and 9.

//SQL Server Locks replace file based system.

//Tray icon images are cached.

//Audits can be filtered by action type.

//Non UI option to log cards read by SmartPay Manager.

//Pebble source of funds and VAT codes.

//Chartwells payment export file.

//Prompt before cancelling account edit changes.

//Pebble reconcilation reports excludes pending sales transactions.

//EPOS terminal names can be imported from Ecr / Stock Manager.

//System file is unencrypted and moved to FILE0090.DAT

//Credit options are hidden unless non UI setting enabled in system options.

//v4.11
//*********************************************************************

//ReplyTo Header for SMTP Email.

//Test button on SMTP Email Options tab.

//v4.10
//*********************************************************************

//EmailSender not required for SMTP Emails.

//Supports use of SPOS SMTP Server.

//BromCom import of dinner items does not report an error if the
//WebAPI call worked but no items were returned.

//BromCom balance export does not report an error if the same
//account is flagged for export twice.

//BromCom export balances and transactions resets the auto shutdown
//timer.

//Payment method 21 is interpreted as purse, allowing work around
//for spend limits.


//v4.09
//*********************************************************************

//Link to Pebble API for sales and transactions

//v4.08
//*********************************************************************

//Filename cache option for database and top-up imports

//Mandatory header option for database and top-up imports

//Password numbers in audit with filter

//Auto delete option for expired purchase control stamps

//The period sales report includes transactions with zero spend

//Starting balance reports can be sorted by group

//Account searches can use info fields 2 to 8

//Bromcom transactions have comments showing sale time and items

//Default comments for audit reports and web payment exports

//Purse 2 refresh day settings work correctly

//v4.07
//*********************************************************************
//Purchase Controls.

//Max Spend and Max Overdraft range extended to 9999.99

//Purse2 Topup button displayed when relevant conditions met.

//SQL Script based back up and restore includes Shift Groups for Group table.

//Group Shift by Date Range information is passed to SPOS in RSP file.


//v4.06
//*********************************************************************
//Supports update tokens for web updates and beta versions.

//v4.05
//*********************************************************************

//Group Shift by date range.

//Group Shift option for on duty staff.

//Bio consent tick option.

//Bio registration date shown in account editing tab.

//UI scaling for high DPI displays.

//Conditional where clause works correctly with a single condition.

//Fixed infinite loop bug with multiple transactions for offline account.

//The Group Price level can be set to zero for no change.

//Fixed infinite loop bug when running a Purchase History report for a single account.

//Bio usage dates and times are saved to SQL table.

//*********************************************************************

//Period refresh works correctly

//v4.03.005
//*********************************************************************

//Wonde imports work correctly for new accounts.

//v4.03.004
//*********************************************************************

//Card Picker dialog shows correct amount of accounts selected when
//a file is used to preselect account.

//The preview report before sending balances to Wisepay shows data
//for the correct accounts.

//v4.03.001
//*********************************************************************

//Fixed infinite loop bug with ParentPay imports.

//v4.03
//*********************************************************************

//Purchase History Statements are sorted correctly by account

//Purchase History Statements show item summary.

//Payment Audit and EOD Report work if more than 10 payment groups

//Fixed Office Meal error message if no meal purchased.


//v4.02.004
//*********************************************************************

//SmartPay sends correct echo for Parent Pay zero payments

//v4.02.003
//*********************************************************************

//Card Picker dialog does not skip every other account

//v4.02.002
//*********************************************************************

//Reduces excess logging of "rename lock folder succeeded..."

//v4.02.001
//*********************************************************************

//Removal of SQL Account Buffer in Server

//Fixes server search bug.


//v4.02
//*********************************************************************

//Supports connection to SQL database using SQL Server Authentication.

//Supports SQLCONNECT.INI file to define connection settings.

//v4.01
//*********************************************************************

//End of Day closing balances are tied to database purse totals.

//Audit reports showing PLU texts are 10x faster

//Time range setting is saved after closing audit dialog


//v4.00.001
//*********************************************************************

//The Gender filter for database exports is not locked to male.
 
//Carriage return and line feed characters are removed from the end
//of user texts.