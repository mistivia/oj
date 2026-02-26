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

set lp 0
set rp [expr {[llength $layout] - 1}]

proc swap {lst a b} {
    upvar $lst uplst
    set t [lindex $uplst $b]
    lset uplst $b [lindex $uplst $a]
    lset uplst $a $t
}

while {$lp < $rp} {
    if {[lindex $layout $lp] != -1} {
        incr lp
        continue
    }
    if {[lindex $layout $rp] == -1} {
        incr rp -1
        continue
    }
    swap layout $lp $rp
}

set checksum 0
set i 0
while {1} {
    if {([lindex $layout $i] == -1) || ($i > [llength $layout])} {
        break
    }
    incr checksum [expr {[lindex $layout $i] * $i}]
    incr i
}
puts $checksum
