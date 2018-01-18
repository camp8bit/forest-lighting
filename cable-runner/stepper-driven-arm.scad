

$fn = 80;

innerSize = [150, 80, 45];
thickness = 2;

length = innerSize[0] + 2*thickness;
width = innerSize[1] + 2*thickness;
height = innerSize[2] + 2*thickness;

speakerR = 14;

armSize = [80, 25, 8];
axleL = 40;
axleR = 4;

pulleyR = 20;
pulleyW = 15;
 
lowerArmH = 50;
lowerArmW = 8;

fanR = 33.5; 
fanEndR = 72/2;
fanEndH = 10;

outerFanR = fanR+thickness;
boxAngle = 45;
outerBoxR = height/2/sin(boxAngle);
outerBoxCurveL = (1 - sin(boxAngle)) * outerBoxR;

lidSize = [ innerSize.x - cos(boxAngle)*outerBoxR, innerSize.y, thickness];
lidIndent = 6;

// How far from the centre are the bottom bolts
bottomBoltShift = innerSize.x/2-30; 

// How far from the centre is the conduit for wires
bottomConduitShift = innerSize.x/2-50;
bottomConduitR = 8;

// Parameters of the middle arm 
middleArmAngle = 60;
middleArmR = (lowerArmH + axleR + 1) / sin(middleArmAngle );
middleFootOffset = middleArmR * cos(middleArmAngle);

armBoltGap = 5;
armBoltDistance = armSize[1]-armBoltGap;
middleArmBoltGap = 12.5;

outerArmGap = height-armSize[2]*2;

lidCorner=12;
rotate([90,0,0])
  translate([0,25,0])
    arm();

 module arm() {
     // Amount of arms that are purely vertical
     flatH = 14;
     
     translate([armSize[0]/2,0,0]) {
        // Mount plate for motor
         difference() {
             //Main body
             union () {
               intersection() {
                  translate([0,0,-(outerBoxR-armSize[1])]) rotate([0,90,0])
                      cylinder(h=armSize[0], r=outerBoxR, center=true);
                  translate([0,0,armSize[1]/2])
                      cube(size=[armSize[0], height, armSize[1]], center=true);
               }

             // mounting plate for stepper
             translate([30,-20.5,10])
               rotate([0,0,0]) {
                 cube([45,8,45], center=true);
               }
             }
  
             // stepper mount holes
             translate([30,-20.5,10]) {
               rotate([0,0,0]) {
                   translate([15,10,15])
                     rotate([90,0,0])
                       cylinder(h=100, r=1.5);
               }
           }
            
             // Split into 2 arms
             smallR = outerArmGap/2;
             bigR = (armSize[0]-armSize[2]) * 1.2 - flatH;
             union () {
               intersection() {
                 translate([-armSize[0]/2+armSize[2],-50,-50]) cube(size=100);
                 translate([height - flatH - 50,0,-50])
                    scale([1,1,1]) cylinder(h=100,r=smallR);
               }
               translate([34, 0, 0]) 
                 cube([100,smallR * 2,50], center=true);

               translate([19, 16, 0]) 
                 cube([100,smallR * 2,50], center=true);
             }
             
             // Bolt holes
             for(transZ = [armBoltGap, armBoltDistance])
                translate([armSize[2]-armSize[0]/2+.01 + 1, 0, transZ])
                    rotate([0,90,0]) m3_bolt_hole(length=100);
             
            // Top bolt hole
            translate([
                armSize[0]/2-10,
                -height/2 - .01 - 30,
                (armSize[1]-5)/2
            ]) rotate([90,0,0])
                m8_square_end_bolt_hole(length=100);

         }
     }
     
     wA = armSize[1]/2;
     wB = armSize[2]/2;
     h = armSize[0];
     offset = armSize[2];
}

/**
 * Countersunk hole for a n M3 bole
 * Drills down in the Z-axis with the origin at the top of the bolt
 */
module m3_bolt_hole(length, countersink=true) {
    headR = 3;
    shaftR = 1.75;
    headH = 3.25;
    
    scale([1,1,-1]) {
        if(countersink) {
            cylinder(r=headR, h=headH, $fs=.5);
        }
        cylinder(r=shaftR, h=length, $fs=.5);
    }
}

/**
 * Bolt hole with a square start for round-head M8 bolts
 */
module m8_square_end_bolt_hole(length) {
    headSide = 8.5;
    shaftR = 4.25;
    headH = 4.5;
    
    scale([1,1,-1]) {
        translate([0,0,headH/2])
            cube([headSide,headSide,headH], center=true);
        cylinder(r=shaftR, h=length, $fs=.5);
    }
}