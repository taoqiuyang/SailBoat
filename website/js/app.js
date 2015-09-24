var waypoint = [];
var markers = [];

$(document).ready(function() {
	
	function updateTable(){
		var table = document.getElementById("pointTable");
		var rowNumber = waypoint.length;
		var row = table.insertRow(rowNumber + 1);
		
		var cell1 = row.insertCell(0);
		var cell2 = row.insertCell(1);
		var cell3 = row.insertCell(2);
		
		// Add some text to the new cells:
		var num = waypoint.length;
		cell1.innerHTML = num;
		cell2.innerHTML = waypoint[num - 1].lat;
		cell3.innerHTML = waypoint[num - 1].lng;
	}
	
	function initialize() {
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
		
		google.maps.event.addListener(map, 'click', function(event){
			$("#latval").val(event.latLng.lat().toFixed(6));
			$("#lngval").val(event.latLng.lng().toFixed(6));
		});
		
		$("#add").click(function(){
			var location = new google.maps.LatLng($("#latval").val(), $("#lngval").val());
			waypoint.push({
				lat: $("#latval").val(), 
				lng: $("#lngval").val()
			});
			
			//calculate center
			var ave_lat = 0;
			var ave_lng = 0;
			
			for (var i = 0; i < waypoint.length; i++){
				ave_lat = ave_lat + Number(waypoint[i].lat);
				ave_lng = ave_lng + Number(waypoint[i].lng);
			}
			
			ave_lat = ave_lat / waypoint.length;
			ave_lng = ave_lng / waypoint.length;
			
			map.setCenter(new google.maps.LatLng(ave_lat, ave_lng));
			
			updateTable();
			
			var marker = new google.maps.Marker({
				position: location,
				map: map
			});
			
			markers.push(marker);
		})
		
		$("#delete").click(function(){
			var num = waypoint.length;
			
			if(num > 0){
				waypoint.pop();
				markers[markers.length - 1].setMap(null);
				markers.pop();
				document.getElementById("pointTable").deleteRow(num + 1);
			}
		})
	}
	
	initialize();
	
});
