;segdes dummy_descriptor,0xffff,0,0,0x9a,0xcf,0

struc segment_descriptor
  seg_length0_15        resw 0  ; low word of the segment length
  base_addr0_15         resw 0  ; low word of base address
  base_addr16_23        resb 0  ; low byte of high word of base addr.
  .flags                resb 0  ; segment type and misc. flags
  access                resb 0  ; highest nibble of segment length
                                ; and access flags
  base_addr24_31        resb 0  ; highest byte of base address
endstruc

struc interrupt_descriptor
  offset0_15            resw 0      ; low word of handler offset
  selector0_15          resw 0      ; segment selector
  zero_byte             resb 0      ; unused in this descriptor format
  .flags                resb 0      ; flag-byte
  offset16_31           resw 0      ; high-word of handler offset
endstruc
