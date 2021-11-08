set ns [new Simulator]
$ns color 1 Red
$ns color 2 Blue

set nf [open dv.nam w]
$ns namtrace-all $nf

set tf [open dv.tr w]
$ns trace-all $tf

proc finish {} {
	global ns nf tf
	$ns flush-trace
	close $nf
	close $tf
	exec nam dv.nam &
	exit 0 
}

for {set i 0} {$i < 12} {incr i} { set n($i) [$ns node]}
for {set i 0} {$i <  8} {incr i} { $ns duplex-link $n($i) $n([expr $i + 1]) 1Mb 10ms DropTail}

$ns duplex-link $n(0) $n(8) 1Mb 10ms DropTail
$ns duplex-link $n(0) $n(9) 1Mb 10ms DropTail
$ns duplex-link $n(1) $n(10) 1Mb 10ms DropTail
$ns duplex-link $n(9) $n(11) 1Mb 10ms DropTail
$ns duplex-link $n(10) $n(11) 1Mb 10ms DropTail
$ns duplex-link $n(11) $n(5) 1Mb 10ms DropTail

set udp0 [new Agent/UDP]
$udp0 set class_ 1
$ns attach-agent $n(0) $udp0
set null0 [new Agent/Null]
$ns attach-agent $n(5) $null0
$ns connect $udp0 $null0
set cbr0 [new Application/Traffic/CBR]
$cbr0 attach-agent $udp0
$cbr0 set type_ CBR

# Check
#$cbr0 set packetSize_ 500
#$cbr0 set interval_ 0.005

set udp1 [new Agent/UDP]
$udp1 set class_ 2
$ns attach-agent $n(1) $udp1
set null1 [new Agent/Null]
$ns attach-agent $n(5) $null1
$ns connect $udp1 $null1
set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1
$cbr1 set type_ CBR

# Check
#$cbr1 set packetSize_ 500
#$cbr1 set interval_ 0.005

$ns rtproto DV

$ns rtmodel-at 11.0 down $n(11) $n(5)
$ns rtmodel-at 11.5 down $n(7) $n(6)
$ns rtmodel-at 12.0 up $n(11) $n(5)
$ns rtmodel-at 12.5 up $n(7) $n(6)

$ns at 0.0 "$cbr0 start"
$ns at 0.0 "$cbr1 start"
$ns at 15.0 "$cbr0 stop"
$ns at 15.0 "$cbr1 stop"
$ns at 15.0 "finish"
$ns run
