/**
* 
*/
var chartData;


var chartRdy = false;
var tick = 0;
var dontDraw = false;
var halt = false;
var chartHeigth = 500;
var simValue1 = 0;
var simValue2 = 0;
var table;
var presc = 1;
var simMssgCnts = 0;
var lastTimeStamp = 0;

var measTimeLastSample;


var firstRequest = true;
var plotTimer = 6; // every 60 seconds plot averaged value
var rows = 0;

var MINUTESPERTICK = 1;// log interval 
var LOGDAYS = 7;
var MAXPOINTS = LOGDAYS * 24 * 60 / MINUTESPERTICK;

var SIMULATE = false;

var displayNames = ["", "temperatuur", "vochtigheid", "CO2", "ref"];
var dayNames = ['zo', 'ma', 'di', 'wo', 'do', 'vr', 'za'];

var options = {
	title: '',
	curveType: 'function',
	legend: { position: 'none' },
	heigth: 200,
	crosshair: { trigger: 'both' },	// Display crosshairs on focus and selection.
	explorer: {
		actions: ['dragToZoom', 'rightClickToReset'],
		//actions: ['dragToPan', 'rightClickToReset'],
		axis: 'horizontal',
		keepInBounds: true,
		maxZoomIn: 100.0
	},
	chartArea: { 'width': '90%', 'height': '60%' },
};

function initChart() {

	chart = new google.visualization.LineChart(document.getElementById('chart'));
	chartData = new google.visualization.DataTable();
	chartData.addColumn('string', 'Time');
	chartData.addColumn('number', '');

	chartRdy = true;
	dontDraw = false;
	if (SIMULATE)
		simplot();
	else
		startTimer();
}


function simplot() {

	var str2 = "";
	//	for (var n = 0; n < 3 * 24 * 4; n++) {
	for (var n = 0; n < 600; n++) {
		simValue1 += 0.01;
		simValue2 = Math.sin(simValue1);

		//                 time             value    		unit	                                                                     
		str2 = str2 + simMssgCnts++ + "," + simValue2 + ",V\n";
	}
	plotArray(str2);
}

function startTimer() {
	if (!SIMULATE)
		setInterval(function() { timer() }, 1000);
}


function plot(channel, value, timeStamp) {
	if (chartRdy) {
		if (channel == 1) {
			chartData.addRow();
			if (chartData.getNumberOfRows() > MAXPOINTS == true)
				chartData.removeRows(0, chartData.getNumberOfRows() - MAXPOINTS);
		}
		value = parseFloat(value); // from string to float
		chartData.setValue(chartData.getNumberOfRows() - 1, channel, value);
		var date = new Date(timeStamp);
		var labelText = date.getHours() + ":" + date.getMinutes() + ":" + date.getSeconds();
		chartData.setValue(chartData.getNumberOfRows() - 1, 0, labelText);
	}
}

// timestamp, value , unit

function plotArray(str) {
	var arr;
	var arr2 = str.split("\n");
	var nrPoints = arr2.length - 1;
	var timeOffset;
	if (nrPoints > 0) {

		arr = arr2[nrPoints - 1].split(",");
		measTimeLastSample = arr[0];  // can be unadjusted time in 10 ms units
		document.getElementById('valueDisplay').innerHTML = arr[1] + " " + arr[2]; // value of last measurement

		var sec = Date.now();//  / 1000;  // mseconds since 1-1-1970 
		timeOffset = sec - parseFloat(measTimeLastSample) * 10;

		for (var p = 0; p < nrPoints; p++) {
			arr = arr2[p].split(",");
			if (arr.length >= 1) {
				plot(1, arr[1], (parseFloat(arr[0]) * 10) + timeOffset);
			}
		}
		chart.draw(chartData, options);
	}
}


function timer() {
	var arr;
	if (SIMULATE) {
		simplot();
	}
	else {
		if (!halt) {
			if (firstRequest) {
				arr = getAllLogs();
				firstRequest = false;
				setInterval(function() { timer() }, 10000);
			}
			else {
				arr = getNewLogs();
			}
			plotArray(arr);
		}
	}
}

function startStop() {
	halt = !halt;
	if (halt)
		document.getElementById('startStopButton').innerHTML = 'start';
	else
		document.getElementById('startStopButton').innerHTML = 'stop';
}

function clearLog() {
	clearRemoteLog();
	chartData.removeRows(0, chartData.getNumberOfRows());
	chart.draw(chartData, options);
}

function clearChart() {
	chartData.removeRows(0, chartData.getNumberOfRows());
	chart.draw(chartData, options);
}



