#Create a simulator object
set ns [new Simulator]

#Define a color for data flow (for NAM)
$ns color 1 Blue

#Open the NAM trace file
set nf [open outtahoe.nam w]
$ns namtrace-all $nf

#Define a 'finish' procedure
proc finish {} {
 global ns nf
 $ns flush-trace
 #Close the NAM trace file
 close $nf
 #Execute NAM on the trace file
 exec nam outtahoe.nam &
 exit 0
}

#Create three nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

#Create links between the nodes
$ns duplex-link $n0 $n1 10Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail

#Set Queue Size of link (n1-n2) to 10
$ns queue-limit $n1 $n2 10

#Give node position (for NAM)
$ns duplex-link-op $n0 $n1 orient right
$ns duplex-link-op $n1 $n2 orient right

#Setup a TCP connection
set tcp [new Agent/TCP]
$tcp set class_ 1
$ns attach-agent $n0 $tcp

set sink [new Agent/TCPSink]
$ns attach-agent $n2 $sink
$ns connect $tcp $sink
$tcp set fid_ 1

# >>> Asked in question 
$tcp set window_ 8000
$tcp set packetSize_ 552

#Setup a FTP over TCP connection
set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP

#used to plot the graph using gnuplot
proc plotWindow {tcpSource outfile} {
 global ns
 set now [$ns now]
 set cwnd [$tcpSource set cwnd_]
 puts $outfile "$now $cwnd"
 $ns at [expr $now+0.1] "plotWindow $tcpSource $outfile"
}

set outfile [open "congestion-tahoe.txt" w]
$ns at 0.0 "plotWindow $tcp $outfile"
$ns at 0.2 "$ftp start"
$ns at 1.0 "$ftp stop"
$ns at 1.0 "$ns detach-agent $n0 $tcp ; $ns detach-agent $n2 $sink"
$ns at 1.2 "finish"

#Run the simulation
$ns run