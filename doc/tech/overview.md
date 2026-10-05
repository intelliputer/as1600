# Intellivision Overview

This is just a quick overview of what the Intellivision is. It's not nearly as comprehensive as, say, De Re Intellivision or the historical information available on the ***Intellivision Lives!*** CD-ROM.

For a more comprehensive history, please find a copy of De Re Intellivison, go buy a copy of ***[Intellivision Lives!](http://www.intellivisionlives.com/)***, or visit the [The Blue Sky Rangers](http://www.makingit.com/bluesky/) homepage.

## The Quick and Dirty History of Intellivision

This is the short version. The Intellivision was the world's first 16-bit home video game system. It was introduced in the early 80s and it competed against the Atari 2600, and later the Colecovision. It was produced by Mattel Electronics until early 1984, when Mattel Electronics was dissolved.

As Mattel Electronics was being dissolved, a new, independent company was formed, Intellivision, Inc., which purchased the rights to the Intellivision name. It specialized in selling Mattel's remaining inventory. It also began introducing games that were complete but not released at the time Mattel closed.

Later, Intellivision, Inc. became INTV, Inc. INTV introduced more Intellivision games and even new Master Components. INTV kept the Intellivision name alive until it folded in 1990.

## What's in the Intellivision?

The Intellivision Master Component consists of the following pieces, most of which are from General Instruments:

- CP-1610 CPU. This is a 16-bit microprocessor whose instruction set is reminiscent of the PDP-8, so I'm told.
- AY-3-8910 Programmable Sound Generator. Arguably, this sound chip is the most popular sound chip of the early 80s.
- AY-3-8900 Standard Television Interface Chip. This puts our favorite Running Man on the screen.
- Two controller pads
- Various RAM and ROM, described in the table below.

| Type of Memory | Number of Words / Locations | Width of Memory (bits) | Part number                           | Address Range |        | Name            | Description                                                                                                                                                                                                                                                                                                                                                                       |
|----------------|-----------------------------|------------------------|---------------------------------------|---------------|--------|-----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| RAM            | 240                         | 8                      | GTE-3539                              | \$0100        | \$01EF | Scratchpad RAM. | This RAM is used by the EXEC and game programs to store variables, etc. Note that the actual RAM chip is a 256-byte chip, but locations \$1F0..\$1FF are overlapped by the Programmable Sound Generator.                                                                                                                                                                          |
| RAM            | 352                         | 16                     | RA-3-9600                             | \$0200        | \$035F | System RAM.     | This RAM contains the display memory, stack, and 16-bit memory that is used by the EXEC and game programs for variables. The System RAM also incorporates a special interface for the STIC which isolates the STIC's memory bus from the CPU bus during active display while the display is being generated, and also allows the CPU to access the STIC during vertical blanking. |
| ROM            | 4096                        | 10                     | Intellivision 1: RO-3-9502, RO-3-9504 | \$1000        | \$1FFF | EXEC ROM.       | The Intellivision "OS" lives here. On the Intellivision 1, this is held in two 2K ROMs. On the Intellivision 2, this is a single 4.25K ROM with an extra, undocumented segment at \$0400-\$04FF.                                                                                                                                                                                  |
|                | 4352                        |                        |                                       |               |        |                 |                                                                                                                                                                                                                                                                                                                                                                                   |
|                |                             |                        | Intellivision 2: RO-3-9506            | \$0400        | \$04FF |                 |                                                                                                                                                                                                                                                                                                                                                                                   |
| ROM            | 2048                        | 8                      | RO-3-9503                             | \$3000        | \$37FF | Graphics ROM.   | This holds the fixed character images that the Intellivision can display. It also holds a small amount of EXEC code that didn't fit in the EXEC ROM.                                                                                                                                                                                                                              |
| RAM            | 512                         | 8                      | two GTE-3539's                        | \$3800        | \$39FF | Graphics RAM.   | This holds the cartridge-defined images for the Intellivision to display. Up to 64 unique 8x8 blocks can be stored here. Note that locations \$3A00-\$3FFF apparently contain read aliases of \$3800-\$39FF, but none of these alias locations is writable. Weird.                                                                                                                |

RAM and ROM in the Intellivision Master Component
