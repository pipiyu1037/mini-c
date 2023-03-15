int a;
int main(){
    int b = 6;
    while(b>0){
        a=read();
        if(a<2) break;
        --b;
        write(b);
    }
}