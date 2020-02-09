<?php

// Modified https://knackforge.com/blog/narendran/kml-google-map-circle-generator-script-php
// Plots a KML circle for the LOP around the Capella GP.

$lat = '46.4867';
$long = '11.0733';
$meter = 5624000;

$coordinatesList2 = getCirclecoordinates($lat, $long, $meter);
$kml  = "    <Placemark>\n";
$kml .= "     <name>Capella - circle</name>\n";
$kml .= "      <styleUrl>#redBearingStyle</styleUrl>\n";
$kml .= "      <LineString>\n";
$kml .= "       <coordinates>".$coordinatesList."</coordinates>\n";
$kml .= "      </LineString>\n";
$kml .= "    </Placemark>\n";
echo $kml;
 
function getCirclecoordinates($lat, $long, $meter) {
  $lat1 = deg2rad((float)$lat);
  $long1 = deg2rad((float)$long);
  $d_rad = ((float)$meter)/6378137;
 
  $coordinatesList = "";
  for($i=0; $i<=360; $i+=10) {
    $radial = deg2rad((float)$i);
    $lat_rad = asin(sin($lat1)*cos($d_rad) + cos($lat1)*sin($d_rad)*cos($radial));
    $dlon_rad = atan2(sin($radial)*sin($d_rad)*cos($lat1), cos($d_rad)-sin($lat1)*sin($lat_rad));
    $lon_rad = fmod(($long1+$dlon_rad + M_PI), 2*M_PI) - M_PI;
    $coordinatesList .= rad2deg($lon_rad).",".rad2deg($lat_rad).",0 ";
  }
  return $coordinatesList;
}
