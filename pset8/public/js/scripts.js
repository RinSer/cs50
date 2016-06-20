/* global google */
/* global _ */
/**
 * scripts.js
 *
 * Computer Science 50
 * Problem Set 8
 *
 * Global JavaScript.
 */

// Google Map
var map;

// markers for map
var markers = [];

// info window
var info = new google.maps.InfoWindow();

// execute when the DOM is fully loaded
$(function() {

    // styles for map
    // https://developers.google.com/maps/documentation/javascript/styling
    var styles = [

        // hide Google's labels
        {
            featureType: "all",
            elementType: "labels",
            stylers: [
                {visibility: "off"}
            ]
        },

        // hide roads
        {
            featureType: "road",
            elementType: "geometry",
            stylers: [
                {visibility: "off"}
            ]
        }

    ];

    // options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    var options = {
        center: {lat: 42.3770, lng: -71.1256}, // Harvard, Cambridge
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,
        panControl: true,
        styles: styles,
        zoom: 13,
        zoomControl: true
    };

    // get DOM node in which map will be instantiated
    var canvas = $("#map-canvas").get(0);

    // instantiate map
    map = new google.maps.Map(canvas, options);

    // configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);
    
    // Show the article preview in a pop up window
    $(document).on('mouseenter', 'a.news', function() {
        
        // Retrieve the article title
        var title = $(this).text();
        // Get the google rss feed in JSON format
        var google = 'https://ajax.googleapis.com/ajax/services/feed/find?v=1.0&q='
        var query = encodeURIComponent(title);
        // Make the query
        $.ajax({url: google+query, 
        dataType: 'jsonp'})
        .done(function(data) {
            
            // Add the snippet text if success
            if (data.responseData.entries.length > 0) {
                var text = data.responseData.entries[0].contentSnippet;
            }
            else {
                var text = "<b>Could not find this article snippet!</b>";
            }
            $('#preview_text').html(text);
            $('#preview').slideDown('slow');
            
        })
        .fail(function(jqXHR, textStatus, errorThrown) {

            // log error to browser's console
            console.log(errorThrown.toString());
            // Apologise if failure
            var text = "<b>Could not find this article snippet!</b>";
            $('#preview_text').html(text);
            $('#preview').show();
            
        });
        
    })
    .on('mouseleave', 'a.news', function() {
        
        $('#preview').slideUp();
        
    });
    // Hide the article preview if infowindow is closed
    google.maps.event.addListener(info, 'closeclick', function() {
        $('#preview').hide();
    });

});

/**
 * Adds marker for place to map.
 */
function addMarker(place)
{
    
    // Marker image url
    var image ="../img/news-icon.png";
    // Create the Marker object
    var marker = new MarkerWithLabel({
        position: {lat: parseFloat(place.latitude), lng: parseFloat(place.longitude)},
        map: map,
        title: 'Click to read the latest news!',
        labelContent: place.place_name,
        labelAnchor: new google.maps.Point(15, 0),
        labelClass: "labels", // the CSS class for the label
        labelInBackground: false,
        icon: image
    });
    // Invoke the Info Window
    marker.addListener('click', function() {
        // Get content for the Info Window
        postArticles(marker, place.postal_code);
    });
    // Add the Marker the the global array
    markers.push(marker);
    
}

/**
 * Configures application.
 */
