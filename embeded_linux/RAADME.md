# Real interview questions for embeded linux position
## 1. Oder of calculate function arguments: `printf("%d, %d", f(), g())`.

**Undefined.**

## 2. Order of put arguments on stack: `printf("%d, %d", f(), g())`.
 
**Left to right: g(), f(), "%d, %d". It's important for function with unknown number of arguments.**

## 3. System call pipeline

![image](https://user-images.githubusercontent.com/8722837/214515598-03291471-1538-460d-9d6d-5a87ad11c97b.png)

![image](https://user-images.githubusercontent.com/8722837/214516514-619780d7-3004-4b71-9060-9ef6d9240a57.png)


See fork.c

All system calls: https://thevivekpandey.github.io/posts/2017-09-25-linux-system-calls.html

## 4. Pipe

Unidirection (FIFO) access.

See pipe.c

## 5. Socket

Bidirection access, network oriented.

See socket.c

## 6. Network protocols (TCP/IP, HTML, ...)

## 7. Concurent read/write

