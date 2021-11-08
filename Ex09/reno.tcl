set ns [new Simulator]
$ns color 1 Red

set nf [open reno.nam w]
$ns namtrace-all $nf

proc finish {} {
	global ns nf
	$ns flush-trace
	close $nf 
	exec nam reno.nam &
	exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

$ns duplex-link $n0 $n1 10Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail

# Check
$ns queue-limit $n1 $n2 10

$ns duplex-link-op $n0 $n1 orient right
$ns duplex-link-op $n1 $n2 orient right

set tcp [new Agent/TCP/Reno]
$tcp set class_ 1
$ns attach-agent $n0 $tcp

set sink [new Agent/TCPSink]
$ns attach-agent $n2 $sink
$ns connect $tcp $sink

set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP

# Check
#$tcp set window_ 8000
#$tcp set packetSize_ 552

proc plotWindow {tcp outfile} {
	global ns
	set now [$ns now]
	set cwnd [$tcp set cwnd_]
	puts $outfile "$now $cwnd"
	$ns at [expr $now + 0.1] "plotWindow $tcp $outfile" 
}

set outfile [open reno.txt w]

$ns at 0.0 "plotWindow $tcp $outfile"
$ns at 0.0 "$ftp start;"
$ns at 1.0 "$ftp stop;"
$ns at 1.0 "finish"

$ns run
