

function start() {
	if (!SIMULATE)
		setInterval(function() { readMeasValues() }, 1000);
	else {
		document.ActualValues.valueDisplay.value = "10.0000V";
	}

}


function readMeasValues() {
	var arr = getNewLogs();
	var arr2 = arr.split("\n");
	var nrPoints = arr2.length - 1;
	if (nrPoints > 0) {
		arr = arr2[nrPoints - 1].split(",");
		document.ActualValues.valueDisplay.value = arr[1] + " " + arr[2]; // display value of last measurement;
	}
}
