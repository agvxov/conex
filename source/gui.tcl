# @BAKE tclsh $@
package require Tk
package require Ttk

if {![info exists ::WRAPPED]} {
    proc cPutRegex {t} {}
    proc cPutInput {i} {}
    proc cGetCorrectness {} {}
    proc cPutEngine {} {}
    proc cGetCompileError {} { return "" }
    set engines {Perl GNU Python}
} else {
    set engines [split [cGetEngines]]
}

# --- Init UI ---
wm title . "Conex - regex builder"

option add *Font "TkDefaultFont 20"

frame .i
pack .i -fill x
frame .msg
pack .msg -fill x
ttk::separator .sep -orient horizontal
pack .sep -fill x -padx 10 -pady 5
frame .o
pack .o -fill both -expand 1

#

text .i.regex_input -height 1
pack .i.regex_input -fill x -side left

tk_optionMenu .i.engine_menu regex_engine {*}$engines
pack .i.engine_menu -side left

#

label .msg.compile_message
.msg.compile_message configure -text "N/A"
pack .msg.compile_message -side left

#

text .o.text_input -width 40 -height 5
pack .o.text_input -fill both -expand 1

.o.text_input tag configure true  -background lightgreen
.o.text_input tag configure match -background yellow
.o.text_input tag configure false -background red

# --- Logic ---
bind .i.regex_input <<Modified>> {
    .i.regex_input edit modified 0
    cPutRegex [.i.regex_input get 1.0 end]
}

bind .o.text_input <<Modified>> {
    .o.text_input edit modified 0
    cPutInput [.o.text_input get 1.0 end]
}

trace variable regex_engine w {cPutEngine $regex_engine}

proc pollCorrectness {} {
    set compile_error [cGetCompileError]
    if {$compile_error == ""} {
        .msg.compile_message configure -text "Regex compiled."
    } else {
        .msg.compile_message configure -text $compile_error
    }

    set results [split [cGetCorrectness]]
    set n [llength $results]
    for {set i 0} {$i < $n} {incr i} {
        set line [expr $i + 1]
        if {[lindex $results $i] == 1} {
            .o.text_input tag add true $line.0 $line.end
        } else {
            .o.text_input tag remove true $line.0 $line.end
        }
    }
    after 100 pollCorrectness
}
pollCorrectness
