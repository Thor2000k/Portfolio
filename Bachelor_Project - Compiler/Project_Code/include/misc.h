#ifndef __H_MISC__
#define __H_MISC__

// performs casting for simplicity  
#define GET_FLAG(F, E) get_flag((F), (int)(E))

// performs casting for simplicity
#define SET_FLAG(F, E) set_flag(&(F), (int)(E))

/* get the next prime number >= lower_bound. */
int prime_next(int lower_bound);

/* returns 1 if num is a prime number, otherwise 0. */
int prime_check(int num);

/* get the bit at pos in the bit array flags */
int get_flag(int flags, int pos);

/* set the bit at pos in the bit array flags */
void set_flag(int *flags, int pos);

#endif
