var waypoint = [];
var markers = [];

function update_GPS_table(imu_y, imu_p, imu_r, gps_quality, gps_utc_flag, gps_utc_hour, gps_utc_minute, gps_utc_second,
						lat, lng, alt, num_satellite, hdop, vdop, pdop, date, v_knot, v_kph, NAdouble){
	var myTable = document.getElementById('gpsTable');
	myTable.rows[2].cells[1].innerHTML = parse_double(imu_y, NAdouble); //imu_y
	myTable.rows[3].cells[1].innerHTML = parse_double(imu_p, NAdouble); //imu_p
	myTable.rows[4].cells[1].innerHTML = parse_double(imu_r, NAdouble); //imu_r
	if(gps_quality == NAdouble){
		myTable.rows[5].cells[1].innerHTML = "N/A";
	} else {
		if(gps_quality == 0) {
			myTable.rows[5].cells[1].innerHTML = "NO Signal(0)"; //gps_quality
		} else if(gps_quality == 1){
			myTable.rows[5].cells[1].innerHTML = "Signal On(1)"; //gps_quality
		} else if (gps_quality == 2) {
			myTable.rows[5].cells[1].innerHTML = "Good Signal(2)"; //gps_quality
		} else {
			myTable.rows[5].cells[1].innerHTML = gps_quality; //gps_quality
		}
	} 
	if(gps_utc_flag==0) {
		myTable.rows[6].cells[1].innerHTML = "N/A";//gps_utc
	} else {
		if(gps_utc_hour.toString().length == 1) {
			gps_utc_hour = "0" + gps_utc_hour.toString();
		}
		if(gps_utc_minute.toString().length == 1) {
			gps_utc_minute = "0" + gps_utc_minute.toString();
		}
		if(gps_utc_second.toString().length == 1) {
			gps_utc_second = "0" + gps_utc_second.toString();
		}
		myTable.rows[6].cells[1].innerHTML = gps_utc_hour + ":" 
				+ gps_utc_minute + ":" + gps_utc_second; //gps_utc
		}
	myTable.rows[7].cells[1].innerHTML = parse_double(lat, NAdouble); //latitude
	myTable.rows[8].cells[1].innerHTML = parse_double(lng, NAdouble); //longtitude
	if(alt == NAdouble){
		myTable.rows[9].cells[1].innerHTML = "N/A";
	} else {
		myTable.rows[9].cells[1].innerHTML = alt.toString() + " m"; //altitude
	}
	if(num_satellite == NAdouble){
		myTable.rows[10].cells[1].innerHTML = "N/A";
	} else {
		myTable.rows[10].cells[1].innerHTML = num_satellite.toString() + "/9"; //num_satellite
	}	
	myTable.rows[11].cells[1].innerHTML = parse_double(hdop, NAdouble); //hdop
	myTable.rows[12].cells[1].innerHTML = parse_double(vdop, NAdouble); //vdop
	myTable.rows[13].cells[1].innerHTML = parse_double(pdop, NAdouble); //pdop
	if(date == NAdouble){
		myTable.rows[14].cells[1].innerHTML = "N/A";
	} else {
		date = date.toString();
		myTable.rows[14].cells[1].innerHTML = date.substr(2,2) + "/" 
						+ date.substr(0,2) + "/20" + date.substr(4,2); //date
	}
	if(v_knot == NAdouble){
		myTable.rows[15].cells[1].innerHTML = "N/A";
	} else {
		myTable.rows[15].cells[1].innerHTML = v_knot.toString() + " knot"; //v_knot
	}
	if(v_kph == NAdouble){
		myTable.rows[16].cells[1].innerHTML = "N/A";
	} else {
		myTable.rows[16].cells[1].innerHTML = v_kph.toString() + " kph"; //v_kph
	}
	
	
}
function parse_double(param, NAdouble){
	if(param == NAdouble){
		return "N/A";
	} else {
		return param.toString();
	}
}
$(document).ready(function() {
	
	function updateTable(taskid){
		var table = document.getElementById("pointTable");
		var index = parseInt(taskid)+1;
		table.rows[index].cells[1].innerHTML = waypoint[taskid - 1].lat;
		table.rows[index].cells[2].innerHTML = waypoint[taskid - 1].lng;
		var rowNumber = waypoint.length;
	}
	
	function initialize() {
		//create google map
		var mapCanvas = document.getElementById('map');
		
		var mapOptions = {
			zoom: 11,
			center: new google.maps.LatLng(51.41859298, 0.089179345),
			mapTypeControl: true,
			scaleControl: true,
			mapTypeControlOptions: {
				style: google.maps.MapTypeControlStyle.DROPDOWN_MENU
			},
			navigationControl: true,
			navigationControlOptions: {
				style: google.maps.NavigationControlStyle.DEFAULT
			},
			mapTypeId: google.maps.MapTypeId.ROADMAP,
			backgroundColor: 'white'
		};
		
		var map = new google.maps.Map(mapCanvas, mapOptions);
		
		//link map to lat/lng input fields
		google.maps.event.addListener(map, 'click', function(event){
			$("#latval").val(event.latLng.lat().toFixed(6));
			$("#lngval").val(event.latLng.lng().toFixed(6));
		});
		
		//re-center the google map
		function resetCenter(){
			//calculate center
			var ave_lat = 0;
			var ave_lng = 0;
			var count = 0;
			for (var i = 0; i < waypoint.length; i++){
				if(waypoint[i]) {
					ave_lat = ave_lat + Number(waypoint[i].lat);
					ave_lng = ave_lng + Number(waypoint[i].lng);
					count++;
				}
			}
			
			ave_lat = ave_lat / count;
			ave_lng = ave_lng / count;
			
			map.setCenter(new google.maps.LatLng(ave_lat, ave_lng));
		}
		
		$("#add").click(function(){
			var location = new google.maps.LatLng($("#latval").val(), $("#lngval").val());
			var e = document.getElementById("select_waypoints");
			var taskid = e.options[e.selectedIndex].value;

			waypoint[taskid-1] = {
				lat: $("#latval").val(), 
				lng: $("#lngval").val()
			};

			
			resetCenter();
			updateTable(taskid);
			
			var marker = new google.maps.Marker({
				position: location,
				animation: google.maps.Animation.DROP,
			});
			
			if(markers[taskid-1]) {
				markers[taskid-1].setMap(null);
			}
			markers[taskid-1] = marker;
			markers[taskid-1].setMap(map);
		})
		
		
		$("#delete").click(function(){
			var num = waypoint.length;
			var e = document.getElementById("select_waypoints");
			var taskid = e.options[e.selectedIndex].value;
			
			if(num > 0){
				waypoint[taskid-1] = null;
				if(markers[taskid-1]) {
					markers[taskid-1].setMap(null);
				}
				markers[taskid-1] = null;
				var table = document.getElementById("pointTable");
				var index = parseInt(taskid)+1;
				table.rows[index].cells[1].innerHTML = null;
				table.rows[index].cells[2].innerHTML = null;
				if(num > 1){
					resetCenter();
				}
			}
		})
		
		//send path information to mbed
		$("#setpath").click(function(){
			for (var i = 0; i < waypoint.length; i++){
				if(waypoint[i]) {
					lat = Number(waypoint[i].lat);
					lng = Number(waypoint[i].lng);
					self.send_to_boat_path(lat, lng, i+1);
				}
				//force a wait time, js doesn't have wait function
				for(var j = 0; j < 100000000; j++){}
			}
		})
		//send rudder information to mbed
		$("#setservo").click(function(){
			var e = document.getElementById("select_servo");
			var rudder = e.options[e.selectedIndex].value;
			
			//it's hard to send string, send float then do conversion instead
			if(rudder == "RUDDER"){
				rudder = 0.0;
			} else {
				rudder = 1.0;
			}
			self.send_to_boat_servo(rudder,  $("#degree").val());
			//force a wait time, js doesn't have wait function
			for(var j = 0; j < 100000000; j++){}
		})
	}
	
	initialize();
	
});
