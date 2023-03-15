int max(int b,int c){
    if(b>c) return b;
    else return c;
}
int fi(int n){
    if(n==1){
        return 1;
    }
    if(n==2){
        return 1;
    }
    return fi(n-1)+fi(n-2);
}   
int main(){
    int a[5],i,j;
    a[0] = 2;
    a[1] = 3;
    i = 0;
    j = 1;
    if(a[i]<a[j]){
        write(a[i]);
    }
    write(a[j]);
    for(i = 0;i<5;++i){
        j = read();
        write(fi(j));
    }
    return 0;
}