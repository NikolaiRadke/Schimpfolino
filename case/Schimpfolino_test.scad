// Case for Schimpf-O-Mat with 0.96" SSD1306 Display and one button

// Round cylinder shapes
$fa = 1;
$fs = 0.5;

difference() {
 minkowski() {
  // Außen abgerunded
  cube([36,46,10]);
  cylinder(r=1, h=1);
 }
 
 // Innen
 translate([2,2,2])
  cube([32,42,10]);
 
 // Display
 translate([4.6,6.6,0])
  cube([26.5,16.7,2]);
 
 // Button
 translate([18,34.7,0])
  cylinder(d=4, h=4);      
} 