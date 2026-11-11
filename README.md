*This project has been created as part of the 42 curriculum by mochoubi.*

# Get Next Line

## Description

The **Get Next Line** (GNL) project is a core programming assignment in the 42 network curriculum. Its primary goal is to code a function in C (`char *get_next_line(int fd)`) that reads and returns a single line of text from a specified file descriptor upon each invocation.

Calling `get_next_line()` repeatedly inside a loop allows reading an entire text stream sequentially—whether from a standard file or Standard Input (`stdin`)—until reaching the End of File (EOF) or encountering a read error.

### Key Learning Objectives
- **Static Variables**: Understanding lifetime and scope in C, storing leftover buffer data between function calls.
- **Dynamic Memory Management**: Allocating, concatenating, trimming, and freeing heap memory cleanly without memory leaks.
- **File I/O System Calls**: Mastering byte-stream retrieval using `read()` with adjustable buffer sizes specified at compilation (`BUFFER_SIZE`).
- **Concurrent Stream Handling**: Managing multiple file descriptors simultaneously using a single static variable array (bonus requirement).

---

## Instructions

### Compilation

The project must be compiled using `cc` with the flags `-Wall -Wextra -Werror` along with the macro flag `-D BUFFER_SIZE=n` to define the read chunk size.

To compile the mandatory part:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

To compile the bonus part (supporting multiple file descriptors):

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c -o gnl_bonus
```

### Execution

Execute the compiled program by passing a target file descriptor or running it with test files:

```bash
./gnl text_file.txt
```

### Buffer Size Flexibility

- If `-D BUFFER_SIZE` is omitted during compilation, the header uses a default buffer size (e.g., `42`).
- The function is designed to operate reliably across extreme buffer size values, from `BUFFER_SIZE=1` (byte-by-byte reading) to `BUFFER_SIZE=10000000` (large block allocation on the heap).

---

## Resources

### References & Documentation

- **System Calls & Manual Pages**:
  - Write System Call Technical Reference & Documentation
  - Linux Manual Pages: `man 2 read`, `man 2 open`, `man 2 close`, `man 3 malloc`, `man 3 free`
- **Visualization & Conceptual Tools**:
  - **tldraw**: Used to sketch visual diagrams of pointer movements, memory stashes, and string trimming operations.
  - **Gemini Notebook / NotebookLM**: Interactive platform used to visualize concept breakdowns, analyze edge-case behaviors, and organize evaluation defense preparation.

### AI Usage Declaration

In compliance with Chapter III (AI Instructions) of the 42 curriculum:

- **Tasks & Scope**: Artificial intelligence tools were consulted for conceptual explanations (understanding static variable storage segments).
