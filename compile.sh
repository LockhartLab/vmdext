
for fname0 in $(ls *.c)
do
  fname=$(basename $fname0 .c)
  gcc -shared -o ${fname}.so -fPIC ${fname}.c -ltcl
done



