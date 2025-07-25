## Description



---
## Echo Client Server - TCP

* File: `TcpServer`
* File: `TcpClient`



---
## Echo Client Server - UDP

* File: `UdpServer`
* File: `UdpClient`

**Requirements:**
* Client sends message to server
* Server echos back the message to the client

**Assumptions:**
* The entire message will be sent with 1 sendto()

**Packet Structure**

    [MESSAGE_ID][SEQUENCE_NUMBER][TOTAL_PARTS][DATA]

* **MESSAGE_ID:** Unique identifier for the message
* **SEQUENCE_NUMBER:** Sequence number of the chunk (e.g. 0,1,2)
* **TOTAL_PARTS:** Total number of chunks expected
* **DATA:** Actual content (partial)


---
## Echo Client Server - Reliable UDP

* File: `ReliableUdpServer`
* File: `ReliableUdpClient`

**Requirements:**
* Client sends message to server
* Server echos back the message to the client
* Verify (ACK) that packets were received
* Remove duplicate packets
* Ensure packets are in the correct order

**Assumptions:**
* More than 1 sendto() is required to send the entire message

**Packet Structure**

    [MESSAGE_ID][SEQUENCE_NUMBER][TOTAL_PARTS][DATA]

* **MESSAGE_ID:** Unique identifier for the message
* **SEQUENCE_NUMBER:** Sequence number of the chunk (e.g. 0,1,2)
* **TOTAL_PARTS:** Total number of chunks expected
* **DATA:** Actual content (partial)


---
## Protocol

### Protocol Format

    ECHO <INSERT_MESSAGE_LENGTH>\r\n<INSERT_MESSAGE_BODY>\r\n

* **Protocol:** `ECHO`
* **MESSAGE_LENGTH:** The length of the message, which is used by client/server to know how many bytes will be sent
* **MESSAGE_BODY:** The actual message (text or bytes) that is sent to the client/server
* **\r\n\r\n**:
  * Added at end of the header
  * Added at the end of the MESSAGE_BODY

### Example

    ECHO 11\r\nHello World\r\n

---
## Developer Onboarding 

### CMake

* https://cmake.org/cmake/help/latest/guide/tutorial/index.html

#### Commands

##### Step-1: Make build folder (doesn't exist)

    mkdir build

##### Step-2: Go inside the build folder
    
    cd build

##### Step-3: 

    cmake ..

##### Step-4: 

    cmake --build .

##### Step-5: Run executable inside the ./build folder

     cd build 

    ./<INSERT_NAME>
