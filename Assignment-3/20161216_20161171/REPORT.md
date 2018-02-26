# Wireshark Assignment on Packet Sniffing

## HTTP

1. **BROWSER**: HTTP/1.1
   **SERVER**: HTTP/1.1
   **LANGUAGE**: en-US,en;q=0.9,hi;q=0.8,la;q=0.7

2. **HOST**: 192.168.43.77
   **SERVER**: 128.119.245.12

3. **STATUS**: 200 OK

4. **Last-Modified**: Sun, 25 Feb 2018 06:59:02 GMT

5. **File Data**: 128 bytes

## HTTP CONDITIONAL GET

7. No, there is no **If-Modified-Since** header in the first request.

8. Yes.
  The **Line-Based text data** field shows the data sent back by the server,
  which is the same as the content on the web page.

9. Yes.
  It shows the time and date the page was accessed last.

10. **HTTP Status Code**: 304
  Server did not return the contents of the file since the file was
  cached and hadn't been modified since the last access time.

## DNS

 1. DNS Queries are sent over *UDP*.

 2. Answers provided:
 	```
     www.ietf.org: type CNAME, class IN, cname www.ietf.org.cdn.cloudflare.net
        Name: www.ietf.org
        Type: CNAME (Canonical NAME for an alias) (5)
        Class: IN (0x0001)
        Time to live: 0
        Data length: 33
        CNAME: www.ietf.org.cdn.cloudflare.net
    
    www.ietf.org.cdn.cloudflare.net: type A, class IN, addr 104.20.1.85
        Name: www.ietf.org.cdn.cloudflare.net
        Type: A (Host Address) (1)
        Class: IN (0x0001)
        Time to live: 0
        Data length: 4
        Address: 104.20.1.85
    www.ietf.org.cdn.cloudflare.net: type A, class IN, addr 104.20.0.85
        Name: www.ietf.org.cdn.cloudflare.net
        Type: A (Host Address) (1)
        Class: IN (0x0001)
        Time to live: 0
        Data length: 4
        Address: 104.20.0.85
    ```
  
 3. Yes, it corresponds to the address sent by the DNS request **104.20.1.85**

 4. No, it did not.

 5. ![](DSN_1.png)


 6. Destination Port of DNS query message : 53
   Source Port of DNS response message : 53

 7. It is a **type A** query and does not contain any answers.

 8. It is a **type A**.
   ```
   www.mit.edu: type A, class IN
   ```
   Yes, it contains 3 answers.
   ```
    www.mit.edu: type CNAME, class IN, cname www.mit.edu.edgekey.net
    www.mit.edu.edgekey.net: type CNAME, class IN, cname e9566.dscb.akamaiedge.net
    e9566.dscb.akamaiedge.net: type A, class IN, addr 23.35.75.93
   ```
 9. DNS Query is being sent to the local DNS Server. (IP Address: 192.168.42.129)

 10. Query Type: **NS**. It did not contain any answers.

 11. Responses provided are:

	 ```
	www.mit.edu: type CNAME, class IN, cname www.mit.edu.edgekey.net
	www.mit.edu.edgekey.net: type CNAME, class IN, cname e9566.dscb.akamaiedge.net

	Authoritative nameservers:
	dscb.akamaiedge.net: type SOA, class IN, mname n0dscb.akamaiedge.net
	 

 No. Additional records were not sent.

  	!(DSN_2.png)


## TCP

 1. **IP Address**: 192.168.1.102
   **PORT**: 1161

 2. **IP Address**: 128.119.245.12
   **PORT**: 80
##### Analyzing TCP behavior: tcpethereal-trace-1 

 1. **SEQ**: 0
   *SYN* Tag present in the request determines it as a SYN Segment.

 2. **SEQ**: 0
   **ACK**: 1
   Determined by the segment number to send next.
   Both, *SYN* and *ACK* bits are present.

 3. Required sequence number is **164041**.

 4. 1: 54
    2: 619
    3: 1514
    4: 1514
    5: 1514
    6: 1514

5. The minimum amount of available buffer space is 5840 bytes.
  No, since we never reach full capacity of the window.

6. No. No previous **ACK** segment was sent by server.

## UDP
 
 1. Four fields:
 	- Source Port
	- Destination Port
	- Length
	- Checksum
 
 2. Total length : 37 bytes
   Header : 8 bytes, Data : 8 bytes

 3. Source Address : **192.168.43.77**
   Verified using **ifconfig** command

 4. Destination Address : **192.168.43.1**

 5. Maximum bytes allowed is ```65535 - 8 = 65527``` bytes. 

 6. Largest possible port number is **65536**

 7. UDP Protocol Number : 17

 8. UDP checksum is the complement of a 16-bit one's complement sum
   calculated over an IP "pseudo-header" and the actual UDP data.
 
 9. The source port on the send message is the destination port of the receive message.
   The destination port on the send message is the source port of the receive message.


 UPD Packets Analyzed:

 	![](UPD_1.pdf)
 	![](UPD_2.pdf)