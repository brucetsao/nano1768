// *****************************************************************************
// Embeda's Ajax Library
// Author: Jason Liao
// Date: 2012/5/6
// Copyright: Embeda Technology Inc. (http://www.embeda.com.tw)
// License: CC BY-SA
// *****************************************************************************

//
// get data from url and return to callback function with text parameter
//
function getDataToCallback(url, callback){
    var xmlHttp = false;
	
    // Mozilla/Safari
    if (window.XMLHttpRequest) {
        xmlHttp = new XMLHttpRequest();
		
	// IE
    } else if (window.ActiveXObject) {
		try {
			xmlHttp = new ActiveXObject("Msxml2.XMLHTTP");
		} catch (e) {
   			xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
		}
	}
	
	if ( xmlHttp )	{
		xmlHttp.open("GET", url +"&"+ Math.random(), true);	
		if ( xmlHttp.overrideMimeType ) {
			xmlHttp.overrideMimeType('text/plain');
		}
		xmlHttp.setRequestHeader ("Content-Type", "text/plain; charset=utf-8") ; 		
		xmlHttp.onreadystatechange = function() {
			if( xmlHttp.readyState == 4 && xmlHttp.status == 200) {
				callback(xmlHttp.responseText);
			}
		}
		xmlHttp.send(null);
	}
}

//
// get data from url to html element (id)
//
function getDataToElement(url, id)
{
	function txtComplete(text)
	{	
		if ( id!="" ){
			document.getElementById(id).innerHTML = text;
		}
		bComplete = true;
	}
	getDataToCallback(url, txtComplete);	
}

//
// get data from url to html element (id)
//
function getDataToInputValue(url, id)
{
	function txtComplete(text)
	{
		document.getElementById(id).value = text;			
	}
	getDataToCallback(url, txtComplete);	
}

//
// auto update data by window timeout
//
function autoDataToElement(url, id, interval)
{
	ms = interval;
	function auto()
	{
		getDataToElement(url, id);								   
		window.setTimeout(auto, ms);
	}
	auto();
}
