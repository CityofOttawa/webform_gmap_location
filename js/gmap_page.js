var lat = Drupal.settings.gmap.lat;
var lon = Drupal.settings.gmap.lon;
var zoom = Drupal.settings.gmap.zoom;
var icon = Drupal.settings.gmap.path;
var limit = Drupal.settings.gmap.limit;
console.log(Drupal.settings.gmap);
var limit_count = 0;
var markers = {};

(function ($) {
  Drupal.behaviors.gmapPage = {
    attach: function(context, settings) {
      function initialize() {
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
        google.maps.event.addListener(autocomplete, 'place_changed', function(event) {
          var place = autocomplete.getPlace();
          if (place.geometry.viewport) {
            map.fitBounds(place.geometry.viewport);
          } else {
            map.setCenter(place.geometry.location);
            map.setZoom(7);
          }
          var address = '';
          if (place.address_components) {
            address = [
              (place.address_components[0] && place.address_components[0].short_name || ''),
              (place.address_components[1] && place.address_components[1].short_name || ''),
              (place.address_components[2] && place.address_components[2].short_name || '')
            ].join(' ');
          }
          placeMarker(place.geometry.location, map);
        });
        google.maps.event.addListener(map, 'click', function(e) {
          console.log(limit);
          if(limit + 1 == limit_count && limit != 0) return false;
          placeMarker(e.latLng, map);
        });
        google.maps.event.addListener(map, 'zoom_changed', function() {
          document.getElementById('gmap_zoom').value = map.getZoom();
        });
      }
      function placeMarker(position, map) {
        var marker = new google.maps.Marker({
          position: position,
          map: map,
          icon: icon
        });
        var id = marker.__gm_id
        markers[id] = marker;
        document.getElementById('gmap_lat').value = document.getElementById('gmap_lat').value + position.lat() + ',';
        document.getElementById('gmap_lon').value = document.getElementById('gmap_lon').value + position.lng() + ',';
        map.panTo(position);
        limit_count = limit_count + 1;
        google.maps.event.addListener(marker, "click", function(point){ id = this.__gm_id; delMarker(id) });
      }

      var delMarker = function(id) {
        marker = markers[id];
        var lat = marker.position.lat();
        var lng = marker.position.lng();
        document.getElementById('gmap_lat').value = document.getElementById('gmap_lat').value.replace(lat + ",","");
        document.getElementById('gmap_lon').value = document.getElementById('gmap_lon').value.replace(lng + ",","");
        marker.setMap(null);
        limit_count  = limit_count - 1;
      }
      google.maps.event.addDomListener(window, 'load', initialize);
    }
  };
})(jQuery);
