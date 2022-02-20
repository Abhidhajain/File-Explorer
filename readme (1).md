rawmodecursor and moveCursor functions are used to position the cursor accordingly.
enableRawMode is used to enter into raw mode and to turn off some flags such as ECHO,OPOST,ICANON etc and disableRawMode is used to exit the raw mode and die function is used to do error handling.The VMIN value sets the minimum number of bytes of input needed before read() can return. We set it to 0 so that read() returns as soon as there is any input to be read. The VTIME value sets the maximum amount of time to wait before read() returns. 
Clear function is used to clear the stacks.
changedir function is used to change the directory and display the updated directory.
sig_handler function is used to resize the terminal.
navigate function is used to update the display length and to call the display function.
display function is used to display all the files in the directories in the requested directory.
printfiles function is used to display the file properties such as file size , ownership and permissions , last modified.
getcwd is used to get the current directory.
<sys/stat.h> is used to facilitate getting information about files attributes.
<dirent.h> is used to facilitate directory traversing.
The  <termios.h>  header contains the definitions used by the terminal I/O interfaces.
keyread function is used to read an input character .
presskey function is used to perform the required action , a switch case is implemented inside it to implement different cases of traversal(left,right,home,backspace,exit),scrolling(up,down),overflow(k,l)and opening respective files and directories and finally to enter into command mode from raw mode on pressing ':'.
getdestination function is used to get relative path from the root .
convert function is used to tokenize the string into tokens for further use in command mode.
After entering in command mode , different functionalities are performed :

create_file function is used to create multiple or single files using "open" command.
create_dir function is used to create single or multiple directories using "mkdir" command.
move function is used to move files and directories using "rename" command.
rename function is used to rename files and directories using "rename" command.
copy function is used to copy single or multiple files and directories using sstream.
goto_ function takes to the new directory given .
search function tells whether a file or directory is present or not by recursively checking in the directories.
delete_file function deletes the given file and delete_dir function deletes the given directory and all it,s contents recursively.
Escape key is used to go back to raw mode.
All the header files used are in header.h.
complete application is implemented in main.cpp.










