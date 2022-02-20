#include "header.h"
int start=0,indexx=0;
string home,presentdir;
vector<string>files;
vector <vector<string>> DFlist;
vector<int>ford;
//vector<vector<int>>permission;
stack<string>leftstack;
stack<string>rightstack;
unordered_map<string,int>destlist;
int lenofdir,row_len;
int cursor_pos,mode=1;
int x=1,y=0,flag;
int endd,current_index;
struct winsize size;
void display(int start,int endd);
void printfiles(string file);
void navigate();
void moveCursor(int x,int y) {
    cout<<"\033["<<x<<";"<<y<<"H";
    fflush(stdout);
}
void rawmodecursor(){
    moveCursor(1,0);
        x=1;
        y=0;

}
void deld(string s);
void die(const char *s) {
  perror(s);
  exit(1);
}
struct termios orig_termios;
void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}
void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}
void Clear(stack <string> &S){
  while(!S.empty())
    S.pop();
}
void changedir(string s,int &start,int &endd){
    DIR *dir;
    dirent *dp;
    char newdest[100];
    strcpy(newdest, s.c_str());
    char tep[256];
    getcwd(tep,256);
    presentdir=tep;
            dir=opendir(newdest);
            vector <string> temp;
            //vector <int> tem;
            temp.push_back(".");
            //tem.push_back(1);
            temp.push_back("..");
            //tem.push_back(1);
            while((dp=readdir(dir))!=NULL){
                if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
                {
                    temp.push_back(string(dp->d_name));
                    
                }
            }
            
            DFlist.push_back(temp);
            //permission.push_back(tem);
            destlist[s]=indexx;
            current_index=indexx;
            indexx++;
            lenofdir=DFlist[current_index].size();
            start=0;
            row_len=size.ws_row-4;
            cursor_pos=(lenofdir > row_len) ? row_len : lenofdir;
            endd=cursor_pos;
            cursor_pos=0;
            presentdir=s;
            //leftstack.push(presentdir);
            chdir(presentdir.c_str());
            //cout<<"\033[2J\033[1;1H";
            temp.clear();
            //tem.clear();
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            display(start,endd);
            rawmodecursor();



}
string getdestination(string dest){
    //cout<<"in get dest...\n";
    string destination="";
    char tp[256];
    getcwd(tp,256);
    if(dest[0]=='/' && dest[1]=='h' && dest[2]=='o' && dest[3]=='m' && dest[4]=='e'){
        destination=dest;
    }
    else if(dest[0]=='~'){
        destination=home+'/'+dest.substr(1,dest.length());
    }
    else if(dest[0]=='/'){
        destination=home+'/'+dest;   
    }
    else if(dest[0]=='.'&&dest[1]=='.'){
        destination=tp+'/'+dest.substr(2,dest.length());
    }
    else{
        destination=tp+'/'+dest.substr(1,dest.length());
    }
    return destination;
}
void convert(string command,vector<string>&cmnd){
    command+=" ";
    string s="";
    for(int i=0;i<=command.length();i++){
        if(command[i]!=' '){
            s+=command[i];
        }
        else{
            cmnd.push_back(s);
            s="";
        }
    }
    

}
void create_file(vector<string>cmnd){
    int l=cmnd.size();
    string dest=getdestination(cmnd[l-1]);
    for(int i=1;i<l-1;i++){
        string res=dest+'/'+cmnd[i];
        //write(STDOUT_FILENO, res.c_str(), 100);
    //printf("\n%s",res);
        // fstream file;
        // file.open(res,ios::out);
        char path[100];
		strcpy(path, res.c_str());
		int fd=open(path , O_RDONLY | O_CREAT| O_WRONLY);
        if(fd==-1)
        {
            string b="can't create this file!!!";
            write(STDOUT_FILENO, b.c_str(),b.length());
            
        }
        else{
            string c="File successfully created ";
  	         write(STDOUT_FILENO, c.c_str(),c.length());

        }
        close(fd);
		chmod(path,S_IRUSR|S_IWUSR);
        chdir(home.c_str());
    }

}
void create_dir(vector<string>cmnd){
    int l=cmnd.size();
    string dest=getdestination(cmnd[l-1]);
    for(int i=1;i<l-1;i++){
        string res=dest+'/'+cmnd[i];
        
		if (mkdir(res.c_str(),S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP |S_IROTH|S_IXUSR)==-1){
            string b=" can't create this/these directory/directories!!!";
            write(STDOUT_FILENO, b.c_str(),b.length());
            return;
        }
        }
        int o=l-2;
        string c=" ";
        c+=to_string(o);;
        c+=" Directory/Directories successfully created ";
  	    write(STDOUT_FILENO, c.c_str(),c.length());
        
        chdir(home.c_str());
    }

