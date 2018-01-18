/**
 * Cable Runner - Dec 2017
 */
 
use <spline.scad>
use <stepper-driven-arm.scad>

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

//middlearm(); translate([lowerArmH,50,0]) middlearm2();

//box();
//test_box();
//m3_nut_trap(up=4, down=30);
// rotate([0,180,0]) lid();
drivePulley();
//stepper_arm();
//arm();
//box();
//test_box();
//m3_nut_trap(up=4, down=30);
//rotate([0,180,0]) lid();
//pulley();
//pulley_spacer();
//test_undercarriage();
//undercarriage();

//exploded_view( explode = (cos($t*360)+1)*25);
exploded_view( explode = 0);

module exploded_view(explode) {
    translate([0,height/2,width/2]) rotate([90,0,0]) {
        color("Gray") box();
        translate([0,0,height + explode]) color("Gray") lid();
    }
    translate([length/2,0,-explode]) rotate([0,-90,0]) color("Gray") undercarriage();
     
    translate([0,0,width + explode]) {
        translate([(length)/2-armSize[1],0,0])
            rotate([0,-90,180]) {
                color("Gray") arm();
                translate([armSize[0]-10,pulleyW/2,armSize[1]/2+explode])
                    rotate([90,0,0]) color("Red") pulley();
            }

        translate([0,(height) /2, 0])
            rotate([0,-90,90]) {
                color("Gray") middlearm();
                translate([0,0,height]) rotate([0,180,0]) color("Gray") middlearm2();
                translate([lowerArmH + explode,0, (height-pulleyW)/2]) color("Red") pulley();
            }
        

        rotate([0,0,180]) translate([(length)/2-armSize[1],0,0])
            rotate([0,-90,180]) {
                color("Gray") stepper_arm();
                translate([armSize[0]-10,pulleyW/2,armSize[1]/2+explode])
                    rotate([90,0,0]) color("Red") pulley();
            }


    }
}


 module drivenArm() {
     // Amount of arms that are purely vertical
     flatH = 14;
     
     translate([armSize[0]/2,0,0]) {
         difference() {
             //Main body
             intersection() {
                translate([0,0,armSize[1]/2])
                    cube(size=[armSize[0], 25 * 2, 45], center=true);
             }
             
             // One side
             translate([20, 10, 0])
               cube(size=[50, 50, 100], center = true);
             
             // Bolt holes
             for(transZ = [armBoltGap, armBoltDistance])
                translate([armSize[2]-armSize[0]/2+.01, 0, transZ])
                    rotate([0,90,0]) m3_bolt_hole(length=40);
         }
     }
     
     wA = armSize[1]/2;
     wB = armSize[2]/2;
     h = armSize[0];
     offset = armSize[2];
}

module lid() {
    spkR = 53.5/2;
    pirR = 13;
    potR = 3.5;

    powerSwW = 19.5;
    powerSwH = 12.5;

    difference() {
        union() {
            translate([0,0,-lidSize.z/2]) cube(lidSize-[.5,.5,0], center=true);
           
            // Corners
            for(x=[1,-1], y=[1,-1]) scale([x,y,1])
                translate([-lidSize.x/2+.25,-lidSize.y/2+.25,-lidIndent])
                    cube([lidCorner,lidCorner,lidIndent]);
        }

        // Corner bolts
        for(x=[1,-1], y=[1,-1]) scale([x,y,1])
            translate([-(lidSize.x-lidCorner)/2,-(lidSize.y-lidCorner)/2,0.01])
                m3_bolt_hole(length=10);
        
        // Switch
        translate([-40,-25,1]) switch_mount();
        
        // Potentiometer
        translate([-20,-25,-5]) cylinder(r=potR, h=10);

        // PIR sensor
        translate([-35,0,-5]) cylinder(r=pirR, h=10);

        // Speaker
        translate([25,0,-5]) cylinder(r=spkR, h=10);

        // Power switch
        translate([-35,25,0]) cube([powerSwW,powerSwH,10], center=true);
    }
}

module switch_mount(down = 10, up=5) {
    switchPinH = 5;
    switchPinW = 12;
    switchPinR = 1;
    switchW = 13.5;
    switchH = 12.5;
    
    union() {
        for(x = [-1,1], y = [1,-1]) scale([x,y,1]) {
            translate([switchPinW/2, switchPinH/2, -down]) cylinder(r=switchPinR, h=down+1);
        }
        //translate([-switchW/2,-switchH/2,0]) cube([switchW, switchH, up]); 
    }
}

module test_box() {
    intersection() {
        cube(1000);
        translate([-40,-20,-17]) box();
    }
}

