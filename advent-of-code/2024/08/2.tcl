set fp [open "input"]
set lines {}
while {[gets $fp line] >= 0} {
    lappend lines $line
}

close $fp

set height [llength $lines]
set width [string length [lindex $lines 0]]

proc char_at {x y} {
    global lines
    return [string index [lindex $lines $y] $x]
}

proc in_range {x y} {
    global height width
    if {$x < 0 || $x >= $width} {
        return 0
    }
    if {$y < 0 || $y >= $height} {
        return 0
    }
    return 1
}

proc build_antenna_map {} {
    global height width
    set antenna_map {}
    for {set x 0} {$x < $width} {incr x} {
        for {set y 0} {$y < $height} {incr y} {
            set c [char_at $x $y]
            if {$c == "."} {
                continue
            }
            dict update antenna_map $c site_list {
                lappend site_list [list $x $y]
            }
        }
    }
    return $antenna_map
}

set antenna_map [build_antenna_map]
set antinodes {}

foreach k [dict keys $antenna_map] {
    set site_list [dict get $antenna_map $k]
    set len [llength $site_list]
    for {set i 0} {$i < $len} {incr i} {
        for {set j [expr {$i + 1}]} {$j < $len} {incr j} {
            set x1 [lindex [lindex $site_list $i] 0]
            set y1 [lindex [lindex $site_list $i] 1]
            set x2 [lindex [lindex $site_list $j] 0]
            set y2 [lindex [lindex $site_list $j] 1]
            set delta_x [expr {$x2 - $x1}]
            set delta_y [expr {$y2 - $y1}]
            while {[in_range $x2 $y2]} {
                dict set antinodes [list $x2 $y2] {}
                set x2 [expr {$x2 + $delta_x}]
                set y2 [expr {$y2 + $delta_y}]
            }
            while {[in_range $x1 $y1]} {
                dict set antinodes [list $x1 $y1] {}
                set x1 [expr {$x1 - $delta_x}]
                set y1 [expr {$y1 - $delta_y}]
            }            
        }
    }
}

puts [dict size $antinodes]
