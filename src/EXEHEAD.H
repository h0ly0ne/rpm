/*
 * Borrowed from Extended Operating System Loader
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found at http://www.gnu.org
 */

#ifndef __exehead_dos_sixteenbit__
    #define __exehead_dos_sixteenbit__

    #include "PACKB.H"
    typedef struct PACKED {
        unsigned short e_magic;         // 2 Byte   // Magic number
        unsigned short e_cblp;          // 2 Byte   // Bytes on last page of file
        unsigned short e_cp;            // 2 Byte   // Pages in file
        unsigned short e_crlc;          // 2 Byte   // Relocations
        unsigned short e_cparhdr;       // 2 Byte   // Size of header in paragraphs
        unsigned short e_minalloc;      // 2 Byte   // Minimum extra paragraphs needed
        unsigned short e_maxalloc;      // 2 Byte   // Maximum extra paragraphs needed
        unsigned short e_ss;            // 2 Byte   // Initial (relative) SS value
        unsigned short e_sp;            // 2 Byte   // Initial SP value
        unsigned short e_csum;          // 2 Byte   // Checksum
        unsigned short e_ip;            // 2 Byte   // Initial IP value
        unsigned short e_cs;            // 2 Byte   // Initial (relative) CS value
        unsigned short e_lfarlc;        // 2 Byte   // File address of relocation table
        unsigned short e_ovno;          // 2 Byte   // Overlay number
        unsigned short e_res[4];        // 8 Byte   // Reserved words [0] - [3]
        unsigned short e_oemid;         // 2 Byte   // OEM identifier (for e_oeminfo)
        unsigned short e_oeminfo;       // 2 Byte   // OEM information; e_oemid specific
        unsigned short e_res2[10];      // 20 Byte  // Reserved words [0] - [9]             // [8] = Size of last binary part   // [9] = Amount of binary parts
        long e_lfanew;					// 4 Byte   // File address of new exe header
    } exehead_dos_sixteenbit;
    #include "PACKE.H"
#endif