int a[10];
int cal(int t1, int t2, int t3, int t4, int t5, int t6, int t7, int t8, int t9, int t10) {
    t10 = 1;
    return t9 + t10;
}
int cal2(int t1, int t2, int t3, int t4, int t5, int t6, int t7, int t8, int t9, int t10, int t11) {
    return t9 + t10 + t11;
}
int main(){
    putint(cal(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
    // putint(cal2(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[9]));
    return 0;
}