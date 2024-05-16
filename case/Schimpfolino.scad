//OLED housing (0.96'' - SSD1306 display + WEMOS D1 mini)


spaceh=21.5;                                                  //Spacing vor mounting holes of OLED horizontally
spacev=21.5;                                                  //Spacing vor mounting holes of OLED vertically


$fn=30;

difference(){
 minkowski(){
  // Außen mit runder Kontur
  cube([33.5,46.5,15], center=true); 
   cylinder(r=2, h=2, center=true);
  }
  // Innen
  translate([0,0,1.1])
   cube([31.5,44.5,15],center=true);
  
  // Display
  hull(){
   translate([0,5.75,-7.8])
    cube([24,13.5,3],center=true);
   translate([0,5.75,-9.1])
    cube([28,17.5,1], center=true);
   }
    translate([0,-18,-7])cylinder(d=4, h=4, center=true);      //hole for push button
    translate([0,1,7.3])cube([33,41,1.2], center=true);        //groove for the lid
    translate([0,1,8.3])cube([32,40,1.2], center=true);        //groove for the lid
        //holu for USB
}

translate([0,-18,-5.9])difference(){                           //recess for push button
    cube([31.5,8,1], center=true);
    cube([6.8,6.8,1.2], center=true);
}