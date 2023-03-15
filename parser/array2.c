int main(){
    int a[3];
    int b[2];
    int c[3][3];
    b[0] = 2;
    a[b[0]] = 1;
    c[1][1] = b[0];
    c[0][0] = a[2];
    c[0][1] = c[0][0];
    write(a[2]);
    write(c[0][0]);
    write(c[0][1]);
    ++c[1][1];
    write(c[1][1]);
    return 0;
}