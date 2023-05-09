;
; Borrowed from Extended Operating System Loader
; Copyright (c) 1999 by Geurt Vos
;
; This code is distributed under GNU General Public License (GPL)
;
; The full text of the license can be found at http://www.gnu.org
;

                .model  compact
                .386p
                .code
                public  _create, _open, _close, _read, _write, _chmod, _getSize

;int create(const char *path);
_create         proc
                push    bp
                mov     bp,sp
                push    ds
                mov     ah,3ch
                xor     cx,cx
                lds     dx,[bp + 4]
                int     21h
                jnc     CreateOK
                mov     ax,-1
CreateOK:       pop     ds
                pop     bp
                ret
_create         endp

;int open(const char *path, int access);
_open           proc
                push    bp
                mov     bp,sp
                push    ds
                mov     ah,3dh
                mov     al,[bp + 8]
                lds     dx,[bp + 4]
                xor     cx,cx
                int     21h
                jnc     OpenOK
                mov     ax,-1
OpenOK:         pop     ds
                pop     bp
                ret
_open           endp

;void close(int handle);
_close          proc
                push    bp
                mov     bp,sp
                mov     ah,3eh
                mov     bx,[bp + 4]
                int     21h
                pop     bp
                ret
_close          endp

;unsigned long getSize(int handle);
_getSize        proc
                push    bp
                mov     bp,sp
                mov     ax,4202h
                mov     bx,[bp + 4]
                xor     cx, cx
                xor     dx, dx
                int     21h
                jc      getSizeKO
                push    dx
                push    ax
                mov     ax,4200h
                mov     bx,[bp + 4]
                xor     cx, cx
                xor     dx, dx
                int     21h
                pop     ax
                pop     dx
                jnc     getSizeOK
getSizeKO:      
                xor     ax, ax
                xor     dx, dx
getSizeOK:
                pop     bp
                ret
_getSize        endp

;unsigned short read(int handle, void *buf, unsigned short len);
_read           proc
                push    bp
                mov     bp,sp
                push    ds
                mov     ah,3fh
                mov     bx,[bp + 4]
                mov     cx,[bp + 10]
                lds     dx,[bp + 6]
                int     21h
                jnc     ReadOK
                xor     ax,ax
ReadOK:         pop     ds
                pop     bp
                ret
_read           endp

;unsigned short write(int handle, void *buf, unsigned short len);
_write          proc
                push    bp
                mov     bp,sp
                push    ds
                mov     ah,40h
                mov     bx,[bp + 4]
                mov     cx,[bp + 10]
                lds     dx,[bp + 6]
                int     21h
                jnc     WriteOK
                xor     ax,ax
WriteOK:        pop     ds
                pop     bp
                ret
_write          endp

;int chmod(const char *path, int amode)
_chmod          proc
                push    bp
                mov     bp,sp
                push    ds
                mov     ax,4301h
                mov     cx,[bp + 8]
                lds     dx,[bp + 4]
                int     21h
                sbb     ax,ax
                pop     ds
                pop     bp
                ret
_chmod          endp

                end