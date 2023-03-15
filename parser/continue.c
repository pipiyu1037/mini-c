int a;
int main(){
    int b = 4;
    while(b>0){
        a=read();
        if(a>2) continue;
        --b;
        write(b);
    }
}