# HTTP Cache Poisoning Attacks: Response Splitting & Request Smuggling

This repository contains the implementation of advanced HTTP attacks focusing on **Cache Poisoning** vulnerabilities. The project demonstrates how protocol manipulation can desynchronize proxy servers and web servers, implemented in **C** using raw socket programming.

## 📌 Project Overview
The project targets a legacy environment (Apache 2.0.45) to demonstrate historical but foundational vulnerabilities in the HTTP/1.1 protocol. It consists of two main attack vectors:

1.  **HTTP Response Splitting**
2.  **HTTP Request Smuggling**

**Course:** Computer Security (67607) @ The Hebrew University of Jerusalem

---

## 📂 Project Structure

* **`ex3_splitting.c`**
    The C implementation of the **HTTP Response Splitting** attack. It establishes a TCP connection to the proxy and sends a crafted GET request containing CRLF injection (`%0d%0a`) in the URL parameters. This forces the server to split the response stream, allowing the injection of a malicious cache entry for a target page.

* **`ex3_smuggling.c`**
    The C implementation of the **HTTP Request Smuggling** attack. It exploits the discrepancy between how the proxy (using `Content-Length`) and the backend server (using `Transfer-Encoding: chunked`) parse request bodies. The code constructs a precise payload to "smuggle" a hidden HTTP request inside a visible one, poisoning the cache.

* **`explanation.txt`**
    A theoretical breakdown of the attacks. It details the specific vulnerabilities exploited in the `course_selector` script and the Apache 2.0.45 proxy/server combination, explaining the logic behind the cache poisoning mechanisms.

* **`readme.txt`**
    Submission metadata containing student identifiers, as required by the course submission guidelines.

---

## 🛠️ Technologies
* **Language:** C (Standard Headers, Socket Programming)
* **Networking:** TCP/IP, HTTP/1.1 Protocol Manipulation
* **Concepts:** Cache Poisoning, CRLF Injection, Request Desynchronization (CL.TE)

---

## 🚀 Attack Implementations details

### 1. HTTP Response Splitting
* **Vulnerability:** Unsanitized user input in the `course_selector` page embedded into HTTP headers.
* **Technique:** Sending a single request that the server interprets as two responses. The second, injected response is cached by the proxy for a different resource (`67607.html`).

### 2. HTTP Request Smuggling
* **Vulnerability:** Desynchronization due to conflicting `Content-Length` and `Transfer-Encoding` headers.
* **Technique:** The proxy sees a body length of 0 (Content-Length), while the backend processes the request as chunked. This allows a second, hidden request to `/poison.html` to be processed and cached for a subsequent victim request.

---

## 💻 Compilation & Usage

This code is designed to run in a specific Dockerized environment with vulnerable Apache servers.

```bash
# Compile Response Splitting Attack
gcc -Wall -Wextra -Werror ex3_splitting.c -o splitting_attack

# Compile Request Smuggling Attack
gcc -Wall -Wextra -Werror ex3_smuggling.c -o smuggling_attack
```

## ⚠️ Disclaimer
This code was written for educational purposes as part of a university assignment. It demonstrates known vulnerabilities in legacy software to understand network security concepts. **Do not use this code against unauthorized targets.**
