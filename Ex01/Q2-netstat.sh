# Q2 - netstat
# Print network connections, routing tables, interface statistics, 
# masquerade connections, and multicast memberships
netstat

# -s: Display summary statistics for each protocol
netstat -s

# -a: Show both listening and non-listening sockets
netstat -a

# -at: To list all tcp ports
netstat -at

# -au: To list all udp ports
netstat -au

# -l: To list only the listening ports
netstat -l

# -lt: To list only the listening tcp ports
netstat -lt

# -lu: To list only the listening udp ports
netstat -lu

# -i: Display a table of all network interfaces (like ifconfig -s)
netstat -i