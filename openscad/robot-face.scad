$fs=1;

ORB_D=22.2;
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

IPD = 50;

// For preventing z-fighting
epsilon = 0.1;

// Thickness of all walls
thickness = 2;

// This has to be enough to accomodate the PIR PCB
occipitalDiameterA = 42 + 30;
occipitalDiameterB = 42 + 15 ;
faceDepth = 20;

// The lower part of the face
jawWidth = 50;
jawDepth = 80;

// How far down the face is the jaw?
jawOffset = 40;

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
        }
        
        union () {
            translate([0, 0, thickness + epsilon]) {
                translate([-IPD / 2, 0, 0])
                    cylinder(h=faceDepth + epsilon, d=occipitalDiameterA, center=true);
                translate([IPD / 2, 0, 0])
                    cylinder(h=faceDepth + epsilon, d=occipitalDiameterB, center=true);

                translate([0, jawDepth - jawOffset, 0]) {
                    cube([jawWidth, jawDepth, faceDepth + epsilon], center=true);
                    translate([0, jawDepth / 2, 0]) {
                        cylinder(h=faceDepth + epsilon, d=jawWidth, center=true);
                    }
                }
            }
        }
    }
}

module facePlate() {
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