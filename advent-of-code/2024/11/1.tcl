set fp [open "input"]
gets $fp line
close $fp

proc is_even {x} {
    return [expr {$x % 2 == 0}]
}

proc zero_trim {x} {
    for {set i 0} {$i < [expr {[string length $x] - 1}]} {incr i} {
        if {[string index $x $i] != 0} {
            break
        }
    }
    return [string range $x $i end]
}

proc blink {x} {
    if {$x == 0} {
        return 1
    }
    if {[is_even [string length $x]]} {
        set len [string length $x]
        set halflen [expr {$len / 2}]
        return [list [zero_trim [string range $x 0 [expr {$halflen - 1}]]] \
                     [zero_trim [string range $x $halflen end]]]
    }
    return [expr {$x * 2024}]
}

set lst $line
for {set i 0} {$i < 25} {incr i} {
    set new_lst {}
    foreach x $lst {
        lappend new_lst {*}[blink $x]
    }
    set lst $new_lst
}

puts [llength $lst]
