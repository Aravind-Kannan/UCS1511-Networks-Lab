# Q1 - tcpdump
# Packet sniffing/ analyzing tool
sudo tcpdump

# -D: List all network interfaces
sudo tcpdump -D

# -i: Listen on speceified interface, here, lo
sudo tcpdump -i lo

# -c: Exit after receiving fixed count of packets, here, 4
sudo tcpdump -c 4 -i lo