$fn=40;

innerSize = [120, 30, 10];
thickness = 3;
r = 3;

screwR = 2;

plated_view();
// exploded_view();

module plated_view() {
    translate([0,0,0]) box_bottom();
    translate([0,innerSize[1] + 10,0]) box_top();
}

module exploded_view() {
    box_bottom();
    // translate([0,0,20]) box_top();
}

module box_bottom() {
    difference() {
        thicknessV = [thickness, thickness, thickness];
        
        // exterior
        flattened_rounded_cube(size = innerSize + thicknessV, r = r);
        
        // interior void
        translate([0,0,20]) z_rounded_cube(size = innerSize + [0,0,40], r = r);
        
        // lip
        translate([0,0,8]) z_rounded_cube(size = innerSize + [2,2,2], r = r);
        
        // make top of cube flush
        translate([0,0,12]) cube(size = innerSize + thicknessV + thicknessV, center = true );
    }
}

module box_top() {
    difference() {
        thicknessV = [thickness, thickness, thickness];
        flattened_rounded_cube(size = innerSize + thicknessV, r = r);
        translate([0,0,20]) z_rounded_cube(size = innerSize + [0,0,40], r = r);
        translate([0,0,8]) z_rounded_cube(size = innerSize + [2,2,2], r = r);
        translate([0,0,12]) cube(size = innerSize + thicknessV + thicknessV, center = true );
    }
}



/**
 * Hole for a countersunk 4g screw of up to 15mm
 * Origin is at the top of the hole
 */
module screw_hole_4g_countersunk() {
    rWide = 2.5;
    rNarrow = 1;
    countersink = 2;
    
    translate([0,0,-countersink]) cylinder(r1 = rNarrow, r2 = rWide, h = countersink);
    translate([0,0,-15]) cylinder(r = rNarrow, h = 15-countersink);
}


/**
 * Rounded cube flattened on the bottom for better printing.
 */
module flattened_rounded_cube(size, r) {
    overhang = (1-sin(45)) * r;
    
    intersection() {
        translate([0,0,-overhang/2]) rounded_cube(size + [0,0,overhang], r);
        cube(size, center = true);
    }
}

/**
 * Cube that is rounded on all 12 edges. Centered on the origin
 */
module rounded_cube(size, r) {
    smallDims = size - [2*r, 2*r, 2*r];
    halfDims = smallDims / 2;

    // Body
    cube(size=[size[0], smallDims[1], smallDims[2] ], center = true);
    cube(size=[ smallDims[0], size[1], smallDims[2] ], center = true);
    cube(size=[size[0] - 2*r, smallDims[1], size[2] ], center = true);

    // Edges
    for(scaleA=[1,-1]) for(scaleB=[1,-1]) {
        translate([ 0, halfDims[1]*scaleA,halfDims[2]*scaleB ])
            rotate([0,90,0]) cylinder(r=r, h= smallDims[0], center = true);
        translate([ halfDims[0]*scaleA, 0, halfDims[2]*scaleB ])
            rotate([90,0,0]) cylinder(r=r, h= smallDims[1], center = true);
        translate([ halfDims[0]*scaleA, halfDims[1]*scaleB, 0 ])
            cylinder(r=r, h= smallDims[2], center = true);
    }
    
    // Corners
    for(scaleX=[1,-1]) for(scaleY=[1,-1]) for(scaleZ=[1,-1]) {
       translate([ halfDims[0]*scaleX, halfDims[1]*scaleY, halfDims[2]*scaleZ ])
            sphere(r=r);
    }
    
    
}

/**
 * Cube that is rounded on the z edges only
 */    
module z_rounded_cube(size, r) {
    smallDims = size - [2*r, 2*r, 2*r];
    halfDims = smallDims / 2;

    // Body
    cube(size=[size[0], smallDims[1], size[2] ], center = true);
    cube(size=[ smallDims[0], size[1], size[2] ], center = true);

    // Edges
    for(scaleA=[1,-1]) for(scaleB=[1,-1]) {
        translate([ halfDims[0]*scaleA, halfDims[1]*scaleB, 0 ])
            cylinder(r=r, h = size[2], center = true);
    }
}