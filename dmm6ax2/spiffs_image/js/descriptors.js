
var descriptors; // string containing all dmm descriptorsettings  
var firstTime = true;

var textFile = null;

function  makeTextFile (text) {
	var data = new Blob([text], {type: 'text/plain'});

	// If we are replacing a previously generated file we need to
	// manually revoke the object URL to avoid memory leaks.
	if (textFile !== null) {
		window.URL.revokeObjectURL(textFile);
	}

	textFile = window.URL.createObjectURL(data);

	//return textFile;
};


//var create = document.getElementById('create'),
//textbox = document.getElementById('textbox');

//create.addEventListener('click', function () {
//var link = document.getElementById('downloadlink');
//link.href = makeTextFile(textbox.value);
//link.style.display = 'block';
//}, false);
//};


var body;
var tbl;
var tblBody;

function displayDescriptors(descriptorData) {

	var body = document.forms[0];// document.getElementsByTagName("body")[0]; 
	if (!firstTime) {
		var x = document.getElementById("descriptorTable").rows.length

		for ( var r = 0; r< x; r++){
			tbl.deleteRow(-1);
		}
	}
	else {
		// creates a <table> element and a <tbody> element
		tbl =  document.getElementById("descriptorTable") ;// ocument.createElement("table");
		tblBody = document.createElement("tbody");
		firstTime = false;
	}

	var rows = descriptorData.split("\n");
	// row 0 us version nr, skip 
	// creating all cells

	for (var i = 1; i < rows.length - 1; i++) {
		// creates a table row
		var row = document.createElement("tr");
		var colls = rows[i].split(",");

		for (var j = 0; j < colls.length - 1; j++) {
			// Create a <td> element and a text node, make the text
			// node the contents of the <td>, and put the <td> at
			// the end of the table row
			var cell = document.createElement("td");
			var cellText = document.createTextNode(colls[j]);
			cell.appendChild(cellText);
			row.appendChild(cell);
		}

		// add the row to the end of the table body
		tblBody.appendChild(row);
	}

	// put the <tbody> in the <table>
	tbl.appendChild(tblBody);
	// appends <table> into <body>
	body.appendChild(tbl);
	// sets the border attribute of tbl to 2;
	tbl.setAttribute("border", "2");

}

function getDescriptors() {
	var req = new XMLHttpRequest();
	req.open("GET", "cgi-bin/descriptors", false);
	req.send();
	if (req.readyState == 4) {
		if (req.status == 200) {
			descriptors = req.responseText.toString();
			displayDescriptors(descriptors);

		}
	}
}
//fileSelect.addEventListener("click", function (e) {
//if (fileElem) {
//fileElem.click();
//}
//e.preventDefault(); // prevent navigation to "#"
//}, false);

//fileElem.addEventListener("change", handleFiles, false); 


//function getDescriptors() {
//descriptors  = "aap,noot,mies, wim,\n zus,jet,teun,vuur,\n"
//displayDescriptors(descriptors);

//}

//URL pointing to the Blob with the file contents
var objUrl = null;
//create the blob with file content, and attach the URL to the downloadlink; 
//NB: link must have the download attribute
//this method can go to your library

function exportFile ( downloadLinkId) {
	// revoke the old object URL to avoid memory leaks.
	if (objUrl !== null) {
		window.URL.revokeObjectURL(objUrl);
	}
	// create the object that contains the file data and that can be referred to with a URL
	var data = new Blob([descriptors], { type: 'text/plain' });
	objUrl = window.URL.createObjectURL(data);
	// attach the object to the download link (styled as button)
	var downloadLinkButton = document.getElementById(downloadLinkId);
	downloadLinkButton.href = objUrl;
}


//var  selectedFile;
function uploadClickx(){
	var text;
	var selectedFile = document.getElementById('upload').files[0];
	const reader = new FileReader();
	reader.addEventListener('load', (event) => {
		text = event.target.result;
		displayDescriptors(text);
		descriptors = text;
	});
	reader.readAsText(selectedFile);

}
function upload( fileInput) {

	var upload_path = "/upload/descriptors.dmm";

	var file = fileInput[0];
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (xhttp.readyState == 4) {
			if (xhttp.status == 200) {
				document.open();
				document.write(xhttp.responseText);
				document.close();
			} else if (xhttp.status == 0) {
				alert("Server closed the connection abruptly!");
				location.reload()
			} else {
				alert(xhttp.status + " Error!\n" + xhttp.responseText);
				location.reload()
			}
		}
	};
	xhttp.open("POST", upload_path, true);
	xhttp.send(fileInput);
}

