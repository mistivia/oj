proc square {x} {
    return [expr {$x * $x}]
}

proc sum_of {x} {
    set result 0
    for {set i 1} {$i <= $x} {incr i} {
        set result [expr {$result + $i}]
    }
    return $result
}

proc sum_of_square {x} {
    set result 0
    for {set i 1} {$i <= $x} {incr i} {
        set result [expr {$result + [square $i]}]
    }
    return $result
}

puts [expr {[square [sum_of 100]] - [sum_of_square 100]}]
