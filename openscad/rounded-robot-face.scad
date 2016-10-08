$fs=1;

ORB_D=22.5;
pirDiameter = ORB_D;

BOX_W=23.3;
BOARD_W=32.5;
BOARD_H=24.3;
MOUNT_SEP=29;

CLEARANCE=1;

ENCLOSURE_HEIGHT=25;
FRONT_THICK=4;
BOX_DEPTH=2;

OVERSIZE = 20;

STAKE_LENGTH=40;

faceMain();
translate([0,0,50])
    facePlate();

// Inter pupilllary distance
IPD = 30;

// For preventing z-fighting
epsilon = 0.1;

// Thickness of all walls
thickness = 3;

// This has to be enough to accomodate the PIR PCB
occipitalDiameterA = 44 + thickness * 2;
occipitalDiameterB = 42 - 10;

// Fake eye
eyeDiameter = pirDiameter - 3;

// Depth of the face - needs to be deep enough for a nano and PIR(21mm)
faceDepth = 30;

// The lower part of the face
jawWidth = 30;

// The jaw needs to be tall enough for a nano + a buck converter (43mm each)
jawDepth = 60;

// How far down the face is the jaw?
jawOffset = 40;

// Size of the mouth
mouthSize = 20;

// Step size
stepSize = thickness * 2;

module faceMain() {
    difference () {
        union () {
            translate([-IPD / 2, 0, 0])
                cylinder(h=faceDepth, d=occipitalDiameterA + thickness, center=true);
            translate([IPD / 2, 0, 0])
                cylinder(h=faceDepth, d=occipitalDiameterB + thickness, center=true);

            translate([0, jawDepth - jawOffset, 0]) {
                cube([jawWidth + thickness, jawDepth + thickness, faceDepth], center=true);
                translate([0, jawDepth / 2, 0]) {
                    cylinder(h=faceDepth, d=jawWidth + thickness, center=true);
                }
            }

            // Cable hole
            translate([0, jawDepth / 2 + jawOffset - 5, 0])
                rotate([90,0,0])
                    cylinder(h=5, d=10, center = true);
            
            // Bottom Mounting tab
            translate([0, 72, -faceDepth / 2 + thickness / 2]){
                difference () {
                    union () {
                        translate([0, -7, 0])
                            cube([15, 15, thickness], center = true);
                        cylinder(h=thickness, d = 15, center = true);
                    }
                    cylinder(h=thickness + epsilon, d = 6, center = true);
                }
            }

            // Top Mounting tab
            translate([0, -28, -faceDepth / 2 + thickness / 2]){
                difference () {
                    union () {
                        translate([0, 7, 0])
                            cube([15, 15, thickness], center = true);
                        cylinder(h=thickness, d = 15, center = true);
                    }
                    cylinder(h=thickness + epsilon, d = 6, center = true);
                }
            }
        }
        
        union () {
            // Cable hole
            translate([0, jawDepth / 2 + jawOffset - 5, 0])
                rotate([90,0,0])
                    cylinder(h=10 + epsilon, d=8, center = true);
            
            // Interior space
            translate([0, 0, thickness + epsilon - thickness]) {
                translate([-IPD / 2, 0, 0])
                    cylinder(h=faceDepth + epsilon - thickness, d=occipitalDiameterA - stepSize, center=true);
                translate([IPD / 2, 0, 0])
                    cylinder(h=faceDepth + epsilon - thickness, d=occipitalDiameterB - stepSize, center=true);

                translate([0, jawDepth - jawOffset, 0]) {
                    cube([jawWidth - stepSize, jawDepth - stepSize, faceDepth + epsilon - thickness], center=true);
                    translate([0, jawDepth / 2, 0]) {
                        cylinder(h=faceDepth + epsilon - thickness, d=jawWidth - stepSize, center=true);
                    }
                }
            }

            // Step
            translate([0, 0, faceDepth / 2 - thickness / 2]) {
                translate([-IPD / 2, 0, 0])
                    cylinder(h=epsilon + thickness, d=occipitalDiameterA, center=true);
                translate([IPD / 2, 0, 0])
                    cylinder(h=epsilon + thickness, d=occipitalDiameterB, center=true);

                translate([0, jawDepth - jawOffset, 0]) {
                    cube([jawWidth, jawDepth, thickness + epsilon], center=true);
                    translate([0, jawDepth / 2, 0]) {
                        cylinder(h=thickness + epsilon, d=jawWidth, center=true);
                    }
                }
            }
            
        }
    }
}

module facePlate() {
    union () {
        // Fake eye
        translate([IPD / 2, 0, 0]) {
            difference() {
                sphere(d=eyeDiameter, center=true);
                translate([0, 0, -eyeDiameter / 2])
                    cube([eyeDiameter, eyeDiameter, eyeDiameter], center = true);
            }
        }
        
        // Occipital
        difference () {
            union () {
                translate([-IPD / 2, 0, 0])
                    cylinder(h=thickness, d=occipitalDiameterA, center=true);
                translate([IPD / 2, 0, 0])
                    cylinder(h=thickness, d=occipitalDiameterB, center=true);

                // Jaw
                translate([0, jawDepth - jawOffset, 0]) {
                    difference() {
                        union () {
                            cube([jawWidth, jawDepth, thickness], center=true);
                            translate([0, jawDepth / 2, 0])
                                cylinder(h=thickness, d=jawWidth, center=true);
                        }

                        translate([0, jawDepth / 2, thickness / 2]) {
                            difference () {
                                // Mouth hole
                                cylinder(h=thickness, d=mouthSize, center=true);

                                // Teeth
                                union () {
                                    translate([-5, -8, 0])
                                        cube([5, 10, thickness + epsilon], center = true);
                                    translate([5, -8, 0])
                                        cube([5, 10, thickness + epsilon], center = true);
                                    translate([0, 10, 0])
                                        cube([5, 10, thickness + epsilon], center = true);
                                }
                            }
                        }                            
                    }
                }
            }

            translate([-IPD / 2, 0, 0])
                cylinder(h=thickness + epsilon, d=ORB_D, center=true);
        }
        
    }
}

/*    
    translate([0,-(STAKE_LENGTH/2)-(BOARD_H+5)/2,-ENCLOSURE_HEIGHT/2+1.5])

    union() {
        cube([6,STAKE_LENGTH,3], center=true);
        translate([0,-STAKE_LENGTH/2,0])
        scale([1,3,1])
        rotate([0,0,45])
            cube([6/1.414,6/1.414,3], center=true);
    }
    */

    

module lid() {
    union() {
        roundedCube(BOARD_W+OVERSIZE+5, BOARD_H+5, 2, 5);
        translate([0,0,-2.5])
            difference() {
                cube([BOARD_W+OVERSIZE+1.5, BOARD_H+1.5, 3], center=true);
                translate([-BOARD_W / 2,BOARD_H*0,0])
                    cube([BOARD_H,5,3.5], center=true);
            }

    }
}

module roundedCube(x, y, z, rounding) {
    
    cylX = (x-rounding)/2;
    cylY = (y-rounding)/2;
    
    union() {
        cube([x-rounding, y, z], center=true);
        cube([x, y-rounding, z], center=true);
        
        for(a=[-cylX, cylX])
            for(b=[-cylY, cylY])
                translate([a,b,0])
                    cylinder(d=rounding,h=z, center=true);
        }
    }