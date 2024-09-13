all: vdls vdget vdcount vdAdd create_vd vddelete
vddelete: vddelete.o edca.a
	gcc -o vddelete vddelete.o decodeFile.o -lm
vdls: vdls.o edca.a
	gcc -o vdls vdls.o encode.o decodeFile.o  -lm
vdget: vdget.o edca.a
	gcc -o vdget vdget.o encode.o decodeFile.o  -lm
vdcount: vdcount.o edca.a
	gcc -o vdcount vdcount.o encode.o decodeFile.o  -lm
vdAdd: vdAdd.o edca.a
	gcc -o vdAdd vdAdd.o encode.o  decodeFile.o cpy_to_fp.o -lm
create_vd: create_vd.o edca.a
	gcc -o create_vd create_vd.o encode.o  decodeFile.o -lm
vddelete.o: vddelete.c
	gcc -c vddelete.c
vdls.o: vdls.c edca.a
	gcc -c vdls.c
vdget.o: vdget.c edca.a
	gcc -c vdget.c
vdcount.o: vdcount.c edca.a
	gcc -c vdcount.c
vdAdd.o: vdAdd.c create_vd
	gcc -c vdAdd.c
create_vd.o: create_vd.c
	gcc -c create_vd.c
edca.a: encode.o  decodeFile.o cpy_to_fp.o
	ar rcs edca.a *.o
encode.o: encode.c
	gcc -c encode.c
decodeFile.o: decodeFile.c
	gcc -c decodeFile.c
cpy_to_fp.o: cpy_to_fp.c
	gcc -c cpy_to_fp.c
clean:
	rm -rf *.o *.a vdAdd vdls vdcount vdget create_vd vddelete vd
