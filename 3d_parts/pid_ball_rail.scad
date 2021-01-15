$fa = 1;
$fs = 0.4;

surface_t = 5; //thickness
half_surface_t = surface_t/2;
fudge1 = 0.01;
fudge2 = fudge1 * 2;

rail_h = 30;
rail_l = 270;

flange_w = 15;

sensor_pad_w = 40;
sensor_pad_h = rail_h + (cos(45) * flange_w);

axle_hole_margin = 10;
axle_hole_count = 26;
axle_hole_lift = 10;
axle_hole_radius = 1.52;

module rail() {
    color("blue")
    translate([0, -half_surface_t, rail_h - surface_t])
      rotate([45, 0, 0])
        cube([rail_l, surface_t, flange_w], center=false);
    
}

module rails() {
    rail();        
    mirror([0, 1, 0]) rail();
}

module hole_puncher(
        hole_r = 1, 
        hole_depth = 10, 
        hole_gap = 10, 
        hole_count = 10) {

    for (iHole=[0:(hole_count - 1)])
        translate([iHole * hole_gap, 0, 0]) 
        cylinder(r=hole_r,h=hole_depth, center=true);
}

module beam(high_detail = true) {
    difference() {
        
        union() {    
            //main beam
            translate([0, -half_surface_t, 0])
            cube([rail_l, surface_t, rail_h],center=false);

            //rail flanges
            rails();

            //end of the rail (distance sensor mount)    
            color("green")
            translate([-half_surface_t + fudge1, -(sensor_pad_w/2), 0])
            cube([half_surface_t, sensor_pad_w, sensor_pad_h], center=false);

        };
        
        union() {
            //print platform flattener
            roof_w = sensor_pad_w + fudge1;
            roof_l = rail_l + surface_t + fudge2;
            translate([-(surface_t + fudge1), -roof_w/2, sensor_pad_h - surface_t])
                cube([roof_l, roof_w, 10]);

            if (high_detail) {

                //axle holes
                translate([axle_hole_margin, 0  , axle_hole_lift])
                rotate([90, 0, 0])
                        hole_puncher(
                            hole_r = axle_hole_radius, 
                            hole_depth = 10,
                            hole_gap = 10,
                            hole_count = axle_hole_count
                        );

                //servo rod holes
                translate([15, 0  , axle_hole_lift - 3])
                rotate([90, 0, 0])
                        hole_puncher(
                            hole_r = 0.6, 
                            hole_depth = 10,
                            hole_gap = 10,
                            hole_count = axle_hole_count - 1
                        );

                // //amazing architectural balls
                // for (iSphere = [0:12]) {
                //     translate([15 + (iSphere * 20) , 0, 25]) 
                //         sphere(8);
                // }
                
                //sensor pad holes 1
                translate([-surface_t, -15, 5])
                rotate([0, -90, 0])
                        hole_puncher(
                            hole_r = 1, 
                            hole_depth = 10,
                            hole_gap = 8,
                            hole_count = 4
                        );

                //sensor pad holes 2
                translate([-surface_t, 15, 5])
                rotate([0, -90, 0])
                        hole_puncher(
                            hole_r = 1, 
                            hole_depth = 10,
                            hole_gap = 8,
                            hole_count = 4
                        );

            }            
        };
        
    }

}

beam();