module box() {
    translate([0,-15,height/2])
        difference() {
            y_axis_rounded_cube(size=[length,width+30,height], r=outerBoxR);

            translate([0,15,0]) {
                // Hollow
                cube(size=[lidSize.x-lidCorner*2,innerSize.y,innerSize.z], center=true);
                y_axis_rounded_cube(size=innerSize-[0,lidCorner*2,0], r=outerBoxR-thickness);

                // Lid hole
                translate([0,0,height/2 + 9.99 - lidIndent])
                    cube(size=[lidSize.x,lidSize.y,20], center=true );

                // Nut traps - lid - 10mm M3 bolts
                for(scaleX=[1,-1]) for(scaleY=[1,-1]) scale([scaleX, scaleY, 1])
                    translate([lidSize.x/2-lidCorner/2, lidSize.y/2-lidCorner/2,innerSize.z/2-lidIndent-3])
                        rotate([0,0,180]) m3_side_nut_trap(up=6, down=6, side=10);

                // Nut traps - arms - 16mm M3 bolts
                for(scaleX=[1,-1]) scale([scaleX, 1, 1]) {
                translate([-length/2 + armBoltGap, lidSize.y/2-lidCorner + 4, 0])
                    rotate([90,0,0]) rotate([0,0,90]) m3_nut_trap(up=6, down=30);
                translate([-length/2 + armBoltDistance, lidSize.y/2-lidCorner + 4,0])
                    rotate([90,0,0]) m3_nut_trap(up=6, down=30);
                }

                // Bolt holes - middle arm
                for(zScale = [1,-1]) 
                    translate([middleFootOffset - armSize[1]/2, width/2 + .01,(height/2-middleArmBoltGap) * zScale]) rotate([-90,0,0])
                        m3_bolt_hole(20, countersink=false);
                
            }
            
            // Cutaway for bottom shell to sit in
            translate([length/2,-(outerFanR+width+thickness*4)/2,0])
                rotate([0,90,180])
                    undercarriage_cylinder();
            
            // Bolt holes - bottom
            for(xScale = [1,-1]) scale([xScale,1,1])
                translate([bottomBoltShift,15.01-innerSize.y/2,0]) rotate([-90,0,0])
                    m3_bolt_hole(20, countersink=false);

            // Hole for electronics - bottom
            translate([-bottomConduitShift,16-innerSize.y/2,0])
                rotate([0,90,90]) cylinder(h=30, r=bottomConduitR, center=true);
            
        }
    /*    
            // Speaker cut-out
            translate([-innerSize[0]/2,0,0])
            rotate([0,90,0])
                cylinder(h=50,r=speakerR,center=true);
         
     */
 }
 
 module arm() {
     // Amount of arms that are purely vertical
     flatH = 14;
     
     translate([armSize[0]/2,0,0]) {
         difference() {
             //Main body
             intersection() {
                translate([0,0,-(outerBoxR-armSize[1])]) rotate([0,90,0])
                    cylinder(h=armSize[0], r=outerBoxR, center=true);
                translate([0,0,armSize[1]/2])
                    cube(size=[armSize[0], height, armSize[1]], center=true);
             }
             
             // Split into 2 arms
             smallR = outerArmGap/2;
             bigR = (armSize[0]-armSize[2]) * 1.2 - flatH;
             intersection() {
                 translate([-armSize[0]/2+armSize[2],-500,-500]) cube(size=1000);
                 translate([height - flatH,0,-50])
                    scale([bigR/smallR,1,1]) cylinder(h=100,r=smallR);
             }
             
             // Bolt holes
             for(transZ = [armBoltGap, armBoltDistance])
                translate([armSize[2]-armSize[0]/2+.01, 0, transZ])
                    rotate([0,90,0]) m3_bolt_hole(length=40);
             
            // Top bolt hole
            translate([
                armSize[0]/2-10,
                -height/2 - .01,
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

module pulley_spacer() {
    innerR = 4.25;
    outerR = innerR + 1;
    pulleyW = 7;

    spacerH = (outerArmGap - pulleyW - 1) /2;

    difference() {
        cylinder(r=outerR, h=spacerH);
        translate([0,0,-1])cylinder(r=innerR, h =spacerH+2);
    }
}
 
module middlearm(boltPull = 0) {
    cutaway = 39;//36;
    armThickness = 11;
    bendThickness = 2;
    
    // Bent arm (to allow flexing)
    translate([0,middleFootOffset,0])
        difference() {
            union() {
                difference() {
                    cylinder(h=lowerArmW, r=middleArmR);
                    translate([0,0,-1]) cylinder(h=lowerArmW +2, r=middleArmR-armThickness);

                    translate([cutaway * cos(middleArmAngle),-cutaway * sin(middleArmAngle),-1])
                        cylinder(h=lowerArmW +2, r=middleArmR - cutaway - bendThickness);

                    translate([-300, -middleFootOffset, -300]) cube(size=600);
                    translate([-600, -300, -300]) cube(size=600);
                }
                translate([lowerArmH-.75,-middleFootOffset+1.5,-.01])
                    cylinder(d=armThickness+3,h=lowerArmW);

                translate([0,-middleArmR,0])
                cube(size = [armSize[2], armSize[1], height/2-0.25]);
            }


            // Top bolt hole
            translate([lowerArmH-.75,-middleFootOffset+1.5,-.02 - boltPull])
                scale([1,1,-1]) m8_square_end_bolt_hole(length=50);

            // Bottom bolt holes
            translate([armSize[2]+.01,armSize[1]/2 -middleArmR, middleArmBoltGap])
                rotate([0,90,0]) m3_bolt_hole(length=40);

       }
}
module middlearm2() {
    scale([-1,1,1]) middlearm(boltPull = 10);
}

module drivePulley() {
    bearingR = 11.25;
    bearingW = 8;
    bearingHold = 1;
    cableW = 3; 
    innerR = pulleyR - (pulleyW/2 - cableW / 2);
    axleGap = axleR + 4;

    //# cylinder(r=11, h= 20);

    insetForAxel = pulleyW / 2 + 2;
    
    difference() {
        translate([0,0,pulleyW/2]) rotate_extrude()
            rotate([0,0,-90]) polygon(points = [
                // Main shape
                [pulleyW/2,2.5], [pulleyW/2,pulleyR],
                // Round for cable
                [cableW/2,innerR], [0, innerR-cableW*.2], [-cableW/2,innerR],
                [-pulleyW/2,pulleyR],
                [-pulleyW/2,5],
                [-pulleyW/2 + insetForAxel,5],
                [-pulleyW/2 + insetForAxel,2.5],
                ]); 

    }
         
 }
 
module pulley() {
    bearingR = 11.25;
    bearingW = 8;
    bearingHold = 1;
    cableW = 3; 
    innerR = pulleyR - (pulleyW/2 - cableW / 2);
    axleGap = axleR + 4;

    //# cylinder(r=11, h= 20);

    difference() {
        translate([0,0,pulleyW/2]) rotate_extrude()
            rotate([0,0,-90]) polygon(points = [
                // Main shape
                [pulleyW/2,axleGap], [pulleyW/2,pulleyR],
                // Round for cable
                [cableW/2,innerR], [0, innerR-cableW*.2], [-cableW/2,innerR],
                [-pulleyW/2,pulleyR],
                [-pulleyW/2,axleGap]
                ]); 

        // Press-fit cut-out for pullet
        translate([0,0,.01+(pulleyW - bearingW) /2])
            scallop_cylinder(
                h = (bearingW + pulleyW) /2,
                r1 = bearingR + .6, r2 = bearingR + 1, sR = 6, sN = 8
            );
    }
         
 }
 
module undercarriage(test = 0) {
    mountAngs = [-30,0,30,60,90,120,150,180,210];

    translate([-outerFanR,0,0]) {

        difference() {
            // Add region for nut traps()
            union() {
                // Main body subtraction
                difference() {
                    union() {
                        undercarriage_cylinder();

                        // Spline for light mounts
                        path = [
                            [2.5,fanR+thickness],
                            [length * 0.2, fanR*1.5],
                            //[length * 0.5, fanR*1.45],
                            [length-2.5 , fanR+thickness]
                        ];
                        
                        // Array of light mounts
                        for(ang = mountAngs)
                            rotate([ang,-90,0])
                                translate([0,0,-6])
                                    linear_extrude(height=12)
                                        spline_ribbon(path, width=5, loop=false, $fn=16);
                    }
                    // Hollow
                    translate([0,0,length-5+.01]) cylinder(h=5, r1 = fanR, r2 = fanEndR);
                    translate([0,0,-1 -5]) cylinder(h=length + 2, r = fanR);

                    // Holes for light wiring
                    for(ang = mountAngs) {
                        translate([0,0,25])
                            rotate([90+ang,90,0]) rotate([0,30,0])
                                translate([-1.5,-3,0]) cube([3,6,50]);

                        translate([0,0,length-80])
                            rotate([90+ang,90,0])
                                translate([0,0,outerFanR-5])
                                rotate([0,-83,0])
                                    cylinder(r=1.25, h=100);
                    }
                }

                for(z = [length/2-bottomBoltShift,length/2+bottomBoltShift])
                    intersection() {
                        translate([fanR,0,z]) cube([5,25,20], center=true);
                        translate([fanR,0,z]) rotate([0,45,0]) cube([50,50,9], center=true);                    translate([fanR,0,z]) rotate([0,-45,0]) cube([50,50,9], center=true);                    undercarriage_cylinder();        
                    }
            }

            // Bolt hole to fix fan
            translate([0,0,length-18]) rotate([90,0,0]) cylinder(d=4, h=50);
            translate([0,-outerFanR,length-18]) rotate([90,0,0]) cylinder(d=8, h=60);
            
            // Nut traps for mounting to box
            for(z = [length/2-bottomBoltShift,length/2+bottomBoltShift]) 
                translate([fanR,0,z]) rotate([0,-90,0])
                    m3_nut_trap(up=3,down=20);

            // Conduit hole
            translate([fanR,0,length/2+bottomConduitShift])
                rotate([0,90,0]) cylinder(h=30, r=bottomConduitR, center=true);
        }
    }
}

module undercarriage_cylinder() {
    cylinder(h=length, r = outerFanR);
    translate([0,0,length-10]) cylinder(h=10, r1 = outerFanR, r2 = fanEndR + 1);
}

// Test print of the undercarriage for fitting the fan
module test_undercarriage() {
    cutSize = 15;
    
    intersection() {
        translate([ 0,0,-length+cutSize]) undercarriage();
        translate([-500,-500,0]) cube(size=[1000,1000,cutSize]);
    }
    /*
    translate([-outerFanR,0,0]) difference() {
        cylinder(r = outerFanR + 8, h =1 );
        cylinder(r = fanR, h =1.01 );
    }
    */
}
 
module y_axis_rounded_cube(l, size) {
    shift = size.x/2-r;
    if(shift < 0) echo("ERROR!");

    intersection() {
        union() {
            translate([shift,0,0])
                rotate([90,0,0])
                    cylinder(r=r,h=size.y,center=true);
            translate([-shift,0,0])
                rotate([90,0,0])
                    cylinder(r=r,h=size.y,center=true);
            cube(size=[size.x-r*2,size.y,size.z], center=true);
        }
        cube(size=[1000,1000,size.z], center=true);
    }
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


/**
 * Side-loaded on the x-axis with a bolt hole
 */
module m3_side_nut_trap(up, down, side) {
    shaftR = 1.75;
    trapH = 3.5;
    trapR = 3.5;
    
    sizeW = sqrt(3)*trapR;
    
    union() {
        translate([0,0,-trapH/2]) {
            cylinder(h=trapH, r=trapR, $fn=6);
            translate([0,-sizeW/2,0]) cube(size=[side, sizeW, trapH]);
        }
        translate([0,0,-down]) cylinder(r=shaftR, h=up+down, $fs=.5);
    }
}

/**
 * End-loaded on the +ve z-axis with a bolt hole on -ve z
 */
module m3_nut_trap(up, down) {
    shaftR = 1.75;
    trapH = up;
    trapR = 3.25;
    
    sizeW = sqrt(3)*trapR;
    
    union() {
        cylinder(h=trapH, r=trapR, $fn=6);
        translate([0,0,-down]) cylinder(r=shaftR, h=up+down, $fs=.5);
    }
}


/**
 * Scallop-cut cylinder, for press-fit openings
 * r1, r2, h - the same as a cylinder
 * sN - the number of scallops
 * sD - the distance to cut inward with the scallops, at r1
 */
module scallop_cylinder(r1, r2, h, sR, sN) {

    // Switch so that r1 or r2 could be higher
    tX = (r1 > r2) ? r1-sR : r2-sR;
    sR1 = (r1 > r2) ? sR : (r1-r2)+sR;
    sR2 = (r1 > r2) ? (r1-r2)+sR : sR;
    echo(sR1, sR2);
    // MATHS
    x = tX * sin(180/sN);
    y = tX * cos(180/sN);
    d1 = sqrt(sR1*sR1 - x*x);
    sD1 = (tX + sR1) - d1 - y;
    echo("sD1",sD1,"Inner R1",y + d1);

    d2 = sqrt(sR2*sR2 - x*x);
    sD2 = (tX + sR2) - d2 - y;
    echo("sD2",sD2,"Inner R2",y + d2);
    
    union() {
        cylinder(r1 = r1-sR1, r2 = r2-sR2, h = h);
        for(ang = [0: (360/sN): 359.99]) {
            rotate([0,0,ang]) {
                translate([0,tX,0]) cylinder(r1 = sR1, r2 = sR2, h=h);
            }
        }
    }
}  