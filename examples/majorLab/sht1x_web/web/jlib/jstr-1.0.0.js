// *****************************************************************************
// Embeda's String Library
// Author: Jason Liao
// Date: 2012/5/1
// Copyright: Embeda Technology Inc. 
// License: CC BY-SA
// *****************************************************************************
function getParamValue(src, index, separation)
{
	var pos,i;
	var str = src;
	
	i = 0;
	do{
		pos = str.search(separation);
		if ( pos>0 )
		{	
			if ( i==index  )
				return str.substr(0, pos);	
			str = str.substr(pos+1);
			i++;
		}
	}while(pos>0);
	return str;	
}

function getEncryCode(szCode) 
{
	var szEnCode,i;
	temp = new Array();
	
	temp[0] = Math.round(Math.random() * 127);
	
	for (i=0;i<szCode.length;i++)
	{
		temp[i+1] = (temp[i] ^ szCode.charCodeAt(i));
	}
	
	szEnCode = "";
	for (i=0; i<temp.length; i++)
	{
		if ( temp[i]<16 ) {
			szEnCode= szEnCode + '-0' + temp[i].toString(16);	
		} else {
			szEnCode= szEnCode + '-' + temp[i].toString(16);	
		}
	}	
	return szEnCode;
}

function checkHEX(e)
{
	if( (e.keyCode>=48 && e.keyCode<=57)  || (e.keyCode>=65 && e.keyCode<=70)){
		return true;
	}else if (e.keyCode>=97 && e.keyCode<=102){
		e.keyCode=e.keyCode-32;
		return true;
	}else{
		e.returnValue = false;
		return false;
	}
}

function CopyToClipboard(s)
{
	if( window.clipboardData && clipboardData.setData )	{
		clipboardData.setData("Text", s);
	} else 	{
		alert("Internet Explorer required");
	}
}

