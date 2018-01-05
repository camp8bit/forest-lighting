bend = 50.4;
r = 37/2;
d = 75;

l = 40;
w = 13.6;
h = 7.2;
thick = 3;

oL = l+2*thick;
oW = w+2*thick;
oH = h+2*thick;

rotate([180,0,0]) vertex_d3();
//vertex_f2u1();
//rotate([bend+180,0,0]) vertex_f2u1d3();
//vertex_f4u2();

//single();
//test();

module test() {
    truncate_z(from=20, to=40, floor = true)
        difference() {
            connect();
            connect_hole();
        }
            
        
    translate([-30,0,0]) truncate_z(from=-10, to=0, floor = true) rotate([0,90,0]) difference() {
        wiring_panel();
        wiring_panel_hole();
    }
}

module single() {
    difference() {
        //connect();
        connect_hole(wireAng = 30, wireR = 60);
    }
}

module vertex_d3() {
    difference() {
        union() {
            intersection() {
                // Connectors with overhang to feed to the shear
                union() {
                    angs_d3() connect(overhang=18);
                }

                // Shearing planes to tidy things up
                intersection_for(ang=[0,120,240]) rotate([90+bend,0,ang]) connect_plane_top();

                translate([0,0,-200 + 3.39]) cube(400, center=true);
            }


//            translate([0,0,1.59]) scale([1,1,-1])
//                linear_extrude(height=14, scale=.3)
//                    polygon([[0,14], [-12.4,-7.2], [12.4,-7.2]]);

            translate([0,8,-18.5]) rotate([90,-90,0])
                wiring_panel(size="small", shape="circle");
        }
        angs_d3() connect_hole(wireAng = 90, wireR = 10);
        translate([0,8,-18.5]) rotate([90,-90,0])
            wiring_panel_hole(size="small");
        
//        translate([0,0,2]) scale([1,1,-1]) cylinder(r=7.5, h=15);
   
 }
}

module vertex_f2u1() {
    difference() {
        union() {
            
            intersection() {
                // Connectors with overhang to feed to the shear
                union() {
                    angs_f2u1() connect(overhang=18);
                }

                // Shearing planes to tidy things up
                rotate([90,0,30]) connect_plane_right();
                scale([-1,1,1]) rotate([90,0,30]) connect_plane_right();
                for(ang=[0]) rotate([90-bend,0,ang])
                    connect_plane_top();
                rotate([90,0,30]) connect_plane_bottom();
            }
            
            rotate([0,90-bend,270]) translate([-8.5,0,8])
                wiring_panel(size="small");
        }

        // Break out connect holes so that cable management can be customised
        for(ang=[30], mirror=[1,-1]) scale([mirror,1,1]) rotate([90,0,ang])
            connect_hole(wireAng =40);
        for(ang=[0]) rotate([90-bend,0,ang]) connect_hole();

        rotate([0,90-bend,270]) translate([-8.5,0,8])
                wiring_panel_hole(size="small");
        translate([0,6,0]) cube([9,20,4], center=true);
    }
}


module vertex_f4u2() {
    difference() {
        union() {
            angs_f4u2() connect();
            rotate([90,0,0]) connect_join();
            translate([-oW/2-.5,0,oH/2]) rotate([0,20,0]) wiring_panel(height=17);
        }

        // Break out connect holes so that cable management can be customised
        union() {
            for(mirror=[1,-1], ang=[120,180])
                scale([1,mirror,1]) rotate([90,0,ang])
                    connect_hole(
                        wireAng = ang-90,
                        wireR = (ang==120) ? 60 : 10
                  );
            for(mirror=[1,-1], ang=[150])
                scale([1,mirror,1]) rotate([90-bend,0,ang])
                    connect_hole(wireAng = 77, wireR = 10);
        }
        
        //translate([0,0,0]) cube([20,15,10], center=true);

        translate([-oW/2-.5,0,oH/2+2]) rotate([0,20,0]) wiring_panel_hole(height=17);
    }
}

module vertex_f2u1d3() {
    difference() {
        union() {
            angs_f2u1d3() connect(nudge = 2.2);
             rotate([90,bend,90]) connect_join();
             rotate([-bend,0,0]) rotate([0,90,0]) translate([-6.6,0,-8])
                wiring_panel();
        }

        rotate([-bend,0,0]) rotate([0,90,0]) translate([-6.6,0,-8])
            wiring_panel_hole();

        // Break out connect holes so that cable management can be customised
        union() {
            for(mirror=[1,-1])
                scale([mirror,1,1]) rotate([90,0,30]) connect_hole(wireAng=60);
            for(ang=[0]) rotate([90-bend,0,ang]) connect_hole(wireAng = 90, wireR = 10);
            for(ang=[60,180,300]) rotate([90+bend,0,ang]) connect_hole(wireAng = 90, wireR = 12);
        }

        //rotate([0,bend,90]) translate([0,0,3])  cube([20,15,16], center=true);
   }
}

