ENTRY(_start)

SECTIONS
{
  . = 0x9000;

  .text : {
    *(.text.entry)
    *(.text*)
  }

  .data : {
    *(.data*)
  }

  .bss : {
    *(.bss*)
    *(COMMON)
  }
}
