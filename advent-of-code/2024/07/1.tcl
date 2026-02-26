set fp [open "input"]
set equations {}
while {[gets $fp line] >= 0} {
    set s1 [split $line ":"]
    lappend equations $s1
}

proc can_match_impl {target numbers current} {
    if {0 == [llength $numbers]} {
        return [expr {$target == $current}]
    }
    set rest [lrange $numbers 1 end]
    set first [lindex $numbers 0]
    return [expr {[can_match_impl $target $rest [expr {$current * $first}]]
                    || [can_match_impl $target $rest [expr {$current + $first}]]}]
}

proc can_match {target numbers} {
    return [can_match_impl $target [lrange $numbers 1 end] [lindex $numbers 0]]
}

set sum 0
foreach elem $equations {
    set test_value [lindex $elem 0]
    set numbers [lindex $elem 1]
    if {[can_match $test_value $numbers]} {
        set sum [expr {$sum + $test_value}]
    }
}
puts $sum