module angs_d3() {
    for(ang=[0,120,240]) rotate([90+bend,0,ang]) children();
}

module angs_f2u1() {
    for(ang=[-30,30]) rotate([90,0,ang]) children();
    for(ang=[0]) rotate([90-bend,0,ang]) children();
}

module angs_f4u2() {
    for(mirror=[1,-1], ang=[120,180])
        scale([1,mirror,1]) rotate([90,0,ang])
            children();
    for(mirror=[1,-1], ang=[150])
        scale([1,mirror,1]) rotate([90-bend,0,ang])
            children();
}

module angs_f2u1d3() {
    union() {
        for(mirror=[1,-1])
            scale([mirror,1,1]) rotate([90,0,30]) children();
        for(ang=[0]) rotate([90-bend,0,ang]) children();
        for(ang=[60,180,300]) rotate([90+bend,0,ang]) children();
    }
}

module connect_down() {
    rotate([90+bend,0,0]) connect();
}

module connect_flat() {
    rotate([90,0,0]) connect();
}

module connect_up() {
    rotate([90-bend,0,0]) connect();
}

// Outer shape for each bar of the connector
module connect(nudge = 0, overhang = 0) {
    translate([0,0,l/2+h-nudge -overhang/2 ]) cube([w+2*thick,h+2*thick,l+2*thick+overhang], center=true);
}

// Shearing plane to be used with intersection
module connect_plane_right() {
    translate([-150 + oW/2,0,0]) cube(300,center=true);
}

module connect_plane_top() {
    translate([0,-150 + oH/2,0]) cube(300,center=true);
}
module connect_plane_bottom() {
    translate([0,150 - oH/2,0]) cube(300,center=true);
}

// Cutaway shape for each bar of the connector
module connect_hole(wireAng = 0, wireR = 16) {
    wireW = 9;
    wireH = 4;
    
    holeHeight = 20;
    
    union() {
        difference() {
            translate([0,0, l/2+h + holeHeight]) cube([w,h,l], center=true);
            for(mirrorX = [1,-1])
                translate([mirrorX*w/2,-h/2+2,0])
                    cylinder(d=0.75,h=l*2,$fs=.2);
        }
        
        // Screw hole
        translate([0,0,l+h-5]) rotate([90,0,90]) cylinder(h=20,r=1,center=true);
        
        // Bent cable channel
        if(wireAng) {
            intersection() {
                translate([wireR,0,h + holeHeight]) rotate([90,0,0])
                    square_ring(r= wireR, h = wireH, w = wireW);
                 translate([wireR,0,h + holeHeight])
                        rotate([0,-wireAng,0])
                            translate([0,0,100])
                                cube([300,h,200], center=true);
                translate([0,0,(h+holeHeight)/2 - 100])
                    cube([300,h,h + holeHeight + 200], center=true);
            }
        }
        
        rotate_about(center=[wireR,0,h + holeHeight],ang=[0,-wireAng,0])
            translate([0,0,(h+holeHeight) /2])
                cube([wireW,wireH,h + holeHeight], center=true);
    }
}
// Join piece to use at the vetext
module connect_join() {
    cube([w+2*thick,h+2*thick,w+2*thick], center=true);
}

module square_ring(r, w, h) {
    rotate_extrude()
        translate([r,0])
            square([w,h], center=true);
}


module wiring_panel(length=15, height=15, size="large") {
    if(size == "large") {
        translate([0,-12,0]) cube([length,24,height]);
    } else {
        if(shape == "circle") {
            translate([0,0,height/2]) rotate([0,90,0]) cylinder(d=height, h=length);
        } else {
            translate([0,-8,0]) cube([length,16,height]);
        }
    }
}

module wiring_panel_hole(size="large") {
    union() {
        if(size == "large") {
            translate([-1,-8,2]) cube([6.5,15.5,2.75]);
            translate([-1,-8,10]) cube([6.5,15.5,2.75]);
            translate([-1,-10,6]) cube([6.5,5.5,2.75]);
            translate([-1,-3,6]) cube([6.5,5.5,2.75]);
            translate([-1,4,6]) cube([6.5,5.5,2.75]);
        } else {
            translate([-1,-4,2]) cube([6.5,8,2.75]);
            translate([-1,-4,10]) cube([6.5,8,2.75]);
            translate([-1,-6,6]) cube([6.5,5.5,2.75]);
            translate([-1,.5,6]) cube([6.5,5.5,2.75]);
        }
    }
}
// Rotate about a given center point
module rotate_about(center, ang) {
     translate(center) rotate(ang) translate(-center)
        children();
}

// limit shape to a given z range
module truncate_z(from, to, floor = false) {
    translate([0,0, floor ? -from : 0]) 
        intersection() {
            translate([-1000,-1000,from]) cube([2000,2000,to-from]);
            union() { children(); }
        }
}