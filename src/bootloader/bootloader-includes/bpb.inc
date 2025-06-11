%ifndef _BDP_INC_HBHH78JKJG0997HHHGJK_INCLUDED
%define _BDP_INC_HBHH78JKJG0997HHHGJK_INCLUDED

        ;; Drive Parameter Table (FAT12 boot sector format)
oem_name:                   db "MSWIN4.1"
bytes_per_sector:           dw 512
sectors_per_cluster:        db 1
reserved_sectors:           dw 1
number_of_fats:             db 2
root_directory_entries:     dw 224
total_sectors:              dw 2880
media_descriptor:           db 0xF0
sectors_per_fat:            dw 9
sectors_per_track:          dw 18
num_heads:                  dw 2
hidden_sectors:             dd 0
large_total_sectors:        dd 0

    ;; Extended Boot Record
drive_number:               db 0
                            db 0 ; reserved
boot_signature:             db 0x29
volume_id:                  dd 0x12345678
volume_label:               db "BOOTDISK   "
file_system_type:           db "FAT12   "

%endif
