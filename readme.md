# Virtual Disk Management System

This project provides a comprehensive virtual disk management system, allowing users to create, manage, and interact with virtual disks. The system includes tools for adding, retrieving, deleting, listing, and defragmenting files stored within the virtual disk, utilizing custom encoding and decoding algorithms for efficient storage and metadata handling.

## Features

- **Create Virtual Disk**: Initialize a new virtual disk file.
- **Add Files**: Store files on the virtual disk, along with metadata.
- **Retrieve Files**: Extract a file from the virtual disk to the local filesystem.
- **Delete Files**: Remove files from the virtual disk and update metadata.
- **List Files**: Display stored files along with their metadata (e.g., size, name).
- **Defragment Disk**: Optimize disk space by rearranging files and metadata.
- **Count Files**: Retrieve the number of files stored in the virtual disk.

## Technology Stack

- **Programming Language**: C
- **Build System**: GCC, Makefile
- **Data Handling**: Custom encoding and decoding techniques, bit-level manipulation
- **File System Operations**: File I/O, metadata management, and space allocation

## Prerequisites

1. **C Compiler**: Ensure `gcc` is installed on your system.
2. **Build Tools**: Make sure `make` is installed for building the project.
3. **Environment**: A Linux or Unix-based environment is recommended for execution.

## Build Instructions

1. Clone the repository:

   ```bash
   git clone https://github.com/ranjanshashi9471/virtualDisk
   cd virtualDisk
   ```

2. Build the project using `make`:

   ```bash
   make
   ```

3. The following executables will be created:
   - `create_vd`
   - `vdAdd`
   - `vdget`
   - `vddelete`
   - `vdls`
   - `vdcount`
   - `vddefrag`

## Usage

### 1. Create Virtual Disk

```bash
./create_vd <disk_name> <size_in_bytes>
```

Example:

```bash
./create_vd vd 1048576
```

### 2. Add File to Disk

```bash
./vdAdd <disk_name> <file_name>
```

Example:

```bash
./vdAdd vd example.txt
```

### 3. Retrieve File from Disk

```bash
./vdget <disk_name> <file_no> <destination_file>
```

Example:

```bash
./vdget vd 1 retrieved_example.txt
```

### 4. Delete File from Disk

```bash
./vddelete <disk_name> <file_no>
```

Example:

```bash
./vddelete vd 2
```

### 5. List Files in Disk

```bash
./vdls <disk_name>
```

Example:

```bash
./vdls vd
```

### 6. Count Files in Disk

```bash
./vdcount <disk_name>
```

Example:

```bash
./vdcount vd
```

### 7. Defragment Disk

```bash
./vddefrag
```

Example:

```bash
./vddefrag vd
```

## Cleaning Build Files

To remove all build artifacts and executables, run:

```bash
make clean
```

## Project Structure

- **Source Files**: C files implementing various functionalities
- **Header Files**: Shared utility functions (e.g., `encdec.h`)
- **Makefile**: Build automation script for compiling the project

<!-- ## License

This project is open-source and available under the [MIT License](LICENSE). -->

## Author

Shashi Ranjan Kumar  
ranjanshashi9471@gmail.com
