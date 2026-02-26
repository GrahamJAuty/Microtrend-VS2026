/**********************************************************************/

CAspAutoComplete::CAspAutoComplete()
{
	m_strText = "";
	m_strMaxLength = "";
	m_strWebServiceUrl = "";
	m_strRows = "10";

	m_strScript = "";
}

/**********************************************************************/

void CAspAutoComplete::SetAttributes ( CCSV& csvAttributes )
{
	CAsp::SetAttributes ( csvAttributes );		// set common attributes

	CString strValue;
	if ( GetParamValue ( csvAttributes, ASP_TEXT, strValue ) == TRUE )			m_strText = strValue;
	if ( GetParamValue ( csvAttributes, ASP_MAX_LENGTH, strValue ) == TRUE )		m_strMaxLength = strValue;
	if ( GetParamValue ( csvAttributes, ASP_WEB_SERVICE_URL, strValue ) == TRUE )	m_strWebServiceUrl = strValue;
	if ( GetParamValue ( csvAttributes, ASP_ROWS, strValue ) == TRUE )			m_strRows = strValue;
}

/**********************************************************************/

void CAspAutoComplete::ParseUserParams ( CCSV& csvUserParams )
{
}

/**********************************************************************/

void CAspAutoComplete::AddScript ( const char* szScript )
{
	m_strScript += szScript;
	m_strScript += "\r\n";
}

/**********************************************************************/

void CAspAutoComplete::Write ( CString& strScript, CString& strStyle, CString& strBody, CString strPadding )
{
	AddScript ("<script>" );
	AddScript ("function " + m_strId + "LoadXMLDoc(url)");
	AddScript ("{");
	AddScript ("  var xmlhttp;");
	AddScript ("  if (window.XMLHttpRequest)");	// code for IE7+, Firefox, Chrome, Opera, Safari
	AddScript ("  {");
	AddScript ("    xmlhttp=new XMLHttpRequest();");
	AddScript ("  }");
	AddScript ("  xmlhttp.onreadystatechange=function()");
	AddScript ("  {");
	AddScript ("    if ( xmlhttp.readyState == 4 )");		// complete
	AddScript ("    {");
	AddScript ("      if ( xmlhttp.status == 200 )");
	AddScript ("      {");
	AddScript ("        var obj = document.getElementById('" + m_strId + "List');");
	AddScript ("        obj.innerHTML=xmlhttp.responseText;");
	AddScript ("        obj.style.display=\"block\";");	// make visible
	AddScript ("      }");
	AddScript ("    }");
	AddScript ("  }");
	AddScript ("  xmlhttp.open ( \"GET\", url, true );");
	AddScript ("  xmlhttp.send();");
	AddScript ("}");
	AddScript ("");
	AddScript ("function "+m_strId+"WebService(obj)");
	AddScript ("{");
	AddScript ("  if ( obj != null )");
	AddScript ("  {");
	AddScript ("    if ( obj.value.length >= 3 )");
	AddScript ("    {");
	AddScript ("      var url = \"" + m_strWebServiceUrl + "?" + m_strId + "=\"+obj.value;");
	AddScript ("      "+m_strId+"LoadXMLDoc(url);");
	AddScript ("    }");
	AddScript ("  }");
	AddScript ("}");
	AddScript ("</script>");

	AddStyleAttribute ( "padding-left:2px;" );

	CHTML h ( strPadding );
	h.Begin ( "div" );
	h.Attribute ( "style", GetStyleAttribute() );
	h.Attribute ( "class", m_strCssClass );
		h.Begin ( "input" );
		h.Attribute ( "id", m_strId );
		h.Attribute ( "type", "text" );
		h.Attribute ( "style", "width:100%;");
		h.Attribute ( "maxlength", m_strMaxLength );
		h.Attribute ( "name", m_strId );
		h.Attribute ( "value", m_strText );
		h.Attribute ( "onkeyup", m_strId+"WebService(this)" );
		h.End();		// input

		h.Begin ( "select" );
		h.Attribute ( "id", m_strId+"List" );
		h.Attribute ( "size", m_strRows );
		h.Attribute ( "onclick", "form.submit()" );
		h.Attribute ( "style", "width:100%;display:none;");	// make invisible
		h.Attribute ( "name", m_strId+"List" );
		h.End();		// select
	h.End();		// div

	strBody += h.GetHTML();
	strBody.TrimRight ( "\r\n" );		// asp object should not force new line
	strScript += m_strScript;
}

/**********************************************************************/

