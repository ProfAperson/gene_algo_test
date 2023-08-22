
void set_bit(unsigned int val, unsigned int pos){
	return (val ^ pos);
}

unsigned int get_bit(unsigned int val, unsigned int pos){
	return ((val & pos) != 0 ) ? 1 : 0;
}