void move(vector<string>cmnd){
    int l=cmnd.size();
    string dest=getdestination(cmnd[l-1]);
    for(int i=1;i<l-1;i++){
        string old=presentdir+'/'+cmnd[i];
        string newt=dest+'/'+cmnd[i];
        if (rename(old.c_str(), newt.c_str()) != 0){
            string b=" Error moving file/directory";
            write(STDOUT_FILENO, b.c_str(),b.length());}
	    else{
            string c=" file/directory successfully moved";
            write(STDOUT_FILENO, c.c_str(),c.length());
        }
		    

    }
}
void rename(vector<string>cmnd){
    string old=cmnd[1];
    string newt=cmnd[2];
        if (rename(old.c_str(), newt.c_str()) != 0){
            string b=" Error renaming file/directory";
            write(STDOUT_FILENO, b.c_str(),b.length());}
	    else{
            string c=" file/directory successfully renamed";
            write(STDOUT_FILENO, c.c_str(),c.length());
        }

}
void copy(vector<string>cmnd){
    int l=cmnd.size();
    fstream fold;
    string temp="";
    
    
    //string dest=getdestination(cmnd[l-1]);
    for(int i=1;i<l-1;i++){
        string src=presentdir+'/'+cmnd[i];
        string dest=getdestination(cmnd[l-1])+'/'+cmnd[i];
        fold.open(dest.c_str(),ios::out);
        ifstream inp {src};
        ofstream outp {dest};
        //outp << inp.rdbuf();}
        if(inp && outp){
            while(getline(inp,temp)){
            outp << temp << "\n";
        }
            string c=" files/directories copied";
            write(STDOUT_FILENO, c.c_str(),c.length());

        }
        else{
            string d="files/directories not copied";
            write(STDOUT_FILENO, d.c_str(),d.length());

        }
        inp.close();
        outp.close(); 

    }   

}
void goto_(vector<string>cmnd,int &start, int &endd){
    string dest=getdestination(cmnd[1]);
	if(chdir(dest.c_str())<0){
		string t="error";
        write(STDOUT_FILENO, t.c_str(),t.length());
	}
    leftstack.push(presentdir);
	changedir(dest,start,endd);
	Clear(rightstack);
    presentdir=dest;
    			
}
bool search(string predir,string s){
    //string s=cmnd[1];
    struct dirent *dp;
    //char tp[256];
    //getcwd(tp,256);
    //string f=presentdir+'/'+s;
    DIR *dir = opendir(predir.c_str());
    if (dir == NULL) {
        return false;

    }
    dp=readdir(dir);
    while(dp!=NULL){
        if(dp->d_name==s){
            
            return true;

        }
        if (dp->d_type == DT_DIR && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            string w=predir;
            w+='/';
            w+=dp->d_name;
            return search(w,s) ;
        }
        
        dp = readdir(dir);

    }
    closedir(dir);
    return false;

}

void delete_file(string path,int &start,int &endd)
{
    int fd=remove(path.c_str());
        if(fd==-1){
            cout<<" error in deleting file "<<endl;
            exit(0);
        }
        else{
            string h=" file deleted";
            write(STDOUT_FILENO, h.c_str(),h.length());
            endd--;

        }
}
void delf(string s,int &start,int &endd){
    string dest=getdestination(s);
    delete_file(dest,start,endd);
}

