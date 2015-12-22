/*
# gb2312_unicode_convert.h
#
#	GB2312 to Unicode table (modified)
# from:
# http://cpansearch.perl.org/src/GUS/Unicode-UTF8simple-1.06/gb2312.txt
#
#
#	General notes:
#
#	This table contains the data Metis and Taligent currently have on how
#       GB2312-80 characters map into Unicode.
#
#	Format:  Three tab-separated columns
#		 Column #1 is the GB2312 code (in hex as 0xXXXX)
#		 Column #2 is the Unicode (in hex as 0xXXXX)
#		 Column #3 the Unicode name (follows a comment sign, '#')
#					The official names for Unicode characters U+4E00
#					to U+9FA5, inclusive, is "CJK UNIFIED IDEOGRAPH-XXXX",
#					where XXXX is the code point.  Including all these
#					names in this file increases its size substantially
#					and needlessly.  The token "<CJK>" is used for the
#					name of these characters.  If necessary, it can be
#					expanded algorithmically by a parser or editor.
#
#	The entries are in GB2312 order
#
#	The following algorithms can be used to change the hex form
#		of GB2312 to other standard forms:
#
#		To change hex to EUC form, add 0x8080
#		To change hex to kuten form, first subtract 0x2020.  Then
#			the high and low bytes correspond to the ku and ten of
#			the kuten form.  For example, 0x2121 -> 0x0101 -> 0101;
#			0x777E -> 0x575E -> 8794
#
#	Any comments or problems, contact <makerdiary@gmail.com>
#
#
#
#
*/

#ifndef _GB2312_UNICODE_CONVERT_H_
#define _GB2312_UNICODE_CONVERT_H_

unsigned short char_gb2312_to_unicode(unsigned short gbcode_euc);

int str_gb2312_to_unicode(const unsigned char *source, unsigned short *destination, int destination_size);



#endif











