# Intellivoice Hardware Overview

The Intellivoice (Model 3330) adds speech synthesis to an Intellivision Master
Component or Keyboard Component. Voice-compatible cartridges use the module's
speech facilities; ordinary cartridges continue to operate normally, without
speech enhancement. The module mixes speech into the console's normal audio
path, and its volume control affects speech only.

## Architecture

The module combines four functions:

- **SP-0256 Orator speech synthesizer.** Its programmable digital filter,
  resident speech ROM (RESROM), microcontroller, and PWM output produce
  speech. It can also accept serial speech data supplied by a cartridge. See
  the [SP-0256 instruction set](sp0256_instr_set.md) for the processor-level
  reference.
- **SPB-640 buffer/interface.** This device connects the cartridge bus to the
  synthesizer. It can trigger RESROM phrases, serialize custom speech data
  through its 640-bit FIFO, and communicate with peripherals on the stacking
  connector.
- **Audio filter and amplifier.** The SP-0256 emits a 40 kHz PWM signal. The
  module filters and amplifies it before mixing it into the console audio;
  the effective speech passband is approximately 150 Hz to 5 kHz.
- **Expansion power path.** The stacking connector permits a future peripheral
  to supplement the console power supply. This is hardware information, not a
  recommended modification procedure.

## SDK use

Use [`examples/library/ivoice.asm`](../../examples/library/ivoice.asm) for
RESROM playback and queued FIFO speech.
[`examples/library/resrom.asm`](../../examples/library/resrom.asm) defines the
resident phrase indices, while
[`examples/library/al2.asm`](../../examples/library/al2.asm) contains the
separately licensed SP0256-AL2 allophone data.

## Sources

This overview is derived from the Intellivoice Model 3330 Product Engineering
Specification (Thomas L. Randolph, 18 March 1982; revised 6 May 1982), the
General Instruments Orator specification, and the Intellivoice Service Manual.
