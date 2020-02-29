<?php

// Modified https://knackforge.com/blog/narendran/kml-google-map-circle-generator-script-php
// Plots a KML circle for the LOP around the Hamal GP.

$arr = array(
	"Hamal" => array(
		"lat" => 23.558,
		"lon" => -38.795,
		"dist" => 2875000,
		"s" => "#redThin",
	),
	"Shedar" => array(
		"lat" => 56.6483,
		"lon" => -60.457,
		"dist" => 2665000,
		"s" => "#grnThin",
	),
	"Capella" => array(
		"lat" => 46.016,
		"lon" => 8.674,
		"dist" => 2534000,
		"s" => "#yelThin",
	),
);

foreach($arr as $key => $data) { 
	$coordinatesList = getCirclecoordinates($data["lat"], $data["lon"], $data["dist"]);
	$kml  = "    <Placemark>\n";
	$kml .= "     <name>$key</name>\n";
	$kml .= "      <styleUrl>${data["s"]}</styleUrl>\n";
	$kml .= "      <LineString>\n";
	$kml .= "       <coordinates>".$coordinatesList."</coordinates>\n";
	$kml .= "      </LineString>\n";
	$kml .= "    </Placemark>\n";
echo $kml;
}
 
function getCirclecoordinates($lat, $long, $meter) {
  // convert coordinates to radians
  $lat1 = deg2rad((float)$lat);
  $long1 = deg2rad((float)$long);
  $d_rad = ((float)$meter)/6378137;
 
  $coordinatesList = "";
  // loop through the array and write path linestrings
  for($i=0; $i<=360; $i+=10) {
    $radial = deg2rad((float)$i);
    $lat_rad = asin(sin($lat1)*cos($d_rad) + cos($lat1)*sin($d_rad)*cos($radial));
    $dlon_rad = atan2(sin($radial)*sin($d_rad)*cos($lat1), cos($d_rad)-sin($lat1)*sin($lat_rad));
    $lon_rad = fmod(($long1+$dlon_rad + M_PI), 2*M_PI) - M_PI;
    $coordinatesList .= rad2deg($lon_rad).",".rad2deg($lat_rad).",0 ";
  }
  return $coordinatesList;
}
