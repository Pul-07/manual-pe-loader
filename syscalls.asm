; Fichier: syscalls.asm
global InternalSyscall
extern g_ssn
extern g_syscall_addr

section .text

InternalSyscall:
    mov r10, rcx                ; Convention NT
    mov eax, [rel g_ssn]
    
    sub rsp, 40                 ; Shadow space (32) + Alignement (8)
    
    mov r11, [rel g_syscall_addr]
    test r11, r11
    jz _error
    
    call r11                    ; Appel du gadget (permet de revenir ici)
    
    add rsp, 40                 ; Nettoyage propre
    ret                         ; Retourne à l'appelant C avec RAX intact

_error:
    mov rax, 0xC0000001         ; STATUS_UNSUCCESSFUL
    add rsp, 40
    ret
