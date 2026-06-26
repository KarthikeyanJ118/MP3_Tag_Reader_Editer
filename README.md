# 🎵 MP3 Tag Reader & Editor using C

## 📌 Project Overview

This project is a **command-line MP3 Tag Reader and Editor** implemented in C. It allows users to **view and modify ID3 tags** of MP3 files such as title, artist, album, year, genre, and comments.

The program works by directly reading and editing **binary data of MP3 files**, demonstrating low-level file handling and data manipulation.

---

## ⚙️ Features

* View MP3 metadata (ID3 tags)
* Edit individual tags in MP3 files
* Supports multiple tag types (Title, Artist, Album, etc.)
* Input validation for all fields
* Handles endian conversion for correct data interpretation
* Command-line based interface
* Safe editing using temporary file handling

---

## 🛠️ Technologies Used

* C Programming
* File Handling (fopen, fread, fwrite)
* Structures & Modular Programming
* String Handling
* Bitwise / Binary Data Processing

---

## 🧠 How It Works

### 🔸 View Mode (`-v`)

1. Validates MP3 file
2. Skips ID3 header
3. Reads tags sequentially
4. Identifies tag types (TIT2, TPE1, etc.)
5. Displays corresponding metadata

---

### 🔸 Edit Mode (`-e`)

1. Validates input and tag type
2. Opens original MP3 file
3. Creates a temporary file (`temp.mp3`)
4. Copies header and scans tags
5. When target tag is found:

   * Updates tag data
   * Writes modified content
6. Copies remaining data
7. Replaces original file with updated version

---

## 📂 Project Structure

```
main.c
mp3.h
validation.c
view.c
edit.c
helper.c
```

---

## ▶️ Compilation

```
gcc *.c -o mp3_editor
```

---

## 🚀 Usage

### 🔹 View MP3 Tags

```
./mp3_editor -v <filename.mp3>
```

Example:

```
./mp3_editor -v song.mp3
```

---

### 🔹 Edit MP3 Tags

```
./mp3_editor -e <tagcode> <new_value> <filename.mp3>
```

Example:

```
./mp3_editor -e -t "New Title" song.mp3
```

---

## 🏷️ Supported Tag Codes

* `-t` → Title
* `-y` → Year
* `-A` → Album
* `-a` → Artist
* `-g` → Genre
* `-c` → Comment

---

## 🔐 Key Implementation Details

* Uses **ID3 tag frames** (e.g., TIT2, TPE1, TALB)
* Performs **endian conversion** for size fields
* Ensures safe updates using **temporary file rewriting**
* Validates all inputs before modification

---

## 📌 Example Output

```
Title: My Song
Artist: John Doe
Album: Demo Album
Year of Release: 2024
Genre of Song: Pop
Comment of file: Sample track
```

---

## 👨‍💻 Author

**Karthikeyan J**

---


