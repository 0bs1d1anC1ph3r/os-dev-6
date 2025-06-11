ENTRY(_start)

SECTIONS
{
  . = 0x8000;

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
