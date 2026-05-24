# 🚀 MiniRedis

<p align="center">
  <b>A Redis-inspired in-memory key-value database built from scratch in C++.</b><br>
  Custom Hash Table • LRU Cache • TCP Sockets • Multithreading • Persistence • CMake
</p>

---

# 📖 Overview

MiniRedis is a lightweight Redis-inspired in-memory key-value database implemented entirely in **C++**. It demonstrates how modern in-memory databases work internally by implementing the core building blocks from scratch instead of relying on STL containers.

The project features a multithreaded TCP server, a custom hash table with open addressing and linear probing, an LRU eviction policy, TTL-based key expiration, snapshot persistence, and graceful shutdown.

---

# ✨ Features

* ✅ TCP client-server architecture using Winsock2
* ✅ Concurrent client handling using `std::thread`
* ✅ Thread-safe database using `std::mutex`
* ✅ **Custom Hash Table** (FNV-1a hashing, linear probing, dynamic rehashing)
* ✅ **LRU Cache** implemented using a doubly linked list
* ✅ TTL-based key expiration
* ✅ Snapshot persistence (`dump.rdb`)
* ✅ Automatic background snapshots
* ✅ Graceful shutdown with final database save
* ✅ Modular architecture following separation of concerns

### Supported Commands

* `SET`
* `GET`
* `DEL`
* `EXISTS`
* `KEYS`
* `EXPIRE`
* `SAVE`

---

# 🏗️ Architecture

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
          ┌─────────┴─────────┐
          ▼                   ▼
   Custom Hash Table      LRU Cache
          │
          ▼
      Snapshot (dump.rdb)
```

---

# 📂 Project Structure

```text
MiniRedis/
│
├── client/
│   └── client.cpp
│
├── include/
│   ├── command_handler.h
│   ├── database.h
│   ├── hashtable.h
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
├── tests/
│   └── test_hashtable.cpp
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

---

# ⚙️ Tech Stack

* **Language:** C++17
* **Networking:** Winsock2 (TCP)
* **Concurrency:** `std::thread`
* **Synchronization:** `std::mutex`, `std::lock_guard`
* **Hashing:** FNV-1a
* **Data Structures:** Custom Hash Table, Doubly Linked List (LRU)
* **Build System:** CMake
* **Persistence:** File I/O (`fstream`)
* **Timing:** `std::chrono`

---

# 💻 Supported Commands

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

# 🧪 Example Session

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

# 💾 Persistence

MiniRedis supports snapshot persistence to ensure data durability.

* Loads the latest snapshot during startup.
* Supports manual snapshots using the `SAVE` command.
* Creates automatic background snapshots.
* Saves the database before graceful shutdown.

---

# 🧠 Internal Design

### Custom Hash Table

* Open Addressing
* Linear Probing
* FNV-1a Hash Function
* Dynamic Rehashing
* Tombstone-based Deletion

### LRU Cache

* Doubly Linked List
* O(1) insertion
* O(1) deletion
* O(1) access
* Automatic eviction when capacity is reached

---

# 🎯 Key Concepts Demonstrated

* TCP Socket Programming
* Client-Server Architecture
* Concurrent Programming
* Thread Synchronization
* Custom Hash Table Implementation
* LRU Cache Design
* Open Addressing & Linear Probing
* Snapshot Persistence
* In-Memory Databases
* Object-Oriented Design
* Modular Software Architecture

---

# 👨‍💻 Author

**Shivam Sachan**

