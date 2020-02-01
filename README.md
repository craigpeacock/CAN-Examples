# CAN-Examples
Example C code for SocketCAN on Linux

* Based on documentation at: https://www.kernel.org/doc/Documentation/networking/can.txt
* A simplified tutorial can be found at: https://www.beyondlogic.org/example-c-socketcan-code/
* Hardcoded to use the vcan0 virtual CAN device.

To install VCAN0:
```
$ ip link add dev vcan0 type vcan
```
Use gcc to build examples:
```
$ gcc cantransmit.c -o cantransmit

$ gcc canreceive.c -o canreceive

$ gcc canfilter.c -o canfilter
```
