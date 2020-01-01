# CAN-Examples
Example C code for SocketCAN on Linux

Based on documentation at:
https://www.kernel.org/doc/Documentation/networking/can.txt

Hardcoded to use the VCAN0 virtual CAN device

To install:
$ sudo ip link add type vcan
$ ip link add dev vcan0 type vcan

