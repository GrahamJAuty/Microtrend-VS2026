<!DOCTYPE html>
<html> 
<head> 
<meta charset="windows-1252">
<title>Software Registration</title>
<style>
body { font-family:Arial; font-size:14pt; }
table { background-color:#99CCFF; min-width:40em; margin-left:auto; margin-right:auto; margin-top:3em; }
td { text-align:center; height:4em; }
h2 { color:#303030; }
input { font-size:1em; }
span.ok { font-size:1.4em; }
span.error { font-size:1.4em; color:red; }
</style>
</head>
<body>
  <form action="/register/" method="post">
    <table>
      <tr>
        <td><h2>Software Registration</h2></td>
      </tr>
      <tr>
        <td>Please enter your 16 digit Access Code: <asp:TextBox Id="AccessCode" Columns="21" MaxLength="19" /> <input type="submit" value="submit"></td>
      </tr>
      <tr>
        <td>
          <asp:Label Id="UserCode" />
        </td>
      </tr>
    </table>
  </form>
</body>
</html>
