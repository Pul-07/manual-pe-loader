# Universal Manual PE Loader (Hardened)

A high-performance, stealthy manual Portable Executable (PE) loader designed for educational security research and advanced red teaming labs. This loader implements modern techniques to bypass EDR/AV monitoring by reconstructing binaries directly in memory.

## 🚀 Key Features

### 1. Obfuscation & Static Stealth
* **Polymorphic Encryption**: Uses a combined XOR + NOT logic with a dynamic key to neutralize static file signatures.
* **Pure In-Memory Execution**: The target payload never touches the disk in its decrypted form, leaving no forensic footprint on the filesystem.
* **IAT Independence**: Dynamically resolves necessary functions at runtime, keeping the Import Address Table (IAT) clean and unsuspicious.

### 2. Anti-EDR & Stealth Mechanisms
* **AMSI Patching**: In-memory neutralization of `AmsiScanBuffer` to bypass Windows script and buffer scanning.
* **ETW Patching**: Disables `EtwEventWrite` to prevent the reporting of suspicious telemetry to the security stack.
* **Indirect Syscalls (HellsGate/HalosGate)**: Dynamically extracts System Service Numbers (SSNs) to communicate directly with the Windows Kernel, bypassing User-Mode API hooks.

### 3. Advanced PE Loading Logic
* **Reflective Mapping**: Manual mapping of sections, handling of Base Relocations (ASLR), and IAT resolution.
* **System Compatibility**: 
    * Full support for **TLS Callbacks** (essential for CRT/C++).
    * **SEH (Structured Exception Handling)** registration for x64 stability.
    * **PEB Fixup**: Portable update of the `ImageBaseAddress` (Offset 0x10).
* **Memory Hygiene**: Finalizes memory protections by transitioning from `RWX` to legitimate `RX`/`R`/`RW` states.

## 🛠️ Build Instructions (Linux)

You can cross-compile this project for Windows using `mingw-w64` and `nasm`.

```bash
# 1. Assemble the syscall stub
nasm -f win64 syscalls.asm -o syscalls.o

# 2. Compile the loader
x86_64-w64-mingw32-gcc loader.c syscalls.o -o Loader.exe -Wall -O2 -lntdll
