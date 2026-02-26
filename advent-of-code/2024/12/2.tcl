set fp [open "input"]
set lines {}
set line {}
while {[gets $fp line] >= 0} {
    lappend lines $line
}
close $fp

set height [llength $lines]
set width [string length [lindex $lines 0]]

proc char_at {x y} {
    global lines
    string index [lindex $lines $y] $x
}

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

proc get_area_impl {pctx c x y} {
    upvar $pctx ctx
    if {[out_of_range $x $y]} {
        return
    }
    if {[dict exists [dict get $ctx cache] "$x,$y"]} {
        return
    }
    if {[char_at $x $y] != $c} {
        return
    }
    dict update ctx sum sum {incr sum}
    dict update ctx cache cache {
        dict set cache "$x,$y" {}
    }
    get_area_impl ctx $c [expr {$x + 1}] $y
    get_area_impl ctx $c [expr {$x - 1}] $y
    get_area_impl ctx $c $x [expr {$y + 1}]
    get_area_impl ctx $c $x [expr {$y - 1}]
}

proc get_area {x y} {
    set ctx {sum 0 cache {}}
    get_area_impl ctx [char_at $x $y] $x $y
    return [dict get $ctx sum]
}

proc get_peri_impl {pctx c x y} {
    proc is_outside {c x y} {
        if {[out_of_range $x $y]} {
            return 1
        }
        if {[char_at $x $y] != $c} {
            return 1
        }
        return 0
    }
    upvar $pctx ctx
    if {[out_of_range $x $y]} {
        return
    }
    if {[dict exists [dict get $ctx cache] "$x,$y"]} {
        return
    }
    if {[char_at $x $y] != $c} {
        return
    }
    set c [char_at $x $y]
    dict update ctx sides sides {
        if {[is_outside $c [expr {$x - 1}] $y]} {
            lappend sides [list 0 $x $y]
        }
        if {[is_outside $c [expr {$x + 1}] $y]} {
            lappend sides [list 1 [expr {$x + 1}] $y] 
        }
        if {[is_outside $c $x [expr {$y + 1}]]} {
            lappend sides [list 2 [expr {$y + 1}] $x]
        }
        if {[is_outside $c $x [expr {$y - 1}]]} {
            lappend sides [list 3 $y $x]
        }
    }
    dict update ctx cache cache {
        dict set cache "$x,$y" {}
    }
    get_peri_impl ctx $c [expr {$x + 1}] $y
    get_peri_impl ctx $c [expr {$x - 1}] $y
    get_peri_impl ctx $c $x [expr {$y + 1}]
    get_peri_impl ctx $c $x [expr {$y - 1}]
}

proc side_cmp {a b} {
    set a_dir [lindex $a 0]
    set b_dir [lindex $b 0]
    if {[lindex $a 0] != [lindex $b 0]} {
        return [expr [lindex $a 0] - [lindex $b 0]]
    }
    if {[lindex $a 1] != [lindex $b 1]} {
        return [expr {[lindex $a 1] - [lindex $b 1]}]
    }
    return [expr {[lindex $a 2] - [lindex $b 2]}]
}

proc count_sides {lst} {
    proc is_cont {a b} {
        if {[lindex $a 0] != [lindex $b 0]} {
            return 0
        }
        if {[lindex $a 1] != [lindex $b 1]} {
            return 0
        }
        if {[lindex $b 2] - [lindex $a 2] != 1} {
            return 0
        }
        return 1
    }
    set ret 0
    for {set i 0} {$i < [expr {[llength $lst] - 1}]} {incr i} {
        if {[is_cont [lindex $lst $i] [lindex $lst [expr {$i + 1}]]]} {
            continue
        }
        incr ret
    }
    incr ret
    return $ret
}

proc get_peri {x y} {
    set ctx {sides {} cache {}}
    get_peri_impl ctx [char_at $x $y] $x $y
    return [count_sides [lsort -command side_cmp [dict get $ctx sides]]]
}

set visited {}
proc mark_visited {c x y} {
    global visited
    if {[out_of_range $x $y]} return
    if {[char_at $x $y] != $c} return
    if {[dict exists $visited "$x,$y"]} return
    dict set visited "$x,$y" {}
    incr x
    mark_visited $c $x $y
    incr x -2
    mark_visited $c $x $y
    incr x
    incr y
    mark_visited $c $x $y
    incr y -2
    mark_visited $c $x $y
}

set sum 0
for {set x 0} {$x < $width} {incr x} {
    for {set y 0} {$y < $height} {incr y} {
        if {[dict exists $visited "$x,$y"]} {
            continue
        }
        set area [get_area $x $y]
        set peri [get_peri $x $y]
        mark_visited [char_at $x $y] $x $y
        incr sum [expr {$peri * $area}]
    }
}

puts $sum
