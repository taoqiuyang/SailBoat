var waypoint = [];
var markers = [];

function update_GPS_table(imu_y, imu_p, imu_r){
	var myTable = document.getElementById('gpsTable');
	myTable.rows[2].cells[1].innerHTML = imu_y; //imu_y
	myTable.rows[3].cells[1].innerHTML = imu_p; //imu_p
	myTable.rows[4].cells[1].innerHTML = imu_r; //imu_r
	//myTable.rows[5].cells[1].innerHTML = gps_quality; //gps_quality
	//myTable.rows[6].cells[1].innerHTML = value; //gps_utc
	/*
	myTable.rows[7].cells[1].innerHTML = lat; //latitude
	myTable.rows[8].cells[1].innerHTML = lng; //longtitude
	myTable.rows[9].cells[1].innerHTML = alt; //altitude
	*/
	/*myTable.rows[10].cells[1].innerHTML = value; //num_satellite
	myTable.rows[10].cells[1].innerHTML = value; //hdop
	myTable.rows[10].cells[1].innerHTML = value; //vdop
	myTable.rows[10].cells[1].innerHTML = value; //pdop
	myTable.rows[10].cells[1].innerHTML = value; //date
	myTable.rows[10].cells[1].innerHTML = value; //v_knot
	myTable.rows[10].cells[1].innerHTML = value; //v_kph */
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
