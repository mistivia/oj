proc gcd {a b} {
    if {$b > $a} {
        set t $b
        set b $a
        set a $t
    }
    while {$b != 0} {
        set t $b
        set b [expr {$a % $b}]
        set a $t
    }
    return $a
}

proc lcm {a b} {
    set g [gcd $a $b]
    return [expr {$g * ($a / $g) * ($b / $g)}]
}

set result 1
for {set i 1} {$i <= 20} {incr i} {
    set result [lcm $result $i]
}

puts $result
