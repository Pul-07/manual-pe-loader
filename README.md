Manual PE Loader – Research Project
📌 Overview

This project is an educational manual PE loader for Windows, written to study and understand how the Windows loader works internally.

It demonstrates how a Portable Executable (PE) can be:

loaded manually in memory,

relocated,

linked,

initialized,

and executed without relying on CreateProcess or the default loader path.

⚠️ This project is intended for research, learning, and defensive understanding only.

🎯 Goals of the Project

The purpose of this loader is to explore:

Windows PE format internals

Manual memory mapping

Relocations and imports resolution

TLS callbacks handling

x64 SEH / exception registration

Entry point logic for EXE vs DLL

The minimal requirements for a PE to run correctly

Differences between what is required vs what is optional in the Windows loader

This is not meant to be a replacement for the Windows loader.

🧠 High-Level Architecture

The loader follows these main steps:

PE validation

DOS header (MZ)

NT header (PE)

Architecture consistency

Memory allocation

Allocate memory for the full image

Copy headers and sections

Relocations

Apply base relocations if the image is not loaded at its preferred base

Import resolution

Resolve imports by name

Resolve imports by ordinal

Load required DLLs

TLS callbacks

Execute TLS callbacks before the entry point

Pass DLL_PROCESS_ATTACH when applicable

Exception handling (x64)

Register the exception directory using RtlAddFunctionTable

Required for proper stack unwinding and stability

Memory protections

Apply final section protections (RX / RW / R)

Transition from writable mapping to execution-safe memory

PEB adjustment

Update PEB->ImageBaseAddress using NtQueryInformationProcess

Avoids hardcoded offsets and improves portability

Execution

EXE: call the entry point directly

DLL: call DllMain(DLL_PROCESS_ATTACH)

🧩 Supported Binary Types
Binary Type	Supported
Native EXE (x64)	✅ Yes (many cases)
Native DLL	✅ Yes
Complex native EXE	⚠️ Depends on features used
.NET / C# EXE	❌ No
Mixed-mode (C++/CLI)	❌ No
Why .NET binaries are not supported

.NET executables do not contain a real native entry point.
They require:

CLR initialization

_CorExeMain

Runtime hosting

This loader is a native PE loader, not a CLR host.

⚙️ Syscall Usage (Research Aspect)

The project includes a minimal syscall invocation stub to study:

Windows x64 calling conventions

Shadow space handling

Direct interaction with NT system calls

The syscall logic is implemented in assembly and follows the x64 ABI:

RCX → R10

SSN loaded into EAX

Proper stack alignment and cleanup

This is used only to study NT internals, not to replace standard APIs.

⚠️ Known Limitations

This loader intentionally does not implement:

Loader lists (PEB->Ldr)

Loader lock synchronization

Full CRT initialization

Delay-load import handling

Dependency graph resolution

Thread notifications (PROCESS_DETACH)

Full process environment setup

As a result:

Some valid PE files will not run

Behavior may vary depending on Windows version and binary complexity

This is expected and by design.

🧪 Why Many Binaries Still Work

Many native binaries:

do not rely on the CRT

do not use delay imports

do not inspect loader lists

only require basic PE initialization

The Windows loader itself is tolerant, and this project leverages that fact.

📚 Educational Value

This project is useful to understand:

How PE files really start executing

Why relocations and imports matter

Why TLS callbacks still exist

Why SEH registration is mandatory on x64

Why .NET binaries are fundamentally different

What Windows actually requires vs assumes

⚖️ Disclaimer

This repository is provided for educational and research purposes only.

The author does not encourage or support:

unauthorized use

bypassing security mechanisms

misuse in production environments

Use responsibly and legally.

🧠 Final Note

This project is not about bypassing Windows,
it is about understanding how Windows works.

“Once you know what is optional, you understand what is essential.”
