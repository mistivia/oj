set fp [open "input"]
gets $fp line
close $fp

set line_len [string length $line]
set layout {}
set is_file 1
set file_no 0
for {set i 0} {$i < $line_len} {incr i} {
    set num [string index $line $i]
    if {$is_file} {
        for {set j 0} {$j < $num} {incr j} {
            lappend layout $file_no
        }
        incr file_no
    } else {
        for {set j 0} {$j < $num} {incr j} {
            lappend layout -1
        }
    }
    set is_file [expr {! $is_file}]
}

proc swap {lst lstart rstart len} {
    upvar $lst uplst
    set lend [expr {$lstart + $len - 1}]
    set rend [expr {$rstart + $len - 1}]
    set t [lrange $uplst $rstart $rend]
    set uplst [lreplace $uplst $rstart $rend {*}[lrange $uplst $lstart $lend]]
    set uplst [lreplace $uplst $lstart $lend {*}$t]
}

proc skip_space {layout rp} {
    while {[lindex $layout $rp] == -1 && $rp >= 0} {
        incr rp -1
    }
    return $rp
}

proc find_next_file {layout rp} {
    set c [lindex $layout $rp]
    while {$rp >= 0 && [lindex $layout $rp] == $c} {
        incr rp -1
    }
    incr rp
    return $rp
}

proc find_space {layout len end} {
    set size 0
    for {set i 0} {$i < $end} {incr i} {
        set x [lindex $layout $i]
        if {$x == -1} {
            incr size
        }
        if {$size == $len} {
            return [expr {$i - $len + 1}]
        }
        if {$x != -1} {
            set size 0
        }
    }
    return -1
}

set rp [expr {[llength $layout] - 1}]

while {$rp >= 0} {
    set rp [skip_space $layout $rp]
    if {$rp < 0} { break }
    set new_rp [find_next_file $layout $rp]
    set len [expr {$rp - $new_rp + 1}]
    if {$rp < 0} { break }
    set lp [find_space $layout $len $new_rp]
    if {$lp >= 0} {
        swap layout $lp $new_rp $len
    }
    set rp $new_rp
    incr rp -1
}
set checksum 0
set i 0
while {1} {
    if {$i >= [llength $layout]} {
        break
    }
    if {([lindex $layout $i] == -1)} {
        incr i
        continue
    }
    incr checksum [expr {[lindex $layout $i] * $i}]
    incr i
}
puts $checksum
