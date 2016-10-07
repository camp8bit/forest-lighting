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

box();
translate([0,0,ENCLOSURE_HEIGHT+10])
rotate([0,180,0])
    lid();

module box() {
    union () {

        translate([BOARD_W, 8, 0])
            difference () {
                cube([10,10,ENCLOSURE_HEIGHT], center = true);
                cube([5,5,ENCLOSURE_HEIGHT + 2], center = true);
            }
        
        translate([-BOARD_W, -8, 0])
            difference () {
                cube([10,10,ENCLOSURE_HEIGHT], center = true);
                cube([5,5,ENCLOSURE_HEIGHT + 2], center = true);
            }
        
        difference() {
            roundedCube(BOARD_W+ OVERSIZE + 5, BOARD_H+5, ENCLOSURE_HEIGHT, 5);
            
            translate([0,0,FRONT_THICK])
                cube([BOARD_W+2 + OVERSIZE, BOARD_H+2,ENCLOSURE_HEIGHT], center=true);
            
            translate([0,0,FRONT_THICK-BOX_DEPTH])
                cube([BOX_W+CLEARANCE + OVERSIZE + 5,BOX_W+CLEARANCE,ENCLOSURE_HEIGHT], center=true);

            cylinder(d=ORB_D+CLEARANCE, h=ENCLOSURE_HEIGHT+1, center=true);
            
            translate([(BOARD_W + OVERSIZE + 5) / 2,-(BOARD_H)*0,ENCLOSURE_HEIGHT-2])
                cube([5,5,ENCLOSURE_HEIGHT], center=true);
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