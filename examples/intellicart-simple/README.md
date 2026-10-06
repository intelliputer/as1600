# Historic Intellicart bank-switch test

`simple.asm` is an early, self-contained test of Intellicart bank-switched
memory. It is retained as a historical hardware-reference program and a
modern-AS1600 regression fixture.

For new programs, use `../banktest/` and the routines in
`../library/ic_banksw.asm`; they provide the maintained bank-switching API and
broader test coverage.

Build from this directory:

```sh
../../bin/as1600 -o simple.bin -l simple.lst simple.asm
```

The resulting BIN+CFG pair is intended for an Intellicart-compatible emulator
or hardware. This source remains GPL-2.0-or-later under its original notice.
