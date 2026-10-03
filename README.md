## BitsZipper

**Version: 1.0.0**

Compressing and decompressing files by byte packing and unpacking

---

### Features
- Lossless Compression (no loss of data)
- Automatically computes the minimum required bits per character based on unique characters
- Packs multiple characters in a single byte
- Works on 8-bit ASCII standard

### Project Structure

```text

├── .gitignore
├── README.md
└── src/
    ├── compressor.cpp      # compresses the file
    ├── decompressor.cpp    # decompresses the file
    |── testing/            # test cases
    └── x64Exe/
        |── compressor.exe      # executable file for compressor
        └── decompressor.exe    # executable file for decompressor
```

### How To Use

1. Download **compressor.exe** and **decompressor.exe** from x64Exe folder.
2. Put the file to compress in the same directory as the executable files.
3. Run the **compressor.exe** first which generates a compressed and key file.
4. Then run the **decompressor.exe** which creates a decompressed file.
5. Both the orignal and decompressed files should match.