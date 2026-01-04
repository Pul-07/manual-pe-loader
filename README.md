# Manual PE Loader

**Manual PE Loader** is an *educational research project* that demonstrates how **native Windows PE files** can be manually loaded and executed in memory **without using the default Windows loader**.

> This project focuses on **understanding**, not replacing, the Windows loader.

---

## 📌 Overview

This loader implements a **minimal but realistic PE loading pipeline**, including:

- Manual memory mapping
- Relocations
- Import resolution
- TLS callbacks
- x64 SEH registration
- EXE vs DLL entry handling

It is intended for **learning, reverse engineering, and OS internals research**.

---

## 🎯 Project Goals

- Learn how the **Portable Executable (PE)** format works
- Understand what the Windows loader *requires* vs *assumes*
- Study manual relocation and import resolution
- Observe TLS and exception handling behavior
- Explore NT-level execution mechanics

---

## 🔁 Loader Workflow

1. **Validate PE headers**
   - DOS header (`MZ`)
   - NT header (`PE`)
2. **Allocate memory** for the image
3. **Copy headers and sections**
4. **Apply relocations** if needed
5. **Resolve imports**
   - By name
   - By ordinal
6. **Execute TLS callbacks**
7. **Register x64 exception tables**
8. **Apply final memory protections**
9. **Update PEB ImageBaseAddress**
10. **Execute entry point**
    - EXE → entry point
    - DLL → `DllMain(DLL_PROCESS_ATTACH)`

---

## 📦 Supported Binary Types

| Binary type | Supported |
|------------|-----------|
| Native EXE (x64) | ✅ Yes (many cases) |
| Native DLL | ✅ Yes |
| Complex native EXE | ⚠️ Partial |
| .NET / C# EXE | ❌ No |
| Mixed-mode (C++/CLI) | ❌ No |

### ❓ Why .NET binaries are not supported

.NET executables do **not** contain a real native entry point.  
They require **CLR hosting** and runtime initialization, which is outside the scope of a native PE loader.

---

## ⚙️ Syscall (Research Aspect)

This project includes a **minimal syscall invocation stub** to study:

- Windows x64 calling conventions
- Shadow space requirements
- NT system call behavior

Example (conceptual):

```asm
mov r10, rcx
mov eax, SSN
syscall


This is included strictly for educational purposes.

⚠️ Known Limitations

This loader does not implement:

Loader lists (PEB->Ldr)

Loader lock synchronization

Full CRT initialization

Delay-load imports

Dependency graph resolution

Process detach notifications

Because of this, some valid PE files may not execute correctly.

🧠 Why Many Binaries Still Work

Many native binaries:

Are self-contained

Do not depend on the CRT

Do not rely on loader lists

Require only minimal PE initialization

The Windows loader itself is tolerant, and this project reflects that reality.

📚 Educational Scope

This project helps illustrate:

How PE files actually start executing

Why relocations and imports matter

Why TLS callbacks still exist

Why SEH registration is mandatory on x64

Why .NET binaries are fundamentally different

Where the Windows loader draws the line

⚖️ Disclaimer

This repository is provided for educational and research purposes only.

It is not intended for:

malicious use

security bypassing

production deployment

Use responsibly and in compliance with applicable laws.

✨ Final Note

This project is not about bypassing Windows.
It is about understanding how Windows works internally.

Once you understand what is optional, you understand what is essential.


---

### Ce README utilise volontairement :
- `# ## ###` → titres
- `**gras**`, *italique*
- `` `inline code` ``
- blocs de code ``` ```
- listes ordonnées / non ordonnées
- tableaux
- citations `>`
- emojis GitHub compatibles

Si tu veux, je peux maintenant :
- le **raccourcir**
- le **traduire en français**
- faire une version **ultra académique**
- ou une version **étudiant / rapport**
