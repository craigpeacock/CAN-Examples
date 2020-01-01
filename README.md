# CAN-Examples
Example C code for SocketCAN on Linux

Based on documentation at:
https://www.kernel.org/doc/Documentation/networking/can.txt

Hardcoded to use the VCAN0 virtual CAN device

To install VCAN0:

$ sudo ip link add type vcan

$ ip link add dev vcan0 type vcan

To build examples:

$ gcc cantransmit.c -o cantransmit

$ gcc canreceive.c -o canreceive

$ gcc canfilter.c -o canfilter
