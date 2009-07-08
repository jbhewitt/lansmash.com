// $Id: picasa_album_gmap.js,v 1.1.2.3 2008/11/08 20:48:36 cyberswat Exp $

google.load('search', '1');
google.load('maps', '2');
// Our global state
var gLocalSearch;
var gMap;
var gSelectedResults = [];
var gCurrentResults = [];
var gSearchForm;

// Set up the map and the local searcher.
function OnLoad() {

    $('#picasa_place_title').html($('#edit-place').val());

		gSearchForm = new google.search.SearchForm(false, document.getElementById("searchform"));
		gSearchForm.setOnSubmitCallback(null, CaptureForm);
		gSearchForm.input.focus();

		// Initialize the map
		gMap = new google.maps.Map2(document.getElementById("map"));
		gMap.addControl(new google.maps.SmallMapControl());
		gMap.addControl(new google.maps.MapTypeControl());
    if($('#edit-lat').val().length > 0 && $('#edit-lng').val().length > 0) {
      gMap.setCenter(new google.maps.LatLng($('#edit-lat').val(), $('#edit-lng').val()), 9 );
      marker = new google.maps.Marker(new google.maps.LatLng(parseFloat($('#edit-lat').val()),
        parseFloat($('#edit-lng').val())),
        {draggable: true});
      GEvent.bind(marker, "dragend", this, function() {
          $('#picasa_place_title').html('');
          $('#edit-place').val(gSearchForm.input.value);
          $('#edit-lat').val(marker.getLatLng().lat());
          $('#edit-lng').val(marker.getLatLng().lng());
      });
      gMap.addOverlay(marker);
    }
    else {
      if (google.loader.ClientLocation &&
          google.loader.ClientLocation.latitude  &&
          google.loader.ClientLocation.longitude) {
          gMap.setCenter(new google.maps.LatLng(google.loader.ClientLocation.latitude,
              google.loader.ClientLocation.longitude), 9 );
      } else {
        gMap.setCenter(new google.maps.LatLng(19.1811, 156.1978), 1 );
      }
    }
		gMap.setMapType(G_HYBRID_MAP);
		gMap.enableScrollWheelZoom();
		// Initialize the local searcher
		gLocalSearch = new google.search.LocalSearch();
		gLocalSearch.setCenterPoint(gMap);
		gLocalSearch.setSearchCompleteCallback(null, OnLocalSearch);

}

// Called when Local Search results are returned, we clear the old
// results and load the new ones.
function OnLocalSearch() {
		if (!gLocalSearch.results) return;
				gMap.clearOverlays();
				$('#picasa_place_title').html('');
				$('#edit-place').val('');
				$('#edit-lat').val('');
				$('#edit-lng').val('');
		var searchWell = document.getElementById("searchwell");

		// Clear the map and the old search well
		searchWell.innerHTML = "";
		for (var i = 0; i < gCurrentResults.length; i++) {
				if (!gCurrentResults[i].selected()) {
						gMap.removeOverlay(gCurrentResults[i].marker());
				}
		}

		gCurrentResults = [];
		for (var i = 0; i < gLocalSearch.results.length; i++) {
				lr = new LocalResult(gLocalSearch.results[i])
				gCurrentResults.push(lr);
		}
		var attribution = gLocalSearch.getAttribution();
		if (attribution) {
				document.getElementById("searchwell").appendChild(attribution);
		}
		if(gLocalSearch.results.length == 1) {
				lr.select();
		}

		// move the map to the first result
		var first = gLocalSearch.results[0];
		gMap.setZoom(11);
		gMap.panTo(new google.maps.LatLng(first.lat, first.lng));
}

// Cancel the form submission, executing an AJAX Search API search.
function CaptureForm(searchForm) {
		gLocalSearch.execute(searchForm.input.value);
		$('#edit-place').val(searchForm.input.value);
		return false;
}

// A class representing a single Local Search result returned by the
// Google AJAX Search API.
function LocalResult(result) {
		this.result_ = result;
		this.resultNode_ = this.unselectedHtml();
		document.getElementById("searchwell").appendChild(this.resultNode_);
}

// Returns the HTML we display for a result before it has been "saved"
LocalResult.prototype.unselectedHtml = function() {
		var container = document.createElement("div");
		container.className = "unselected";
		var saveDiv = document.createElement("div");
		saveDiv.className = "select";
		saveDiv.innerHTML = this.result_.titleNoFormatting;
		GEvent.bindDom(saveDiv, "click", this, function() {
				gMap.clearOverlays();
				gMap.closeInfoWindow();
				this.select();
				gSelectedResults = new Array();
				gSelectedResults.push(this);
				gMap.panTo(new google.maps.LatLng(this.result_.lat,this.result_.lng));
		});
		container.appendChild(saveDiv);
		return container;
}

// "Saves" this result if it has not already been saved
LocalResult.prototype.select = function() {
		if (!this.selected()) {
				this.selected_ = true;

				// Remove the old marker and add the new marker
				gMap.removeOverlay(this.marker());
				this.marker_ = null;
						gMap.addOverlay(this.marker(G_DEFAULT_ICON));
				$('#picasa_place_title').html(this.result_.titleNoFormatting);
				$('#edit-place').val(this.result_.titleNoFormatting);
				$('#edit-lat').val(this.result_.lat);
				$('#edit-lng').val(this.result_.lng);
				// Remove the old search result from the search well
				this.resultNode_.parentNode.removeChild(this.resultNode_);
		}
}

// Returns true if this result is currently "saved"
LocalResult.prototype.selected = function() {
		return this.selected_;
}

// Returns the GMap marker for this result, creating it with the given
// icon if it has not already been created.
LocalResult.prototype.marker = function(opt_icon) {
		if (this.marker_) return this.marker_;
		var marker = new google.maps.Marker(new google.maps.LatLng(parseFloat(this.result_.lat),
				parseFloat(this.result_.lng)),
				{draggable: true});

		GEvent.bind(marker, "dragend", this, function() {
				$('#picasa_place_title').html('');
				$('#edit-place').val(gSearchForm.input.value);
				$('#edit-lat').val(marker.getLatLng().lat());
				$('#edit-lng').val(marker.getLatLng().lng());
		});
		this.marker_ = marker;
		return marker;
}
// Returns the HTML we display for a result after it has been "saved"
LocalResult.prototype.selectedHtml = function() {
		return this.result_.html.cloneNode(true);
}

google.setOnLoadCallback(OnLoad, true);
