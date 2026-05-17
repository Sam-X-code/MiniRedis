# 🚀 MiniRedis

<p align="center">
  <b>A Redis-inspired in-memory key-value database built from scratch in C++.</b><br>
  TCP Sockets • Multithreading • Thread Safety • Persistence • CMake
</p>

<p align="center">




\

</p>

---

## 📖 Overview

MiniRedis is a lightweight Redis-inspired in-memory database implemented in **C++**. It demonstrates the core concepts behind modern backend systems, including TCP networking, concurrent client handling, thread-safe data storage, command parsing, TTL-based expiration, snapshot persistence, and graceful server shutdown.

The project is designed to understand how a real key-value database works internally rather than relying on external libraries.

---

## ✨ Features

* ✅ TCP client-server architecture using Winsock
* ✅ Multi-client support using `std::thread`
* ✅ Thread-safe database using `std::mutex`
* ✅ In-memory storage with `std::unordered_map`
* ✅ Redis-inspired commands

  * `SET`
  * `GET`
  * `DEL`
  * `EXISTS`
  * `KEYS`
  * `EXPIRE`
  * `SAVE`
* ✅ Snapshot persistence (`dump.rdb`)
* ✅ Automatic background snapshots
* ✅ Graceful shutdown with final database save
* ✅ Modular architecture following separation of concerns

---

## 🏗️ Architecture

```text
                Client 1
                    │
                TCP Socket
                    │
                    ▼
             ┌──────────────┐
             │    Server    │
             └──────────────┘
                    │
          accept() + std::thread
                    │
      ┌─────────────┴─────────────┐
      ▼                           ▼
 handleClient()             handleClient()
      │                           │
      └─────────────┬─────────────┘
                    ▼
            CommandHandler
                    │
                    ▼
                 Parser
                    │
                    ▼
                Database
                    │
              std::mutex
                    │
                    ▼
            unordered_map
                    │
                    ▼
               dump.rdb
```

---

## 📂 Project Structure

```text
MiniRedis/
│
├── client/
│   └── client.cpp
│
├── include/
│   ├── command_handler.h
│   ├── database.h
│   ├── parser.h
│   └── server.h
│
├── src/
│   ├── command_handler.cpp
│   ├── database.cpp
│   ├── parser.cpp
│   ├── server.cpp
│   └── main.cpp
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

---

## ⚙️ Tech Stack

* **Language:** C++17
* **Networking:** Winsock2 (TCP)
* **Build System:** CMake
* **Concurrency:** `std::thread`
* **Synchronization:** `std::mutex`, `std::lock_guard`
* **Storage:** `std::unordered_map`
* **Persistence:** File I/O (`fstream`)
* **Timing:** `std::chrono`

---

## 💻 Supported Commands

| Command              | Description            |
| -------------------- | ---------------------- |
| `SET key value`      | Store a key-value pair |
| `GET key`            | Retrieve the value     |
| `DEL key`            | Delete a key           |
| `EXISTS key`         | Check if a key exists  |
| `KEYS`               | List all keys          |
| `EXPIRE key seconds` | Set key expiration     |
| `SAVE`               | Save snapshot to disk  |

---

## 🧪 Example Session

```text
> SET name Shivam
OK

> GET name
Shivam

> EXISTS name
1

> KEYS
name

> EXPIRE name 5
OK

> GET name
(nil)

> SAVE
OK
```

---

## 💾 Persistence

MiniRedis supports snapshot persistence.

* Loads the latest snapshot during startup.
* Manual snapshots using the `SAVE` command.
* Automatic snapshots in a background thread.
* Final snapshot during graceful shutdown.

---

## 🎯 Key Concepts Demonstrated

* TCP Socket Programming
* Client-Server Architecture
* Concurrent Programming
* Thread Synchronization
* Command Parsing
* In-Memory Databases
* Snapshot Persistence
* Object-Oriented Design
* Modular Software Architecture

---

## 👨‍💻 Author

**Shivam Sachan**
