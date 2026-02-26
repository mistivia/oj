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
    dict update ctx sum sum {
        set sum [expr {$sum + [is_outside $c [expr {$x - 1}] $y] \
                            + [is_outside $c [expr {$x + 1}] $y] \
                            + [is_outside $c $x [expr {$y + 1}]] \
                            + [is_outside $c $x [expr {$y - 1}]]}]
    }
    dict update ctx cache cache {
        dict set cache "$x,$y" {}
    }
    get_peri_impl ctx $c [expr {$x + 1}] $y
    get_peri_impl ctx $c [expr {$x - 1}] $y
    get_peri_impl ctx $c $x [expr {$y + 1}]
    get_peri_impl ctx $c $x [expr {$y - 1}]
}

proc get_peri {x y} {
    set ctx {sum 0 cache {}}
    get_peri_impl ctx [char_at $x $y] $x $y
    return [dict get $ctx sum]
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
        set peri [get_peri $x $y]
        set area [get_area $x $y]
        mark_visited [char_at $x $y] $x $y
        incr sum [expr {$peri * $area}]
    }
}

puts $sum
