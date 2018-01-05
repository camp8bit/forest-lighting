$fn=40;

circumference = 400; // 24 led of 60/m strip
ledHeight = 12;
mountHeight = 20;
thick = 2;
bigHole = 11;
littleHole = 5.25;

outerR = circumference / PI / 2;

//exploded_view();
plated_view();


module plated_view() {
    mount();
    translate([-outerR/2,0,mountHeight]) scale([1,1,-1]) pegs();
}

module exploded_view() {
    mount();
    translate([0,0,40]) pegs();
}
module mount() {

    difference() {
        union() {
            //connecting struts
            for(rot = [0,120,240]) {
                rotate([0,0,rot])
                    translate([0,-ledHeight/2,0])
                        cube(size=[outerR-(thick/2),ledHeight,thick]);    
            }
            // centre mount
            cylinder(r = bigHole+thick, h = mountHeight);
        }

        translate([0,0,-1]) cylinder(r = bigHole, h = mountHeight+2);
        
        // Wire hole
        translate([10,-4,2]) cube(size=[10,8,3]);
    };

    // Outer ring
    difference() {
        cylinder(r = outerR, h = ledHeight);
        translate([0,0,-1]) cylinder(r = outerR-thick, h = ledHeight+2);
        // Wire hole
       translate([outerR,2.5,ledHeight/2]) rotate([0,0,45]) cube(size=[10,3,8], center=true);
        
    };
}

module pegs() {
    difference() {
        cylinder(r1 = bigHole - 2, r2 = bigHole + 0.5, h = mountHeight);
        translate([0,0,-1]) cylinder(r = littleHole, h = mountHeight+2);
        translate([0,0,mountHeight/2])
            cube([littleHole*2-3, bigHole*2+2, mountHeight+2], center=true);
    }
}