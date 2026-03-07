<!DOCTYPE html>
<html>
<head>
<meta charset="windows-1252">
<title>Smartcard Customer Database</title>
<style>
body { font-family:arial; }
h2 { text-align:center; font-size:1.5em; color:#606060; }
table.dg1 { margin-left:auto; margin-right:auto; }
</style>
</head>
<body>
  <h2>Smartcard Customer Database</h2>
  <form action="default.aspx" method="post">
    <asp:DataGrid Id="dg1" 
	Sql="SELECT CustNo,Customer,Dealer,Comment FROM private/custlist/custlist.csv WHERE CustNo>=5000;" 
	Sort="true" 
	SortType[]="int,text,text,text"
	TextAlign[]="left,left,left,left"
	Width="800px"
	CellPadding="4px"
	CellSpacing="0px"
	TextColor="#606060" 
	BorderWidth="1px"
	BorderStyle="solid"
	BorderColor="LightGray" 
	BackColor="White" 
	BackColorAlt="Beige" />
  </form>
</body>
</html>
