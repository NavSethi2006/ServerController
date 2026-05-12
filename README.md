# ServerController

A lightweight remote Minecraft server controller backend written in C.

This project acts as a relay/controller server that allows authenticated clients to:

- Wake a remote Minecraft host PC using Wake-on-LAN
- Start and stop the Minecraft server remotely
- Authenticate securely using SHA-256 challenge-response
- Maintain persistent client sessions
- Log and monitor connections

Designed to run efficiently on low-power hardware such as Raspberry Pi systems or lightweight Linux servers.

---

# Features

- TCP server backend
- SHA-256 challenge-response authentication
- Wake-on-LAN support
- Persistent authenticated sessions
- Command handling system
- Lightweight C implementation
- Logging system
- Linux/Raspberry Pi friendly

---

# Authentication Flow

The server uses a challenge-response authentication model.

## Process

1. Client connects
2. Server generates random challenge
3. Client computes:
4. Client sends computed hash
5. Server validates hash
6. Client becomes authenticated

The password itself is never transmitted across the network.

---

# Server Commands

| Command | Description |
|---|---|
| `WOL` | Sends Wake-on-LAN packet |
| `START` | Starts Minecraft server |
| `STOP` | Stops Minecraft server |
| `PING` | Status check |
---
