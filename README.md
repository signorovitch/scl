# SCL: Simple Calculator Language

Project structure:

```bash
$ tree -d
.
├── build       # Build directory; where binaries (except the main one) go.
│   ├── obj     # Project objects.
│   └── test    # Test build directory.
│       └── obj # Test objects.
├── src         # Source files (*.c).
│   └── include # Header files (*.h)
└── test        # Tests (file names match those in src/*.c).
    └── unity   # Unity test framework files.
```
