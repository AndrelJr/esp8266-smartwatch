// All units in millimeters
handle_height = 2;
handle_outer_width = 31;
handle_inner_width = 27;
handle_distance = 2;
handle_conn_width = 3;

pcb_width = 35;
pcb_height = 30;
pcb_thickness = 1;

oled_offset_top = 3;
oled_nodisp_height = 4;
oled_disp_height = 15;
oled_width = 26.5;
oled_thickness = 2;

battery_width = 31.5;
battery_height = 20;
battery_thickness = 6;
battery_offset_left = 0;
battery_offset_bottom = 8;

button_height = 5;
button_width = 3.5;
button_thickness = 2;

button_knob_thickness = 1.4;
button_knob_height = 2.6;
button_knob_width = 1;

button1_offset_top =  2.5;
button2_offset_top = 12.5;
button3_offset_top = 22.5;

usbconn_width = 6.5;
usbconn_height = 8;
usbconn_thickness = 2.5;
usbconn_width_pcb = 4.5;
usbconn_offset_bottom = 1;

pcb_drillhole_offset_bottom = 3.5;
pcb_drillhole_offset_right = 14;
pcb_drillhole_diameter = 2;

module pcb_handle() {
	linear_extrude(height = pcb_thickness)  polygon(points=[
		[-handle_inner_width / 2, pcb_height / 2 + handle_distance],
		[handle_inner_width / 2, pcb_height / 2 + handle_distance],
		[handle_outer_width / 2, pcb_height / 2 + handle_distance + handle_height],
		[-handle_outer_width / 2, pcb_height / 2 + handle_distance + handle_height]
	]);

	linear_extrude(height = pcb_thickness)  polygon(points=[
		[-pcb_width / 2, pcb_height / 2],
		[-pcb_width / 2 + handle_conn_width, pcb_height / 2],
		[-handle_inner_width / 2, pcb_height / 2 + handle_distance],
		[-handle_outer_width / 2, pcb_height / 2 + handle_distance + handle_height]
	]);

	linear_extrude(height = pcb_thickness)  polygon(points=[
		[pcb_width / 2, pcb_height / 2],
		[pcb_width / 2 - handle_conn_width, pcb_height / 2],
		[handle_inner_width / 2, pcb_height / 2 + handle_distance],
		[handle_outer_width / 2, pcb_height / 2 + handle_distance + handle_height]
	]);
}

module pcb_drillhole() {
	translate([pcb_width / 2 - pcb_drillhole_offset_right,
			-pcb_height / 2 + pcb_drillhole_offset_bottom, 0]) {
		cylinder(h = pcb_thickness * 2, d1 = pcb_drillhole_diameter, $fn = 90, center = true);
	}
}

module pcb() {
	color([0, 0.6, 0, 1]) {
		difference() {
			translate([-pcb_width / 2, -pcb_height / 2, -pcb_thickness / 2]) {
				cube(size = [pcb_width, pcb_height, pcb_thickness]);
			}
			pcb_drillhole();
		}

		translate([0, 0, -pcb_thickness / 2]) {
			// Upper handle
			pcb_handle();

			// Lower handle
			mirror([0, 1, 0]) pcb_handle();
		}
	}
}

module oled() {
	translate([-oled_width / 2, pcb_height / 2 - oled_offset_top, pcb_thickness / 2]) {
		// Controller area
		color([0.5, 0.5, 0.5, 0.5]) {
			translate([0, -oled_nodisp_height, 0])
				cube(size = [oled_width, oled_nodisp_height, oled_thickness]);
		}

		// Display area
		color([0, 0, 0, 0.8]) {
			translate([0, -oled_nodisp_height - oled_disp_height, 0])
				cube(size = [oled_width, oled_disp_height, oled_thickness]);
		}
	}
}

module battery() {
	translate([-pcb_width / 2, -pcb_height / 2, -pcb_thickness / 2]) {
		translate([battery_offset_left, battery_offset_bottom, -battery_thickness]) {
			color([0.5, 0.5, 0.5, 1.0])
			cube(size = [battery_width, battery_height, battery_thickness]);
		}
	}
}

module button(offsetx, offsety) {
	translate([offsetx, offsety, -button_thickness - pcb_thickness / 2]) {
		color([0.7, 0.7, 0.7, 1.0]) {
			cube(size = [button_width, button_height, button_thickness]);
		}
	
		translate([button_width, button_height / 2 - button_knob_height / 2, 0]) {
			color([0.2, 0.2, 0.2, 1.0]) {
				cube(size = [button_knob_width, button_knob_height, button_knob_thickness]);
			}
		}
	}
}

module buttons() {
	button(pcb_width / 2 - button_width, pcb_height / 2 - button_height -  button1_offset_top);
	button(pcb_width / 2 - button_width, pcb_height / 2 - button_height -  button2_offset_top);
	button(pcb_width / 2 - button_width, pcb_height / 2 - button_height -  button3_offset_top);
}

module usbconn(offsetx, offsety) {
	translate([offsetx, offsety, -pcb_thickness / 2 - usbconn_thickness]) {
		color([0.3, 0.3, 0.3, 1.0]) {
			cube([usbconn_width, usbconn_height, usbconn_thickness]);
		}
	}
}

module connectors() {
	usbconn(-pcb_width / 2 - usbconn_width + usbconn_width_pcb, -pcb_height / 2 + usbconn_offset_bottom);
}

module watch(centerx, centery) {
	translate([centerx, centery]) {
		pcb();
		oled();
		battery();
		buttons();
		connectors();
	}
}

watch(0, 0);