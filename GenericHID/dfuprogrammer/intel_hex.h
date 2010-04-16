/*
 * dfu-programmer
 *
 * $Id: intel_hex.h 48 2007-03-28 07:35:45Z schmidtw $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */
// generichid, DIY HID device 
// Copyright (C) 2009, Frank Tkalcevic, www.franksworkshop.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#ifndef __INTEL_HEX_H__
#define __INTEL_HEX_H__

#include "inttypes.h"

/**
 *  Used to read in a file in intel hex format and return a chunck of
 *  memory containing the memory image described in the file.
 *
 *  \param filename the name of the intel hex file to process
 *  \param max_size the maximum size of the memory image in bytes
 *  \param usage[out] the amount of the available memory image used
 *
 *  \return an array of int16_t's where the values 0-255 are valid memory
 *          values, and anything else indicates an unused memory location,
 *          NULL on anything other than a success
 */
QVector<int16_t> intel_hex_file_to_buffer( const char *filename, unsigned int max_size, unsigned int *usage );
QVector<int16_t> intel_hex_to_buffer( QIODevice &file, unsigned int max_size, unsigned int *usage );
QVector<int16_t> intel_hex_string_to_buffer( const QString &s, unsigned int max_size, unsigned int *usage );
void intel_hex_string_to_buffer_append( QVector<int16_t> &memory, const QString &s, unsigned int max_size, unsigned int *usage );
void intel_hex_to_buffer_append( QVector<int16_t> &memory, QIODevice &file, unsigned int max_size, unsigned int *usage );

#endif