void delete_dir(string path){
    if(path==presentdir){
        exit(0);
    }
    
    DIR *dir = opendir(path.c_str());
    if (dir)
    {
        struct dirent *dp;
        while ((dp = readdir(dir)) != NULL)
        {
            
            if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
                continue;
            else
            {
                string pathm = path + "/" + dp->d_name;
               
                struct stat st;

                if (stat(pathm.c_str(), &st) == -1)
                {
                    string h="error opening directory";
                    write(STDOUT_FILENO, h.c_str(),h.length());
                    return;
                }
                if (S_ISDIR(st.st_mode))
                {   
                    deld(pathm);
                }
                else
                {
                    delf(pathm,start,endd);
                }
            }
        }
        closedir(dir);
        int val = rmdir(path.c_str());
        if (val == -1)
        {
            string h=" error deleting directory";
            write(STDOUT_FILENO, h.c_str(),h.length());
        }
        else{
            string h=" directory deleted";
            write(STDOUT_FILENO, h.c_str(),h.length());

        }
    }
    else{
        string h=" error opening dir";
        write(STDOUT_FILENO, h.c_str(),h.length());
    }
        
    }
    void deld(string s){
        string dest=getdestination(s);
        delete_dir(dest);
}

   


    char keyread() {
        int nread;
        char c;
        while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
            if (nread == -1 && errno != EAGAIN) die("read");
        }
        return c;
    }

    void presskey() {
    if(mode==1){
        inplace:
    char c = keyread();
    switch (c) {
    case ('q'):
        label:
        exit(0);
        break;
    case ('A'):
        if(x>0){
            x--;
            moveCursor(x,y);
            cursor_pos--;
        } 
        break;   
    case ('B'):
        if(x<endd){
            flag=0;
            x++;
            moveCursor(x,y);
            cursor_pos++;
            //cout<<cursor_pos;
        }   
        break;
    
    case ('k'):
        if(cursor_pos > start){
      cursor_pos--;
      printf("\033[1A");
        }
    else if(cursor_pos == start && start > 0){
      flag=0;
      start--;
      cursor_pos--;
      endd--;
            //changedir(presentdir,start,endd);
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        display(start,endd);
        rawmodecursor();
      int temp=row_len-1;
      while(temp--){
      cout<<"\033[1A";  
          }           
        }
        break;
    case ('l'):
        if(cursor_pos<endd-1){
            cursor_pos++;
        } 
        else if(cursor_pos==endd-1 && endd<lenofdir){
            flag=1;
            int gh=cursor_pos;
            endd++;
            cursor_pos++;
            start++;
            
            //x++;
            //changedir(presentdir,start,endd);
            moveCursor(gh,0);
            //cout<<"\033[1A";
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            display(start,endd);
            
            //moveCursor(endd-1,0);

            //rawmodecursor();
      //cout<<"\033[1A";
            } 
        break;          
    
    case (13):
        {int val;
        
            val=cursor_pos;
        
        struct stat sb;
            char curr_dest[200];
            //string dest=presentdir+'/'+dp->d_name;
            string dest=DFlist[indexx-1][val];
            strcpy(curr_dest, dest.c_str());
            if (stat(curr_dest, &sb) == -1) {
                perror("stat");
                exit(EXIT_FAILURE);
            }
            switch (sb.st_mode & S_IFMT) {
                case S_IFDIR : 
                     {
                    string s=DFlist[indexx-1][val];
                    leftstack.push(presentdir);
                    string st=presentdir+'/'+s;
                    changedir(st,start,endd);
            
                    break;}
                    
                break;
                case S_IFREG : 
                    {char *args[3];
                    string vim="vi";
            //cout<<vim;
                   args[0]=(char*)vim.c_str();
                   string of=DFlist[indexx-1][val];
            
                    args[1]=(char*)of.c_str();
                    args[2]=NULL;
                    pid_t pid=fork();
                    if(pid == 0)
                    {
                        if(execvp(args[0],args)== -1)
                        {
                            perror("exec");
                        }
            exit(0);
            }
            if(pid>0)
            {
            wait(0);
            

        }
        //if(files[val]==2){
           
        //}
        //else if(permission[indexx-1][val]==1){
            
            
            
        //}
        break;}
        break;}}
    case('C'):                            //RIGHT
        flag=0;
    if(!rightstack.empty()){
        leftstack.push(presentdir);
        string newdest=rightstack.top();
        rightstack.pop();
        presentdir=newdest;
        changedir(newdest, start, endd);
        }
        break; 
            
    case('D'):                    //LEFT
        flag=0;
        if(!leftstack.empty()){
            rightstack.push(presentdir);
            string newfolder=leftstack.top();
            leftstack.pop();
            presentdir=newfolder;
            changedir(newfolder, start, endd);}
            break;    
        
    case('h'):
        flag=0;
        if(presentdir!=home){
            Clear(rightstack);
            //string newdest=presentdir;
            leftstack.push(presentdir);
            changedir(home, start, endd);
            //chdir(home.c_str());
            presentdir=home;
        }
        
    break;
    

    case(':'):
        
        {ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		int movedown=size.ws_row-2, cursor=cursor_pos;
		int temp=cursor;
		while(cursor<movedown){
			printf("\033[1B");
			cursor++;	
		}
        moveCursor(cursor,0);
        //printf(":");
        string str="Command Mode:";
        write(STDOUT_FILENO, str.c_str(), str.length());
        mode=2;					
		string command="";
		char ch;
        while(1){
            char ch=keyread();
            if(ch==27)
               {mode=1;
                char temp[256];
                getcwd(temp,256);
                write(STDOUT_FILENO, "\x1b[2J", 4);
                write(STDOUT_FILENO, "\x1b[H", 3);
                changedir(temp,start,endd);
                //display(start,endd);
                rawmodecursor();
				goto inplace;
				}					
				else if(ch==127){
					string temp="\b\033[J";
                    write(STDOUT_FILENO, temp.c_str(), 1);			
					command.pop_back();	
				}
                else if(ch=='q'){
                    // mode=1;
                    // goto inplace;
                    exit(0);
                }
				else if(ch!=27 && ch!=13){
					//prinf("%c",&h);
                    string str="";
                    str+=ch;
                    write(STDOUT_FILENO, str.c_str(), 1);
					command.push_back(ch);	
				}
                
                else if(ch==13){
                    vector <string> cmnd;
                    cmnd.clear();
	                convert(command,cmnd); 
	                
                    if(cmnd[0]=="create_file"){
		                create_file(cmnd);
                        //display(start,endd);
		                
	                }
                    else if(cmnd[0]=="create_dir"){
		                create_dir(cmnd);
		                
	                }
                    else if(cmnd[0]=="move"){
		                move(cmnd);
		                
	                }
                    else if(cmnd[0]=="rename"){
		                rename(cmnd);}
                    else if(cmnd[0]=="copy"){
		                copy(cmnd);
	                }
                    else if(cmnd[0]=="goto"){
		                goto_(cmnd, start, endd);
		                
	                }
                    else if(cmnd[0]=="search"){
		                if(search(presentdir,cmnd[1])){
                            string t=" true";
                            write(STDOUT_FILENO, t.c_str(),t.length());

                        }
                        else{
                            string e=" false";
                            write(STDOUT_FILENO, e.c_str(),e.length());

                        }
		                
	                }
	                else if(cmnd[0]=="delete_file"){
		                delf(cmnd[1],start,endd);
                        //display(start,--endd);
		                
	                }
	                else if(cmnd[0]=="delete_dir"){
		                deld(cmnd[1]);
		                
	                }
	                
	               
		                
	                // }
	                
                    if(cmnd[0]=="goto"){
                        mode=1;
                        goto inplace;
                    }
                    else{
                    int len=command.length();
				    //while(len--){
					    string tem="\n";
                        write(STDOUT_FILENO, tem.c_str(), tem.length());
                        moveCursor(++cursor,0);
				    //}
				    command="";	}

                }
        }
        }
        break;

    case(127):
        {string s="";
        s+="..";
        changedir(s,start,endd);    
        rightstack.push(s);
        presentdir=s;
        break;      
        
            
                 
    }       
}
}}

