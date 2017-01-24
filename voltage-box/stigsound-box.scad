$fn = 40;
INNER_W=75;
INNER_D=55;
INNER_H=20;
THICK=1;
SCREW_R=2.5/2;

//box_top();
//box_bottom();
//exploded_view();
plated_view();

module plated_view() {
    translate([0,INNER_D+10,0]) box_top();
    box_bottom();
}

module exploded_view() {
    translate([0,0,20]) rotate([180,0,0]) box_top();
    box_bottom();
}

module box_top() {
    rotate([180,0,0]) union() {
        intersection() {
            translate([0,0,500]) cube([1000,1000,1000], center=true);
            box(screw_r = 2.25);
        }
        // Tabs to hold top to bottom
        for(a=[0,180])
            rotate([0,0,a])
                translate([0,INNER_D/2-0.5,5])
                    cube([INNER_W-16,1,INNER_H/2+1.5], center=true);
    }
}

module box_bottom() {
   intersection() {
        translate([0,0,-500]) cube([1000,1000,1000], center=true); 
        box(screw_r =2.25);
    }
}
    
module box() {
    difference() {
        union() {
            difference() {
                rounded_cube(
                    size=[INNER_W+THICK*2, INNER_D+THICK*2, INNER_H+THICK*2],
                    r=4
                );
                cube(size=[INNER_W, INNER_D, INNER_H], center=true);
            }
            for(x=[-1,1]) for(y=[-1,1])
                translate([x*(INNER_W/2-4+THICK),y*(INNER_D/2-4+THICK),0])
                    cylinder(r=4,h=INNER_H+THICK,center=true);
        }

        // Screw holes
        for(x=[-1,1]) for(y=[-1,1])
            translate([x*(INNER_W/2-4+THICK),y*(INNER_D/2-4+THICK),0]) {
                // Screw
                cylinder(r=screw_r,h=INNER_H+THICK*2+0.01,center=true);
            }
            
        // Cut holes for components
        translate([INNER_W/2,0,0])
            cube(size=[10,5,3], center=true);

        translate([-INNER_W/2,0,-5.5])
            rotate([0,90,0])
                cylinder(r=3.25, h=10, center=true);


/*(
        translate([INNER_W/2,30,0])
            cube(size=[10,5,2], center=true);
            
        for(y=[10,30])
            translate([-INNER_W/2,y,-7.5])

*/
    }
    
    
}

module lid() {
    union() {
        rounded_cube(size=[INNER_W+THICK*2, INNER_D+THICK*2, THICK], r=5);
        translate([0,0,THICK])
        rounded_cube(size=[INNER_W - 0.5, INNER_D - 0.5, THICK], r=5);
    }
}

module rounded_cube(size, r, inset = [0,0,0]) {
    union() {
        cube(size=[size[0]-r*2, size[1]-inset[1], size[2]-inset[2]], center=true);
        cube(size=[size[0]-inset[0], size[1]-r*2, size[2]-inset[2]], center=true);
        for(x=[-1,1]) for(y=[-1,1])
            translate([x*(size[0]/2-r),y*(size[1]/2-r),0])
                cylinder(r=r,h=size[2],center=true);
    }
}