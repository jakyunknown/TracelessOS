<div align="center">

# TRACELESS OS
### *Privacy is a right, not a privliage*

A privacy-hardened, security-focused operating system built **from scratch** in C and x86 Assembly — by a 15 year old who decided to build an OS instead of doing homework.

![Status](https://img.shields.io/badge/status-active%20development-brightgreen?style=for-the-badge&logo=linux&logoColor=white)
![Language](https://img.shields.io/badge/language-C%20%2B%20x86%20Assembly-00ff88?style=for-the-badge&logo=c&logoColor=black)
![License](https://img.shields.io/badge/license-GPL%20v3-blue?style=for-the-badge)
![Base](https://img.shields.io/badge/kernel-custom%20from%20scratch-red?style=for-the-badge)
![Arch](https://img.shields.io/badge/arch-x86__64-orange?style=for-the-badge&logo=intel&logoColor=white)
![Tor](https://img.shields.io/badge/network-tor%20routed-7D4698?style=for-the-badge&logo=torproject&logoColor=white)

</div>

---

Most operating systems are built to remember. **Traceless is built to forget.**

This isn't a school project. This isn't a tutorial follow-along. This is a fully custom OS kernel written from the ground up no borrowed base, no shortcuts, no Linux underneath. Just raw C, x86 Assembly, and a vision for what a truly private, security-hardened operating system should look like.

Think Kali Linux crossed with Tails OS. Built by one person from absolute zero.

---

## Why Traceless?

Every major privacy OS out there makes you choose. You either get powerful offensive security tools, or you get privacy guarantees. Traceless refuses to make that tradeoff.

- Every packet routed through **Tor by default** — no configuration needed, no leaks possible
- **RAM wiped on shutdown** — forensics tools find nothing, ever
- **MAC address spoofed on boot** — your hardware is unidentifiable
- **Hardened kernel** — built from scratch with security as the first principle, not an afterthought
- **Full penetration testing toolkit** — armed and ready out of the box

---

## What makes this different

There are hundreds of security distros. Almost all of them are just Debian or Arch with some tools thrown on top and a custom wallpaper.

Traceless is different because the kernel itself is custom. The bootloader is custom. Every layer of the stack is written by hand with a specific purpose. When the privacy layer ships it won't be a config file on top of someone else's OS it'll be baked into the kernel at the lowest level possible.

---

## Current Status

The kernel boots. Protected mode is working. The C kernel is running on bare metal with direct VGA memory access. The foundation is solid.
[✅] Custom bootloader — Assembly, loads kernel from disk
[✅] Protected mode — 32 bit, full memory access
[✅] C kernel — running on bare metal
[✅] VGA driver — direct hardware memory access
[✅] Boot screen — because aesthetics matter
[🔨] Kernel printf — in progress
[⏳] Memory management
[⏳] Interrupt handling
[⏳] Keyboard input
[⏳] Shell
[⏳] File system
[⏳] Network stack
[⏳] Tor routing
[⏳] RAM wiper
[⏳] MAC spoofing
[⏳] Hacker toolkit
[⏳] ISO release

---

## Built With

**C** — kernel and all system logic

**x86 Assembly** — bootloader and hardware interface

**NASM** — assembler

**GCC** — compiler with no standard library

**QEMU** — virtual machine testing

**GNU ld** — custom linker with hand written linker script

---

## Building from source

```bash
git clone https://github.com/jakyunknown/TracelessOS.git
cd traceless
make run
```

Requirements: `nasm` `gcc` `gcc-multilib` `qemu-system-x86` `make`

---

## Philosophy

> Privacy is not a feature. It is the foundation.
> Security is not a layer. It is the architecture.
> Anonymity is not optional. It is the default.

---

## Contributing

This project is in early development. If you know OS development, kernel programming, or low level security and want to contribute open an issue and let's talk.

---

## Disclaimer

Traceless is built for **ethical use**, security research, and privacy protection. The author is not responsible for misuse.

---

<div align="center">

*This OS was built and developed by Judah Coskun. Started April 2026. Built in the open. One commit at a time.*

*If you believe privacy is a right and not a privilege star this repo.*

</div>
