//include <pid_ball_rail.scad>;
//use <pid_ball_rail.scad>;
$fa = 1;
$fs = 0.4;

fudge1 = 0.01;
wall_thickness = 2;
base_thickness = 2.5;

module base() {
    //cube([100, 100, base_thickness], center = true);

    difference() {
        union() {
            scale([2.0,1.0,1.0])
                cylinder(r=50, h=base_thickness, center = true);
        }
        
        union() {
            translate([50, 5, 0])
            cylinder(r=22, h=base_thickness + (2 * fudge1) + 10,center = true);
            
            translate([-50, -5, 0])
            cylinder(r=22, h=base_thickness + (2 * fudge1) + 10,center = true);

            translate([-18, 28, 0])
            cylinder(r=12, h=base_thickness + (2 * fudge1) + 10,center = true);

            translate([18, -28, 0])
            cylinder(r=12, h=base_thickness + (2 * fudge1) + 10,center = true);

        }
    }
}

module flange(height = 100, base_length = 20, top_length = 10) {
    echo("flange thickness:");
    echo(wall_thickness);
    
    CubePoints = [
      [  0,  0,  0 ],  //0 NW
      [ base_length,  0,  0 ],  //1 SW
      [ base_length,  wall_thickness,  0 ],  //2 SE
      [  0,  wall_thickness,  0 ],  //3 NE
      [  0,  0,  height ],  //4 NW top
      [ top_length,  0,  height ],  //5 SW
      [ top_length,  wall_thickness,  height ],  //6 SE
      [  0,  wall_thickness,  height ]]; //7 NE
      
    CubeFaces = [
      [0,1,2,3],  // bottom
      [4,5,1,0],  // front
      [7,6,5,4],  // top
      [5,6,2,1],  // right
      [6,7,3,2],  // back
      [7,4,0,3]]; // left
      
    polyhedron( CubePoints, CubeFaces );
    
    translate([base_length, wall_thickness/2, 2])
        cylinder(r=(wall_thickness * 1.7), h=(base_thickness * 1), center=true);
}

module tower(
        axle_support_height = 100,
        support_height = 75,
        rail_width = 5.3,
        support_width = 10
    ) {

    rail_width_half = rail_width/2;
    support_width_half = support_width / 2;
    
    //N (origin faces North)
    color("blue")
    translate([-support_width_half, rail_width_half, 0])
    rotate([0, 0, 0])
        flange(height = axle_support_height, top_length = support_width);

    //W
    color("red")
    translate([rail_width, -support_width_half, 0])
    rotate([0, 0, 90])
        flange(height = support_height);

    //S
    color("white")
    translate([support_width_half, -rail_width_half, 0])
        rotate([0, 0, 180])
            flange(height = axle_support_height, top_length = support_width);
    
    //E
    color("green")
    translate([-rail_width, support_width_half, 0])
        rotate([0, 0, 270])
            flange(height = support_height);       
}

module tower_with_holes(
        axle_support_height = 100,
        support_height = 75,
        rail_width = 5.3,
        support_width = 10
    ) {
    tower_width = (rail_width + (2*wall_thickness) + fudge1);
    tower_axle_height = axle_support_height - 5;

    difference() {
        union() {   
            tower(
                axle_support_height = axle_support_height,
                support_height = support_height,
                rail_width = rail_width,
                support_width = support_width
            );
        }
        union() {
            translate([0, 0, tower_axle_height])
            rotate([90, 0, 0])
            cylinder(d=3.2, h=tower_width, center = true);
        }
    }
}

module measurement_references() {
    color("lime")
    translate([0, 0, 100])
    rotate([90, 0, 0])
        cylinder(d=1, h=5, center = true);
    
}

module stand() {

    color("red")
        base(); 
    
    tower_with_holes();
    
    //measurement_references();
}

    //translate([100, 0, 0])
    stand();

