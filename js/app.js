var waypoint = [];

$(document).ready(function() {
	
	function updateTable(){
		var table = document.getElementById("pointTable");
		var rowNumber = waypoint.length;
		var row = table.insertRow(rowNumber + 1);
		
		var cell1 = row.insertCell(0);
		var cell2 = row.insertCell(1);
		var cell3 = row.insertCell(2);
		
		var num = waypoint.length;
		
		// Add some text to the new cells:
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
			
		   // alert('Lat: ' + event.latLng.lat() + ' Lng: ' + event.latLng.lng());
		});
		
		$("#send").click(function(){
			map.setCenter(new google.maps.LatLng($("#latval").val(), $("#lngval").val()));
			waypoint.push({
				lat: $("#latval").val(), 
				lng: $("#lngval").val()
			});
			updateTable();
		})
		
		$("#delete").click(function(){
			var num = waypoint.length;
			waypoint.pop();
			
			if(num > 0){
				document.getElementById("pointTable").deleteRow(num + 1);
			}
		})
		
	}
	
	initialize();
	
	// google.maps.event.addDomListener(window, 'load', initialize);
});
