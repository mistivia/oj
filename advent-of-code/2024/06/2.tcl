#!/usr/bin/env tclsh

set fp [open input r]
set map ""
while {1} {
    set ret [gets $fp line]
    if {$ret == -1} {
        break
    }
    if {[string length $line] == 0} {
        break
    }
    lappend map $line
}
close $fp

proc char_at {x y} {
    set line [lindex $::map $y]
    return [string index $line $x]
}

set height [llength $map]
set width [string length [lindex $map 0]]

proc find_start {} {
    for {set i 0} {$i < $::width} {incr i} {
        for {set j 0} {$j < $::height} {incr j} {
            if {[char_at $i $j] == "^"} {
                return "$i $j"
            }
        }
    }
}

set start_point [find_start]

proc front_point {x y direction} {
    if {$direction == "up"} {
        return [list $x [expr {$y - 1}]]
    } elseif {$direction == "down"} {
        return [list $x [expr {$y + 1}]]
    } elseif {$direction == "left"} {
        return [list [expr {$x - 1}] $y]
    } elseif {$direction == "right"} {
        return [list [expr {$x + 1}] $y]
    }
}

proc out_of_range {x y} {
    return [expr {
        ($x < 0) || ($x >= $::width) || ($y < 0) || ($y >= $::height)
    }]
}

proc turn_right {direction} {
    if {$direction == {up}} {
        return right
    } elseif {$direction == {down}} {
        return left
    } elseif {$direction == {right}} {
        return down
    } else {
        return up
    }
}


proc next_state {x y direction} {
    set forward [front_point $x $y $direction]
    if {[out_of_range {*}$forward]} {
        return [list {*}$forward $direction]
    }
    while {[char_at {*}$forward] == {#}} {
        set forward [front_point $x $y [turn_right $direction]]
        set direction [turn_right $direction]
    }
    return [list {*}$forward $direction]
}


proc will_loop {} {
    global start_point
    set state [list {*}$start_point up]
    set past_states {}
    while {1} {
        if {[out_of_range {*}[lrange $state 0 1]]} {
            return 0
        }
        if {[dict exists $past_states $state]} {
            return 1
        }
        dict set past_states $state { }
        set state [next_state {*}$state]
    }    
}

proc set_obstruction {x y} {
    global map
    set orig_line [lindex $map $y]
    lset map $y [string replace $orig_line $x $x "#"]
}

proc clear_obstruction {x y} {
    global map
    set orig_line [lindex $map $y]
    lset map $y [string replace $orig_line $x $x "."]
}

set count 0
for {set i 0} {$i < $width} {incr i} {
    for {set j 0} {$j < $height} {incr j} {
        if {[char_at $i $j] == "^"} {
            continue
        }
        if {[char_at $i $j] == "#"} {
            continue
        }
        set_obstruction $i $j
        if {[will_loop]} {
            set count [expr {$count + 1}]
        }
        clear_obstruction $i $j
    }
}
puts $count
