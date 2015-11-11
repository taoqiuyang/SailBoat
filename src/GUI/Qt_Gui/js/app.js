var waypoint = [];
var markers = [];

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
			var e = document.getElementById("select");
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
			var e = document.getElementById("select");
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
		
		//send information to mbed
		$("#send").click(function(){
			for (var i = 0; i < waypoint.length; i++){
				if(waypoint[i]) {
					lat = Number(waypoint[i].lat);
					lng = Number(waypoint[i].lng);
					self.send_to_boat(lat, lng);
				}
				//force a wait time, js doesn't have wait function
				for(var j = 0; j < 100000000; j++){}
			}
		})
	}
	
	initialize();
	
});
