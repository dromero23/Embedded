
int counter = 0; 
int main(){
  int *p_int;
  p_int = &counter;
  while (  *p_int < 21) {
    ++(*p_int);
  }
  p_int = (int*) 0x20000002u; //must type cast the address to set it to the pointer (it's tne next address after counter in the Tiva C)
   *p_int = 0xDEADBEEF; //will overwite the value in the counter varilae
  return 0;
}