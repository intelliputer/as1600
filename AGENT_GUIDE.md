# SDK-1600 agent guide

This guide is the fast entry point for an agent working on CP-1600 and
Intellivision software in this repository. It summarizes the constraints that
matter when planning, generating, building, or reviewing a change. The linked
documents remain the source of truth for low-level details.

## Platform model

- The Intellivision uses General Instrument's **CP-1610**, a member of the
  16-bit CP-1600 family.
- Programs execute in a memory-mapped machine with ROM, system RAM,
  scratchpad RAM, the STIC video chip, PSG sound chip, GROM, and optional
  peripherals such as the Intellivoice and ECS.
- The processor bus, STIC display timing, and interrupts are externally
  observable. Code that works logically can still fail if it accesses video
  memory at the wrong time or violates memory-map attributes.
- Cartridge images use either a paired **BIN+CFG** representation or a single
  Intellicart **`.rom`** image. The image format includes mapping and access
  attributes, not only program bytes.

Read these references before making low-level changes:

- [CP-1610 reference](doc/programming/cp_1610.md)
- [Memory map](doc/programming/memory_map.md)
- [Interrupts and timing](doc/programming/interrupts.md)
- [STIC reference](doc/programming/stic.md)
- [GROM and GRAM graphics memory](doc/programming/graphics_mem.md)
- [Intellicart programming](doc/programming/intellicart.md)
- [Intellicart ROM metadata tag format](doc/rom_format/id_tag.txt)
- [Technical hardware reference](doc/tech/index.md)

## Repository map

| Location | Use it for |
| --- | --- |
| `bin/` | Native Linux SDK tools, including `as1600`, `dasm1600`, `bin2rom`, and `rom2bin` |
| `examples/` | Small, working reference programs and reusable library routines |
| `examples/library/` | Shared assembly includes such as `gimini.asm`, `print.asm`, and `fillmem.asm` |
| `src/` | Toolchain source and its native Linux build |
| `doc/programming/` | CPU, memory-map, STIC, timing, PSG, and cartridge details |
| `doc/rom_format/` | Intellicart manual and ROM metadata-tag specification |
| `doc/utilities/` | Tool-specific syntax and behavior |
| `doc/tech/` | Detailed hardware, ECS, Intellivoice, keyboard, and SP-0256 material |
| `doc/voice/` | Primary SP0256-AL2 reference and editable speech-filter diagram source |

## Validated build workflow

Build the SDK tools from the repository root:

```sh
make -C src -j32
```

The build writes native executables to `bin/`. `as1600` is built from the
Mira-derived source closure in `src/modern-as1600/` and supports the modern
example set, including metadata, macro, JLP, and CP-1600X features.

Assemble an example from its own directory so its relative include paths work:

```sh
cd examples/hello
../../bin/as1600 -o hello.bin -l hello.lst hello.asm
../../bin/bin2rom hello.bin
../../bin/rom2bin hello.rom
```

Successful assembly reports zero errors and zero warnings. A useful smoke test
is a lossless binary-text-binary conversion:

```sh
../../bin/tohex hello.bin > hello.hex
../../bin/fromhex hello.hex > hello.roundtrip.bin
cmp hello.bin hello.roundtrip.bin
```

Do not write generated ROMs, listings, object files, or temporary output back
into source example directories during validation. Use a temporary copy of the
example tree when practical.

## Assembly and image rules

- Use `ROMW` to declare the intended ROM width; the standard Intellivision
  examples typically use a 10-bit ROM width.
- Use `ORG` to place content deliberately. ROM location, width, and access
  attributes are part of the program's behavior.
- Use `MEMATTR` only when normal `ORG` and reservation directives cannot
  express the desired Intellicart attributes. It can mark readable, writable,
  narrow, or bank-switched ranges.
- Keep `INCLUDE` paths explicit. Examples assume their current directory and
  `examples/library/` retain the repository layout. For another layout, use
  `as1600 -i <directory>` or `AS1600_PATH`.
- Use `bin2rom` and `rom2bin` for format conversion rather than constructing
  Intellicart headers manually.
- Keep generated `*.bin`, `*.cfg`, `*.rom`, and `*.lst` files out of commits
  unless an artifact is explicitly intended for distribution.

See the [AS1600 addendum](doc/utilities/as1600.md) for directive details.

## Hardware-sensitive constraints

1. **Display timing matters.** GROM and GRAM access is constrained by display
   activity. Consult the graphics-memory and interrupt references before
   scheduling writes.
2. **Interrupt code is timing-sensitive.** Preserve register, stack, and
   interrupt behavior when modifying interrupt handlers or EXEC-facing code.
3. **Address-space overlap is normal.** Peripherals and ROM ranges can overlap
   or be selectively decoded. Validate against the memory map rather than
   assuming a flat address space.
4. **Bank switching changes image semantics.** A byte-identical image with
   different `.cfg` or ROM attributes may be a different program.
5. **Hardware utilities are not ordinary tests.** `ec_*`, `test_cart`, and
   `test_hcif` interact with physical hardware or I/O permissions. Do not run
   them unless the necessary device and explicit authorization are available.

## Productive agent loop

1. Start from the closest example and identify its required library includes.
2. Read the relevant memory, STIC, interrupt, and cartridge references before
   changing code that touches hardware-visible state.
3. Make the smallest assembly or toolchain change that meets the goal.
4. Build the native tools if the change affects `src/`.
5. Assemble a representative example in a temporary workspace.
6. Validate output format conversion and, when relevant, disassemble the
   result for inspection.
7. State clearly what was verified in software and what still requires an
   emulator or physical Intellivision hardware.

## Boundaries

- This repository can validate assembly, conversion, and inspection tools. It
  cannot by itself prove video timing or peripheral behavior on real hardware.
- EXEC, GROM, and game ROM images may be copyrighted or unavailable. Do not
  invent, download, or redistribute them without the required rights.
- Historical documents may include obsolete external URLs or platform-specific
  instructions. Prefer repository-local Markdown references and the current
  Linux build workflow.
