#
# Makefile.in	-- Examples for posix extension
# 
# Copyright © 2005-2007 Erick Gallesio - I3S-CNRS/ESSI <eg@essi.fr>
# 
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, 
# USA.
# 
#           Author: Erick Gallesio [eg@essi.fr]
#    Creation date:  5-Jan-2006 19:28 (eg)
# Last file update: 12-Jun-2007 18:58 (eg)
#

BINS= fstat
SOFILE=stklos-posix.so

all: link $(BINS)

link:
	ln -s ../../../$(SOFILE) ./$(SOFILE) 

fstat: fstat.stk
	stklos-compile -o fstat fstat.stk

install:

clean: 
	rm -f *~ $(BINS) $(SOFILE)

distclean: clean

