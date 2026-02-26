set fp [open "input" r]
set line {}
while {[gets $fp line] >= 0} {
    lappend lines $line
}
close $fp

set height [llength $lines]
set width [string length [lindex $lines 0]]

proc out_of_range {x y} {
    global height width
    if {$x < 0 || $x >= $width} {
        return 1
    }
    if {$y < 0 || $y >= $height} {
        return 1
    }
    return 0
}

proc char_at {x y} {
    global lines
    if {[out_of_range $x $y]} {
        return -1
    }
    return [string index [lindex $lines $y] $x]
}

set path_cache {}

proc find_path {x y expect} {
    global path_cache
    if {[out_of_range $x $y]} {
        return 0
    }
    if {$expect != [char_at $x $y]} {
        return 0
    }
    if {$expect == 9} {
        dict set path_cache "$x,$y" {}
        return 1
    }
    set next [expr {$expect + 1}]
    set ret [expr {
        [find_path [expr {$x + 1}] $y $next]
        + [find_path [expr {$x - 1}] $y $next]
        + [find_path $x [expr {$y + 1}] $next]
        + [find_path $x [expr {$y - 1}] $next]
    }]
}

set sum 0
for {set x 0} {$x < $width} {incr x} {
    for {set y 0} {$y < $height} {incr y} {
        set path_cache {}
        find_path $x $y 0
        incr sum [dict size $path_cache]
    }
}
puts $sum
