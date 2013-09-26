(function ($) {
  Drupal.behaviors.gmapAdmin = {
    attach: function(context, settings) {
      var lat = Drupal.settings.gmap.lat;
      var lon = Drupal.settings.gmap.lon;
      var zoom = Drupal.settings.gmap.zoom;
      var icon = Drupal.settings.gmap.path;
      var latlng = new google.maps.LatLng(lat, lon);
      var myOptions = {
        zoom: parseInt(zoom),
        center: latlng,
        mapTypeId: google.maps.MapTypeId.ROADMAP
      };
      var map = new google.maps.Map(document.getElementById("gmap"), myOptions);
      var input = (document.getElementById('MapLocation'));
      var autocomplete = new google.maps.places.Autocomplete(input);
      autocomplete.bindTo('bounds', map);
      var infowindow = new google.maps.InfoWindow();
      google.maps.event.addListener(autocomplete, 'place_changed', function(event) {
        infowindow.close();
        marker.setVisible(false);
        var place = autocomplete.getPlace();
        if (place.geometry.viewport) {
          map.fitBounds(place.geometry.viewport);
        } else {
          map.setCenter(place.geometry.location);
          map.setZoom(17);
        }
        marker.setIcon(({
          url: icon
        }));
        marker.setPosition(place.geometry.location);
        marker.setVisible(true);
        var address = '';
        if (place.address_components) {
          address = [
            (place.address_components[0] && place.address_components[0].short_name || ''),
            (place.address_components[1] && place.address_components[1].short_name || ''),
            (place.address_components[2] && place.address_components[2].short_name || '')
          ].join(' ');
        }
        infowindow.setContent('<div><strong>' + place.name + '</strong><br>' + address);
        infowindow.open(map, marker);
        placeMarker(place.geometry.location);
      });
      var marker = new google.maps.Marker({
        position: latlng,
        map: map,
        icon: icon
      });
      google.maps.event.addListener(map, 'zoom_changed', function() {
        document.getElementById('gmap_zoom').value = map.getZoom();
      });
      google.maps.event.addListener(map, 'click', function(event) {
        placeMarker(event.latLng);
      });
      function placeMarker(location) {
        marker.setPosition(location);
        document.getElementById('gmap_lat').value = location.lat();
        document.getElementById('gmap_lon').value = location.lng();
      }
    }
  };
})(jQuery);