/* minconf.hh */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#ifndef MINCONF_HH
#define MINCONF_HH

#include "view.hh"

struct minimal_config { /* this goes to bitstream */

  unsigned char r, c;  // SAI row,column subscript
  unsigned char mmode; /* view merging mode, 0=LS, 1=geometric weight, 2=median*/
  unsigned char level;
  unsigned short encoding_flags;

};

minimal_config makeMinimalConfig(view *view0);
void setup_form_minimal_config(minimal_config *mconf, view *view0);

#endif
