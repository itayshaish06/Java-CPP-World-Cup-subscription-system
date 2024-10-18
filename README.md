# BGU SPL Assignment 3: World Cup Update Subscription Service

This project implements a **community-led World Cup update subscription service** using the **STOMP** (Simple Text Oriented Messaging Protocol). 

The project consists of two main components: a **Java-based server** and a **C++ client**. 

Users can subscribe to a game channel to report and receive updates about World Cup games.

## Overview

The project implements a multi-client messaging system using **Java** for the server-side and **C++** for the client-side. The server supports both the **Thread-Per-Client (TPC)** and **Reactor** models.

Clients can subscribe to game channels to receive live updates and report on the events of World Cup games.

## Features

- **Client-Server Communication via STOMP**: Clients can send and receive messages from other clients via the server.
- **Subscription Model**: Clients can subscribe to game channels to receive updates or send reports.
- **Multi-threading Support**: The server can handle multiple clients concurrently using the TPC or Reactor pattern.
- **Game Event Reporting**: Clients can send detailed reports of game events such as goals, possession changes, and more.
- **Client Management**: The server tracks active clients and their subscriptions, ensuring efficient communication between users.

## STOMP Protocol

This project uses the **STOMP 1.2** protocol for client-server communication. STOMP is a simple, text-based messaging protocol that enables asynchronous message exchange.

### Example STOMP Frames:
- **CONNECT Frame**: Used by the client to initiate a connection to the server.
- **SUBSCRIBE Frame**: Allows a client to subscribe to a game channel.
- **SEND Frame**: Sends a report or update to the server, which forwards it to all subscribed clients.
- **DISCONNECT Frame**: Ends the client's session with the server.

For more details, see the [STOMP 1.2 specification](https://stomp.github.io/stomp-specification-1.2.html).

## Project Structure

- **Client**: C++ client that interacts with the server and handles game events.
- **Server**: Java-based server that manages client connections and broadcasts messages.

## Installation and Usage

### Build and Run Instructions
The Project was used on linux ubuntu os.

#### Server(TPC) OR Server(Reactor):
Run `build_TPC_Server.sh` OR `build_Reactor_Server.sh`.

#### Client:
Run `buildClient.sh`.

# STOMP World Cup Informer Client Commands

## Login Command
- **Structure:** `login {host:port} {username} {password}`
- **Description:** Sends a CONNECT frame to the server.
- **Possible Outputs:**
  - "Could not connect to server"
  - "The client is already logged in, log out before trying again"
  - "Login successful"
  - "User already logged in"
  - "Wrong password"

## Join Game Channel Command
- **Structure:** `join {game_name}`
- **Description:** Sends a SUBSCRIBE frame to the {game_name} topic.
- **Output:** "Joined channel {game_name}"

## Exit Game Channel Command
- **Structure:** `exit {game_name}`
- **Description:** Sends an UNSUBSCRIBE frame to the {game_name} topic.
- **Output:** "Exited channel {game_name}"

## Report to Channel Command
- **Structure:** `report {file}`
- **Description:** 
  1. Reads and parses the provided {file}
  2. Saves each event on the client
  3. Sends a SEND frame for each game event to the {game_name} topic

## Summarize Game Command
- **Structure:** `summary {game_name} {user} {file}`
- **Description:** Prints game updates from {user} for {game_name} into the provided {file}.

## Logout Command
- **Structure:** `logout`
- **Description:** Sends a DISCONNECT frame to the server and closes the socket upon receiving a RECEIPT frame.
