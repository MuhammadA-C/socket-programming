## Description


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