function configure()
{
    // update UI after map has been dragged
    google.maps.event.addListener(map, "dragend", function() {
        update();
    });

    // update UI after zoom level changes
    google.maps.event.addListener(map, "zoom_changed", function() {
        update();
    });

    // remove markers whilst dragging
    google.maps.event.addListener(map, "dragstart", function() {
        removeMarkers();
    });

    // configure typeahead
    // https://github.com/twitter/typeahead.js/blob/master/doc/jquery_typeahead.md
    $("#q").typeahead({
        autoselect: true,
        highlight: true,
        minLength: 1
    },
    {
        source: search,
        templates: {
            empty: "no places found yet",
            suggestion: _.template("<p><%- place_name %>,&nbsp;<%- admin_name1 %>&nbsp;&nbsp;<span><%- postal_code %></span></p>")
        }
    });

    // re-center map after place is selected from drop-down
    $("#q").on("typeahead:selected", function(eventObject, suggestion, name) {

        // ensure coordinates are numbers
        var latitude = (_.isNumber(suggestion.latitude)) ? suggestion.latitude : parseFloat(suggestion.latitude);
        var longitude = (_.isNumber(suggestion.longitude)) ? suggestion.longitude : parseFloat(suggestion.longitude);

        // set map's center
        map.setCenter({lat: latitude, lng: longitude});

        // update UI
        update();
    });

    // hide info window when text box has focus
    $("#q").focus(function(eventData) {
        hideInfo();
    });

    // re-enable ctrl- and right-clicking (and thus Inspect Element) on Google Map
    // https://chrome.google.com/webstore/detail/allow-right-click/hompjdfbfmmmgflfjdlnkohcplmboaeo?hl=en
    document.addEventListener("contextmenu", function(event) {
        event.returnValue = true; 
        event.stopPropagation && event.stopPropagation(); 
        event.cancelBubble && event.cancelBubble();
    }, true);

    // update UI
    update();

    // give focus to text box
    $("#q").focus();
}

/**
 * Hides info window.
 */
function hideInfo()
{
    info.close();
    $('#preview').hide();
}

/**
 * Removes markers from map.
 */
function removeMarkers()
{
    
    // Remove all the markers from the map
    _.each(markers, function(marker) {
        
       marker.setMap(null); 
        
    });
    // Empty the global array
    markers = [];
    
}

/**
 * Searches database for typeahead's suggestions.
 */
function search(query, cb)
{
    // get places matching query (asynchronously)
    var parameters = {
        geo: query
    };
    $.getJSON("search.php", parameters)
    .done(function(data, textStatus, jqXHR) {

        // call typeahead's callback with search results (i.e., places)
        cb(data);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());
    });
}

/**
 * Shows info window at marker with content.
 */
function showInfo(marker, content)
{
    // start div
    var div = "<div id='info'>";
    if (typeof(content) === "undefined")
    {
        // http://www.ajaxload.info/
        div += "<img alt='loading' src='img/loader.gif'/>";
    }
    else
    {
        div += content;
    }

    // end div
    div += "</div>";

    // set info window's content
    info.setContent(div);
    
    // open info window (if not already open)
    info.open(map, marker);
}

/**
 * Updates UI's markers.
 */
function update() 
{
    // get map's bounds
    var bounds = map.getBounds();
    var ne = bounds.getNorthEast();
    var sw = bounds.getSouthWest();

    // get places within bounds (asynchronously)
    var parameters = {
        ne: ne.lat() + "," + ne.lng(),
        q: $("#q").val(),
        sw: sw.lat() + "," + sw.lng()
    };
    $.getJSON("update.php", parameters)
    .done(function(data, textStatus, jqXHR) {

        // remove old markers from map
        removeMarkers();

        // add new markers to map
        for (var i = 0; i < data.length; i++)
        {
            addMarker(data[i]);
        }
     })
     .fail(function(jqXHR, textStatus, errorThrown) {

         // log error to browser's console
         console.log(errorThrown.toString());
     });
}

/**
 * Get the Articles
 */
function postArticles(marker, zipcode) {
    
    // Empty string to store the news links
    var news = '<ul>';
    // Set query for URL
    var query = {
        geo: zipcode
    };
    // Get JSON with data
    $.getJSON('articles.php', query)
    .done(function(data) {
       
       _.each(data, function(item) {
           news += '<li><a class="news" target="_blank" href="' + item.link + '" >' + item.title + '</a></li>';
       });
       news += '</ul>'
       // call the showInfo
       showInfo(marker, news);
        
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());
        
    });
    
}