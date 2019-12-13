# text_stretch
a utility to stretch hard-break text paragraphs into single lines

instructions:

1. put text_stretch.exe in a directory where you are going to 
   store the input and output files.
2. copy blocks of complete paragraphs into the input text file
3. run the program:  
text_stretch input_filename.txt

output will look like this:

> text_stretch test.txt  
input:  test.txt  
output: test.out  
35 lines read from input file  
8 lines written to output file, max line length: 558 bytes  

4. the corrected lines will be in input_filename.out
5. open input_filename.out in pspad
6. copy lines from input_filename.out into your word document, and proceed

You can keep both files open in your text editor.
Just be sure not to change the output file, 
doing so would open the file for writing, and text_stretch would not be able to open it itself.

