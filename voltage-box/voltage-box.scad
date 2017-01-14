$fn = 40;
INNER_W=100;
INNER_D=100;
INNER_H=30;
THICK=2;
SCREW_R=2.5/2;

//box_top();
box_bottom();
//exploded_view();

module exploded_view() {
    translate([0,0,20]) scale([1,1,-1]) box_top();
    box_bottom();
}

module box_top() {
    scale([1,1,-1]) intersection() {
        translate([0,0,500]) cube([1000,1000,1000], center=true);
        box(screw_r = 1.5);
    }
}

module box_bottom() {
   intersection() {
        translate([0,0,-500]) cube([1000,1000,1000], center=true); 
        box(screw_r = 1.25);
    }
}
    
module box() {
    difference() {
        union() {
            difference() {
                rounded_cube(
                    size=[INNER_W+THICK*2, INNER_D+THICK*2, INNER_H+THICK*2],
                    r=5,
                    inset=[5,0,0]
                );
                cube(size=[INNER_W - 2.5, INNER_D, INNER_H], center=true);
            }
            for(x=[-1,1]) for(y=[-1,1])
                translate([x*(INNER_W/2-5+THICK),y*(INNER_D/2-5+THICK),0])
                    cylinder(r=5,h=INNER_H+THICK,center=true);
        }

        // Screw holes
        for(x=[-1,1]) for(y=[-1,1])
            translate([x*(INNER_W/2-5+THICK),y*(INNER_D/2-5+THICK),THICK]) {
                // Countersink
                translate([0,0, INNER_H/2]) cylinder(r=3.5,h=6,center=true);
                // Screw
                cylinder(r=screw_r,h=INNER_H+THICK+0.01,center=true);
            }
            
        // Cut holes for components
        translate([INNER_W/2,-30,0])
            cube(size=[10,12,19], center=true);

        translate([INNER_W/2,-10,0])
            cube(size=[10,5,3], center=true);

        translate([INNER_W/2,30,0])
            cube(size=[10,5,2], center=true);

            
        for(y=[10,30])
            translate([-INNER_W/2,y,-7.5])
                rotate([0,90,0])
                    cylinder(r=2.5, h=10, center=true);

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