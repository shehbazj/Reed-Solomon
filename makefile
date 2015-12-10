# Galois.tar - Fast Galois Field Arithmetic Library in C/C++
# Copright (C) 2007 James S. Plank
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
# 
# James S. Plank
# Department of Computer Science
# University of Tennessee
# Knoxville, TN 37996
# plank@cs.utk.edu

CC = gcc  
CFLAGS = -I$(HOME)/include -pg -g -lpthread

CPP = g++
CPPFLAGS = -I$(HOME)/include

ALL =	gf_mult gf_div gf_xor gf_log gf_ilog gf_basic_tester gf_inverse gf_xor_tester rs_ec

all: $(ALL)

clean:
	rm -f core *.o $(ALL) a.out

.SUFFIXES: .cpp .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c

.cpp.o:
	$(CPP) $(CPPFLAGS) -c $*.cpp

galois.o: galois.h

gf_xor_tester.o: galois.h galois.o
gf_xor_tester: gf_xor_tester.o galois.o
	$(CC) $(CFLAGS) -o gf_xor_tester gf_xor_tester.o galois.o

gf_basic_tester.o: galois.h galois.o
gf_basic_tester: gf_basic_tester.o galois.o
	$(CC) $(CFLAGS) -o gf_basic_tester gf_basic_tester.o galois.o

gf_inverse.o: galois.h galois.o
gf_inverse: gf_inverse.o galois.o
	$(CC) $(CFLAGS) -o gf_inverse gf_inverse.o galois.o

gf_ilog.o: galois.h galois.o
gf_ilog: gf_ilog.o galois.o
	$(CC) $(CFLAGS) -o gf_ilog gf_ilog.o galois.o

gf_log.o: galois.h galois.o
gf_log: gf_log.o galois.o
	$(CC) $(CFLAGS) -o gf_log gf_log.o galois.o

gf_mult.o: galois.h galois.o
gf_mult: gf_mult.o galois.o
	$(CC) $(CFLAGS) -o gf_mult gf_mult.o galois.o

gf_div.o: galois.h galois.o
gf_div: gf_div.o galois.o
	$(CC) $(CFLAGS) -o gf_div gf_div.o galois.o

gf_xor: gf_xor.o galois.o
	$(CC) $(CFLAGS) -o gf_xor gf_xor.o

rs_ec: rs_ec.o galois.o multiply.c rs_ec.h
	@echo Making $@ from $<
	$(CC) $(CFLAGS) -c multiply.c -lpthread
	$(CC) $(CFLAGS) -o rs_ec galois.o rs_ec.o multiply.o -lpthread
