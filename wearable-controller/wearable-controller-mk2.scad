
exploded_view();

$fn = 40;
$fs = 1;

innerD = 35;
innerL = 75;
thickness = 2;
cutoffA = 45;
rounding = 4;

// Intermediary vars
outerD = innerD + 2*thickness;
outerL = innerL + 2*thickness + rounding*2;
lift = outerD/2*sin(cutoffA);
holeWidth = innerD*cos(cutoffA);

module exploded_view() {
    body();
    //translate([0,0,outerD-lift + 20]) lid();


    // Battery
//    # translate([-3,8,13]) rotate([0,90,0]) cylinder(r=9, h=65, center=true);
    
    // nano
//    # translate([0,0,5]) cube([43.18,1,17.78]);
}

module body() {
    difference() {
        // Outer cylinder
        translate([0,0,lift])
            rotate([0, 90, 0])
               rounded_cylinder(h=outerL, r1=outerD/2, r2=rounding);
 
        // Hollow interior
        difference() {
            translate([0,0,lift])
                rotate([0, 90, 0])
                    cylinder(h=innerL, r=innerD/2, center=true);
         
            // Shear off the hollowing too, to keep a button-plate
            translate([0,0,-500 + thickness]) cube(size=[1000,1000,1000], center=true);
            translate([0,0,500 + innerD-lift]) cube(size=[1000,1000,1000], center=true);
        }

        
        // Shear off face-plate
        translate([0,0,-500]) cube(size=[1000,1000,1000], center=true);

        // Shear off back-plate
        translate([0,0,500 + outerD-lift]) cube(size=[1000,1000,1000], center=true);

        // Cut out back
       translate([0,0,50 + 10]) cube([innerL, holeWidth, 100], center=true);
       translate([0,0,50 + outerD-lift - 2]) cube([innerL+8, holeWidth+2, 100], center=true);
        
        // Screw holes
        translate([0,0,outerD-lift]) screw_holes();

       // Button holes 
        scale([1,1,-1]) {
            translate([28,6,-1]) button_pin_holes();
            translate([28,-6,-1]) button_pin_holes()    ;
            translate([-20,7,0]) cube(size=[11.5,6,10], center=true);
        }
        
        // End socket
//        cylinder(h=50
        
    }
}

module lid() {
    difference() {
        translate([0,0,1]) cube([innerL+8, holeWidth+2, 2], center=true);
        translate([0,0,2]) screw_holes();
        
        // Etching
        translate([-12.5,-10,2]) linear_extrude(height = 2, center = true, convexity = 10)
            import (file = "invaderout.dxf");

    }
}

// 4 screw holes to go into the lid and box
module screw_holes() {
    for(scaleA=[1,-1]) for(scaleB=[1,-1]) {
        scale([scaleA, scaleB, 1]) translate([innerL/2+2,holeWidth/2-2,0]) screw_hole_2g();
    }
}

// Models a rounded cylinder by capping with torii
module rounded_cylinder(h, r1, r2) {
    union() {
        cylinder(h = h-r2*2, r=r1, center=true);
        cylinder(h = h, r=r1-r2, center=true);
        rotate_extrude(convexity = 10) {
            translate([r1-r2, h/2 - r2, 0])
                circle(r = r2);
            translate([r1-r2, -h/2 + r2, 0])
                circle(r = r2);
        }
    }
}

/**
 * Holes for the pins of a push button switch
 */
module button_pin_holes() {
    for(scaleA=[2.5,-2.5]) for(scaleB=[3,-3]) {
        translate([scaleA,scaleB,0]) cylinder(h=10,r=0.8, center=true);
        translate([0,0,5]) cube(size=[7,8,10], center=true);        
    }
}

/**
 * Hole for a 2g screw of up to 10mm
 * Origin is at the top of the hole
 */
module screw_hole_2g() {
    rWide = 1.5;
    rNarrow = 0.5;
    countersink = 1;
    length = 15;
    
    translate([0,0,-countersink]) cylinder(r = rWide, h = countersink);
    translate([0,0,-length]) cylinder(r = rNarrow, h = length-countersink);
}