/*
<!-- Scripts by hscripts.com  -->
<!-- Copywrite of HIOXINDIA --->
<!-- More scripts @ www.hscripts.com  -->

<script type="text/javascript">
<!-- Declare the array and store the values according to ur usage -->
var suggestions = new Array("bank", "back", "peter","hindu","huge", "test","bums","cat","kind","fight","argue","append","run","sad","silk","light","little","rate","orange","office","lucky","cable","monitor","narration","early","pick","put","hungry","gain","gift","java","junction","vegtable","fan","north","needle","winter","nation","carry","dance","danger","iteration","facile","yahoo","quick","quee","arrangement","vechicle","urban","xerox","zeebra","xML");
var outp;
var oldins;
var posi = -1;
var words = new Array();
var input;
var key;
function setVisible(visi)
{
  var x = document.getElementById("shadow");
  var t = document.getElementsByName("text")[0];
  x.style.position = 'absolute';
  x.style.top = (findPosY(t)+3)+"px";
  x.style.left = (findPosX(t)+2)+"px";
  x.style.visibility = visi;
}
function init()
{
  outp = document.getElementById("output");
  window.setInterval("lookAt()", 100);
  setVisible("hidden");
  document.onkeydown = keygetter; //needed for Opera...
  document.onkeyup = keyHandler;
}
function findPosX(obj)
{
  var curleft = 0;
  if (obj.offsetParent)
  {
    while (obj.offsetParent)
    {
      curleft += obj.offsetLeft;
      obj = obj.offsetParent;
    }
   }
  else if (obj.x)
    curleft += obj.x;
        return curleft;
}
function findPosY(obj)
{
  var curtop = 0;
  if (obj.offsetParent)
  {
    curtop += obj.offsetHeight;
    while (obj.offsetParent)
    {
      curtop += obj.offsetTop;
      obj = obj.offsetParent;
     }
   }
   else if (obj.y)
   {
     curtop += obj.y;
     curtop += obj.height;
   }
   return curtop;
}
function lookAt()
{
   var ins = document.getElementsByName("text")[0].value;
   if (oldins == ins)
      return;
   else if (posi > -1);
   else if (ins.length > 0)
   {
     words = getWord(ins);
     if (words.length > 0)
     {
        clearOutput();
        for (var i=0;i < words.length; ++i)
             addWord (words[i]);
        setVisible("visible");
        input = document.getElementsByName("text")[0].value;
     }
     else
     {
        setVisible("hidden");
        posi = -1;
     }
   }
   else
   {
    setVisible("hidden");
    posi = -1;
   }
   oldins = ins;
}
function addWord(word)
{
  var sp = document.createElement("div");
  sp.appendChild(document.createTextNode(word));
  sp.onmouseover = mouseHandler;
  sp.onmouseout = mouseHandlerOut;
  sp.onclick = mouseClick;
  outp.appendChild(sp);
}
function clearOutput()
{
  while (outp.hasChildNodes())
  {
    noten=outp.firstChild;
    outp.removeChild(noten);
  }
   posi = -1;
}
function getWord(beginning)
{
  var words = new Array();
  for (var i=0;i < suggestions.length; ++i)
   {
    var j = -1;
    var correct = 1;
    while (correct == 1 && ++j < beginning.length)
    {
     if (suggestions[i].charAt(j) != beginning.charAt(j))
         correct = 0;
    }
    if (correct == 1)
       words[words.length] = suggestions[i];
  }
    return words;
  
}       
function setColor (_posi, _color, _forg)
{
   outp.childNodes[_posi].style.background = _color;
   outp.childNodes[_posi].style.color = _forg;
}
function keygetter(event)
{
  if (!event && window.event) 
      event = window.event;
  if (event)
      key = event.keyCode;
  else
      key = event.which;
}
function keyHandler(event)
{
  if (document.getElementById("shadow").style.visibility == "visible")
  {
     var textfield = document.getElementsByName("text")[0];
     if (key == 40)//key down
     { 
        if (words.length > 0 && posi <= words.length-1)
        {
          if (posi >=0)
            setColor(posi, "#fff", "black");
          else 
             input = textfield.value;
             setColor(++posi, "blue", "white");
             textfield.value = outp.childNodes[posi].firstChild.nodeValue;
        }
      }
      else if (key == 38)
      { //Key up
        if (words.length > 0 && posi >= 0)
         {
           if (posi >=1)
           {
              setColor(posi, "#fff", "black");
              setColor(--posi, "blue", "white");
              textfield.value = outp.childNodes[posi].firstChild.nodeValue;
           }
           else
           {
              setColor(posi, "#fff", "black");
              textfield.value = input;
              textfield.focus();
              posi--;
           }
         }
        }
         else if (key == 27)
         { // Esc
            textfield.value = input;
            setVisible("hidden");
            posi = -1;
            oldins = input;
          }
          else if (key == 8) 
          { // Backspace
            posi = -1;
            oldins=-1;
          } 
              }
   }
    var mouseHandler=function()
    {
      for (var i=0; i < words.length; ++i)
        setColor (i, "white", "black");
      this.style.background = "blue";
      this.style.color= "white";
     }
     var mouseHandlerOut=function()
     {
       this.style.background = "white";
       this.style.color= "black";
     }
     var mouseClick=function()
     {
        document.getElementsByName("text")[0].value = this.firstChild.nodeValue;
        setVisible("hidden");
        posi = -1;
        oldins = this.firstChild.nodeValue;
     }
</script>
<style type="text/css">

.output
{
        font-family:Arial;
        font-size: 10pt;
        color:black;
        padding-left: 3px;
        padding-top: 3px;
        border: 1px solid #000000;
        width: 100px;
        background: #fff;
}
.shadow
{
        width:102px;
        position:relative;
        top: 2px;
        left: 2px;
        background: #555;
}
.shadow div{
        position:relative;
        top: -2px;
        left: -2px;
}

</style>

<body onLoad="init();">
<form name="test" id="test">
<center>
<input type="text" name="text" autocomplete="off"></center>
</form>
<div class="shadow" id="shadow">
<div class="output" id="output">
</div>
</div><div align=center>
©<a href="http://www.hscripts.com" style="color:#3D366F;text-decoration:none;cursor:pointer;font-size:13px">hscripts.com</a></div>
</body>           
*/
