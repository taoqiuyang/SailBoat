$(document).ready(function() {

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
		})
	}
	
	initialize();
	
	// google.maps.event.addDomListener(window, 'load', initialize);
});