void sig_handler(int sig)
{
    if(SIGWINCH==sig)
    {
       navigate();
       moveCursor(1,0);
    }

}
int main(){
    //gotoxy(0, 0);
    signal(SIGWINCH, sig_handler);
    enableRawMode();
    //while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
    
   

    //moveCursor(1,1);
    
    char temp[256];
    getcwd(temp,256);
    home=string(temp);
    //home=string(temp);
    //home+='/';
    presentdir=home;
    char dest[1000];
    struct dirent *dp;
    DIR *dir = opendir(temp);
    

    
    if (!dir)
        return 0;
    files.push_back(".");
    //ford.push_back(1);
    
    files.push_back("..");
    //ford.push_back(1);
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            
            files.push_back(string(dp->d_name));
            
    }
            

            

            //ford.push_back

        
    }
    presentdir=home;
    chdir(presentdir.c_str());
    DFlist.push_back(files);
    //permission.push_back(ford);

    if(destlist.find(home)==destlist.end()){
        destlist[home]==indexx;
        indexx++;
    }
    current_index=indexx;
    lenofdir=files.size();
    navigate();
    while (1) {
        presskey();
    }
}
    
void navigate(){
        
  int flag=0;
  char key;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
  row_len=size.ws_row-4;
  if(lenofdir> row_len){
        cursor_pos=row_len;
     }
    else{
        cursor_pos=lenofdir;
     }
  endd=cursor_pos;
  cursor_pos=0;
  cout<<"\033[2J\033[1;1H";
  display(start, endd);
  moveCursor(1,0);
    
  //cout << "\033[H";

}
void printfiles(string file){
    struct group *g;
    struct passwd *pw;
    struct stat obj;
    register uid_t id1;
    register gid_t id2;
    double tot;
    //struct stat obj;
    char temp[256];
    getcwd(temp,256);
    string dest=string(temp);
    dest+="/";
    dest+=file;
    char cdest[200];
    strcpy(cdest, dest.c_str());
    //stat(cdest ,&obj);
    if(stat(cdest ,&obj) < 0)
    {
        cout<<"Error Criteria Approached"<<endl;
    }
    if(S_ISDIR(obj.st_mode) !=0) 
      {
          cout<<"d";
      }   
    else
      {
          cout<<"-";
      }
    if((obj.st_mode & S_IRUSR) != 0) 
    {
        cout<<"r";
    }
    else
    {
        cout<<"-";
    }
    if((obj.st_mode & S_IWUSR) != 0)
     {
         cout<<"w";
     }
     else
     {
         cout<<"-";
     }
    if((obj.st_mode & S_IXUSR) != 0) 
    {
        cout<<"x";
    }
    else
    {
        cout<<"-";
    }
    if((obj.st_mode & S_IRGRP) != 0) 
    {
        cout<<"r";
    }  
    else
    {
        cout<<"-";
    }
    if((obj.st_mode & S_IWGRP) != 0) 
    {
        cout<<"w";
    }
    else
    {
        cout<<"-";
    }
    if((obj.st_mode & S_IXGRP) != 0) 
    {
        cout<<"x";
    }
    else
    {
        cout<<"-";
    }
    if((obj.st_mode & S_IROTH) != 0) 
    {
        cout<<"r";
    }
    else
    {
        cout<<"-";
    }
    if((obj.st_mode & S_IWOTH) != 0) 
    {
        cout<<"w";
    }
    else
    {
        cout<<"-";
    }
    if((obj.st_mode & S_IXOTH) != 0){ 
    
        cout<<"x";
    }
    else{
       cout<<"-";
    }
    tot=obj.st_size;
    //int z=3;
    int count=0;
    while(tot>1024&&count<3){
        
        tot = tot/1024.0;
        count++;       
    }
    if(count==1){
        printf("\t%10.4f KB",tot);}
    else if(count==2){
        printf("\t%10.4f MB",tot);
    } 
    else if(count==3){
        printf("\t%10.4f GB",tot);

    }  
    else if(count==0){
        printf("\t%10.4f B",tot);

    }    
    id1 = geteuid();
    pw = getpwuid(id1);
    if(pw)
    {
        printf("\t%s",pw->pw_name);
        //cout<<std::left<<std::setw(20) <<pw->pw_name<<endl;
        
    }
    id2 = obj.st_gid;
    g = getgrgid(id2);
    //cout<<std::left<<std::setw(20) <<g->gr_name<<endl;

    printf("\t%s",g->gr_name);
    stringstream ss;
    ss << ctime(&obj.st_mtime);
    string ts = ss.str();
    string mod_time="";
    int i=0,len=ts.length();
    while(i<len-1){
        mod_time+=ts[i];
        i++;
    }
    //cout<<std::left<<std::setw(20) <<mod_time<<endl;
    //cout<<std::left<<std::setw(20) <<file<<endl;

    cout<<"\t"<<mod_time;
    cout<<"\t"<<file.substr(0,15);
    printf("\r\n");
}
void display(int start,int endd){
    //cout<<"\033c";//clear screen
    while(start < endd){
        printfiles(DFlist[indexx-1][start]);
        start++;
    }
}
    

       