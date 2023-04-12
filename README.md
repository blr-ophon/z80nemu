
# Z80 emulator

Zilog Z80 cpu emulator. Currently passes all tests for documented flags and instructions (zexdoc), but is still missing some I/O instructions and modes.

## Building
Clone the project
```bash
  git clone https://github.com/blr-ophon/z80nemu
```
Compile using:

```bash
  cd z80nemu
  make
```
## Running tests

Run zexdoc test with:

```bash
  make zexdoc
```
Which outputs:

```
    ./cpuZ80test ./tests/zexdoc.cim

    Z80doc instruction exerciser
    $<adc,sbc> hl,<bc,de,hl,sp>....$  OK
    $add hl,<bc,de,hl,sp>..........$  OK
    $add ix,<bc,de,ix,sp>..........$  OK
    $add iy,<bc,de,iy,sp>..........$  OK
    $aluop a,nn....................$  OK
    $aluop a,<b,c,d,e,h,l,(hl),a>..$  OK
    $aluop a,<ixh,ixl,iyh,iyl>.....$  OK
    $aluop a,(<ix,iy>+1)...........$  OK
    $bit n,(<ix,iy>+1).............$  OK
    $bit n,<b,c,d,e,h,l,(hl),a>....$  OK
    $cpd<r>........................$  OK
    $cpi<r>........................$  OK
    $<daa,cpl,scf,ccf>.............$  OK
    $<inc,dec> a...................$  OK
    $<inc,dec> b...................$  OK
    $<inc,dec> bc..................$  OK
    $<inc,dec> c...................$  OK
    $<inc,dec> d...................$  OK
    $<inc,dec> de..................$  OK
    $<inc,dec> e...................$  OK
    $<inc,dec> h...................$  OK
    $<inc,dec> hl..................$  OK
    $<inc,dec> ix..................$  OK
    $<inc,dec> iy..................$  OK
    $<inc,dec> l...................$  OK
    $<inc,dec> (hl)................$  OK
    $<inc,dec> sp..................$  OK
    $<inc,dec> (<ix,iy>+1).........$  OK
    $<inc,dec> ixh.................$  OK
    $<inc,dec> ixl.................$  OK
    $<inc,dec> iyh.................$  OK
    $<inc,dec> iyl.................$  OK
    $ld <bc,de>,(nnnn).............$  OK
    $ld hl,(nnnn)..................$  OK
    $ld sp,(nnnn)..................$  OK
    $ld <ix,iy>,(nnnn).............$  OK
    $ld (nnnn),<bc,de>.............$  OK
    $ld (nnnn),hl..................$  OK
    $ld (nnnn),sp..................$  OK
    $ld (nnnn),<ix,iy>.............$  OK
    $ld <bc,de,hl,sp>,nnnn.........$  OK
    $ld <ix,iy>,nnnn...............$  OK
    $ld a,<(bc),(de)>..............$  OK
    $ld <b,c,d,e,h,l,(hl),a>,nn....$  OK
    $ld (<ix,iy>+1),nn.............$  OK
    $ld <b,c,d,e>,(<ix,iy>+1)......$  OK
    $ld <h,l>,(<ix,iy>+1)..........$  OK
    $ld a,(<ix,iy>+1)..............$  OK
    $ld <ixh,ixl,iyh,iyl>,nn.......$  OK
    $ld <bcdehla>,<bcdehla>........$  OK
    $ld <bcdexya>,<bcdexya>........$  OK
    $ld a,(nnnn) / ld (nnnn),a.....$  OK
    $ldd<r> (1)....................$  OK
    $ldd<r> (2)....................$  OK
    $ldi<r> (1)....................$  OK
    $ldi<r> (2)....................$  OK
    $neg...........................$  OK
    $<rrd,rld>.....................$  OK
    $<rlca,rrca,rla,rra>...........$  OK
    $shf/rot (<ix,iy>+1)...........$  OK
    $shf/rot <b,c,d,e,h,l,(hl),a>..$  OK
    $<set,res> n,<bcdehl(hl)a>.....$  OK
    $<set,res> n,(<ix,iy>+1).......$  OK
    $ld (<ix,iy>+1),<b,c,d,e>......$  OK
    $ld (<ix,iy>+1),<h,l>..........$  OK
    $ld (<ix,iy>+1),a..............$  OK
    $ld (<bc,de>),a................$  OK
    Tests complete
```
