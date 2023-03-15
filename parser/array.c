int main()
{
    int a[6],i,j,temp;
    for(i=0;i<6;++i)
        a[i]=read();
    for(i=1;i<5;++i)
    {
        for(j=0;j<6-i;++j)
            if (a[j]>a[j+1])
            {
                temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
    }
    for(i=0;i<6;++i){
        write(a[i]);
    }
    return 1;
}