set ns [new Simulator]
$ns color 1 Red
$ns color 2 Blue

set tf [open tcp-udp.tr w]
$ns trace-all $tf

set nf [open tcp-udp.nam w]
$ns namtrace-all $nf

proc finish {} {
	global ns nf tf
	$ns flush-trace 
	close $tf
	close $nf
	exec nam tcp-udp.nam &
	exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]

$ns duplex-link $n0 $n2 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns simplex-link $n2 $n3 0.3Mb 100ms DropTail
$ns simplex-link $n3 $n2 0.3Mb 100ms DropTail
$ns duplex-link $n3 $n4 0.5Mb 40ms DropTail
$ns duplex-link $n3 $n5 0.5Mb 40ms DropTail

# Check
$ns queue-limit $n2 $n3 10

$ns duplex-link-op $n0 $n2 orient right-down
$ns duplex-link-op $n1 $n2 orient right-up
$ns simplex-link-op $n2 $n3 orient right
$ns duplex-link-op $n3 $n4 orient right-up
$ns duplex-link-op $n3 $n5 orient right-down

set tcp [new Agent/TCP]
$tcp set class_ 1
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n4 $sink
$ns connect $tcp $sink
set ftp [new Application/FTP]
$ftp set type_ FTP
$ftp attach-agent $tcp

# Check
#$tcp set window_ 8000
#$tcp set packetSize_ 512

set udp [new Agent/UDP]
$udp set class_ 2
$ns attach-agent $n1 $udp
set null [new Agent/Null]
$ns attach-agent $n5 $null
$ns connect $udp $null
set cbr [new Application/Traffic/CBR]
$cbr set type_ CBR
$cbr attach-agent $udp

# Check
#$cbr set rate_ 0.1Mb
#$cbr set packetSize 1000
#$cbr set random_ false

$ns at 0.0 "$cbr start"
$ns at 0.0 "$ftp start"
$ns at 5.0 "$cbr stop"
$ns at 5.0 "$ftp stop"
$ns at 5.5 "finish"

$ns run
