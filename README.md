# Git Lite Project

## Overview
Git Lite is a simplified version control system designed to manage and track changes in a file system efficiently. The project focuses on providing robust file organization, version management, and data integrity verification.

## Features

### 1. **File System Implementation**
- Developed using file-based trees:
  - **B-Trees**: For efficient indexing and searching.
  - **AVL Trees**: To maintain balanced data structures with self-balancing properties.
  - **Red-Black Trees**: For faster rebalancing during insertions and deletions.

### 2. **Version Control**
- Tracks changes to files and directories.
- Provides basic commit, rollback, and version comparison functionalities.

### 3. **Data Integrity with Merkle Trees**
- Integrated **Merkle Trees** for ensuring data integrity.
- Each node in the Merkle Tree stores a hash, allowing quick detection of changes or corruption in any part of the file system.
- Ensures secure and reliable data verification.

### 4. **Data Corruption Detection**
- The system detects data corruption by comparing the hash values in the Merkle Tree.
- Ensures users can identify and recover from file inconsistencies or tampering.

## Key Benefits
- **Efficiency**: Balanced tree structures optimize storage and retrieval.
- **Reliability**: Hash-based integrity checks provide robust protection against data corruption.
- **Scalability**: The system is designed to handle growing file systems without performance degradation.

## Technologies Used
- **Data Structures**: B-Trees, AVL Trees, Red-Black Trees, Merkle Trees.
- **Programming Concepts**: File systems, hashing, and version control mechanisms.

This project provided hands-on experience with advanced data structures, system design, and ensuring data reliability in real-world applications.
