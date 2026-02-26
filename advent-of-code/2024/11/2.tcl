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

set blinked_length_cache {}

proc blinked_length {x n} {
    global blinked_length_cache
    if {$n == 0} {
        return 1
    }
    set key "$x,$n"
    if {[dict exists $blinked_length_cache $key]} {
        return [dict get $blinked_length_cache $key]
    }
    if {$x == 0} {
        set result [blinked_length 1 [expr {$n - 1}]]
        dict set blinked_length_cache $key $result
        return $result
    }
    if {[is_even [string length $x]]} {
        set len [string length $x]
        set halflen [expr {$len / 2}]
        set first [zero_trim [string range $x 0 [expr {$halflen - 1}]]]
        set second [zero_trim [string range $x $halflen end]]
        set next_n [expr {$n - 1}]
        set result [expr {[blinked_length $first $next_n] \
                    + [blinked_length $second $next_n]}]
        dict set blinked_length_cache $key $result
        return $result
    }
    set result [blinked_length [expr {$x * 2024}] [expr {$n - 1}]]
    dict set blinked_length_cache $key $result
    return $result
}

set lst $line
set sum 0
foreach x $lst {
    incr sum [blinked_length $x 75]
}
puts $sum
