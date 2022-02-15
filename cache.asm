;; defining constants, you can use these as immediate values in your code
CACHE_LINES  EQU 100
CACHE_LINE_SIZE EQU 8
OFFSET_BITS  EQU 3
TAG_BITS EQU 29 ; 32 - OFSSET_BITS

section .text
    global load

;; void load(char* reg, char** tags, char cache[CACHE_LINES][CACHE_LINE_SIZE], char* address, int to_replace);
load:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     eax, [ebp + 8]  ; address of reg
    mov     ebx, [ebp + 12] ; tags
    mov     ecx, [ebp + 16] ; cache
    mov     edx, [ebp + 20] ; address
    mov     edi, [ebp + 24] ; to_replace (index of the cache line that needs to be replaced in case of a cache MISS)
    ;; DO NOT MODIFY

    ;; TODO: Implement load
    ;; FREESTYLE STARTS HERE
    
    mov     ebx, dword 7    ; 0x...111
    mov     eax, edx        ; address
    and     eax, ebx        ; get last 3 bits of address -> it is offset value
    push    eax            ; pushed offset value

    shr     edx, OFFSET_BITS ; obtain tag

    mov     ecx, 0          ; increment contor
    mov     ebx, [ebp + 12] ; put the tags address back
iterate_here:
    lea     ebx, [ebx + 4 * ecx]    ; get address of tags[i]
    mov     ebx, [ebx]          ; get the value from tags[i]
    cmp     ebx, edx            ; compare tag with address
    je      found_tag            ; if equal go to found_tag
    mov     ebx, [ebp + 12]     ; put tag address back  
    inc     ecx                 ; increment contor 
    cmp     ecx, CACHE_LINES    ; compare with cache_lines
    jl      iterate_here         ; if less - go next
    jmp     put_in_cache_and_tags
found_tag:
    mov     eax, [ebp + 16]     ; get cache address
    lea     eax, [eax + ecx * CACHE_LINE_SIZE]    ; get cache[i] address
    pop     ebx                 ; get offset value
    lea     eax, [eax + ebx]    ; get address of cache[i][offset]
    mov     eax, [eax]          ; get value from cache[i][offset]
    mov     ebx, [ebp + 8]      ; get register address
    mov     dword [ebx], eax    ; put the value at register address
    jmp     exit_cache

put_in_cache_and_tags:
    mov     ebx, [ebp + 12]     ; get tag address
    lea     ebx, [ebx + 4 * edi]; get tags[to_replace] address
    mov     dword [ebx], edx    ; put the new tag there
    mov     ecx, [ebp + 16]     ; get cache address
    lea     ecx, [ecx + CACHE_LINE_SIZE * edi] ; cache[to_replace] address
    shl     edx, 3      ;   obtain initial address from tag 0xABCD...000
    mov     edi, 0
iterate_cache_column:
    mov     bl, [edx + edi]       ;   get value from that address
    mov     byte [ecx + edi], bl  ;   put value in cache
    inc     edi
    cmp     edi, CACHE_LINE_SIZE
    jl      iterate_cache_column

    mov     edi, [ebp + 24] ; to_replace
    mov     ecx, [ebp + 16] ; cache
    pop     eax             ; get offset value
    lea     ecx, [ecx + edi * CACHE_LINE_SIZE]  
    lea     ecx, [ecx + eax]    ; get cache[to_replace][offset] address
    mov     cl, [ecx]           ; save in cl cache[to_replace][offset] value
    mov     eax, [ebp + 8] ; get address of reg
    mov     byte [eax], cl ; write there value from cache


exit_cache:
    ;; FREESTYLE ENDS HERE
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY