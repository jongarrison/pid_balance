//include <pid_ball_rail.scad>;
use <pid_ball_rail.scad>;

$fn = 400;

roller_w = 25;
roller_h = 15;
roller_offset = 40;
groove_w = 10.5; //measured 11.5
groove_d = 5.5; //measured 5.2
hole_r = 10;
groove_tip_remove_h = 2;
groove_h = roller_h + groove_d - groove_tip_remove_h;
shoulder_width = (roller_w - groove_w)/2;

tip_mask_h = 10;
tip_mask_w = 20;

mask_z = -28;
mask_x = -10;


module simple_roller_crossection() {
    //starting bottom left of shape and going counterclockwise
   
    difference() {
        polygon(
            [
            [hole_r, -roller_w/2], [hole_r, roller_w/2], //bottom line on x axis
            [roller_h, roller_w/2], [roller_h, groove_w/2], //right shoulder to begining of the groove
            [roller_h + groove_d, 0], //groove point
            [roller_h, -groove_w/2], [roller_h, -roller_w/2] //left of groove to left shoulder
            ]
         );

        translate([groove_h, -tip_mask_w/2, 0]) 
        square([tip_mask_h, tip_mask_w]);
    }

}

rotate_extrude()
simple_roller_crossection();
//roller_cross_section();

module complex_roller() {
    translate([20, 0, 0])
    union() {
        //circle(10);
        roller_cross_section();    
    }
}

module roller_cross_section() {
    rotate([0, 90, 0])
    difference() {
        union() {
            color("pink")
            rotate([0, -90, 0])
            translate([0, -roller_w/2, 0])
            square([roller_h, roller_w]);
            
        }

        translate([mask_x, 0, mask_z])
        beam(false);

        translate([0, -10, 0])
        rotate([0, -90, 0])
        square([10, 10], center=true);

        translate([mask_x, -20, mask_z])
        beam(false);

        translate([0, 10, 0])
        rotate([0, -90, 0])
        square([10, 10], center=true);

        translate([mask_x, 20, mask_z])
        beam(false);

    }
}
