## Description


---
## Protocol

    ECHO <INSERT_MESSAGE_LENGTH>\r\n\r\n<INSERT_MESSAGE_BODY>\r\n\r\n

* Protocol: ECHO
* MESSAGE_LENGTH:
* MESSAGE_BODY:
* `\r\n\r\n`:
  * Added at end of the header
  * Added at the end of the MESSAGE_BODY


Example

    ECHO 11\r\n\r\nHello World\r\n\r\n

---
## CMake

* https://cmake.org/cmake/help/latest/guide/tutorial/index.html


### Commands

#### Step-1: Make build folder (doesn't exist)

    mkdir build

#### Step-2: Go inside the build folder
    
    cd build

#### Step-3: 

    cmake ..

#### Step-4: 

    cmake --build .

#### Step-5: Run executable inside the ./build folder

     cd build 

    ./<INSERT_NAME>